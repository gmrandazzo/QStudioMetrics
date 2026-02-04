/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for chart.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

// ONLY 2D SCATTER PLOTS
#include <QMessageBox>
#include <QPageLayout>
#include <QStyleOptionFocusRect>
#include <QStylePainter>
#include <QToolButton>
#include <QtGui>
#include <QtPrintSupport/QPrinter>
#include <cmath>
#include <QCache>
#include <QToolTip>
#include <algorithm>
#include <QBuffer>
#include <QByteArray>
#include <QSettings>

#include "chart.h"

#define EPSILON 1e-3 /*Define your own tolerance*/

using namespace std;

// Static cache for marker pixmaps to improve performance
static QCache<QString, QPixmap> markerCache;

// Helper to generate cache key
static QString getMarkerKey(int type, int radius, const QColor& color, bool selected) {
    return QString("%1_%2_%3_%4").arg(type).arg(radius).arg(color.name(QColor::HexArgb)).arg(selected);
}

inline double round(double n, unsigned d) {
  return floor(n * pow(10., d) + .5) / pow(10., d);
}

Chart::Chart(QWidget *parent) : QWidget(parent) {
// #ifdef DEBUG
//   printf("Chart::Chart\n");
// #endif
  antialiasing = true;
  labeldetail = false;
  m_indexDirty = false;
  m_isLassoActive = false;
  
  // Enable mouse tracking for hover events
  setMouseTracking(true);

  QPalette Pal(palette());

  // set black background
  Pal.setColor(QPalette::Window, Qt::white);
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
  titleSize = 1;
  axisValueSize = 1;
  xLabelSize = 1;
  yLabelSize = 1;
  
  // Initialize cache capacity (e.g., 1000 unique markers)
  markerCache.setMaxCost(1000);
  
  setPlotSettings(PlotSettings());
}

Chart::~Chart() {
// #ifdef DEBUG
//   printf("Chart::~Chart\n");
// #endif
  for (int i = 0; i < p.size(); i++)
    delete p[i];
  p.clear();

  delete zoomInButton;
  delete zoomOutButton;
}

QWidget *Chart::weview() { return this; }

void Chart::Plot() { Refresh(); }

void Chart::Center() {
  qreal rescalefactor = 0.25;
  if (zoomStack.size() > 0 && p.size() > 0) {
    qreal minX, maxX, minY, maxY;
    // search min max between scatters
    if (!p.isEmpty()) {
        minX = maxX = p[0]->x();
        minY = maxY = p[0]->y();
        for (int i = 1; i < p.size(); i++) {
          if (p[i]->x() < minX) minX = p[i]->x();
          if (p[i]->x() > maxX) maxX = p[i]->x();
          if (p[i]->y() < minY) minY = p[i]->y();
          if (p[i]->y() > maxY) maxY = p[i]->y();
        }
    } else {
        minX = 0; maxX = 1; minY = 0; maxY = 1;
    }

    // search min max between curves
    for (int i = 0; i < curveMap.size(); i++) {
      const QVector<QPointF>& points = curveMap[i].getPoints();
      for (int j = 0; j < points.size(); j++) {
        if (points[j].x() < minX) minX = points[j].x();
        if (points[j].x() > maxX) maxX = points[j].x();
        if (points[j].y() < minY) minY = points[j].y();
        if (points[j].y() > maxY) maxY = points[j].y();
      }
    }

    zoomStack.first().minX = (minX - fabs(minX * rescalefactor));
    zoomStack.first().maxX = (maxX + fabs(maxX * rescalefactor));
    zoomStack.first().minY = (minY - fabs(minY * rescalefactor));
    zoomStack.first().maxY = (maxY + fabs(maxY * rescalefactor));
  }
}

void Chart::buildIndex() {
    m_searchIndex.clear();
    m_searchIndex.reserve(p.size());
    for(int i=0; i<p.size(); ++i) {
        m_searchIndex.append({p[i]->x(), i});
    }
    std::sort(m_searchIndex.begin(), m_searchIndex.end());
    m_indexDirty = false;
}

void Chart::Refresh() {
// #ifdef DEBUG
//   printf("Chart::Refresh\n");
// #endif
  if (m_indexDirty) {
      buildIndex();
  }
  refreshPixmap();
}

void Chart::setAntialiasing(bool antialiasing_) {
  antialiasing = antialiasing_;
}

void Chart::setXaxisName(QString xaxisname) {
  m_xaxisname = xaxisname;
}

