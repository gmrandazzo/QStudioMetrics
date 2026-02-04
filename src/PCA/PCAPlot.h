/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for pcaplot.
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

#ifndef PCAPLOT_H
#define PCAPLOT_H
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "../Plotlib/BarPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class PCAPlot {
public:
  explicit PCAPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }
  void setNLatentVariables(int nlv_) { nlv = nlv_; }
  void setGroups(LABELS g_) { g = g_; }
  /*ScatterPlot *ScorePlot2D();*/
  void ScorePlot2D(ScatterPlot **plot2D);
  void ScorePlotPrediction2D(ScatterPlot **plot2D);
  void ExpVarPlot(SimpleLine2DPlot **plot2D);
  void LoadingsMVANormDistrib(ScatterPlot **plot2D);
  void LoadingsPlot2D(ScatterPlot **plot2D);
  void DModXPlot(BarPlot **);
  void TsqContributionPlot(BarPlot **);
  void ScorePlot3D(ScatterPlot **plot3D);
  void ScorePlotPrediction3D(ScatterPlot **plot3D);
  void LoadingsPlot3D(ScatterPlot **plot3D);

private:
  PROJECTS *projects;
  int pid;
  int mid;
  int predid;
  int nlv;
  LABELS g;
};
#endif
