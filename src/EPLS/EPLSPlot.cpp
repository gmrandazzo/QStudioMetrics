#include "EPLSPlot.h"
#include "scientific.h"

#ifdef DEBUG
#include <QDebug>
#endif

void EPLSPlot::PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getEPLSModel(mid)->getYVarName();

  if(nlv > projects->value(pid)->getEPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getEPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getEPLSModel(mid)->getDataHash());
  if(did > -1 && predid > -1){
    int pred_id = projects->value(pid)->getMatrixID(projects->value(pid)->getEPLSModel(mid)->getEPLSPrediction(predid)->getDataHash()); // used to get real y when find real y
    if(pred_id > -1){
      QStringList model_objsel = projects->value(pid)->getEPLSModel(mid)->getObjName();
      QStringList yvarsel = projects->value(pid)->getEPLSModel(mid)->getYVarName();
      uint nobjects = model_objsel.size();
      uint nvars = yvarsel.size();
      matrix *model_predicted_y;
      NewMatrix(&model_predicted_y, nobjects, nvars);
      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for(uint i = 0; i < nobjects; i++){
        uint ny = nvars;
        for(uint j = 0; j < ny; j++){
          setMatrixValue(model_predicted_y, i, j, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->y_predicted, i, j+(ny*nlv)-ny));
        }
      }

      matrix *model_exp_y;
      NewMatrix(&model_exp_y, nobjects, nvars);
      int ii = 0;
      for(int i = 0; i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++){
        if(model_objsel.contains(projects->value(pid)->getMatrix(did)->getObjName()[i]) == true){
          int jy= 0;
          for(int j = 1; j < projects->value(pid)->getMatrix(did)->getVarName().size(); j++){
            if(yvarsel.contains(projects->value(pid)->getMatrix(did)->getVarName()[j]) == true){
              setMatrixValue(model_exp_y, ii, jy, getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(), i, j-1));
              jy++;
            }
            else{
              continue;
            }
          }
          ii++;
        }
        else{
          continue;
        }
      }

      // Get predictions to plot...
      QStringList pred_objsel = projects->value(pid)->getEPLSModel(mid)->getEPLSPrediction(predid)->getObjName();
      uint pred_nobjects = pred_objsel.size();
      matrix *pred_predicted_y;
      NewMatrix(&pred_predicted_y, pred_nobjects, nvars);

      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for(uint i = 0; i < pred_nobjects; i++){
        uint ny = nvars;
        for(uint j = 0; j < ny; j++){
          setMatrixValue(pred_predicted_y, i, j, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->getEPLSPrediction(predid)->py, i, j+(ny*nlv)-ny));
        }
      }


      /*bug... not did ma HASH!!*/
      matrix *pred_exp_y;
      NewMatrix(&pred_exp_y, pred_nobjects, nvars);
      ii = 0;
      for(int i = 0; i < projects->value(pid)->getMatrix(pred_id)->getObjName().size(); i++){
        if(pred_objsel.contains(projects->value(pid)->getMatrix(pred_id)->getObjName()[i]) == true){
          int jy= 0;
          for(int j = 1; j < projects->value(pid)->getMatrix(pred_id)->getVarName().size(); j++){
            if(yvarsel.contains(projects->value(pid)->getMatrix(pred_id)->getVarName()[j]) == true){
              setMatrixValue(pred_exp_y, ii, jy, getMatrixValue(projects->value(pid)->getMatrix(pred_id)->Matrix(), i, j-1));
              jy++;
            }
            else{
              continue;
            }
          }
          ii++;
        }
        else{
          continue;
        }
      }

      QList<matrix*> mx, my;
      mx.append(model_exp_y);
      mx.append(pred_exp_y);
      my.append(model_predicted_y);
      my.append(pred_predicted_y);
      QList<QStringList> objname;
      objname.append(projects->value(pid)->getEPLSModel(mid)->getObjName());
      objname.append(projects->value(pid)->getEPLSModel(mid)->getEPLSPrediction(predid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
      xhash.append(projects->value(pid)->getEPLSModel(mid)->getEPLSPrediction(predid)->getDataHash());
      yhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getEPLSModel(mid)->getEPLSPrediction(predid)->getDataHash());
      (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), projectname + modelname + " - PLS Recalc VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
      (*plot2D)->BuildDiagonal();
      (*plot2D)->setAxisNameExtensions(varname);
      (*plot2D)->setPID(pid);
      DelMatrix(&model_exp_y);
      DelMatrix(&model_predicted_y);
      DelMatrix(&pred_exp_y);
      DelMatrix(&pred_predicted_y);
    }
  }
}

