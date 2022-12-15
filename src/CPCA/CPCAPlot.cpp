#include "CPCAPlot.h"

void CPCAPlot::SuperScorePlot2D(ScatterPlot** plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();

  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getCPCAModel(mid)->Model()->super_scores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getCPCAModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst,
                              objnamelst,
                              &projects->value(pid)->getMATRIXList(),
                              xhash,
                              yhash,
                              &projects->value(pid)->getObjectLabels(),
                              &projects->value(pid)->getVariableLabels(),
                              "PC",
                              "PC",
                              QString(projectname + modelname + " - CPCA Super Score Plot"),
                              ScatterPlot::SCORES);
  (*plot2D)->setHotellingConfidenceEllipse(true);
  (*plot2D)->setPID(pid);
}

void CPCAPlot::SuperScorePlotPrediction2D(ScatterPlot **plot2D)
{
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  QList<matrix*> mxlst;
  mxlst.append(projects->value(pid)->getCPCAModel(mid)->Model()->super_scores);
  mxlst.append(projects->value(pid)->getCPCAModel(mid)->getCPCAPrediction(predid)->getPredSuperScores());
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getCPCAModel(mid)->getObjName());
  objnamelst.append(projects->value(pid)->getCPCAModel(mid)->getCPCAPrediction(predid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)->getCPCAModel(mid)->getCPCAPrediction(predid)->getDataHash());
  (*plot2D) = new ScatterPlot(mxlst,
                              objnamelst,
                              &projects->value(pid)->getMATRIXList(),
                              xhash,
                              yhash,
                              &projects->value(pid)->getObjectLabels(),
                              &projects->value(pid)->getVariableLabels(),
                              "PC",
                              "PC",
                              QString(projectname + modelname +" - CPCA Score Plot Prediction"),
                              ScatterPlot::SCORES);
  (*plot2D)->setHotellingConfidenceEllipse(true);
  (*plot2D)->setPID(pid);
}

QList< ScatterPlot * > CPCAPlot::BlockLoadingsPlot2D()
{
  QList< ScatterPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();

  LABELS varname = projects->value(pid)->getCPCAModel(mid)->getVarName();

  for(size_t i = 0; i < projects->value(pid)->getCPCAModel(mid)->Model()->block_loadings->order; i++){
    QList<matrix*> mxlst;
    mxlst.append(projects->value(pid)->getCPCAModel(mid)->Model()->block_loadings->m[i]);
    QList<QStringList> objnamelst;
    objnamelst.append(varname[i].objects);
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());

    plots.append(new ScatterPlot(mxlst,
                                objnamelst,
                                &projects->value(pid)->getMATRIXList(),
                                xhash,
                                yhash,
                                &projects->value(pid)->getObjectLabels(),
                                &projects->value(pid)->getVariableLabels(),
                                "PC",
                                "PC",
                                QString(projectname + modelname + " - CPCA Block %1 Loadings Plot").arg(i+1),
                                ScatterPlot::LOADINGS));
    plots.last()->setPID(pid);
    plots.last()->setMID(mid);
    plots.last()->setModelType(CPCA_);
  }
  return plots;
}

QList< ScatterPlot * > CPCAPlot::BlockScoresPlot2D()
{
  QList< ScatterPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  LABELS varname = projects->value(pid)->getCPCAModel(mid)->getVarName();

  for(size_t i = 0; i < projects->value(pid)->getCPCAModel(mid)->Model()->block_scores->order; i++){
    QList<matrix*> mxlst;
    mxlst.append(projects->value(pid)->getCPCAModel(mid)->Model()->block_scores->m[i]);
    QList<QStringList> objnamelst;
    objnamelst.append(varname[i].objects);
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());
    plots.append(new ScatterPlot(mxlst,
                                objnamelst,
                                &projects->value(pid)->getMATRIXList(),
                                xhash,
                                yhash,
                                &projects->value(pid)->getObjectLabels(),
                                &projects->value(pid)->getVariableLabels(),
                                "PC",
                                "PC",
                                QString(projectname + modelname + " - CPCA Block %1 Scores Plot").arg(i+1),
                                ScatterPlot::SCORES));
    plots.last()->setPID(pid);
    plots.last()->setMID(mid);
    plots.last()->setModelType(CPCA_);
  }
  return plots;
}

void CPCAPlot::ExpVarPlot(SimpleLine2DPlot **plot2D)
{
  QList< SimpleLine2DPlot* > plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  uint npc = projects->value(pid)->getCPCAModel(mid)->getNPC();
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, npc, 3);
  curvenames << "Explained Variance" << "Broken-Stick model";

  // set the X assis that is the principal component
  for(uint i = 0; i < npc; i++){
    m->data[i][0] = i+1;
  }

  //set the explained variance and the calculated broken-stick model calculated with npc as k!
  dvector *varexp = projects->value(pid)->getCPCAModel(mid)->Model()->total_expvar;
  int bsm = 1;
  for(uint i = 0; i < varexp->size; i++){
    m->data[i][1] = varexp->data[i];
    int d = bsm;
    for(uint j = 0; j < npc; j++){
      m->data[i][2] += 1/(float)(d+j);
    }
    m->data[i][2] /= npc;
    m->data[i][2] *= 100;
    bsm+=1;
  }

  #ifdef DEBUG
  qDebug() << "Final Matrix";
  PrintMatrix(m);
  #endif
  (*plot2D) = new SimpleLine2DPlot(m,
                                   curvenames,
                                   QString(" %1 - %2 - Explained Variance Plot").arg(projectname).arg(modelname),
                                   "PC",
                                   "Exp. Var.");
  DelMatrix(&m);
}


CPCAPlot::CPCAPlot(PROJECTS* projects_)
{
  pid = mid = predid = -1;
  projects = projects_;
}
