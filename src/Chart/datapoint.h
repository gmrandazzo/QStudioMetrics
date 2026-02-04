/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for datapoint.
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
#ifndef CHART_FLOAT_EQ
#define CHART_FLOAT_EQ(x, v, EPSILON) (((v - EPSILON) < x) && (x < (v + EPSILON)))
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
    if (CHART_FLOAT_EQ(this->x(), p.x(), QEPSILON) &&
        CHART_FLOAT_EQ(this->y(), p.y(), QEPSILON) &&
        CHART_FLOAT_EQ(this->z(), p.z(), QEPSILON)) {
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
