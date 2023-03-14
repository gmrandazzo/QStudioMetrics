#include "MLRPlot.h"
#include "scientific.h"

void MLRPlot::RecalcVSExperimentalAndPrediction(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getMLRModel(mid)->getName();

  QStringList varname = projects->value(pid)->getMLRModel(mid)->getYVarName();

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getMLRModel(mid)->getDataHash());

  if(did > -1 && predid > -1){
    int pred_id = projects->value(pid)->getMatrixID(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getDataHash());
    if(pred_id > -1){
      QStringList objsel = projects->value(pid)->getMLRModel(mid)->getObjName();
      QStringList yvarsel = projects->value(pid)->getMLRModel(mid)->getYVarName();
      auto nobjects = objsel.size();
      auto nvars = yvarsel.size();
      matrix *recalc_y;
      initMatrix(&recalc_y);
      MatrixCopy(projects->value(pid)->getMLRModel(mid)->Model()->recalculated_y, &recalc_y);

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
      QStringList pred_objsel = projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getObjName();
      uint pred_nobjects = pred_objsel.size();
      matrix *pred_y;
      initMatrix(&pred_y);
      MatrixCopy(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getYDipVar(), &pred_y);

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
      objname.append(projects->value(pid)->getMLRModel(mid)->getObjName());
      objname.append(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
      xhash.append(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getDataHash());
      yhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getDataHash());
      (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental Y"), QString("Recalculated and Predicted Y"), projectname + modelname + " - MLR Recalc VS Experimental Plot", ScatterPlot::SCORES);
      DelMatrix(&recalc_y);
      (*plot2D)->BuildDiagonal();
      (*plot2D)->setPID(pid);
      DelMatrix(&y);
      DelMatrix(&real_pred_y);
      DelMatrix(&pred_y);
    }
  }
}

void MLRPlot::PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getMLRModel(mid)->getName();
  QStringList varname = projects->value(pid)->getMLRModel(mid)->getYVarName();
  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getMLRModel(mid)->getDataHash());

  if(did > -1 && predid > -1){

    int pred_id = projects->value(pid)->getMatrixID(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getDataHash()); // used to get real y when find real y

    if(pred_id > -1){
      QStringList model_objsel = projects->value(pid)->getMLRModel(mid)->getObjName();
      QStringList yvarsel = projects->value(pid)->getMLRModel(mid)->getYVarName();
      uint nobjects = model_objsel.size();
      uint nvars = yvarsel.size();
      matrix *model_predicted_y;
      initMatrix(&model_predicted_y);
      MatrixCopy(projects->value(pid)->getMLRModel(mid)->Model()->predicted_y, &model_predicted_y);

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
      QStringList pred_objsel = projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getObjName();
      uint pred_nobjects = pred_objsel.size();
      matrix *pred_predicted_y;
      initMatrix(&pred_predicted_y);

      MatrixCopy(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getYDipVar(), &pred_predicted_y);

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
      objname.append(projects->value(pid)->getMLRModel(mid)->getObjName());
      objname.append(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
      xhash.append(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getDataHash());
      yhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getMLRModel(mid)->getMLRPrediction(predid)->getDataHash());
      (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), QString(projectname + modelname + " - MLR Recalc VS Experimental Plot"), ScatterPlot::SCORES);
      (*plot2D)->setAxisNameExtensions(varname);
      (*plot2D)->BuildDiagonal();
      (*plot2D)->setPID(pid);
      DelMatrix(&model_exp_y);
      DelMatrix(&model_predicted_y);
      DelMatrix(&pred_exp_y);
      DelMatrix(&pred_predicted_y);
    }
  }
}

QList< BarPlot* > MLRPlot::BetaCoefficients()
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getMLRModel(mid)->getName();
  QStringList varnames = projects->value(pid)->getMLRModel(mid)->getXVarName();

  matrix *betas = projects->value(pid)->getMLRModel(mid)->Model()->b;
  QList< BarPlot *> barplots;
  for(size_t j = 0; j < betas->col; j++){
    dvector *v = getMatrixColumn(betas, j);
    barplots.append(new BarPlot(v, varnames, QString("Betas Model - %1").arg(projects->value(pid)->getMLRModel(mid)->getName()), "Betas", "Variables"));
    DelDVector(&v);
  }
  return barplots;
}

void MLRPlot::RecalcVSExperimental(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getMLRModel(mid)->getName();
  QStringList varname = projects->value(pid)->getMLRModel(mid)->getYVarName();
  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getMLRModel(mid)->getDataHash());

  QStringList objsel = projects->value(pid)->getMLRModel(mid)->getObjName();
  QStringList yvarsel = projects->value(pid)->getMLRModel(mid)->getYVarName();
  uint nobjects = objsel.size();
  uint nvars = yvarsel.size();
  matrix *model_recalc_y;
  initMatrix(&model_recalc_y);
  MatrixCopy(projects->value(pid)->getMLRModel(mid)->Model()->recalculated_y, &model_recalc_y);

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
  objname.append( projects->value(pid)->getMLRModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
  yhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated"), projectname + modelname + " - MLR Recalculated VS Experimental Plot", ScatterPlot::SCORES);
  (*plot2D)->BuildDiagonal();
  (*plot2D)->setPID(pid);
  (*plot2D)->setAxisNameExtensions(varname);
  DelMatrix(&model_recalc_y);
  DelMatrix(&model_exp_y);
}

