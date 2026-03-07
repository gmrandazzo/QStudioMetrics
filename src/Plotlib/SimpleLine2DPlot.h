/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for simpleline2dplot.
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

// SimpleLine2DPlot.h
#ifndef SIMPLELINE2DPLOT_H
#define SIMPLELINE2DPLOT_H


#include "ui_SimpleLine2DPlot.h"
#include <QColor>
#include <QList>
#include <QWidget>
#include <scientific.h>

#include "Chart/chart.h"
#include "qstudiometricstypes.h"

class SimpleLine2DPlot : public QWidget {
  Q_OBJECT

public:
  SimpleLine2DPlot(QList<matrix *> mlst, QStringList curvenames,
                   QString windowtitle, QString xaxestitle, QString yaxestitle, bool smooth = false);
  SimpleLine2DPlot(matrix *m, QStringList curvenames, QString windowtitle,
                   QString xaxestitle, QString yaxestitle, bool smooth = false);
  // Simple Line Plot with one x (first column of m) and more y objects
  SimpleLine2DPlot(matrix *m, QString curvename, QString windowtitle,
                   QString xaxestitle, QString yaxestitle, bool smooth = false);
  void setPlotTitle(QString);
  void setXminXmaxXTick(double xmin, double xmax, int xtick);
  void setYminYmaxYTick(double ymin, double ymax, int ytick);
  void setImages(QList<IMAGE> &images);

private slots:
  void slotExit();
  void SavePlotImage();

private:
  int random_(int low, int high);
  Ui::SimpleLine2DPlot ui;
  Chart *chart;
};

#endif
