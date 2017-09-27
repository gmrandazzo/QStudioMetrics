#include "PLSPlot.h"
#include "scientific.h"

#ifdef DEBUG
#include <QDebug>
#endif

void PLSPlot::TU_Plot(ScatterPlot2D **plot2D)
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
  (*plot2D) = new ScatterPlot2D(mx, my, b, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "t", "u", projectname + modelname + " - PLS Plot", ScatterPlot2D::SCORES);
  (*plot2D)->setPID(pid);
  (*plot2D)->resize(500, 300);
}

void PLSPlot::T_ScorePlot2D(ScatterPlot2D **plot2D)
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
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "t", "t", QString(projectname + modelname + " - PLS T/T Score Plot"), ScatterPlot2D::SCORES);
  (*plot2D)->setPID(pid);
}

void PLSPlot::P_LoadingsPlot2D(ScatterPlot2D **plot2D)
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
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "p", "p", QString("%1 - %2 - PLS P Loadings Plot").arg(projectname).arg(modelname), ScatterPlot2D::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
}

void PLSPlot::WeightsPlot2D(ScatterPlot2D **plot2D)
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
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "w", "w", QString("%1 - %2 - PLS W Weights Plot").arg(projectname).arg(modelname), ScatterPlot2D::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
  DelMatrix(&yloadingsbis);
}

void PLSPlot::U_ScorePlot2D(ScatterPlot2D **plot2D)
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
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "u", "u", QString(projectname + modelname + " - PLS U/U Score Plot"), ScatterPlot2D::SCORES);
  (*plot2D)->setPID(pid);
}

void PLSPlot::Q_LoadingsPlot2D(ScatterPlot2D **plot2D)
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
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "q", "q", QString("%1 - %2 - PLS Q Loadings Plot").arg(projectname).arg(modelname), ScatterPlot2D::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
}

void PLSPlot::T_ScorePlotPrediction2D(ScatterPlot2D **plot2D)
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
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "t", "t", QString(projectname + modelname +" - PLS T/T Score Plot Prediction"), ScatterPlot2D::SCORES);
  (*plot2D)->setPID(pid);
}

void PLSPlot::RecalcVSExperimentalAndPrediction(ScatterPlot2D **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getPLSModel(mid)->getNPC();
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
          setMatrixValue(recalc_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y, i, j+(ny*npc)-ny));
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
          setMatrixValue(pred_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getYDipVar(), i, j+(ny*npc)-ny));
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
      (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalc. and Predicted"), projectname + modelname + " - PLS Recalc VS Experimental Plot" + QString("  (LV: %1)").arg(QString::number(npc)), ScatterPlot2D::SCORES);
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
  if(npc > projects->value(pid)->getPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getPLSModel(mid)->getNPC();
  }

  dvector *betas;
  initDVector(&betas);
  PLSBetasCoeff(projects->value(pid)->getPLSModel(mid)->Model(), npc, &betas);

  //compute the intercept
  //meanY - meanX*beta
  PrintDVector(betas);

  (*betas_bar) = new BarPlot(betas, varnames, QString("Betas Model - %1 LV %2").arg(projects->value(pid)->getPLSModel(mid)->getName()).arg(QString::number(npc)), "Variables", "Betas");

  DelDVector(&betas);
}

void PLSPlot::BetaCoefficientsDurbinWatson(SimpleLine2DPlot **dw_betas_plot)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  uint npc = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0

  dvector *dw;
  NewDVector(&dw, npc-1);

  PLSMODEL *plsmod = projects->value(pid)->getPLSModel(mid)->Model();
  PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->xweights);
  for(uint i = 1; i < npc; i++){
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
  NewMatrix(&m, npc, 2);
  curvenames << "DW";

  QString yname;
  for(int j = 0; j < projects->value(pid)->getPLSModel(mid)->getYVarName().size(); j++){
   yname += " - " + projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
  }
  QStringList columntitle;
  columntitle << "Latent Variables" << QString("DW %1").arg(yname);
  m->data[0][0] = 0.f;
  m->data[0][1] = 0.f;
  for(uint i = 0; i < dw->size; i++){
    m->data[i+1][0] = i+1;
    m->data[i+1][1] = dw->data[i];
  }

  (*dw_betas_plot) = new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - DW Plot %3").arg(projectname).arg(modelname).arg(yname), columntitle, "Latent Variables", "DW");
  (*dw_betas_plot)->setLabelDetail(true);
  (*dw_betas_plot)->setXminXmanXTicks(0, npc, npc);
  (*dw_betas_plot)->setYminYmanYTicks(0, 3, 10);

  DelMatrix(&m);
  DelDVector(&dw);
}

