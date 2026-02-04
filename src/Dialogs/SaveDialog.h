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

// SaveDialog.h
#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QList>
#include <QModelIndex>
#include <QString>

#include "qsmdata.h"
#include "ui_SaveDialog.h"

class SaveDialog : public QDialog {
  Q_OBJECT

public:
  explicit SaveDialog(PROJECTS *projects);
  int getProjectID() { return pid; };
  QString getPathToSave() { return savepath; };
  void setPath(const QString &path_) { path = path_; }

private slots:
  void setProjectID(QModelIndex current);
  void setSaveIn();
  void OK();

private:
  Ui::SaveDialog ui;
  QString path;
  QList<int> pids;

  QString savepath;
  int pid;
};

#endif
