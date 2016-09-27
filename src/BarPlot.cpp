#include "BarPlot.h"

#include <QDialog>
#include <QString>
#include <QFileDialog>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>
#include <vtkStringArray.h>
#include <unistd.h>

void BarPlot::slotExit()
{
  qApp->exit();
}

void BarPlot::PlotUpdate()
{  
  chart->Modified();
  chart->Update();
  ui.qvtkWidget->repaint();
}

void BarPlot::RescalePlot()
{
  chart->RecalculateBounds();
  ui.qvtkWidget->repaint();
}

void BarPlot::SavePlotImage()
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

BarPlot::BarPlot(dvector *v_, QStringList varnames, QString windowtitle, QString xaxestitle, QString yaxestitle)
{
  ui.setupUi(this);
  
  setWindowTitle(windowtitle);

  // deep copy;
  initDVector(&v);
  DVectorCopy(v_, &v);
  
  // Set up a 2D scene, add an XY chart to it
  view = vtkSmartPointer<vtkContextView>::New();
  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
  view->GetRenderWindow()->SetSize(400, 300);
  
  chart = vtkSmartPointer<vtkChartXY>::New();
  view->GetScene()->AddItem(chart);
  chart->SetTitle(windowtitle.toStdString());
  chart->SetShowLegend(false);
 
  // Create a table with some points in it...
  table = vtkSmartPointer<vtkTable>::New();
 
  vtkSmartPointer<vtkDoubleArray> arrStep = vtkSmartPointer<vtkDoubleArray>::New();
  arrStep->SetName("Step Size");
  table->AddColumn(arrStep);
  
  vtkSmartPointer<vtkDoubleArray> arrVar = vtkSmartPointer<vtkDoubleArray>::New();
  arrVar->SetName("Variables");
  table->AddColumn(arrVar);
  
  vtkSmartPointer<vtkStringArray> xLabels;
  if(varnames.size() > 0){
    xLabels = vtkSmartPointer<vtkStringArray>::New();
    xLabels->SetNumberOfValues(varnames.size());
    for(int i = 0; i < varnames.size(); i++){
      xLabels->SetValue(i, varnames[i].toStdString().c_str());
    }
  }

  table->SetNumberOfRows(v->size);
  for (uint i = 0; i < v->size; i++){
    table->SetValue(i, 0, i+1);
    table->SetValue(i, 1, getDVectorValue(v, i));
  }
  
  int QtColours[]= {9, 7, 8, 2, 14, 13, 15, 10, 16, 11, 17, 12, 18, 5, 4, 6, 19}; // used for get the different colors
  // Add multiple line plots, setting the colors etc
  vtkPlot *plot = chart->AddPlot(vtkChart::BAR);
  plot->SetInputData(table, 0, 1);

  if(varnames.size() > 0)
    plot->SetIndexedLabels(xLabels);
  
  QColor color = QColor((Qt::GlobalColor)QtColours[0]);
  plot->SetColor(color.redF(), color.greenF(), color.blueF());
  
  chart->SetSelectionMode(vtkContextScene::SELECTION_NONE);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle(xaxestitle.toStdString().c_str());
  chart->GetAxis(vtkAxis::LEFT)->SetTitle(yaxestitle.toStdString().c_str());
  //Finally render the scene
  chart->Update();
  view->GetRenderWindow()->SetMultiSamples(0);
  
  // VTK/Qt wedded
  view->SetInteractor(ui.qvtkWidget->GetInteractor()); 
  ui.qvtkWidget->SetRenderWindow(view->GetRenderWindow());
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(ui.rescaleplotButton, SIGNAL(clicked(bool)), SLOT(RescalePlot()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
  PlotUpdate();
}

BarPlot::BarPlot(QList<dvector*> vlst_, QString windowtitle, QString xaxestitle, QString yaxestitle, QStringList labelname)
{
  ui.setupUi(this);
  
  setWindowTitle(windowtitle);
  
  v = 0;
  // deep copy
  for(int i = 0; i < vlst_.size(); i++){
    vlst.append(new dvector);
    initDVector(&vlst.last());
    DVectorCopy(vlst_[i], &vlst.last());
  }
  
  // Set up a 2D scene, add an XY chart to it
  view = vtkSmartPointer<vtkContextView>::New();
  view->GetRenderer()->SetBackground(1.0, 1.0, 1.0);
  view->GetRenderWindow()->SetSize(400, 300);
  
  chart = vtkSmartPointer<vtkChartXY>::New();
  view->GetScene()->AddItem(chart);
  chart->SetTitle(windowtitle.toStdString());
  chart->SetShowLegend(true);
 
  // Create a table with some points in it...
  table = vtkSmartPointer<vtkTable>::New();
 
  vtkSmartPointer<vtkDoubleArray> arrStep = vtkSmartPointer<vtkDoubleArray>::New();
  arrStep->SetName("Step Size");
  table->AddColumn(arrStep);
  
  int c = 0;
  for(int i = 0; i < vlst.size(); i++){
    vtkSmartPointer<vtkDoubleArray> arr = vtkSmartPointer<vtkDoubleArray>::New();
    if(i < labelname.size()){
      arr->SetName(labelname[i].toStdString().c_str());
    }
    else{
      arr->SetName(QString("Generic %1").arg(c).toStdString().c_str());
      c++;
    }
    table->AddColumn(arr);
  }
 
  //get min and max....
  double min, max;
  DVectorMinMax(vlst[0], &min, &max);
  
  for(int i = 1; i < vlst.size(); i++){
    double tmp_min = 0.f, tmp_max = 0.f;
    DVectorMinMax(vlst[i], &tmp_min, &tmp_max);
    if(tmp_min < min){
      min = tmp_min;
    }
    
    if(tmp_max > max){
      max = tmp_max;
    }
  }
  
  // step of 20
  double step = (max - min) / 20;
  table->SetNumberOfRows(20);
  double from = min, to = min+step;
  for (int i = 0; i < 20; i++){
    table->SetValue(i, 0, from);
    for(int k = 0; k < vlst.size(); k++){
      int nobj = 0;
      for(uint j = 0; j < vlst[k]->size; j++){
        double val = getDVectorValue(vlst[k], j);
        if((val > from && val < to) || (FLOAT_EQ(val, from, 1e-6) || FLOAT_EQ(val, to, 1e-6))){
          nobj++;
        }
        else{
          continue;
        }
      }
      table->SetValue(i, k+1, nobj);
    }
    from += step;
    to += step;
  }

  int QtColours[]= {9, 7, 8, 2, 14, 13, 15, 10, 16, 11, 17, 12, 18, 5, 4, 6, 19}; // used for get the different colors
  // Add multiple line plots, setting the colors etc
  for(int i = 0; i < vlst.size(); i++){
    vtkPlot *plot = chart->AddPlot(vtkChart::BAR);
    plot->SetInputData(table, 0, i+1);
    
    QColor color;
    if(i < (int)(sizeof(QtColours)/sizeof(QtColours[0]))){
      color = QColor((Qt::GlobalColor)QtColours[i]);
    }
    else{
      color.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
    }
    
    plot->SetColor(color.redF(), color.greenF(), color.blueF());
  }
  
  chart->SetSelectionMode(vtkContextScene::SELECTION_NONE);
  chart->GetAxis(vtkAxis::BOTTOM)->SetTitle(xaxestitle.toStdString().c_str());
  chart->GetAxis(vtkAxis::LEFT)->SetTitle(yaxestitle.toStdString().c_str());
  //Finally render the scene
  chart->Update();
  view->GetRenderWindow()->SetMultiSamples(0);
  
  // VTK/Qt wedded
  view->SetInteractor(ui.qvtkWidget->GetInteractor()); 
  ui.qvtkWidget->SetRenderWindow(view->GetRenderWindow());
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(ui.rescaleplotButton, SIGNAL(clicked(bool)), SLOT(RescalePlot()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
  PlotUpdate();
}

BarPlot::~BarPlot()
{
  if(v != 0){
    DelDVector(&v);
  }
  
  for(int i = 0; i < vlst.size(); i++){
    if(vlst[i]->size > 0){
      DelDVector(&vlst[i]);
    }
    else
      continue;
  }
  vlst.clear();
}
