/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for plots operations.
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

#ifndef DIALOGPLOTS_H
#define DIALOGPLOTS_H

#include "qsmdata.h"
#include "ui_DialogPlots.h"
#include <QDialog>
#include <QModelIndex>
#include <QStandardItemModel>

class DialogPlots : public QDialog {
  Q_OBJECT

public:
  enum { TwoColumns = 0, ThreeColumns };

  DialogPlots(ProjectTree pjtree_, int dtype_);
  ~DialogPlots();
  void hideOptions(bool);
  int getProjectID();
  int getModelID();
  int getPredictionID();
  int getNLV();

private slots:
  void actionPlot();
  void setProject(QModelIndex, QModelIndex);
  void setModel(QModelIndex, QModelIndex);
  void setPrediction(QModelIndex, QModelIndex);

private:
  Ui::DialogPlots ui;
  ProjectTree pjtree;
  int dtype;
  int pid, mid, predid;
  QStandardItemModel *tab1, *tab2, *tab3;
  void EnableDisableButtons();
};

#endif
