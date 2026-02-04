/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for graph.
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

#ifndef GRAPH_H
#define GRAPH_H

#include "datacurve.h"
#include "datapoint.h"
#include <QWidget>
#include <QtCore>

enum PLOTTYPE { _2D, _3D };

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
  virtual int getPlotTitleSize() = 0;
  virtual void setAxisValueSize(int size) = 0;
  virtual int getAxisValueSize() = 0;
  // virtual void setLabelDetail(bool) = 0;
  virtual void setXLabelSize(int size) = 0;
  virtual int getXLabelSize() = 0;
  virtual void setXminXmaxXTick(double xmin, double xmax, int xtick) = 0;
  virtual void getXminXmaxXTick(double *xmin, double *xmax, int *xtick) = 0;
  virtual void setYLabelSize(int size) = 0;
  virtual int getYLabelSize() = 0;
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
  virtual void addCurve(QVector<QPointF> curve, QString name, QColor color, bool smooth = false) = 0;
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
  virtual void setImages(const QMap<QString, QPixmap> &images) {}
  virtual void LoadSettings() = 0;
};

#endif
