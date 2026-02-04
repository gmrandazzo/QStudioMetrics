/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for uplsplot.
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

#ifndef UPLSPLOT_H
#define UPLSPLOT_H
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/ScatterPlot3D.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class UPLSPlot {
public:
  UPLSPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }
  void setNPrincipalComponent(int npc_) { npc = npc_; }

  void TU_Plot(ScatterPlot2D **plot2D);
  void T_ScorePlot2D(ScatterPlot2D **plot2D);
  void U_ScorePlot2D(ScatterPlot2D **plot2D);
  void T_ScorePlotPrediction2D(ScatterPlot2D **plot2D);
  void P_LoadingsPlot2D(QList<ScatterPlot2D *> *plot2D);
  void Q_LoadingsPlot2D(QList<ScatterPlot2D *> *plot2D);
  void WeightsPlot2D(QList<ScatterPlot2D *> *plot2D);
  void T_ScorePlot3D(ScatterPlot3D **plot3D);
  void U_ScorePlot3D(ScatterPlot3D **plot3D);
  void T_ScorePlotPrediction3D(ScatterPlot3D **plot3D);
  void P_LoadingsPlot3D(QList<ScatterPlot3D *> *plot3D);
  void Q_LoadingsPlot3D(QList<ScatterPlot3D *> *plot3D);
  void WeightsPlot3D(QList<ScatterPlot3D *> *plot3D);
  void RecalcVSExperimentalAndPrediction(QList<ScatterPlot2D *> *plot2D);
  void PredictedVSExperimentalAndPrediction(QList<ScatterPlot2D *> *plot2D);
  void RecalcVSExperimental(QList<ScatterPlot2D *> *plot2D);
  void RecalcResidualsVSExperimental(QList<ScatterPlot2D *> *plot2D);
  void PredictedVSExperimental(QList<ScatterPlot2D *> *plot2D);
  void PredictedResidualsVSExperimental(QList<ScatterPlot2D *> *plot2D);
  void R2Q2(QList<SimpleLine2DPlot *> *plot2D);
  void R2R2Prediction(QList<SimpleLine2DPlot *> *plot2D);

private:
  PROJECTS *projects;
  int pid, mid, predid, npc;
};
#endif
