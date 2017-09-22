#include <QtGui>
#include <QToolButton>
#include <QPrinter>
#include <QMessageBox>
#include <QStylePainter>
#include <QStyleOptionFocusRect>
#include <cmath>

#include "chart.h"

#define EPSILON 1e-3  /* Define your own tolerance*/
#define FLOAT_EQ(x,v, EPSILON) (((v - EPSILON) < x) && (x <( v + EPSILON)))

using namespace std;

inline double round(double n, unsigned d)
{
  return floor(n * pow(10., d) + .5) / pow(10., d);
}

Chart::Chart(QWidget *parent)
    : QWidget(parent)
{
  #ifdef DEBUG
  printf("Chart::Chart\n");
  #endif
  rescalefactor = 0.25;
  labeldetail = false;

  QPalette Pal(palette());

  // set black background
  Pal.setColor(QPalette::Background, Qt::white);
  setBackgroundRole(QPalette::Light);
  setAutoFillBackground(true);
  setPalette(Pal);

  setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
//   setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
  setFocusPolicy(Qt::StrongFocus);
  rubberBandIsShown = false;

  zoomInButton = new QToolButton(this);
  zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
  zoomInButton->adjustSize();
  connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

  zoomOutButton = new QToolButton(this);
  zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
  zoomOutButton->adjustSize();
  connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

  m_xaxisname = "x";
  m_yaxisname = "y";
  setPlotSettings(PlotSettings());
}

Chart::~Chart()
{
  #ifdef DEBUG
  printf("Chart::~Chart\n");
  #endif
  for(int i = 0; i < p.size(); i++)
    delete p[i];
  p.clear();

  delete zoomInButton;
  delete zoomOutButton;
}

void Chart::Refresh()
{
  #ifdef DEBUG
  printf("Chart::Refresh\n");
  #endif
  if(zoomStack.size() > 0 && p.size() > 0){
    qreal minX, maxX, minY, maxY;
    // search min max between scatters
    minX = maxX = p[0]->x();
    minY = maxY = p[0]->y();
    for(int i = 1; i < p.size(); i++){
      if(p[i]->x() < minX){
        minX = p[i]->x();
      }

      if(p[i]->x() > maxX){
        maxX = p[i]->x();
      }

      if(p[i]->y() < minY){
        minY = p[i]->y();
      }

      if(p[i]->y() > maxY){
        maxY = p[i]->y();
      }
    }

    // search min max between curves
    for(int i = 0; i < curveMap.size(); i++){
      for(int j = 0; j < curveMap[i].getPoints().size(); j++){
        if(curveMap[i].getPoints()[j].x() < minX){
          minX = curveMap[i].getPoints()[j].x();
        }

        if(curveMap[i].getPoints()[j].x() > maxX){
          maxX = curveMap[i].getPoints()[j].x();
        }

        if(curveMap[i].getPoints()[j].y() < minY){
          minY = curveMap[i].getPoints()[j].y();
        }

        if(curveMap[i].getPoints()[j].y() > maxY){
          maxY = curveMap[i].getPoints()[j].y();
        }
      }
    }


    zoomStack.first().minX = (minX - fabs(minX*rescalefactor));
    zoomStack.first().maxX = (maxX + fabs(maxX*rescalefactor));
    zoomStack.first().minY = (minY - fabs(minY*rescalefactor));
    zoomStack.first().maxY = (maxY + fabs(maxY*rescalefactor));

//     zoomStack.first().numXTicks =  zoomStack.first().spanX();
//     zoomStack.first().numYTicks =  zoomStack.first().spanY();

    /*maxX = zoomStack.first().maxX - (int)zoomStack.first().spanX() % zoomStack.first().numXTicks;

    if(FLOAT_EQ(maxX, 0, EPSILON))
      zoomStack.first().maxX = 1;
    else
      zoomStack.first().maxX = maxX;

    maxY = zoomStack.first().maxY - (int)zoomStack.first().spanY() % zoomStack.first().numYTicks;
    if(FLOAT_EQ(maxY, 0, EPSILON))
      zoomStack.first().maxY = 1;
    else
      zoomStack.first().maxY = maxY;
    */
  }
  refreshPixmap();
}

void Chart::setXaxisName(QString xaxisname)
{
  #ifdef DEBUG
  printf("Chart::setXaxisName\n");
  #endif
  m_xaxisname = xaxisname;
}

