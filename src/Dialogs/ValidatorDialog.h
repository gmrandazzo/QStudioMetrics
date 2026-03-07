/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for validator operations.
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

#ifndef VALIDATORDIALOG_H
#define DoPLSUPLSValidationDialog_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QString>

#include "qsmdata.h"
#include "qstudiometricstypes.h"
#include "ui_ValidatorDialog.h"

#include "ClassDialog.h"

class ValidatorDialog : public QDialog {
  Q_OBJECT

public:
  ValidatorDialog(PROJECTS *projects, int type_);
  ~ValidatorDialog();

  bool compute() { return compute_; }
  int getselectedProject() { return selectedproject_; }
  LABELS getKFoldClasses() { return kfc; };
  int getNumberOfGroup() { return ngroup; }
  int getNumberOfIteration() { return niter; }
  int getModelID() { return modelid; }
  int getValidType() { return validtype; }
  bool ModelYScrambling() { return yscrambling; };
  int getYSCramblingModels() { return n_yscrambling; }
  CombinationRule getCombinationRule() { return crule; }

private slots:
  void OK();
  void setProject(QModelIndex current);
  void setModelID(QModelIndex current);
  void setValidationType();
  void setKFoldClass();
  void setNIterations();
  void setNGroup();
  void setYScrambling();
  void setYSCramblingModels();

private:
  Ui::ValidatorDialog ui;

  PROJECTS *projects_;
  QList<int> pids;
  QList<int> mids;
  LABELS kfc;
  int ClassNameContains(QString);
  QStandardItemModel *tab1, *tab2;
  int selectedproject_;
  int type;      // 0 PLS, 1 UPLS
  int validtype; // 0 for cross validation 1 for Leave One Out
  bool compute_, yscrambling;
  int modelid, ngroup, niter, n_yscrambling; // Number of Principal Component
  CombinationRule crule;
};

#endif
