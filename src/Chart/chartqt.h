/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for chartqt.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CHARTQT_H
#define CHARTQT_H
#include "graph.h"

#include "databar.h"
#include "datacurve.h"
#include "datapoint.h"
#include <QLabel>
#include <QMap>
#include <QtCharts/QAbstractSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts>

class QToolButton;

class ChartQt : public QChartView, public Graphs {
  Q_OBJECT
public:
  ChartQt(QWidget *parent = 0);
  ~ChartQt();
  QWidget *weview() override;
  void Plot() override;
  void setAntialiasing(bool antialiasing_);
  void setXaxisName(QString xaxisname) override;
  void setYaxisName(QString yaxisname) override;
  void setZaxisName(QString yaxisname) override {}; // not working!

  void setPlotTitle(QString plottitle) override;
  void setPlotTitleSize(int size) override;
  int getPlotTitleSize() override { return titleSize; }
  void setLabelDetail(bool);
  void setAxisValueSize(int size) override;
  int getAxisValueSize() override { return axisValueSize; }

  void setXLabelSize(int size) override;
  int getXLabelSize() override { return xLabelSize; }
  void setXminXmaxXTick(double xmin, double xmax, int xtick) override;
  void getXminXmaxXTick(double *xmin, double *xmax, int *xtick) override;

  void setYLabelSize(int size) override;
  int getYLabelSize() override { return yLabelSize; }
  void setYminYmaxYTick(double ymin, double ymax, int ytick) override;
  void getYminYmaxYTick(double *ymin, double *ymax, int *ytick) override;

  void Center() override;
  void Refresh() override;

  void SelectAll() override;
  void ClearSelection() override;
  /* Unselect from by index (from index to index) */
  void Unselect(int from, int to);
  /* Select from by index (from index to index) */
  void Select(int from, int to);

  //     void setCurveData(int id, const QVector<QPointF> &data);
  void addPoint(qreal x, qreal y, QString name) override;
  void addPoint(qreal x, qreal y, QString name, QColor color, int radius) override;
  void addPoint(qreal x, qreal y, qreal z, QString name, QColor color,
                        int radius) override {};
  void addCurve(QVector<QPointF> curve, QString name, QColor color, bool smooth = false) override;
  void setCurveStyle(int indx, LTYPE cs);

  // Bar plot
  void addBars(QStringList x, QVector<qreal> y, QStringList text, QColor color);
  void updateBarsData(int indx, QVector<qreal> y, QColor color);

  int PointSize() const override;
  DataPoint *getPoint(int id) override;
  void RemovePointAt(int id) override;
  void RemovePoint(DataPoint *dp) override;
  void RemoveAllPoints() override;

  //     QVector<DataPoint*> getPoints();
  QVector<DataCurve> getCurves() override;
  void RemoveCurveAt(int cid) override;
  void RemoveAllCurves() override;

  //     void clearCurve(int id);
  void SaveAsImage(QString imgname) override;

  void setImages(const QMap<QString, QPixmap> &images) override;
  void LoadSettings() override;

public slots:
  void showLabels();
  void zoomIn();
  void zoomOut();
  void recentre();
  void slotPointHoverd(const QPointF &point, bool state);
  void slotPointClicked(const QPointF &point);
  void slotBarHovered(bool status, int index, QBarSet *barset);

protected:
  bool viewportEvent(QEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private:
  QToolButton *zoomInButton;
  QToolButton *zoomOutButton;
  QToolButton *recentreButton;

  /*Divide, conqueror method for selection/unselection */
  void refreshPlot();
  QMap<MarkerType, QMap<QColor, QMap<bool, QList<int>>>>
  getColors_Shapes_Selected();
  void drawScatters();
  void updateScatters();
  void drawCurves();
  void updateCurves();
  void drawBars();
  void updateBars();
  void DoUnselection(int low, int high);
  void DoSelection(int low, int high);

  QVector<DataPoint *> p;      // used for scatter plot
  QVector<DataCurve> curveMap; // used for line plot
  QVector<DataBar *> b;        // used for bar plot

  QList<QXYSeries *> seriesList;
  QList<QXYSeries *> curvesList;
  QList<QBarSet *> barsList;

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
  QPoint m_lastMousePos;

  QList<QLabel *> plotLabels;
  QLabel *m_valueLabel;
  QRectF zoom_region;

  QMap<QString, QPixmap> m_images;
};

#endif // CHARTQT_H
