/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for MDIChild.
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

#include "MDIChild.h"
#include "ListView.h"
#include <QProgressDialog>
#include <QScreen>
#include <QStringListModel>

void MDIChild::newModelInfo() {
  setWindowTitle("Model Info");
  modinfo = new ModelInfo(this);
  setWidget(modinfo);
  modinfo->Adjust();
  setWindowIcon(QIcon(QPixmap(1, 1)));
  adjustSize();
}

void MDIChild::newListView(QString &tabname, QStringList lst) {
  setWindowTitle(tabname);
  listview = new ListView(lst);
  setWidget(listview);
  setWindowIcon(QIcon(QPixmap(1, 1)));
  adjustSize();
}

void MDIChild::newTable(QString &tabname_) {
  setWindowTitle(tabname_);
  table = new Table(this);
  table->setObjectName(tabname_);
  setWidget(table);
  setWindowIcon(QIcon(QPixmap(1, 1)));
  resize(300, 260);
}

void MDIChild::newTable(QString &tabname_, matrix *m) {
  setWindowTitle(tabname_);
  table = new Table(m, this);
  table->setObjectName(tabname_);
  setWidget(table);
  setWindowIcon(QIcon(QPixmap(1, 1)));
  resize(300, 260);
}

void MDIChild::newTable(QString &tabname_, matrix *m, LABELS *objlabels_,
                        LABELS *varlabels_) {
  setWindowTitle(tabname_);
  table = new Table(m, objlabels_, varlabels_, this);
  table->setObjectName(tabname_);
  setWidget(table);
  setWindowIcon(QIcon(QPixmap(1, 1)));
  resize(300, 260);
}

void MDIChild::newTable(QString &tabname, QList<QStringList> tab,
                        LABELS *objlabels_, LABELS *varlabels_) {
  setWindowTitle(tabname);
  table = new Table(tab, objlabels_, varlabels_, this);
  table->setObjectName(tabname);
  setWidget(table);
  setWindowIcon(QIcon(QPixmap(1, 1)));
  resize(300, 260);
}

void MDIChild::newTable(QString &tabname, QStringList names,
                        QList<QPixmap> images, QList<QColor> colors) {
  setWindowTitle(tabname);
  table = new Table(names, images, colors, this);
  table->setObjectName(tabname);
  setWidget(table);
  setWindowIcon(QIcon(QPixmap(1, 1)));
  resize(300, 260);
}

MDIChild::MDIChild() {
  table = 0;
  modinfo = 0;
  listview = 0;
  setAttribute(Qt::WA_DeleteOnClose);
  setWindowIcon(QIcon(QPixmap(1, 1)));
  id = -1;
}

/*
MDIChild::~MDIChild()
{
  #ifdef DEBUG
  qDebug() << "Delete MDIChild with id : " << id;
  #endif
  if(table != 0)
    delete table;

  if(modinfo != 0)
    delete modinfo;
}
*/
