#include <QtGui>
#include <QApplication>
#include "chart.h"

/*
void PlotFromfile(Plotter *plotter, const QString &fileName)
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
      plotter->addPoint(x, y, name);
    }
  }

//   plotter->getPoint(0)->setSelection(true);

//   plotter->getPoint(1)->setSelection(true);
  plotter->Refresh();
}
*/

double uniform0to1Random() {
    double r = qrand();
    return r / ((double)RAND_MAX + 1);
}

// Returns uniformly distributed random numbers from [-10.0, 10.0].
double randnum(double low, double high){
  return (high-low) * uniform0to1Random() - high;
}

void RandomPlot(Chart *chart)
{
  qsrand(20);
  for(int i = 0; i < 20; i++){
    qreal x = (qreal)randnum(-0.01, 0.01);
    qreal y = (qreal)randnum(-0.01, 0.01);
    QString name = QString("Obj%1").arg(i+1);
    chart->addPoint(x, y, name);
  }
//   chart->getPoint(0)->setSelection(true);

//   chart->getPoint(1)->setSelection(true);

  QVector< QPointF > line;
  line.append(QPointF(0,0));
  line.append(QPointF(0.01,0.01));

  chart->addCurve(line, "ciccio", Qt::red);
  chart->Refresh();
}

void GaussianPlot(Chart *chart)
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
  curve3.append(QPointF(2,2));

  chart->addCurve(curve1, "fit1", Qt::red);
  chart->addCurve(curve2, "fit2", Qt::blue);
  chart->addCurve(curve3, "fit3", Qt::black);

  chart->Refresh();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Chart chart;
    chart.setWindowTitle(QObject::tr("Chart"));
    //PlotFromfile(&plotter, argv[1]);
    //RandomPlot(&plotter);
    GaussianPlot(&chart);
    chart.setXaxisName("X axis");
    chart.setYaxisName("Y axis");
    chart.setPlotTitle("Test Plot...");
    chart.resize(400, 400);
    chart.show();
    return app.exec();
}
