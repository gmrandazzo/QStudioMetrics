#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <QtGlobal>
#include <QColor>
#include <QList>
#include <QMetaType>
#include <cmath>

#include <QDebug>

#define QEPSILON 1e-4  /* Define your own tolerance*/
#define FLOAT_EQ(x,v, EPSILON) (((v - EPSILON) < x) && (x <( v + EPSILON)))

class DataPoint
{
public:
  DataPoint();
  DataPoint(qreal x, qreal y, QString name);
  DataPoint(qreal x, qreal y, qreal z, QString name);

  void setX(qreal x);
  void setY(qreal y);
  void setZ(qreal z);
  void setName(QString name);
  QString &getName();
  void setColor(const QColor &color);
  QColor &getColor();
  void setWidth(int width);
  void setRadius(int radius);
  void setSelection(bool selection);
  void setVisible(bool visible);
  void setLabelVisible(bool lvisible);

  qreal x() const;
  qreal y() const;
  qreal z() const;
  QString name() const;
  bool isSelected() const;
  bool isVisible() const;
  bool isLabelVisible() const;

  QColor color() const;
  int radius() const;
  int width() const;

  bool compare(DataPoint& p)
  {
    if(FLOAT_EQ(this->x(), p.x(), QEPSILON) &&
       FLOAT_EQ(this->y(), p.y(), QEPSILON) &&
       FLOAT_EQ(this->z(), p.z(), QEPSILON)){
      if(this->getName().compare(p.getName()) == 0){
        return true;
      }
      else{
        return false;
      }
    }
    else{
      return false;
    }
  }

private:
  qreal m_x;
  qreal m_y;
  qreal m_z;
  QString m_name;
  QColor m_color;
  int m_width;
  int m_radius;
  bool m_selection;
  bool m_visible, m_lvisible;
};

#endif // DATAPOINT_H
