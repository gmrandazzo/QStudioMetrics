/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for VariableSelectionModel.
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

#ifndef VARIABLESELECTIONMODEL_H
#define VARIABLESELECTIONMODEL_H

#include "qstudiometricstypes.h"
#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <scientific.h>

class VariableSelectionModel {
public:
  VariableSelectionModel();
  ~VariableSelectionModel();

  void ImportVarSelModel(QString path, QString name_);
  void WriteVarSelModel(QString path);

  void setName(QString varselname_) { varselname = varselname_; }
  QString getName() { return varselname.toUtf8(); }

  QStringList &getVariableNames() { return variablenames; }
  matrix **MapPointer() { return &map; }
  matrix *getMap() { return map; }
  uivector **SelectedVablesPointer() { return &selectedvariables; }
  uivector *getSelectedVariables() { return selectedvariables; }
  uivector **VariableDistributionPointer() { return &vardistribution; }
  uivector *getVariableDistribution() { return vardistribution; }

  void setProjectID(int pid_) { pid = pid_; }
  int getProjectID() { return pid; }
  void setModelID(int modelid_) { modelid = modelid_; }
  int getModelID() { return modelid; }

  void setModelHash(QString hash_) { hash = hash_; }
  QString &getModelHash() { return hash; }

  void setVariableSelectionAlgorithm(QString varselalgo_) {
    varselalgo = varselalgo_;
  }
  QString getVariableSelectionAlgorithm() { return varselalgo; }

  void setVariableSelectionAlgorithmOptions(QString varseloptions_) {
    varseloptions = varseloptions_;
  }
  QString getVariableSelectionAlgorithmOptions() { return varseloptions; }

private:
  QStringList variablenames;
  QString varselname, varselalgo, varseloptions;
  QString hash;
  matrix *map;
  uivector *selectedvariables, *vardistribution;
  int pid, modelid;
};

#endif
