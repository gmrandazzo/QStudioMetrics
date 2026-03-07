/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for classplot operations.
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

// ClassPlotDialog.ui
#ifndef CLASSPLOTDIALOG_H
#define CLASSPLOTDIALOG_H

#include "qsmdata.h"
#include <QDialog>
#include <QList>
#include <QModelIndex>
#include <QStandardItemModel>
#include <QStringList>

#include "ui_ClassPlotDialog.h"

class ClassPlotDialog : public QDialog {
  Q_OBJECT

public:
  enum WindowType { PCALOADINGS = 0, PLSLOADINGS = 1 };

  ClassPlotDialog(PROJECTS *projects,
                  int type); // 0 = PCA;   1 = PLS; 2 = UPCA;   3 = UPLS; LDA
  ~ClassPlotDialog();

  int selectedProject() { return selectedproject_; }
  int getModelID() { return modelid; }
  LABELS getClass() { return g; }
  bool Plot() { return plot_; }

private slots:
  void actionPlot();
  void setProject(QModelIndex);
  void setModelID(QModelIndex);
  void Find();
  void FindAll();
  void AddGroup();
  void RemoveGroup();
  void SelectAll();
  void InvertSelection();
  void SelectBy();
  void UnselectAll();

private:
  Ui::ClassPlotDialog ui;
  PROJECTS *projects_;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4;
  QList<int> pids;
  QList<int> mids;
  int selectedproject_;
  int type;
  int modelid;
  bool plot_;
  void CheckPlot();
  LABELS labels, g;
};

#endif
