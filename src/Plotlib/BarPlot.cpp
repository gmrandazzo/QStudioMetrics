#include "BarPlot.h"

#include <QDialog>
#include <QString>
#include <QFileDialog>

#include <unistd.h>

#include "QPlotly/qplotly.h"

void BarPlot::slotExit()
{
  qApp->exit();
}

void BarPlot::genBars(dvector *v, int split, double min, double max, QVector<qreal> *bval, QStringList *bnames)
{
  size_t i, j;
  double step, x;
  step = (max-min)/(double)split;

  QList<double> dx;

  x = min+step;
  for(i = 0; i < split; i++){
    (*bval) << 0.f;
    dx << x;
    (*bnames).append(QString("%1 - %2").arg(QString::number(x-step, 'f', 2)).arg(QString::number(dx[i], 'f', 2)));
    x+=step;
  }

  for(i = 0; i < v->size; i++){
    for(j = 0; j < dx.size()-1; j++){
      if(v->data[i] < dx[j]){
        break;
      }
      else{
        continue;
      }
    }
    (*bval)[j] += 1;
  }
}

BarPlot::BarPlot(dvector *v_, QStringList varnames, QString windowtitle, QString xaxestitle, QString yaxestitle, QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);
  setWindowTitle(windowtitle);
  chart = new QPlotlyWindow(this);
  chart->setXaxisName(xaxestitle);
  chart->setYaxisName(yaxestitle);
  chart->setPlotTitle(windowtitle);


  QStringList bnames;
  QVector<qreal> y;
  double min, max;
  DVectorMinMax(v_, &min, &max);
  genBars(v_, 10.f, min, max, &y, &bnames);

  chart->addBars(bnames, y, bnames, Qt::black);

  QVBoxLayout *plotLayout = new QVBoxLayout();
  chart = new QPlotlyWindow(this);
  plotLayout->addWidget(chart);
  ui.widget->setLayout(plotLayout);
  //Finally render the scene
  chart->weview()->setContextMenuPolicy(Qt::NoContextMenu);
  chart->Refresh();
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

BarPlot::BarPlot(QList<dvector*> vlst_, QString windowtitle, QString xaxestitle, QString yaxestitle, QStringList labelname, QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);

  setWindowTitle(windowtitle);
  chart = new QPlotlyWindow(this);
  chart->setXaxisName(xaxestitle);
  chart->setYaxisName(yaxestitle);
  chart->setPlotTitle(windowtitle);
  QVBoxLayout *plotLayout = new QVBoxLayout();
  plotLayout->addWidget(chart);
  ui.widget->setLayout(plotLayout);

  int QtColours[]= {9, 7, 8, 2, 14, 13, 15, 10, 16, 11, 17, 12, 18, 5, 4, 6, 19}; // used for get the different colors

  double min, max;

  for(size_t i = 0; i < vlst_.size(); i++){
    double t_min, t_max;
    DVectorMinMax(vlst_[i], &t_min, &t_max);
    if(i == 0){
      min = t_min;
      max = t_max;
    }
    else{
      if(t_min < min)
        min = t_min;

      if(t_max > max)
        t_max = max;
    }
  }

  for(size_t i = 0; i < vlst_.size(); i++){
    QStringList bnames;
    QVector<qreal> y;
    QStringList text;
    genBars(vlst_[i], 10.f, min, max, &y, &bnames);

    QColor color;
    if(i < (int)(sizeof(QtColours)/sizeof(QtColours[0]))){
      color = QColor((Qt::GlobalColor)QtColours[i]);
    }
    else{
      color.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
    }

    for(size_t j = 0; j < bnames.size(); j++)
      text << labelname[i];

    chart->addBars(bnames, y, text, color);
  }
  //Finally render the scene
  chart->weview()->setContextMenuPolicy(Qt::NoContextMenu);
  chart->Refresh();

  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

BarPlot::~BarPlot()
{
  delete chart;
}
