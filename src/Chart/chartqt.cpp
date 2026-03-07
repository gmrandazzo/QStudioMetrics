/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for chartqt.
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

#include "chartqt.h"
#include "databar.h"
#include "datapoint.h"
#include <QLabel>
#include <QMap>
#include <QToolButton>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QValueAxis>
#include <QtCore/QEvent>
#include <QtCore/QtMath>
#include <QtGui/QImage>
#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QGraphicsTextItem>
#include <QBuffer>
#include <QByteArray>
#include <QSettings>

bool ChartQt::viewportEvent(QEvent *event) {
  if (event->type() == QEvent::TouchBegin) {
    // By default touch events are converted to mouse events. So
    // after this event we will get a mouse event also but we want
    // to handle touch events as gestures only. So we need this safeguard
    // to block mouse events that are actually generated from touch.
    m_isTouching = true;

    // Turn off animations when handling gestures they
    // will only slow us down.
    chart()->setAnimationOptions(QChart::NoAnimation);
  }

  return QChartView::viewportEvent(event);
}

void ChartQt::resizeEvent(QResizeEvent *event) {
  QChartView::resizeEvent(event);
  int spacing = 5;
  int x = width() - (zoomInButton->width() + zoomOutButton->width() + recentreButton->width() + spacing * 3);
  zoomInButton->move(x, 5);
  x += zoomInButton->width() + spacing;
  zoomOutButton->move(x, 5);
  x += zoomOutButton->width() + spacing;
  recentreButton->move(x, 5);
}

