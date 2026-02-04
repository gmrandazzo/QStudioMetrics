/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for importfile operations.
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

#ifndef IMPORTFILEDIALOG_H
#define IMPORTFILEDIALOG_H

#include <QAbstractListModel>
#include <QDialog>
#include <QObject>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "qsmdata.h"
#include "ui_ImportFileDialog.h"

struct FSIZE {
  size_t row;
  size_t col;
  size_t linelength;
};

class ImportFileDialog : public QDialog {
  Q_OBJECT

public:
  ImportFileDialog();
  ~ImportFileDialog();

  QString getLastPath() { return path; }
  QString getFileName();
  MATRIX *getMatrix() { return m; }
  ARRAY *getArray() { return a; }
  void setPath(QString &path_) { path = path_; }

private slots:
  void Open();
  void OK();
  void Cancel();
  void Preview();

private:
  Ui::ImportFileDialog ui;
  QString path;
  MATRIX *m;
  ARRAY *a;
  QStandardItemModel *model;

  void AssignName(QStringList &list, QString name);
  QString getSkipChar();
  QString getSeparator();
  QString getLabel();

  FSIZE GetSize();
  size_t getHeader(QStringList *header);
  QList<size_t> getLineToSkip();

  void ImportType0();
  void ImportType1();
  void ImportType2();
  void ImportType3();

  void BuildMatrix();
  void Clean_rnames();
  QMap<QString, int> rnames;
  //   void BuildArray();
};

#endif
