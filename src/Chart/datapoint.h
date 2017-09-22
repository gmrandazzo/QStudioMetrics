#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <QtGlobal>
#include <QColor>
#include <QList>
#include <QMetaType>

class DataPoint
{
public:
  DataPoint();
  DataPoint(qreal x, qreal y, QString name);

  void setX(qreal x);
  void setY(qreal y);
  void setName(QString name);
  QString &getName();
  void setColor(const QColor &color);
  QColor &getColor();
  void setWidth(int width);
  void setRadius(int radius);
  void setSelection(bool selection);
  void setVisible(bool visible);

  qreal x() const;
  qreal y() const;
  QString name() const;
  bool isSelected() const;
  bool isVisible() const;

  QColor color() const;
  int radius() const;
  int width() const;

private:
  qreal m_x;
  qreal m_y;
  QString m_name;
  QColor m_color;
  int m_width;
  int m_radius;
  bool m_selection;
  bool m_visible;
};

#endif // DATAPOINT_H
