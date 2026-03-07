/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for mlrmodel.
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

#include "mlrmodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

MLRPREDICTION::MLRPREDICTION() {
  initMatrix(&py);
  initDVector(&r2y);
  initDVector(&sdec);
  id = did = -1;
}

MLRPREDICTION::~MLRPREDICTION() {
#ifdef DEBUG
  qDebug() << "Delete MLRPREDICTION: " << name;
#endif
  DelMatrix(&py);
  DelDVector(&r2y);
  DelDVector(&sdec);
}

MLRModel::MLRModel() {
  NewMLRModel(&m);
  did = modelid = -1;
  validation = 0;
}

MLRModel::~MLRModel() {
#ifdef DEBUG
  qDebug() << "Delete MLR Model: " << name;
#endif
  delMLRPredictions();
  DelMLRModel(&m);
}
