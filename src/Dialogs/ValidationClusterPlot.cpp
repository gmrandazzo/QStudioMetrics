/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for validationclusterplot.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

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
  chart->addCurve(curve, "Cluster", Qt::blue, true);
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
