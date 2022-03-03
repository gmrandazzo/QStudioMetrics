#include "PLSPlot.h"
#include "scientific.h"

//#ifdef DEBUG
#include <QDebug>
//#endif

void PLSPlot::TU_Plot(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mx, my;
  mx.append(projects->value(pid)->getPLSModel(mid)->Model()->xscores);
  my.append(projects->value(pid)->getPLSModel(mid)->Model()->yscores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
  dvector *b = projects->value(pid)->getPLSModel(mid)->Model()->b;
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mx, my, b, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "t", "u", projectname + modelname + " - PLS Plot", ScatterPlot::SCORES);
  (*plot2D)->setPID(pid);
  (*plot2D)->resize(500, 300);
}

void PLSPlot::T_ScorePlot2D(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xscores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "t", "t", QString(projectname + modelname + " - PLS T/T Score Plot"), ScatterPlot::SCORES);
  (*plot2D)->setPID(pid);
}

void PLSPlot::P_LoadingsPlot2D(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xloadings);
  QList<QStringList> objnamelst;
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getXVarName();
  objnamelst.append(varname);
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "p", "p", QString("%1 - %2 - PLS P Loadings Plot").arg(projectname).arg(modelname), ScatterPlot::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
}

void PLSPlot::WeightsPlot2D(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xweights);

  matrix *yloadingsbis;

  NewMatrix(&yloadingsbis, projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row, projects->value(pid)->getPLSModel(mid)->Model()->yloadings->col);

  if(projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row > 1){
    for(uint j = 0; j < projects->value(pid)->getPLSModel(mid)->Model()->xweights->col; j++){
      double xmin, xmax;
      double ymin, ymax;
      MatrixColumnMinMax(projects->value(pid)->getPLSModel(mid)->Model()->xweights, j, &xmin, &xmax);
      MatrixColumnMinMax(projects->value(pid)->getPLSModel(mid)->Model()->yloadings, j, &ymin, &ymax);

      xmax += xmax*0.1;
      xmin += xmin*0.1;

      /*
               (xmax-xmin)(x - ymin)
        f(x) = ---------------------  + xmin
                    ymax - ymin
      */
      for(uint i = 0; i < projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row; i++){
        yloadingsbis->data[i][j] = (((xmax-xmin) * (projects->value(pid)->getPLSModel(mid)->Model()->yloadings->data[i][j] - ymin))  / (ymax -ymin)) + xmin;
      }
    }
  }
  else{
    for(uint j = 0; j < projects->value(pid)->getPLSModel(mid)->Model()->xweights->col; j++){
      double xmin, xmax;
      MatrixColumnMinMax(projects->value(pid)->getPLSModel(mid)->Model()->xweights, j, &xmin, &xmax);

      xmax += xmax*0.1;

      for(uint i = 0; i < projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row; i++){
        yloadingsbis->data[i][j] = xmax * projects->value(pid)->getPLSModel(mid)->Model()->yloadings->data[i][j];
      }
    }
  }

  mxlst.append(yloadingsbis);

  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPLSModel(mid)->getXVarName());
  objnamelst.append(projects->value(pid)->getPLSModel(mid)->getYVarName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "w", "w", QString("%1 - %2 - PLS W Weights Plot").arg(projectname).arg(modelname), ScatterPlot::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
  DelMatrix(&yloadingsbis);
}

void PLSPlot::U_ScorePlot2D(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->yscores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "u", "u", QString(projectname + modelname + " - PLS U/U Score Plot"), ScatterPlot::SCORES);
  (*plot2D)->setPID(pid);
}

void PLSPlot::Q_LoadingsPlot2D(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->yloadings);
  QList<QStringList> objnamelst;
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();
  objnamelst.append(varname);
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "q", "q", QString("%1 - %2 - PLS Q Loadings Plot").arg(projectname).arg(modelname), ScatterPlot::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
}

