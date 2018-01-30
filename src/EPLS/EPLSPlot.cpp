#include "EPLSPlot.h"
#include "scientific.h"

#ifdef DEBUG
#include <QDebug>
#endif

void EPLSPlot::PredictedVSExperimentalAndPrediction(ScatterPlot2D **plot2D)
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
      (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), projectname + modelname + " - PLS Recalc VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot2D::SCORES);
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

void EPLSPlot::RecalcVSExperimental(ScatterPlot2D **plot2D)
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
  (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated"), projectname + modelname + " - PLS Recalculated VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot2D::SCORES);
  (*plot2D)->BuildDiagonal();
  (*plot2D)->setAxisNameExtensions(varname);
  (*plot2D)->setPID(pid);
  DelMatrix(&model_recalc_y);
  DelMatrix(&model_exp_y);
}

void EPLSPlot::RecalcResidualsVSExperimental(ScatterPlot2D** plot2D)
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
    (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated Residuals"), projectname + modelname + " - PLS Experimental VS Recalculated Residuals Y Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot2D::SCORES);
    DelMatrix(&recalc_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
  }
}


void EPLSPlot::PredictedVSExperimental(ScatterPlot2D **plot2D)
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
    (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), projectname + modelname + "- PLS Predicted VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot2D::SCORES);
    (*plot2D)->setAxisNameExtensions(varname);
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setPID(pid);
    DelMatrix(&model_exp_y);
    DelMatrix(&model_pred_y);
  }
}

void EPLSPlot::PredictedResidualsVSExperimental(ScatterPlot2D** plot2D)
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
    (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted Residuals"), projectname + modelname + " - PLS Experimental VS Predicted Residuals Y Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot2D::SCORES);
    DelMatrix(&pred_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
  }
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

    QStringList columntitle;
    columntitle << "Latent Variables" << QString("R2 %1").arg(yname) << QString("Q2 %1").arg(yname);

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
    plots.append(new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - R2 Q2 Plot %3").arg(projectname).arg(modelname).arg(yname), columntitle, "Latent Variables", "R2 / Q2"));
    plots.last()->setLabelDetail(true);
    plots.last()->setXminXmanXTicks(0, nlv, nlv);
    plots.last()->setYminYmanYTicks(0, 1.2, 10);
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

    QStringList columntitle;
    columntitle << "Latent Variables" << QString("R^2 Y %1").arg(yname);

    if(getq2 == true){
      columntitle << QString("Q^2 Y %1").arg(yname);
      curvenames << QString("Q^2 Y %1").arg(yname);
    }

    columntitle << QString("R^2 Predicted Y %1").arg(yname);
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


    plots.append(new SimpleLine2DPlot(m, curvenames, QString("%1 - %2 - R2 Q2 Plot Y %3").arg(projectname).arg(modelname).arg(yname), columntitle, "Latent Variables", yaxisname));
    plots.last()->setLabelDetail(true);
  }
  DelMatrix(&m);
  return plots;
}

/*QList<ScatterPlot2D*> EPLSPlot::YScramblingPlot()
{
  QList<ScatterPlot2D*> plots2D;
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
    plots2D.append(new ScatterPlot2D(mxlst, objnamelst, "Correlation with Y Real Vector", "R2/Q2", QString("PLS Y Scrambling Plot")));
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
