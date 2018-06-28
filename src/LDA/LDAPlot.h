#ifndef LDAPLOT_H
#define LDAPLOT_H

#include "../Plotlib/ScatterPlot.h"

#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class LDAPlot
{
public:
  LDAPlot(PROJECTS* projects_);
  void setPID(int pid_){ pid = pid_; }
  void setMID(int mid_){ mid = mid_; }
  void setPREDID(int predid_){ predid = predid_; }

  void FeaturePlot2D(ScatterPlot **plot2D);
  void FeaturePlot3D(ScatterPlot **plot3D);
  void ProbabilityDistribution(ScatterPlot **plot2D);
  void FeaturePlotAndPrediction2D(ScatterPlot **plot2D);
  void FeaturePlotAndPrediction3D(ScatterPlot **plot3D);
  void ProbabilityDistributionWithPredictions(ScatterPlot** plot2D);

private:
  PROJECTS *projects;
  int pid, mid, predid;
};

#endif