void EPLSPlot::ClassPredictedVSExperimental(QList<QStringList> *cellnames, QList<QList<QPixmap>> *images, QList<QList<QColor>> *colors)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();

  if(nlv > projects->value(pid)->getEPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getEPLSModel(mid)->getNPC();
  }

  QStringList objsel = projects->value(pid)->getEPLSModel(mid)->getObjName();
  LABELS classes = projects->value(pid)->getEPLSModel(mid)->getClasses();

  uint nclass = classes.size();
  if(nclass == 2){
    nclass = 1;
  }

  matrix *mx;

  for(int j = 0; j < nclass; j++){
    QStringList row_name;
    QList<QPixmap> row_pixmap;
    QList<QColor> row_colors;

    NewMatrix(&mx, projects->value(pid)->getEPLSModel(mid)->y_predicted->row, 3);
    for(int i = 0; i < projects->value(pid)->getEPLSModel(mid)->y_predicted->row; i++){
      mx->data[i][0] = projects->value(pid)->getEPLSModel(mid)->y_predicted->data[i][j+(nclass*nlv)-nclass];
      if(classes[j].objects.contains(objsel[i]) == true){
        // True
        mx->data[i][1] = 1.f;
      }
      else{
        //False
        mx->data[i][1] = 0.f;
      }
      mx->data[i][2] = i; // objec row id
    }
    // Sort by the 1 probabilities to the 0 problabilities
    MatrixReverseSort(mx, 0);
    for(int i = 0; i < (int)mx->row; i++){
      int objid = (int)mx->data[i][2];
      QString name = objsel[objid]+ "\n";
      if((int)mx->data[i][1] == 1){
        name += QString("Is %1\n").arg(classes[j].name);
        row_colors << Qt::green;
      }
      else{
        name += QString("Not %1\n").arg(classes[j].name);
        row_colors << Qt::red;
      }
      name += QString("Score: %1").arg(QString::number(mx->data[i][0], 'g', 3));
      row_name << name;

      int pixid = -1;
      for(int k = 0; k < projects->value(pid)->getImages().size(); k++){
        if(objsel[objid].compare(projects->value(pid)->getImages()[k].name) == 0){
          pixid = k;
          break;
        }
        else{
          continue;
        }
      }

      if(pixid > -1)
        row_pixmap << projects->value(pid)->getImages()[pixid].image;
      else
        row_pixmap << QPixmap();
    }
    DelMatrix(&mx);
    (*cellnames) << row_name;
    (*images) << row_pixmap;
    (*colors) << row_colors;
  }
}

