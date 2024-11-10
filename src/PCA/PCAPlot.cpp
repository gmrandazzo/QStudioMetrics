#include "PCAPlot.h"
#include <memory>

void PCAPlot::ScorePlot2D(ScatterPlot **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->scores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPCAModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(),
      xhash, yhash, &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "PC", "PC",
      QString(projectname + modelname + " - PCA Score Plot"),
      ScatterPlot::SCORES);
  
  temp_plot->setHotellingConfidenceEllipse(true);
  temp_plot->setPID(pid);
  *plot2D = temp_plot.release();
}

void PCAPlot::ScorePlotPrediction2D(ScatterPlot **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->scores);
  mxlst.append(projects->value(pid)
                   ->getPCAModel(mid)
                   ->getPCAPrediction(predid)
                   ->getPredScores());
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getPCAModel(mid)->getObjName());
  objnamelst.append(projects->value(pid)
                        ->getPCAModel(mid)
                        ->getPCAPrediction(predid)
                        ->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)
                   ->getPCAModel(mid)
                   ->getPCAPrediction(predid)
                   ->getDataHash());
  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "PC", "PC",
      QString(projectname + modelname + " - PCA Score Plot Prediction"),
      ScatterPlot::SCORES);
  temp_plot->setHotellingConfidenceEllipse(true);
  temp_plot->setPID(pid);
  *plot2D = temp_plot.release();
}

void PCAPlot::LoadingsPlot2D(ScatterPlot **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->loadings);
  QList<QStringList> objnamelst;
  QStringList varname = projects->value(pid)->getPCAModel(mid)->getVarName();
  objnamelst.append(varname);

  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());

  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "PC", "PC",
      projectname + modelname + " - PCA Loadings Plot", ScatterPlot::LOADINGS);
  temp_plot->setHotellingConfidenceEllipse(true);
  temp_plot->setPID(pid);
  temp_plot->setMID(mid);
  temp_plot->setModelType(PCA_);
  *plot2D = temp_plot.release();
}

void PCAPlot::TsqContributionPlot(BarPlot **bar_plots) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QStringList objnames = projects->value(pid)->getPCAModel(mid)->getObjName();
  QStringList varnames = projects->value(pid)->getPCAModel(mid)->getVarName();

  int did = projects->value(pid)->getPCAModel(mid)->getDID();
  if (nlv > projects->value(pid)->getPCAModel(mid)->getNPC()) {
    nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
  }

  /*
   * Reconstruct the original matrix using nlv latent variables
   */
  matrix *reconstructed_mx;
  initMatrix(&reconstructed_mx);
  PCAIndVarPredictor(
    projects->value(pid)->getPCAModel(mid)->Model()->scores,
    projects->value(pid)->getPCAModel(mid)->Model()->loadings,
    projects->value(pid)->getPCAModel(mid)->Model()->colaverage,
    projects->value(pid)->getPCAModel(mid)->Model()->colscaling,
    nlv,
    reconstructed_mx
  );

  const matrix *orig_x = projects->value(pid)->getMatrix(did)->Matrix();

  /*
  * Calculate SPE (Squared Prediction Error) and SPE contributions in one pass
  */
  std::vector<double> spe;
  QList<dvector *> spe_contributions;
  for (size_t i = 0; i < orig_x->row; i++) {
      double sum_squared_diff = 0.0;
      spe_contributions.append(new dvector);
      NewDVector(&spe_contributions.last(), orig_x->col);
      for (size_t j = 0; j < orig_x->col; j++) {
          double diff = orig_x->data[i][j] - reconstructed_mx->data[i][j];
          double squared_diff = diff * diff;
          sum_squared_diff += squared_diff;
          spe_contributions.last()->data[j] = squared_diff;
      }
      spe.push_back(sum_squared_diff);
  }

  QStringList windowtitles;
  for (size_t i = 0; i < orig_x->row; i++){
    windowtitles.append(QString("%1 - Sample %2 -  Total SPE = %3").arg(projectname).arg(objnames[i]).arg(QString::number(spe[i], 'f', 4)));
  }

  auto temp_plot = std::make_unique<BarPlot>(
        spe_contributions,
        windowtitles,
        "Features",
        "Contribution to SPE",
        varnames);

  for (dvector* contribution : spe_contributions) {
    DelDVector(&contribution);
  }
  spe_contributions.clear();
  spe.clear();
  DelMatrix(&reconstructed_mx);
  *bar_plots = temp_plot.release();
}

