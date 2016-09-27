#include "SimpleLine2DPlot.h"

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QFileDialog>

#include <iostream>
#include <cstdio>
#include <ctime>

#include <QDebug>

void SimpleLine2DPlot::slotExit() 
{
  qApp->exit();
}


int random(int low, int high)
{
  return rand() % (high - low + 1) + low;
}

void SimpleLine2DPlot::SavePlotImage()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot to Image"), "", tr("JPEG (*.jpg);;PNG (*.png);;All Files (*)"));
  if(!fileName.isEmpty()){
    plotter->SaveAsImage(fileName);
  }
}

void SimpleLine2DPlot::setLabelDetail(bool labeldetail_)
{
  plotter->setLabelDetail(labeldetail_);
}

void SimpleLine2DPlot::setXminXmanXTicks(double xmin, double xmax, int xtick)
{
    plotter->setXminXmaxXTick(xmin, xmax, xtick);
}

void SimpleLine2DPlot::setYminYmanYTicks(double ymin, double ymax, int ytick)
{
    plotter->setYminYmaxYTick(ymin, ymax, ytick);
}

SimpleLine2DPlot::SimpleLine2DPlot(matrix *m, QString curvename, QString windowtitle, QString xaxestitle, QString yaxestitle)
{
  ui.setupUi(this);

  setWindowTitle(windowtitle);
  
  QVBoxLayout *plotLayout = new QVBoxLayout();
  plotter = new Plotter();
  plotLayout->addWidget(plotter);
  ui.plotwidget->setLayout(plotLayout);
  
  plotter->setXaxisName(xaxestitle);
  plotter->setYaxisName(yaxestitle);

  for(uint j = 1; j < m->col; j++){
    //QVector< QPointF > curve;
    for(uint i = 0; i < m->row; i++){
      qreal x = m->data[i][0];
      qreal y = m->data[i][j];
      plotter->addPoint(x, y, QString("%1").arg(curvename), Qt::black, 5);
      //curve.append(QPointF(x, y));
    }
    //plotter->addCurve(curve, QString("%1").arg(curvenames[j-1]), Qt::blue); Mean
    //plotter->addCurve(curve, QString("%1").arg(curvenames[j-1]), Qt::red); Mediane
  }
  
  plotter->Refresh();
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
}

SimpleLine2DPlot::SimpleLine2DPlot(matrix *m, QStringList curvenames, QString windowtitle, QStringList columntitle, QString xaxestitle, QString yaxestitle)
{
  ui.setupUi(this);

  setWindowTitle(windowtitle);
  
  QVBoxLayout *plotLayout = new QVBoxLayout();
  plotter = new Plotter();
  plotLayout->addWidget(plotter);
  ui.plotwidget->setLayout(plotLayout);
  
  // Set default colors
  QList<QColor> colors;
  colors << Qt::black << Qt::blue << Qt::red << Qt::green << Qt::magenta << Qt::cyan;
  
  if(m->col > (uint)colors.size()){
    srand(time(0));
    for(uint i = 0; i < m->col - colors.size(); i++){
      colors.append(QColor(random(0, 256), random(0, 256), random(0, 256)));
    }
  }

  if(columntitle.size() < (int)m->col)
    return;
  
  plotter->setXaxisName(xaxestitle);
  plotter->setYaxisName(yaxestitle);
  
  for(uint j = 1; j < m->col; j++){
    QVector< QPointF > curve;
    for(uint i = 0; i < m->row; i++){
      qreal x = m->data[i][0];
      qreal y = m->data[i][j];
      plotter->addPoint(x, y, QString("%1").arg(curvenames[j-1]), colors[j-1], 5);
      curve.append(QPointF(x, y));
    }
    plotter->addCurve(curve, QString("%1").arg(curvenames[j-1]), colors[j-1]);
  }
  
  plotter->Refresh();
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
}
