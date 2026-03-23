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

#include "PlotSettingsDialog.h"

int PlotSettingsDialog::getPlotTitleSize() { return ui.titleSize->value(); }

int PlotSettingsDialog::getAxisValueSize() { return ui.axisValueSize->value(); }

int PlotSettingsDialog::getXlabelSize() { return ui.xLabelSize->value(); }

double PlotSettingsDialog::getXmin() { return ui.xMin->value(); }

double PlotSettingsDialog::getXmax() { return ui.xMax->value(); }

int PlotSettingsDialog::getXTick() { return ui.xTick->value(); }

int PlotSettingsDialog::getYlabelSize() { return ui.yLabelSize->value(); }

double PlotSettingsDialog::getYmin() { return ui.yMin->value(); }

double PlotSettingsDialog::getYmax() { return ui.yMax->value(); }

int PlotSettingsDialog::getYTick() { return ui.yTick->value(); }

void PlotSettingsDialog::Cancel() { return reject(); }

void PlotSettingsDialog::OK() { return accept(); }

PlotSettingsDialog::PlotSettingsDialog(double xmin, double xmax, int xtick,
                                       double ymin, double ymax, int ytick,
                                       int titlesize, int axisvaluesize,
                                       int xlabelsize, int ylabelsize)
    : QDialog(0) {
  ui.setupUi(this);
  setWindowTitle("Visualization Configuration");
  ui.xMin->setValue(xmin);
  ui.xMax->setValue(xmax);
  ui.xTick->setValue(xtick);
  ui.yMin->setValue(ymin);
  ui.yMax->setValue(ymax);
  ui.yTick->setValue(ytick);

  ui.titleSize->setValue(titlesize);
  ui.axisValueSize->setValue(axisvaluesize);
  ui.xLabelSize->setValue(xlabelsize);
  ui.yLabelSize->setValue(ylabelsize);

  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
}