void EPLSPlot::RecalcVSExperimental(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getEPLSModel(mid)->getYVarName();

  if(nlv > projects->value(pid)->getEPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getEPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getEPLSModel(mid)->getDataHash());

  QStringList objsel = projects->value(pid)->getEPLSModel(mid)->getObjName();
  QStringList yvarsel = projects->value(pid)->getEPLSModel(mid)->getYVarName();
  uint nobjects = objsel.size();
  uint nvars = yvarsel.size();
  matrix *model_recalc_y;
  NewMatrix(&model_recalc_y, nobjects, nvars);

  /*
   Prediction for example are organized as this example
  y = 2
  number_of_pc = 4

      pc1    pc2     pc3       pc4
     |  |   |   |   |   |    |    |
     y1 y2  y1  y2  y1  y2   y1   y2
 pos 0 1    2   3   4   5    6    7
  */

  for(uint i = 0; i < nobjects; i++){
    uint ny = nvars;
    for(uint j = 0; j < ny; j++){
      setMatrixValue(model_recalc_y, i, j, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->y_recalculated, i, j+(ny*nlv)-ny));
    }
  }

  /*bug... not did ma HASH!!*/
  matrix *model_exp_y;
  NewMatrix(&model_exp_y, nobjects, nvars);
  int ii = 0;
  for(int i = 0; i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++){
    if(objsel.contains(projects->value(pid)->getMatrix(did)->getObjName()[i]) == true){
      int jy= 0;
      for(int j = 1; j < projects->value(pid)->getMatrix(did)->getVarName().size(); j++){
        if(yvarsel.contains(projects->value(pid)->getMatrix(did)->getVarName()[j]) == true){
          setMatrixValue(model_exp_y, ii, jy, getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(), i, j-1));
          jy++;
        }
        else{
          continue;
        }
      }
      ii++;
    }
    else{
      continue;
    }
  }

  QList<matrix*> mx, my;
  mx.append(model_exp_y);
  my.append(model_recalc_y);
  QList<QStringList> objname;
  objname.append( projects->value(pid)->getEPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated"), projectname + modelname + " - PLS Recalculated VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
  (*plot2D)->BuildDiagonal();
  (*plot2D)->setAxisNameExtensions(varname);
  (*plot2D)->setPID(pid);
  DelMatrix(&model_recalc_y);
  DelMatrix(&model_exp_y);
}

void EPLSPlot::RecalcResidualsVSExperimental(ScatterPlot** plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getEPLSModel(mid)->getYVarName();

  if(nlv > projects->value(pid)->getEPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getEPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getEPLSModel(mid)->getDataHash());

  if(did > -1){
    QStringList objsel = projects->value(pid)->getEPLSModel(mid)->getObjName();
    QStringList yvarsel = projects->value(pid)->getEPLSModel(mid)->getYVarName();
    uint nobjects = objsel.size();
    uint nvars = yvarsel.size();
    matrix *recalc_res;
    NewMatrix(&recalc_res, nobjects, nvars);

    /*
    Prediction for example are organized as this example
    y = 2
    number_of_pc = 4

        pc1    pc2     pc3       pc4
      |  |   |   |   |   |    |    |
      y1 y2  y1  y2  y1  y2   y1   y2
  pos 0 1    2   3   4   5    6    7
    */

    for(uint i = 0; i < nobjects; i++){
      uint ny = nvars;
      for(uint j = 0; j < ny; j++){
        setMatrixValue(recalc_res, i, j, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->y_recalculated_residuals, i, j+(ny*nlv)-ny));
      }
    }

    /*bug... not did ma HASH!!*/
    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++){
      if(objsel.contains(projects->value(pid)->getMatrix(did)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getMatrix(did)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getMatrix(did)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(), i, j-1));
            jy++;
          }
          else{
            continue;
          }
        }
        ii++;
      }
      else{
        continue;
      }
    }

    QList<matrix*> mx, my;
    mx.append(model_exp_y);
    my.append(recalc_res);
    QList<QStringList> objname;
    objname.append( projects->value(pid)->getEPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
    yhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated Residuals"), projectname + modelname + " - PLS Experimental VS Recalculated Residuals Y Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
    DelMatrix(&recalc_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
  }
}

