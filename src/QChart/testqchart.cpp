#include <QtGui>
#include <QApplication>
#include "qchart.h"

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

void RandomPlot(QChart *qchart)
{
  qsrand(20);
  for(int i = 0; i < 20; i++){
    qreal x = (qreal)randnum(-0.01, 0.01);
    qreal y = (qreal)randnum(-0.01, 0.01);
    QString name = QString("Obj%1").arg(i+1);
    qchart->addPoint(x, y, name);
  }
//   qchart->getPoint(0)->setSelection(true);

//   qchart->getPoint(1)->setSelection(true);

  QVector< QPointF > line;
  line.append(QPointF(0,0));
  line.append(QPointF(0.01,0.01));

  qchart->addCurve(line, "ciccio", Qt::red);
  qchart->Refresh();
}

void GaussianPlot(QChart *qchart)
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
    qchart->addPoint(x, y, name);
    curve1.append(QPointF(x, y));
    cc++;
  }

  qreal dx = 7.5 / (steps-1);
  for (int i = 0; i < steps; ++i){
    qreal x = i * dx;
    qreal y = cos(i*dx);
    QString name = QString("Obj%1").arg(cc);
    qchart->addPoint(x, y, name);
    curve2.append(QPointF(x, y));
    cc++;
  }

  qchart->addPoint(0, 0, "zero");
  curve3.append(QPointF(0,0));
  qchart->addPoint(1, 1, "uno");
  curve3.append(QPointF(1,1));
  qchart->addPoint(2, 2, "due");
  curve3.append(QPointF(2,2));

  qchart->addCurve(curve1, "fit1", Qt::red);
  qchart->addCurve(curve2, "fit2", Qt::blue);
  qchart->addCurve(curve3, "fit3", Qt::black);

  qchart->Refresh();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QChart qchart;
    qchart.setWindowTitle(QObject::tr("QChart"));
    //PlotFromfile(&plotter, argv[1]);
    //RandomPlot(&plotter);
    GaussianPlot(&qchart);
    qchart.setXaxisName("X axis");
    qchart.setYaxisName("Y axis");
    qchart.setPlotTitle("Test Plot...");
    qchart.resize(400, 400);
    qchart.show();
    return app.exec();
}
