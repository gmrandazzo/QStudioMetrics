#include "UPLSPlot.h"

void UPLSPlot::TU_Plot(ScatterPlot2D **plot2D)
{
  qDebug()<< "TU Plot: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  QList<matrix*> mx, my;
  mx.append(projects->value(pid)->getUPLSModel(mid)->Model()->xscores);
  my.append(projects->value(pid)->getUPLSModel(mid)->Model()->yscores);
  dvector *b = projects->value(pid)->getUPLSModel(mid)->Model()->b;
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
  (*plot2D) = new ScatterPlot2D(mx, my, b, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("T"), QString("U"), QString("%1 - %2 - UPLS T/U Plot").arg(projectname).arg(modelname), ScatterPlot2D::SCORES);
  (*plot2D)->setPID(pid);
}

void UPLSPlot::T_ScorePlot2D(ScatterPlot2D **plot2D)
{
  qDebug()<< "T Score Plot: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getUPLSModel(mid)->Model()->xscores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
  /*
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
  */
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, "T", "T",
  QString("%1 - %2 - UPLS T/T Score Plot").arg(projectname).arg(modelname));
  (*plot2D)->setPID(pid);
}

void UPLSPlot::P_LoadingsPlot2D(QList< ScatterPlot2D* > *plot2D)
{
  qDebug()<< "Loadings Plot: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  /* xloadings->order represent the number of pc computed.
  * xloadings->m[i]->row represent the number of descriptor that is equal to X_->m[i]->col
  * xloadings->m[i]->col represent the number of order present in the array X_ and is equal to X_->order
  */

  matrix *m;

  // the size is the number of variable x the number of components and we generate a plot for each layer and the number of layer is  m[i]->col
  uint row = projects->value(pid)->getUPLSModel(mid)->Model()->xloadings->m[0]->row;
  uint col = projects->value(pid)->getUPLSModel(mid)->Model()->xloadings->order;
  uint layer = projects->value(pid)->getUPLSModel(mid)->Model()->xloadings->m[0]->col;

  for(uint k = 0; k < layer; k++){
    NewMatrix(&m, row, col);

    //copy the data on matrix to plot
    for(uint i = 0; i < row; i++){
      for(uint j = 0; j < col; j++){
        setMatrixValue(m, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->xloadings, j, i, k));
      }
    }

    QList<matrix*> mxlst;
    mxlst.append(m);
    QList<QStringList> objnamelst;
    QStringList varname = projects->value(pid)->getUPLSModel(mid)->getXVarName();
    objnamelst.append(varname);
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    (*plot2D).append(new ScatterPlot2D(mxlst, objnamelst, "P", "P",
    QString("%1 - %2 -  UPLS P Loadings Plot Layer: %1").arg(projectname).arg(modelname).arg(QString::number(k+1))));
    (*plot2D).last()->setPID(pid);
    DelMatrix(&m);
  }
}

void UPLSPlot::WeightsPlot2D(QList< ScatterPlot2D* > *plot2D)
{
  qDebug()<< "Weigths Plot: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  /* xloadings->order represent the number of pc computed.
  * xloadings->m[i]->row represent the number of descriptor that is equal to X_->m[i]->col
  * xloadings->m[i]->col represent the number of order present in the array X_ and is equal to X_->order
  */

  matrix *m;

  // the size is the number of variable x the number of components and we generate a plot for each layer and the number of layer is  m[i]->col
  uint row = projects->value(pid)->getUPLSModel(mid)->Model()->xweights->m[0]->row;
  uint col = projects->value(pid)->getUPLSModel(mid)->Model()->xweights->order;
  uint layer = projects->value(pid)->getUPLSModel(mid)->Model()->xweights->m[0]->col;

  for(uint k = 0; k < layer; k++){
    NewMatrix(&m, row, col);

    //copy the data on matrix to plot
    for(uint i = 0; i < row; i++){
      for(uint j = 0; j < col; j++){
        setMatrixValue(m, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->xweights, j, i, k));
      }
    }

    QList<matrix*> mxlst;
    mxlst.append(m);
    QList<QStringList> objnamelst;
    QStringList varname = projects->value(pid)->getUPLSModel(mid)->getXVarName();
    objnamelst.append(varname);
    /*
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    */
    (*plot2D).append(new ScatterPlot2D(mxlst, objnamelst, "W", "W",
    QString("%1 - %2 -  UPLS W Weights Plot Layer: %3").arg(projectname).arg(modelname).arg(QString::number(k+1))));
    (*plot2D).last()->setPID(pid);
    DelMatrix(&m);
  }
}

