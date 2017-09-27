#ifndef PLSPLOT_H
#define PLSPLOT_H

#include "../Plotlib/ScatterPlot2D.h"
#include "../Plotlib/ScatterPlot3D.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "../Plotlib/SimpleScatterPlot3D.h"
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
  void setNPrincipalComponent(int npc_){ npc = npc_; }

  void TU_Plot(ScatterPlot2D **plot2D);
  void T_ScorePlot2D(ScatterPlot2D **plot2D);
  void U_ScorePlot2D(ScatterPlot2D **plot2D);
  void T_ScorePlotPrediction2D(ScatterPlot2D **plot2D);
  void P_LoadingsPlot2D(ScatterPlot2D **plot2D);
  void Q_LoadingsPlot2D(ScatterPlot2D **plot2D);
  void WeightsPlot2D(ScatterPlot2D **plot2D);
  void BetaCoefficients(BarPlot **betas_barplot);
  void BetaCoefficientsDurbinWatson(SimpleLine2DPlot **dw_betas_plot);
  void T_ScorePlot3D(ScatterPlot3D **plot3D);
  void U_ScorePlot3D(ScatterPlot3D **plot3D);
  void T_ScorePlotPrediction3D(ScatterPlot3D **plot3D);
  void P_LoadingsPlot3D(ScatterPlot3D **plot3D);
  void Q_LoadingsPlot3D(ScatterPlot3D **plot3D);
  void WeightsPlot3D(ScatterPlot3D **plot3D);
  void PredictedVSExperimentalAndPrediction(ScatterPlot2D **plot2D);
  void RecalcVSExperimentalAndPrediction(ScatterPlot2D **plot2D);
  void RecalcVSExperimental(ScatterPlot2D **plot2D);
  void RecalcResidualsVSExperimental(ScatterPlot2D **plot2D);
  void PredictedVSExperimental(ScatterPlot2D **plot2D);
  void PredictedResidualsVSExperimental(ScatterPlot2D **plot2D);
  QList< SimpleLine2DPlot* > R2Q2();
  QList< SimpleLine2DPlot* > R2R2Prediction();
  QList< ScatterPlot2D *> YScramblingPlot();
  QList< SimpleLine2DPlot* > Q2SampleValidator();
  QList< SimpleLine2DPlot* > SDEPSampleValidator();
  QList< SimpleScatterPlot3D* >  Q2SurfacePlot();
  QList< SimpleScatterPlot3D* >  SDEPSurfacePlot();

private:
  PROJECTS *projects;
  int pid, mid, predid, npc;
};
#endif
