// BarPlot.h
// SimpleLine2DPlot.h
#ifndef BARPLOT_H
#define BARPLOT_H

#include "ui_BarPlot.h"
#include <QColor>
#include <QList>
#include <QWidget>

#include "Chart/chartqt.h"

#include "qsmdata.h"

extern "C" {
#include "scientific.h"
}

class BarPlot : public QWidget {
  Q_OBJECT

public:
  // Simple bar plot. Every value in v_ will be plotted in a bar and the height
  // of the bar correspond to the v_ value itself.
  BarPlot(dvector *v_, QStringList varnames, QString windowtitle,
          QWidget *parent = 0);

  BarPlot(dvector *v_, QStringList varnames, QString windowtitle,
          QString xaxestitle, QString yaxestitle, QWidget *parent = nullptr);

  BarPlot(QList<dvector *> v_, QString windowtitle, QString xaxestitle,
          QString yaxestitle, QStringList labelname, QWidget *parent = nullptr);

  BarPlot(QList<dvector *> bar_lists_, QStringList windowtitles, QString xaxestitle,
          QString yaxestitle, QStringList labelnames, QWidget *parent = nullptr);

  ~BarPlot();

private slots:
  void slotExit();
  void BarPlotUpdate();

private:
  Ui::BarPlot ui;
  // QPlotlyWindow *chart;
  QStringList windowtitles;
  QStringList labelnames;
  QString xaxestitle;
  QString yaxestitle;
  ChartQt *chart = nullptr;
  QList<dvector *> bars;
  void genBars(dvector *v, int split, double min, double max,
               QVector<qreal> *bval, QStringList *bnames);
};

#endif
