#ifndef LDAPLOT_H
#define LDAPLOT_H

#include "ScatterPlot2D.h"
#include "ScatterPlot3D.h"
#include "SimpleLine2DPlot.h"
#include "data.h"

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
  
  void FeaturePlot2D(ScatterPlot2D **plot2D);
  void FeaturePlot3D(ScatterPlot3D **plot3D);
  void ProbabilityDistribution(ScatterPlot2D **plot2D);
  void FeaturePlotAndPrediction2D(ScatterPlot2D **plot2D);
  void FeaturePlotAndPrediction3D(ScatterPlot3D **plot3D);
  void ProbabilityDistributionWithPredictions(ScatterPlot2D** plot2D);
  
private:
  PROJECTS *projects;
  int pid, mid, predid;
};

#endif