void ChartQt::mousePressEvent(QMouseEvent *event) {
  qDebug() << "mousePressEvent " << event;
  if (m_isTouching)
    return;

  auto const widgetPos = event->position();
  auto const scenePos = mapToScene(
      QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
  auto const chartItemPos = chart()->mapFromScene(scenePos);
  mPresscc = chart()->mapToValue(chartItemPos);

  if (event->button() == Qt::LeftButton) {
      setDragMode(QGraphicsView::RubberBandDrag);
  } else if (event->button() == Qt::RightButton) {
      m_lastMousePos = event->pos();
      setCursor(Qt::ClosedHandCursor);
  }

  QChartView::mousePressEvent(event);
}

void ChartQt::mouseMoveEvent(QMouseEvent *event) {
  if (m_isTouching)
    return;

  if (event->buttons() & Qt::RightButton) {
      QPoint delta = event->pos() - m_lastMousePos;
      chart()->scroll(-delta.x(), delta.y());
      m_lastMousePos = event->pos();
      event->accept();
  }

  QChartView::mouseMoveEvent(event);
}

void ChartQt::showLabels() {
  for (int i = 0; i < plotLabels.size(); i++)
    delete plotLabels[i];
  plotLabels.clear();

  for (int i = 0; i < p.size(); i++) {
    if (p[i]->isSelected()) {
      plotLabels.append(new QLabel(this));
      plotLabels.last()->setStyleSheet(
          QString("QLabel{color:#1564FF; font-family:\"SansSerif\"; "
                  "font-size:12px; font-weight:normal;"
                  " background-color:rgba(21, 100, 255, 51); "
                  "border-radius:4px; text-align:center;}"));
      plotLabels.last()->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
      plotLabels.last()->setText(QString("%1 (%2 ; %3)")
                                     .arg(p[i]->name())
                                     .arg(p[i]->x())
                                     .arg(p[i]->y()));
      QPointF pos = chart()->mapToPosition(QPointF(p[i]->x(), p[i]->y()));
      plotLabels.last()->move(pos.x() - plotLabels.last()->width() / 2,
                              pos.y() - plotLabels.last()->height() * 1.5);
      plotLabels.last()->show();
    } else {
      continue;
    }
  }
}

void ChartQt::mouseReleaseEvent(QMouseEvent *event) {
  qDebug() << "mouseReleaseEvent " << event;
  m_isTouching = false;

  if (event->button() == Qt::RightButton) {
      unsetCursor();
  }

  auto const widgetPos = event->position();
  auto const scenePos = mapToScene(
      QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
  auto const chartItemPos = chart()->mapFromScene(scenePos);
  mReleasecc = chart()->mapToValue(chartItemPos);
  /* auto const valueGivenSeries = chart()->mapToValue(chartItemPos);
   maxX = valueGivenSeries.x();
   maxY = valueGivenSeries.y();*/
  qreal minX, maxX, minY, maxY;

  if (mPresscc.x() > mReleasecc.x()) {
    minX = mReleasecc.x();
    maxX = mPresscc.x();
  } else {
    minX = mPresscc.x();
    maxX = mReleasecc.x();
  }

  if (mPresscc.y() > mReleasecc.y()) {
    minY = mReleasecc.y();
    maxY = mPresscc.y();
  } else {
    minY = mPresscc.y();
    maxY = mReleasecc.y();
  }

  if (event->button() == Qt::MiddleButton) {
    chart()->zoomIn();
  } else if (event->button() == Qt::LeftButton) {
    // select points
    for (int i = 0; i < p.size(); i++) {
      if (p[i]->x() > minX && p[i]->x() < maxX && p[i]->y() > minY &&
          p[i]->y() < maxY) {
        if (p[i]->isSelected()) {
          p[i]->setSelection(false);
        } else {
          p[i]->setSelection(true);
        }
      } else {
        continue;
      }
    }

    // select bars
    const QBarCategoryAxis *axisX =
        qobject_cast<QBarCategoryAxis *>(chart()->axes(Qt::Horizontal).at(0));
    if (axisX) {
        for (int i = 0; i < b.size(); i++) {
            for (int j = 0; j < b[i]->x().size(); j++) {
                // For categorical axis, the values are indices 0, 1, 2...
                // We check if the index j is within [minX, maxX] 
                // and the value y[j] is within [minY, maxY]
                if (j >= minX - 0.5 && j <= maxX + 0.5 &&
                    ((b[i]->y()[j] >= minY && b[i]->y()[j] <= maxY) ||
                     (b[i]->y()[j] <= minY && b[i]->y()[j] >= maxY))) {
                    // Note: DataBar selection might need to be per-bar if we want to be precise,
                    // but the current DataBar class seems to have a single selection state.
                    // If we want to support per-bar selection we might need to change DataBar.
                    // For now let's toggle the whole DataBar if any of its bars are selected.
                    b[i]->setSelection(!b[i]->isSelected());
                    break; // break inner loop if series is selected
                }
            }
        }
    }
    refreshPlot();
  } else if (event->button() == Qt::RightButton) {
      // Panning handled in mouseMove, do nothing here to avoid zoomOut
  } else {
    chart()->zoomOut();
  }

  // Because we disabled animations when touch event was detected
  // we must put them back on.
  chart()->setAnimationOptions(QChart::SeriesAnimations);
  QChartView::mouseReleaseEvent(event);
}

void ChartQt::wheelEvent(QWheelEvent *event) {
    qreal factor = event->angleDelta().y() > 0 ? 1.1 : 0.9;
    
    QRectF rect = chart()->plotArea();
    QPointF mousePos = event->position();
    
    // Zoom around mouse position
    qreal width = rect.width() / factor;
    qreal height = rect.height() / factor;
    qreal x = mousePos.x() - (mousePos.x() - rect.left()) / factor;
    qreal y = mousePos.y() - (mousePos.y() - rect.top()) / factor;
    
    chart()->zoomIn(QRectF(x, y, width, height));
    
    event->accept();
}

void ChartQt::keyPressEvent(QKeyEvent *event) {
  qDebug() << "Key " << event->key();
  switch (event->key()) {
  case Qt::Key_Plus:
    chart()->zoomIn();
    break;
  case Qt::Key_Minus:
    chart()->zoomOut();
    break;

  case Qt::Key_Left:
    chart()->scroll(-10, 0);
    break;
  case Qt::Key_Right:
    chart()->scroll(10, 0);
    break;
  case Qt::Key_Up:
    chart()->scroll(0, 10);
    break;
  case Qt::Key_Down:
    chart()->scroll(0, -10);
    break;
  default:
    QGraphicsView::keyPressEvent(event);
    break;
  }
}

void ChartQt::drawCurves() {
  // #ifdef DEBUG
  printf("Chart::drawCurves\n");
  // #endif
  for (int i = 0; i < curveMap.size(); i++) {
    const DataCurve data = curveMap[i];
    if (data.getPoints().size() > 0) {
      QXYSeries *series = 0;
      if (data.isSmooth()) {
          series = new QSplineSeries;
      } else {
          series = new QLineSeries;
      }
      QLineSeries *line = static_cast<QLineSeries *>(series);

      // QLineSeries *lseries = new QLineSeries();
      for (int j = 0; j < data.getPoints().size(); ++j) {
        line->append(data.getPoints()[j].x(), data.getPoints()[j].y());
      }
      line->setColor(data.color());
      // line->setUseOpenGL(true);
      chart()->addSeries(line);
      curvesList.append(series);
    } else {
      continue;
    }
  }
}

void ChartQt::updateCurves() {
  // #ifdef DEBUG
  printf("Chart::updateCurves\n");
  // #endif
  for (int i = 0; i < curveMap.size(); i++) {
    const DataCurve data = curveMap[i];
    QLineSeries *line = static_cast<QLineSeries *>(curvesList[i]);

    if (data.isVisible() == true) {
      if (data.getPoints().size() > 0) {
        // QLineSeries *lseries = new QLineSeries();
        for (int j = 0; j < data.getPoints().size(); ++j) {
          line->replace(j, data.getPoints()[j].x(), data.getPoints()[j].y());
        }
        line->setColor(data.color());
      } else {
        continue;
      }

    } else {
      line->hide();
    }
  }
}

void ChartQt::slotPointHoverd(const QPointF &point, bool state) {
  if (state) {
    const DataPoint *nearest = nullptr;
    for (int i = 0; i < p.size(); ++i) {
      if (std::abs(p[i]->x() - point.x()) < 1e-7 &&
          std::abs(p[i]->y() - point.y()) < 1e-7) {
        nearest = p[i];
        break;
      }
    }

    QString name = nearest ? nearest->name() : "Unknown";
    QString text =
        QString("<b>%1</b><br>(%.2f; %.2f)").arg(name).arg(point.x()).arg(point.y());

    if (nearest && m_images.contains(name)) {
      QByteArray bArray;
      QBuffer buffer(&bArray);
      if (buffer.open(QIODevice::WriteOnly)) {
        QPixmap pm = m_images[name];
        if (pm.width() > 200) {
          pm = pm.scaledToWidth(200, Qt::SmoothTransformation);
        }
        pm.save(&buffer, "PNG");
        QString imgBase64 = QString::fromLatin1(bArray.toBase64().data());
        text =
            QString("<img src='data:image/png;base64,%1'><br>").arg(imgBase64) +
            text;
      }
    }

    m_valueLabel->setText(text);
    m_valueLabel->adjustSize();
    QPoint curPos = mapFromGlobal(QCursor::pos());
    m_valueLabel->move(curPos.x() - m_valueLabel->width() / 2,
                       curPos.y() - m_valueLabel->height() - 10);
    m_valueLabel->show();
  } else {
    m_valueLabel->hide();
  }
}

void ChartQt::slotBarHovered(bool status, int index, QBarSet *barset) {
  if (status) {
    QBarCategoryAxis *axisX =
        qobject_cast<QBarCategoryAxis *>(chart()->axes(Qt::Horizontal).at(0));
    if (!axisX || index >= axisX->categories().size())
      return;

    QString name = axisX->categories().at(index);
    QString text =
        QString("<b>%1</b><br>Value: %2").arg(name).arg(barset->at(index));

    if (m_images.contains(name)) {
      QByteArray bArray;
      QBuffer buffer(&bArray);
      if (buffer.open(QIODevice::WriteOnly)) {
        QPixmap pm = m_images[name];
        if (pm.width() > 200) {
          pm = pm.scaledToWidth(200, Qt::SmoothTransformation);
        }
        pm.save(&buffer, "PNG");
        QString imgBase64 = QString::fromLatin1(bArray.toBase64().data());
        text =
            QString("<img src='data:image/png;base64,%1'><br>").arg(imgBase64) +
            text;
      }
    }

    m_valueLabel->setText(text);
    m_valueLabel->adjustSize();
    QPoint curPos = mapFromGlobal(QCursor::pos());
    m_valueLabel->move(curPos.x() - m_valueLabel->width() / 2,
                       curPos.y() - m_valueLabel->height() - 10);
    m_valueLabel->show();
  } else {
    m_valueLabel->hide();
  }
}

void ChartQt::slotPointClicked(const QPointF &point) { qDebug() << point; }

bool operator<(const QColor &a, const QColor &b) {
  return a.redF() < b.redF() || a.greenF() < b.greenF() ||
         a.blueF() < b.blueF() || a.alphaF() < b.alphaF();
}

QMap<MarkerType, QMap<QColor, QMap<bool, QList<int>>>>
ChartQt::getColors_Shapes_Selected() {
  /*
   * Dictionary:
   *    MarkerType->Color Type->Selected_or_not->point id.
   */

  QMap<MarkerType, QMap<QColor, QMap<bool, QList<int>>>> dict;

  for (int i = 0; i < p.size(); i++) {
    QColor c = p[i]->getColor();
    bool selected = p[i]->isSelected();
    if (dict.keys().contains(p[i]->marker()) == true) { // if contain marker
      if (dict[p[i]->marker()].keys().contains(c) == true) { // if contain color
        if (dict[p[i]->marker()][c].contains(selected) ==
            true) { // if contain selected or not
          dict[p[i]->marker()][c][selected] << i;
        } else { // not contain selected or not
          dict[p[i]->marker()][c].insert(selected, QList<int>());
          dict[p[i]->marker()][c][selected] << i;
        }
      } else { // not contain color and then neither selecter or not
        dict[p[i]->marker()].insert(c, QMap<bool, QList<int>>());
        dict[p[i]->marker()][c].insert(selected, QList<int>());
        dict[p[i]->marker()][c][selected] << i;
      }
    } else { // Not contain marker neither color neither selected or not
      dict.insert(p[i]->marker(), QMap<QColor, QMap<bool, QList<int>>>());
      dict[p[i]->marker()].insert(c, QMap<bool, QList<int>>());
      dict[p[i]->marker()][c].insert(selected, QList<int>());
      dict[p[i]->marker()][c][selected] << i;
    }
  }
  return dict;
}

void ChartQt::drawScatters() {
  // #ifdef DEBUG
  printf("ChartQt::drawScatters\n");
  // #endif

  for (int i = 0; i < p.size(); i++) {
    QXYSeries *series = 0;
    series = new QScatterSeries;
    QScatterSeries *scatter = static_cast<QScatterSeries *>(series);

    if (p[i]->marker() == CIRCLE) {
      scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    } else if (p[i]->marker() == SQUARE) {
      scatter->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    } else { // TRIANGLE
      scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    }

    QColor c = p[i]->getColor();
    if (!p[i]->isSelected()) c.setAlpha(127);
    scatter->setColor(c);

    if (p[i]->isSelected()) {
        scatter->setPen(QPen(Qt::red, 2.0));
    } else {
        scatter->setPen(QPen(c.darker(150), 0.5));
    }

    scatter->setMarkerSize(p[i]->radius());
    scatter->append(p[i]->x(), p[i]->y());

    // scatter->setUseOpenGL(true);
    chart()->addSeries(scatter);
    connect(scatter, &QScatterSeries::hovered, this, &ChartQt::slotPointHoverd);
    seriesList.append(series);
  }
}

void ChartQt::updateScatters() {
  // #ifdef DEBUG
  printf("ChartQt::updateScatters\n");
  // #endif

  for (int i = 0; i < p.size(); i++) {
    QScatterSeries *scatter = static_cast<QScatterSeries *>(seriesList[i]);
    if (p[i]->marker() == CIRCLE) {
      scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    } else if (p[i]->marker() == SQUARE) {
      scatter->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    } else { // TRIANGLE
      scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    }

    QColor c = p[i]->getColor();
    if (!p[i]->isSelected()) c.setAlpha(127);
    scatter->setColor(c);
    
    if (p[i]->isSelected()) {
        scatter->setPen(QPen(Qt::red, 2.0));
    } else {
        scatter->setPen(QPen(c.darker(150), 0.5));
    }

    scatter->setMarkerSize(p[i]->radius());
    scatter->replace(0, p[i]->x(), p[i]->y());
  }
}

void ChartQt::drawBars() {
  //#ifdef DEBUG
  printf("ChartQt::drawBars\n");
  //#endif

  const auto allSeries = chart()->series();
  for (auto s : allSeries) {
    if (dynamic_cast<QBarSeries *>(s)) {
      chart()->removeSeries(s);
      delete s;
    }
  }

  barsList.clear();

  QStringList categories;
  QBarSeries *series = new QBarSeries();
  series->setLabelsVisible(true);
  series->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
  series->setLabelsAngle(-90);

  for (int i = 0; i < b.size(); i++) {
    QBarSet *set = new QBarSet("");
    for (int j = 0; j < b[i]->x().size(); j++) {
      categories << b[i]->x()[j];
      *set << b[i]->y()[j];
      //                set->color(b[i]->color());
    }
    // set->color(b[i]->color());
    if (b[i]->isSelected()) {
        set->setPen(QPen(Qt::red, 2.0));
    } else {
        set->setPen(QPen(Qt::transparent, 0));
    }
    series->append(set);
    barsList.append(set);
    // void addBars(QStringList x, QVector<qreal> y, QStringList text, QColor
    // color);
  }
  chart()->addSeries(series);
  connect(series, &QBarSeries::hovered, this, &ChartQt::slotBarHovered);

  if (plot_ready == false) {
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsAngle(-90);
    chart()->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    // axisY->setRange(0,15);
    chart()->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
  }
}

void ChartQt::updateBars() {
  //#ifdef DEBUG
  printf("ChartQt::updateBars\n");
  //#endif
  for (int i = 0; i < b.size(); i++) {
    for (int j = 0; j < b[i]->x().size(); j++) {
      barsList[i]->replace(j, b[i]->y()[j]);
    }
    if (b[i]->isSelected()) {
        barsList[i]->setPen(QPen(Qt::red, 2.0));
    } else {
        barsList[i]->setPen(QPen(Qt::transparent, 0));
    }
     // barsList[i]->color(b[i]->color());
  }
  chart()->update();
}

void ChartQt::refreshPlot() {
  // #ifdef DEBUG
  qDebug() << "ChartQt::refreshPlot()";
  // #endif
  if (curveMap.size() > 0 || p.size() > 0) {
    if (curveMap.size() > 0 && curvesList.size() == curveMap.size())
      updateCurves();
    else {
      drawCurves();
    }

    if (p.size() > 0 && seriesList.size() == p.size()) {
      updateScatters();
    } else {
      drawScatters();
    }

    chart()->setTitle(m_plottitle);
    chart()->axes()[0]->setTitleText(m_xaxisname);
    chart()->axes()[1]->setTitleText(m_yaxisname);
  }

  if (b.size() > 0 && barsList.size() == b.size()) {
    updateBars();
    chart()->setTitle(m_plottitle);
    chart()->legend()->setAlignment(Qt::AlignLeft);
  } else {
    drawBars();
  }
}

void ChartQt::Plot() {
  // #ifdef DEBUG
  qDebug() << "ChartQt::Plot";
  // #endif
  if (curveMap.size() > 0 || p.size() > 0) {
    if (curveMap.size() > 0)
      drawCurves();

    if (p.size() > 0) {
      drawScatters();
    }

    chart()->setTitle(m_plottitle);
    QFont titleFont = chart()->titleFont();
    titleFont.setBold(true);
    chart()->setTitleFont(titleFont);

    chart()->createDefaultAxes();
    chart()->setDropShadowEnabled(false);
    chart()->legend()->hide();
    chart()->axes()[0]->setTitleText(m_xaxisname);
    chart()->axes()[1]->setTitleText(m_yaxisname);
  } else {
    if (b.size() > 0)
      drawBars();

    chart()->setTitle(m_plottitle);
    chart()->legend()->setVisible(false);
    chart()->legend()->hide();
    chart()->legend()->setAlignment(Qt::AlignLeft);
  }

  plot_ready = true;
}

void ChartQt::setAntialiasing(bool antialiasing_) {
  antialiasing = antialiasing_;
  setRenderHint(QPainter::Antialiasing, antialiasing);
}

void ChartQt::setXaxisName(QString xaxisname) { m_xaxisname = xaxisname; }

void ChartQt::setYaxisName(QString yaxisname) { m_yaxisname = yaxisname; }

void ChartQt::setPlotTitle(QString plottitle) {
#ifdef DEBUG
  printf("ChartQt::setPlotTitle\n");
#endif
  m_plottitle = plottitle;
}

void ChartQt::setPlotTitleSize(int size) {
#ifdef DEBUG
  printf("ChartQt::setPlotTitleSize\n");
#endif
  titleSize = size;
}

void ChartQt::setLabelDetail(bool labeldetail_) {
#ifdef DEBUG
  printf("ChartQt::setLabelDetail\n");
#endif
  labeldetail = labeldetail_;
}

void ChartQt::setAxisValueSize(int size) { axisValueSize = size; }

void ChartQt::setXLabelSize(int size) { xLabelSize = size; }

void ChartQt::setXminXmaxXTick(double xmin, double xmax, int xtick) {
  /*
  chart()->axes()[0]->setMin(xmin);
  chart()->axes()[0]->setMin(xmax);
  chart()->axes()[0]->setTickCount(xtick);
  */
}

void ChartQt::getXminXmaxXTick(double *xmin, double *xmax, int *xtick) {
  /*
  (*xmin) = chart()->axes()[0].min();
  (*xmax) = chart()->axes()[0].max();
  (*xtick) = chart()->axes()[0].tickCount();
  */
}

void ChartQt::setYLabelSize(int size) { yLabelSize = size; }

void ChartQt::setYminYmaxYTick(double ymin, double ymax, int ytick) {
  /*
      chart()->axes()[1].setMin(xmin);
      chart()->axes()[1].setMin(xmax);
      chart()->axes()[1].setTickCount(xtick);
      */
}

void ChartQt::getYminYmaxYTick(double *ymin, double *ymax, int *ytick) {
  /*
  (*ymin) = chart()->axes()[1].min();
  (*ymax) = chart()->axes()[1].max();
  (*ytick) = chart()->axes()[1].tickCount();
  */
}

void ChartQt::Center() {}

void ChartQt::Refresh() { refreshPlot(); }

void ChartQt::DoSelection(int low, int high) {
  // #ifdef DEBUG
  printf("ChartQt::DoSelection\n");
  // #endif
  int i;
  for (i = low; i < high; i++) {
    p[i]->setSelection(true);
  }
}

void ChartQt::Select(int from, int to) {
  // #ifdef DEBUG
  printf("ChartQt::Select\n");
  // #endif
  if (from < to) {
    int mid = (from + to) / 2;
    Select(from, mid);
    Select(mid + 1, to);
    DoSelection(from, to);
  }
}

void ChartQt::SelectAll() {
  // #ifdef DEBUG
  printf("ChartQt::SelectAll\n");
  // #endif
  Select(0, p.size());
  refreshPlot();
}

void ChartQt::DoUnselection(int low, int high) {
#ifdef DEBUG
  printf("ChartQt::DoUnselection\n");
#endif
  int i;
  for (i = low; i < high; i++) {
    if (p[i]->isSelected() == true) {
      p[i]->setSelection(false);
    }
  }
}

void ChartQt::Unselect(int from, int to) {
#ifdef DEBUG
  printf("ChartQt::Unselect\n");
#endif
  if (from < to) {
    int mid = (from + to) / 2;
    Unselect(from, mid);
    Unselect(mid + 1, to);
    DoUnselection(from, to);
  }
}

void ChartQt::ClearSelection() {
  // #ifdef DEBUG
  printf("ChartQt::ClearSelection\n");
  // #endif
  /* Unselection by divide and conqueror technique*/
  Unselect(0, p.size());
  refreshPlot();
}

// void ChartQt::setCurveData(int id, const QVector<QPointF> &data){}
void ChartQt::addPoint(qreal x, qreal y, QString name) {
#ifdef DEBUG
  printf("ChartQt::addPoint(qreal x, qreal y, QString name)\n");
#endif
  p.append(new DataPoint(x, y, name));
}

void ChartQt::addPoint(qreal x, qreal y, QString name, QColor color,
                       int radius) {
#ifdef DEBUG
  printf("ChartQt::addPoint(qreal x, qreal y, QString name, QColor color, int "
         "radius)\n");
#endif
  p.append(new DataPoint(x, y, name));
  p.last()->setColor(color);
  p.last()->setRadius(radius);
}

void ChartQt::addCurve(QVector<QPointF> curve, QString name, QColor color, bool smooth) {
#ifdef DEBUG
  printf("ChartQt::addCurve\n");
#endif
  DataCurve dc(curve, name, color);
  dc.setSmooth(smooth);
  curveMap.append(dc);
}

void ChartQt::setCurveStyle(int indx, LTYPE cs) {
  if (indx < curveMap.size()) {
    curveMap[indx].setStyle(cs);
  }
}

void ChartQt::addBars(QStringList x, QVector<qreal> y, QStringList text,
                      QColor color) {
#ifdef DEBUG
  printf("ChartQt::addBars\n");
#endif
  b.append(new DataBar(x, y, text));
  b.last()->setColor(color);
}

void ChartQt::updateBarsData(int indx, QVector<qreal> y, QColor color) {
#ifdef DEBUG
  printf("ChartQt::updateBars\n");
#endif
  if(indx < b.size()){
    b[indx]->setY(y);
    b[indx]->setColor(color);
    updateBars();
    refreshPlot();
  }
}


int ChartQt::PointSize() const { return p.size(); }

DataPoint *ChartQt::getPoint(int id) {
  if (id < p.size())
    return p[id];
  else
    return 0;
}

void ChartQt::RemovePointAt(int id) {}
void ChartQt::RemovePoint(DataPoint *dp) {}
void ChartQt::RemoveAllPoints() {}

//     QVector<DataPoint*> getPoints(){}
QVector<DataCurve> ChartQt::getCurves() { return curveMap; }

void ChartQt::RemoveCurveAt(int cid) {
  // #ifdef DEBUG
  printf("Chart::RemoveCurveAt\n");
  // #endif
  curveMap.remove(cid);
  refreshPlot();
}

void ChartQt::RemoveAllCurves() {
  // #ifdef DEBUG
  printf("ChartQt::RemoveAllCurves\n");
  // #endif
  curveMap.clear();
  refreshPlot();
}

// void ChartQt::clearCurve(int id){}

void ChartQt::SaveAsImage(QString imgname) {}

QWidget *ChartQt::weview() { return this; }

void ChartQt::zoomIn() { chart()->zoomIn(); }

void ChartQt::zoomOut() { chart()->zoomOut(); }

void ChartQt::recentre() { chart()->zoomReset(); }

ChartQt::ChartQt(QWidget *parent)
    : QChartView(new QChart(), parent), m_isTouching(false) {
  // setRubberBand(QChartView::RectangleRubberBand); Disable default zoom in/out
  setRubberBand(QChartView::NoRubberBand);
  setRenderHint(QPainter::Antialiasing);

  zoomInButton = new QToolButton(this);
  zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
  zoomInButton->adjustSize();
  connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));

  zoomOutButton = new QToolButton(this);
  zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
  zoomOutButton->adjustSize();
  connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));

  recentreButton = new QToolButton(this);
  recentreButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
  recentreButton->setToolTip(tr("Recentre Plot"));
  recentreButton->adjustSize();
  connect(recentreButton, SIGNAL(clicked()), this, SLOT(recentre()));

  m_valueLabel = new QLabel(this);
  m_valueLabel->setStyleSheet(
      QString("QLabel{color:#1564FF; font-family:\"Microsoft Yahei\"; "
              "font-size:12px; font-weight:bold;"
              " background-color:rgba(21, 100, 255, 51); border-radius:4px; "
              "text-align:center;}"));
  m_valueLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_valueLabel->hide();

  plot_ready = false;
  qDebug() << "ChartQt::ChartQt";
}

ChartQt::~ChartQt() {
  for (int i = 0; i < p.size(); i++)
    delete p[i];
  p.clear();
}

void ChartQt::setImages(const QMap<QString, QPixmap> &images) {
  m_images = images;
}

void ChartQt::LoadSettings() {
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