void Chart::setYaxisName(QString yaxisname)
{
  #ifdef DEBUG
  printf("Chart::setYaxisName\n");
  #endif
  m_yaxisname = yaxisname;
}

void Chart::setPlotTitle(QString plottitle)
{
  #ifdef DEBUG
  printf("Chart::setPlotTitle\n");
  #endif
  m_plottitle = plottitle;
}

void Chart::setLabelDetail(bool labeldetail_)
{
  #ifdef DEBUG
  printf("Chart::setLabelDetail\n");
  #endif
  labeldetail = labeldetail_;
}

void Chart::setXminXmaxXTick(double xmin, double xmax, int xtick)
{
  #ifdef DEBUG
  printf("Chart::setXminXmaxXTick\n");
  #endif
  if(zoomStack.size() > 0 && curZoom < zoomStack.size()){
    zoomStack[curZoom].minX = xmin;
    zoomStack[curZoom].maxX = xmax;
    zoomStack[curZoom].numXTicks = xtick;
    zoomStack[curZoom].adjust();
  }
}


void Chart::setYminYmaxYTick(double ymin, double ymax, int ytick)
{
  #ifdef DEBUG
  printf("Chart::setYminYmaxYTick\n");
  #endif
  if(zoomStack.size() > 0 && curZoom < zoomStack.size()){
    zoomStack[curZoom].minY = ymin;
    zoomStack[curZoom].maxY = ymax;
    zoomStack[curZoom].numXTicks = ytick;
    zoomStack[curZoom].adjust();
  }
}

void Chart::DoSelection(int low, int high)
{
  #ifdef DEBUG
  printf("Chart::DoSelection\n");
  #endif
  int i;
  for(i = low; i < high; i++){
    p[i]->setSelection(true);
  }
}

void Chart::Select(int from, int to)
{
  #ifdef DEBUG
  printf("Chart::Select\n");
  #endif
  if(from < to){
    int mid = (from+to)/2;
    Select(from, mid);
    Select(mid+1, to);
    DoSelection(from, to);
  }
}

void Chart::SelectAll()
{
  #ifdef DEBUG
  printf("Chart::SelectAll\n");
  #endif
  Select(0, p.size());
  /*
  for(int i = 0; i < p.size(); i++)
    p[i]->setSelection(true);
  */
  refreshPixmap();
}

void Chart::DoUnselection(int low, int high)
{
  #ifdef DEBUG
  printf("Chart::DoUnselection\n");
  #endif
  int i;
  for(i = low; i < high; i++){
    if(p[i]->isSelected() == true){
      p[i]->setSelection(false);
      pforward.append(i);
    }
  }
}

void Chart::Unselect(int from, int to)
{
  #ifdef DEBUG
  printf("Chart::Unselect\n");
  #endif
  if(from < to){
    int mid = (from+to)/2;
    Unselect(from, mid);
    Unselect(mid+1, to);
    DoUnselection(from, to);
  }
}

void Chart::ClearSelection()
{
  #ifdef DEBUG
  printf("Chart::ClearSelection\n");
  #endif
  /* Unselection by divide and conqueror technique*/
  Unselect(0, p.size());
  /*for(int i = 0; i < p.size(); i++){
    if(p[i]->isSelected() == true){
       p[i]->setSelection(false);
       pforward.append(i);
    }
  }*/

  refreshPixmap();
}

void Chart::RemoveCurveAt(int cid)
{
  #ifdef DEBUG
  printf("Chart::RemoveCurveAt\n");
  #endif
  curveMap.remove(cid);
}

void Chart::RemoveAllCurves()
{
  #ifdef DEBUG
  printf("Chart::RemoveAllCurves\n");
  #endif
  curveMap.clear();
}

void Chart::setPlotSettings(const PlotSettings &settings)
{
  #ifdef DEBUG
  printf("Chart::setPlotSettings\n");
  #endif
  zoomStack.clear();
  zoomStack.append(settings);
  curZoom = 0;
  zoomInButton->hide();
  zoomOutButton->hide();
  refreshPixmap();
}

void Chart::zoomOut()
{
  #ifdef DEBUG
  printf("Chart::zoomOut\n");
  #endif
  if(curZoom > 0){
    --curZoom;
    zoomOutButton->setEnabled(curZoom > 0);
    zoomInButton->setEnabled(true);
    zoomInButton->show();
    refreshPixmap();
  }
}

