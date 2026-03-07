/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for objectselector operations.
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

#ifndef OBJECTSELECTORDIALOG_H
#define OBJECTSELECTORDIALOG_H

#include "qsmdata.h"
#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "ui_ObjectSelectorDialog.h"

class ObjectSelectorDialog : public QDialog {
  Q_OBJECT

public:
  enum { MenuSelection = 0, PlotSelection = 1 };

  ObjectSelectorDialog(QList<MATRIX *> *mxlst, LABELS *objlabels,
                       LABELS *varlabels, int windowtype);
  ObjectSelectorDialog(QStringList &currentvariables, LABELS *objlabels,
                       LABELS *varlabels);
  int getSelectionType();
  QString getLabel();
  int getMetric();
  QStringList getVariableList();
  int getNumberOfObjects();

  int getDataType(); /*0 for current plot coordinates; 1 for Source Data*/
  int getDataID();

private slots:
  void EnableDisable();
  void setVariablesView(QModelIndex current);
  void SelectAll();
  void InvertSelection();
  void Unselect();
  void SelectByVarLabel();
  void OK();

private:
  Ui::ObjectSelectorDialog ui;
  int windowtype;
  LABELS *objlabels_, *varlabels_;
  int dataindex;
  QList<MATRIX *> *mxlst_;
};

#endif