void EPLSPlot::ClassRecalcVSExperimental(QList<QStringList> *cellnames, QList<QList<QPixmap>> *images, QList<QList<QColor>> *colors)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();

  if(nlv > projects->value(pid)->getEPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getEPLSModel(mid)->getNPC();
  }

  QStringList objsel = projects->value(pid)->getEPLSModel(mid)->getObjName();
  LABELS classes = projects->value(pid)->getEPLSModel(mid)->getClasses();

  uint nclass = classes.size();
  if(nclass == 2){
    nclass = 1;
  }

  matrix *mx;

  for(int j = 0; j < nclass; j++){
    QStringList row_name;
    QList<QPixmap> row_pixmap;
    QList<QColor> row_colors;

    NewMatrix(&mx, projects->value(pid)->getEPLSModel(mid)->y_recalculated->row, 3);
    for(int i = 0; i < projects->value(pid)->getEPLSModel(mid)->y_recalculated->row; i++){
      mx->data[i][0] = projects->value(pid)->getEPLSModel(mid)->y_recalculated->data[i][j+(nclass*nlv)-nclass];
      if(classes[j].objects.contains(objsel[i]) == true){
        // True
        mx->data[i][1] = 1.f;
      }
      else{
        //False
        mx->data[i][1] = 0.f;
      }
      mx->data[i][2] = i; // objec row id
    }
    // Sort by the 1 probabilities to the 0 problabilities
    MatrixReverseSort(mx, 0);
    for(int i = 0; i < (int)mx->row; i++){
      int objid = (int)mx->data[i][2];
      QString name = objsel[objid]+ "\n";
      if((int)mx->data[i][1] == 1){
        name += QString("Is %1\n").arg(classes[j].name);
        row_colors << Qt::green;
      }
      else{
        name += QString("Not %1\n").arg(classes[j].name);
        row_colors <<  Qt::red;
      }
      name += QString("Probability: %1").arg(QString::number(mx->data[i][0], 'g', 3));
      row_name << name;

      int pixid = -1;
      for(int k = 0; k < projects->value(pid)->getImages().size(); k++){
        if(objsel[objid].compare(projects->value(pid)->getImages()[k].name) == 0){
          pixid = k;
          break;
        }
        else{
          continue;
        }
      }

      if(pixid > -1)
        row_pixmap << projects->value(pid)->getImages()[pixid].image;
      else
        row_pixmap << QPixmap();

    }
    DelMatrix(&mx);
    (*cellnames) << row_name;
    (*images) << row_pixmap;
    (*colors) << row_colors;
  }
}

void EPLSPlot::PredictedVSExperimental(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getEPLSModel(mid)->getYVarName();

  if(nlv > projects->value(pid)->getEPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getEPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getEPLSModel(mid)->getDataHash());

  if(did > -1){
    QStringList objsel = projects->value(pid)->getEPLSModel(mid)->getObjName();
    QStringList yvarsel = projects->value(pid)->getEPLSModel(mid)->getYVarName();
    uint nobjects = objsel.size();
    uint nvars = yvarsel.size();
    matrix *model_pred_y;
    NewMatrix(&model_pred_y, nobjects, nvars);

    /*
    Prediction for example are organized as this example
    y = 2
    number_of_pc = 4

        pc1    pc2     pc3       pc4
      |  |   |   |   |   |    |    |
      y1 y2  y1  y2  y1  y2   y1   y2
  pos 0 1    2   3   4   5    6    7
    */

    for(uint i = 0; i < nobjects; i++){
      uint ny = nvars;
      for(uint j = 0; j < ny; j++){
        setMatrixValue(model_pred_y, i, j, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->y_predicted, i, j+(ny*nlv)-ny));
      }
    }

    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++){
      if(objsel.contains(projects->value(pid)->getMatrix(did)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getMatrix(did)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getMatrix(did)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(), i, j-1));
            jy++;
          }
          else{
            continue;
          }
        }
        ii++;
      }
      else{
        continue;
      }
    }

    QList<matrix*> mx, my;
    mx.append(model_exp_y);
    my.append(model_pred_y);
    QList<QStringList> objname;
    objname.append( projects->value(pid)->getEPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
    yhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), projectname + modelname + "- PLS Predicted VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
    (*plot2D)->setAxisNameExtensions(varname);
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setPID(pid);
    DelMatrix(&model_exp_y);
    DelMatrix(&model_pred_y);
  }
}