void Chart::setYaxisName(QString yaxisname) {
  m_yaxisname = yaxisname;
}

void Chart::setPlotTitle(QString plottitle) {
  m_plottitle = plottitle;
}

void Chart::setPlotTitleSize(int size) {
  titleSize = size;
}

void Chart::setLabelDetail(bool labeldetail_) {
  labeldetail = labeldetail_;
}

void Chart::setAxisValueSize(int size) { axisValueSize = size; }

void Chart::setXLabelSize(int size) { xLabelSize = size; }

void Chart::setXminXmaxXTick(double xmin, double xmax, int xtick) {
  if (zoomStack.size() > 0 && curZoom < zoomStack.size()) {
    zoomStack[curZoom].minX = xmin;
    zoomStack[curZoom].maxX = xmax;
    zoomStack[curZoom].numXTicks = xtick;
    zoomStack[curZoom].adjust();
  } else {
    return;
  }
}

void Chart::getXminXmaxXTick(double *xmin, double *xmax, int *xtick) {
  if (zoomStack.size() > 0 && curZoom < zoomStack.size()) {
    (*xmin) = zoomStack[curZoom].minX;
    (*xmax) = zoomStack[curZoom].maxX;
    (*xtick) = zoomStack[curZoom].numXTicks;
  } else {
    (*xmin) = (*xmax) = (*xtick) = 0;
  }
}

void Chart::setYLabelSize(int size) { yLabelSize = size; }

void Chart::setYminYmaxYTick(double ymin, double ymax, int ytick) {
  if (zoomStack.size() > 0 && curZoom < zoomStack.size()) {
    zoomStack[curZoom].minY = ymin;
    zoomStack[curZoom].maxY = ymax;
    zoomStack[curZoom].numYTicks = ytick;
    zoomStack[curZoom].adjust();
  }
}

void Chart::getYminYmaxYTick(double *ymin, double *ymax, int *ytick) {
  if (zoomStack.size() > 0 && curZoom < zoomStack.size()) {
    (*ymin) = zoomStack[curZoom].minY;
    (*ymax) = zoomStack[curZoom].maxY;
    (*ytick) = zoomStack[curZoom].numYTicks;
  } else {
    (*ymin) = (*ymax) = (*ytick) = 0;
  }
}

void Chart::DoSelection(int low, int high) {
  int i;
  for (i = low; i < high; i++) {
    p[i]->setSelection(true);
  }
}

void Chart::Select(int from, int to) {
  if (from < to) {
    int mid = (from + to) / 2;
    Select(from, mid);
    Select(mid + 1, to);
    DoSelection(from, to);
  }
}

void Chart::SelectAll() {
  Select(0, p.size());
  refreshPixmap();
}

void Chart::DoUnselection(int low, int high) {
  int i;
  for (i = low; i < high; i++) {
    if (p[i]->isSelected() == true) {
      p[i]->setSelection(false);
      pforward.append(i);
    }
  }
}

void Chart::Unselect(int from, int to) {
  if (from < to) {
    int mid = (from + to) / 2;
    Unselect(from, mid);
    Unselect(mid + 1, to);
    DoUnselection(from, to);
  }
}

void Chart::ClearSelection() {
  /* Unselection by divide and conqueror technique*/
  Unselect(0, p.size());
  refreshPixmap();
}

void Chart::RemoveCurveAt(int cid) {
  curveMap.remove(cid);
}

void Chart::RemoveAllCurves() {
  curveMap.clear();
}

void Chart::setPlotSettings(const PlotSettings &settings) {
  zoomStack.clear();
  zoomStack.append(settings);
  curZoom = 0;
  zoomInButton->hide();
  zoomOutButton->hide();
  refreshPixmap();
}

void Chart::zoomOut() {
  if (curZoom > 0) {
    --curZoom;
    zoomOutButton->setEnabled(curZoom > 0);
    zoomInButton->setEnabled(true);
    zoomInButton->show();
    refreshPixmap();
  }
}

void Chart::zoomIn() {
  if (curZoom < zoomStack.count() - 1) {
    ++curZoom;
    zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
    zoomOutButton->setEnabled(true);
    zoomOutButton->show();
    refreshPixmap();
  }
}

void Chart::addPoint(qreal x, qreal y, QString name) {
  p.append(new DataPoint(x, y, name));
  m_indexDirty = true;
}

void Chart::addPoint(qreal x, qreal y, QString name, QColor color, int radius) {
  p.append(new DataPoint(x, y, name));
  p.last()->setColor(color);
  p.last()->setRadius(radius);
  m_indexDirty = true;
}

