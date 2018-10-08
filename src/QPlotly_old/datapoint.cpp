#include "datapoint.h"

DataPoint::DataPoint() : m_x(0.f), m_y(0.f), m_z(0.f), m_name("No name"), m_color(Qt::black), m_width(10), m_radius(5), m_selection(false), m_visible(true), m_lvisible(false)
{
}

DataPoint::DataPoint(qreal x, qreal y, QString name) : m_x(x), m_y(y), m_z(0.f), m_name(name), m_color(Qt::black),  m_width(10), m_radius(10), m_selection(false), m_visible(true), m_lvisible(false)
{
}

DataPoint::DataPoint(qreal x, qreal y, qreal z, QString name) : m_x(x), m_y(y), m_z(z), m_name(name), m_color(Qt::black),  m_width(10), m_radius(10), m_selection(false), m_visible(true), m_lvisible(false)
{
}

void DataPoint::setX(qreal x)
{
  m_x = x;
}

void DataPoint::setY(qreal y)
{
  m_y = y;
}

void DataPoint::setZ(qreal z)
{
  m_z = z;
}

void DataPoint::setName(QString name)
{
  m_name = name;
}

QString &DataPoint::getName()
{
  return m_name;
}


void DataPoint::setColor(const QColor &color)
{
  m_color = color;
}

QColor &DataPoint::getColor()
{
  return m_color;
}

void DataPoint::setWidth(int width)
{
  m_width = width;
}

void DataPoint::setRadius(int radius)
{
  m_radius = radius;
}

void DataPoint::setSelection(bool selection)
{
  m_selection = selection;
}

void DataPoint::setVisible(bool visible)
{
  m_visible = visible;
}

void DataPoint::setLabelVisible(bool lvisible)
{
  m_lvisible = lvisible;
}

qreal DataPoint::x() const
{
  return m_x;
}

qreal DataPoint::y() const
{
  return m_y;
}

qreal DataPoint::z() const
{
  return m_z;
}

QString DataPoint::name() const
{
  return m_name;
}

bool DataPoint::isSelected() const
{
  return m_selection;
}

bool DataPoint::isVisible() const
{
  return m_visible;
}

bool DataPoint::isLabelVisible() const
{
  return m_lvisible;
}

QColor DataPoint::color() const
{
  return m_color;
}

int DataPoint::width() const
{
  return m_width;
}

int DataPoint::radius() const
{
  return m_radius;
}
