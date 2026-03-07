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

#ifndef CPCAPLOT_H
#define CPCAPLOT_H
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class CPCAPlot {
public:
  explicit CPCAPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }
  void setGroups(LABELS g_) { g = g_; }
  void SuperScorePlot2D(ScatterPlot **plot2D) const;
  void SuperWeightsPlot2D(ScatterPlot **plot2D) const;
  void SuperScorePlotPrediction2D(ScatterPlot **plot2D) const;
  QList<ScatterPlot *> BlockScorePlotPrediction2D();
  QList<ScatterPlot *> BlockScoresPlot2D();
  QList<ScatterPlot *> BlockLoadingsPlot2D();
  void ExpVarPlot(SimpleLine2DPlot **plot2D);

private:
  PROJECTS *projects;
  int pid, mid, predid;
  LABELS g;
};
#endif
