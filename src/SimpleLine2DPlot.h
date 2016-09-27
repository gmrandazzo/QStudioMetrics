// SimpleLine2DPlot.h
#ifndef SIMPLELINE2DPLOT_H
#define SIMPLELINE2DPLOT_H

#include <scientific.h>
#include <QWidget>
#include <QList>
#include <QColor>
#include "ui_SimpleLine2DPlot.h"


#include "Plotter/plotter.h"

class SimpleLine2DPlot: public QWidget
{
  Q_OBJECT
  
public:
  SimpleLine2DPlot(matrix *m, QStringList curvenames, QString windowtitle, QStringList columntitle, QString xaxestitle, QString yaxestitle);
  // Simple Line Plot with one x (first column of m) and more y objects
  SimpleLine2DPlot(matrix *m, QString curvename, QString windowtitle, QString xaxestitle, QString yaxestitle);
  void setLabelDetail(bool);
  void setXminXmanXTicks(double xmin, double xmax, int xtick);
  void setYminYmanYTicks(double ymin, double ymax, int ytick);

private slots:
  void slotExit();
  void SavePlotImage();
  
private:
  Ui::SimpleLine2DPlot ui;
  Plotter *plotter;
};

#endif
