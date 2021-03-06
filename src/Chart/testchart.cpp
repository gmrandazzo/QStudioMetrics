#include <QApplication>
#include <QtGlobal>
#include <QFile>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
#include <QRandomGenerator>
#endif
#include "plotinfo.h"
#include "chart.h"
#include <cmath>

void PlotFromfile(Chart *chart, const QString &fileName)
{
  QFile file(fileName);
  if(file.open(QIODevice::ReadOnly)){
    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine();
      QStringList fields = line.split(';', QString::SkipEmptyParts);
      qreal x = fields[1].toDouble();
      qreal y = fields[2].toDouble();
      QString name = fields[0];
//       qDebug() << x << y << name;
      chart->addPoint(x, y, name);
    }
  }

//   plotter->getPoint(0)->setSelection(true);

//   plotter->getPoint(1)->setSelection(true);
  chart->Refresh();
}

double uniform0to1Random(QRandomGenerator rndgen) {
    double r = rndgen.generateDouble();
    return r / ((double)RAND_MAX + 1);
}

// Returns uniformly distributed random numbers from [-10.0, 10.0].
double randnum(double low, double high, QRandomGenerator rndgen){
  return (high-low) * uniform0to1Random(rndgen) - high;
}

void _2DScattePlotExample(Chart *chart)
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
    chart->getPoint(i)->setMarkerType(TRIANGLE);
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
  chart->getPoint(chart->PointSize()-1)->setMarkerType(SQUARE);
  curve3.append(QPointF(0,0));
  chart->addPoint(1, 1, "uno");
  chart->getPoint(chart->PointSize()-1)->setMarkerType(SQUARE);
  curve3.append(QPointF(1,1));
  chart->addPoint(2, 2, "due");
  chart->getPoint(chart->PointSize()-1)->setMarkerType(SQUARE);
  chart->getPoint(chart->PointSize()-1)->setLabelVisible(true);
  curve3.append(QPointF(2,2));

  chart->addCurve(curve1, "fit1", Qt::red);
  chart->addCurve(curve2, "fit2", Qt::blue);
  chart->addCurve(curve3, "fit3", Qt::black);

  chart->setXaxisName("Test X Axis");
  chart->setYaxisName("Test Y Axis");
  chart->setPlotTitle("Test Plot Title");
}

void _2DScattePlotExampleBIS(Chart *chart)
{
  int n_points = 100000;
  QRandomGenerator rndgen(n_points);
  for(int i = 0; i < n_points; i++){
    #if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    qreal x = QRandomGenerator::global()->generateDouble();
    qreal y = QRandomGenerator::global()->generateDouble();
    #else
    qreal x = randnum(-10,10,rndgen);
    qreal y = randnum(-10,10,rndgen);
    #endif
    QString name = QString("Obj%1").arg(i);
    chart->addPoint(x, y, name);
  }

  chart->setXaxisName("Test X Axis");
  chart->setYaxisName("Test Y Axis");
  chart->setPlotTitle("Test Plot Title");
}

void _CurvePlotExample(Chart *chart)
{
  QVector< QPointF > curve;
  double x, dx;
  x = -6;
  dx = 0.1;
  while(x < 6){
    double y = sin(x);
    curve.append(QPointF(x, y));
    x+=dx;
  }
  chart->addCurve(curve, "curve", Qt::red);
}

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  Chart chart;
  int major, minor, patch;
  QPlotVersion(&major, &minor, &patch);
  printf("QPlot Version %d.%d.%d\n", major, minor, patch);
  _2DScattePlotExample(&chart);
  //_2DScattePlotExampleBIS(&chart);
  //_BarPlotExample(&chart);
  //_CurvePlotExample(&chart);
  chart.setXaxisName("X axis");
  chart.setYaxisName("Y axis");
  chart.setPlotTitle("Test Plot...");
  chart.Refresh();
  chart.show();
  return app.exec();
}
