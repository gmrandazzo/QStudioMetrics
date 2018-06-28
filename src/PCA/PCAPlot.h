#ifndef PCAPLOT_H
#define PCAPLOT_H
#include "../Plotlib/ScatterPlot.h"
#include "qsmdata.h"

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
  /*ScatterPlot *ScorePlot2D();*/
  void ScorePlot2D(ScatterPlot **plot2D);
  void ScorePlotPrediction2D(ScatterPlot **plot2D);
  void LoadingsMVANormDistrib(ScatterPlot **plot2D);
  void LoadingsPlot2D(ScatterPlot **plot2D);
  void ScorePlot3D(ScatterPlot **plot3D);
  void ScorePlotPrediction3D(ScatterPlot **plot3D);
  void LoadingsPlot3D(ScatterPlot **plot3D);

private:
  PROJECTS *projects;
  int pid, mid, predid;
  LABELS g;
};
#endif