void PLSPlot::PredictedVSExperimentalAndPrediction(ScatterPlot2D **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getPLSModel(mid)->getNPC();
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
          setMatrixValue(model_predicted_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->predicted_y, i, j+(ny*npc)-ny));
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
          setMatrixValue(pred_predicted_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->getPLSPrediction(predid)->getYDipVar(), i, j+(ny*npc)-ny));
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
      (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), projectname + modelname + " - PLS Recalc VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(npc)), ScatterPlot2D::SCORES);
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

void PLSPlot::RecalcVSExperimental(ScatterPlot2D **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getPLSModel(mid)->getNPC();
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
      setMatrixValue(model_recalc_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y, i, j+(ny*npc)-ny));
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
  (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated"), projectname + modelname + " - PLS Recalculated VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(npc)), ScatterPlot2D::SCORES);
  (*plot2D)->BuildDiagonal();
  (*plot2D)->setAxisNameExtensions(varname);
  (*plot2D)->setPID(pid);
  DelMatrix(&model_recalc_y);
  DelMatrix(&model_exp_y);
}

void PLSPlot::RecalcResidualsVSExperimental(ScatterPlot2D** plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getPLSModel(mid)->getNPC();
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
        setMatrixValue(recalc_res, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->recalc_residuals, i, j+(ny*npc)-ny));
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
    (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated Residuals"), projectname + modelname + " - PLS Experimental VS Recalculated Residuals Y Plot"+ QString("  (LV: %1)").arg(QString::number(npc)), ScatterPlot2D::SCORES);
    DelMatrix(&recalc_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
  }
}


void PLSPlot::PredictedVSExperimental(ScatterPlot2D **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getPLSModel(mid)->getNPC();
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
        setMatrixValue(model_pred_y, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->predicted_y, i, j+(ny*npc)-ny));
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
    (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), projectname + modelname + "- PLS Predicted VS Experimental Plot"+ QString("  (LV: %1)").arg(QString::number(npc)), ScatterPlot2D::SCORES);
    (*plot2D)->setAxisNameExtensions(varname);
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setPID(pid);
    DelMatrix(&model_exp_y);
    DelMatrix(&model_pred_y);
  }
}

void PLSPlot::PredictedResidualsVSExperimental(ScatterPlot2D** plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if(npc > projects->value(pid)->getPLSModel(mid)->getNPC()){
    npc = projects->value(pid)->getPLSModel(mid)->getNPC();
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
        setMatrixValue(pred_res, i, j, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->pred_residuals, i, j+(ny*npc)-ny));
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
    (*plot2D) = new ScatterPlot2D(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted Residuals"), projectname + modelname + " - PLS Experimental VS Predicted Residuals Y Plot"+ QString("  (LV: %1)").arg(QString::number(npc)), ScatterPlot2D::SCORES);
    DelMatrix(&pred_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
  }
}

QList< SimpleLine2DPlot* > PLSPlot::R2Q2()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  uint npc = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
  uint yval = projects->value(pid)->getPLSModel(mid)->Model()->r2y_model->col;
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, npc, 3);
  curvenames << "R2" << "Q2";

  // set the X assis that is the principal component
  for(uint i = 0; i < npc; i++){
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

    QStringList columntitle;
    columntitle << "Latent Variables" << QString("R2 %1").arg(yname) << QString("Q2 %1").arg(yname);

    setMatrixValue(m, 0, 1, 0); // R^2 in 0 pc is 0
    setMatrixValue(m, 0, 2, 0); // Q^2 in 0 pc is 0

    for(uint i = 0; i < npc-1; i++){
      setMatrixValue(m, i+1, 1, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->r2y_model, i, l));
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
    plots.append(new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - R2 Q2 Plot %3").arg(projectname).arg(modelname).arg(yname), columntitle, "Latent Variables", "R2 / Q2"));
    plots.last()->setLabelDetail(true);
    plots.last()->setXminXmanXTicks(0, npc, npc);
    plots.last()->setYminYmanYTicks(0, 1.2, 10);
  }
  DelMatrix(&m);
  return plots;
}


