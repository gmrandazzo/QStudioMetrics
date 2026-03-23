/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for importlabellist operations.
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

#include "ImportLabelListDialog.h"
#include <QFileDialog>
#include <QFileInfo>

QString ImportLabelListDialog::getFileName() { return ui.file->text(); }

QString ImportLabelListDialog::getLabel() { return ui.labelname->text(); }

void ImportLabelListDialog::Open() {
#ifdef WIN32
  if (path.isEmpty()) {
    ui.file->setText(QFileDialog::getOpenFileName(
        this, tr("Open File"), QDir::currentPath(),
        tr("Text Files (*.txt *.csv *.lst *.list);;All files (*.*)")));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  } else {
    ui.file->setText(QFileDialog::getOpenFileName(
        this, tr("Open File"), path,
        tr("Text Files (*.txt *.csv *.lst *.list);;All files (*.*)")));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }

  QStringList list = ui.file->text().split("/", Qt::SkipEmptyParts);
  if (list.size() > 0) {
    list.last().remove(".txt");
    list.last().remove(".csv");
    ui.labelname->setText(list.last());
  }
#else
  if (path.isEmpty()) {
    ui.file->setText(QFileDialog::getOpenFileName(
        this, tr("Open File"), QDir::currentPath(),
        tr("Text Files (*.txt *.csv *.lst *.list);;All files (*.*)"), 0,
        QFileDialog::DontUseNativeDialog));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  } else {
    ui.file->setText(QFileDialog::getOpenFileName(
        this, tr("Open File"), path,
        tr("Text Files (*.txt *.csv *.lst *.list);;All files (*.*)"), 0,
        QFileDialog::DontUseNativeDialog));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }

  QStringList list = ui.file->text().split("/", Qt::SkipEmptyParts);
  if (list.size() > 0) {
    list.last().remove(".txt", Qt::CaseInsensitive);
    list.last().remove(".csv", Qt::CaseInsensitive);
    ui.labelname->setText(list.last());
  }
#endif
}

ImportLabelListDialog::ImportLabelListDialog(QString path_) {
  ui.setupUi(this);
  setWindowTitle("Import Label Metadata");
  path = path_;
  connect(ui.openButton, SIGNAL(clicked(bool)), this, SLOT(Open()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
}
