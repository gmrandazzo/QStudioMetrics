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
