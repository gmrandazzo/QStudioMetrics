/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for lineplotstyle operations.
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

#ifndef LINEPLOTSTYLEDIALOG_H
#define LINEPLOTSTYLEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "ui_LinePlotStyleDialog.h"

class LinePlotStyleDialog : public QDialog {
  Q_OBJECT

public:
  LinePlotStyleDialog();
  int ChangeShape(); // 0 is no color change, 1 change color
  int ChangeColor(); // 0 is no color change, 1 change color
  int ChangeSize();  // 0 is no color change, 1 change color

  int getLineType() { return ltype; }
  QColor getLineColor() { return color; }
  int getLineSize() { return lsize; }

private slots:
  void Cancel();
  void OK();
  void SetLineType();
  void SetLineColor();
  void SetLineSize();

private:
  Ui::LinePlotStyleDialog ui;
  int lsize, ltype;
  QColor color;
};

#endif
