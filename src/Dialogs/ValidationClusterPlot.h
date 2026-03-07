/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for validationclusterplot.
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

// ValidationClusterPlot.h
#ifndef VALIDATIONCLUSTERPLOT_H
#define VALIDATIONCLUSTERPLOT_H

#include "Chart/chart.h"
#include "ui_ValidationClusterPlot.h"
#include <QColor>
#include <QDialog>
#include <QList>
#include <scientific.h>

class ValidationClusterPlot : public QDialog {
  Q_OBJECT

public:
  ValidationClusterPlot(dvector *toplot, QString yaxestitle);
  ~ValidationClusterPlot();
  int getNClusters();

private slots:
  void SavePlotImage();

private:
  Ui::ValidationClusterPlot ui;
  Chart *chart;
  QVBoxLayout *plotLayout;
};

#endif