void PLSPlot::PQ_LoadingsPlot2D(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xloadings);
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->yloadings);
  QList<QStringList> objnamelst;
  QStringList pvarname = projects->value(pid)->getPLSModel(mid)->getXVarName();
  QStringList qvarname = projects->value(pid)->getPLSModel(mid)->getYVarName();
  objnamelst.append(pvarname);
  objnamelst.append(qvarname);
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "p/q", "p/q", QString("%1 - %2 - PLS P/Q Loadings Plot").arg(projectname).arg(modelname), ScatterPlot::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
}
void PLSPlot::T_ScorePlotPrediction2D(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xscores);
  mxlst.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getXPredScores());
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPLSModel(mid)->getObjName());
  objnamelst.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  xhash.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "t", "t", QString(projectname + modelname +" - PLS T/T Score Plot Prediction"), ScatterPlot::SCORES);
  (*plot2D)->setPID(pid);
}

void PLSPlot::RecalcVSExperimentalAndPrediction(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getPLSModel(mid)->getDataHash());

  if(did > -1 && predid > -1){
    int pred_id = projects->value(pid)->getMatrixID(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getDataHash());
    if(pred_id > -1){
      QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel = projects->value(pid)->getPLSModel(mid)->getYVarName();
      uint nobjects = objsel.size();
      uint nvars = yvarsel.size();
      matrix *recalc_y;
      NewMatrix(&recalc_y, nobjects, nvars);

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
          setMatrixValue(recalc_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y, i, j+(ny*nlv)-ny));
        }
      }

      /*bug... not did ma HASH!!*/
      matrix *y;
      NewMatrix(&y, nobjects, nvars);
      int ii = 0;
      for(int i = 0; i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++){
        if(objsel.contains(projects->value(pid)->getMatrix(did)->getObjName()[i]) == true){
          int jy= 0;
          for(int j = 1; j < projects->value(pid)->getMatrix(did)->getVarName().size(); j++){
            if(yvarsel.contains(projects->value(pid)->getMatrix(did)->getVarName()[j]) == true){
              setMatrixValue(y, ii, jy, getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(), i, j-1));
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
      QStringList pred_objsel = projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getObjName();
      uint pred_nobjects = pred_objsel.size();
      matrix *pred_y;
      NewMatrix(&pred_y, pred_nobjects, nvars);

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
          setMatrixValue(pred_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getYDipVar(), i, j+(ny*nlv)-ny));
        }
      }

      /*bug... not did ma HASH!!*/
      matrix *real_pred_y;
      NewMatrix(&real_pred_y, pred_nobjects, nvars);
      ii = 0;
      for(int i = 0; i < projects->value(pid)->getMatrix(pred_id)->getObjName().size(); i++){
        if(pred_objsel.contains(projects->value(pid)->getMatrix(pred_id)->getObjName()[i]) == true){
          int jy= 0;
          for(int j = 1; j < projects->value(pid)->getMatrix(pred_id)->getVarName().size(); j++){
            if(yvarsel.contains(projects->value(pid)->getMatrix(pred_id)->getVarName()[j]) == true){
              setMatrixValue(real_pred_y, ii, jy, getMatrixValue(projects->value(pid)->getMatrix(pred_id)->Matrix(), i, j-1));
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
      mx.append(y);
      mx.append(real_pred_y);
      my.append(recalc_y);
      my.append(pred_y);
      QList<QStringList> objname;
      objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
      objname.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      xhash.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getDataHash());
      (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalc. and Predicted"), projectname + modelname + " - PLS Recalc VS Experimental Plot" + QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
      DelMatrix(&recalc_y);
      (*plot2D)->BuildDiagonal();
      (*plot2D)->setAxisNameExtensions(varname);
      (*plot2D)->setPID(pid);
      DelMatrix(&y);
      DelMatrix(&real_pred_y);
      DelMatrix(&pred_y);
    }
  }
}


void PLSPlot::BetaCoefficients(BarPlot **betas_bar)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varnames = projects->value(pid)->getPLSModel(mid)->getXVarName();
  if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
  }

  dvector *betas;
  initDVector(&betas);
  PLSBetasCoeff(projects->value(pid)->getPLSModel(mid)->Model(), nlv, &betas);

  //compute the intercept
  //meanY - meanX*beta
  PrintDVector(betas);

  (*betas_bar) = new BarPlot(betas,
                             varnames,
                             QString("Betas Model - %1 LV %2").arg(projects->value(pid)->getPLSModel(mid)->getName()).arg(QString::number(nlv)));

  DelDVector(&betas);
}

