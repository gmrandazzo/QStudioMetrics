/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for Imagetable.
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

#ifndef IMAGETABLE_H
#define IMAGETABLE_H

#include <QFile>
#include <QStandardItemModel> // Remove this
#include <QStringList>
#include <QTextStream>
#include <QWidget>
#include <scientific.h>

#include <QAbstractTableModel>

#include <QContextMenuEvent>

#include "Dialogs/GenericProgressDialog.h"
#include "qsmdata.h"
#include "ui_Table.h"

#include <iostream>

class ImageTableModel : public QAbstractTableModel {
  Q_OBJECT
public:
  explicit ImageTableModel(QObject *parent);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
};

#endif
