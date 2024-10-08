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
                     double ymax, int ytick);
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
