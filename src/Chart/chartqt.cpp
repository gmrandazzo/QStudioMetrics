#include "chartqt.h"
#include "datapoint.h"
#include "databar.h"
#include <QtCore/QEvent>
#include <QMap>
#include <QLabel>
#include <QToolButton>
#include <QtWidgets/QGraphicsTextItem>
#include <QtCharts/QScatterSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QLegendMarker>
#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtCore/QtMath>
#include <QtGui/QMouseEvent>

bool ChartQt::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin){
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

void ChartQt::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "mousePressEvent " << event;
    if (m_isTouching)
        return;

    auto const widgetPos = event->position();
    auto const scenePos = mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
    auto const chartItemPos = chart()->mapFromScene(scenePos);
    mPresscc = chart()->mapToValue(chartItemPos);

    setDragMode(QGraphicsView::RubberBandDrag);
    QChartView::mousePressEvent(event);
}

void ChartQt::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isTouching)
        return;
    QChartView::mouseMoveEvent(event);
}

void ChartQt::showLabels()
{
    for(int i = 0; i < plotLabels.size(); i++)
        delete plotLabels[i];
    plotLabels.clear();

    for(int i = 0; i < p.size(); i++){
        if(p[i]->isSelected()){
            plotLabels.append(new QLabel(this));
            plotLabels.last()->setStyleSheet(QString("QLabel{color:#1564FF; font-family:\"SansSerif\"; font-size:12px; font-weight:normal;"
                                                    " background-color:rgba(21, 100, 255, 51); border-radius:4px; text-align:center;}"));
            plotLabels.last()->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            plotLabels.last()->setText(QString("%1 (%2 ; %3)").arg(p[i]->name()).arg(p[i]->x()).arg(p[i]->y()));
            QPointF pos = chart()->mapToPosition(QPointF(p[i]->x(), p[i]->y()));
            plotLabels.last()->move(pos.x() - plotLabels.last()->width() / 2, pos.y() - plotLabels.last()->height() * 1.5);
            plotLabels.last()->show();
        }
        else{
            continue;
        }
    }
}

