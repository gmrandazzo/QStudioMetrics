/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for advancedpretreatment operations.
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

#ifndef ADVANCEDPRETREATMENTDIALOG_H
#define ADVANCEDPRETREATMENTDIALOG_H


#include <QDialog>
#include <QStandardItemModel>

#include "qsmdata.h"
#include "ui_AdvancedPretreatmentDialog.h"

class AdvancedPretreatmentDialog : public QDialog {
  Q_OBJECT

public:
  explicit AdvancedPretreatmentDialog(PROJECTS *projects_);
  ~AdvancedPretreatmentDialog();
  MATRIX *getMergedMatrix() { return mx; }
  int getProjectID() { return pid; }
private slots:
  void OK();
  void EnableOKButton();
  void setMatrixID(QModelIndex current);
  void setProjectID(QModelIndex current);
  void SelectAllVars();
  void InvertVarSelection();
  void UnselectAllVars();
  void SelectVarsBy(int current);

private:
  Ui::AdvancedPretreatmentDialog ui;
  PROJECTS *projects;
  MATRIX *mx;
  QStandardItemModel *tab1, *tab2, *tab3;
  QList<int> pids;
  int pid, mxid;
};

#endif
