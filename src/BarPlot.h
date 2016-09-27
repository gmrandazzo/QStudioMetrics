// BarPlot.h
// SimpleLine2DPlot.h
#ifndef BARPLOT_H
#define BARPLOT_H

#include <QWidget>
#include <QList>
#include <QColor>
#include "ui_BarPlot.h"

#include <vtkVersion.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkPlot.h>
#include <vtkTable.h>
#include <vtkDoubleArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxis.h>

#include "data.h"

extern "C" {
  #include "scientific.h"
}

class BarPlot: public QWidget
{
  Q_OBJECT
  
public:
  BarPlot(dvector *v_, QStringList varnames, QString windowtitle, QString xaxestitle, QString yaxestitle);
  BarPlot(QList<dvector*> v_, QString windowtitle, QString xaxestitle, QString yaxestitle, QStringList labelname);
  ~BarPlot();

private slots:
  void slotExit();
  void SavePlotImage();
  void RescalePlot();
  void PlotUpdate();
  
private:
  Ui::BarPlot ui;
  QList<dvector*> vlst;
  dvector *v;
  QList<QColor> colors;
  
  vtkSmartPointer<vtkContextView> view;
  vtkSmartPointer<vtkChartXY> chart;
  vtkSmartPointer<vtkTable> table;
};

#endif

