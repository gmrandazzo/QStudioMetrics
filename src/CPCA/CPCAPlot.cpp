/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for cpcaplot.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "CPCAPlot.h"
#include <memory>

void CPCAPlot::SuperScorePlot2D(ScatterPlot **plot2D) const {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();

  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getCPCAModel(mid)->Model()->super_scores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getCPCAModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());
  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "PC", "PC",
      QString(projectname + modelname + " - CPCA Super Score Plot"),
      ScatterPlot::SCORES);
  temp_plot->setHotellingConfidenceEllipse(true);
  temp_plot->setPID(pid);
  temp_plot->setImages(projects->value(pid)->getImages());
  *plot2D = temp_plot.release();
}

void CPCAPlot::SuperWeightsPlot2D(ScatterPlot **plot2D) const {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  LABELS varnames = projects->value(pid)->getCPCAModel(mid)->getVarName();

  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getCPCAModel(mid)->Model()->super_weights);

  QList<QStringList> objnamelst;
  objnamelst.append(QStringList());
  for (int i = 0; i < varnames.size(); i++) {
    objnamelst.last().append(varnames[i].name);
  }

  (*plot2D) = new ScatterPlot(
      mxlst, objnamelst, "PC", "PC",
      QString(projectname + modelname + " - CPCA Super Score Plot"));
  (*plot2D)->setPID(pid);
}

void CPCAPlot::SuperScorePlotPrediction2D(ScatterPlot **plot2D) const {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getCPCAModel(mid)->Model()->super_scores);
  mxlst.append(projects->value(pid)
                   ->getCPCAModel(mid)
                   ->getCPCAPrediction(predid)
                   ->getPredSuperScores());
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getCPCAModel(mid)->getObjName());
  objnamelst.append(projects->value(pid)
                        ->getCPCAModel(mid)
                        ->getCPCAPrediction(predid)
                        ->getObjName());
  QStringList xhash;
  QStringList yhash;
  xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)
                   ->getCPCAModel(mid)
                   ->getCPCAPrediction(predid)
                   ->getDataHash());
  (*plot2D) = new ScatterPlot(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "PC", "PC",
      QString(projectname + modelname + " - CPCA Score Plot Prediction"),
      ScatterPlot::SCORES);
  (*plot2D)->setHotellingConfidenceEllipse(true);
  (*plot2D)->setPID(pid);
  (*plot2D)->setImages(projects->value(pid)->getImages());
}

QList<ScatterPlot *> CPCAPlot::BlockScorePlotPrediction2D() {
  QList<ScatterPlot *> plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  size_t nobjs =
      projects->value(pid)->getCPCAModel(mid)->Model()->super_scores->row;
  size_t npc =
      projects->value(pid)->getCPCAModel(mid)->Model()->super_scores->col;
  size_t nblocks = projects->value(pid)->getCPCAModel(mid)->getVarName().size();
  size_t pred_nobjs = projects->value(pid)
                          ->getCPCAModel(mid)
                          ->getCPCAPrediction(predid)
                          ->getObjName()
                          .size();

  for (size_t k = 0; k < nblocks; k++) {
    QList<matrix *> mxlst;
    QList<QStringList> objnamelst;
    matrix *m, *p;

    NewMatrix(&m, nobjs, npc);
    for (size_t i = 0; i < nobjs; i++) {
      for (size_t j = 0; j < npc; j++) {
        m->data[i][j] = projects->value(pid)
                            ->getCPCAModel(mid)
                            ->Model()
                            ->block_scores->m[j]
                            ->data[i][k];
      }
    }
    mxlst.append(m);
    objnamelst.append(projects->value(pid)->getCPCAModel(mid)->getObjName());

    NewMatrix(&p, pred_nobjs, npc);
    for (size_t i = 0; i < pred_nobjs; i++) {
      for (size_t j = 0; j < npc; j++) {
        p->data[i][j] = projects->value(pid)
                            ->getCPCAModel(mid)
                            ->getCPCAPrediction(predid)
                            ->getPredBlockScores()
                            ->m[j]
                            ->data[i][k];
      }
    }
    mxlst.append(p);
    objnamelst.append(projects->value(pid)
                          ->getCPCAModel(mid)
                          ->getCPCAPrediction(predid)
                          ->getObjName());

    QStringList xhash;
    QStringList yhash;
    xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());
    plots.append(new ScatterPlot(
        mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), "PC", "PC",
        QString(projectname + modelname + " - CPCA Block %1 Scores Plot")
            .arg(k + 1),
        ScatterPlot::SCORES));
    plots.last()->setPID(pid);
    plots.last()->setMID(mid);
    plots.last()->setModelType(CPCA_);
    plots.last()->setImages(projects->value(pid)->getImages());
    DelMatrix(&m);
    DelMatrix(&p);
  }
  return plots;
}