void UPLSPlot::U_ScorePlot2D(ScatterPlot2D **plot2D)
{
  qDebug()<< "Plotting: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getUPLSModel(mid)->Model()->yscores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
  /*
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
  */
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, "U", "U",
  QString("%1 - %2 - UPLS U/U Score Plot").arg(projectname).arg(modelname));
  (*plot2D)->setPID(pid);
}

void UPLSPlot::Q_LoadingsPlot2D(QList< ScatterPlot2D* > *plot2D)
{
  qDebug()<< "Plotting UPLS QQ Loadings PLOT: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  /* xloadings->order represent the number of pc computed.
  * xloadings->m[i]->row represent the number of descriptor that is equal to X_->m[i]->col
  * xloadings->m[i]->col represent the number of order present in the array X_ and is equal to X_->order
  */

  matrix *m;

  // the size is the number of variable x the number of components and we generate a plot for each layer and the number of layer is  m[i]->col
  uint row = projects->value(pid)->getUPLSModel(mid)->Model()->yloadings->m[0]->row;
  uint col = projects->value(pid)->getUPLSModel(mid)->Model()->yloadings->order;
  uint layer = projects->value(pid)->getUPLSModel(mid)->Model()->yloadings->m[0]->col;

  for(uint k = 0; k < layer; k++){
    NewMatrix(&m, row, col);

    //copy the data on matrix to plot
    for(uint i = 0; i < row; i++){
      for(uint j = 0; j < col; j++){
        setMatrixValue(m, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->yloadings, j, i, k));
      }
    }

    QList<matrix*> mxlst;
    mxlst.append(m);
    QList<QStringList> objnamelst;
    objnamelst.append(projects->value(pid)->getUPLSModel(mid)->getYVarName());
    /*
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    */
    (*plot2D).append(new ScatterPlot2D(mxlst, objnamelst, "Q", "Q",
    QString("%1 - %2 -  UPLS Q Loadings Plot Layer: %1").arg(projectname).arg(modelname).arg(QString::number(k+1))));
    (*plot2D).last()->setPID(pid);
    DelMatrix(&m);
  }
}

void UPLSPlot::T_ScorePlotPrediction2D(ScatterPlot2D **plot2D)
{
  qDebug()<< "Plotting: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getUPLSModel(mid)->Model()->xscores);
  mxlst.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getXPredScores());
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
  objnamelst.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getObjName());
  /*
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
  */
  (*plot2D) =new ScatterPlot2D(mxlst, objnamelst, "T", "T",
  QString("%1 - %2 - UPLS T/T Score Plot Prediction").arg(projectname).arg(modelname));
  (*plot2D)->setPID(pid);
}

void UPLSPlot::RecalcVSExperimentalAndPrediction(QList< ScatterPlot2D* > *plot2D)
{
  qDebug()<< "Plotting UPLS Prediction VS Experimental and Predictions: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getUPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getUPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int ydid = projects->value(pid)->getArrayID(projects->value(pid)->getUPLSModel(mid)->getYHash());

  QStringList model_objsel = projects->value(pid)->getUPLSModel(mid)->getObjName();
  QStringList yvarsel = projects->value(pid)->getUPLSModel(mid)->getYVarName();
  uint nobjects = model_objsel.size();
  uint nvars = yvarsel.size();

  for(uint k = 0; k < projects->value(pid)->getArray(ydid)->Array()->order; k++){
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
      for(uint j = 0; j < nvars; j++){
        setMatrixValue(model_recalc_y, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->recalculated_y, k, i, j+(nvars*npc)-nvars));
      }
    }

    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getArray(ydid)->getObjName().size(); i++){
      if(model_objsel.contains(projects->value(pid)->getArray(ydid)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getArray(ydid)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getArray(ydid)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getArrayValue(projects->value(pid)->getArray(ydid)->Array(), k, i, j-1));
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
    QStringList pred_objsel = projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getObjName();
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
      for(uint j = 0; j < nvars; j++){
        setMatrixValue(pred_predicted_y, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getYDipVar(), k+(nvars*npc)-nvars, i, j));
      }
    }

    int pred_id = projects->value(pid)->getArrayID(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getYHash());
    /*bug... not did ma HASH!!*/
    matrix *pred_exp_y;
    NewMatrix(&pred_exp_y, pred_nobjects, nvars);
    ii = 0;
    for(int i = 0; i < projects->value(pid)->getArray(pred_id)->getObjName().size(); i++){
      if(pred_objsel.contains(projects->value(pid)->getArray(pred_id)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getArray(pred_id)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getArray(pred_id)->getVarName()[j]) == true){
            setMatrixValue(pred_exp_y, ii, jy, getArrayValue(projects->value(pid)->getArray(pred_id)->Array(), k, i, j-1));
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
    my.append(model_recalc_y);
    my.append(pred_predicted_y);
    QList<QStringList> objname;
    objname.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
    objname.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getObjName());

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getYHash());

    (*plot2D).append(new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(),  QString("Layer %1 Experimental Y").arg(QString::number(k+1)), QString("Layer %1 Recalculated and Predicted Y").arg(QString::number(k+1)), projectname + modelname + " - UPLS Recalculated and Predicted VS Experimental Plot", ScatterPlot2D::SCORES));
    (*plot2D).last()->BuildDiagonal();
    (*plot2D).last()->setPID(pid);
    DelMatrix(&model_exp_y);
    DelMatrix(&model_recalc_y);
    DelMatrix(&pred_exp_y);
    DelMatrix(&pred_predicted_y);
  }
}

