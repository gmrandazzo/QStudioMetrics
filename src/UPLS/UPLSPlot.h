#ifndef UPLSPLOT_H
#define UPLSPLOT_H
#include "../Plotlib/ScatterPlot2D.h"
#include "../Plotlib/ScatterPlot3D.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class UPLSPlot
{
public:
  UPLSPlot(PROJECTS* projects_);
  void setPID(int pid_){ pid = pid_; }
  void setMID(int mid_){ mid = mid_; }
  void setPREDID(int predid_){ predid = predid_; }
  void setNPrincipalComponent(int npc_){ npc = npc_; }

  void TU_Plot(ScatterPlot2D **plot2D);
  void T_ScorePlot2D(ScatterPlot2D **plot2D);
  void U_ScorePlot2D(ScatterPlot2D **plot2D);
  void T_ScorePlotPrediction2D(ScatterPlot2D **plot2D);
  void P_LoadingsPlot2D(QList< ScatterPlot2D* > *plot2D);
  void Q_LoadingsPlot2D(QList< ScatterPlot2D* > *plot2D);
  void WeightsPlot2D(QList< ScatterPlot2D* > *plot2D);
  void T_ScorePlot3D(ScatterPlot3D **plot3D);
  void U_ScorePlot3D(ScatterPlot3D **plot3D);
  void T_ScorePlotPrediction3D(ScatterPlot3D **plot3D);
  void P_LoadingsPlot3D(QList< ScatterPlot3D* > *plot3D);
  void Q_LoadingsPlot3D(QList< ScatterPlot3D* > *plot3D);
  void WeightsPlot3D(QList< ScatterPlot3D* > *plot3D);
  void RecalcVSExperimentalAndPrediction(QList< ScatterPlot2D* > *plot2D);
  void PredictedVSExperimentalAndPrediction(QList< ScatterPlot2D* > *plot2D);
  void RecalcVSExperimental(QList< ScatterPlot2D* > *plot2D);
  void RecalcResidualsVSExperimental(QList< ScatterPlot2D* > *plot2D);
  void PredictedVSExperimental(QList< ScatterPlot2D* > *plot2D);
  void PredictedResidualsVSExperimental(QList< ScatterPlot2D* > *plot2D);
  void R2Q2(QList< SimpleLine2DPlot* > *plot2D);
  void R2R2Prediction(QList< SimpleLine2DPlot* > *plot2D);

private:
  PROJECTS *projects;
  int pid, mid, predid, npc;
};
#endif