void EPLSPlot::PredictedResidualsVSExperimental(ScatterPlot** plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getEPLSModel(mid)->getYVarName();

  if(nlv > projects->value(pid)->getEPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getEPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getEPLSModel(mid)->getDataHash());

  if(did > -1){
    QStringList objsel = projects->value(pid)->getEPLSModel(mid)->getObjName();
    QStringList yvarsel = projects->value(pid)->getEPLSModel(mid)->getYVarName();
    uint nobjects = objsel.size();
    uint nvars = yvarsel.size();
    matrix *pred_res;
    NewMatrix(&pred_res, nobjects, nvars);

    /*
    Prediction for example are organized as this example
    y = 2
    number_of_pc = 4

        pc1    pc2     pc3       pc4
      |  |   |   |   |   |    |    |
      y1 y2  y1  y2  y1  y2   y1   y2
  pos 0 1    2   3   4   5    6    7
    */

    for(uint i = 0; i < nobjects; i++){
      uint ny = nvars;
      for(uint j = 0; j < ny; j++){
        setMatrixValue(pred_res, i, j, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->y_predicted_residuals, i, j+(ny*nlv)-ny));
      }
    }

    /*bug... not did ma HASH!!*/
    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++){
      if(objsel.contains(projects->value(pid)->getMatrix(did)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getMatrix(did)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getMatrix(did)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(), i, j-1));
            jy++;
          }
          else{
            continue;
          }
        }
        ii++;
      }
      else{
        continue;
      }
    }

    QList<matrix*> mx, my;
    mx.append(model_exp_y);
    my.append(pred_res);
    QList<QStringList> objname;
    objname.append( projects->value(pid)->getEPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
    yhash.append(projects->value(pid)->getEPLSModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted Residuals"), projectname + modelname + " - PLS Experimental VS Predicted Residuals Y Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
    DelMatrix(&pred_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
  }
}

QList< SimpleLine2DPlot* > EPLSPlot::ROCAUCs()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  uint nlv = projects->value(pid)->getEPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
  uint yval = projects->value(pid)->getEPLSModel(mid)->roc_auc_recalculated->col;
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, nlv, 3);
  curvenames << "AUC Recalculated" << "AUC Predicted";

  // set the X assis that is the principal component
  for(uint i = 0; i < nlv; i++){
    setMatrixValue(m, i, 0, i);
  }

  // Set the r2 values into the matrix and set the q2 values into the matrix
  uint l = 0;
  for(uint j = 0; j < yval; j++){
    QString yname = projects->value(pid)->getEPLSModel(mid)->getClasses()[j].name;

    setMatrixValue(m, 0, 1, 0); // AUC Recalc in 0 pc is 0
    setMatrixValue(m, 0, 2, 0); // AUC Predicted in 0 pc is 0

    for(uint i = 0; i < nlv-1; i++){
      setMatrixValue(m, i+1, 1, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->roc_auc_recalculated, i, l));
      setMatrixValue(m, i+1, 2, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->roc_auc_predicted, i, l));
    }
    l++;

    plots.append(new SimpleLine2DPlot(m,
                                      curvenames,
                                      QString(" %1 - %2 - ROC AUC Recalculated/Predicted Plot %3").arg(projectname).arg(modelname).arg(yname),
                                      "Latent Variables", "ROC AUC Recalculated / AUC Predicted"));
  }
  DelMatrix(&m);
  return plots;
}

