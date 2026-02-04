/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for addfile operations.
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

#ifndef ADDFILEDIALOG_H
#define ADDFILEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
// #include "Table.h"

#include "ui_addfileDialog.h"

class AddFileDialog : public QDialog {
  Q_OBJECT

public:
  AddFileDialog();
  ~AddFileDialog();
  QString getFileName();
  QString getLabel();
  QString getSeparator();
  QString getSkipChar();
  QString getFileObjName();
  QString getFileVarName();
  void setPath(QString &path_) { path = path_; }
  void setSimpleOpen();

private slots:
  void Open();
  void OpenFileObjName();
  void OpenFileVarName();
  void Accept();
  void Preview();

private:
  Ui::AddFileDialog ui;
  QStandardItemModel *model;
  QString path;
  QStringList ListRead(QString file_);
  auto CountNumberRow(QString file_);
  auto CountNumberColum(QString file_);
};

#endif
