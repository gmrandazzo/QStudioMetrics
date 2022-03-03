#ifndef DATACURVE_H
#define DATACURVE_H

#include <QtGlobal>
#include <QColor>
#include <QVector>
#include <QPointF>
#include <QList>
#include <QMetaType>
#include <QString>

enum LTYPE
{
   L, //Lines
   LM//Lines and markers
};

class DataCurve
{
public:
  DataCurve();
  DataCurve(QVector< QPointF > curve, QString name, QColor color);

  void addPoint(qreal x, qreal y);
  void setName(QString name);
  void setColor(const QColor &color);
  void setRadius(int radius);
  void setWidth(int width);
  void setVisible(bool visible);
  void setStyle(LTYPE lt_);

  QVector <QPointF> getPoints() const;
  QString name() const;
  QColor color() const;
  LTYPE getStyle() const;
  int radius() const;
  int width() const;
  bool isVisible() const;

private:
  QVector < QPointF> m_curve;
  QString m_name;
  QColor m_color;
  int m_radius;
  int m_width;
  bool m_visible;
  LTYPE lt;
};

#endif // DATACURVE_H