void Chart::addCurve(QVector<QPointF> curve, QString name, QColor color, bool smooth) {
  DataCurve dc(curve, name, color);
  dc.setSmooth(smooth);
  curveMap.append(dc);
}

int Chart::PointSize() const {
  return p.size();
}

DataPoint *Chart::getPoint(int id) {
  if (id < p.size())
    return p[id];
  else
    return 0;
}

void Chart::RemovePointAt(int id) {
  if (id < p.size()) {
    delete p[id];
    p.remove(id);
    m_indexDirty = true;
  }
}

void Chart::RemovePoint(DataPoint *dp) {
  auto indx = p.indexOf(dp);
  if (indx > -1) {
    delete p[indx];
    p.remove(indx);
    m_indexDirty = true;
  }
}

void Chart::RemoveAllPoints() {
  for (int i = 0; i < p.size(); i++) {
    delete p[i];
  }
  p.clear();
  m_indexDirty = true;
}

QVector<DataCurve> Chart::getCurves() {
  return curveMap;
}

QSize Chart::minimumSizeHint() const {
  return QSize(6 * Margin, 4 * Margin);
}

QSize Chart::sizeHint() const {
  return QSize(12 * Margin, 8 * Margin);
}

void Chart::SaveAsImage(QString imgname) {
  if (imgname.contains(".png", Qt::CaseInsensitive) == true) {
    pixmap.save(imgname, "PNG", 100);
  } else if (imgname.contains(".jpg", Qt::CaseInsensitive) == true ||
             imgname.contains(".jpeg", Qt::CaseInsensitive) == true) {
    pixmap.save(imgname, "JPG", 100);
  } else if (imgname.contains(".pdf", Qt::CaseInsensitive) == true) {
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(imgname);
    printer.setResolution(300); // override to 300dpi
    printer.setFontEmbeddingEnabled(true);
    printer.setColorMode(QPrinter::Color);
    printer.setPageOrientation(QPageLayout::Portrait);
    printer.setPageSize(QPageSize(size()));
    printer.setFullPage(true);
    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing, antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing, antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, antialiasing);
    painter.drawPixmap(rect(), pixmap);
  } else {
    pixmap.save(imgname + ".png");
  }
  QMessageBox::information(
      this, "Plot Saved!",
      QString("The plot has been saved as %1").arg(imgname), QMessageBox::Ok);
}

void Chart::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QStylePainter painter(this);
  painter.drawPixmap(0, 0, pixmap);

  if (rubberBandIsShown) {
    QPen rubberbandpen = QPen(QColor(255, 0, 0, 125));
    painter.setPen(rubberbandpen);
    painter.drawRect(rubberBandRect.normalized().adjusted(0, 0, -1, -1));
  }
  
  if (m_isLassoActive && !m_lassoPolygon.isEmpty()) {
      painter.setPen(QPen(QColor(255, 0, 0, 125), 1, Qt::DashLine));
      painter.setBrush(QColor(255, 0, 0, 30));
      painter.drawPolygon(m_lassoPolygon);
  }

  if (hasFocus()) {
    QStyleOptionFocusRect option;
    option.initFrom(this);
    option.backgroundColor = QColor(255, 0, 0, 125);
    painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);
  }
}

void Chart::resizeEvent(QResizeEvent *event) {
  Q_UNUSED(event);
  int x = width() - (zoomInButton->width() + zoomOutButton->width() + 10);
  zoomInButton->move(x, 5);
  zoomOutButton->move(x + zoomInButton->width() + 5, 5);
  refreshPixmap();
}

void Chart::mousePressEvent(QMouseEvent *event) {
  QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);

  if (event->button() == Qt::MiddleButton) {
    // Zoom Logic
    if (rect.contains(event->pos())) {
      rubberBandIsShown = true;
      rubberBandRect.setTopLeft(event->pos());
      rubberBandRect.setBottomRight(event->pos());
      updateRubberBandRegion();
      setCursor(Qt::CrossCursor);
    }
  } else if (event->button() == Qt::LeftButton) {
    // Lasso Selection Logic
    if (rect.contains(event->pos())) {
        m_isLassoActive = true;
        m_lassoPolygon.clear();
        m_lassoPolygon << event->pos();
        setCursor(Qt::CrossCursor);
    }
  } else if ((event->button() == Qt::RightButton)) {
    rubberBandIsShown = false;
    m_isLassoActive = false;
    m_lassoPolygon.clear();
    update();
  }
}