void Chart::zoomIn()
{
  #ifdef DEBUG
  printf("Chart::zoomIn\n");
  #endif
  if(curZoom < zoomStack.count() - 1){
    ++curZoom;
    zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
    zoomOutButton->setEnabled(true);
    zoomOutButton->show();
    refreshPixmap();
  }
}

/*
void Chart::setCurveData(int id, const QVector<QPointF> &data)
{
    curveMap[id] = data;

}
*/

void Chart::addPoint(qreal x, qreal y, QString name)
{
  #ifdef DEBUG
  printf("Chart::addPoint(qreal x, qreal y, QString name)\n");
  #endif
  p.append(new DataPoint(x, y, name));
//   p.last()->setX(x);
//   p.last()->setY(y);
//   p.last()->setName(name);
}

void Chart::addPoint(qreal x, qreal y, QString name, QColor color, int radius)
{
  #ifdef DEBUG
  printf("Chart::addPoint(qreal x, qreal y, QString name, QColor color, int radius)\n");
  #endif
  p.append(new DataPoint(x, y, name));
  p.last()->setColor(color);
  p.last()->setRadius(radius);
}

void Chart::addCurve(QVector< QPointF > curve, QString name, QColor color)
{
  #ifdef DEBUG
  printf("Chart::addCurve\n");
  #endif
  curveMap.append(DataCurve(curve, name, color));
}


int Chart::PointSize() const
{
  #ifdef DEBUG
  printf("Chart::PointSize\n");
  #endif
  return p.size();
}

DataPoint* Chart::getPoint(int id)
{
  #ifdef DEBUG
  printf("Chart::getPoint\n");
  #endif
  if(id < p.size())
    return p[id];
  else
    return 0;
}

void Chart::RemovePointAt(int id)
{
  #ifdef DEBUG
  printf("Chart::RemovePointAt\n");
  #endif
  if(id < p.size()){
    delete p[id];
    p.remove(id);
  }
}

void Chart::RemovePoint(DataPoint *dp)
{
  #ifdef DEBUG
  printf("Chart::RemovePoint\n");
  #endif
  int indx = p.indexOf(dp);
  if(indx > -1){
    delete p[indx];
    p.remove(indx);
  }
}

void Chart::RemoveAllPoints()
{
  #ifdef DEBUG
  printf("Chart::RemoveAllPoints\n");
  #endif
  for(int i = 0; i < p.size(); i++){
    delete p[i];
  }
  p.clear();
}


QVector< DataCurve > Chart::getCurves()
{
  #ifdef DEBUG
  printf("Chart::getCurves\n");
  #endif
  return curveMap;
}

/*
void Chart::clearCurve(int id)
{
    curveMap.remove(id);
    refreshPixmap();
}
*/

QSize Chart::minimumSizeHint() const
{
  #ifdef DEBUG
  printf("Chart::minimumSizeHint\n");
  #endif
  return QSize(6 * Margin, 4 * Margin);
}

QSize Chart::sizeHint() const
{
  #ifdef DEBUG
  printf("Chart::sizeHint\n");
  #endif
  return QSize(12 * Margin, 8 * Margin);
}

void Chart::SaveAsImage(QString imgname)
{
  #ifdef DEBUG
  printf("Chart::SaveAsImage\n");
  #endif
  /*
  QByteArray bytes;
  QBuffer buffer(&bytes);
  buffer.open(QIODevice::WriteOnly);
  pixmap.save(&buffer, "PNG"); // writes pixmap into bytes in PNG format
  */
  if(imgname.contains(".png", Qt::CaseInsensitive) == true){
    pixmap.save(imgname);
  }
  else if(imgname.contains(".jpg", Qt::CaseInsensitive) == true ||
    imgname.contains(".jpeg", Qt::CaseInsensitive) == true){
    pixmap.save(imgname);
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
    painter.drawPixmap(rect(), pixmap);
  }
  else{
    pixmap.save(imgname+".png");
  }
   QMessageBox::information(this, "Plot Saved!", QString("The plot has been saved as %1").arg(imgname), QMessageBox::Ok);
}

void Chart::paintEvent(QPaintEvent * event)
{
  #ifdef DEBUG
  printf("Chart::paintEvent\n");
  #endif
  Q_UNUSED(event);

  QStylePainter painter(this);
  painter.drawPixmap(0, 0, pixmap);

//   painter.eraseRect(rect());

  if(rubberBandIsShown){
    QPen rubberbandpen = QPen(QColor(255, 0, 0, 125));
    painter.setPen(rubberbandpen);
    painter.drawRect(rubberBandRect.normalized().adjusted(0, 0, -1, -1));
  }

  if(hasFocus()){
    QStyleOptionFocusRect option;
    option.initFrom(this);
    option.backgroundColor = QColor(255, 0, 0, 125);
    painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
  }
}

