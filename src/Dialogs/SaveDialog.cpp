/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for save operations.
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

#include "SaveDialog.h"
#include <QDialog>
#include <QFileDialog>
#include <QMapIterator>
#include <QModelIndex>
#include <QStringList>
#include <QStringListModel>

#include "qsmdata.h"

void SaveDialog::setProjectID(QModelIndex current) {
  if (current.isValid()) {
    pid = pids[current.row()];
  }
}

void SaveDialog::setSaveIn() {
  if (path.isEmpty()) {
    //     ui.savein->setText(QFileDialog::getExistingDirectory( this, tr("Save
    //     Project"), QDir::currentPath(), tr("Directory (*)"), 0,
    //     QFileDialog::DontUseNativeDialog));
    ui.savein->setText(QFileDialog::getExistingDirectory(
        this, "Save Project", getenv("HOME"), QFileDialog::ShowDirsOnly));
    QFileInfo last(ui.savein->text());
    path = QString::fromUtf8(last.absoluteFilePath().toUtf8());
  } else {
    ui.savein->setText(QFileDialog::getExistingDirectory(
        this, "Save Project", path, QFileDialog::ShowDirsOnly));
    //     ui.savein->setText(QFileDialog::getExistingDirectory( this, tr("Save
    //     Project"), path, tr("Directory (*)"), 0,
    //     QFileDialog::DontUseNativeDialog));
    QFileInfo last(ui.savein->text());
    path = QString::fromUtf8(last.absoluteFilePath().toUtf8());
  }
  savepath = QString::fromUtf8(ui.savein->text().toUtf8());
}

void SaveDialog::OK() {
  if (!savepath.isEmpty() && pid != -1) {
    accept();
  }
}

SaveDialog::SaveDialog(PROJECTS *projects) {
  ui.setupUi(this);
  setWindowTitle("Save Configuration");

  pid = -1;

  QStringList pname;
  QMapIterator<int, DATA *> i((*projects));
  while (i.hasNext()) {
    i.next();
    pname.append(i.value()->getProjectName());
    pids.append(i.value()->getProjectID());
  }

  ui.listView->setModel(new QStringListModel(pname));

  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui.saveinButton, SIGNAL(clicked(bool)), this, SLOT(setSaveIn()));
  connect(ui.listView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setProjectID(QModelIndex)));
}
