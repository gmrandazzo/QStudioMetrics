/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for selectby operations.
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

#ifndef SELECTBYDIALOG_H
#define SELECTBYDIALOG_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QStringList>

#include "qsmdata.h"
#include "ui_SelectByDialog.h"

class SelectByDialog : public QDialog {
  Q_OBJECT

public:
  SelectByDialog(QStringList objname_, matrix *coordinate_,
                 QList<MATRIX *> *mxlst_, QList<int> mxid_, LABELS *objlabel_,
                 LABELS *varlabel_);
  ~SelectByDialog();
  QList<int> GetSelectedObjID() { return selectedobjid; }

private slots:
  void UpdateSelectionCounter();
  void selectButton();
  void invertselectButton();
  void appendselectionButton();
  void unselectButton();
  void SetSelectionList();
  void OpenSelectorButton();
  void FindName();
  void Abort();
  void OK();
  void Cancel();

private:
  Ui::SelectByDialog ui;
  QStringList selectedobjname;
  QList<int> selectedobjid;
  QStringList objname;
  matrix *coordinate;
  QList<MATRIX *> *mxlst;
  QList<int> mxid;
  LABELS *objlabel, *varlabel;
  bool append;
  bool abort;
  // Used for other selection...
  int selectiontype, metric, nobjects, datatype, dataid, layerid;
  QString label;
  QStringList varlist;

  void SetSelectionView();
  bool CheckIDSmxlst(QList<int> *ids);

  void StartSelectionRun();
  void WaitSelectionRun();
  void StopSelectionRun();
};

#endif