void Chart::resizeEvent(QResizeEvent *event)
{
  #ifdef DEBUG
  printf("Chart::resizeEvent\n");
  #endif
  Q_UNUSED(event);
  int x = width() - (zoomInButton->width()
                      + zoomOutButton->width() + 10);
  zoomInButton->move(x, 5);
  zoomOutButton->move(x + zoomInButton->width() + 5, 5);
  refreshPixmap();
}

void Chart::mousePressEvent(QMouseEvent *event)
{
  #ifdef DEBUG
  printf("Chart::mousePressEvent\n");
  #endif
  QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

  //zoom with the middle button
  if(event->button() == Qt::MiddleButton || event->button() == Qt::LeftButton){
    if(rect.contains(event->pos())){
      rubberBandIsShown = true;
      rubberBandRect.setTopLeft(event->pos());
      rubberBandRect.setBottomRight(event->pos());
      updateRubberBandRegion();
      setCursor(Qt::CrossCursor);
    }
  }
  else if((event->button() == Qt::RightButton)){
    rubberBandIsShown = false;
    // show qcontext menu event
  }
}

void Chart::mouseMoveEvent(QMouseEvent *event)
{
  #ifdef DEBUG
  printf("Chart::mouseMoveEvent\n");
  #endif
  if(rubberBandIsShown == true){
    updateRubberBandRegion();
    rubberBandRect.setBottomRight(event->pos());
    updateRubberBandRegion();
  }
}

void Chart::mouseReleaseEvent(QMouseEvent *event)
{
  #ifdef DEBUG
  printf("Chart::mouseReleaseEvent\n");
  #endif
  if((event->button() == Qt::MiddleButton) && rubberBandIsShown == true){
    rubberBandIsShown = false;
    updateRubberBandRegion();
    unsetCursor();

    QRect rect = rubberBandRect.normalized();
    if(rect.width() < 4 || rect.height() < 4)
        return;
    rect.translate(-Margin, -Margin);

    PlotSettings prevSettings = zoomStack[curZoom];
    PlotSettings settings;
    double dx = prevSettings.spanX() / (width() - 2 * Margin);
    double dy = prevSettings.spanY() / (height() - 2 * Margin);
    settings.minX = prevSettings.minX + dx * rect.left();
    settings.maxX = prevSettings.minX + dx * rect.right();
    settings.minY = prevSettings.maxY - dy * rect.bottom();
    settings.maxY = prevSettings.maxY - dy * rect.top();
    settings.adjust();

    zoomStack.resize(curZoom + 1);
    zoomStack.append(settings);
    zoomIn();
  }
  else if((event->button() == Qt::LeftButton) && rubberBandIsShown == true){
    rubberBandIsShown = false;
    updateRubberBandRegion();
    unsetCursor();

    QRect rect = rubberBandRect.normalized();
    if(rect.width() < 4 || rect.height() < 4)
        return;
    rect.translate(-Margin, -Margin);

    PlotSettings settings = zoomStack[curZoom];
    double dx = settings.spanX() / (width() - 2 * Margin);
    double dy = settings.spanY() / (height() - 2 * Margin);
    qreal minX = settings.minX + dx * rect.left();
    qreal maxX = settings.minX + dx * rect.right();
    qreal minY = settings.maxY - dy * rect.bottom();
    qreal maxY = settings.maxY - dy * rect.top();


    /*qDebug() << "Select Objects..";
    qDebug() << minX << maxX << minY << maxY;*/
    for(int i = 0; i < p.size(); i++){
      if(p[i]->x() > minX && p[i]->x() < maxX
        && p[i]->y() > minY && p[i]->y() < maxY){
        if(p[i]->isSelected()){
          p[i]->setSelection(false);
        }
        else{
          p[i]->setSelection(true);
        }
      }
      else{
        continue;
      }
    }

    refreshPixmap();
    // Select objects
  }
  /*else if((event->button() == Qt::LeftButton) && rubberBandIsShown == false){
    //Show QMenu and do operation....
  }*/
  else{
    return;
  }
}