void Chart::mouseMoveEvent(QMouseEvent *event) {
  if (rubberBandIsShown == true) {
    updateRubberBandRegion();
    rubberBandRect.setBottomRight(event->pos());
    updateRubberBandRegion();
  } else if (m_isLassoActive) {
      m_lassoPolygon << event->pos();
      update();
  } else {
      // Hover Logic
      if (p.isEmpty() || zoomStack.isEmpty()) return;
      if (m_indexDirty) {
          // Should have been built in Refresh, but if modified after...
          // Don't build here to avoid lag. 
          return; 
      }
      
      QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
      if (!rect.contains(event->pos())) {
          QToolTip::hideText();
          return;
      }

      PlotSettings settings = zoomStack[curZoom];
      double spanX = settings.spanX();
      double spanY = settings.spanY();
      
      // Prevent division by zero
      if (std::abs(spanX) < 1e-9 || std::abs(spanY) < 1e-9) return; 
      
      // Calculate data coordinates of mouse
      double dx = (event->pos().x() - rect.left()) * spanX / (rect.width() - 1);
      double mouseDataX = settings.minX + dx;
      
      // Search tolerance in pixels
      const double tolPx = 10.0;
      const double tolSq = tolPx * tolPx;
      
      // Convert tolerance to data units for searching
      double tolDataX = tolPx * spanX / (rect.width() - 1);
      
      // Binary search for range of X values
      auto itLow = std::lower_bound(m_searchIndex.begin(), m_searchIndex.end(), mouseDataX - tolDataX);
      auto itHigh = std::lower_bound(m_searchIndex.begin(), m_searchIndex.end(), mouseDataX + tolDataX);
      
      DataPoint* nearest = nullptr;
      double minDistSq = tolSq + 1.0; // Start with max allowed
      
      for(auto it = itLow; it != itHigh; ++it) {
          int idx = it->index;
          DataPoint* dp = p[idx];
          if (!dp->isVisible()) continue;
          
          // Calculate screen position of point
          double ptDx = dp->x() - settings.minX;
          double ptDy = dp->y() - settings.minY;
          double screenX = rect.left() + (ptDx * (rect.width() - 1) / spanX);
          double screenY = rect.bottom() - (ptDy * (rect.height() - 1) / spanY);
          
          double distSq = std::pow(screenX - event->pos().x(), 2) + std::pow(screenY - event->pos().y(), 2);
          
          if (distSq < minDistSq) {
              minDistSq = distSq;
              nearest = dp;
          }
      }
      
      if (nearest) {
          QString text = QString("<b>%1</b><br>x: %2<br>y: %3").arg(nearest->name()).arg(nearest->x()).arg(nearest->y());

          if (m_images.contains(nearest->name())) {
              QByteArray bArray;
              QBuffer buffer(&bArray);
              buffer.open(QIODevice::WriteOnly);
              QPixmap pm = m_images[nearest->name()];
              if (pm.width() > 200) {
                 pm = pm.scaledToWidth(200, Qt::SmoothTransformation);
              }
              pm.save(&buffer, "PNG");
              QString imgBase64 = QString::fromLatin1(bArray.toBase64().data());
              text = QString("<img src='data:image/png;base64,%1'><br>").arg(imgBase64) + text;
          }

          QToolTip::showText(event->globalPosition().toPoint(), text, this);
      } else {
          QToolTip::hideText(); // Or let it timeout
      }
  }
}

