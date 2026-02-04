/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for variableplot operations.
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

#ifndef VARIABLEPLOTDIALOG_H
#define VARIABLEPLOTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "qsmdata.h"

#include "ui_VariablePlotDialog.h"

class VariablePlotDialog : public QDialog {
  Q_OBJECT

public:
  enum WindowType { VariableDistribution = 0, VariableVSVariable = 1 };
  VariablePlotDialog(PROJECTS *projects_,
                     int windowtype_); // used in mainwindow
  ~VariablePlotDialog();

  int getVarType() { return vtype; }
  int getProjectID() { return pid; }
  QStringList getDataHash() { return hash; }
  QList<QStringList> getObjects() { return objects; }
  int getVariableID1() { return varid1; }
  int getVariableID2() { return varid2; }
  QString getVariableName1();
  QString getVariableName2();
  QStringList getObjLabelSelected();

private slots:
  void setVariableID1(QModelIndex);
  void setVariableID2(QModelIndex);
  void GenVariableViewAndSetHash();
  void GenDataViewAndLabelView(QModelIndex);
  void getObjectNames();
  void OK();
  void Cancel();

private:
  Ui::VariablePlotDialog ui;
  int windowtype;
  int vtype; // 0 matrix; 1 array
  int pid;
  int varid1, varid2;
  QList<uint> pids;
  QStringList hash;
  QStringList labelobjsel;
  QList<QStringList> objects;
  PROJECTS *projects;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5;

  bool EnableOKButton();
  bool checkOK();
};

#endif