void MLRPlot::RecalcResidualsVSExperimental(ScatterPlot** plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getMLRModel(mid)->getName();
  QStringList varname = projects->value(pid)->getMLRModel(mid)->getYVarName();

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getMLRModel(mid)->getDataHash());

  if(did > -1){
    QStringList objsel = projects->value(pid)->getMLRModel(mid)->getObjName();
    QStringList yvarsel = projects->value(pid)->getMLRModel(mid)->getYVarName();
    uint nobjects = objsel.size();
    uint nvars = yvarsel.size();
    matrix *recalc_res;
    initMatrix(&recalc_res);
    MatrixCopy(projects->value(pid)->getMLRModel(mid)->Model()->recalc_residuals, &recalc_res);

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
    objname.append( projects->value(pid)->getMLRModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
    yhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Recalculated Residuals"), projectname + modelname + " - MLR Experimental VS Recalculated Residuals Y Plot", ScatterPlot::SCORES);
    DelMatrix(&recalc_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
  }
}


void MLRPlot::PredictedVSExperimental(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getMLRModel(mid)->getName();
  QStringList varname = projects->value(pid)->getMLRModel(mid)->getYVarName();
  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getMLRModel(mid)->getDataHash());

  if(did > -1){
    QStringList objsel = projects->value(pid)->getMLRModel(mid)->getObjName();
    QStringList yvarsel = projects->value(pid)->getMLRModel(mid)->getYVarName();
    uint nobjects = objsel.size();
    uint nvars = yvarsel.size();
    matrix *model_pred_y;
    initMatrix(&model_pred_y);
    MatrixCopy(projects->value(pid)->getMLRModel(mid)->Model()->predicted_y, &model_pred_y);

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
    objname.append( projects->value(pid)->getMLRModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
    yhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted"), projectname + modelname + " - MLR Predicted VS Experimental Plot", ScatterPlot::SCORES);
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
    DelMatrix(&model_pred_y);
  }
}

void MLRPlot::PredictedResidualsVSExperimental(ScatterPlot** plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getMLRModel(mid)->getName();
  QStringList varname = projects->value(pid)->getMLRModel(mid)->getYVarName();


  // Get Prediction
  int did = projects->value(pid)->getMatrixID(projects->value(pid)->getMLRModel(mid)->getDataHash());

  if(did > -1){
    QStringList objsel = projects->value(pid)->getMLRModel(mid)->getObjName();
    QStringList yvarsel = projects->value(pid)->getMLRModel(mid)->getYVarName();
    uint nobjects = objsel.size();
    uint nvars = yvarsel.size();
    matrix *pred_res;
    initMatrix(&pred_res);
    MatrixCopy(projects->value(pid)->getMLRModel(mid)->Model()->pred_residuals, &pred_res);

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
    objname.append( projects->value(pid)->getMLRModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
    yhash.append(projects->value(pid)->getMLRModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("Experimental"), QString("Predicted Residuals"), projectname + modelname + " - MLR Experimental VS Predicted Residuals Y Plot", ScatterPlot::SCORES);
    DelMatrix(&pred_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(varname);
    DelMatrix(&model_exp_y);
  }
}

QList<ScatterPlot*> MLRPlot::YScramblingPlot()
{
  QList<ScatterPlot*> plots2D;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getMLRModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(new matrix);
  size_t nobj = projects->value(pid)->getMLRModel(mid)->Model()->r2q2scrambling->row;
  size_t ndepvar = (projects->value(pid)->getMLRModel(mid)->Model()->r2q2scrambling->col-1)/2;
  NewMatrix(&mxlst.last(), nobj, 2); // R2
  mxlst.append(new matrix);
  NewMatrix(&mxlst.last(), nobj, 2); // Q2
  QList<QStringList> objnamelst;
  objnamelst.append(QStringList());
  objnamelst.append(QStringList());
  /*set the X Constant */
  for(size_t i = 0; i < nobj; i++){
    mxlst[0]->data[i][0] = projects->value(pid)->getMLRModel(mid)->Model()->r2q2scrambling->data[i][0];
    mxlst[1]->data[i][0] = projects->value(pid)->getMLRModel(mid)->Model()->r2q2scrambling->data[i][0];
    objnamelst[0].append("R2");
    objnamelst[1].append("Q2");
  }

  for(size_t j = 0; j < ndepvar; j++){
    for(size_t i = 0; i < nobj; i++){
      mxlst[0]->data[i][1] = projects->value(pid)->getMLRModel(mid)->Model()->r2q2scrambling->data[i][j+1];
      mxlst[1]->data[i][1] = projects->value(pid)->getMLRModel(mid)->Model()->r2q2scrambling->data[i][j+1+ndepvar];
    }
    plots2D.append(new ScatterPlot(mxlst, objnamelst,  "Correlation with Y Real Vector", "R2/Q2", QString("MLR Y Scrambling Plot")));
    plots2D.last()->setPID(pid);
  }

  DelMatrix(&mxlst[1]);
  DelMatrix(&mxlst[0]);
  mxlst.clear();
  return plots2D;
}

MLRPlot::MLRPlot(PROJECTS* projects_)
{
  pid = mid = predid = -1;
  projects = projects_;
}