QList< SimpleLine2DPlot* > PLSPlot::R2R2Prediction()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  uint npc = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
  uint yval = projects->value(pid)->getPLSModel(mid)->Model()->r2y_model->col;

  bool getq2 = false;
  matrix *m;

  int ncol = 3;

  if(projects->value(pid)->getPLSModel(mid)->Model()->q2y->row > 0){
    ncol++;
    getq2 = true;
  }

  NewMatrix(&m, npc, ncol);

  // set the X assis that is the principal component
  for(uint i = 0; i < npc; i++){
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

    for(uint i = 0; i < npc-1; i++){
      k = 1;
      setMatrixValue(m, i+1, k, getMatrixValue(projects->value(pid)->getPLSModel(mid)->Model()->r2y_model, i, l));
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


    plots.append(new SimpleLine2DPlot(m, curvenames, QString("%1 - %2 - R2 Q2 Plot Y %3").arg(projectname).arg(modelname).arg(yname), columntitle, "Latent Variables", yaxisname));
    plots.last()->setLabelDetail(true);
  }
  DelMatrix(&m);
  return plots;
}

QList<ScatterPlot2D*> PLSPlot::YScramblingPlot()
{
  QList<ScatterPlot2D*> plots2D;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(new matrix);
  size_t nobj = projects->value(pid)->getPLSModel(mid)->Model()->r2q2scrambling->row;
  size_t ndepvar = (projects->value(pid)->getPLSModel(mid)->Model()->r2q2scrambling->col-1)/2;
  NewMatrix(&mxlst.last(), nobj, 2); // R2
  mxlst.append(new matrix);
  NewMatrix(&mxlst.last(), nobj, 2); // Q2
  QList<QStringList> objnamelst;
  objnamelst.append(QStringList());
  objnamelst.append(QStringList());
  /*set the X Constant */
  for(size_t i = 0; i < nobj; i++){
    mxlst[0]->data[i][0] = projects->value(pid)->getPLSModel(mid)->Model()->r2q2scrambling->data[i][0];
    mxlst[1]->data[i][0] = projects->value(pid)->getPLSModel(mid)->Model()->r2q2scrambling->data[i][0];
    objnamelst[0].append("R2");
    objnamelst[1].append("Q2");
  }

  for(size_t j = 0; j < ndepvar; j++){
    for(size_t i = 0; i < nobj; i++){
      mxlst[0]->data[i][1] = projects->value(pid)->getPLSModel(mid)->Model()->r2q2scrambling->data[i][j+1];
      mxlst[1]->data[i][1] = projects->value(pid)->getPLSModel(mid)->Model()->r2q2scrambling->data[i][j+1+ndepvar];
    }
    plots2D.append(new ScatterPlot2D(mxlst, objnamelst, "Correlation with Y Real Vector", "R2/Q2", QString("PLS Y Scrambling Plot")));
    plots2D.last()->setPID(pid);
  }

  DelMatrix(&mxlst[1]);
  DelMatrix(&mxlst[0]);
  mxlst.clear();
  return plots2D;
}


