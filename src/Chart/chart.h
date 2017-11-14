#ifndef CHART_H
#define CHART_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

#include "datapoint.h"
#include "datacurve.h"

class id;
class id;
class QToolButton;
class PlotSettings;

class Chart : public QWidget
{
    Q_OBJECT

public:
    Chart(QWidget *parent = 0);
    ~Chart();

    void setXaxisName(QString xaxisname);
    void setYaxisName(QString yaxisname);
    void setPlotTitle(QString plottitle);
    void setLabelDetail(bool);

    void setXminXmaxXTick(double xmin, double xmax, int xtick);
    void setYminYmaxYTick(double ymin, double ymax, int ytick);

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
    void addCurve(QVector< QPointF > curve, QString name, QColor color);

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
    void drawScatters(QPainter *painter);

    /*Divide, conqueror method for selection/unselection */
    void DoUnselection(int low, int high);
    void DoSelection(int low, int high);

    enum { Margin = 80 };
    qreal rescalefactor;
    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;

    QVector <DataPoint*> p; //used for scatter plot
    QVector <DataCurve> curveMap; // used for line plot

    QVector< int > pforward; //used to store point selected and bringed to front.
    QVector<PlotSettings> zoomStack;
    int curZoom;
    bool rubberBandIsShown, labeldetail;
    QRect rubberBandRect;
    QPixmap pixmap;
    QString m_xaxisname, m_yaxisname, m_plottitle;
};

class PlotSettings
{
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