QList< SimpleLine2DPlot* > EPLSPlot::ROCCurves()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  uint nlv = projects->value(pid)->getEPLSModel(mid)->getNPC(); // == roc_recalculated->order;
  uint yval = projects->value(pid)->getEPLSModel(mid)->roc_recalculated->m[0]->col/2.;

  QStringList curvenames;
  curvenames << "ROC Recalculated" << "ROC Predicted";

  tensor *roc_recalculated = projects->value(pid)->getEPLSModel(mid)->roc_recalculated;
  tensor *roc_predicted = projects->value(pid)->getEPLSModel(mid)->roc_predicted;


  for(uint i = 0; i < nlv; i++){
    for(uint j = 0; j < yval; j++){
      QString yname = projects->value(pid)->getEPLSModel(mid)->getClasses()[j].name;
      matrix *mrec, *mpred;
      NewMatrix(&mrec, roc_recalculated->m[i]->row, 2);
      NewMatrix(&mpred, roc_predicted->m[i]->row, 2);
      for(uint k = 0; k < roc_recalculated->m[i]->row; k++){
        mrec->data[k][0] =  roc_recalculated->m[i]->data[k][j*nlv];
        mrec->data[k][1] =  roc_recalculated->m[i]->data[k][j*nlv+1];
      }

      for(uint k = 0; k < roc_predicted->m[i]->row; k++){
        mpred->data[k][0] =  roc_predicted->m[i]->data[k][j*nlv];
        mpred->data[k][1] =  roc_predicted->m[i]->data[k][j*nlv+1];
      }
      QList <matrix*> mlst;
      mlst.append(mrec);
      mlst.append(mpred);
      plots.append(new SimpleLine2DPlot(mlst,
                                        curvenames,
                                        QString("N. LV: %1 %2 - %3 - ROC Curve Recalculated/Predicted Plot %4").arg(QString::number(i+1)).arg(projectname).arg(modelname).arg(yname),
                                        "False positive rate", "True positive rate"));
      plots.last()->setPlotTitle(QString("N. LV: %1; Class name: %2").arg(QString::number(i+1)).arg(yname));
      DelMatrix(&mrec);
      DelMatrix(&mpred);
      mlst.clear();
    }
  }
  return plots;
}

QList< SimpleLine2DPlot* > EPLSPlot::PrecisionRecallAveragePrecision()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  uint nlv = projects->value(pid)->getEPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
  uint yval = projects->value(pid)->getEPLSModel(mid)->precision_recall_ap_predicted->col;
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, nlv, 3);
  curvenames << "Precision-Recall AUC Recalculated" << "Precision-Recall AUC Predicted";

  // set the X assis that is the principal component
  for(uint i = 0; i < nlv; i++){
    setMatrixValue(m, i, 0, i);
  }

  // Set the r2 values into the matrix and set the q2 values into the matrix
  uint l = 0;
  for(uint j = 0; j < yval; j++){
    QString yname = projects->value(pid)->getEPLSModel(mid)->getClasses()[j].name;

    setMatrixValue(m, 0, 1, 0); // AUC Recalc in 0 pc is 0
    setMatrixValue(m, 0, 2, 0); // AUC Predicted in 0 pc is 0

    for(uint i = 0; i < nlv-1; i++){
      setMatrixValue(m, i+1, 1, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->precision_recall_ap_recalculated, i, l));
      setMatrixValue(m, i+1, 2, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->precision_recall_ap_predicted, i, l));
    }
    l++;

    plots.append(new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - Precision-Recall AUC Recalculated/Predicted Plot %3").arg(projectname).arg(modelname).arg(yname), "Latent Variables", "Prec./Rec. Recalculated and Predicted"));
  }
  DelMatrix(&m);
  return plots;
}

QList< SimpleLine2DPlot* > EPLSPlot::PrecisionRecallCurves()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  uint nlv = projects->value(pid)->getEPLSModel(mid)->getNPC(); // == roc_recalculated->order;
  uint yval = projects->value(pid)->getEPLSModel(mid)->precision_recall_recalculated->m[0]->col/2.;

  QStringList curvenames;
  curvenames << "Precision-Recall Recalculated" << "Precision-Recall Predicted";

  tensor *pr_recalculated = projects->value(pid)->getEPLSModel(mid)->precision_recall_recalculated;
  tensor *pr_predicted = projects->value(pid)->getEPLSModel(mid)->precision_recall_predicted;


  for(uint i = 0; i < nlv; i++){
    for(uint j = 0; j < yval; j++){
      QString yname = projects->value(pid)->getEPLSModel(mid)->getClasses()[j].name;
      matrix *mrec, *mpred;
      NewMatrix(&mrec, pr_recalculated->m[i]->row, 2);
      NewMatrix(&mpred, pr_predicted->m[i]->row, 2);
      for(uint k = 0; k < pr_recalculated->m[i]->row; k++){
        mrec->data[k][0] =  pr_recalculated->m[i]->data[k][j*nlv];
        mrec->data[k][1] =  pr_recalculated->m[i]->data[k][j*nlv+1];
      }

      for(uint k = 0; k < pr_predicted->m[i]->row; k++){
        mpred->data[k][0] =  pr_predicted->m[i]->data[k][j*nlv];
        mpred->data[k][1] =  pr_predicted->m[i]->data[k][j*nlv+1];
      }

      QList <matrix*> mlst;
      mlst.append(mrec);
      mlst.append(mpred);
      plots.append(new SimpleLine2DPlot(mlst,
                                        curvenames,
                                        QString("N. LV: %1 %2 - %3 - Precision-Recall Recalculated/Predicted Plot %4").arg(QString::number(i+1)).arg(projectname).arg(modelname).arg(yname),
                                        "Recall", "Precision"));
      plots.last()->setPlotTitle(QString("N. LV: %1; Class name: %2").arg(QString::number(i+1)).arg(yname));
      DelMatrix(&mrec);
      DelMatrix(&mpred);
      mlst.clear();
    }
  }
  return plots;
}

