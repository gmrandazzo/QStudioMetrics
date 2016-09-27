#include "ValidationClusterPlot.h"
#include "run.h"

#include <QLabel>
#include <QFileDialog>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>

#include <iostream>
#include <cstdio>
#include <ctime>

int ValidationClusterPlot::getNClusters()
{
  return ui.nclustersBox->value();
}

void ValidationClusterPlot::SavePlotImage()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot to Image"), "", tr("JPEG (*.jpg);;PNG (*.png);;All Files (*)"));
  
  if(!fileName.isEmpty()){
    vtkWindowToImageFilter *filter = vtkWindowToImageFilter::New();
    filter->SetInput(ui.qvtkWidget->GetRenderWindow());
    if(fileName.split(".").last().compare("png", Qt::CaseInsensitive) == 0){
      vtkPNGWriter *jw = vtkPNGWriter::New();
      jw->SetInputData(filter->GetOutput());
      jw->SetFileName(fileName.toStdString().c_str());
      jw->Write();
      jw->Delete();
    }
    else{
      vtkJPEGWriter *jw = vtkJPEGWriter::New();
      jw->SetInputData(filter->GetOutput());
      jw->SetFileName(fileName.toStdString().c_str());
      jw->Write();
      jw->Delete();
    }
    filter->Delete();
  }
}

ValidationClusterPlot::ValidationClusterPlot(dvector* toplot, QString yaxestitle)
{
  ui.setupUi(this);
  
  view = vtkSmartPointer<vtkContextView>::New();
  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
  view->GetRenderWindow()->SetSize(400, 300);
 
  chart = vtkSmartPointer<vtkChartXY>::New();
  view->GetScene()->AddItem(chart);
  chart->SetShowLegend(true);
 
  // Create a table with some points in it...
  table =vtkSmartPointer<vtkTable>::New();
 
  // Test charting with a few more points..
  
  vtkSmartPointer<vtkFloatArray> arrX = 
    vtkSmartPointer<vtkFloatArray>::New();
  arrX->SetName("Number of Clusters");
  table->AddColumn(arrX);
 
  vtkSmartPointer<vtkFloatArray> arrY = vtkSmartPointer<vtkFloatArray>::New();
  arrY->SetName(yaxestitle.toStdString().c_str());
  table->AddColumn(arrY);
 
  table->SetNumberOfRows(toplot->size);  
  for(uint i = 0; i < toplot->size; i++){
    table->SetValue(i, 0, i);
    table->SetValue(i, 1, getDVectorValue(toplot, i));
  }
 
  vtkPlot *line = chart->AddPlot(vtkChart::LINE);
  line->SetInputData(table, 0, 1);
  line->SetColor(0, 255, 0, 255);
  line->SetWidth(2.0);
 
 
  //Finally render the scene
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle("Number of Clusters");
  chart->GetAxis(vtkAxis::LEFT)->SetTitle(yaxestitle.toStdString().c_str());
  
  chart->Modified();
  chart->Update();
  view->GetRenderWindow()->SetMultiSamples(1);
  
  // VTK/Qt wedded
  view->SetInteractor(this->ui.qvtkWidget->GetInteractor());
  this->ui.qvtkWidget->SetRenderWindow(view->GetRenderWindow());
  
  connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
  adjustSize();
}

