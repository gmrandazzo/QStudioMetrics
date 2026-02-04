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

#include "HighlightTableVarDialog.h"
#include "qstudiometricstypes.h"
#include <QColorDialog>

void HighlightTableVarDialog::Cancel() { reject(); }

void HighlightTableVarDialog::OK() { accept(); }

void HighlightTableVarDialog::SetMaxColor() {
  colormax = QColorDialog::getColor(colormax, this);
  ui.actionSelectColorMax->setStyleSheet(
      QString("background-color: rgb(%1, %2, %3);")
          .arg(colormax.red())
          .arg(colormax.green())
          .arg(colormax.blue()));
}

void HighlightTableVarDialog::SetMinColor() {
  colormin = QColorDialog::getColor(colormin, this);
  ui.actionSelectColorMin->setStyleSheet(
      QString("background-color: rgb(%1, %2, %3);")
          .arg(colormin.red())
          .arg(colormin.green())
          .arg(colormin.blue()));
}

void HighlightTableVarDialog::SetVariable() {
  var = ui.varlist->currentIndex();
}

HighlightTableVarDialog::HighlightTableVarDialog(QStringList varname)
    : QDialog() {
  ui.setupUi(this);

  var = 0;

  colormin = QColor(Qt::green);
  ui.actionSelectColorMin->setStyleSheet(
      QString("background-color: rgb(%1, %2, %3);")
          .arg(colormin.red())
          .arg(colormin.green())
          .arg(colormin.blue()));

  colormax = QColor(Qt::red);
  ui.actionSelectColorMax->setStyleSheet(
      QString("background-color: rgb(%1, %2, %3);")
          .arg(colormax.red())
          .arg(colormax.green())
          .arg(colormax.blue()));

  for (int i = 0; i < varname.size(); i++) {
    if (varname[i].compare(firstcol_name, Qt::CaseInsensitive) == 0 ||
        varname[i].compare("Principal Component", Qt::CaseInsensitive) == 0 ||
        varname[i].compare("Variables", Qt::CaseInsensitive) == 0 ||
        varname[i].compare("Models Name", Qt::CaseInsensitive) == 0) {
      continue;
    } else {
      ui.varlist->addItem(varname[i]);
    }
  }

  connect(ui.varlist, SIGNAL(currentIndexChanged(int)), SLOT(SetVariable()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.actionSelectColorMin, SIGNAL(clicked(bool)), SLOT(SetMinColor()));
  connect(ui.actionSelectColorMax, SIGNAL(clicked(bool)), SLOT(SetMaxColor()));
}
