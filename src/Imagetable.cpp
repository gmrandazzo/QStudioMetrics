/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for Imagetable.
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

#include "Imagetable.h"

ImageTableModel::ImageTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

int ImageTableModel::rowCount(const QModelIndex & /*parent*/) const {
  return 2;
}
int ImageTableModel::columnCount(const QModelIndex & /*parent*/) const {
  return 3;
}
QVariant ImageTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    return QString("Row%1, Column%2")
        .arg(index.row() + 1)
        .arg(index.column() + 1);
  }
  return QVariant();
}
