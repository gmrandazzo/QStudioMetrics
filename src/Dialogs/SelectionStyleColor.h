/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for SelectionStyleColor.
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

#ifndef SELECTIONSTYLECOLOR_H
#define SELECTIONSTYLECOLOR_H

#include <QColor>
#include <QDialog>
#include <scientific.h>

#include "qsmdata.h"
#include "ui_SelectionStyleColor.h"

class SelectionStyleColor : public QDialog {
  Q_OBJECT

public:
  SelectionStyleColor();
  SelectionStyleColor(const QStringList &xvarname_, const QStringList &yvarname_);
  SelectionStyleColor(const QStringList &xvarname_, const QStringList &yvarname_,
                      const int &xlevels_,
                      const int &ylevels_); // used for array multiway where could be
                                      // more than one layer

  int ChangeColorType();

  QColor &getSolidSymbolColor() { return color1; }
  int getVariableTypeSymbolColor() { return varcolor; }
  QColor &getMinSymbolColor() { return color2; }
  QColor &getMaxSymbolColor() { return color3; }
  int getVariableSymbolColor() { return cvar; }
  int getVariableLevelSymbolColor() { return clevel; }

public slots:
  void Cancel();
  void OK();
  void SetSolidSymbolColor();
  void SetVariableTypeSymbolColor();
  void SetMinSymbolColor();
  void SetMaxSymbolColor();
  void SetVariableSymbolColor();
  void SetVariableLevelSymbolColor();
  void CheckUncheck();

private:
  Ui::SelectionStyleColor ui;
  QStringList xvarname, yvarname;
  int xlevels, ylevels;
  QColor color1, color2, color3;
  int cvar, varcolor, clevel;
};

#endif