void Chart::keyPressEvent(QKeyEvent *event)
{
  #ifdef DEBUG
  printf("Chart::keyPressEvent\n");
  #endif
  switch (event->key()){
  case Qt::Key_Plus:
    zoomIn();
    break;
  case Qt::Key_Minus:
    zoomOut();
    break;
  case Qt::Key_Left:
    zoomStack[curZoom].scroll(-1, 0);
    refreshPixmap();
    break;
  case Qt::Key_Right:
    zoomStack[curZoom].scroll(+1, 0);
    refreshPixmap();
    break;
  case Qt::Key_Down:
    zoomStack[curZoom].scroll(0, -1);
    refreshPixmap();
    break;
  case Qt::Key_Up:
    zoomStack[curZoom].scroll(0, +1);
    refreshPixmap();
    break;
  default:
      QWidget::keyPressEvent(event);
  }
}

void Chart::wheelEvent(QWheelEvent *event)
{
  #ifdef DEBUG
  printf("Chart::wheelEvent\n");
  #endif
  int numDegrees = event->delta() / 8;
  int numTicks = numDegrees / 15;

  if(event->orientation() == Qt::Horizontal){
    zoomStack[curZoom].scroll(numTicks, 0);
  }
  else{
    zoomStack[curZoom].scroll(0, numTicks);
  }

  refreshPixmap();
}

void Chart::updateRubberBandRegion()
{
  #ifdef DEBUG
  printf("Chart::updateRubberBandRegion\n");
  #endif
  QRect rect = rubberBandRect.normalized();
  update(rect.left(), rect.top(), rect.width(), 1);
  update(rect.left(), rect.top(), 1, rect.height());
  update(rect.left(), rect.bottom(), rect.width(), 1);
  update(rect.right(), rect.top(), 1, rect.height());
}

void Chart::refreshPixmap()
{
  #ifdef DEBUG
  printf("Chart::refreshPixmap\n");
  #endif
  // fill the background with the white color!
  pixmap = QPixmap(size());
  pixmap.fill(Qt::white);

  /*pixmap.fill(this, 0, 0); OBSOLETE FUNCTION in qt5 */

  QPainter painter(&pixmap);
  painter.initFrom(this);

  painter.setRenderHint(QPainter::Antialiasing);
  painter.setRenderHint(QPainter::TextAntialiasing);
  painter.setRenderHint(QPainter::HighQualityAntialiasing);
  //painter.setRenderHint(QPainter::SmoothPixmapTransform);

  drawGrid(&painter);

  if(curveMap.size() > 0)
    drawCurves(&painter);

  if(p.size() > 0)
    drawScatters(&painter);

  update();
}

int getDecimals(double x){
  #ifdef DEBUG
  printf("Chart::getDecimals\n");
  #endif
  QString str = QString::number(x);
  QString decimals = str.split(".").last();
  int i, d = 1;
  for(i = 0; i < decimals.size(); i++){
    if(decimals[i].digitValue() == 0)
      d++;
    else
      break;
  }
  if(decimals.contains("e") == true){
    return d+6;
  }
  else{
    return d+1;
  }
}

int integer_digits(double x)
{
  int n = (int)x;
  int i = 0;
  while(n != 0){
    n /= 10.f;
    i++;
  }
  return i;
}

int zeros_after_decimal_point(double x)
{
  int i=-1;
  double n = x;
  while(n < 1.f){
    //printf("%f\n", n);
    n *= 10.f;
    i++;
  }
  return i;
}

