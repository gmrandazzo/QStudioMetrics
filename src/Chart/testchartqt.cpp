#include <QApplication>
#include <QFile>
#include <QtGlobal>
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
#include <QRandomGenerator>
#endif
#include "chartqt.h"
#include "plotinfo.h"
#include <cmath>

void PlotFromfile(ChartQt *chart, const QString &fileName) {
  QFile file(fileName);
  if (file.open(QIODevice::ReadOnly)) {
    QTextStream in(&file);
    while (!in.atEnd()) {
      QString line = in.readLine();
      QStringList fields = line.split(';'); //, QString::SkipEmptyParts);
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

void _BarPlotExample(ChartQt *chart) {
  QStringList x1, text1;
  QVector<qreal> y1;
  x1 << "JAN"
     << "FEB"
     << "MAR"
     << "APR"
     << "MAY"
     << "JUN"
     << "JUL"
     << "AUG"
     << "SEP"
     << "OCT"
     << "NOV"
     << "DEC";
  y1 << 19 << 14 << 22 << 16 << 17 << 21 << 28 << 32 << 27 << 25 << 17 << 12;
  text1 << "January"
        << "February"
        << "March"
        << "April"
        << "May"
        << "June"
        << "July"
        << "August"
        << "September"
        << "October"
        << "November"
        << "December";
  chart->addBars(x1, y1, text1, Qt::black);

  QStringList x2, text2;
  QVector<qreal> y2;
  x2 << "JAN"
     << "FEB"
     << "MAR"
     << "APR"
     << "MAY"
     << "JUN"
     << "JUL"
     << "AUG"
     << "SEP"
     << "OCT"
     << "NOV"
     << "DEC";
  y2 << 12 << 22 << 25 << 30 << 35 << 43 << 45 << 40 << 34 << 20 << 15 << 10;
  text2 << "January"
        << "February"
        << "March"
        << "April"
        << "May"
        << "June"
        << "July"
        << "August"
        << "September"
        << "October"
        << "November"
        << "December";

  chart->addBars(x2, y2, text2, Qt::blue);
}

double uniform0to1Random(QRandomGenerator rndgen) {
  double r = rndgen.generateDouble();
  return r / ((double)RAND_MAX + 1);
}

// Returns uniformly distributed random numbers from [-10.0, 10.0].
double randnum(double low, double high, QRandomGenerator rndgen) {
  return (high - low) * uniform0to1Random(rndgen) - high;
}

void _2DScattePlotExample(ChartQt *chart) {
  QList<int> domain;
  domain << -1 << 1;
  int steps = 100;
  qreal stepsize = (domain[1] - domain[0]) / ((steps - 1) * 1.0);
  int cc = 1;
  QVector<QPointF> curve1, curve2, curve3;
  for (int i = 0; i < steps; i++) {
    qreal x = domain[0] + stepsize * i;
    qreal y = exp(-((x - 0.12) * (x - 0.12)) / 0.23);
    QString name = QString("Obj%1").arg(cc);
    chart->addPoint(x, y, name);
    chart->getPoint(i)->setMarkerType(CIRCLE);
    curve1.append(QPointF(x, y));
    cc++;
  }

  qreal dx = 7.5 / (steps - 1);
  for (int i = 0; i < steps; ++i) {
    qreal x = i * dx;
    qreal y = cos(i * dx);
    QString name = QString("Obj%1").arg(cc);
    chart->addPoint(x, y, name);
    curve2.append(QPointF(x, y));
    cc++;
  }

  chart->addPoint(0, 0, "zero");
  chart->getPoint(chart->PointSize() - 1)->setMarkerType(SQUARE);
  curve3.append(QPointF(0, 0));
  chart->addPoint(1, 1, "uno");
  chart->getPoint(chart->PointSize() - 1)->setMarkerType(SQUARE);
  curve3.append(QPointF(1, 1));
  chart->addPoint(2, 2, "due");
  chart->getPoint(chart->PointSize() - 1)->setMarkerType(SQUARE);
  chart->getPoint(chart->PointSize() - 1)->setLabelVisible(true);
  curve3.append(QPointF(2, 2));

  chart->addCurve(curve1, "fit1", Qt::red);
  chart->addCurve(curve2, "fit2", Qt::blue);
  chart->addCurve(curve3, "fit3", Qt::black);

  chart->setXaxisName("Test X Axis");
  chart->setYaxisName("Test Y Axis");
  chart->setPlotTitle("Test Plot Title");
}

void _2DScattePlotExampleBIS(ChartQt *chart) {
  int n_points = 5000;
  QRandomGenerator rndgen(n_points);
  for (int i = 0; i < n_points; i++) {
#if (QT_VERSION >= QT_VERSION_CHECK(5, 10, 0))
    qreal x = QRandomGenerator::global()->generateDouble();
    qreal y = QRandomGenerator::global()->generateDouble();
#else
    qreal x = randnum(-10, 10, rndgen);
    qreal y = randnum(-10, 10, rndgen);
#endif
    QString name = QString("Obj%1").arg(i);
    chart->addPoint(x, y, name);
  }

  chart->setXaxisName("Test X Axis");
  chart->setYaxisName("Test Y Axis");
  chart->setPlotTitle("Test Plot Title");
}

void _CurvePlotExample(ChartQt *chart) {
  QVector<QPointF> curve;
  double x, dx;
  x = -6;
  dx = 0.1;
  while (x < 6) {
    double y = sin(x);
    curve.append(QPointF(x, y));
    x += dx;
  }
  chart->addCurve(curve, "curve", Qt::red);
}

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  ChartQt chart;
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
  chart.Plot();
  chart.resize(400, 300);
  chart.show();

  return app.exec();
}
