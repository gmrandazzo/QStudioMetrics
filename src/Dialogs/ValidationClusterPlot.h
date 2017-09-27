// ValidationClusterPlot.h
#ifndef VALIDATIONCLUSTERPLOT_H
#define VALIDATIONCLUSTERPLOT_H

#include <scientific.h>
#include <QDialog>
#include <QList>
#include <QColor>
#include "ui_ValidationClusterPlot.h"


#include <vtkVersion.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderWindow.h>
#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkTable.h>
#include <vtkPlot.h>
#include <vtkFloatArray.h>
#include <vtkContextView.h>
#include <vtkContextScene.h>
#include <vtkPen.h>
#include <vtkAxis.h>

class ValidationClusterPlot: public QDialog
{
  Q_OBJECT
  
public:
  ValidationClusterPlot(dvector* toplot, QString yaxestitle);
  int getNClusters();

private slots:
  void SavePlotImage();
  
private:
  Ui::ValidationClusterPlot ui;
  vtkSmartPointer<vtkContextView> view;
  vtkSmartPointer<vtkChartXY> chart;
  vtkSmartPointer<vtkTable> table;
  vtkPlot* plots;
};

#endif