void Chart::drawGrid(QPainter *painter)
{
  #ifdef DEBUG
  printf("Chart::drawGrid\n");
  #endif
  QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
  if(!rect.isValid())
      return;

  PlotSettings settings = zoomStack[curZoom];

  QPen gridpen = QPen(QColor(200, 200, 255, 125));
  QPen axespen = QPen(QColor(Qt::black));

  qreal min, max, stepx, stepy;

  int nintegers = integer_digits(settings.spanX());
  //printf("nintegers %d\n", nintegers);

  if(nintegers > 0){
    stepx = (0.5 * (double)nintegers);
  }
  else{
    int nzeros = zeros_after_decimal_point(settings.spanX())+1;
    //printf("nzeros %d\n", nzeros);
    stepx = 0.5/pow(10, nzeros);
  }

  //stepx = settings.spanX() / settings.numXTicks;
  //stepx = 0.5;
  min = floor(settings.minX);
  max = ceil(settings.maxX);

  //float factor = rect.width() / painter->fontMetrics().width("-20");

  qreal factor = rect.width()/480.;
  QFont font("SansSerif", 5);
  font.setPointSizeF(font.pointSizeF()*factor);
  font.setStyleStrategy(QFont::ForceOutline);
  painter->setFont(font);

  for (qreal ix = min; ix < max; ix += stepx){
      qreal dx = ix - settings.minX;
      qreal x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      if(x > rect.left() && x < rect.right()){
        painter->setPen(gridpen);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(axespen);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        if(FLOAT_EQ(ix, 0.f, 1e-5)){
          painter->drawText(x - 50, rect.bottom() + 10, 100, 15,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(0));
        }
        else{
          painter->drawText(x - 50, rect.bottom() + 10, 100, 15,
                          Qt::AlignHCenter | Qt::AlignTop,
                          QString::number(ix, 'g', getDecimals(stepx)));
          /*
          painter->drawText(x - 2, rect.bottom()+20, QString::number(ix, 'g', getDecimals(stepx)));
          */
        }
      }
      else{
        continue;
      }
  }

  //stepy = settings.spanY() / settings.numYTicks;
  nintegers = integer_digits(settings.spanY());
  if(nintegers > 0){
    stepy = (0.5 * (double)nintegers);
  }
  else{
    int nzeros = zeros_after_decimal_point(settings.spanX())+1;
    stepy = 0.5/pow(10, nzeros);
  }

  min = floor(settings.minY);
  max = ceil(settings.maxY);

  for (qreal iy = min; iy < max; iy += stepy){
    qreal dy = iy - settings.minY;
    qreal y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
    if(y > rect.top() && y < rect.bottom()){
      painter->setPen(gridpen);
      painter->drawLine(rect.left(), y, rect.right(), y);
      painter->setPen(axespen);
      painter->drawLine(rect.left() - 5, y, rect.left(), y);
      if(FLOAT_EQ(iy, 0.f, 1e-5)){
        painter->drawText(rect.left() - Margin -5, y - 10, Margin - 5, 20,
                       Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(0));
      }
      else{
        painter->drawText(rect.left() - Margin -5, y - 10, Margin - 5, 20,
                       Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(iy, 'g', getDecimals(stepy)));
      }
    }
    else{
      continue;
    }
  }

  //Draw origin axis in the margin limits
  painter->setPen(axespen);

  qreal dx = 0 - settings.minX;
  qreal dy = 0 - settings.minY;
  qreal x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
  qreal y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());

  if(x > rect.left() && x < rect.right()){
    painter->drawLine(x, rect.top(), x, rect.bottom());
  }


  if(y > rect.top() && y < rect.bottom()){
    painter->drawLine(rect.left(), y, rect.right(), y);
  }

  // write the 0 coordinate
  if(x > rect.left() && x < rect.right() &&
    y > rect.top() && y < rect.bottom()){
    painter->drawText(x - 50, y + 5, 100, 15,
                Qt::AlignHCenter | Qt::AlignTop,
                QString::number(0));
  }

  // Draw Axis names...
  //factor = rect.height()/640.;
  font.setPointSize(10);
  font.setPointSizeF(font.pointSizeF()*factor);
  painter->setFont(font);
  QFontMetrics fm(font);
  qreal xmarkTextWidth = fm.width(m_xaxisname);
  // qreal markTextHeight = fm.height();
  x = Margin + (rect.right() -rect.left())/2. - xmarkTextWidth/2.;

  painter->drawText(x, rect.bottom() + Margin/2., m_xaxisname);

  // draw top title.
  xmarkTextWidth = fm.width(m_plottitle);
  x = Margin + (rect.right() -rect.left())/2. - xmarkTextWidth/2.;
  painter->drawText(x, rect.top() - Margin/2., m_plottitle);

  // write y axis name vertically
  painter->save();
  qreal ymarkTextWidth = fm.width(m_yaxisname);
  y = Margin + (rect.bottom() - rect.top())/2. + ymarkTextWidth/2.;
  // painter->translate(rect.left() - (Margin+40), y + 160);
  painter->translate(rect.left() - Margin/2., y);
  painter->rotate(270); // or 270
  painter->drawText(0, 0, m_yaxisname);
  painter->restore();

  // draw limit rectangle
  painter->drawRect(rect.adjusted(0, 0, -1, -1));
}

