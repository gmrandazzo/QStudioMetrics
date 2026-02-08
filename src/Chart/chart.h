/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for chart.
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

#ifndef CHART_H
#define CHART_H


// ONLY 2D SCATTER PLOTS
#include "graph.h"
#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>
#include <algorithm>

#include "datacurve.h"
#include "datapoint.h"
#include "plotsettings.h"

class id;
class id;
class QToolButton;
class PlotSettings;

class Chart : public QWidget, public Graphs {
  Q_OBJECT

public:
  explicit Chart(QWidget *parent = 0);
  ~Chart() override;
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

  void setPlotSettings(const PlotSettings &settings);
  //     void setCurveData(int id, const QVector<QPointF> &data);
  void addPoint(qreal x, qreal y, QString name) override;
  void addPoint(qreal x, qreal y, QString name, QColor color, int radius) override;
  void addPoint(qreal x, qreal y, qreal z, QString name, QColor color,
                        int radius) override {};
  void addCurve(QVector<QPointF> curve, QString name, QColor color, bool smooth = false) override;

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
  QSize minimumSizeHint() const override;
  QSize sizeHint() const override;

  void SaveAsImage(QString imgname) override;

  void setImages(const QMap<QString, QPixmap> &images) override;
  void LoadSettings() override;

public slots:
  void zoomIn();
  void zoomOut();
  void recentre();

protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;

private:
  void updateRubberBandRegion();
  void refreshPixmap();
  void drawGrid(QPainter *painter);
  void drawCurves(QPainter *painter);
  void PointDraw(QPainter *painter, QRect rect, PlotSettings settings,
                 DataPoint *p);
  void drawScatters(QPainter *painter);
  
  // Spatial Indexing for Hover
  void buildIndex();
  struct IndexNode {
      double x;
      int index;
      // Define operator< for std::sort and std::lower_bound
      bool operator<(const IndexNode& other) const { return x < other.x; }
      bool operator<(double val) const { return x < val; }
  };
  friend bool operator<(double val, const IndexNode& node) { return val < node.x; }

  /*Divide, conqueror method for selection/unselection */
  void DoUnselection(int low, int high);
  void DoSelection(int low, int high);

  double Margin = 80;
  QToolButton *zoomInButton;
  QToolButton *zoomOutButton;
  QToolButton *recentreButton;

  QVector<DataPoint *> p;      // used for scatter plot
  QVector<DataCurve> curveMap; // used for line plot
  
  // Optimization: Sorted Index by X coordinate
  QVector<IndexNode> m_searchIndex;
  bool m_indexDirty;

  QVector<int> pforward; // used to store point selected and bringed to front.
  QVector<PlotSettings> zoomStack;
  int curZoom;
  bool rubberBandIsShown, labeldetail;
  QRect rubberBandRect;
  
  // Lasso Selection
  bool m_isLassoActive;
  QPolygon m_lassoPolygon;
  
  QPixmap pixmap;
  QString m_xaxisname, m_yaxisname, m_plottitle;
  bool antialiasing;

  int titleSize;
  int axisValueSize;
  int xLabelSize;
  int yLabelSize;

  QMap<QString, QPixmap> m_images;
};

#endif