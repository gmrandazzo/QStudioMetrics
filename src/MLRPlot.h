#ifndef MLRPLOT_H
#define MLRPLOT_H

#include "ScatterPlot2D.h"
#include "ScatterPlot3D.h"
#include "SimpleLine2DPlot.h"
#include "BarPlot.h"
#include "data.h"

#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"


class MLRPlot
{
public:
  MLRPlot(PROJECTS* projects_);
  void setPID(int pid_){ pid = pid_; }
  void setMID(int mid_){ mid = mid_; }
  void setPREDID(int predid_){ predid = predid_; }
  
  QList< BarPlot* > BetaCoefficients();
  void PredictedVSExperimentalAndPrediction(ScatterPlot2D **plot2D);
  void RecalcVSExperimentalAndPrediction(ScatterPlot2D **plot2D);
  void RecalcVSExperimental(ScatterPlot2D **plot2D);
  void RecalcResidualsVSExperimental(ScatterPlot2D **plot2D);
  void PredictedVSExperimental(ScatterPlot2D **plot2D);
  void PredictedResidualsVSExperimental(ScatterPlot2D **plot2D);
  QList<ScatterPlot2D*> YScramblingPlot();

private:
  PROJECTS *projects;
  int pid, mid, predid;
};

#endif

