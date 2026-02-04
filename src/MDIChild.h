/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for MDIChild.
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

#ifndef MDICHILD_H
#define MDICHILD_H

#include "ListView.h"
#include "ModelInfo.h"
#include "Table.h"
#include "qsmdata.h"
#include <QMdiSubWindow>
#include <QStandardItem>
#include <QString>

#include <scientific.h>

class MDIChild : public QMdiSubWindow {
  Q_OBJECT

public:
  MDIChild();

  void newTable(QString &tabname); // used in order to create a new matrix
                                   // inside the model
  void newTable(QString &tabname,
                matrix *m); // show in this table an'allocated matrix
  void newTable(QString &tabname, matrix *m, LABELS *objlabels_,
                LABELS *varlabels_); // show in this table an'allocated matrix
  void newTable(QString &tabname, QList<QStringList> tab, LABELS *objlabels_,
                LABELS *varlabels_); // show in this table an'allocated matrix
  void newTable(QString &tabname, QStringList names, QList<QPixmap> images,
                QList<QColor> colors);
  void newListView(QString &tabname, QStringList lst);
  void newModelInfo();

  void setWindowID(int id_) { id = id_; }
  int getWindowID() { return id; }
  Table *getTable() { return table; }
  Model *getTableModel() { return table->model(); }
  ModelInfo *getModelInfoWindow() { return modinfo; }
  ListView *getListView() { return listview; }

private:
  Table *table;
  ModelInfo *modinfo;
  ListView *listview;
  int id;
};
#endif
