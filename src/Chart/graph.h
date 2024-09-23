#ifndef GRAPH_H
#define GRAPH_H

#include "datacurve.h"
#include "datapoint.h"
#include <QWidget>
#include <QtCore>

enum PLOTTYPE { _2D, _3D };

/* Polymorphism to choose between two plot engines: plotly or qt.
 *  How to use?
 *

Graphs* function(int i){ // function returning a base class pointer.
  switch(i) {
    case 1:
      return new Chart();
    case 2:
      return new QPlotlyWindow();
    default:
      return new Chart();
  }
}

int main(void)
{
  Graphs *s =  function(1);
  s-> ... call same virtual methods...
  delete s;
  return 0;
}

 */
class Graphs {
public:
  Graphs() {}
  virtual ~Graphs() {}
  virtual void Plot() = 0;
  virtual QWidget *weview() = 0;
  // virtual void setAntialiasing(bool antialiasing_) = 0;
  virtual void setXaxisName(QString xaxisname) = 0;
  virtual void setYaxisName(QString yaxisname) = 0;
  virtual void setZaxisName(QString yaxisname) = 0;
  virtual void setPlotTitle(QString plottitle) = 0;
  virtual void setPlotTitleSize(int size) = 0;
  virtual void setAxisValueSize(int size) = 0;
  // virtual void setLabelDetail(bool) = 0;
  virtual void setXLabelSize(int size) = 0;
  virtual void setXminXmaxXTick(double xmin, double xmax, int xtick) = 0;
  virtual void getXminXmaxXTick(double *xmin, double *xmax, int *xtick) = 0;
  virtual void setYLabelSize(int size) = 0;
  virtual void setYminYmaxYTick(double ymin, double ymax, int ytick) = 0;
  virtual void getYminYmaxYTick(double *ymin, double *ymax, int *ytick) = 0;
  virtual void Center() = 0;
  virtual void Refresh() = 0;
  virtual void SelectAll() = 0;
  virtual void ClearSelection() = 0;
  // virtual void Unselect(int from, int to) = 0;
  // virtual void Select(int from, int to) = 0;
  virtual void addPoint(qreal x, qreal y, QString name) = 0;
  virtual void addPoint(qreal x, qreal y, QString name, QColor color,
                        int radius) = 0;
  virtual void addPoint(qreal x, qreal y, qreal z, QString name, QColor color,
                        int radius) = 0;
  virtual void addCurve(QVector<QPointF> curve, QString name, QColor color) = 0;
  virtual int PointSize() const = 0;
  virtual DataPoint *getPoint(int id) = 0;
  virtual void RemovePointAt(int id) = 0;
  virtual void RemovePoint(DataPoint *dp) = 0;
  virtual void RemoveAllPoints() = 0;
  virtual QVector<DataCurve> getCurves() = 0;
  virtual void RemoveCurveAt(int cid) = 0;
  virtual void RemoveAllCurves() = 0;
  // virtual QSize minimumSizeHint() = 0;
  // virtual QSize sizeHint() = 0;
  virtual void SaveAsImage(QString imgname) = 0;
};

#endif
