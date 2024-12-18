#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <QColor>
#include <QList>
#include <QMetaType>
#include <QtGlobal>
#include <cmath>

#ifdef DEBUG
#include <QDebug>
#endif

#define QEPSILON 1e-3 /* Define your own tolerance*/
#ifndef FLOAT_EQ
#define FLOAT_EQ(x, v, EPSILON) (((v - EPSILON) < x) && (x < (v + EPSILON)))
#endif

enum MarkerType { CIRCLE, SQUARE, TRIANGLE };

class DataPoint {
public:
  DataPoint();
  DataPoint(qreal x, qreal y, QString name);
  DataPoint(qreal x, qreal y, qreal z, QString name);

  void setX(qreal x);
  void setY(qreal y);
  void setZ(qreal z);
  void setName(QString name);
  QString getName() const;
  void setColor(const QColor &color);
  QColor getColor() const;
  void setWidth(int width);
  void setRadius(int radius);
  void setSelection(bool selection);
  void setVisible(bool visible);
  void setLabelVisible(bool lvisible);
  void setMarkerType(MarkerType type);

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
  MarkerType marker() const;

  bool compare(DataPoint &p) {
    if (FLOAT_EQ(this->x(), p.x(), QEPSILON) &&
        FLOAT_EQ(this->y(), p.y(), QEPSILON) &&
        FLOAT_EQ(this->z(), p.z(), QEPSILON)) {
      if (this->getName().compare(p.getName()) == 0) {
        return true;
      } else {
        return false;
      }
    } else {
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
  MarkerType m_type;
};

#endif // DATAPOINT_H
