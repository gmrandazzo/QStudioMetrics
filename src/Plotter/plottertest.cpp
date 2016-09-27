#include <QtGui>
#include "plotter.h"

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

void RandomPlot(Plotter *plotter)
{
  qsrand(20);
  for(int i = 0; i < 20; i++){
    qreal x = (qreal)randnum(-0.01, 0.01);
    qreal y = (qreal)randnum(-0.01, 0.01);
    QString name = QString("Obj%1").arg(i+1);
    plotter->addPoint(x, y, name);
  }
//   plotter->getPoint(0)->setSelection(true);
  
//   plotter->getPoint(1)->setSelection(true);

  QVector< QPointF > line;
  line.append(QPointF(0,0));
  line.append(QPointF(0.01,0.01));
  
  plotter->addCurve(line, "ciccio", Qt::red);
  plotter->Refresh();
}

void GaussianPlot(Plotter *plotter)
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
    plotter->addPoint(x, y, name);
    curve1.append(QPointF(x, y));
    cc++;
  }
  
  qreal dx = 7.5 / (steps-1);
  for (int i = 0; i < steps; ++i){
    qreal x = i * dx;
    qreal y = cos(i*dx);
    QString name = QString("Obj%1").arg(cc);
    plotter->addPoint(x, y, name);
    curve2.append(QPointF(x, y));
    cc++;
  }
  
  plotter->addPoint(0, 0, "zero");
  curve3.append(QPointF(0,0));
  plotter->addPoint(1, 1, "uno");
  curve3.append(QPointF(1,1));
  plotter->addPoint(2, 2, "due");
  curve3.append(QPointF(2,2));

  plotter->addCurve(curve1, "fit1", Qt::red);
  plotter->addCurve(curve2, "fit2", Qt::blue);
  plotter->addCurve(curve3, "fit3", Qt::black);
  
  plotter->Refresh();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Plotter plotter;
    plotter.setWindowTitle(QObject::tr("Plotter"));
//     PlotFromfile(&plotter, argv[1]);
    //RandomPlot(&plotter);
    GaussianPlot(&plotter);
    plotter.setXaxisName("X axis");
    plotter.setYaxisName("Y axis");
    plotter.setPlotTitle("Test Plot...");
    plotter.resize(400, 400);
    plotter.show();
    return app.exec();
}