void PCAPlot::ExpVarPlot(SimpleLine2DPlot **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  uint npc = projects->value(pid)->getPCAModel(mid)->getNPC();
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, npc, 3);
  curvenames << "Explained Variance"
             << "Broken-Stick model";

  // set the X assis that is the principal component
  for (uint i = 0; i < npc; i++) {
    m->data[i][0] = i + 1;
  }

  // set the explained variance and the calculated broken-stick model calculated
  // with npc as k!
  dvector *varexp = projects->value(pid)->getPCAModel(mid)->Model()->varexp;
  int bsm = 1;
  for (uint i = 0; i < varexp->size; i++) {
    m->data[i][1] = varexp->data[i];
    int d = bsm;
    for (uint j = 0; j < npc; j++) {
      m->data[i][2] += 1 / (float)(d + j);
    }
    m->data[i][2] /= npc;
    m->data[i][2] *= 100;
    bsm += 1;
  }

#ifdef DEBUG
  qDebug() << "Final Matrix";
  PrintMatrix(m);
#endif
  auto temp_plot = std::make_unique<SimpleLine2DPlot>(
    m,
    curvenames,
    QString(" %1 - %2 - Explained Variance Plot").arg(projectname).arg(modelname),
    "PC", "Exp. Var.");
  DelMatrix(&m);
  *plot2D = temp_plot.release();
}

