/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for highlighttablevar operations.
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

#ifndef HIGHLIGHTTABLEVARDIALOG_H
#define HIGHLIGHTTABLEVARDIALOG_H

#include "ui_HighlightTableVarDialog.h"
#include <QColor>
#include <QDialog>

extern "C" {
#include "scientific.h"
}

class HighlightTableVarDialog : public QDialog {
  Q_OBJECT

public:
  explicit HighlightTableVarDialog(QStringList varname);

  int getVariableType() { return var; }
  QColor &getMinColor() { return colormin; }
  QColor &getMaxColor() { return colormax; }

public slots:
  void Cancel();
  void OK();
  void SetVariable();
  void SetMinColor();
  void SetMaxColor();

private:
  Ui::HighlightTableVarDialog ui;
  QColor colormin, colormax;
  int var;
};

#endif