QList< SimpleLine2DPlot* > EPLSPlot::R2Q2()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  uint nlv = projects->value(pid)->getEPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
  uint yval = projects->value(pid)->getEPLSModel(mid)->r2->col;
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, nlv, 3);
  curvenames << "R2" << "Q2";

  // set the X assis that is the principal component
  for(uint i = 0; i < nlv; i++){
    setMatrixValue(m, i, 0, i);
  }

  // Set the r2 values into the matrix and set the q2 values into the matrix
  uint l = 0;
  for(uint j = 0; j < yval; j++){
    QString yname = projects->value(pid)->getEPLSModel(mid)->getYVarName()[j];
    #ifdef DEBUG
    qDebug() << "r2";
    PrintMatrix(projects->value(pid)->getEPLSModel(mid)->r2);
    qDebug() << "q2";
    PrintMatrix(projects->value(pid)->getEPLSModel(mid)->q2);
    #endif


    setMatrixValue(m, 0, 1, 0); // R^2 in 0 pc is 0
    setMatrixValue(m, 0, 2, 0); // Q^2 in 0 pc is 0

    for(uint i = 0; i < nlv-1; i++){
      setMatrixValue(m, i+1, 1, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->r2, i, l));
      double q2 = getMatrixValue(projects->value(pid)->getEPLSModel(mid)->q2, i, l);
      if(q2 < 0){
        setMatrixValue(m, i+1, 2, 0.f);
      }
      else{
        setMatrixValue(m, i+1, 2, q2);
      }
    }
    l++;


    #ifdef DEBUG
    qDebug() << "Final Matrix";
    PrintMatrix(m);
    #endif
    plots.append(new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - R2 Q2 Plot %3").arg(projectname).arg(modelname).arg(yname), "Latent Variables", "R2 / Q2"));
  }
  DelMatrix(&m);
  return plots;
}


