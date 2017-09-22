#ifndef PCAPLOT_H
#define PCAPLOT_H
#include "ScatterPlot2D.h"
#include "ScatterPlot3D.h"
#include "data.h"

#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class PCAPlot
{
public:
  PCAPlot(PROJECTS* projects_);
  void setPID(int pid_){ pid = pid_; }
  void setMID(int mid_){ mid = mid_; }
  void setPREDID(int predid_){ predid = predid_; }
  void setGroups(LABELS g_){ g = g_; }
  /*ScatterPlot2D *ScorePlot2D();*/
  void ScorePlot2D(ScatterPlot2D **plot2D);
  void ScorePlotPrediction2D(ScatterPlot2D **plot2D);
  void LoadingsMVANormDistrib(ScatterPlot2D **plot2D);
  void LoadingsPlot2D(ScatterPlot2D **plot2D);
  void ScorePlot3D(ScatterPlot3D **plot3D);
  void ScorePlotPrediction3D(ScatterPlot3D **plot3D);
  void LoadingsPlot3D(ScatterPlot3D **plot3D);

private:
  PROJECTS *projects;
  int pid, mid, predid;
  LABELS g;
};
#endif
