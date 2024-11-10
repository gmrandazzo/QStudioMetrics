#ifndef CPCAPLOT_H
#define CPCAPLOT_H
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class CPCAPlot {
public:
  explicit CPCAPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }
  void setGroups(LABELS g_) { g = g_; }
  void SuperScorePlot2D(ScatterPlot **plot2D) const;
  void SuperWeightsPlot2D(ScatterPlot **plot2D) const;
  void SuperScorePlotPrediction2D(ScatterPlot **plot2D) const;
  QList<ScatterPlot *> BlockScorePlotPrediction2D();
  QList<ScatterPlot *> BlockScoresPlot2D();
  QList<ScatterPlot *> BlockLoadingsPlot2D();
  void ExpVarPlot(SimpleLine2DPlot **plot2D);

private:
  PROJECTS *projects;
  int pid, mid, predid;
  LABELS g;
};
#endif
