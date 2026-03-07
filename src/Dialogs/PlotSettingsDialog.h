/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for plotsettings operations.
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

#ifndef PLOTSETTINGSDIALOG_H
#define PLOTSETTINGSDIALOG_H

#include "qsmdata.h"
#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "ui_PlotSettingsDialog.h"

class PlotSettingsDialog : public QDialog {
  Q_OBJECT

public:
  PlotSettingsDialog(double xmin, double xmax, int xtick, double ymin,
                     double ymax, int ytick, int titlesize, int axisvaluesize,
                     int xlabelsize, int ylabelsize);
  int getPlotTitleSize();
  int getAxisValueSize();
  int getXlabelSize();
  double getXmin();
  double getXmax();
  int getXTick();
  int getYlabelSize();
  double getYmin();
  double getYmax();
  int getYTick();

private slots:
  void Cancel();
  void OK();

private:
  Ui::PlotSettingsDialog ui;
};

#endif