void PLSPlot::BetaCoefficientsDurbinWatson(SimpleLine2DPlot **dw_betas_plot)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0

  dvector *dw;
  NewDVector(&dw, nlv-1);

  PLSMODEL *plsmod = projects->value(pid)->getPLSModel(mid)->Model();
  PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->xweights);
  for(uint i = 1; i < nlv; i++){
    dvector *betas;
    initDVector(&betas);

    PLSBetasCoeff(plsmod, i, &betas);
    //PrintDVector(betas);

    double n = 0.f, d = 0.f;
    d += square(betas->data[0]);
    for(uint k = 1; k < betas->size; k++){
      n += square(betas->data[k] - betas->data[k-1]);
      d += square(betas->data[k]);
    }
    dw->data[i-1] = n/d;

    DelDVector(&betas);
  }

  //PrintMatrix(dw);
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, nlv, 2);
  curvenames << "DW";

  QString yname;
  for(int j = 0; j < projects->value(pid)->getPLSModel(mid)->getYVarName().size(); j++){
   yname += " - " + projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
  }

  m->data[0][0] = 0.f;
  m->data[0][1] = 0.f;
  for(uint i = 0; i < dw->size; i++){
    m->data[i+1][0] = i+1;
    m->data[i+1][1] = dw->data[i];
  }

  (*dw_betas_plot) = new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - DW Plot %3").arg(projectname).arg(modelname).arg(yname), "Latent Variables", "DW");

  DelMatrix(&m);
  DelDVector(&dw);
}

void PLSPlot::PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
    nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getPLSModel(mid)->getDataHash());
  if(did > -1 && predid > -1){
    int pred_id = projects->value(pid)->getMatrixID(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getDataHash()); // used to get real y when find real y
    if(pred_id > -1){
      QStringList model_objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel = projects->value(pid)->getPLSModel(mid)->getYVarName();
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
          setMatrixValue(model_predicted_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->predicted_y, i, j+(ny*nlv)-ny));
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
      QStringList pred_objsel = projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getObjName();
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
          setMatrixValue(pred_predicted_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getYDipVar(), i, j+(ny*nlv)-ny));
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
      objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
      objname.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      xhash.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getDataHash());
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

void PLSPlot::RecalcVSExperimental(ScatterPlot **plot2D)
{
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

    if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    // Get Prediction
    int did = projects->value(pid)->getMatrixID(projects->value(pid)->getPLSModel(mid)->getDataHash());

    QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
    QStringList yvarsel = projects->value(pid)->getPLSModel(mid)->getYVarName();
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
        setMatrixValue(model_recalc_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y, i, j+(ny*nlv)-ny));
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
    objname.append( projects->value(pid)->getPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
    yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated"), projectname + modelname + " - PLS Recalculated VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setAxisNameExtensions(varname);
    (*plot2D)->setPID(pid);
    DelMatrix(&model_recalc_y);
    DelMatrix(&model_exp_y);
  }
}

