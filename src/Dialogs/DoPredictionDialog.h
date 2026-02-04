/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for doprediction operations.
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

#ifndef DOPREDICTIONDIALOG_H
#define DOPREDICTIONDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QString>

#include "qsmdata.h"
#include "qstudiometricstypes.h"
#include "ui_DoPredictionDialog.h"

class DoPredictionDialog : public QDialog {
  Q_OBJECT

public:
  DoPredictionDialog(PROJECTS *projects,
                     int type_); // 0 pca, 1 pls, 2 upca, 3 upls
  ~DoPredictionDialog();

  bool compute() { return compute_; }
  int getselectedProject() { return selectedproject_; }
  QString getPredictionName() { return ui.predname->text(); }
  int getselectedData() { return selecteddata_; }
  int getselectedYData() { return selectedydata_; }
  int getselectedModel() { return selectedmodel_; }
  QStringList getObjectSelected() { return objsel; }
  QStringList getYVariableSelected() { return yvarsel; }

private slots:
  void previous();
  void next();
  void OK();
  void setProject(QModelIndex current);
  void setselectedModel(QModelIndex current);
  void setselectedData(QModelIndex current);
  void setselectedYData(QModelIndex current); // used for upls prediction
  void EnableOKButton();

  void ObjSelectAll();
  void ObjInvertSelection();
  void ObjSelectBy();
  void ObjUnselectAll();

  void VarSelectAll();
  void VarInvertSelection();
  void VarSelectBy();
  void VarUnselectAll();

private:
  Ui::DoPredictionDialog ui;
  int state;

  PROJECTS *projects_;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5;
  QList<int> pids;
  QList<int> mids;
  QStringList objsel, yvarsel;
  int selectedproject_, selecteddata_, selectedydata_, selectedmodel_;
  int type; // 0 pca, 1 pls, 2 upca, 3 upls
  bool compute_;

  void CheckDataForPrediction();
};

#endif
