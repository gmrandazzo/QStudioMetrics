#ifndef QPLOTLY_H
#define QPLOTLY_H

#include <QColor>
#include <QWidget>
#include <QString>
#include <QWebEngineView>
#include <QWebEngineCookieStore>
#include <QNetworkCookie>
#include <QUrl>
#include <QSize>
#include <QTemporaryDir>

#include "datapoint.h"
#include "datacurve.h"
#include "databar.h"


#define MAJOR 1
#define MINOR 1
#define PATCH 1

void QPlotlyVersion(int *major, int *minor, int *patch);

enum PLOTTYPE
{
   _2D,
   _3D
};

class QPlotlyWindow : public QWidget
{
  Q_OBJECT
public:
    QPlotlyWindow(QWidget *parent = 0);
    ~QPlotlyWindow();
    QWebEngineView *weview(){ return wview; }

    void Demo();

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

    QVector<DataCurve> getCurves();
    void RemoveCurveAt(int cid);
    void RemoveAllCurves();

    void Refresh();
    void SelectAll();
    void ClearSelection();
    void SaveAsImage(QString imgname);

private slots:
    void handleCookieAdded(const QNetworkCookie &cookie);
    void close();

private:
  PLOTTYPE pt;
  QVector <DataPoint*> p; //used for scatter 2D/3D plot
  QVector <DataCurve> c; // used for line plot
  QVector <DataBar*> b; // used for bar plot
  QList <int> selected_points;
  QString code;
  QSize wsize;
  QString xaxisname;
  QString yaxisname;
  QString zaxisname;
  QString plot_title;
  QWebEngineView *wview;
  QWebEngineCookieStore *cookie_store;
  QTemporaryDir dir;
  QUrl WriteTemporaryPage(QString code);
  void ModifyTemporaryPage(QString code, QList<int> line_ids);
  QStringList JSonScatterWorker(int from, int to);
  QString genJSONScatter();
  QStringList JSon3DScatterWorker(int from, int to);
  QString genJSON3DScatter();
  QString genJSONCurve();
  QString genJSONBar();
  QString genJSONInteractions();
  int FindPoint(qreal x, qreal y, qreal z, QString name_);
  QString json;
  int trace_id;
  int xtickangle, ytickangle;
};

#endif
