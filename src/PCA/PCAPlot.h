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
