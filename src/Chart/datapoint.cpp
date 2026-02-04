/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for datapoint.
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

#include "datapoint.h"

DataPoint::DataPoint()
    : m_x(0.f), m_y(0.f), m_z(0.f), m_name("No name"), m_color(Qt::black),
      m_width(10), m_radius(5), m_selection(false), m_visible(true),
      m_lvisible(false), m_type(CIRCLE) {}

DataPoint::DataPoint(qreal x, qreal y, QString name)
    : m_x(x), m_y(y), m_z(0.f), m_name(name), m_color(Qt::black), m_width(10),
      m_radius(10), m_selection(false), m_visible(true), m_lvisible(false),
      m_type(CIRCLE) {}

DataPoint::DataPoint(qreal x, qreal y, qreal z, QString name)
    : m_x(x), m_y(y), m_z(z), m_name(name), m_color(Qt::black), m_width(10),
      m_radius(10), m_selection(false), m_visible(true), m_lvisible(false),
      m_type(CIRCLE) {}

void DataPoint::setX(qreal x) { m_x = x; }

void DataPoint::setY(qreal y) { m_y = y; }

void DataPoint::setZ(qreal z) { m_z = z; }

void DataPoint::setName(QString name) { m_name = name; }

QString DataPoint::getName() const { return m_name; }

void DataPoint::setColor(const QColor &color) { m_color = color; }

QColor DataPoint::getColor() const { return m_color; }

void DataPoint::setWidth(int width) { m_width = width; }

void DataPoint::setRadius(int radius) { m_radius = radius; }

void DataPoint::setSelection(bool selection) { m_selection = selection; }

void DataPoint::setVisible(bool visible) { m_visible = visible; }

void DataPoint::setLabelVisible(bool lvisible) { m_lvisible = lvisible; }

void DataPoint::setMarkerType(MarkerType type) { m_type = type; }

qreal DataPoint::x() const { return m_x; }

qreal DataPoint::y() const { return m_y; }

qreal DataPoint::z() const { return m_z; }

QString DataPoint::name() const { return m_name; }

bool DataPoint::isSelected() const { return m_selection; }

bool DataPoint::isVisible() const { return m_visible; }

bool DataPoint::isLabelVisible() const { return m_lvisible; }

QColor DataPoint::color() const { return m_color; }

int DataPoint::width() const { return m_width; }

int DataPoint::radius() const { return m_radius; }

MarkerType DataPoint::marker() const { return m_type; }
