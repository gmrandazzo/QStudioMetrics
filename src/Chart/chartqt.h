#ifndef CHARTQT_H
#define CHARTQT_H
#include "graph.h"

#include <QMap>
#include <QLabel>
#include <QtCharts>
#include <QtCharts/QChartView>
#include "datapoint.h"
#include "datacurve.h"
#include "databar.h"
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>


class QToolButton;

class ChartQt : public QChartView,  public Graphs
{
    Q_OBJECT
public:
    ChartQt(QWidget *parent = 0);
    ~ChartQt();
    QWidget *weview();
    void Plot();
    void setAntialiasing(bool antialiasing_);
    void setXaxisName(QString xaxisname);
    void setYaxisName(QString yaxisname);
    void setZaxisName(QString yaxisname){}; // not working!

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

//     void setCurveData(int id, const QVector<QPointF> &data);
    void addPoint(qreal x, qreal y, QString name);
    void addPoint(qreal x, qreal y, QString name, QColor color, int radius);
    virtual void addPoint(qreal x, qreal y, qreal z, QString name, QColor color, int radius){};
    void addCurve(QVector< QPointF > curve, QString name, QColor color);
    void setCurveStyle(int indx, LTYPE cs);
    
    // Bar plot
    void addBars(QStringList x, QVector<qreal> y, QStringList text, QColor color);

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

    void SaveAsImage(QString imgname);

public slots:
    void showLabels();
    void zoomIn();
    void zoomOut();
    void slotPointHoverd(const QPointF &point, bool state);
    void slotPointClicked(const QPointF &point);

protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;

    /*Divide, conqueror method for selection/unselection */
    void refreshPlot();
    QMap<MarkerType, QMap<QColor, QMap<bool, QList<int>>>> getColors_Shapes_Selected();
    void drawScatters();
    void drawCurves();
    void drawBars();
    void DoUnselection(int low, int high);
    void DoSelection(int low, int high);

    QVector <DataPoint*> p; //used for scatter plot
    QVector <DataCurve> curveMap; // used for line plot
    QVector<DataBar*> b; // used for bar plot

    QPixmap pixmap;
    QString m_xaxisname, m_yaxisname, m_plottitle;
    bool antialiasing;
    bool labeldetail;
    bool m_isTouching;
    bool plot_ready;

    int titleSize;
    int axisValueSize;
    int xLabelSize;
    int yLabelSize;

    // used to define "point selection";
    QPointF mPresscc, mReleasecc;

    QList<QLabel*> plotLabels;
    QLabel *m_valueLabel;
    QRectF zoom_region;
};

#endif // CHARTQT_H
