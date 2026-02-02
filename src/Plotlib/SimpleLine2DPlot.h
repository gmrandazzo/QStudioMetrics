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
