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

#include "LinePlotStyleDialog.h"

#include <QColorDialog>

void LinePlotStyleDialog::Cancel() { reject(); }

void LinePlotStyleDialog::OK() { accept(); }

int LinePlotStyleDialog::ChangeShape() {
  if (ui.ShapeGroupBox->isChecked()) {
    return 1;
  } else {
    return 0;
  }
}

int LinePlotStyleDialog::ChangeColor() {
  if (ui.ColorGroupBox->isChecked()) {
    return 1;
  } else {
    return 0;
  }
}

int LinePlotStyleDialog::ChangeSize() {
  if (ui.SizeGroupBox->isChecked()) {
    return 1;
  } else {
    return 0;
  }
}

void LinePlotStyleDialog::SetLineType() {
  ltype = ui.actionSelectShape->currentIndex();
}

void LinePlotStyleDialog::SetLineColor() {
  color = QColorDialog::getColor(color, this);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                   .arg(color.red())
                                   .arg(color.green())
                                   .arg(color.blue()));
}

void LinePlotStyleDialog::SetLineSize() {
  lsize = ui.actionSelectSize->value();
}

LinePlotStyleDialog::LinePlotStyleDialog() : QDialog(0) {
  ui.setupUi(this);
  setWindowTitle("Line Plot Visualization Styles");

  color = QColor(Qt::red);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);")
                                   .arg(color.red())
                                   .arg(color.green())
                                   .arg(color.blue()));

  lsize = ui.actionSelectSize->value();
  ltype = ui.actionSelectShape->currentIndex();

  connect(ui.actionSelectShape, SIGNAL(currentIndexChanged(int)),
          SLOT(SetLineType()));
  connect(ui.actionSelectColor, SIGNAL(clicked(bool)), SLOT(SetLineColor()));
  connect(ui.actionSelectSize, SIGNAL(valueChanged(int)), SLOT(SetLineSize()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
}