void Chart::mouseReleaseEvent(QMouseEvent *event) {
  if ((event->button() == Qt::MiddleButton) && rubberBandIsShown == true) {
    rubberBandIsShown = false;
    updateRubberBandRegion();
    unsetCursor();

    QRect rect = rubberBandRect.normalized();
    if (rect.width() < 4 || rect.height() < 4)
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
  } else if ((event->button() == Qt::LeftButton) && m_isLassoActive) {
    m_isLassoActive = false;
    unsetCursor();
    
    // Perform Selection
    QRect rect = m_lassoPolygon.boundingRect(); // Screen space bounding box
    QRect plotRect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    
    PlotSettings settings = zoomStack[curZoom];
    double spanX = settings.spanX();
    double spanY = settings.spanY();
    
    // Optimization: Filter by X-range using sorted index
    double minScreenX = rect.left();
    double maxScreenX = rect.right();
    
    double minDataX = settings.minX + (minScreenX - plotRect.left()) * spanX / (plotRect.width() - 1);
    double maxDataX = settings.minX + (maxScreenX - plotRect.left()) * spanX / (plotRect.width() - 1);
    
    // Ensure bounds are correct order
    if (minDataX > maxDataX) std::swap(minDataX, maxDataX);
    
    auto itLow = std::lower_bound(m_searchIndex.begin(), m_searchIndex.end(), minDataX);
    auto itHigh = std::lower_bound(m_searchIndex.begin(), m_searchIndex.end(), maxDataX);
    
    // Allow toggle behavior or multi-select? Usually plain click clears, drag selects.
    // Assuming cumulative selection or just this selection? 
    // The previous code toggled selection state for rect. Let's keep toggle/set logic.
    // Replicating previous logic:
    /*
        if (p[i]->isSelected()) {
          p[i]->setSelection(false);
        } else {
          p[i]->setSelection(true);
        }
    */
    
    for(auto it = itLow; it != itHigh; ++it) {
        int idx = it->index;
        DataPoint* dp = p[idx];
        if (!dp->isVisible()) continue;
        
        // Project point to screen
        double ptDx = dp->x() - settings.minX;
        double ptDy = dp->y() - settings.minY;
        double screenX = plotRect.left() + (ptDx * (plotRect.width() - 1) / spanX);
        double screenY = plotRect.bottom() - (ptDy * (plotRect.height() - 1) / spanY);
        QPoint pt(screenX, screenY);
        
        // Fast Bounding Box Check first
        if (!rect.contains(pt)) continue;
        
        // Detailed Polygon Check
        if (m_lassoPolygon.containsPoint(pt, Qt::OddEvenFill)) {
            // Toggle selection
             dp->setSelection(!dp->isSelected());
        }
    }

    m_lassoPolygon.clear();
    refreshPixmap();
  }
  else {
    return;
  }
}