void UPLSPlot::PredictedVSExperimentalAndPrediction(QList< ScatterPlot2D* > *plot2D)
{
  qDebug()<< "Plotting UPLS Prediction VS Experimental and Predictions: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getUPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getUPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int ydid = projects->value(pid)->getArrayID(projects->value(pid)->getUPLSModel(mid)->getYHash());

  QStringList model_objsel = projects->value(pid)->getUPLSModel(mid)->getObjName();
  QStringList yvarsel = projects->value(pid)->getUPLSModel(mid)->getYVarName();
  uint nobjects = model_objsel.size();
  uint nvars = yvarsel.size();

  for(uint k = 0; k < projects->value(pid)->getArray(ydid)->Array()->order; k++){
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
    qDebug() << QString("%1 %2").arg(nvars).arg(npc);
    for(uint i = 0; i < nobjects; i++){
      for(uint j = 0; j < nvars; j++){
        setMatrixValue(model_predicted_y, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->predicted_y, k, i, j+(nvars*npc)-nvars));
      }
    }

    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getArray(ydid)->getObjName().size(); i++){
      if(model_objsel.contains(projects->value(pid)->getArray(ydid)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getArray(ydid)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getArray(ydid)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getArrayValue(projects->value(pid)->getArray(ydid)->Array(), k, i, j-1));
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
    QStringList pred_objsel = projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getObjName();
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
      for(uint j = 0; j < nvars; j++){
        setMatrixValue(pred_predicted_y, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getYDipVar(), k+(nvars*npc)-nvars, i, j));
      }
    }

    int pred_id = projects->value(pid)->getArrayID(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getYHash());
    /*bug... not did ma HASH!!*/
    matrix *pred_exp_y;
    NewMatrix(&pred_exp_y, pred_nobjects, nvars);
    ii = 0;
    for(int i = 0; i < projects->value(pid)->getArray(pred_id)->getObjName().size(); i++){
      if(pred_objsel.contains(projects->value(pid)->getArray(pred_id)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getArray(pred_id)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getArray(pred_id)->getVarName()[j]) == true){
            setMatrixValue(pred_exp_y, ii, jy, getArrayValue(projects->value(pid)->getArray(pred_id)->Array(), k, i, j-1));
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

    qDebug() << "warnig to test!!";
    PrintMatrix(model_exp_y);
    PrintMatrix(pred_exp_y);
    PrintMatrix(model_predicted_y);
    PrintMatrix(pred_predicted_y);
    puts("XXXXXXXXXX");

    QList<matrix*> mx, my;
    mx.append(model_exp_y);
    mx.append(pred_exp_y);
    my.append(model_predicted_y);
    my.append(pred_predicted_y);
    QList<QStringList> objname;
    objname.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
    objname.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getYHash());
    (*plot2D).append(new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(),  QString("Layer %1 Experimental Y").arg(QString::number(k+1)), QString("Layer %1 Predicted Y").arg(QString::number(k+1)), projectname + modelname + " - UPLS Predicted VS Experimental Plot", ScatterPlot2D::SCORES));
    (*plot2D).last()->BuildDiagonal();
    (*plot2D).last()->setPID(pid);
    DelMatrix(&model_exp_y);
    DelMatrix(&model_predicted_y);
    DelMatrix(&pred_exp_y);
    DelMatrix(&pred_predicted_y);
  }
}