QList< SimpleLine2DPlot* > EPLSPlot::R2R2Prediction()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  uint nlv = projects->value(pid)->getEPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
  uint yval = projects->value(pid)->getEPLSModel(mid)->r2->col;

  bool getq2 = false;
  matrix *m;

  int ncol = 3;

  if(projects->value(pid)->getEPLSModel(mid)->q2->row > 0){
    ncol++;
    getq2 = true;
  }

  NewMatrix(&m, nlv, ncol);

  // set the X assis that is the principal component
  for(uint i = 0; i < nlv; i++){
    setMatrixValue(m, i, 0, i);
  }

  // Set the r2, r2prediction, q2, e q2consistency values into the matrix
  uint l = 0;
  for(uint j = 0; j < yval; j++){

    QString yname = projects->value(pid)->getEPLSModel(mid)->getYVarName()[j];
    #ifdef DEBUG
    qDebug() << "r2";
    PrintMatrix(projects->value(pid)->getEPLSModel(mid)->r2);
    qDebug() << "q2";
    PrintMatrix(projects->value(pid)->getEPLSModel(mid)->q2);
    #endif

    QStringList curvenames;
    curvenames << "R2";


    if(getq2 == true){
      curvenames << QString("Q^2 Y %1").arg(yname);
    }

    curvenames << QString("R^2 Predicted Y %1").arg(yname);

    int k = 1;
    setMatrixValue(m, 0, k, 0); // R^2 in 0 pc is 0
    k++;

    if(getq2 == true){
      setMatrixValue(m, 0, k, 0); // Q^2 in 0 pc is 0
      k++;
    }

    setMatrixValue(m, 0, k, 0);  // R2 Predicted

    for(uint i = 0; i < nlv-1; i++){
      k = 1;
      setMatrixValue(m, i+1, k, getMatrixValue(projects->value(pid)->getEPLSModel(mid)->r2, i, l));
      k++;

      if(getq2 == true){
        double q2 = getMatrixValue(projects->value(pid)->getEPLSModel(mid)->q2, i, l);
        if(q2 < 0){
          setMatrixValue(m, i+1, k, 0.f);
        }
        else{
          setMatrixValue(m, i+1, k, q2);
        }
        k++;
      }

      double r2pred = getMatrixValue(projects->value(pid)->getEPLSModel(mid)->getEPLSPrediction(predid)->r2, i, l);
      if(r2pred < 0){
        setMatrixValue(m, i+1, k, 0.f);
      }
      else{
        setMatrixValue(m, i+1, k, r2pred);
      }
    }
    l++;


    #ifdef DEBUG
    qDebug() << "Final Matrix";
    PrintMatrix(m);
    #endif
    QString yaxisname = "R^2";

    if(getq2 == true)
      yaxisname += " / Q2";

    yaxisname += " / R2 Predicted";


    plots.append(new SimpleLine2DPlot(m, curvenames, QString("%1 - %2 - R2 Q2 Plot Y %3").arg(projectname).arg(modelname).arg(yname), "Latent Variables", yaxisname));
  }
  DelMatrix(&m);
  return plots;
}

/*QList<ScatterPlot*> EPLSPlot::YScramblingPlot()
{
  QList<ScatterPlot*> plots2D;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getEPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(new matrix);
  size_t nobj = projects->value(pid)->getEPLSModel(mid)->Model()->yscrambling->row;
  size_t ndepvar = (projects->value(pid)->getEPLSModel(mid)->Model()->yscrambling->col-1)/2;
  NewMatrix(&mxlst.last(), nobj, 2); // R2
  mxlst.append(new matrix);
  NewMatrix(&mxlst.last(), nobj, 2); // Q2
  QList<QStringList> objnamelst;
  objnamelst.append(QStringList());
  objnamelst.append(QStringList());
  // set the X Constant
  for(size_t i = 0; i < nobj; i++){
    mxlst[0]->data[i][0] = projects->value(pid)->getEPLSModel(mid)->Model()->yscrambling->data[i][0];
    mxlst[1]->data[i][0] = projects->value(pid)->getEPLSModel(mid)->Model()->yscrambling->data[i][0];
    objnamelst[0].append("R2");
    objnamelst[1].append("Q2");
  }

  for(size_t j = 0; j < ndepvar; j++){
    for(size_t i = 0; i < nobj; i++){
      mxlst[0]->data[i][1] = projects->value(pid)->getEPLSModel(mid)->Model()->yscrambling->data[i][j+1];
      mxlst[1]->data[i][1] = projects->value(pid)->getEPLSModel(mid)->Model()->yscrambling->data[i][j+1+ndepvar];
    }
    plots2D.append(new ScatterPlot(mxlst, objnamelst, "Correlation with Y Real Vector", "R2/Q2", QString("PLS Y Scrambling Plot")));
    plots2D.last()->setPID(pid);
  }

  DelMatrix(&mxlst[1]);
  DelMatrix(&mxlst[0]);
  mxlst.clear();
  return plots2D;
}*/

EPLSPlot::EPLSPlot(PROJECTS* projects_)
{
  pid = mid = predid = -1;
  projects = projects_;
}