void PLSPlot::ClassRecalcVSExperimental(QList<QStringList> *cellnames, QList<QList<QPixmap>> *images, QList<QList<QColor>> *colors)
{
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_DA_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

    if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
    LABELS classes = projects->value(pid)->getPLSModel(mid)->getClasses();

    uint nclass = classes.size();
    if(nclass == 2){
      nclass = 1;
    }

    matrix *mx;

    for(size_t j = 0; j < nclass; j++){
      QStringList row_name;
      QList<QPixmap> row_pixmap;
      QList<QColor> row_colors;

      NewMatrix(&mx, projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y->row, 3);
      for(size_t i = 0; i < projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y->row; i++){
        mx->data[i][0] = projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y->data[i][j+(nclass*nlv)-nclass];
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
}

void PLSPlot::RecalcResidualsVSExperimental(ScatterPlot** plot2D)
{
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

    if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    // Get Prediction
    int did = projects->value(pid)->getMatrixID(projects->value(pid)->getPLSModel(mid)->getDataHash());

    if(did > -1){
      QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel = projects->value(pid)->getPLSModel(mid)->getYVarName();
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
          setMatrixValue(recalc_res, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->recalc_residuals, i, j+(ny*nlv)-ny));
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
      objname.append( projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated Residuals"), projectname + modelname + " - PLS Experimental VS Recalculated Residuals Y Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
      DelMatrix(&recalc_res);
      (*plot2D)->setPID(pid);
      (*plot2D)->setAxisNameExtensions(varname);
      DelMatrix(&model_exp_y);
    }
  }
}

void PLSPlot::ClassPredictedVSExperimental(QList<QStringList> *cellnames, QList<QList<QPixmap>> *images, QList<QList<QColor>> *colors)
{
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_DA_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

    if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
    LABELS classes = projects->value(pid)->getPLSModel(mid)->getClasses();

    uint nclass = classes.size();
    if(nclass == 2){
      nclass = 1;
    }

    matrix *mx;

    for(size_t j = 0; j < nclass; j++){
      QStringList row_name;
      QList<QPixmap> row_pixmap;
      QList<QColor> row_colors;

      NewMatrix(&mx, projects->value(pid)->getPLSModel(mid)->Model()->predicted_y->row, 3);
      for(size_t i = 0; i < projects->value(pid)->getPLSModel(mid)->Model()->predicted_y->row; i++){
        mx->data[i][0] = projects->value(pid)->getPLSModel(mid)->Model()->predicted_y->data[i][j+(nclass*nlv)-nclass];
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
}

void PLSPlot::PredictedVSExperimental(ScatterPlot **plot2D)
{
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

    if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    // Get Prediction
    int did = projects->value(pid)->getMatrixID(projects->value(pid)->getPLSModel(mid)->getDataHash());

    if(did > -1){
      QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel = projects->value(pid)->getPLSModel(mid)->getYVarName();
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
          setMatrixValue(model_pred_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->predicted_y, i, j+(ny*nlv)-ny));
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
      objname.append( projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), projectname + modelname + "- PLS Predicted VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
      (*plot2D)->setAxisNameExtensions(varname);
      (*plot2D)->BuildDiagonal();
      (*plot2D)->setPID(pid);
      DelMatrix(&model_exp_y);
      DelMatrix(&model_pred_y);
    }
  }
}

void PLSPlot::PredictedResidualsVSExperimental(ScatterPlot** plot2D)
{
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

    QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

    if(nlv > projects->value(pid)->getPLSModel(mid)->getNPC()){
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    // Get Prediction
    int did = projects->value(pid)->getMatrixID(projects->value(pid)->getPLSModel(mid)->getDataHash());

    if(did > -1){
      QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel = projects->value(pid)->getPLSModel(mid)->getYVarName();
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
          setMatrixValue(pred_res, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->pred_residuals, i, j+(ny*nlv)-ny));
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
      objname.append( projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted Residuals"), projectname + modelname + " - PLS Experimental VS Predicted Residuals Y Plot"+ QString("  (LV: %1)").arg(QString::number(nlv)), ScatterPlot::SCORES);
      DelMatrix(&pred_res);
      (*plot2D)->setPID(pid);
      (*plot2D)->setAxisNameExtensions(varname);
      DelMatrix(&model_exp_y);
    }
  }
}

QList< SimpleLine2DPlot* > PLSPlot::R2Q2()
{
  QList< SimpleLine2DPlot* > plots;
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
    uint yval = projects->value(pid)->getPLSModel(mid)->Model()->r2y_recalculated->col;
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
      QString yname = projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
      #ifdef DEBUG
      qDebug() << "r2";
      PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->r2y_model);
      qDebug() << "q2";
      PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->q2y);
      #endif


      setMatrixValue(m, 0, 1, 0); // R^2 in 0 pc is 0
      setMatrixValue(m, 0, 2, 0); // Q^2 in 0 pc is 0

      for(uint i = 0; i < nlv-1; i++){
        setMatrixValue(m, i+1, 1, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->r2y_recalculated, i, l));
        double q2 = getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->q2y, i, l);
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
  }
  return plots;
}

