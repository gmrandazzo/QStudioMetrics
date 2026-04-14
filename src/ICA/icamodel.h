/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for icamodel.
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

#ifndef ICAMODEL_H
#define ICAMODEL_H

#include "../qstudiometricsdataoperations.h"
#include <QCryptographicHash>
#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <cmath>
#include <scientific.h>

class ICAPREDICTION {
public:
  ICAPREDICTION();
  ~ICAPREDICTION();

  void setName(QString name_) { name = name_; }
  QString &getName() { return name; }
  void setObjName(const QStringList &objname_) { objname = objname_; }
  QStringList &getObjName() { return objname; }
  void setPredID(int id_) { id = id_; }
  int getPredID() { return id; }
  void setDID(int id) { did = id; }
  int getDID() { return did; }
  void setDataHash(QString hash_) { hash = hash_; }
  QString &getDataHash() { return hash; }
  matrix *getPredScores() { return pscores; }

private:
  QStringList objname;
  QString name;
  QString hash;
  int id, did;
  matrix *pscores;
};

class ICAModel {
public:
  ICAModel();
  ~ICAModel();
  ICAMODEL *Model() { return m; }

  void setName(QString name_) { name = name_; }
  QString &getName() { return name; }
  void setObjName(const QStringList &objname_) { objname = objname_; }
  QStringList &getObjName() { return objname; }
  void setVarName(const QStringList &varname_) { varname = varname_; }
  QStringList &getVarName() { return varname; }
  void setDID(int did_) { did = did_; }
  void setDataHash(QString hash_) { hash = hash_; }
  QString &getDataHash() { return hash; }
  void setXScaling(int xscaling_) { xscaling = xscaling_; }
  void setNIC(int nic_) { nic = nic_; }
  void setModelID(int modelid_) { modelid = modelid_; }
  int getDID() { return did; }
  int getXScaling() { return xscaling; }
  int getNIC() { return nic; }
  int getModelID() { return modelid; }
  void addICAPrediction() { prediction.append(new ICAPREDICTION); };
  void delICAPredictionAt(int id) {
    delete prediction[id];
    prediction.removeAt(id);
  }
  void delICAPredictions() {
    for (int i = 0; i < prediction.size(); i++) {
      delete prediction[i];
    }
    prediction.clear();
  }
  ICAPREDICTION *getICAPrediction(int id) {
    Q_ASSERT(id < prediction.size());
    return prediction[id];
  }
  ICAPREDICTION *getLastICAPrediction() { return prediction.last(); }
  int ICAPredictionCount() { return prediction.size(); }

  QString &getHash() {
    if (icahash.size() == 0) {
      icahash = GenHashFromStrlst((QStringList() << name << "icamodel_type") +
                                  objname + varname);
    }
    return icahash;
  }

private:
  ICAMODEL *m;
  QList<ICAPREDICTION *> prediction;
  QStringList objname, varname;
  QString name;
  int did, xscaling, nic, modelid;
  QString hash, icahash;
};

#endif
