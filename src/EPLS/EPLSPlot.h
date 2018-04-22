#ifndef EPLSPLOT_H
#define EPLSPLOT_H

#include "../Plotlib/ScatterPlot2D.h"
#include "../Plotlib/ScatterPlot3D.h"
#include "../Plotlib/SimpleLine2DPlot.h"
#include "../Plotlib/SimpleScatterPlot3D.h"
#include "../Plotlib/BarPlot.h"
#include "qsmdata.h"

#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class EPLSPlot
{
public:
  EPLSPlot(PROJECTS* projects_);
  void setPID(int pid_){ pid = pid_; }
  void setMID(int mid_){ mid = mid_; }
  void setPREDID(int predid_){ predid = predid_; }
  void setNLatentVariables(int nlv_){ nlv = nlv_; }
  void PredictedVSExperimentalAndPrediction(ScatterPlot2D **plot2D);
  void RecalcVSExperimentalAndPrediction(ScatterPlot2D **plot2D);
  void RecalcVSExperimental(ScatterPlot2D **plot2D);
  void ClassRecalcVSExperimental(QList<QStringList> *cellnames, QList<QList<QPixmap>> *images, QList<QList<QColor>> *colors);
  void RecalcResidualsVSExperimental(ScatterPlot2D **plot2D);
  void PredictedVSExperimental(ScatterPlot2D **plot2D);
  void ClassPredictedVSExperimental(QList<QStringList> *cellnames, QList<QList<QPixmap>> *images, QList<QList<QColor>> *colors);
  void PredictedResidualsVSExperimental(ScatterPlot2D **plot2D);
  QList< SimpleLine2DPlot* > ROCAUCs();
  QList< SimpleLine2DPlot* > ROCCurves();
  QList< SimpleLine2DPlot* > PrecisionRecallAveragePrecision();
  QList< SimpleLine2DPlot* > PrecisionRecallCurves();
  QList< SimpleLine2DPlot* > R2Q2();
  QList< SimpleLine2DPlot* > R2R2Prediction();
  //QList< ScatterPlot2D *> YScramblingPlot();

private:
  PROJECTS *projects;
  int pid, mid, predid, nlv;
};
#endif
