/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for barplot.
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

// BarPlot.h
// SimpleLine2DPlot.h
#ifndef BARPLOT_H
#define BARPLOT_H

#include "ui_BarPlot.h"
#include <QColor>
#include <QList>
#include <QWidget>

#include "Chart/chartqt.h"

#include "qsmdata.h"

extern "C" {
#include "scientific.h"
}

class BarPlot : public QWidget {
  Q_OBJECT

public:
  // Simple bar plot. Every value in v_ will be plotted in a bar and the height
  // of the bar correspond to the v_ value itself.
  BarPlot(dvector *v_, QStringList varnames, QString windowtitle,
          QWidget *parent = 0);

  BarPlot(dvector *v_, QStringList varnames, QString windowtitle,
          QString xaxestitle, QString yaxestitle, QWidget *parent = nullptr);

  BarPlot(QList<dvector *> v_, QString windowtitle, QString xaxestitle,
          QString yaxestitle, QStringList labelname, QWidget *parent = nullptr);

  BarPlot(QList<dvector *> bar_lists_, QStringList windowtitles, QString xaxestitle,
          QString yaxestitle, QStringList labelnames, QWidget *parent = nullptr);

  ~BarPlot();

  void setImages(QList<IMAGE> &images);

private slots:
  void slotExit();
  void BarPlotUpdate();

private:
  Ui::BarPlot ui;
  // QPlotlyWindow *chart;
  QStringList windowtitles;
  QStringList labelnames;
  QString xaxestitle;
  QString yaxestitle;
  ChartQt *chart = nullptr;
  QList<dvector *> bars;
  void genBars(dvector *v, int split, double min, double max,
               QVector<qreal> *bval, QStringList *bnames);
};

#endif
