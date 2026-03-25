/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for plsplot.
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

#ifndef PLSPLOT_H
#define PLSPLOT_H

#include "../Plotlib/BarPlot.h"
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class PLSPlot {
public:
  explicit PLSPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }
  void setNLatentVariables(int nlv_) { nlv = nlv_; }

  void TU_Plot(ScatterPlot **plot2D);
  void T_ScorePlot2D(ScatterPlot **plot2D);
  void U_ScorePlot2D(ScatterPlot **plot2D);
  void T_ScorePlotPrediction2D(ScatterPlot **plot2D);
  void P_LoadingsPlot2D(ScatterPlot **plot2D);
  void Q_LoadingsPlot2D(ScatterPlot **plot2D);
  void PQ_LoadingsPlot2D(ScatterPlot **plot2D);
  void WeightsPlot2D(ScatterPlot **plot2D);
  void BetaCoefficients(BarPlot **betas_barplot);
  void BetaCoefficientsDurbinWatson(SimpleLine2DPlot **dw_betas_plot);
  void T_ScorePlot3D(ScatterPlot **plot3D);
  void U_ScorePlot3D(ScatterPlot **plot3D);
  void T_ScorePlotPrediction3D(ScatterPlot **plot3D);
  void P_LoadingsPlot3D(ScatterPlot **plot3D);
  void Q_LoadingsPlot3D(ScatterPlot **plot3D);
  void WeightsPlot3D(ScatterPlot **plot3D);
  void PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D);
  void RecalcVSExperimentalAndPrediction(ScatterPlot **plot2D);
  void RecalcVSExperimental(ScatterPlot **plot2D);
  void ClassRecalcVSExperimental(QList<QStringList> *cellnames,
                                 QList<QList<QPixmap>> *images,
                                 QList<QList<QColor>> *colors);
  void RecalcResidualsVSExperimental(ScatterPlot **plot2D);
  void PredictedVSExperimental(ScatterPlot **plot2D);
  void ClassPredictedVSExperimental(QList<QStringList> *cellnames,
                                    QList<QList<QPixmap>> *images,
                                    QList<QList<QColor>> *colors);
  void PredictedResidualsVSExperimental(ScatterPlot **plot2D);
  QList<SimpleLine2DPlot *> R2Q2();
  QList<SimpleLine2DPlot *> RMSE();
  QList<SimpleLine2DPlot *> ROCAUCs();
  QList<SimpleLine2DPlot *> ROCCurves();
  QList<SimpleLine2DPlot *> PrecisionRecallAveragePrecision();
  QList<SimpleLine2DPlot *> PrecisionRecallCurves();
  QList<SimpleLine2DPlot *> R2R2Prediction();
  QList<SimpleLine2DPlot *> RMSEPrediction();
  QList<ScatterPlot *> YScramblingPlot();

private:
  PROJECTS *projects;
  int pid, mid, predid, nlv;

  QString getProjectName() const;
  QString getModelName() const;
  PLSModel *getPLSModel() const;
  int getNPC() const;
  QString getDataHash() const;
  bool isPLSDA() const;
  bool isPLS() const;
  void extractExperimentalY(int matrix_id, const QStringList &objsel,
                            const QStringList &yvarsel, matrix **out_y);
  void extractModelMatrix(matrix *source, uint nobjects, uint nvars,
                          matrix **out_y);
};
#endif