QList<ScatterPlot *> CPCAPlot::BlockLoadingsPlot2D() {
  QList<ScatterPlot *> plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  LABELS varname = projects->value(pid)->getCPCAModel(mid)->getVarName();

  for (size_t i = 0;
       i <
       projects->value(pid)->getCPCAModel(mid)->Model()->block_loadings->order;
       i++) {
    QList<matrix *> mxlst;
    mxlst.append(
        projects->value(pid)->getCPCAModel(mid)->Model()->block_loadings->m[i]);
    QList<QStringList> objnamelst;
    objnamelst.append(varname[i].objects);
    QStringList xhash;
    QStringList yhash;
    xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());

    plots.append(new ScatterPlot(
        mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), "PC", "PC",
        QString(projectname + modelname + " - CPCA Block %1 Loadings Plot")
            .arg(i + 1),
        ScatterPlot::LOADINGS));
    plots.last()->setPID(pid);
    plots.last()->setMID(mid);
    plots.last()->setModelType(CPCA_);
  }
  return plots;
}

QList<ScatterPlot *> CPCAPlot::BlockScoresPlot2D() {
  QList<ScatterPlot *> plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  size_t nobjs =
      projects->value(pid)->getCPCAModel(mid)->Model()->super_scores->row;
  size_t npc =
      projects->value(pid)->getCPCAModel(mid)->Model()->super_scores->col;
  size_t nblocks = projects->value(pid)->getCPCAModel(mid)->getVarName().size();
  for (size_t k = 0; k < nblocks; k++) {
    QList<matrix *> mxlst;
    matrix *m;
    NewMatrix(&m, nobjs, npc);
    for (size_t i = 0; i < nobjs; i++) {
      for (size_t j = 0; j < npc; j++) {
        m->data[i][j] = projects->value(pid)
                            ->getCPCAModel(mid)
                            ->Model()
                            ->block_scores->m[j]
                            ->data[i][k];
      }
    }

    mxlst.append(m);
    QList<QStringList> objnamelst;
    objnamelst.append(projects->value(pid)->getCPCAModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getCPCAModel(mid)->getDataHash());
    plots.append(new ScatterPlot(
        mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), "PC", "PC",
        QString(projectname + modelname + " - CPCA Block %1 Scores Plot")
            .arg(k + 1),
        ScatterPlot::SCORES));
    plots.last()->setPID(pid);
    plots.last()->setMID(mid);
    plots.last()->setModelType(CPCA_);
    plots.last()->setImages(projects->value(pid)->getImages());
    DelMatrix(&m);
  }
  return plots;
}

void CPCAPlot::ExpVarPlot(SimpleLine2DPlot **plot2D) {
  QList<SimpleLine2DPlot *> plots;
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
  uint npc = projects->value(pid)->getCPCAModel(mid)->getNPC();
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
  dvector *varexp =
      projects->value(pid)->getCPCAModel(mid)->Model()->total_expvar;
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
  (*plot2D) = new SimpleLine2DPlot(m, curvenames,
                                   QString(" %1 - %2 - Explained Variance Plot")
                                       .arg(projectname)
                                       .arg(modelname),
                                   "PC", "Exp. Var.");
  DelMatrix(&m);
}

CPCAPlot::CPCAPlot(PROJECTS *projects_) {
  pid = mid = predid = -1;
  projects = projects_;
}
