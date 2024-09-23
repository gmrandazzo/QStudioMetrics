#ifndef MLRPLOT_H
#define MLRPLOT_H

#include "../Plotlib/BarPlot.h"
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class MLRPlot {
public:
  explicit MLRPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }

  QList<BarPlot *> BetaCoefficients();
  void PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D);
  void RecalcVSExperimentalAndPrediction(ScatterPlot **plot2D);
  void RecalcVSExperimental(ScatterPlot **plot2D);
  void RecalcResidualsVSExperimental(ScatterPlot **plot2D);
  void PredictedVSExperimental(ScatterPlot **plot2D);
  void PredictedResidualsVSExperimental(ScatterPlot **plot2D);
  QList<ScatterPlot *> YScramblingPlot();

private:
  PROJECTS *projects;
  int pid, mid, predid;
};

#endif
