/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for classmodel operations.
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

// ClassModelDialog.h

#ifndef CLASSMODELDIALOG_H
#define CLASSMODELDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QString>

#include "qsmdata.h"
#include "qstudiometricstypes.h"
#include "ui_ClassModelDialog.h"

class ClassModelDialog : public QDialog {
  Q_OBJECT

public:
  explicit ClassModelDialog(PROJECTS *projects);
  ~ClassModelDialog();

  bool compute() { return compute_; }
  int getselectedProject() { return selectedproject_; }
  QString &getModelName() { return modelname_; }
  int getselectedData() { return selecteddata_; }
  QList<QStringList> getClasses() { return classes; }
  QStringList getNameClasses() { return nameclasses; }
  QStringList getVarSelected() { return varsel; }

private slots:
  void next();
  void previous();
  void OK();
  void genListView(QModelIndex current);
  void setData(QModelIndex current);
  void EnableDisableButtons();

  void AddClass();
  void RemoveClass();

  void ObjSelectAll();
  void ObjInvertSelection();
  void ObjUnselectAll();
  void ObjSelectBy();

  void VarSelectAll();
  void VarInvertSelection();
  void VarUnselectAll();
  void VarSelectBy();

private:
  Ui::ClassModelDialog ui;
  int state;

  PROJECTS *projects_;
  QList<int> pids;
  QList<QStringList> classes;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5;
  QStringList nameclasses, varsel;
  QString modelname_;
  int selectedproject_, selecteddata_;
  bool compute_;
};

#endif
