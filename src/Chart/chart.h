#ifndef CHART_H
#define CHART_H
// ONLY 2D SCATTER PLOTS
#include "graph.h"
#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

#include "datacurve.h"
#include "datapoint.h"

class id;
class id;
class QToolButton;
class PlotSettings;

class Chart : public QWidget, public Graphs {
  Q_OBJECT

public:
  Chart(QWidget *parent = 0);
  ~Chart();
  QWidget *weview();
  void Plot();
  void setAntialiasing(bool antialiasing_);
  void setXaxisName(QString xaxisname);
  void setYaxisName(QString yaxisname);
  void setZaxisName(QString yaxisname) {}; // not working!

  void setPlotTitle(QString plottitle);
  void setPlotTitleSize(int size);
  void setLabelDetail(bool);
  void setAxisValueSize(int size);

  void setXLabelSize(int size);
  void setXminXmaxXTick(double xmin, double xmax, int xtick);
  void getXminXmaxXTick(double *xmin, double *xmax, int *xtick);

  void setYLabelSize(int size);
  void setYminYmaxYTick(double ymin, double ymax, int ytick);
  void getYminYmaxYTick(double *ymin, double *ymax, int *ytick);

  void Center();
  void Refresh();

  void SelectAll();
  void ClearSelection();
  /* Unselect from by index (from index to index) */
  void Unselect(int from, int to);
  /* Select from by index (from index to index) */
  void Select(int from, int to);

  void setPlotSettings(const PlotSettings &settings);
  //     void setCurveData(int id, const QVector<QPointF> &data);
  void addPoint(qreal x, qreal y, QString name);
  void addPoint(qreal x, qreal y, QString name, QColor color, int radius);
  virtual void addPoint(qreal x, qreal y, qreal z, QString name, QColor color,
                        int radius) {};
  void addCurve(QVector<QPointF> curve, QString name, QColor color);

  int PointSize() const;
  DataPoint *getPoint(int id);
  void RemovePointAt(int id);
  void RemovePoint(DataPoint *dp);
  void RemoveAllPoints();

  //     QVector<DataPoint*> getPoints();
  QVector<DataCurve> getCurves();
  void RemoveCurveAt(int cid);
  void RemoveAllCurves();

  //     void clearCurve(int id);
  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void SaveAsImage(QString imgname);
public slots:
  void zoomIn();
  void zoomOut();

protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void keyPressEvent(QKeyEvent *event);
  void wheelEvent(QWheelEvent *event);

private:
  void updateRubberBandRegion();
  void refreshPixmap();
  void drawGrid(QPainter *painter);
  void drawCurves(QPainter *painter);
  void PointDraw(QPainter *painter, QRect rect, PlotSettings settings,
                 DataPoint *p);
  void drawScatters(QPainter *painter);

  /*Divide, conqueror method for selection/unselection */
  void DoUnselection(int low, int high);
  void DoSelection(int low, int high);

  double Margin = 80;
  QToolButton *zoomInButton;
  QToolButton *zoomOutButton;

  QVector<DataPoint *> p;      // used for scatter plot
  QVector<DataCurve> curveMap; // used for line plot

  QVector<int> pforward; // used to store point selected and bringed to front.
  QVector<PlotSettings> zoomStack;
  int curZoom;
  bool rubberBandIsShown, labeldetail;
  QRect rubberBandRect;
  QPixmap pixmap;
  QString m_xaxisname, m_yaxisname, m_plottitle;
  bool antialiasing;

  int titleSize;
  int axisValueSize;
  int xLabelSize;
  int yLabelSize;
};

class PlotSettings {
public:
  PlotSettings();

  void scroll(int dx, int dy);
  void adjust();
  double spanX() const { return maxX - minX; }
  double spanY() const { return maxY - minY; }

  double minX;
  double maxX;
  int numXTicks;
  double minY;
  double maxY;
  int numYTicks;

private:
  static void adjustAxis(double &min, double &max, int &numTicks);
};

#endif