void ChartQt::mouseReleaseEvent(QMouseEvent *event)
{
    qDebug() << "mouseReleaseEvent " << event;
    if (m_isTouching)
        m_isTouching = false;

    auto const widgetPos = event->position();
    auto const scenePos = mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
    auto const chartItemPos = chart()->mapFromScene(scenePos);
    mReleasecc = chart()->mapToValue(chartItemPos);
   /* auto const valueGivenSeries = chart()->mapToValue(chartItemPos);
    maxX = valueGivenSeries.x();
    maxY = valueGivenSeries.y();*/
    qreal minX, maxX, minY, maxY;

    if(mPresscc.x() > mReleasecc.x()){
        minX = mReleasecc.x();
        maxX = mPresscc.x();
    }
    else{
        minX = mPresscc.x();
        maxX = mReleasecc.x();
    }

    if(mPresscc.y() > mReleasecc.y()){
        minY = mReleasecc.y();
        maxY = mPresscc.y();
    }
    else{
        minY = mPresscc.y();
        maxY = mReleasecc.y();
    }

    if(event->button() == Qt::MiddleButton){

        chart()->zoomIn();
        //zoom and store the zoom domain region
        /*
        zoom_region = this->sceneRect();
        or ..
        zoom_region.setX(minX);
        zoom_region.setY(maxY);
        zoom_region.setWidth(maxX-minX);
        zoom_region.setHeight(maxY-minY);

        qDebug() << zoom_region;
        */
    }
    else if(event->button() == Qt::LeftButton){
        //select
        /*qDebug() << "Select Objects..";
        qDebug() << minX << maxX << minY << maxY;*/
        for(int i = 0; i < p.size(); i++){
            if(p[i]->x() > minX && p[i]->x() < maxX && p[i]->y() > minY && p[i]->y() < maxY){
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
        refreshPlot();
    }
    else{
      chart()->zoomOut();
    }

    // Because we disabled animations when touch event was detected
    // we must put them back on.
    chart()->setAnimationOptions(QChart::SeriesAnimations);
    QChartView::mouseReleaseEvent(event);
}

void ChartQt::wheelEvent(QWheelEvent *event)
{
    /*
    QPoint numDegrees = event->angleDelta() / 8;
    qreal rVal = std::pow(0.999, numDegrees.x());
    QRectF oPlotAreaRect = chart()->plotArea();

    oPlotAreaRect.setWidth(oPlotAreaRect.width() * rVal);
    oPlotAreaRect.setHeight(oPlotAreaRect.height() * rVal);

    auto const widgetPos = event->globalPosition();
    auto const scenePos = mapToScene(QPoint(static_cast<int>(widgetPos.x()), static_cast<int>(widgetPos.y())));
    auto const chartItemPos = chart()->mapFromScene(scenePos);
    auto const valueGivenSeries = chart()->mapToValue(chartItemPos);
    qDebug() << valueGivenSeries;
    oPlotAreaRect.moveCenter(valueGivenSeries);
    chart()->zoomIn(oPlotAreaRect);
    QChartView::wheelEvent(event);
    */
    QChartView::wheelEvent(event);
}

void ChartQt::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key " << event->key();
    switch (event->key()){
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

void ChartQt::drawCurves()
{
    //#ifdef DEBUG
    printf("Chart::drawCurves\n");
    //#endif
    for(int i = 0; i < curveMap.size(); i++){
        const DataCurve data = curveMap[i];
        if(data.getPoints().size() > 0){
            QXYSeries *series = 0;
            series = new QLineSeries;
            QLineSeries *line = static_cast<QLineSeries *>(series);

            //QLineSeries *lseries = new QLineSeries();
            for(int j = 0; j < data.getPoints().size(); ++j){
                line->append(data.getPoints()[j].x(), data.getPoints()[j].y());
            }
            line->setColor(data.color());
            //line->setUseOpenGL(true);
            chart()->addSeries(line);
            curvesList.append(series);
        }
        else{
            continue;
        }
    }
}

void ChartQt::updateCurves()
{
    //#ifdef DEBUG
    printf("Chart::updateCurves\n");
    //#endif
    for(int i = 0; i < curveMap.size(); i++){
        const DataCurve data = curveMap[i];
        QLineSeries *line = static_cast<QLineSeries *>(curvesList[i]);

        if(data.isVisible() == true){
            if(data.getPoints().size() > 0){
                //QLineSeries *lseries = new QLineSeries();
                for(int j = 0; j < data.getPoints().size(); ++j){
                    line->replace(j, data.getPoints()[j].x(), data.getPoints()[j].y());
                }
                line->setColor(data.color());
            }
            else{
                continue;
            }

        }
        else{
            line->hide();
        }
    }

}

void ChartQt::slotPointHoverd(const QPointF &point, bool state)
{
    if (state) {
        m_valueLabel->setText(QString::asprintf("%s - (%.2f; %.2f)", "pippone", point.x(), point.y()));
        QPoint curPos = mapFromGlobal(QCursor::pos());
        m_valueLabel->move(curPos.x() - m_valueLabel->width() / 2, curPos.y() - m_valueLabel->height() * 1.5);
        m_valueLabel->show();

        /*QScatterSeries *series1 = (QScatterSeries *)chart()->series().at(1);
        series1->clear();
        series1->append(point);
        series1->setVisible(true);*/
    }
    else {
        m_valueLabel->hide();
        /*QScatterSeries *series1 = (QScatterSeries *)chart()->series().at(1);
        series1->setVisible(false);*/
    }
}

void ChartQt::slotPointClicked(const QPointF &point)
{
    qDebug() << point;
}

bool operator<(const QColor & a, const QColor & b) {
   return a.redF() < b.redF()
       || a.greenF() < b.greenF()
       || a.blueF() < b.blueF()
       || a.alphaF() < b.alphaF();
}

QMap<MarkerType, QMap<QColor, QMap<bool, QList<int>>>>  ChartQt::getColors_Shapes_Selected()
{
    /*
     * Dictionary:
     *    MarkerType->Color Type->Selected_or_not->point id.
     */

    QMap<MarkerType, QMap<QColor, QMap<bool, QList<int>>>> dict;

    for(int i = 0; i < p.size(); i++){
        QColor c = p[i]->getColor();
        bool selected = p[i]->isSelected();
        if(dict.keys().contains(p[i]->marker()) == true){ // if contain marker
            if(dict[p[i]->marker()].keys().contains(c) == true){ // if contain color
                if(dict[p[i]->marker()][c].contains(selected) == true){ // if contain selected or not
                    dict[p[i]->marker()][c][selected] << i;
                }
                else{ // not contain selected or not
                    dict[p[i]->marker()][c].insert(selected, QList<int>());
                    dict[p[i]->marker()][c][selected] << i;
                }
            }
            else{ // not contain color and then neither selecter or not
                dict[p[i]->marker()].insert(c, QMap<bool, QList<int>>());
                dict[p[i]->marker()][c].insert(selected, QList<int>());
                dict[p[i]->marker()][c][selected] << i;
            }
        }
        else{ // Not contain marker neither color neither selected or not
            dict.insert(p[i]->marker(), QMap<QColor, QMap<bool, QList<int>>>());
            dict[p[i]->marker()].insert(c, QMap<bool, QList<int>>());
            dict[p[i]->marker()][c].insert(selected, QList<int>());
            dict[p[i]->marker()][c][selected] << i;
        }
    }
    return dict;
}

void ChartQt::drawScatters()
{
  //#ifdef DEBUG
  printf("ChartQt::drawScatters\n");
  //#endif

    for(int i = 0; i < p.size(); i++){
        QXYSeries *series = 0;
        series = new QScatterSeries;
        QScatterSeries *scatter = static_cast<QScatterSeries *>(series);

        if(p[i]->marker() == CIRCLE){
            scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        }
        else if(p[i]->marker() == SQUARE){
            scatter->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
        }
        else{ //TRIANGLE
            scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        }


        QColor c = p[i]->getColor();
        c.setAlpha(127);
        series->setColor(c);

        scatter->setMarkerSize(p[i]->radius());
        scatter->append(p[i]->x(), p[i]->y());

        //scatter->setUseOpenGL(true);
        chart()->addSeries(scatter);
        connect(scatter, &QScatterSeries::hovered, this, &ChartQt::slotPointHoverd);
        seriesList.append(series);
    }
}


void ChartQt::updateScatters()
{
  //#ifdef DEBUG
  printf("ChartQt::updateScatters\n");
  //#endif

    for(int i = 0; i < p.size(); i++){
        QScatterSeries *scatter = static_cast<QScatterSeries *>(seriesList[i]);
        if(p[i]->marker() == CIRCLE){
            scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        }
        else if(p[i]->marker() == SQUARE){
            scatter->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
        }
        else{ //TRIANGLE
            scatter->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        }

        QColor c = p[i]->getColor();
        c.setAlpha(127);
        scatter->setColor(c);
        scatter->setMarkerSize(p[i]->radius());
        scatter->replace(0, p[i]->x(), p[i]->y());
    }
}

void ChartQt::drawBars()
{
    //#ifdef DEBUG
    printf("ChartQt::drawBars\n");
    //#endif

    QStringList categories;
    QBarSeries *series = new QBarSeries();

    for(int i = 0; i < b.size(); i++){
        QBarSet *set = new QBarSet("");
        for(int j = 0; j < b[i]->x().size(); j++){
            categories << b[i]->x()[j];
            *set << b[i]->y()[j];
    //                set->color(b[i]->color());
        }
        //set->color(b[i]->color());
        series->append(set);
        barsList.append(set);
        //void addBars(QStringList x, QVector<qreal> y, QStringList text, QColor color);
    }
    chart()->addSeries(series);

    if(plot_ready == false){
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        chart()->addAxis(axisX, Qt::AlignBottom);
        series->attachAxis(axisX);

        QValueAxis *axisY = new QValueAxis();
        //axisY->setRange(0,15);
        chart()->addAxis(axisY, Qt::AlignLeft);
        series->attachAxis(axisY);
    }
}

void ChartQt::updateBars(){
    //#ifdef DEBUG
    printf("ChartQt::updateBars\n");
    //#endif
    printf("%lld %lld\n", b.size(), barsList.size());
    for(int i = 0; i < b.size(); i++){
        QBarSet *set = barsList[i];
        if(b[i]->isVisible() == true){
            for(int j = 0; j < b[i]->x().size(); j++){
                set->insert(j, b[i]->y()[j]);
        //                set->color(b[i]->color());
            }
            //barsList.append(set);
            //void addBars(QStringList x, QVector<qreal> y, QStringList text, QColor color);
        }
        else{
            // HIDE
            continue;
            //barsList[i]->hide();
        }
    }

}

void ChartQt::refreshPlot()
{
    //#ifdef DEBUG
    qDebug() << "ChartQt::refreshPlot()";
    //#endif
    if(curveMap.size() > 0 || p.size() > 0){
        if(curveMap.size() > 0 && curvesList.size() == curveMap.size())
            updateCurves();
        else{
            drawCurves();
        }

        if(p.size() > 0 && seriesList.size() == p.size()){
            updateScatters();
        }
        else{
            drawScatters();
        }

        chart()->setTitle(m_plottitle);
        chart()->axes()[0]->setTitleText(m_xaxisname);
        chart()->axes()[1]->setTitleText(m_yaxisname);
    }

    if(b.size() > 0 && barsList.size() == b.size()){
        updateBars();
        chart()->setTitle(m_plottitle);
        chart()->legend()->setAlignment(Qt::AlignLeft);
    }
    else{
        drawBars();
    }
}

void ChartQt::Plot(){
    //#ifdef DEBUG
    qDebug() << "ChartQt::Plot";
    //#endif
    if(curveMap.size() > 0 || p.size() > 0){
        if(curveMap.size() > 0)
            drawCurves();

        if(p.size() > 0){
            drawScatters();
        }

        chart()->setTitle(m_plottitle);

        chart()->createDefaultAxes();
        chart()->setDropShadowEnabled(false);
        chart()->legend()->hide();
        chart()->axes()[0]->setTitleText(m_xaxisname);
        chart()->axes()[1]->setTitleText(m_yaxisname);
    }
    else{
        if(b.size() > 0)
            drawBars();

        chart()->setTitle(m_plottitle);
        chart()->legend()->setVisible(false);
        chart()->legend()->hide();
        chart()->legend()->setAlignment(Qt::AlignLeft);
    }

    plot_ready = true;
}

void ChartQt::setAntialiasing(bool antialiasing_){}

void ChartQt::setXaxisName(QString xaxisname)
{
    m_xaxisname = xaxisname;
}

void ChartQt::setYaxisName(QString yaxisname)
{
    m_yaxisname = yaxisname;
}

void ChartQt::setPlotTitle(QString plottitle)
{
  #ifdef DEBUG
  printf("ChartQt::setPlotTitle\n");
  #endif
  m_plottitle = plottitle;
}

void ChartQt::setPlotTitleSize(int size)
{
  #ifdef DEBUG
  printf("ChartQt::setPlotTitleSize\n");
  #endif
  titleSize = size;
}

void ChartQt::setLabelDetail(bool labeldetail_)
{
  #ifdef DEBUG
  printf("ChartQt::setLabelDetail\n");
  #endif
  labeldetail = labeldetail_;
}

void ChartQt::setAxisValueSize(int size)
{
  axisValueSize = size;
}

void ChartQt::setXLabelSize(int size)
{
  xLabelSize = size;
}

void ChartQt::setXminXmaxXTick(double xmin, double xmax, int xtick)
{
    /*
    chart()->axes()[0]->setMin(xmin);
    chart()->axes()[0]->setMin(xmax);
    chart()->axes()[0]->setTickCount(xtick);
    */
}

void ChartQt::getXminXmaxXTick(double *xmin, double *xmax, int *xtick)
{
    /*
    (*xmin) = chart()->axes()[0].min();
    (*xmax) = chart()->axes()[0].max();
    (*xtick) = chart()->axes()[0].tickCount();
    */
}

void ChartQt::setYLabelSize(int size)
{
    yLabelSize = size;
}

void ChartQt::setYminYmaxYTick(double ymin, double ymax, int ytick)
{
/*
    chart()->axes()[1].setMin(xmin);
    chart()->axes()[1].setMin(xmax);
    chart()->axes()[1].setTickCount(xtick);
    */
}

void ChartQt::getYminYmaxYTick(double *ymin, double *ymax, int *ytick)
{
    /*
    (*ymin) = chart()->axes()[1].min();
    (*ymax) = chart()->axes()[1].max();
    (*ytick) = chart()->axes()[1].tickCount();
    */
}

void ChartQt::Center()
{

}

void ChartQt::Refresh()
{
    refreshPlot();
}

void ChartQt::DoSelection(int low, int high)
{
    //#ifdef DEBUG
    printf("ChartQt::DoSelection\n");
    //#endif
    int i;
    for(i = low; i < high; i++){
        p[i]->setSelection(true);
    }
}

void ChartQt::Select(int from, int to)
{
    //#ifdef DEBUG
    printf("ChartQt::Select\n");
    //#endif
    if(from < to){
        int mid = (from+to)/2;
        Select(from, mid);
        Select(mid+1, to);
        DoSelection(from, to);
    }
}

void ChartQt::SelectAll()
{
    //#ifdef DEBUG
    printf("ChartQt::SelectAll\n");
    //#endif
    Select(0, p.size());
    refreshPlot();
}

void ChartQt::DoUnselection(int low, int high)
{
  #ifdef DEBUG
  printf("ChartQt::DoUnselection\n");
  #endif
  int i;
  for(i = low; i < high; i++){
    if(p[i]->isSelected() == true){
      p[i]->setSelection(false);
    }
  }
}

void ChartQt::Unselect(int from, int to)
{
  #ifdef DEBUG
  printf("ChartQt::Unselect\n");
  #endif
  if(from < to){
    int mid = (from+to)/2;
    Unselect(from, mid);
    Unselect(mid+1, to);
    DoUnselection(from, to);
  }
}

void ChartQt::ClearSelection()
{
    //#ifdef DEBUG
    printf("ChartQt::ClearSelection\n");
    //#endif
    /* Unselection by divide and conqueror technique*/
    Unselect(0, p.size());
    refreshPlot();
}

// void ChartQt::setCurveData(int id, const QVector<QPointF> &data){}
void ChartQt::addPoint(qreal x, qreal y, QString name)
{
  #ifdef DEBUG
  printf("ChartQt::addPoint(qreal x, qreal y, QString name)\n");
  #endif
  p.append(new DataPoint(x, y, name));
}

void ChartQt::addPoint(qreal x, qreal y, QString name, QColor color, int radius)
{
  #ifdef DEBUG
  printf("ChartQt::addPoint(qreal x, qreal y, QString name, QColor color, int radius)\n");
  #endif
  p.append(new DataPoint(x, y, name));
  p.last()->setColor(color);
  p.last()->setRadius(radius);
}

void ChartQt::addCurve(QVector< QPointF > curve, QString name, QColor color)
{
  #ifdef DEBUG
  printf("ChartQt::addCurve\n");
  #endif
  curveMap.append(DataCurve(curve, name, color));
}

void ChartQt::setCurveStyle(int indx, LTYPE cs)
{
  if(indx < curveMap.size()){
    curveMap[indx].setStyle(cs);
  }
}


void ChartQt::addBars(QStringList x, QVector<qreal> y, QStringList text, QColor color)
{
  #ifdef DEBUG
  printf("ChartQt::addBars\n");
  #endif
  b.append(new DataBar(x, y, text));
  b.last()->setColor(color);
}

int ChartQt::PointSize() const
{
  return p.size();
}

DataPoint* ChartQt::getPoint(int id)
{
    if(id < p.size())
        return p[id];
    else
        return 0;
}

void ChartQt::RemovePointAt(int id){}
void ChartQt::RemovePoint(DataPoint *dp){}
void ChartQt::RemoveAllPoints(){}

//     QVector<DataPoint*> getPoints(){}
QVector<DataCurve> ChartQt::getCurves()
{
    return curveMap;
}

void ChartQt::RemoveCurveAt(int cid)
{
    //#ifdef DEBUG
    printf("Chart::RemoveCurveAt\n");
    //#endif
    curveMap.remove(cid);
    refreshPlot();
}

void ChartQt::RemoveAllCurves()
{
    //#ifdef DEBUG
    printf("ChartQt::RemoveAllCurves\n");
    //#endif
    curveMap.clear();
    refreshPlot();
}

// void ChartQt::clearCurve(int id){}

void ChartQt::SaveAsImage(QString imgname){}

QWidget *ChartQt::weview()
{
  return this;
}

void ChartQt::zoomIn()
{
    chart()->zoomIn();
}

void ChartQt::zoomOut()
{
    chart()->zoomOut();
}

ChartQt::ChartQt(QWidget *parent) : QChartView(new QChart(), parent), m_isTouching(false)
{
    //setRubberBand(QChartView::RectangleRubberBand); Disable default zoom in/out
    setRubberBand(QChartView::NoRubberBand);
    setRenderHint(QPainter::Antialiasing);
    /*
    zoomInButton = new QToolButton(this);
    zoomInButton->setIcon(QIcon(":/images/zoomin.png"));
    zoomInButton->adjustSize();
    zoomInButton->move(QPoint(5, 5));
    connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));


    zoomOutButton = new QToolButton(this);
    zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));
    zoomOutButton->adjustSize();
    zoomOutButton->move(QPoint(40, 5));
    connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));
    */

    m_valueLabel = new QLabel(this);
    m_valueLabel->setStyleSheet(QString("QLabel{color:#1564FF; font-family:\"Microsoft Yahei\"; font-size:12px; font-weight:bold;"
                                        " background-color:rgba(21, 100, 255, 51); border-radius:4px; text-align:center;}"));
    m_valueLabel->setFixedSize(44, 24);
    m_valueLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    m_valueLabel->hide();

    plot_ready = false;
    qDebug() << "ChartQt::ChartQt";
}

ChartQt::~ChartQt()
{
    //#ifdef DEBUG
    printf("ChartQt::~ChartQt\n");
    //#endif
    int i;
    for(i = 0; i < p.size(); i++)
        delete p[i];
    p.clear();

    for(i = 0; i < seriesList.size(); i++)
        delete seriesList[i];
    seriesList.clear();

    for(i = 0; i < curvesList.size(); i++)
        delete curvesList[i];
    curvesList.clear();

    for(i = 0; i < barsList.size(); i++)
        delete barsList[i];
    barsList.clear();

    for(i = 0; i < plotLabels.size(); i++)
        delete plotLabels[i];
    plotLabels.clear();


    /*delete zoomInButton;
    delete zoomOutButton;*/
    delete m_valueLabel;
}
