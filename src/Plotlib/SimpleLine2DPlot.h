// SimpleLine2DPlot.h
#ifndef SIMPLELINE2DPLOT_H
#define SIMPLELINE2DPLOT_H

#include <scientific.h>
#include <QWidget>
#include <QList>
#include <QColor>
#include "ui_SimpleLine2DPlot.h"


#include "QPlotly/qplotly.h"

class SimpleLine2DPlot: public QWidget
{
  Q_OBJECT

public:
  SimpleLine2DPlot(QList<matrix *> mlst, QStringList curvenames, QString windowtitle, QString xaxestitle, QString yaxestitle);
  SimpleLine2DPlot(matrix *m, QStringList curvenames, QString windowtitle, QString xaxestitle, QString yaxestitle);
  // Simple Line Plot with one x (first column of m) and more y objects
  SimpleLine2DPlot(matrix *m, QString curvename, QString windowtitle, QString xaxestitle, QString yaxestitle);
  void setPlotTitle(QString);

private slots:
  void slotExit();
  void SavePlotImage();

private:
  int random_(int low, int high);
  Ui::SimpleLine2DPlot ui;
  QPlotlyWindow *chart;
};

#endif