QList< SimpleLine2DPlot* > PLSPlot::RMSE()
{
  QList< SimpleLine2DPlot* > plots;
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    uint yval = projects->value(pid)->getPLSModel(mid)->Model()->r2y_recalculated->col;
    matrix *m;

    QStringList curvenames;
    NewMatrix(&m, nlv, 3);
    curvenames << "RMSE(Training)" << "RMSE(Validation)";

    // set the X assis that is the principal component
    for(uint i = 0; i < nlv; i++){
      setMatrixValue(m, i, 0, i+1);
    }

    // Set the rmse values into the matrix
    uint l = 0;
    for(uint j = 0; j < yval; j++){
      QString yname = projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
      #ifdef DEBUG
      qDebug() << "training";
      PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->sdec);
      qDebug() << "validation";
      PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->sdep);
      #endif


      setMatrixValue(m, 0, 1, 0); // SDEC in 0 pc is 0
      setMatrixValue(m, 0, 2, 0); // SDEP in 0 pc is 0

      for(uint i = 0; i < nlv; i++){
        setMatrixValue(m, i, 1, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->sdec, i, l));
        setMatrixValue(m, i, 2, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->sdep, i, l));
      }
      l++;


      #ifdef DEBUG
      qDebug() << "Final Matrix";
      PrintMatrix(m);
      #endif
      plots.append(new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - RMSE Training/Validation Plot %3").arg(projectname).arg(modelname).arg(yname), "Latent Variables", "RMSE (Training/Validation)"));
    }
    DelMatrix(&m);
  }
  return plots;
}

QList< SimpleLine2DPlot* > PLSPlot::ROCAUCs()
{
  QList< SimpleLine2DPlot* > plots;
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_DA_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
    uint yval = projects->value(pid)->getPLSModel(mid)->Model()->roc_auc_recalculated->col;
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
      QString yname = projects->value(pid)->getPLSModel(mid)->getClasses()[j].name;

      setMatrixValue(m, 0, 1, 0); // AUC Recalc in 0 pc is 0
      setMatrixValue(m, 0, 2, 0); // AUC Predicted in 0 pc is 0

      for(uint i = 0; i < nlv-1; i++){
        setMatrixValue(m, i+1, 1, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->roc_auc_recalculated, i, l));
        setMatrixValue(m, i+1, 2, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->roc_auc_validation, i, l));
      }
      l++;

      plots.append(new SimpleLine2DPlot(m,
                                        curvenames,
                                        QString(" %1 - %2 - ROC AUC Recalculated/Predicted Plot %3").arg(projectname).arg(modelname).arg(yname),
                                        "Latent Variables", "ROC AUC Recalculated / AUC Predicted"));
    }
    DelMatrix(&m);
  }
  return plots;
}

QList< SimpleLine2DPlot* > PLSPlot::ROCCurves()
{
  QList< SimpleLine2DPlot* > plots;
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_DA_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC(); // == roc_recalculated->order;
    uint yval = projects->value(pid)->getPLSModel(mid)->Model()->roc_recalculated->m[0]->col/2.;

    QStringList curvenames;
    curvenames << "ROC Recalculated" << "ROC Predicted";

    tensor *roc_recalculated = projects->value(pid)->getPLSModel(mid)->Model()->roc_recalculated;
    tensor *roc_predicted = projects->value(pid)->getPLSModel(mid)->Model()->roc_validation;


    for(uint i = 0; i < nlv; i++){
      for(uint j = 0; j < yval; j++){
        QString yname = projects->value(pid)->getPLSModel(mid)->getClasses()[j].name;
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
  }
  return plots;
}

QList< SimpleLine2DPlot* > PLSPlot::PrecisionRecallAveragePrecision()
{
  QList< SimpleLine2DPlot* > plots;
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_DA_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
    uint yval = projects->value(pid)->getPLSModel(mid)->Model()->precision_recall_ap_recalculated->col;
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
      QString yname = projects->value(pid)->getPLSModel(mid)->getClasses()[j].name;

      setMatrixValue(m, 0, 1, 0); // AUC Recalc in 0 pc is 0
      setMatrixValue(m, 0, 2, 0); // AUC Predicted in 0 pc is 0

      for(uint i = 0; i < nlv-1; i++){
        setMatrixValue(m, i+1, 1, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->precision_recall_ap_recalculated, i, l));
        setMatrixValue(m, i+1, 2, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->precision_recall_ap_validation, i, l));
      }
      l++;

      plots.append(new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - Precision-Recall AUC Recalculated/Predicted Plot %3").arg(projectname).arg(modelname).arg(yname), "Latent Variables", "Prec./Rec. Recalculated and Predicted"));
    }
    DelMatrix(&m);
  }
  return plots;
}

