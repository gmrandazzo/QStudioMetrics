#include "databar.h"
#include <QVector>
#include <QStringList>

DataBar::DataBar() : m_x(QStringList()), m_y(QVector<qreal>()), m_text(QStringList()), m_color(Qt::black), m_width(10), m_selection(false), m_visible(true), m_tvisible(false)
{
}

DataBar::DataBar(QStringList x, QVector<qreal> y, QStringList text) : m_x(x), m_y(y), m_text(text), m_color(Qt::black),  m_width(10), m_selection(false), m_visible(true), m_tvisible(false)
{
}


void DataBar::setX(QStringList x)
{
  m_x = x;
}

void DataBar::setY(QVector<qreal> y)
{
  m_y = y;
}

void DataBar::setText(QStringList text)
{
  m_text = text;
}

QStringList &DataBar::getText()
{
  return m_text;
}


void DataBar::setColor(const QColor &color)
{
  m_color = color;
}

QColor &DataBar::getColor()
{
  return m_color;
}

void DataBar::setWidth(int width)
{
  m_width = width;
}


void DataBar::setSelection(bool selection)
{
  m_selection = selection;
}

void DataBar::setVisible(bool visible)
{
  m_visible = visible;
}

void DataBar::setTextVisible(bool tvisible)
{
  m_tvisible = tvisible;
}

QStringList DataBar::x() const
{
  return m_x;
}

QVector<qreal> DataBar::y() const
{
  return m_y;
}

QStringList DataBar::text() const
{
  return m_text;
}

bool DataBar::isSelected() const
{
  return m_selection;
}

bool DataBar::isVisible() const
{
  return m_visible;
}

bool DataBar::isTextVisible() const
{
  return m_tvisible;
}

QColor DataBar::color() const
{
  return m_color;
}

int DataBar::width() const
{
  return m_width;
}
