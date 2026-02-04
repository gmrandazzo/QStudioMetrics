/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for exporttable operations.
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

#ifndef EXPORTTABLEDIALOG_H
#define EXPORTTABLEDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QString>
#include <QStringList>

#include "qsmdata.h"

#include "ui_ExportTableDialog.h"

class ExportTableDialog : public QDialog {
  Q_OBJECT

public:
  ExportTableDialog(QStringList objname, QStringList varname,
                    LABELS *objectlabels_, LABELS *varlabels_);

  void setPath(QString path_) { path = path_; }
  QString getFileName();
  QString getSeparator();
  QStringList getSelectedObjects();
  QStringList getSelectedVariables();

private slots:
  void SaveAs();
  void EnableOKButton();

  void ObjSelectAll();
  void ObjInvertSelection();
  void ObjUnselectAll();
  void ObjSelectBy();

  void VarSelectAll();
  void VarInvertSelection();
  void VarUnselectAll();
  void VarSelectBy();

private:
  Ui::ExportTableDialog ui;
  QString path;
  LABELS objectlabels;
  LABELS varlabels;
  QString filename;
  QString separator;
  QStringList selected_objects;
  QStringList selected_variables;
};

#endif
