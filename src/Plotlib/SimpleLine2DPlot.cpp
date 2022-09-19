#include "SimpleLine2DPlot.h"

#include <QPushButton>
#include <QLabel>
#include <QSpinBox>
#include <QFileDialog>

#include <iostream>
#include <cstdio>
#include <ctime>


void SimpleLine2DPlot::slotExit()
{
  qApp->exit();
}


int SimpleLine2DPlot::random_(int low, int high)
{
  return rand() % (high - low + 1) + low;
}

void SimpleLine2DPlot::SavePlotImage()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot to Image"), "", tr("JPEG (*.jpg);;PNG (*.png);;All Files (*)"));
  if(!fileName.isEmpty()){
    chart->SaveAsImage(fileName);
  }
}

void SimpleLine2DPlot::setXminXmaxXTick(double xmin, double xmax, int xtick)
{
  chart->setXminXmaxXTick(xmin, xmax, xtick);
}

void SimpleLine2DPlot::setYminYmaxYTick(double ymin, double ymax, int ytick)
{
  chart->setYminYmaxYTick(ymin, ymax, ytick);
}

void SimpleLine2DPlot::setPlotTitle(QString title)
{
  chart->setPlotTitle(title);
}

SimpleLine2DPlot::SimpleLine2DPlot(matrix *m, QString curvename, QString windowtitle, QString xaxestitle, QString yaxestitle)
{
  ui.setupUi(this);
  setWindowTitle(windowtitle);

  QVBoxLayout *plotLayout = new QVBoxLayout();
  chart = new Chart(this);
  plotLayout->addWidget(chart);
  ui.plotwidget->setLayout(plotLayout);


  for(uint j = 1; j < m->col; j++){
    //QVector< QPointF > curve;
    for(uint i = 0; i < m->row; i++){
      qreal x = m->data[i][0];
      qreal y = m->data[i][j];
      chart->addPoint(x, y, QString("%1").arg(curvename), Qt::black, 5);
      //curve.append(QPointF(x, y));
    }
    //chart->addCurve(curve, QString("%1").arg(curvenames[j-1]), Qt::blue); Mean
    //chart->addCurve(curve, QString("%1").arg(curvenames[j-1]), Qt::red); Mediane
  }

  chart->setXaxisName(xaxestitle);
  chart->setYaxisName(yaxestitle);
  chart->Plot();
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
}

SimpleLine2DPlot::SimpleLine2DPlot(matrix *m, QStringList curvenames, QString windowtitle, QString xaxestitle, QString yaxestitle)
{
  ui.setupUi(this);
  setWindowTitle(windowtitle);

  QVBoxLayout *plotLayout = new QVBoxLayout();
  chart = new Chart(this);
  plotLayout->addWidget(chart);
  ui.plotwidget->setLayout(plotLayout);

  // Set default colors
  QList<QColor> colors;
  colors << Qt::black << Qt::blue << Qt::red << Qt::green << Qt::magenta << Qt::cyan;

  if(m->col > (uint)colors.size()){
    srand(time(0));
    for(uint i = 0; i < m->col - colors.size(); i++){
      colors.append(QColor(random_(0, 256), random_(0, 256), random_(0, 256)));
    }
  }

  for(uint j = 1; j < m->col; j++){
    QVector< QPointF > curve;
    for(uint i = 0; i < m->row; i++){
      qreal x = m->data[i][0];
      qreal y = m->data[i][j];
      chart->addPoint(x, y, QString("%1").arg(curvenames[j-1]), colors[j-1], 5);
      curve.append(QPointF(x, y));
    }
    chart->addCurve(curve, QString("%1").arg(curvenames[j-1]), colors[j-1]);
    //chart->setCurveStyle(j-1, LM);
  }

  chart->setXaxisName(xaxestitle);
  chart->setYaxisName(yaxestitle);
  chart->Plot();

  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
}

SimpleLine2DPlot::SimpleLine2DPlot(QList< matrix* > mlst, QStringList curvenames, QString windowtitle, QString xaxestitle, QString yaxestitle)
{
  ui.setupUi(this);
  setWindowTitle(windowtitle);

  QVBoxLayout *plotLayout = new QVBoxLayout();
  chart = new Chart(this);
  plotLayout->addWidget(chart);
  ui.plotwidget->setLayout(plotLayout);

  // Set default colors
  QList<QColor> colors;
  colors << Qt::black << Qt::blue << Qt::red << Qt::green << Qt::magenta << Qt::cyan;
  // Generate some colors
  if(mlst.size() > colors.size()){
    srand(time(0));
    for(int i = 0; i < mlst.size() - colors.size(); i++){
      colors.append(QColor(random_(0, 256), random_(0, 256), random_(0, 256)));
    }
  }

  for(int i = 0; i < mlst.size(); i++){
    QVector< QPointF > curve;
    for(uint j = 0; j < mlst[i]->row; j++){
      qreal x = mlst[i]->data[j][0];
      qreal y = mlst[i]->data[j][1];
      curve.append(QPointF(x, y));
    }
    chart->addCurve(curve, QString("%1").arg(curvenames[i]), colors[i]);
    //chart->setCurveStyle(i, LM);
  }

  chart->setXaxisName(xaxestitle);
  chart->setYaxisName(yaxestitle);
  chart->Plot();
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  connect(ui.saveimageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
}
