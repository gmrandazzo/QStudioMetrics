// BarPlot.h
// SimpleLine2DPlot.h
#ifndef BARPLOT_H
#define BARPLOT_H

#include <QWidget>
#include <QList>
#include <QColor>
#include "ui_BarPlot.h"

#include "QPlotly/qplotly.h"

#include "qsmdata.h"

extern "C" {
  #include "scientific.h"
}

class BarPlot: public QWidget
{
  Q_OBJECT

public:
  BarPlot(dvector *v_, QStringList varnames, QString windowtitle, QString xaxestitle, QString yaxestitle, QWidget *parent = 0);
  BarPlot(QList<dvector*> v_, QString windowtitle, QString xaxestitle, QString yaxestitle, QStringList labelname, QWidget *parent = 0);
  ~BarPlot();

private slots:
  void slotExit();

private:
  Ui::BarPlot ui;
  QPlotlyWindow *chart;
  void genBars(dvector *v, int split, double min, double max, QVector<qreal> *bval, QStringList *bnames);
};

#endif
