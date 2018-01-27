#include "PCAPlot.h"

void PCAPlot::ScorePlot2D(ScatterPlot2D** plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->scores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPCAModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst,  &projects->value(pid)->getMATRIXList(), xhash, yhash,  &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "PC", "PC", QString(projectname + modelname + " - PCA Score Plot"), ScatterPlot2D::SCORES);
  (*plot2D)->setHotellingConfidenceEllipse(true);
  (*plot2D)->setPID(pid);
}

void PCAPlot::ScorePlotPrediction2D(ScatterPlot2D **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->scores);
  mxlst.append(projects->value(pid)->getPCAModel(mid)->getPCAPrediction(predid)->getPredScores());
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPCAModel(mid)->getObjName());
  objnamelst.append(projects->value(pid)->getPCAModel(mid)->getPCAPrediction(predid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)->getPCAModel(mid)->getPCAPrediction(predid)->getDataHash());
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "PC", "PC", QString(projectname + modelname +" - PCA Score Plot Prediction"), ScatterPlot2D::SCORES);
  (*plot2D)->setHotellingConfidenceEllipse(true);
  (*plot2D)->setPID(pid);
}

void PCAPlot::LoadingsPlot2D(ScatterPlot2D **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->loadings);
  QList<QStringList> objnamelst;
  QStringList varname = projects->value(pid)->getPCAModel(mid)->getVarName();
  objnamelst.append(varname);

  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());

  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "PC", "PC",  projectname + modelname + " - PCA Loadings Plot", ScatterPlot2D::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PCA_);
}

void PCAPlot::LoadingsMVANormDistrib(ScatterPlot2D **plot2D)
{
  if(mid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

    QList<matrix*> mx, my;

    /*mx.append(projects->value(pid)->getPCAModel(mid)->Model()->loadings);*/

    QList<QStringList> objnamelst;
    QStringList varname = projects->value(pid)->getPCAModel(mid)->getVarName();

    /*
    objnamelst.append(varname);

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());

    matrix *loading_mvnd;
    dvector *mean, *sdev;

    NewMatrix(&loading_mvnd, projects->value(pid)->getPCAModel(mid)->Model()->loadings->row, projects->value(pid)->getPCAModel(mid)->Model()->loadings->col);
    initDVector(&mean);
    initDVector(&sdev);

    MatrixColSDEV(projects->value(pid)->getPCAModel(mid)->Model()->loadings, &sdev);
    MatrixColAverage(projects->value(pid)->getPCAModel(mid)->Model()->loadings, &mean);

    for(uint j = 0; j < projects->value(pid)->getPCAModel(mid)->Model()->loadings->col; j++){
      for(uint i = 0; i < projects->value(pid)->getPCAModel(mid)->Model()->loadings->row; i++){
        loading_mvnd->data[i][j] = 1./sqrt(2 * pi* sdev->data[j]) * exp(-square((projects->value(pid)->getPCAModel(mid)->Model()->loadings->data[i][j] - mean->data[j])/sdev->data[j])/2.f);
      }
    }
    DelDVector(&mean);
    DelDVector(&sdev);

    my.append(loading_mvnd);
    */

    /*tyemporaneo per test*/
    tensor *classvar;


    NewTensor(&classvar, g.size());


    QList< QList< int > > clsid;

    for(int i = 0; i < g.size(); i++){
      clsid.append(QList<int>());
      objnamelst.append(QStringList());
      for(int j = 0; j < g[i].objects.size(); j++){
        int indx = varname.indexOf(g[i].objects[j]);
        if(indx > -1){
          clsid.last().append(indx);
          objnamelst.last().append(g[i].objects[j]);
        }
        else{
          continue;
        }
      }
    }

    for(int i = 0; i < g.size(); i++){
      NewTensorMatrix(&classvar, i, clsid[i].size(), projects->value(pid)->getPCAModel(mid)->Model()->loadings->col);
    }

    for(int k = 0; k < clsid.size(); k++){
      for(int i = 0; i < clsid[k].size(); i++){
        for(uint j = 0; j < projects->value(pid)->getPCAModel(mid)->Model()->loadings->col; j++){
          classvar->m[k]->data[i][j] = projects->value(pid)->getPCAModel(mid)->Model()->loadings->data[clsid[k][i]][j];
        }
      }
    }

    dvector *mean, *sdev;
    for(uint k = 0; k < classvar->order; k++){
      initDVector(&mean);
      initDVector(&sdev);

      MatrixColSDEV(classvar->m[k], &sdev);
      MatrixColAverage(classvar->m[k], &mean);

      mx.append(classvar->m[k]);
      my.append(new matrix);
      NewMatrix(&my.last(), classvar->m[k]->row, classvar->m[k]->col);

      for(uint j = 0; j < classvar->m[k]->col; j++){
        for(uint i = 0; i < classvar->m[k]->row; i++){
          my[k]->data[i][j] = 1./sqrt(2 * _pi_* sdev->data[j]) * exp(-square((classvar->m[k]->data[i][j] - mean->data[j])/sdev->data[j])/2.f);
        }
      }

      DelDVector(&mean);
      DelDVector(&sdev);
    }

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());

    (*plot2D) = new ScatterPlot2D(mx, my, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,  &projects->value(pid)->getVariableTabLabels(), &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "PC", "Prob PC", projectname + modelname + " - PCA Loadings Multivariate Normal Distribution", ScatterPlot2D::LOADINGS);
    (*plot2D)->setPID(pid);
    DelTensor(&classvar);
  }

}


void PCAPlot::ScorePlot3D(ScatterPlot3D **plot3D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->scores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPCAModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PCA Score Plot").arg(projectname).arg(modelname), "PC", "PC", "PC", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}

void PCAPlot::LoadingsPlot3D(ScatterPlot3D **plot3D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPCAModel(mid)->getVarName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->loadings);
  QList<QStringList> objname;
  objname.append(varname);
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PCA Loadings Plot").arg(projectname).arg(modelname), "PC", "PC", "PC", ScatterPlot3D::LOADINGS);
  (*plot3D)->setPID(pid);
}

void PCAPlot::ScorePlotPrediction3D(ScatterPlot3D **plot3D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->scores);
  mxlst.append(projects->value(pid)->getPCAModel(mid)->getPCAPrediction(predid)->getPredScores());
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPCAModel(mid)->getObjName());
  objname.append(projects->value(pid)->getPCAModel(mid)->getPCAPrediction(predid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)->getPCAModel(mid)->getPCAPrediction(predid)->getDataHash());
  (*plot3D) = new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - PCA Score Plot Prediction").arg(projectname).arg(modelname), "PC", "PC", "PC", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}

PCAPlot::PCAPlot(PROJECTS* projects_)
{
  pid = mid = predid = -1;
  projects = projects_;
}
