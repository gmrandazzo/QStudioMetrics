#ifndef QPLOTLY_H
#define QPLOTLY_H

#include "graph.h"

#include <QColor>
#include <QString>
#include <QWidget>

#ifdef WEBKIT
#include <QNetworkAccessManager>
#include <QNetworkCookieJar>
#include <QSettings>
#include <QWebView>
#else
#include <QWebEngineCookieStore>
#include <QWebEngineView>
#endif

#include <QNetworkCookie>
#include <QSize>
#include <QTemporaryDir>
#include <QUrl>

#include "databar.h"
#include "datacurve.h"
#include "datapoint.h"

#ifdef WEBKIT
class CustomNetworkCookieJar : public QNetworkCookieJar {
  Q_OBJECT

public:
  CustomNetworkCookieJar(QObject *p_pParent = NULL);
  ~CustomNetworkCookieJar(void);

  bool setCookiesFromUrl(const QList<QNetworkCookie> &p_grCookieList,
                         const QUrl &p_grUrl);

signals:
  void cookiesChanged(QString cookie);
};
#endif

class QPlotlyWindow : public QWidget, public Graphs {
  Q_OBJECT
public:
  QPlotlyWindow(QWidget *parent = 0);
  ~QPlotlyWindow();

#ifdef WEBKIT
  QWebView *weview() { return wview; }
#else
  QWebEngineView *weview() { return wview; }
#endif

  void Demo();

  void Plot();
  void setWindowSize(int height, int width) {
    wsize.setHeight(height);
    wsize.setWidth(width);
  }

  void setXaxisName(QString xaxisname_);
  void setYaxisName(QString yaxisname_);
  void setZaxisName(QString zaxisname_);
  void setPlotTitle(QString plot_title_);
  void setPlotTitleSize(int size);
  void setAxisValueSize(int size);

  void setXLabelSize(int size);
  void setXminXmaxXTick(double xmin, double xmax, int xtick);
  void getXminXmaxXTick(double *xmin, double *xmax, int *xtick);

  void setYLabelSize(int size);
  void setYminYmaxYTick(double ymin, double ymax, int ytick);
  void getYminYmaxYTick(double *ymin, double *ymax, int *ytick);

  // 2D Scatter plot
  void addPoint(qreal x, qreal y, QString name);
  void addPoint(qreal x, qreal y, QString name, QColor color, int radius);

  // 3D Scatter plot
  void addPoint(qreal x, qreal y, qreal z, QString name);
  void addPoint(qreal x, qreal y, qreal z, QString name, QColor color,
                int radius);

  // 2D Curve
  void addCurve(QVector<QPointF> curve, QString name, QColor color);
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
  void Center();
  void Refresh();
  void SelectAll();
  void ClearSelection();
  void SaveAsImage(QString imgname);

private slots:
#ifdef WEBKIT
  void WebKithandleCookieAdded(QString cookie);
#else
  void WebEnginehandleCookieAdded(const QNetworkCookie &cookie);
#endif
  void close();

private:
  PLOTTYPE pt;
  QVector<DataPoint *> p; // used for scatter 2D/3D plot
  QVector<DataCurve> c;   // used for line plot
  QVector<DataBar *> b;   // used for bar plot
  QList<int> selected_points;
  QString code;
  QSize wsize;
  QString xaxisname;
  QString yaxisname;
  QString zaxisname;
  QString plot_title;
  QUrl page;

#ifdef WEBKIT
  QWebView *wview;
  // QNetworkCookieJar *cookieJar;
  CustomNetworkCookieJar *cookieJar;
  QNetworkAccessManager *nam;
#else
  QWebEngineView *wview;
  QWebEngineCookieStore *cookie_store;
#endif

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
  void CookieParser(QString cookie);
  QString json;
  int trace_id;
  int xtickangle, ytickangle;
};

#endif
