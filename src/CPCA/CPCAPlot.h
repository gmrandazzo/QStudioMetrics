#ifndef CPCAPLOT_H
#define CPCAPLOT_H
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class CPCAPlot
{
public:
  explicit CPCAPlot(PROJECTS* projects_);
  void setPID(int pid_){ pid = pid_; }
  void setMID(int mid_){ mid = mid_; }
  void setPREDID(int predid_){ predid = predid_; }
  void setGroups(LABELS g_){ g = g_; }
  /*ScatterPlot *ScorePlot2D();*/
  void SuperScorePlot2D(ScatterPlot **plot2D);
  void SuperScorePlotPrediction2D(ScatterPlot **plot2D);
  void ExpVarPlot(SimpleLine2DPlot **plot2D);
  void SuperWeigthsPlot2D(ScatterPlot **plot2D);
  QList< ScatterPlot * > BlockScoresPlot2D();
  QList< ScatterPlot * > BlockLoadingsPlot2D();

private:
  PROJECTS *projects;
  int pid, mid, predid;
  LABELS g;
};
#endif
