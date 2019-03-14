#ifndef QPLOT_H
#define QPLOT_H

#include "plotinfo.h"

#include <QColor>
#include <QWidget>
#include <QString>
#include <QChart>

#include <QChartGlobal>
#include <QChartView>
#include <QScatterSeries>

#include <Q3DScatter>

#include "datapoint.h"
#include "datacurve.h"
#include "databar.h"

class QPlotWindow : public QWidget, public Graphs
{
  Q_OBJECT
public:
    QPlotWindow(QWidget *parent = 0, PLOTTYPE pt_ = _2D);
    ~QPlotWindow();

    //void Demo();

    void Plot();

    void setWindowSize(int height, int width) { wsize.setHeight(height); wsize.setWidth(width); }
    void setXaxisName(QString xaxisname_);
    void setYaxisName(QString yaxisname_);
    void setZaxisName(QString zaxisname_);
    void setPlotTitle(QString plot_title_);

    // 2D Scatter plot
    void addPoint(qreal x, qreal y, QString name);
    void addPoint(qreal x, qreal y, QString name, QColor color, int radius);

    // 3D Scatter plot
    void addPoint(qreal x, qreal y, qreal z, QString name);
    void addPoint(qreal x, qreal y, qreal z, QString name, QColor color, int radius);

    // 2D Curve
    void addCurve(QVector< QPointF > curve, QString name, QColor color);
    void setCurveStyle(int indx, LTYPE cs);

    // 2D Bar plot
    void addBars(QStringList x, QVector<qreal> y, QStringList text, QColor color);

    int PointSize() const;
    DataPoint *getPoint(int id);
    void RemovePointAt(int id);
    void RemovePoint(DataPoint *dp);
    void RemoveAllPoints();

    // QVector<DataCurve> getCurves();
    void RemoveCurveAt(int cid);
    void RemoveAllCurves();

    void Refresh();
    void SelectAll();
    void ClearSelection();
    void SaveAsImage(QString imgname);

private slots:
    //void handleClickedPoint(const QPointF &point);
    void close();

private:
  PLOTTYPE pt;
  QMap<QString, DataPoint*> p; //used for scatter 2D/3D plot
  QMap<QString, DataCurve*> c; // used for line/curve plot
  QVector<DataBar*> b; // used for bar plot

  QList <int> selected_points;
  QString code;
  QSize wsize;
  QString xaxisname;
  QString yaxisname;
  QString zaxisname;
  QString plot_title;


  QtCharts::QChartView *_2dview;
  //Q3DScatter *_3dview;

  //QScatterSeries *m_scatter;

  int FindPoint(qreal x, qreal y, qreal z, QString name_);
};

#endif
