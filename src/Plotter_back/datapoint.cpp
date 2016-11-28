#include "datapoint.h"
  
DataPoint::DataPoint() : m_x(0), m_y(0), m_name("No name"), m_color(Qt::black), m_width(10), m_radius(5), m_selection(false), m_visible(true)
{
}
  
DataPoint::DataPoint(qreal x, qreal y, QString name) : m_x(x), m_y(y), m_name(name), m_color(Qt::black),  m_width(10), m_radius(10), m_selection(false), m_visible(true)
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

void DataPoint::setName(QString name)
{
  name = m_name;
}

void DataPoint::setColor(const QColor &color)
{
  m_color = color;
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


qreal DataPoint::x() const
{
  return m_x;
}
  
qreal DataPoint::y() const
{
  return m_y;
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

