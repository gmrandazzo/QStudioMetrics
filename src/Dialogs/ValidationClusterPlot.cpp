#include "ValidationClusterPlot.h"
#include "run.h"

#include <QFileDialog>
#include <QLabel>

#include <cstdio>
#include <ctime>
#include <iostream>

int ValidationClusterPlot::getNClusters() { return ui.nclustersBox->value(); }

void ValidationClusterPlot::SavePlotImage() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Save Plot to Image"), "",
      tr("JPEG (*.jpg);;PNG (*.png);;All Files (*)"));

  if (!fileName.isEmpty()) {
    /*vtkWindowToImageFilter *filter = vtkWindowToImageFilter::New();
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
    filter->Delete();*/
  }
}

ValidationClusterPlot::ValidationClusterPlot(dvector *toplot,
                                             QString yaxestitle) {
  ui.setupUi(this);
  chart = new Chart();
  plotLayout = new QVBoxLayout();
  plotLayout->addWidget(chart);
  ui.plotwidget->setLayout(plotLayout);
  chart->setXaxisName("Number of clusters");
  chart->setYaxisName(yaxestitle);

  QVector<QPointF> curve;
  for (uint i = 0; i < toplot->size; i++) {
    curve << QPointF(i + 1, toplot->data[i]);
  }
  chart->addCurve(curve, "Cluster", Qt::blue);
  chart->Refresh();

  connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
  adjustSize();
}

ValidationClusterPlot::~ValidationClusterPlot() {
  delete chart;
  delete plotLayout;
}