QList< SimpleLine2DPlot* > PLSPlot::Q2SampleValidator()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  size_t npc = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
  size_t yval = projects->value(pid)->getPLSModel(mid)->Model()->q2y->col;
  size_t nrandmodels = projects->value(pid)->getPLSModel(mid)->Model()->q2_sample_validation->col/yval;
  matrix *q2_sample_validation = projects->value(pid)->getPLSModel(mid)->Model()->q2_sample_validation;
  matrix *m;

  NewMatrix(&m, npc, nrandmodels+1);// First column is the PC

  // set the X assis that is the principal component
  for(size_t i = 0; i < npc; i++){
    setMatrixValue(m, i, 0, i);
  }

   // Q^2 in 0 pc is 0
  for(size_t i = 0; i < nrandmodels; i++){
    setMatrixValue(m, 0, i, 0.f);
  }


  for(size_t j = 0; j < yval; j++){
    QString yname = projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
    for(size_t i = 0; i < npc-1; i++){
      m->data[i+1][1] = q2_sample_validation->data[i][j];
      size_t inc = yval;
      for(size_t k = 1; k < nrandmodels; k++){
        m->data[i+1][k+1] = q2_sample_validation->data[i][j+inc];
        inc += yval;
      }
    }

#ifdef DEBUG
    qDebug() << "Final Matrix";
    PrintMatrix(m);
#endif
    plots.append(new SimpleLine2DPlot(m, "Q2", QString(" %1 - %2 - Q^2 Sample Valiation Plot Y %3").arg(projectname).arg(modelname).arg(yname), "Principal Components", "Q^2"));
    plots.last()->setLabelDetail(true);
  }
  DelMatrix(&m);
  return plots;
}

QList< SimpleLine2DPlot* > PLSPlot::SDEPSampleValidator()
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  size_t npc = projects->value(pid)->getPLSModel(mid)->getNPC() + 1; // +1 because we start from 0
  size_t yval = projects->value(pid)->getPLSModel(mid)->Model()->q2y->col;
  size_t nrandmodels = projects->value(pid)->getPLSModel(mid)->Model()->sdep_sample_validation->col/yval;
  matrix *sdep_sample_validation = projects->value(pid)->getPLSModel(mid)->Model()->sdep_sample_validation;
  matrix *m;

  NewMatrix(&m, npc, nrandmodels+1);// First column is the PC

  // set the X assis that is the principal component
  for(size_t i = 0; i < npc; i++){
    setMatrixValue(m, i, 0, i);
  }

   // Q^2 in 0 pc is 0
  for(size_t i = 0; i < nrandmodels; i++){
    setMatrixValue(m, 0, i, 0.f);
  }


  for(size_t j = 0; j < yval; j++){
    QString yname = projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
    for(size_t i = 0; i < npc-1; i++){
      m->data[i+1][1] = sdep_sample_validation->data[i][j];
      size_t inc = yval;
      for(size_t k = 1; k < nrandmodels; k++){
        m->data[i+1][k+1] = sdep_sample_validation->data[i][j+inc];
        inc += yval;
      }
    }

#ifdef DEBUG
    qDebug() << "Final Matrix";
    PrintMatrix(m);
#endif
    plots.append(new SimpleLine2DPlot(m, "SDEP", QString(" %1 - %2 - SDEP Sample Valiation Plot Y %3").arg(projectname).arg(modelname).arg(yname), "Principal Components", "Q^2"));
    plots.last()->setLabelDetail(true);
  }
  DelMatrix(&m);
  return plots;
}

QList< SimpleScatterPlot3D* >  PLSPlot::Q2SurfacePlot()
{
  QList < SimpleScatterPlot3D * > plots;
  QString projectname =  projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList ynames =  projects->value(pid)->getPLSModel(mid)->getYVarName();

  matrix *q2_sample_validation_surface = projects->value(pid)->getPLSModel(mid)->Model()->q2_sample_validation_surface;
  matrix *m;
  NewMatrix(&m, q2_sample_validation_surface->row, 3);
  for(size_t i = 0; i < q2_sample_validation_surface->row; i++){
    m->data[i][0] = q2_sample_validation_surface->data[i][0];
    m->data[i][1] = q2_sample_validation_surface->data[i][1];
  }

  for(size_t j = 0; j < q2_sample_validation_surface->col-2; j++){
    QList<double> colorvalue;
    for(size_t i = 0; i < q2_sample_validation_surface->row; i++){
      m->data[i][2] = q2_sample_validation_surface->data[i][2+j];
      colorvalue << q2_sample_validation_surface->data[i][2+j];
    }

    plots.append(new SimpleScatterPlot3D(m, colorvalue, QString(" %1 - %2 - Q^2 Dynamic Sample Valiation Plot Y %3").arg(projectname).arg(modelname).arg(ynames[j]), "N. Obj", "Latent Variables", "Q^2"));
    plots.last()->setPID(pid);
  }
  DelMatrix(&m);
  return plots;
}

