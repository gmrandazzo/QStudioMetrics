/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for extractdata operations.
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

#ifndef EXTRACTDATADIALOG_H
#define EXTRACTDATADIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "GenericProgressDialog.h"
#include "qsmdata.h"
#include "qstudiometricstypes.h"
#include "ui_ExtractDataDialog.h"

class ExtractDataDialog : public QDialog {
  Q_OBJECT

public:
  explicit ExtractDataDialog(PROJECTS *projects);
  ~ExtractDataDialog();

  int getProjectID() { return pid; }
  int getDataType() { return type; }
  MATRIX *getMatrix() { return mx; }

private slots:
  void ObjectsSelectAll();
  void ObjectsInvertSelection();
  void ObjectsSelectBy();
  void ObjectsUnselect();
  void VariablesSelectAll();
  void VariablesInvertSelection();
  void VariablesSelectBy();
  void VariablesUnselect();
  void UpdateSelectedObjectCounter();
  void UpdateSelectedVariableCounter();
  void OK();
  void genListView();
  void setProjectID(QModelIndex current);
  void genObjVarView(QModelIndex current);

private:
  Ui::ExtractDataDialog ui;

  PROJECTS *projects_;
  QList<int> pids;
  QStringList hash;
  QString selectedhash_;
  int pid;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4;
  int type;

  MATRIX *mx;

  GenericProgressDialog pdialog;
  bool stoprun;
  void StartSelectionRun();
  void StopSelectionRun();
};

#endif
