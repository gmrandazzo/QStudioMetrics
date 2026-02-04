/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for upcaplot.
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

#ifndef UPCAPLOT_H
#define UPCAPLOT_H
#include "../Plotlib/ScatterPlot2D.h"
#include "../Plotlib/ScatterPlot3D.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class UPCAPlot {
public:
  UPCAPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }
  void ScorePlot2D(ScatterPlot2D **plot2D);
  void ScorePlotPrediction2D(ScatterPlot2D **plot2D);
  void LoadingsPlot2D(QList<ScatterPlot2D *> *plot2D);
  void ScorePlot3D(ScatterPlot3D **plot3D);
  void ScorePlotPrediction3D(ScatterPlot3D **plot3D);
  void LoadingsPlot3D(QList<ScatterPlot3D *> *plot3D);

private:
  PROJECTS *projects;
  int pid, mid, predid;
};
#endif
