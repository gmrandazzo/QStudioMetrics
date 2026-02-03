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