QList< SimpleLine2DPlot* > PLSPlot::PrecisionRecallCurves()
{
  QList< SimpleLine2DPlot* > plots;
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_DA_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC(); // == roc_recalculated->order;
    uint yval = projects->value(pid)->getPLSModel(mid)->Model()->precision_recall_recalculated->m[0]->col/2.;

    QStringList curvenames;
    curvenames << "Precision-Recall Recalculated" << "Precision-Recall Predicted";

    tensor *pr_recalculated = projects->value(pid)->getPLSModel(mid)->Model()->precision_recall_recalculated;
    tensor *pr_predicted = projects->value(pid)->getPLSModel(mid)->Model()->precision_recall_validation;


    for(uint i = 0; i < nlv; i++){
      for(uint j = 0; j < yval; j++){
        QString yname = projects->value(pid)->getPLSModel(mid)->getClasses()[j].name;
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
  }
  return plots;
}

QList< SimpleLine2DPlot* > PLSPlot::R2R2Prediction()
{
  QList< SimpleLine2DPlot* > plots;
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
    uint yval = projects->value(pid)->getPLSModel(mid)->Model()->r2y_recalculated->col;

    bool getq2 = false;
    matrix *m;

    int ncol = 3;

    if(projects->value(pid)->getPLSModel(mid)->Model()->q2y->row > 0){
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

      QString yname = projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
      #ifdef DEBUG
      qDebug() << "r2";
      PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->r2y_model);
      qDebug() << "q2";
      PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->q2y);
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
        setMatrixValue(m, i+1, k, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->r2y_recalculated, i, l));
        k++;

        if(getq2 == true){
          double q2 = getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->q2y, i, l);
          if(q2 < 0){
            setMatrixValue(m, i+1, k, 0.f);
          }
          else{
            setMatrixValue(m, i+1, k, q2);
          }
          k++;
        }

        double r2pred = getMatrixValue(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getR2Y(), i, l);
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
  }
  return plots;
}

QList< SimpleLine2DPlot* > PLSPlot::RMSEPrediction()
{
  QList< SimpleLine2DPlot* > plots;
  if(projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_DA_){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    uint nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    uint yval = projects->value(pid)->getPLSModel(mid)->Model()->r2y_recalculated->col;

    bool getsdep = false;
    matrix *m;

    int ncol = 3;

    if(projects->value(pid)->getPLSModel(mid)->Model()->sdep->row > 0){
      ncol++;
      getsdep = true;
    }

    NewMatrix(&m, nlv, ncol);

    // set the X assis that is the principal component
    for(uint i = 0; i < nlv; i++){
      setMatrixValue(m, i, 0, i+1);
    }

    // Set the sdec, r2prediction, sdep into the matrix
    uint l = 0;
    for(uint j = 0; j < yval; j++){

      QString yname = projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
      #ifdef DEBUG
      qDebug() << "SDEC";
      PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->sdec);
      qDebug() << "SDEP";
      PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->sdep);
      #endif

      QStringList curvenames;
      curvenames << "RMSE(Training)";

      if(getsdep == true){
        curvenames << QString("RMSE (Testing) Y %1").arg(yname);
      }

      curvenames << QString("RMSE (Prediction) Y %1").arg(yname);

      for(uint i = 0; i < nlv; i++){
        int k = 1;
        setMatrixValue(m, i, k, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->sdec, i, l));
        k++;

        if(getsdep == true){
          setMatrixValue(m, i, k, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->sdep, i, l));
          k++;
        }

        setMatrixValue(m, i, k, getMatrixValue(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getSDEC(), i, l));
      }
      l++;

      #ifdef DEBUG
      qDebug() << "Final Matrix";
      PrintMatrix(m);
      #endif
      QString yaxisname = "RMSE (Training";

      if(getsdep == true)
        yaxisname += "/Testing";

      yaxisname += " /Prediction)";

      plots.append(new SimpleLine2DPlot(m, curvenames, QString("%1 - %2 - RMSE External Prediction Plot Y %3").arg(projectname).arg(modelname).arg(yname), "Latent Variables", yaxisname));
    }
    DelMatrix(&m);
  }
  return plots;
}

