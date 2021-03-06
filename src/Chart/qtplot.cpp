#include "qtplot.h"
#include "qplotly.h"

#include <QVBoxLayout>
#include <QWidget>

#include <QChartGlobal>
#include <QChartView>
#include <QScatterSeries>

#include <Q3DScatter>


void QPlotWindow::Plot()
{
  QScatterSeries *m_scatter = new QScatterSeries();

}

void QPlotWindow::setXaxisName(QString xaxisname_)
{
  xaxisname = xaxisname_;
}

void QPlotWindow::setYaxisName(QString yaxisname_)
{
  yaxisname = yaxisname_;
}

void QPlotWindow::setZaxisName(QString zaxisname_)
{
  zaxisname = zaxisname_;
}

void QPlotWindow::setPlotTitle(QString plot_title_)
{
  plot_title = plot_title_;
}

void QPlotWindow::addPoint(qreal x, qreal y, QString name)
{
  #ifdef DEBUG
  printf("QPlotWindow::addPoint(qreal x, qreal y, QString name)\n");
  #endif
  p[name] = new DataPoint(x, y, name);
}

void QPlotWindow::addPoint(qreal x, qreal y, QString name, QColor color, int radius)
{
  #ifdef DEBUG
  printf("QPlotWindow::addPoint(qreal x, qreal y, QString name, QColor color, int radius)\n");
  #endif
  p[name] = new DataPoint(x, y, name);
  p[name]->setColor(color);
  p[name]->setRadius(radius);
}

void QPlotWindow::addPoint(qreal x, qreal y, qreal z, QString name)
{
  pt = _3D;
  #ifdef DEBUG
  printf("QPlotWindow::addPoint(qreal x, qreal y, QString name)\n");
  #endif
  p[name] = new DataPoint(x, y, z, name);
}

void QPlotWindow::addPoint(qreal x, qreal y, qreal z, QString name, QColor color, int radius)
{
  pt = _3D;
  #ifdef DEBUG
  printf("QPlotWindow::addPoint(qreal x, qreal y, QString name, QColor color, int radius)\n");
  #endif
  p[name] = new DataPoint(x, y, z, name);
  p[name]->setColor(color);
  p[name]->setRadius(radius);
}

void QPlotWindow::addCurve(QVector< QPointF > curve, QString name, QColor color)
{
  #ifdef DEBUG
  printf("QPlotWindow::addCurve\n");
  #endif
  c[name] = new DataCurve(curve, name, color);
}

void QPlotWindow::setCurveStyle(int indx, LTYPE cs){
  if(indx < c.count()){
    c.values()[indx]->setStyle(cs);
  }
}

void QPlotWindow::addBars(QStringList x, QVector<qreal> y, QStringList text, QColor color)
{
  #ifdef DEBUG
  printf("QPlotWindow::addBars(QStringList x, QVector<qreal> y, QStringList text)\n");
  #endif
  b.append(new DataBar(x, y, text));
  b.last()->setColor(color);
}

int QPlotWindow::PointSize() const
{
  #ifdef DEBUG
  printf("QPlotWindow::PointSize\n");
  #endif
  return p.count();
}

DataPoint* QPlotWindow::getPoint(int id)
{
  #ifdef DEBUG
  printf("QPlotWindow::getPoint\n");
  #endif
  if(id < p.count())
    return p.values()[id];
  else
    return 0;
}

void QPlotWindow::RemovePointAt(int id)
{
  #ifdef DEBUG
  printf("QPlotWindow::RemovePointAt\n");
  #endif
  if(id < p.count()){
    QString key = p.keys()[id];
    delete p[key];
    p.remove(key);
  }
}

void QPlotWindow::RemovePoint(DataPoint *dp)
{
  #ifdef DEBUG
  printf("QPlotWindow::RemovePoint\n");
  #endif
  /*int indx = p.indexOf(dp);
  if(indx > -1){
    delete p[indx];
    p.remove(indx);
  }*/
}

void QPlotWindow::RemoveAllPoints()
{
  #ifdef DEBUG
  printf("QPlotWindow::RemoveAllPoints\n");
  #endif
  QList<QString> keys = p.keys();
  for(int i = 0; i < keys.size(); i++){
    delete p[keys[i]];
  }
  p.clear();
}


/*QVector< DataCurve > QPlotWindow::getCurves()
{
  #ifdef DEBUG
  printf("QPlotWindow::getCurves\n");
  #endif
  return c;
}
*/

void QPlotWindow::RemoveCurveAt(int cid)
{
  #ifdef DEBUG
  printf("QPlotWindow::RemoveCurveAt\n");
  #endif
  QString key = c.keys()[cid];
  delete c[key];
  c.remove(key);
}

void QPlotWindow::RemoveAllCurves()
{
  #ifdef DEBUG
  printf("QPlotWindow::RemoveAllCurves\n");
  #endif
  QList<QString> keys = c.keys();
  for(int i = 0; i < keys.size(); i++){
    delete c[keys[i]];
  }
  c.clear();
}

void QPlotWindow::Center()
{
  return;
}

void QPlotWindow::Refresh()
{
  //printf("Refresh\n");
  // instead of  Plotly.update('graph', data_update) do a replot!
  Plot();
}

void QPlotWindow::SelectAll()
{
  selected_points.clear();
  for(int i = 0; i < p.count(); i++){
    p.values()[i]->setSelection(true);
    selected_points << i;
  }
}

void QPlotWindow::ClearSelection()
{
  #ifdef DEBUG
  printf("QPlotWindow::ClearSelection");
  #endif
  for(int i = 0; i < selected_points.size(); i++){
    p.values()[selected_points[i]]->setSelection(false);
  }
  selected_points.clear();
}

void QPlotWindow::SaveAsImage(QString imgname)
{
  #ifdef DEBUG
  printf("QPlotWindow::SaveAsImage\n");
  #endif
  /*
  QByteArray bytes;
  QBuffer buffer(&bytes);
  buffer.open(QIODevice::WriteOnly);
  pixmap.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format
  */
  /*
  if(imgname.contains(".png", Qt::CaseInsensitive) == true){
    pixmap.save(imgname, "PNG", 100);
  }
  else if(imgname.contains(".jpg", Qt::CaseInsensitive) == true ||
    imgname.contains(".jpeg", Qt::CaseInsensitive) == true){
    pixmap.save(imgname, "JPG", 100);
  }
  else if(imgname.contains(".pdf", Qt::CaseInsensitive) == true){
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(imgname);
    printer.setResolution(300); //override to 300dpi
    printer.setFontEmbeddingEnabled(true);
    printer.setColorMode(QPrinter::Color);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPaperSize(size(), QPrinter::DevicePixel);
    printer.setFullPage(true);
    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing, antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing, antialiasing);
    painter.drawPixmap(rect(), pixmap);
  }
  else{
    pixmap.save(imgname+".png");
  }
   QMessageBox::information(this, "Plot Saved!", QString("The plot has been saved as %1").arg(imgname), QMessageBox::Ok);
   */
}

void QPlotWindow::close() {}

QPlotWindow::QPlotWindow(QWidget *parent, PLOTTYPE pt_) : QWidget(parent)
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  this->setMinimumSize(this->size());

  pt = pt_;
  if(pt == _2D){
    _2dview = new QtCharts::QChartView();
    mainLayout->addWidget(_2dview);
  }
/*  else{
    _3dview = new Q3DScatter();
    mainLayout->addWidget(_3dview);
  }
  */
  this->setLayout(mainLayout);
}

QPlotWindow::~QPlotWindow()
{
  if(pt == _2D)
    delete _2dview;
  /*else
    delete _3dview;
    */
}
