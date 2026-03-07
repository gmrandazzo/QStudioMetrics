/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for label operations.
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

#ifndef LABELDIALOG_H
#define LABELDIALOG_H

#include "qstudiometricstypes.h"
#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "qsmdata.h"

#include "ui_LabelDialog.h"

struct labtype {
  int id;
  int type;
};

class LabelDialog : public QDialog {
  Q_OBJECT

public:
  enum { OBJLABELS = 0, VARLABELS = 1, SHOWOBJLABELS = 2, SHOWVARLABELS = 3 };
  LabelDialog(PROJECTS *projects_, int windowtype_); // used in mainwindow
  LabelDialog(LABELS *objlabels_, const QStringList &selectedobject_,
              int windowtype_); // used for table selection and plot2D/3D
  ~LabelDialog();
  void setPath(QString path_) { lastpath = path_; }

signals:
  void ShowList(ListSignal lsig);

private slots:
  void AddLabel();
  void ImportLabelList();
  void ImportTable();
  void Show();
  void RemoveLabel();
  void OK();
  void Cancel();
  void Close();
  void GenLabelView(QModelIndex);
  void setLabelId(QModelIndex);

private:
  QList<labtype> lidlst;
  Ui::LabelDialog ui;
  int windowtype;
  QString lastpath;
  int pid;
  int lid;
  PROJECTS *projects;
  LABELS *labels;
  TABLABELS *tablabels;
  QStringList selectedobject;
  QStandardItemModel *tab1, *tab2;
  bool ObjlabelContains(QString name);
  ListSignal lsig_;
  int ntablabels();
  int nlabels();
};

#endif
