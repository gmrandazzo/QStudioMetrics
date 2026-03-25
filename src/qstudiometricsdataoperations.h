/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for qstudiometricsdataoperations.
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

#ifndef QSTUDIOMETRICSDATAOPERATIONS_H
#define QSTUDIOMETRICSDATAOPERATIONS_H
#include "qstudiometricstypes.h"

#include <QCryptographicHash>
#include <QString>
#include <QStringList>
#include <iostream>
#include <scientific.h>
#include <string>

// Conversion functions for sql export
// N.B.: To serialize vector,matrix and tensor is utilised a std::string
// to avoid double conversion problems and truncated floating points present in
// QString

namespace QSM_SQL_SERIALIZATION {
const QString SEP_VALUE = ";";
const QString SEP_ROW = "//";
const QString SEP_LABEL = "/#";
const QString SEP_ORDER = "o";
}

LABELS DeserializeLABELS(QString serialized_l);
QString SerializeLABELS(LABELS l);
QStringList DeserializeQStringList(QString serialized_lst);
QString SerializeQStringList(QStringList lst);
void DeserializeDVector(QString serialized_dvector, dvector *v);
QString SerializeDVector(const dvector *v);
void DeserializeUIVector(QString serialized_dvector, uivector *v);
QString SerializeUIVector(const uivector *v);
void DeserializeMatrix(QString serialized_mx, matrix *mx);
QString SerializeMatrix(const matrix *mx);
void DeserializeTensor(QString serialized_ar, tensor *ar);
QString SerializeTensor(const tensor *ar);

// hash functions
// Serialze the matrix and return a QCryptographicHash in hexadecimal

QString GenHashFromStrlst(QStringList slst);

#endif
