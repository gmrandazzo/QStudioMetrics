/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for ListView.
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

#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QContextMenuEvent>
#include <QStringList>
#include <QWidget>

#include "ui_ListView.h"

class ListView : public QWidget {
  Q_OBJECT

public:
  explicit ListView(QStringList);

private slots:
  void copy();
  void exportlst();
  void search();

private:
  Ui::ListView ui;

protected:
  void contextMenuEvent(QContextMenuEvent *event);
};

#endif
