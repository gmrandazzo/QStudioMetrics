/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for datacurve.
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

#ifndef DATACURVE_H
#define DATACURVE_H

#include <QColor>
#include <QList>
#include <QMetaType>
#include <QPointF>
#include <QString>
#include <QVector>
#include <QtGlobal>

enum LTYPE {
  L, // Lines
  LM // Lines and markers
};

class DataCurve {
public:
  DataCurve();
  DataCurve(QVector<QPointF> curve, QString name, QColor color);

  void addPoint(qreal x, qreal y);
  void setName(QString name);
  void setColor(const QColor &color);
  void setRadius(int radius);
  void setWidth(int width);
  void setVisible(bool visible);
  void setStyle(LTYPE lt_);
  void setSmooth(bool smooth);

  QVector<QPointF> getPoints() const;
  QString name() const;
  QColor color() const;
  LTYPE getStyle() const;
  int radius() const;
  int width() const;
  bool isVisible() const;
  bool isSmooth() const;

private:
  QVector<QPointF> m_curve;
  QString m_name;
  QColor m_color;
  int m_radius;
  int m_width;
  bool m_visible;
  bool m_isSmooth;
  LTYPE lt;
};

#endif // DATACURVE_H
