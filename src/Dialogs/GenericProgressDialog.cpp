/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for genericprogress operations.
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

#include "GenericProgressDialog.h"
#include <QScreen>

void GenericProgressDialog::setValue(int value) {
  ui.progressBar->setValue(value);
}

void GenericProgressDialog::setMax(int max) { ui.progressBar->setMaximum(max); }

void GenericProgressDialog::setMin(int min) { ui.progressBar->setMinimum(min); }

void GenericProgressDialog::setRange(int min, int max) {
  ui.progressBar->setRange(min, max);
}

void GenericProgressDialog::hideCancel() { ui.cancelButton->hide(); }

void GenericProgressDialog::showcancel() { ui.cancelButton->show(); }

void GenericProgressDialog::Cancel() { emit runCancelled(); }

GenericProgressDialog::GenericProgressDialog() : QDialog() {
  ui.setupUi(this);
  setWindowFlags(
      ((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint));
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  this->move(screenGeometry.center() - this->rect().center());
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
}
