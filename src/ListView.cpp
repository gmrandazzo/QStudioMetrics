/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for ListView.
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

#include "ListView.h"

#include <QAction>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QStringListModel>
#include <QTextStream>

void ListView::contextMenuEvent(QContextMenuEvent *event) {
  QWidget::contextMenuEvent(event);
  QAction *copyAct = 0, *exportAct = 0, *searchAct = 0;

  QMenu menu(this);
  copyAct = new QAction(tr("&Copy"), this);
  //   copyAct->setShortcuts(QKeySequence::Copy);
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                           "clipboard"));
  connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

  menu.addAction(copyAct);

  exportAct = new QAction(tr("&Export.."), this);
  //   exportAct->setShortcuts(QKeySequence::Save);
  exportAct->setStatusTip(tr("Export the current list content"));
  connect(exportAct, SIGNAL(triggered()), this, SLOT(exportlst()));

  menu.addAction(exportAct);

  /*
  searchAct = new QAction(tr("&Search.."), this);
//   searchAct->setShortcuts(QKeySequence::Find);
  searchAct->setStatusTip(tr("Search in the current list contents"));
  connect(searchAct, SIGNAL(triggered()), this, SLOT(search()));

  menu.addAction(searchAct);
  */
  menu.exec(event->globalPos());

  delete copyAct;
  delete searchAct;
  delete exportAct;
}

void ListView::copy() {
  QStringList strings;
  for (int i = 0; i < ui.listView->selectionModel()->selectedRows().size(); ++i)
    strings << ui.listView->selectionModel()
                   ->selectedRows()[i]
                   .data(Qt::DisplayRole)
                   .toString();
  QApplication::clipboard()->setText(strings.join("\n"));
}

void ListView::exportlst() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Export list to file..."), "", tr("TXT (*.txt);;All Files (*)"));

  if (!fileName.isEmpty()) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QTextStream out(&file);
    for (int i = 0; i < ui.listView->model()->rowCount(); i++) {
      out << ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString()
          << "\n";
    }
    file.close();
  }
}

ListView::ListView(QStringList lst) : QWidget(0) {
  ui.setupUi(this);
  ui.listView->setModel(new QStringListModel(lst));
}