QList<ScatterPlot*> PLSPlot::YScramblingPlot()
{
  QList<ScatterPlot*> plots2D;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(new matrix);
  size_t nobj = projects->value(pid)->getPLSModel(mid)->Model()->yscrambling->row;
  size_t ndepvar = (projects->value(pid)->getPLSModel(mid)->Model()->yscrambling->col-1)/2;
  NewMatrix(&mxlst.last(), nobj, 2); // R2
  mxlst.append(new matrix);
  NewMatrix(&mxlst.last(), nobj, 2); // Q2
  QList<QStringList> objnamelst;
  objnamelst.append(QStringList());
  objnamelst.append(QStringList());
  /*set the X Constant */
  for(size_t i = 0; i < nobj; i++){
    mxlst[0]->data[i][0] = projects->value(pid)->getPLSModel(mid)->Model()->yscrambling->data[i][0];
    mxlst[1]->data[i][0] = projects->value(pid)->getPLSModel(mid)->Model()->yscrambling->data[i][0];
    objnamelst[0].append("R2");
    objnamelst[1].append("Q2");
  }

  for(size_t j = 0; j < ndepvar; j++){
    for(size_t i = 0; i < nobj; i++){
      mxlst[0]->data[i][1] = projects->value(pid)->getPLSModel(mid)->Model()->yscrambling->data[i][j+1];
      mxlst[1]->data[i][1] = projects->value(pid)->getPLSModel(mid)->Model()->yscrambling->data[i][j+1+ndepvar];
    }
    plots2D.append(new ScatterPlot(mxlst, objnamelst, "Correlation with Y Real Vector", "R2/Q2", QString("PLS Y Scrambling Plot")));
    plots2D.last()->setPID(pid);
  }

  DelMatrix(&mxlst[1]);
  DelMatrix(&mxlst[0]);
  mxlst.clear();
  return plots2D;
}

void PLSPlot::T_ScorePlot3D(ScatterPlot **plot3D)
{
  QString projectname =  projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xscores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot(mxlst,
                              objname,
                              &projects->value(pid)->getMATRIXList(),
                              xhash,
                              yhash,
                              &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), 
                              "t",
                              "t",
                              "t",
                              QString("%1 - %2 - PLS T Score Plot").arg(projectname).arg(modelname),
                              ScatterPlot::SCORES);
  (*plot3D)->setPID(pid);
}

void PLSPlot::P_LoadingsPlot3D(ScatterPlot **plot3D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getXVarName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xloadings);
  QList<QStringList> objname;
  objname.append(varname);
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot(mxlst,
                              objname,
                              &projects->value(pid)->getMATRIXList(),
                              xhash,
                              yhash,
                              &projects->value(pid)->getObjectLabels(),
                              &projects->value(pid)->getVariableLabels(),
                              "p",
                              "p",
                              "p",
                              QString("%1 - %2 - PLS P Loadings Plot").arg(projectname).arg(modelname),
                              ScatterPlot::LOADINGS);
  (*plot3D)->setPID(pid);
  /*ScatterPlot *plt3D = new ScatterPlot(mxlst, objname, &projects->value(pid)->getVariableLabels(), projectname + modelname +" - PLS Loadings Plot", ScatterPlot::VARIABLELABELS);*/
}

