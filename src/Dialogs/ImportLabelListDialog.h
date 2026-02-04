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

#ifndef IMPORTLABELLISTDIALOG_H
#define IMPORTLABELLISTDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_ImportLabelListDialog.h"

class ImportLabelListDialog : public QDialog {
  Q_OBJECT

public:
  ImportLabelListDialog(QString path_);
  QString getFileName();
  QString getLabel();
  void setPath(QString &path_) { path = path_; }

private slots:
  void Open();

private:
  Ui::ImportLabelListDialog ui;
  QString path;
};

#endif
