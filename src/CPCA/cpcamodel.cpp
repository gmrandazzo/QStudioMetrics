/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for cpcamodel.
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

#include "cpcamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

CPCAPREDICTION::CPCAPREDICTION() {
  initMatrix(&p_super_scores);
  initTensor(&p_block_scores);
  id = did = -1;
}

CPCAPREDICTION::~CPCAPREDICTION() {
#ifdef DEBUG
  qDebug() << "Delete CPCAPREDICTION: " << name;
#endif
  DelMatrix(&p_super_scores);
  DelTensor(&p_block_scores);
}

CPCAModel::~CPCAModel() {
#ifdef DEBUG
  qDebug() << "Delete CPCA Model: " << name;
#endif
  delCPCAPredictions();
  DelCPCAModel(&m);
}

CPCAModel::CPCAModel() {
  NewCPCAModel(&m);
  did = xscaling = npc = modelid = -1;
}