void PLSPlot::WeightsPlot3D(ScatterPlot **plot3D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getXVarName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xweights);
  matrix *yloadingsbis;

  NewMatrix(&yloadingsbis, projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row, projects->value(pid)->getPLSModel(mid)->Model()->yloadings->col);

  if(projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row > 1){
    for(uint j = 0; j < projects->value(pid)->getPLSModel(mid)->Model()->xweights->col; j++){
      double xmin, xmax;
      double ymin, ymax;
      MatrixColumnMinMax(projects->value(pid)->getPLSModel(mid)->Model()->xweights, j, &xmin, &xmax);
      MatrixColumnMinMax(projects->value(pid)->getPLSModel(mid)->Model()->yloadings, j, &ymin, &ymax);

      xmax += xmax*0.1;
      xmin += xmin*0.1;

      /*
               (xmax-xmin)(x - ymin)
        f(x) = ---------------------  + xmin
                    ymax - ymin
      */
      for(uint i = 0; i < projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row; i++){
        yloadingsbis->data[i][j] = (((xmax-xmin) * (projects->value(pid)->getPLSModel(mid)->Model()->yloadings->data[i][j] - ymin))  / (ymax -ymin)) + xmin;
      }
    }
  }
  else{
    for(uint j = 0; j < projects->value(pid)->getPLSModel(mid)->Model()->xweights->col; j++){
      double xmin, xmax;
      MatrixColumnMinMax(projects->value(pid)->getPLSModel(mid)->Model()->xweights, j, &xmin, &xmax);

      xmax += xmax*0.1;

      for(uint i = 0; i < projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row; i++){
        yloadingsbis->data[i][j] = xmax * projects->value(pid)->getPLSModel(mid)->Model()->yloadings->data[i][j];
      }
    }
  }

  mxlst.append(yloadingsbis);

  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPLSModel(mid)->getXVarName());
  objnamelst.append(projects->value(pid)->getPLSModel(mid)->getYVarName());

  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot(mxlst,
                              objnamelst,
                              &projects->value(pid)->getMATRIXList(),
                              xhash,
                              yhash,
                              &projects->value(pid)->getObjectLabels(),
                              &projects->value(pid)->getVariableLabels(),
                              "w",
                              "w",
                              "w",
                              QString("%1 - %2 - PLS W Weights Plot").arg(projectname).arg(modelname), ScatterPlot::LOADINGS);
  /*ScatterPlot *plt3D = new ScatterPlot(mxlst, objname, &projects->value(pid)->getVariableLabels(), projectname + modelname +" - W/W/W Loadings Plot", ScatterPlot::VARIABLELABELS);*/
  (*plot3D)->setPID(pid);
  DelMatrix(&yloadingsbis);
}

void PLSPlot::U_ScorePlot3D(ScatterPlot **plot3D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->yscores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot(mxlst,
                              objname,
                              &projects->value(pid)->getMATRIXList(),
                              xhash,
                              yhash,
                              &projects->value(pid)->getObjectLabels(),
                              &projects->value(pid)->getVariableLabels(),
                              "u",
                              "u",
                              "u",
                              QString("%1 - %2 - PLS U Score Plot").arg(projectname).arg(modelname),
                              ScatterPlot::SCORES);
  (*plot3D)->setPID(pid);
}


void PLSPlot::Q_LoadingsPlot3D(ScatterPlot **plot3D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(pid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getXVarName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->yloadings);
  QList<QStringList> objname;
  objname.append(varname);
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot(mxlst,
                              objname,
                              &projects->value(pid)->getMATRIXList(),
                              xhash,
                              yhash,
                              &projects->value(pid)->getObjectLabels(),
                              &projects->value(pid)->getVariableLabels(),
                              "q",
                              "q",
                              "q",
                              QString("%1 - %2 - PLS Q Loadings Plot").arg(projectname).arg(modelname), ScatterPlot::LOADINGS);
  /*
  ScatterPlot *plt3D = new ScatterPlot(mxlst, objname, &projects->value(pid)->getVariableLabels(), projectname + modelname +" - Q/Q/Q Loadings Plot", ScatterPlot::VARIABLELABELS);*/
  (*plot3D)->setPID(pid);
}

void PLSPlot::T_ScorePlotPrediction3D(ScatterPlot **plot3D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xscores);
  mxlst.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getXPredScores());
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
  objname.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  xhash.append(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getDataHash());
  yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot(mxlst,
                              objname,
                              &projects->value(pid)->getMATRIXList(),
                              xhash,
                              yhash,
                              &projects->value(pid)->getObjectLabels(),
                              &projects->value(pid)->getVariableLabels(),
                              "t",
                              "t",
                              "t",
                              QString("%1 - %2 - PLS T Score Plot Predicion").arg(projectname).arg(modelname),
                              ScatterPlot::SCORES);
  (*plot3D)->setPID(pid);
}


PLSPlot::PLSPlot(PROJECTS* projects_)
{
  pid = mid = predid = -1;
  projects = projects_;
}
