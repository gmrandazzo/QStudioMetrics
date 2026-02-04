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

#ifndef GENERICPROGRESSDIALOG_H
#define GENERICPROGRESSDIALOG_H

#include <QDialog>

#include "ui_GenericProgressDialog.h"

class GenericProgressDialog : public QDialog {
  Q_OBJECT

public:
  GenericProgressDialog();
  void setRange(int min, int max);
  void setMin(int min);
  void setMax(int max);
  void setValue(int value);

  void showcancel();
  void hideCancel();

private slots:
  void Cancel();

signals:
  void runCancelled();

private:
  Ui::GenericProgressDialog ui;
};

#endif