void PCAPlot::LoadingsMVANormDistrib(ScatterPlot **plot2D) {
  if (mid > -1) {
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

    QList<matrix *> mx, my;

    /*mx.append(projects->value(pid)->getPCAModel(mid)->Model()->loadings);*/

    QList<QStringList> objnamelst;
    QStringList varname = projects->value(pid)->getPCAModel(mid)->getVarName();

    /*
    objnamelst.append(varname);

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());

    matrix *loading_mvnd;
    dvector *mean, *sdev;

    NewMatrix(&loading_mvnd,
    projects->value(pid)->getPCAModel(mid)->Model()->loadings->row,
    projects->value(pid)->getPCAModel(mid)->Model()->loadings->col);
    initDVector(&mean);
    initDVector(&sdev);

    MatrixColSDEV(projects->value(pid)->getPCAModel(mid)->Model()->loadings,
    &sdev);
    MatrixColAverage(projects->value(pid)->getPCAModel(mid)->Model()->loadings,
    &mean);

    for(uint j = 0; j <
    projects->value(pid)->getPCAModel(mid)->Model()->loadings->col; j++){
      for(uint i = 0; i <
    projects->value(pid)->getPCAModel(mid)->Model()->loadings->row; i++){
        loading_mvnd->data[i][j] = 1./sqrt(2 * pi* sdev->data[j]) *
    exp(-square((projects->value(pid)->getPCAModel(mid)->Model()->loadings->data[i][j]
    - mean->data[j])/sdev->data[j])/2.f);
      }
    }
    DelDVector(&mean);
    DelDVector(&sdev);

    my.append(loading_mvnd);
    */

    /*tyemporaneo per test*/
    tensor *classvar;

    NewTensor(&classvar, g.size());

    QList<QList<int>> clsid;

    for (int i = 0; i < g.size(); i++) {
      clsid.append(QList<int>());
      objnamelst.append(QStringList());
      for (int j = 0; j < g[i].objects.size(); j++) {
        int indx = varname.indexOf(g[i].objects[j]);
        if (indx > -1) {
          clsid.last().append(indx);
          objnamelst.last().append(g[i].objects[j]);
        } else {
          continue;
        }
      }
    }

    for (int i = 0; i < g.size(); i++) {
      NewTensorMatrix(
          classvar, i, clsid[i].size(),
          projects->value(pid)->getPCAModel(mid)->Model()->loadings->col);
    }

    for (int k = 0; k < clsid.size(); k++) {
      for (int i = 0; i < clsid[k].size(); i++) {
        for (uint j = 0;
             j < projects->value(pid)->getPCAModel(mid)->Model()->loadings->col;
             j++) {
          classvar->m[k]->data[i][j] = projects->value(pid)
                                           ->getPCAModel(mid)
                                           ->Model()
                                           ->loadings->data[clsid[k][i]][j];
        }
      }
    }

    dvector *mean, *sdev;
    for (uint k = 0; k < classvar->order; k++) {
      initDVector(&mean);
      initDVector(&sdev);

      MatrixColSDEV(classvar->m[k], sdev);
      MatrixColAverage(classvar->m[k], mean);

      mx.append(classvar->m[k]);
      my.append(new matrix);
      NewMatrix(&my.last(), classvar->m[k]->row, classvar->m[k]->col);

      for (uint j = 0; j < classvar->m[k]->col; j++) {
        for (uint i = 0; i < classvar->m[k]->row; i++) {
          my[k]->data[i][j] =
              1. / sqrt(2 * _pi_ * sdev->data[j]) *
              exp(-square((classvar->m[k]->data[i][j] - mean->data[j]) /
                          sdev->data[j]) /
                  2.f);
        }
      }

      DelDVector(&mean);
      DelDVector(&sdev);
    }

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());

    auto temp_plot = std::make_unique<ScatterPlot>(
        mx, my, objnamelst, &projects->value(pid)->getMATRIXList(), xhash,
        yhash, &projects->value(pid)->getVariableTabLabels(),
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), "PC", "Prob PC",
        projectname + modelname +
            " - PCA Loadings Multivariate Normal Distribution",
        ScatterPlot::LOADINGS);
    temp_plot->setPID(pid);
    DelTensor(&classvar);
    *plot2D = temp_plot.release();
  }
}

void PCAPlot::ScorePlot3D(ScatterPlot **plot3D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->scores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPCAModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "PC", "PC", "PC",
      QString("%1 - %2 - PCA Score Plot").arg(projectname).arg(modelname),
      ScatterPlot::SCORES);
  temp_plot->setPID(pid);
  *plot3D = temp_plot.release();

}

void PCAPlot::LoadingsPlot3D(ScatterPlot **plot3D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPCAModel(mid)->getVarName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->loadings);
  QList<QStringList> objname;
  objname.append(varname);
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "PC", "PC", "PC",
      QString("%1 - %2 - PCA Loadings Plot").arg(projectname).arg(modelname),
      ScatterPlot::LOADINGS);
  temp_plot->setPID(pid);
  *plot3D = temp_plot.release();
}

void PCAPlot::ScorePlotPrediction3D(ScatterPlot **plot3D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getPCAModel(mid)->Model()->scores);
  mxlst.append(projects->value(pid)
                   ->getPCAModel(mid)
                   ->getPCAPrediction(predid)
                   ->getPredScores());
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getPCAModel(mid)->getObjName());
  objname.append(projects->value(pid)
                     ->getPCAModel(mid)
                     ->getPCAPrediction(predid)
                     ->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getPCAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)
                   ->getPCAModel(mid)
                   ->getPCAPrediction(predid)
                   ->getDataHash());
  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "PC", "PC", "PC",
      QString("%1 - %2 - PCA Score Plot Prediction")
          .arg(projectname)
          .arg(modelname),
      ScatterPlot::SCORES);
   temp_plot->setPID(pid);
  *plot3D = temp_plot.release();
}

PCAPlot::PCAPlot(PROJECTS *projects_) {
  pid = mid = predid = -1;
  projects = projects_;
}
