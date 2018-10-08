#include <QApplication>
#include "qplotly.h"
#include <cmath>

double uniform0to1Random() {
    double r = qrand();
    return r / ((double)RAND_MAX + 1);
}

// Returns uniformly distributed random numbers from [-10.0, 10.0].
double randnum(double low, double high){
  return (high-low) * uniform0to1Random() - high;
}

void _3DRandomPlotExample(QPlotlyWindow *chart)
{
  int npnt = 10000;
  qsrand(npnt);
  for(int i = 0; i < npnt; i++){
    qreal x = (qreal)randnum(-0.01, 0.01);
    qreal y = (qreal)randnum(-0.01, 0.01);
    qreal z = (qreal)randnum(-0.01, 0.01);
    QString name = QString("Obj%1").arg(i+1);
    chart->addPoint(x, y, z, name);
  }
}

void _2DScattePlotExample(QPlotlyWindow *chart)
{
  QList<int> domain;
  domain << -1 << 1;
  int steps = 100;
  qreal stepsize = (domain[1] - domain[0]) / ((steps - 1)*1.0);
  int cc = 1;
  QVector< QPointF > curve1, curve2, curve3;
  for(int i = 0; i < steps; i++){
    qreal x = domain[0] + stepsize * i;
    qreal y = exp(- ((x-0.12)*(x-0.12))/0.23);
    QString name = QString("Obj%1").arg(cc);
    chart->addPoint(x, y, name);
    curve1.append(QPointF(x, y));
    cc++;
  }

  qreal dx = 7.5 / (steps-1);
  for (int i = 0; i < steps; ++i){
    qreal x = i * dx;
    qreal y = cos(i*dx);
    QString name = QString("Obj%1").arg(cc);
    chart->addPoint(x, y, name);
    curve2.append(QPointF(x, y));
    cc++;
  }

  chart->addPoint(0, 0, "zero");
  curve3.append(QPointF(0,0));
  chart->addPoint(1, 1, "uno");
  curve3.append(QPointF(1,1));
  chart->addPoint(2, 2, "due");
  chart->getPoint(chart->PointSize()-1)->setLabelVisible(true);
  curve3.append(QPointF(2,2));

  chart->addCurve(curve1, "fit1", Qt::red);
  chart->addCurve(curve2, "fit2", Qt::blue);
  chart->addCurve(curve3, "fit3", Qt::black);

  chart->setXaxisName("Test X Axis");
  chart->setYaxisName("Test Y Axis");
  chart->setPlotTitle("Test Plot Title");

}


void _BarPlotExmaple(QPlotlyWindow *chart)
{
  QStringList x1, text1;
  QVector<qreal> y1;
  x1 << "JAN" << "FEB" << "MAR" << "APR" << "MAY" << "JUN" << "JUL" << "AUG" << "SEP" << "OCT" << "NOV" << "DEC";
  y1 << 19 << 14 << 22 << 16 << 17 << 21 << 28 << 32 << 27 << 25 << 17 << 12;
  text1 << "January" << "February" << "March" << "April" << "May" << "June" << "July" << "August" << "September" << "October" << "November" << "December";
  chart->addBars(x1, y1, text1, Qt::black);

  QStringList x2, text2;
  QVector<qreal> y2;
  x2 << "JAN" << "FEB" << "MAR" << "APR" << "MAY" << "JUN" << "JUL" << "AUG" << "SEP" << "OCT" << "NOV" << "DEC";
  y2 << 12 << 22 << 25 << 30 << 35 << 43 << 45 << 40 << 34 << 20 << 15 << 10;
  text2 << "January" << "February" << "March" << "April" << "May" << "June" << "July" << "August" << "September" << "October" << "November" << "December";

  chart->addBars(x2, y2, text2, Qt::blue);

}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  QPlotlyWindow chart;
  //_2DScattePlotExample(&chart);
  _BarPlotExmaple(&chart);
  /*_3DRandomPlotExample(&chart);*/
  chart.Plot();
  chart.show();
  return app.exec();
}
