#ifndef PLSPLOT_H
#define PLSPLOT_H

#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "../Plotlib/BarPlot.h"
#include "qsmdata.h"

#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class PLSPlot
{
public:
  PLSPlot(PROJECTS* projects_);
  void setPID(int pid_){ pid = pid_; }
  void setMID(int mid_){ mid = mid_; }
  void setPREDID(int predid_){ predid = predid_; }
  void setNLatentVariables(int nlv_){ nlv = nlv_; }

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
  void ClassRecalcVSExperimental(QList<QStringList> *cellnames, QList<QList<QPixmap>> *images, QList<QList<QColor>> *colors);
  void RecalcResidualsVSExperimental(ScatterPlot **plot2D);
  void PredictedVSExperimental(ScatterPlot **plot2D);
  void ClassPredictedVSExperimental(QList<QStringList> *cellnames, QList<QList<QPixmap>> *images, QList<QList<QColor>> *colors);
  void PredictedResidualsVSExperimental(ScatterPlot **plot2D);
  QList< SimpleLine2DPlot* > R2Q2();
  QList< SimpleLine2DPlot* > ROCAUCs();
  QList< SimpleLine2DPlot* > ROCCurves();
  QList< SimpleLine2DPlot* > PrecisionRecallAveragePrecision();
  QList< SimpleLine2DPlot* > PrecisionRecallCurves();
  QList< SimpleLine2DPlot* > R2R2Prediction();
  QList< ScatterPlot *> YScramblingPlot();

private:
  PROJECTS *projects;
  int pid, mid, predid, nlv;
};
#endif
