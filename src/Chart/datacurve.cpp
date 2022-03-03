#include "datacurve.h"
#include <QVector>
#include <QPointF>

DataCurve::DataCurve()  : m_name("No name"), m_color(Qt::black), m_radius(2), m_width(1), m_visible(true), lt(L)
{

}

DataCurve::DataCurve(QVector< QPointF > curve, QString name, QColor color): m_name(name), m_color(color), m_radius(2), m_width(1), m_visible(true), lt(L)
{
  m_curve = curve;
}

void DataCurve::addPoint(qreal x, qreal y)
{
  m_curve.append(QPointF(x,y));
}

void DataCurve::setName(QString name)
{
  m_name = name;
}

void DataCurve::setColor(const QColor& color)
{
  m_color = color;
}

void DataCurve::setRadius(int radius)
{
  m_radius = radius;
}

void DataCurve::setWidth(int width)
{
  m_width = width;
}

void DataCurve::setVisible(bool visible)
{
  m_visible = visible;
}

void DataCurve::setStyle(LTYPE lt_){
  lt = lt_;
}

QVector < QPointF > DataCurve::getPoints() const
{
  return m_curve;
}

QString DataCurve::name() const
{
  return m_name;
}

QColor DataCurve::color() const
{
  return m_color;
}

LTYPE DataCurve::getStyle() const
{
  return lt;
}

int DataCurve::radius() const
{
  return m_radius;
}

int DataCurve::width() const
{
  return m_width;
}

bool DataCurve::isVisible() const
{
  return m_visible;
}
