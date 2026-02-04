/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for ldamodel.
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

#include "ldamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#ifdef DEBUG
#include <QDebug>
#endif

#include <QFile>
#include <QString>
#include <QTextStream>

LDAPREDICTION::LDAPREDICTION() {
  initMatrix(&pclass);
  initMatrix(&pfeatures);
  initMatrix(&probpred);
  initMatrix(&mnpdf);
  id = did = -1;
}

LDAPREDICTION::~LDAPREDICTION() {
#ifdef DEBUG
  qDebug() << "Delete LDAPREDICTION: " << name;
#endif
  DelMatrix(&pclass);
  DelMatrix(&pfeatures);
  DelMatrix(&probpred);
  DelMatrix(&mnpdf);
}

LDAModel::LDAModel() {
  NewLDAModel(&m);
  did = modelid = -1;
  validation = 0;
}

LDAModel::~LDAModel() {
#ifdef DEBUG
  qDebug() << "Delete LDA Model: " << name;
#endif
  delLDAPredictions();
  DelLDAModel(&m);
}
