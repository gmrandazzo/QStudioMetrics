/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for selectionstyle operations.
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

// PlotSelectionStyleDialog.h
#ifndef SELECTIONSTYLEDIALOG_H
#define SELECTIONSTYLEDIALOG_H

#include <QColor>
#include <QDialog>
#include <scientific.h>

#include "qsmdata.h"
#include "ui_SelectionStyleDialog.h"

class SelectionStyleDialog : public QDialog {
  Q_OBJECT

public:
  enum ChangeType { NOCHANGE, FIXED, GRADIENT };

  SelectionStyleDialog();
  SelectionStyleDialog(QStringList &varname);
  void setSymbolNames(QStringList);

  int ChangeShape();
  ChangeType ChangeColor(); // 0 is no color change, 1 is solid change color, 2
                            // is from min to max change color
  ChangeType ChangeSize();  // 0 is no size change, 1 is solid change size, 2 is
                            // from min to max change size

  int getSymbolType(); // 0 = circle; 1 = square; 2 = triangle;
  QColor &getSolidSymbolColor();
  QColor &getMinSymbolColor();
  QColor &getMaxSymbolColor();
  QString getVariableSymbolColor();

  int getFixedSymbolSize();
  int getMinSymbolSize();
  int getMaxSymbolSize();
  QString getVariableSymbolSize();

public slots:
  void Cancel();
  void OK();
  void SetSolidSymbolColor();
  void SetMinSymbolColor();
  void SetMaxSymbolColor();
  void CheckUncheck();

private:
  Ui::SelectionStyleDialog ui;
  QStringList varnames;
  QColor color1, color2, color3;
};

#endif
