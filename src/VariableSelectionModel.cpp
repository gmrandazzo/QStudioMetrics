/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for VariableSelectionModel.
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

#include "VariableSelectionModel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

void VariableSelectionModel::ImportVarSelModel(QString path, QString name_) {
  varselname = name_;
  std::string sep = " \t";
  DATAIO::ImportMatrix(QString(path + "/map.txt").toUtf8().data(), sep, map);
  DATAIO::ImportUIvector(QString(path + "/selectedvars.txt").toUtf8().data(),
                         selectedvariables);
  DATAIO::ImportUIvector(QString(path + "/vardistribution.txt").toUtf8().data(),
                         vardistribution);
  DATA::ImportRows(path + "/varnames.txt", variablenames);
}

void VariableSelectionModel::WriteVarSelModel(QString path) {
  if (DATAIO::DirExists(path.toUtf8().data()) == false) {
    DATAIO::MakeDir(path.toUtf8().data());
  }

  DATAIO::WriteMatrix(QString(path + "/map.txt").toUtf8().data(), getMap());
  DATAIO::WriteUIvector(QString(path + "/selectedvars.txt").toUtf8().data(),
                        getSelectedVariables());
  DATAIO::WriteUIvector(QString(path + "/vardistribution.txt").toUtf8().data(),
                        getVariableDistribution());
  DATA::WriteList(getVariableNames(), path + "/varnames.txt");

  QStringList info;
  info.append(getVariableSelectionAlgorithm());
  info.append(getVariableSelectionAlgorithmOptions());
  DATA::WriteList(info, path + "/info.txt");
}

VariableSelectionModel::VariableSelectionModel() {
  initMatrix(&map);
  initUIVector(&selectedvariables);
  initUIVector(&vardistribution);
  pid = modelid = -1;
}

VariableSelectionModel::~VariableSelectionModel() {
#ifdef DEBUG
  qDebug() << "Delete Variable Selection Model";
#endif
  DelMatrix(&map);
  DelUIVector(&selectedvariables);
  DelUIVector(&vardistribution);
}
