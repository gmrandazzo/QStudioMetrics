/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for icamodel.
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

#include "icamodel.h"
#include "../DATAIO.h"
#include "../qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

ICAPREDICTION::ICAPREDICTION() {
  initMatrix(&pscores);
  id = did = -1;
}

ICAPREDICTION::~ICAPREDICTION() {
#ifdef DEBUG
  qDebug() << "Delete ICAPREDICTION: " << name;
#endif
  DelMatrix(&pscores);
}

ICAModel::~ICAModel() {
#ifdef DEBUG
  qDebug() << "Delete ICA Model: " << name;
#endif
  delICAPredictions();
  DelICAModel(&m);
}

ICAModel::ICAModel() {
  NewICAModel(&m);
  did = xscaling = nic = modelid = -1;
}