void Chart::drawCurves(QPainter *painter)
{
  #ifdef DEBUG
  printf("Chart::drawCurves\n");
  #endif
  PlotSettings settings = zoomStack[curZoom];
  QRect rect(Margin, Margin,
              width() - 2 * Margin, height() - 2 * Margin);
  if(!rect.isValid())
      return;

  painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

  for(int i = 0; i < curveMap.size(); i++){
    const DataCurve data = curveMap[i];
    if(data.isVisible() == true){
      if(data.getPoints().size() > 0){
        QPolygonF polyline(data.getPoints().size());
        for(int j = 0; j < data.getPoints().size(); ++j){
          double dx = data.getPoints()[j].x() - settings.minX;
          double dy = data.getPoints()[j].y() - settings.minY;
          double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
          double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
          polyline[j] = QPointF(x, y); // -2.5 to good centering datas...
        }
        painter->setPen(QPen(data.color(), data.width(), Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawPolyline(polyline);
      }
      else
        continue;
    }
    else{
      continue;
    }
  }
}

/*
typedef struct{
  QVector <QRectF> point; //this will be shared from the main thread
  QVector <DataPoint*> p; // this will be shared from the main thread
  PlotSettings settings;
  QRect rect;
  QVector<int> pforward;
  int from, to;
} scatter_th_arg;


void *drawScatterThread(void *arg_)
{
  #ifdef DEBUG
  printf("Chart::drawScatterThread\n");
  #endif
  scatter_th_arg *arg = (scatter_th_arg*) arg_;

  for(int i = arg->from; i < arg->to; i++){
    if(i < arg->p.size()){
      int indx = arg->pforward.indexOf(i);
      if(arg->p[i]->isVisible() == true && (arg->p[i]->isSelected() == false || indx == -1)){
        double dx = arg->p[i]->x() - arg->settings.minX;
        double dy = arg->p[i]->y() - arg->settings.minY;
        double x = arg->rect.left() + (dx * (arg->rect.width() - 1) / arg->settings.spanX());
        double y = arg->rect.bottom() - (dy * (arg->rect.height() - 1) / arg->settings.spanY());
        qreal radius = arg->p[i]->radius();
        arg->point.append(QRectF(x-radius/2., y-radius/2., radius, radius));
      }
      else{
        continue;
      }
    }
    else
      break;
  }
}*/

void Chart::drawScatters(QPainter *painter)
{
  #ifdef DEBUG
  printf("Chart::drawScatters\n");
  #endif
  PlotSettings settings = zoomStack[curZoom];
  QRect rect(Margin, Margin,
              width() - 2 * Margin, height() - 2 * Margin);
  if(!rect.isValid())
      return;

  painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

  // Draw all not selected if plotsettings changed
  for(int i = 0; i < p.size(); i++){
    int indx = pforward.indexOf(i);
    if(p[i]->isVisible() == true && (p[i]->isSelected() == false || indx == -1)){
      double dx = p[i]->x() - settings.minX;
      double dy = p[i]->y() - settings.minY;
      double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
      qreal radius = p[i]->radius();
      QColor color = p[i]->color();
      painter->setPen(color);
      QRectF point = QRectF(x-radius/2., y-radius/2., radius, radius);
      painter->drawEllipse(point);
      QPainterPath path;
      path.addEllipse(point);
      painter->fillPath(path, QBrush(color, Qt::SolidPattern));
      //highlihht point with a red circle and give them in forward...
      //if(p[i]->isSelected() == true){
      //  painter->setRenderHint(QPainter::Antialiasing);
      //  painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
      //  painter->drawEllipse(point);
      //}
    }
    else{
      continue;
    }
  }

  //Draw only selected to be in forward
    for(int i = 0; i < p.size(); i++){
    int indx = pforward.indexOf(i);
    if(p[i]->isVisible() == true && (p[i]->isSelected() == true || indx > -1)){
      double dx = p[i]->x() - settings.minX;
      double dy = p[i]->y() - settings.minY;
      double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
      qreal radius = p[i]->radius();
      QColor color = p[i]->color();
      painter->setPen(color);
      QRectF point = QRectF(x-radius/2., y-radius/2., radius, radius);
      painter->drawEllipse(point);
      QPainterPath path;
      path.addEllipse(point);
      painter->fillPath(path, QBrush(color, Qt::SolidPattern));
      //highlihht point with a red circle and give them in forward...
      if(p[i]->isSelected() == true){
        painter->setRenderHint(QPainter::Antialiasing);
        painter->setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawEllipse(point);
        pforward.append(i);
      }

      if(indx > -1)
        pforward.remove(indx);
    }
    else{
      continue;
    }
  }

  // used to rescale fonts
  int pxelsz = (int)ceil(8*height()/480.);

  // Show label after selection...
  for(int i = 0; i < p.size(); i++){
    if(p[i]->isVisible() == true){
      double dx = p[i]->x() - settings.minX;
      double dy = p[i]->y() - settings.minY;
      double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
      qreal radius = p[i]->radius();

      if(p[i]->isSelected() == true){
        QFont font("SansSerif", 8);
        painter->setPen(QPen(Qt::black, 0.5, Qt::SolidLine, Qt::RoundCap));
        font.setStyleStrategy(QFont::ForceOutline);
        font.setPixelSize(pxelsz);
        painter->setFont(font);

        QFontMetrics fm(font);
        qreal markTextWidth;
        if(labeldetail == true)
          markTextWidth = fm.width(QString("%1 (%2 ; %3)").arg(p[i]->name()).arg(p[i]->x()).arg(p[i]->y()));
        else
          markTextWidth = fm.width(QString("%1").arg(p[i]->name()));
        qreal markTextHeight = fm.height();
        QRectF trect;

        trect.setX(x);
        trect.setY(y+radius);
        trect.setWidth(markTextWidth);
        trect.setHeight(markTextHeight);
        /*
        if(x > rect.right() - (Margin+120)){
          trect.setX(x-140);
          trect.setY(y+radius);
          trect.setWidth(markTextWidth);
          trect.setHeight(markTextHeight);
        }
        else{
          trect.setX(x);
          trect.setY(y+radius);
          trect.setWidth(markTextWidth);
          trect.setHeight(markTextHeight);
        }*/
        painter->fillRect(trect, Qt::white);
        if(labeldetail == true)
          painter->drawText(trect, Qt::AlignCenter, QString("%1 (%2 ; %3)").arg(p[i]->name()).arg(p[i]->x()).arg(p[i]->y()));
        else
          painter->drawText(trect, Qt::AlignCenter, QString("%1").arg(p[i]->name()));
        painter->drawRect(trect);
      }
    }
    else{
      continue;
    }
  }
}

PlotSettings::PlotSettings()
{
  #ifdef DEBUG
  printf("PlotSettings::PlotSettings\n");
  #endif
  minX = 0.0;
  maxX = 10.0;
  numXTicks = 10;

  minY = 0.0;
  maxY = 10.0;
  numYTicks = 10;
}

void PlotSettings::scroll(int dx, int dy)
{
  #ifdef DEBUG
  printf("PlotSettings::scroll\n");
  #endif
  double stepX = spanX() / numXTicks;
  minX += dx * stepX;
  maxX += dx * stepX;

  double stepY = spanY() / numYTicks;
  minY += dy * stepY;
  maxY += dy * stepY;
}

void PlotSettings::adjust()
{
  #ifdef DEBUG
  printf("PlotSettings::adjust\n");
  #endif
  adjustAxis(minX, maxX, numXTicks);
  adjustAxis(minY, maxY, numYTicks);
}

// void PlotSettings::adjustAxis(double &min, double &max,
//                               int &numTicks)
// {
// //     const int MinTicks = 4;
// //     double grossStep = (max - min) / MinTicks;
// //     double step = pow(10.0, floor(log10(grossStep)));
//
//     double grossStep = (max - min) / numTicks;
//     double step = pow(10.0, floor(log10(grossStep)));
//
//     /*
//     if(8 * step < grossStep){
//         step *= 8;
//     } else if(2 * step < grossStep){
//         step *= 2;
//     }
//     */
//
//   /*numTicks = int(ceil(max / step) - floor(min / step));
//     if(numTicks < MinTicks)
//         numTicks = MinTicks;
//     */
//     min = floor(min / step) * step;
//     max = ceil(max / step) * step;
// }

void PlotSettings::adjustAxis(double &min, double &max, int &numTicks)
{
  #ifdef DEBUG
  printf("PlotSettings::adjustAxis\n");
  #endif
  const int MinTicks = 4;
  double grossStep = (max - min) / MinTicks;
  double step = pow(10.0, floor(log10(grossStep)));

  if (5 * step < grossStep) {
      step *= 5;
  } else if (2 * step < grossStep) {
      step *= 2;
  }

  numTicks = int(ceil(max / step) - floor(min / step));
  if (numTicks < MinTicks)
      numTicks = MinTicks;
  min = floor(min / step) * step;
  max = ceil(max / step) * step;
}