void UPLSPlot::RecalcVSExperimental(QList< ScatterPlot2D* > *plot2D)
{
  qDebug()<< "Plotting UPLS Recalculated VS Experimental: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getUPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getUPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int ydid = projects->value(pid)->getArrayID(projects->value(pid)->getUPLSModel(mid)->getYHash());

  QStringList objsel = projects->value(pid)->getUPLSModel(mid)->getObjName();
  QStringList yvarsel = projects->value(pid)->getUPLSModel(mid)->getYVarName();
  uint nobjects = objsel.size();
  uint nvars = yvarsel.size();


  for(uint k = 0; k < projects->value(pid)->getArray(ydid)->Array()->order; k++){
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
      for(uint j = 0; j < nvars; j++){
        setMatrixValue(model_recalc_y, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->recalculated_y, k, i, j+(nvars*npc)-nvars));
      }
    }

    /*bug... not did ma HASH!!*/
    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getArray(ydid)->getObjName().size(); i++){
      if(objsel.contains(projects->value(pid)->getArray(ydid)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getArray(ydid)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getArray(ydid)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getArrayValue(projects->value(pid)->getArray(ydid)->Array(), k, i, j-1));
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
    objname.append( projects->value(pid)->getUPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    (*plot2D).append(new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Layer %1 Experimental Y").arg(QString::number(k+1)), QString("Layer %1 Recalculated Y").arg(QString::number(k+1)), QString("- %1 - %2 - UPLS Recalculated VS Experimental Plot Layer %3 ").arg(projectname).arg(modelname).arg(QString::number(k+1)), ScatterPlot2D::SCORES));
    (*plot2D).last()->BuildDiagonal();
    (*plot2D).last()->setPID(pid);
    DelMatrix(&model_recalc_y);
    DelMatrix(&model_exp_y);
  }
}

void UPLSPlot::RecalcResidualsVSExperimental(QList< ScatterPlot2D* >* plot2D)
{
  qDebug()<< "Plotting UPLS Recalculated VS Experimental: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getUPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getUPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int ydid = projects->value(pid)->getArrayID(projects->value(pid)->getUPLSModel(mid)->getYHash());

  QStringList objsel = projects->value(pid)->getUPLSModel(mid)->getObjName();
  QStringList yvarsel = projects->value(pid)->getUPLSModel(mid)->getYVarName();
  uint nobjects = objsel.size();
  uint nvars = yvarsel.size();

  for(uint k = 0; k < projects->value(pid)->getArray(ydid)->Array()->order; k++){
    matrix *model_recalc_res;
    NewMatrix(&model_recalc_res, nobjects, nvars);

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
      for(uint j = 0; j < nvars; j++){
        setMatrixValue(model_recalc_res, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->recalc_residuals, k, i, j+(nvars*npc)-nvars));
      }
    }

    /*bug... not did ma HASH!!*/
    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getArray(ydid)->getObjName().size(); i++){
      if(objsel.contains(projects->value(pid)->getArray(ydid)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getArray(ydid)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getArray(ydid)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getArrayValue(projects->value(pid)->getArray(ydid)->Array(), k, i, j-1));
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
    my.append(model_recalc_res);
    QList<QStringList> objname;
    objname.append( projects->value(pid)->getUPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    (*plot2D).append(new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Layer %1 Experimental Y").arg(QString::number(k+1)), QString("Layer %1 Recalculated Y").arg(QString::number(k+1)), QString("- %1 - %2 - UPLS Recalculated Residuals VS Experimental Plot Layer %3 ").arg(projectname).arg(modelname).arg(QString::number(k+1)), ScatterPlot2D::SCORES));
    (*plot2D).last()->setPID(pid);
    DelMatrix(&model_recalc_res);
    DelMatrix(&model_exp_y);
  }
}

void UPLSPlot::PredictedVSExperimental(QList< ScatterPlot2D* > *plot2D)
{
  qDebug()<< "Plotting UPLS Recalculated VS Experimental: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getUPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getUPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int ydid = projects->value(pid)->getArrayID(projects->value(pid)->getUPLSModel(mid)->getYHash());

  QStringList objsel = projects->value(pid)->getUPLSModel(mid)->getObjName();
  QStringList yvarsel = projects->value(pid)->getUPLSModel(mid)->getYVarName();
  uint nobjects = objsel.size();
  uint nvars = yvarsel.size();


  for(uint k = 0; k < projects->value(pid)->getArray(ydid)->Array()->order; k++){
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
      for(uint j = 0; j < nvars; j++){
        setMatrixValue(model_pred_y, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->predicted_y, k, i, j+(nvars*npc)-nvars));
      }
    }

    /*bug... not did ma HASH!!*/
    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getArray(ydid)->getObjName().size(); i++){
      if(objsel.contains(projects->value(pid)->getArray(ydid)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getArray(ydid)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getArray(ydid)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getArrayValue(projects->value(pid)->getArray(ydid)->Array(), k, i, j-1));
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
    objname.append( projects->value(pid)->getUPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    (*plot2D).append(new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Layer %1 Experimental Y").arg(QString::number(k+1)), QString("Layer %1 Predicted Y").arg(QString::number(k+1)), QString("- %1 - %2 - UPLS Predicted VS Experimental Plot Layer %3 ").arg(projectname).arg(modelname).arg(QString::number(k+1)), ScatterPlot2D::SCORES));
    (*plot2D).last()->BuildDiagonal();
    (*plot2D).last()->setPID(pid);
    DelMatrix(&model_pred_y);
    DelMatrix(&model_exp_y);
  }
}

void UPLSPlot::PredictedResidualsVSExperimental(QList< ScatterPlot2D* >* plot2D)
{
  qDebug()<< "Plotting UPLS Recalculated VS Experimental: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getUPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getUPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int ydid = projects->value(pid)->getArrayID(projects->value(pid)->getUPLSModel(mid)->getYHash());

  QStringList objsel = projects->value(pid)->getUPLSModel(mid)->getObjName();
  QStringList yvarsel = projects->value(pid)->getUPLSModel(mid)->getYVarName();
  uint nobjects = objsel.size();
  uint nvars = yvarsel.size();

  for(uint k = 0; k < projects->value(pid)->getArray(ydid)->Array()->order; k++){
    matrix *model_pred_res;
    NewMatrix(&model_pred_res, nobjects, nvars);

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
      for(uint j = 0; j < nvars; j++){
        setMatrixValue(model_pred_res, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->pred_residuals, k, i, j+(nvars*npc)-nvars));
      }
    }

    /*bug... not did ma HASH!!*/
    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for(int i = 0; i < projects->value(pid)->getArray(ydid)->getObjName().size(); i++){
      if(objsel.contains(projects->value(pid)->getArray(ydid)->getObjName()[i]) == true){
        int jy= 0;
        for(int j = 1; j < projects->value(pid)->getArray(ydid)->getVarName().size(); j++){
          if(yvarsel.contains(projects->value(pid)->getArray(ydid)->getVarName()[j]) == true){
            setMatrixValue(model_exp_y, ii, jy, getArrayValue(projects->value(pid)->getArray(ydid)->Array(), k, i, j-1));
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
    my.append(model_pred_res);
    QList<QStringList> objname;
    objname.append( projects->value(pid)->getUPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
    yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
    (*plot2D).append(new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Layer %1 Experimental Y").arg(QString::number(k+1)), QString("Layer %1 Predicted Residuals Y").arg(QString::number(k+1)), QString("- %1 - %2 - UPLS Predicted Residuals VS Experimental Plot Layer %3 ").arg(projectname).arg(modelname).arg(QString::number(k+1)), ScatterPlot2D::SCORES));
    (*plot2D).last()->setPID(pid);
    DelMatrix(&model_pred_res);
    DelMatrix(&model_exp_y);
  }
}

void UPLSPlot::R2Q2(QList< SimpleLine2DPlot* > *plot2D)
{
  qDebug()<< "Plotting R2 Q2: " << projects->value(pid)->getProjectName();
  /*
  array *r2y_validation   && array *q2y   each order correspond to a principal component; each row correspond to an y dependent variable and each col to an order
  */
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  uint npc = projects->value(pid)->getUPLSModel(mid)->getNPC();
  uint yval = projects->value(pid)->getUPLSModel(mid)->Model()->r2y_model->m[0]->row; // each row is an y dependent value
  uint layers = projects->value(pid)->getUPLSModel(mid)->Model()->r2y_model->m[0]->col; // each col is a layer for the y
  matrix *m;

  QStringList curvenames;
  if(projects->value(pid)->getUPLSModel(mid)->Model()->q2y_yscrambling->order > 0){
    NewMatrix(&m, npc+1, 4);  // +1 because we start from 0
    curvenames << "R2" << "Q2" << "Y Scrambling";
  }
  else{
    NewMatrix(&m, npc+1, 3);  // +1 because we start from 0
    curvenames << "R2" << "Q2";
  }

  // set the X assis that is the principal component
  for(uint i = 0; i < npc+1; i++){
    setMatrixValue(m, i, 0, i);
  }

  // Set the r2 values into the matrix and set the q2 values into the matrix

  for(uint j = 0; j < yval; j++){ // per ogni y
    for(uint k = 0; k < layers; k++){ // per ogni layer

      #ifdef DEBUG
      qDebug() << "r2";
      PrintMatrix(projects->value(pid)->getUPLSModel(mid)->Model()->r2y_model->m[k]);
      qDebug() << "q2";
      PrintMatrix(projects->value(pid)->getUPLSModel(mid)->Model()->q2y->m[k]);
      #endif

      QStringList columntitle;
      if(projects->value(pid)->getUPLSModel(mid)->Model()->q2y_yscrambling->order > 0){
        columntitle << "Principal Components" << QString("R^2 Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1)) << QString("Q^2 Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1)) << QString("Q^2 Model Consistency Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));
      }
      else{
        columntitle << "Principal Components" << QString("R^2 Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1)) << QString("Q^2 Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));
      }

      setMatrixValue(m, 0, 1, 0); // R^2 in 0 pc is 0
      setMatrixValue(m, 0, 2, 0); // Q^2 in 0 pc is 0

      for(uint i = 0; i < npc; i++){
        setMatrixValue(m, i+1, 1, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->r2y_model, i, j, k));
        double q2 = getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->q2y, i, j, k);
        if(q2 < 0){
          setMatrixValue(m, i+1, 2, 0.f);
        }
        else{
          setMatrixValue(m, i+1, 2, q2);
        }
        if(projects->value(pid)->getUPLSModel(mid)->Model()->q2y_yscrambling->order > 0){
          setMatrixValue(m, i+1, 3, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->q2y_yscrambling, i, j, k));
        }
        else{
          continue;
        }
      }

      #ifdef DEBUG
      qDebug() << "Final Matrix";
      PrintMatrix(m);
      #endif

      if(projects->value(pid)->getUPLSModel(mid)->Model()->q2y_yscrambling->order > 0){
        (*plot2D).append(new SimpleLine2DPlot(m, curvenames, projectname + modelname + QString(" - R^2 Q^2 and Q^2 Model Consistency Plot Y %1  Layer %2").arg(j+1).arg(QString::number(k+1)), columntitle, "Principal Components", "R^2 / Q^2 / Q^2 Model Consistency"));
      }
      else{
        (*plot2D).append(new SimpleLine2DPlot(m, curvenames, projectname + modelname + QString(" - R^2 Q^2 and Q^2 Model Consistency Plot Y %1  Layer %2").arg(j+1).arg(QString::number(k+1)), columntitle, "Principal Components", "R^2 / Q^2 / Q^2 Model Consistency"));
      }
      (*plot2D).last()->setLabelDetail(true);
    }
  }
  DelMatrix(&m);
}

void UPLSPlot::R2R2Prediction(QList< SimpleLine2DPlot* > *plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  npc = projects->value(pid)->getUPLSModel(mid)->getNPC();
  uint yval = projects->value(pid)->getUPLSModel(mid)->Model()->r2y_model->m[0]->row; // each row is an y dependent value
  uint layers = projects->value(pid)->getUPLSModel(mid)->Model()->r2y_model->m[0]->col; // each col is a layer for the y
  matrix *m;

  bool getq2 = false, getq2consistency = false;
  int ncol = 3;


  if(projects->value(pid)->getUPLSModel(mid)->Model()->q2y->order > 0){
    ncol++;
    getq2 = true;
  }

  if(projects->value(pid)->getUPLSModel(mid)->Model()->q2y_yscrambling->order > 0){
    ncol++;
    getq2consistency = true;
  }

  NewMatrix(&m, npc+1, ncol);

  // set the X assis that is the principal component
  for(uint i = 0; i < (uint)npc+1; i++){
    setMatrixValue(m, i, 0, i);
  }

  // Set the r2 values into the matrix and set the q2 values into the matrix

  for(uint j = 0; j < yval; j++){ // per ogni y
    for(uint k = 0; k < layers; k++){ // per ogni layer
      #ifdef DEBUG
      qDebug() << "r2";
      PrintMatrix(projects->value(pid)->getUPLSModel(mid)->Model()->r2y_model->m[k]);
      qDebug() << "q2";
      PrintMatrix(projects->value(pid)->getUPLSModel(mid)->Model()->q2y->m[k]);
      #endif

      QStringList columntitle;
      QStringList curvenames;
      curvenames << "R2";
      columntitle << "Principal Components" << QString("R^2 Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));

      if(getq2 == true){
        columntitle << QString("Q^2 Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));
        curvenames << QString("Q^2 Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));
      }

      if(getq2consistency){
        columntitle << QString("Y Scrambling Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));
        curvenames << QString("Y Scrambling Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));
      }

      columntitle << QString("R^2 Predicted Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));
      curvenames << QString("R^2 Predicted Y %1 Layer %2").arg(QString::number(j+1)).arg(QString::number(k+1));

      int l = 1;
      setMatrixValue(m, 0, l, 0); // R^2 in 0 pc is 0
      l++;

      if(getq2 == true){
        setMatrixValue(m, 0, l, 0); // Q^2 in 0 pc is 0
        l++;
      }

      if(getq2consistency == true){
        setMatrixValue(m, 0, l, 0); //Q2 Consistency
        l++;
      }

      setMatrixValue(m, 0, l, 0);  // R2 Predicted

      for(uint i = 0; i < (uint)npc; i++){
        l = 1;
        setMatrixValue(m, i+1, l, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->r2y_model, i, j, k));
        l++;

        if(getq2 == true){
          double q2 = getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->q2y, i, j, k);
          if(q2 < 0){
            setMatrixValue(m, i+1, l, 0.f);
          }
          else{
            setMatrixValue(m, i+1, l, q2);
          }
          l++;
        }

        if(getq2consistency == true){
          setMatrixValue(m, i+1, l, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->q2y_yscrambling, i, j, k));
          l++;
        }

        double r2pred = getArrayValue(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getR2Y(), i, j, k);
        if(r2pred < 0){
          setMatrixValue(m, i+1, l, 0.f);
        }
        else{
          setMatrixValue(m, i+1, l, r2pred);
        }
      }

      QString yaxisname = "R^2";

      if(getq2 == true)
        yaxisname += " / Q2";

      if(getq2consistency == true)
        yaxisname += " / Q^2 Model Consistency";

      yaxisname += " / R2 Predicted";

      (*plot2D).append(new SimpleLine2DPlot(m, curvenames, QString("%1 - %2 - Y %3 R2 /R2 Predicted Plot").arg(projectname).arg(modelname).arg(QString::number(j+1)), columntitle, "Principal Components", yaxisname));
      (*plot2D).last()->setLabelDetail(true);
    }
  }
  DelMatrix(&m);
}

void UPLSPlot::T_ScorePlot3D(ScatterPlot3D **plot3D)
{
  qDebug()<< "T Score Plot: " << pid;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getUPLSModel(mid)->Model()->xscores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - UPLS T Score Plot").arg(projectname).arg(modelname), "PC", "PC", "PC", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}

void UPLSPlot::P_LoadingsPlot3D(QList< ScatterPlot3D* > *plot3D)
{
  qDebug()<< "P Loadings Plot: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getXVarName();

  /* xloadings->order represent the number of pc computed.
  * xloadings->m[i]->row represent the number of descriptor that is equal to X_->m[i]->col
  * xloadings->m[i]->col represent the number of order present in the array X_ and is equal to X_->order
  */

  matrix *m;

  // the size is the number of variable x the number of components and we generate a plot for each layer and the number of layer is  m[i]->col
  uint row = projects->value(pid)->getUPLSModel(mid)->Model()->xloadings->m[0]->row;
  uint col = projects->value(pid)->getUPLSModel(mid)->Model()->xloadings->order;
  uint layer = projects->value(pid)->getUPLSModel(mid)->Model()->xloadings->m[0]->col;

  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());

  for(uint k = 0; k < layer; k++){
    NewMatrix(&m, row, col);

    //copy the data on matrix to plot
    for(uint i = 0; i < row; i++){
      for(uint j = 0; j < col; j++){
        setMatrixValue(m, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->xloadings, j, i, k));
      }
    }

    QList<matrix*> mxlst;
    mxlst.append(m);
    QList<QStringList> objname;
    objname.append(varname);
    (*plot3D).append(new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - UPLS P Loadings Plot Layer: %3").arg(projectname).arg(modelname).arg(QString::number(k+1)), "PC", "PC", "PC", ScatterPlot3D::LOADINGS));
    DelMatrix(&m);
    (*plot3D).last()->setPID(pid);
  }
}

void UPLSPlot::WeightsPlot3D(QList< ScatterPlot3D* > *plot3D)
{
  qDebug()<< "Plotting UPCA Loadings PLOT: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getXVarName();
  /* xloadings->order represent the number of pc computed.
  * xloadings->m[i]->row represent the number of descriptor that is equal to X_->m[i]->col
  * xloadings->m[i]->col represent the number of order present in the array X_ and is equal to X_->order
  */

  matrix *m;

  // the size is the number of variable x the number of components and we generate a plot for each layer and the number of layer is  m[i]->col
  uint row = projects->value(pid)->getUPLSModel(mid)->Model()->xweights->m[0]->row;
  uint col = projects->value(pid)->getUPLSModel(mid)->Model()->xweights->order;
  uint layer = projects->value(pid)->getUPLSModel(mid)->Model()->xweights->m[0]->col;
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());

  for(uint k = 0; k < layer; k++){
    NewMatrix(&m, row, col);

    //copy the data on matrix to plot
    for(uint i = 0; i < row; i++){
      for(uint j = 0; j < col; j++){
        setMatrixValue(m, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->xweights, j, i, k));
      }
    }

    QList<matrix*> mxlst;
    mxlst.append(m);
    QList<QStringList> objname;
    objname.append(varname);
    (*plot3D).append(new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - UPLS W Weights Plot Layer: %3").arg(projectname).arg(modelname).arg(QString::number(k+1)), "PC", "PC", "PC", ScatterPlot3D::LOADINGS));
    DelMatrix(&m);
    (*plot3D).last()->setPID(pid);
  }
}

void UPLSPlot::U_ScorePlot3D(ScatterPlot3D **plot3D)
{
  qDebug()<< "U Score Plot: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getUPLSModel(mid)->Model()->yscores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - UPLS U Score Plot").arg(projectname).arg(modelname), "PC", "PC", "PC", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}

void UPLSPlot::Q_LoadingsPlot3D(QList< ScatterPlot3D* > *plot3D)
{
  qDebug()<< "Plotting UPLS Q Loadings PLOT: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPLSModel(mid)->getYVarName();

  /* xloadings->order represent the number of pc computed.
  * xloadings->m[i]->row represent the number of descriptor that is equal to X_->m[i]->col
  * xloadings->m[i]->col represent the number of order present in the array X_ and is equal to X_->order
  */

  matrix *m;

  // the size is the number of variable x the number of components and we generate a plot for each layer and the number of layer is  m[i]->col
  uint row = projects->value(pid)->getUPLSModel(mid)->Model()->yloadings->m[0]->row;
  uint col = projects->value(pid)->getUPLSModel(mid)->Model()->yloadings->order;
  uint layer = projects->value(pid)->getUPLSModel(mid)->Model()->yloadings->m[0]->col;
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
  for(uint k = 0; k < layer; k++){
    NewMatrix(&m, row, col);

    //copy the data on matrix to plot
    for(uint i = 0; i < row; i++){
      for(uint j = 0; j < col; j++){
        setMatrixValue(m, i, j, getArrayValue(projects->value(pid)->getUPLSModel(mid)->Model()->yloadings, j, i, k));
      }
    }

    QList<matrix*> mxlst;
    mxlst.append(m);
    QList<QStringList> objname;
    objname.append(varname);
    (*plot3D).append(new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - UPLS Q Loadings Plot Layer: %3").arg(projectname).arg(modelname).arg(QString::number(k+1)), "PC", "PC", "PC", ScatterPlot3D::LOADINGS));
    DelMatrix(&m);
    (*plot3D).last()->setPID(pid);
  }
}

void UPLSPlot::T_ScorePlotPrediction3D(ScatterPlot3D **plot3D)
{
  qDebug()<< "Plotting: " << projects->value(pid)->getProjectName();
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getUPLSModel(mid)->Model()->xscores);
  mxlst.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getXPredScores());
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getUPLSModel(mid)->getObjName());
  objname.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getXHash());
  xhash.append(projects->value(pid)->getUPLSModel(mid)->getUPLSPrediction(predid)->getXHash());
  yhash.append(projects->value(pid)->getUPLSModel(mid)->getYHash());
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getARRAYList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - UPLS T Score Plot Predictions").arg(projectname).arg(modelname), "PC", "PC", "PC", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}

UPLSPlot::UPLSPlot(PROJECTS* projects_)
{
  pid = mid = predid = npc = -1;
  projects = projects_;
}