void Chart::keyPressEvent(QKeyEvent *event) {
  switch (event->key()) {
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

void Chart::wheelEvent(QWheelEvent *event) {
  QPoint numDegrees = event->angleDelta() / 8;
  int numTicks = numDegrees.y() / 15;
  zoomStack[curZoom].scroll(0, numTicks);
  refreshPixmap();
}

void Chart::updateRubberBandRegion() {
  QRect rect = rubberBandRect.normalized();
  update(rect.left(), rect.top(), rect.width(), 1);
  update(rect.left(), rect.top(), 1, rect.height());
  update(rect.left(), rect.bottom(), rect.width(), 1);
  update(rect.right(), rect.top(), 1, rect.height());
}

void Chart::refreshPixmap() {
  pixmap = QPixmap(size());
  pixmap.fill(Qt::white);

  QPainter painter(&pixmap);
  painter.setRenderHint(QPainter::Antialiasing, antialiasing);
  painter.setRenderHint(QPainter::TextAntialiasing, antialiasing);
  painter.setRenderHint(QPainter::SmoothPixmapTransform, antialiasing);

  drawGrid(&painter);

  if (curveMap.size() > 0)
    drawCurves(&painter);

  if (p.size() > 0)
    drawScatters(&painter);

  update();
}

int getDecimals(double x) {
  QString str = QString::number(x);
  QString decimals = str.split(".").last();
  int i, d = 1;
  for (i = 0; i < decimals.size(); i++) {
    if (decimals[i].digitValue() == 0)
      d++;
    else
      break;
  }
  if (decimals.contains("e") == true) {
    return d + 6;
  } else {
    return d + 1;
  }
}

void Chart::drawGrid(QPainter *painter) {
  painter->setRenderHint(QPainter::Antialiasing, antialiasing);
  painter->setRenderHint(QPainter::TextAntialiasing, antialiasing);
  painter->setRenderHint(QPainter::SmoothPixmapTransform, antialiasing);

  QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
  if (!rect.isValid())
    return;

  PlotSettings settings = zoomStack[curZoom];

  QPen gridpen = QPen(QColor(220, 220, 220));
  gridpen.setStyle(Qt::DotLine);
  QPen axespen = QPen(QColor(120, 120, 120), 1);

  // Subtle background for the plot area
  painter->fillRect(rect, QColor(252, 252, 252));

  qreal min, max, stepx, stepy;

  min = floor(settings.minX);
  max = ceil(settings.maxX);
  stepx = (max - min) / (double)settings.numXTicks;
  if (stepx > 1.f) {
    stepx = ceil(stepx);
  }

  QFont font("Inter", 10);
  font.setPointSizeF(font.pointSizeF() * axisValueSize);
  font.setStyleHint(QFont::SansSerif);
  painter->setFont(font);
  painter->setPen(axespen);

  int xsteps = static_cast<int>((max - min) / stepx) + 1;
  for (int i = 0; i < xsteps; ++i) {
      double ix = min + i * stepx;
      double dx = ix - settings.minX;
      double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
      if (x > rect.left() && x < rect.right()) {
          painter->setPen(gridpen);
          painter->drawLine(x, rect.top(), x, rect.bottom());
          painter->setPen(axespen);
          painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
          if (std::abs(ix) < 1e-5) {
              painter->drawText(x - 50, rect.bottom() + 10, 100, 15,
                                Qt::AlignHCenter | Qt::AlignTop, QString::number(0));
          } else {
              painter->drawText(x - 50, rect.bottom() + 10, 100, 15,
                                Qt::AlignHCenter | Qt::AlignTop,
                                QString::number(ix, 'f', getDecimals(stepx)));
          }
      }
  }

  min = floor(settings.minY);
  max = ceil(settings.maxY);
  stepy = (max - min) / (double)settings.numYTicks;
  if (stepy > 1.f) {
    stepy = ceil(stepy);
  }

  int ysteps = static_cast<int>((max - min) / stepy) + 1;
  for (int i = 0; i < ysteps; ++i) {
      double iy = min + i * stepy;
      double dy = iy - settings.minY;
      double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
      if (y > rect.top() && y < rect.bottom()) {
          painter->setPen(gridpen);
          painter->drawLine(rect.left(), y, rect.right(), y);
          painter->setPen(axespen);
          painter->drawLine(rect.left() - 5, y, rect.left(), y);
          if (std::abs(iy) < 1e-5) {
              painter->drawText(rect.left() - Margin - 5, y - 10, Margin - 5, 20,
                                Qt::AlignRight | Qt::AlignVCenter,
                                QString::number(0));
          } else {
              painter->drawText(rect.left() - Margin - 5, y - 10, Margin - 5, 20,
                                Qt::AlignRight | Qt::AlignVCenter,
                                QString::number(iy, 'f', getDecimals(stepy)));
          }
      }
  }

  painter->setPen(axespen);

  font.setPointSize(10);
  font.setPointSizeF(font.pointSizeF() * xLabelSize);
  font.setBold(true);
  painter->setFont(font);
  QFontMetrics fm(font);
  qreal xmarkTextWidth = (qreal)fm.horizontalAdvance(m_xaxisname);
  qreal x_text = Margin + (rect.right() - rect.left()) / 2. - xmarkTextWidth / 2.;

  painter->drawText(x_text, rect.bottom() + Margin / 2., m_xaxisname);

  font.setPointSize(10);
  font.setPointSizeF(font.pointSizeF() * titleSize);
  font.setBold(true);
  painter->setFont(font);
  xmarkTextWidth = (qreal)fm.horizontalAdvance(m_plottitle);
  x_text = Margin + (rect.right() - rect.left()) / 2. - xmarkTextWidth / 2.;
  painter->drawText(x_text, rect.top() - Margin / 2., m_plottitle);

  font.setPointSize(10);
  font.setPointSizeF(font.pointSizeF() * yLabelSize);
  font.setBold(true);
  painter->setFont(font);
  painter->save();
  qreal ymarkTextWidth = (qreal)fm.horizontalAdvance(m_yaxisname);
  qreal y_text = Margin + (rect.bottom() - rect.top()) / 2. + ymarkTextWidth / 2.;
  painter->translate(rect.left() - Margin / 1.2, y_text);
  painter->rotate(270);
  painter->drawText(0, 0, m_yaxisname);
  painter->restore();

  // Draw axis lines
  painter->setPen(axespen);
  painter->drawLine(rect.bottomLeft(), rect.bottomRight());
  painter->drawLine(rect.topLeft(), rect.bottomLeft());
}

void Chart::drawCurves(QPainter *painter) {
  painter->setRenderHint(QPainter::Antialiasing, antialiasing);

  PlotSettings settings = zoomStack[curZoom];
  QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
  if (!rect.isValid())
    return;

  painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

  // Pre-calculate scaling factors
  double scaleX = (rect.width() - 1) / settings.spanX();
  double scaleY = (rect.height() - 1) / settings.spanY();
  double offsetX = rect.left() - settings.minX * scaleX;
  double offsetY = rect.bottom() + settings.minY * scaleY;

  for (int i = 0; i < curveMap.size(); i++) {
    const DataCurve& data = curveMap[i];
    if (data.isVisible() && !data.getPoints().isEmpty()) {
        const QVector<QPointF>& points = data.getPoints();
        QPainterPath path;
        
        // Move to the first point
        QPointF p0 = points[0];
        path.moveTo(offsetX + p0.x() * scaleX, offsetY - p0.y() * scaleY);

        if (data.isSmooth()) {
            for (int j = 0; j < points.size() - 1; ++j) {
                QPointF p1 = points[j];
                QPointF p2 = points[j+1];

                // Screen coordinates
                double x1 = offsetX + p1.x() * scaleX;
                double y1 = offsetY - p1.y() * scaleY;
                double x2 = offsetX + p2.x() * scaleX;
                double y2 = offsetY - p2.y() * scaleY;

                // Control points for cubic Bezier
                QPointF c1((x1 + x2) / 2, y1);
                QPointF c2((x1 + x2) / 2, y2);
                
                path.cubicTo(c1, c2, QPointF(x2, y2));
            }
        } else {
            for (int j = 1; j < points.size(); ++j) {
                QPointF p = points[j];
                path.lineTo(offsetX + p.x() * scaleX, offsetY - p.y() * scaleY);
            }
        }
        
        painter->setPen(QPen(data.color(), data.width(), Qt::SolidLine,
                             Qt::RoundCap, Qt::RoundJoin));
        painter->drawPath(path);
    }
  }
}

void Chart::PointDraw(QPainter *painter, QRect rect, PlotSettings settings,
                      DataPoint *p) {
    // This method is now legacy/fallback or for selected points.
    // Logic integrated into drawScatters for batching.
    // Keeping it for single point draws if needed.
    
    double dx = p->x() - settings.minX;
    double dy = p->y() - settings.minY;
    double x = rect.left() + (dx * (rect.width() - 1) / settings.spanX());
    double y = rect.bottom() - (dy * (rect.height() - 1) / settings.spanY());
    
    QString key = getMarkerKey(p->marker(), p->radius(), p->color(), p->isSelected());
    QPixmap* pm = markerCache.object(key);
    
    if (pm) {
         painter->drawPixmap(x - pm->width()/2, y - pm->height()/2, *pm);
    } else {
        // Fallback or generate? Better generate in drawScatters
        qreal radius = p->radius();
        QRectF point = QRectF(x - radius / 2., y - radius / 2., radius, radius);
        painter->setPen(p->isSelected() ? QPen(Qt::red, 2) : QPen(p->color().darker(150), 0.5));
        painter->setBrush(p->color());
        if (p->marker() == CIRCLE) painter->drawEllipse(point);
        else if (p->marker() == SQUARE) painter->drawRect(point);
        else { /* triangle logic */ }
    }
}

void Chart::drawScatters(QPainter *painter) {
  painter->setRenderHint(QPainter::Antialiasing, antialiasing);
  painter->setRenderHint(QPainter::SmoothPixmapTransform, antialiasing);

  PlotSettings settings = zoomStack[curZoom];
  QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
  if (!rect.isValid())
    return;

  painter->setClipRect(rect.adjusted(+1, +1, -1, -1));

  double scaleX = (rect.width() - 1) / settings.spanX();
  double scaleY = (rect.height() - 1) / settings.spanY();
  double offsetX = rect.left() - settings.minX * scaleX;
  double offsetY = rect.bottom() + settings.minY * scaleY;

  // Spatial Hashing Grid for Decimation
  // We use a simple 2D bool array flattened.
  // Resolution: 2x2 pixels. If a point is drawn there, skip others.
  const int cellSize = 2; 
  int gridW = rect.width() / cellSize + 1;
  int gridH = rect.height() / cellSize + 1;
  // Use generic vector, minimal memory allocation
  QVector<quint8> grid;
  // Only allocate grid if we have many points to justify overhead
  bool useDecimation = (p.size() > 5000); 
  if (useDecimation) {
      grid.resize(gridW * gridH);
      grid.fill(0);
  }

  // Helper to get cached marker
  auto drawMarker = [&](double x, double y, DataPoint* dp, bool selected) {
       QString key = getMarkerKey(dp->marker(), dp->radius(), dp->color(), selected);
       QPixmap* pm = markerCache.object(key);
       if (!pm) {
           // Create and cache
           int r = dp->radius();
           if (r < 1) r = 1;
           // Make pixmap slightly larger for AA and thicker selection pen
           int margin = selected ? 3 : 2;
           int size = r + margin * 2; 
           QPixmap* newPm = new QPixmap(size, size);
           newPm->fill(Qt::transparent);
           QPainter pPm(newPm);
           pPm.setRenderHint(QPainter::Antialiasing, true);
           if (selected) {
               pPm.setPen(QPen(Qt::red, 2));
           } else {
               pPm.setPen(QPen(dp->color().darker(150), 0.5));
           }
           pPm.setBrush(dp->color());
           
           QRectF shapeRect(margin, margin, r, r); // Centered
           if (dp->marker() == CIRCLE) {
               pPm.drawEllipse(shapeRect);
           } else if (dp->marker() == SQUARE) {
               pPm.drawRect(shapeRect);
           } else {
               QPolygonF tri;
               tri << QPointF(size/2.0, margin) << QPointF(size-margin, size-margin) << QPointF(margin, size-margin);
               pPm.drawPolygon(tri);
           }
           markerCache.insert(key, newPm);
           pm = newPm;
       }
       painter->drawPixmap(x - pm->width()/2.0, y - pm->height()/2.0, *pm);
  };

  // Draw unselected (decimated)
  for (int i = 0; i < p.size(); i++) {
    DataPoint* dp = p[i];
    if (!dp->isVisible() || dp->isSelected()) continue; // Draw selected later

    double x = offsetX + dp->x() * scaleX;
    double y = offsetY - dp->y() * scaleY;

    // Bounds check
    if (x < rect.left() || x > rect.right() || y < rect.top() || y > rect.bottom()) continue;

    if (useDecimation) {
        int gx = (x - rect.left()) / cellSize;
        int gy = (y - rect.top()) / cellSize;
        if (gx >= 0 && gx < gridW && gy >= 0 && gy < gridH) {
            if (grid[gy * gridW + gx]) continue; // Already occupied
            grid[gy * gridW + gx] = 1;
        }
    }
    drawMarker(x, y, dp, false);
  }

  // Draw selected (ALWAYS draw, no decimation, on top)
  for (int i = 0; i < p.size(); i++) {
    DataPoint* dp = p[i];
    if (dp->isVisible() && dp->isSelected()) {
       double x = offsetX + dp->x() * scaleX;
       double y = offsetY - dp->y() * scaleY;
       
       drawMarker(x, y, dp, true);
    }
  }

  // Draw labels (on top of everything)
  painter->setPen(Qt::red);
  QFont labelFont = painter->font();
  labelFont.setPointSize(8);
  painter->setFont(labelFont);
  for (int i = 0; i < p.size(); i++) {
    DataPoint* dp = p[i];
    if (dp->isVisible() && dp->isLabelVisible()) {
       double x = offsetX + dp->x() * scaleX;
       double y = offsetY - dp->y() * scaleY;
       
       painter->drawText(x + dp->radius() / 2.0 + 2, y + 4, dp->name());
    }
  }
}

PlotSettings::PlotSettings() {
  minX = 0.0;
  maxX = 10.0;
  numXTicks = 10;
  minY = 0.0;
  maxY = 10.0;
  numYTicks = 10;
}

void PlotSettings::scroll(int dx, int dy) {
  double stepX = spanX() / numXTicks;
  minX += dx * stepX;
  maxX += dx * stepX;

  double stepY = spanY() / numYTicks;
  minY += dy * stepY;
  maxY += dy * stepY;
}

void PlotSettings::adjust() {
  adjustAxis(minX, maxX, numXTicks);
  adjustAxis(minY, maxY, numYTicks);
}

void Chart::setImages(const QMap<QString, QPixmap> &images) {
  m_images = images;
}

void Chart::LoadSettings() {
  QSettings settings("QStudioMetrics", "PlotSettings");
  if (settings.contains("titleSize")) {
    setPlotTitleSize(settings.value("titleSize").toInt());
  }
  if (settings.contains("axisValueSize")) {
    setAxisValueSize(settings.value("axisValueSize").toInt());
  }
  if (settings.contains("xLabelSize")) {
    setXLabelSize(settings.value("xLabelSize").toInt());
  }
  if (settings.contains("yLabelSize")) {
    setYLabelSize(settings.value("yLabelSize").toInt());
  }
}
void PlotSettings::adjustAxis(double &min, double &max, int &numTicks) {
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
