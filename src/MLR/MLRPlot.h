/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for mlrplot.
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

#ifndef MLRPLOT_H
#define MLRPLOT_H

#include "../Plotlib/BarPlot.h"
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class MLRPlot {
public:
  explicit MLRPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }

  QList<BarPlot *> BetaCoefficients();
  void PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D);
  void RecalcVSExperimentalAndPrediction(ScatterPlot **plot2D);
  void RecalcVSExperimental(ScatterPlot **plot2D);
  void RecalcResidualsVSExperimental(ScatterPlot **plot2D);
  void PredictedVSExperimental(ScatterPlot **plot2D);
  void PredictedResidualsVSExperimental(ScatterPlot **plot2D);
  QList<ScatterPlot *> YScramblingPlot();

private:
  PROJECTS *projects;
  int pid, mid, predid;

  QString getProjectName() const;
  QString getModelName() const;
  MLRModel *getMLRModel() const;
  void extractExperimentalY(int matrix_id, const QStringList &objsel,
                            const QStringList &yvarsel, matrix **out_y);
};

#endif