QList< SimpleScatterPlot3D* >  PLSPlot::SDEPSurfacePlot()
{
  QList < SimpleScatterPlot3D * > plots;
  QString projectname =  projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList ynames =  projects->value(pid)->getPLSModel(mid)->getYVarName();

  matrix *sdep_sample_validation_surface = projects->value(pid)->getPLSModel(mid)->Model()->sdep_sample_validation_surface;
  matrix *m;
  NewMatrix(&m, sdep_sample_validation_surface->row, 3);
  for(size_t i = 0; i < sdep_sample_validation_surface->row; i++){
    m->data[i][0] = sdep_sample_validation_surface->data[i][0];
    m->data[i][1] = sdep_sample_validation_surface->data[i][1];
  }

  for(size_t j = 0; j < sdep_sample_validation_surface->col-2; j++){
    QList<double> colorvalue;
    for(size_t i = 0; i < sdep_sample_validation_surface->row; i++){
      m->data[i][2] = sdep_sample_validation_surface->data[i][2+j];
      colorvalue << sdep_sample_validation_surface->data[i][2+j];
    }

    plots.append(new SimpleScatterPlot3D(m, colorvalue, QString(" %1 - %2 - Q^2 Dynamic Sample Valiation Plot Y %3").arg(projectname).arg(modelname).arg(ynames[j]), "N. Obj", "Latent Variables", "SDEP"));
    plots.last()->setPID(pid);
  }
  DelMatrix(&m);
  return plots;
}

void PLSPlot::T_ScorePlot3D(ScatterPlot3D **plot3D)
{
  QString projectname =  projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPLSModel(mid)->Model()->xscores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PLS T Score Plot").arg(projectname).arg(modelname), "t", "t", "t", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}

void PLSPlot::P_LoadingsPlot3D(ScatterPlot3D **plot3D)
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
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PLS P Loadings Plot").arg(projectname).arg(modelname), "p", "p", "p", ScatterPlot3D::LOADINGS);
  (*plot3D)->setPID(pid);
  /*ScatterPlot3D *plt3D = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getVariableLabels(), projectname + modelname +" - PLS Loadings Plot", ScatterPlot3D::VARIABLELABELS);*/
}

void PLSPlot::WeightsPlot3D(ScatterPlot3D **plot3D)
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
  (*plot3D) = new ScatterPlot3D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PLS W Weights Plot").arg(projectname).arg(modelname), "w", "w", "w", ScatterPlot3D::LOADINGS);
  /*ScatterPlot3D *plt3D = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getVariableLabels(), projectname + modelname +" - W/W/W Loadings Plot", ScatterPlot3D::VARIABLELABELS);*/
  (*plot3D)->setPID(pid);
  DelMatrix(&yloadingsbis);
}

void PLSPlot::U_ScorePlot3D(ScatterPlot3D **plot3D)
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
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PLS U Score Plot").arg(projectname).arg(modelname), "u", "u", "u", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}


void PLSPlot::Q_LoadingsPlot3D(ScatterPlot3D **plot3D)
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
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PLS Q Loadings Plot").arg(projectname).arg(modelname), "q", "q", "q", ScatterPlot3D::LOADINGS);
  /*
  ScatterPlot3D *plt3D = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getVariableLabels(), projectname + modelname +" - Q/Q/Q Loadings Plot", ScatterPlot3D::VARIABLELABELS);*/
  (*plot3D)->setPID(pid);
}

void PLSPlot::T_ScorePlotPrediction3D(ScatterPlot3D **plot3D)
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
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PLS T Score Plot Predicion").arg(projectname).arg(modelname), "t", "t", "t", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}


PLSPlot::PLSPlot(PROJECTS* projects_)
{
  pid = mid = predid = -1;
  projects = projects_;
}
