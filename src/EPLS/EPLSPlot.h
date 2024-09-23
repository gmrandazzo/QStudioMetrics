#ifndef EPLSPLOT_H
#define EPLSPLOT_H

#include "../Plotlib/BarPlot.h"
#include "../Plotlib/ScatterPlot.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class EPLSPlot {
public:
  explicit EPLSPlot(PROJECTS *projects_);
  void setPID(int pid_) { pid = pid_; }
  void setMID(int mid_) { mid = mid_; }
  void setPREDID(int predid_) { predid = predid_; }
  void setNLatentVariables(int nlv_) { nlv = nlv_; }
  void PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D);
  void RecalcVSExperimentalAndPrediction(ScatterPlot **plot2D);
  void RecalcVSExperimental(ScatterPlot **plot2D);
  void ClassRecalcVSExperimental(QList<QStringList> *cellnames,
                                 QList<QList<QPixmap>> *images,
                                 QList<QList<QColor>> *colors);
  void RecalcResidualsVSExperimental(ScatterPlot **plot2D);
  void PredictedVSExperimental(ScatterPlot **plot2D);
  void ClassPredictedVSExperimental(QList<QStringList> *cellnames,
                                    QList<QList<QPixmap>> *images,
                                    QList<QList<QColor>> *colors);
  void PredictedResidualsVSExperimental(ScatterPlot **plot2D);
  QList<SimpleLine2DPlot *> ROCAUCs();
  QList<SimpleLine2DPlot *> ROCCurves();
  QList<SimpleLine2DPlot *> PrecisionRecallAveragePrecision();
  QList<SimpleLine2DPlot *> PrecisionRecallCurves();
  QList<SimpleLine2DPlot *> R2Q2();
  QList<SimpleLine2DPlot *> R2R2Prediction();
  // QList< ScatterPlot *> YScramblingPlot();

private:
  PROJECTS *projects;
  int pid, mid, predid, nlv;
};
#endif
