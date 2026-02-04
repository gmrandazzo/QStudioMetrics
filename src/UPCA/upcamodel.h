/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for upcamodel.
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

#ifndef UPCAMODEL_H
#define UPCAMODEL_H

#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <scientific.h>

class UPCAPREDICTION {
public:
  UPCAPREDICTION();
  ~UPCAPREDICTION();
  void setName(QString name_) { name = name_; }
  QString &getName() { return name; }
  void setObjName(QStringList &objname_) { objname = objname_; }
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
  int id;
  int did;
  matrix *pscores;
};

class UPCAModel {
public:
  UPCAModel();
  ~UPCAModel();
  UPCAMODEL *Model() { return m; }
  void setName(QString name_) { name = name_; }
  QString &getName() { return name; }
  void setObjName(QStringList &objname_) { objname = objname_; }
  QStringList &getObjName() { return objname; }
  void setVarName(QStringList &varname_) { varname = varname_; }
  QStringList &getVarName() { return varname; }
  void setDID(int did_) { did = did_; }
  void setDataHash(QString hash_) { hash = hash_; }
  void setXScaling(int xscaling_) { xscaling = xscaling_; }
  void setNPC(int npc_) { npc = npc_; }
  void setModelID(int modelid_) { modelid = modelid_; }
  int getDID() { return did; }
  QString &getDataHash() { return hash; }
  int getXScaling() { return xscaling; }
  int getNPC() { return npc; }
  int getModelID() { return modelid; }
  void addUPCAPrediction() { prediction.append(new UPCAPREDICTION); };
  void delUPCAPredictionAt(int id) {
    delete prediction[id];
    prediction.removeAt(id);
  }
  void delUPCAPredictions() {
    for (int i = 0; i < prediction.size(); i++) {
      delete prediction[i];
    }
    prediction.clear();
  }
  UPCAPREDICTION *getUPCAPrediction(int id) {
    Q_ASSERT(id < prediction.size());
    return prediction[id];
  }
  UPCAPREDICTION *getLastUPCAPrediction() { return prediction.last(); }
  int UPCAPredictionCount() { return prediction.size(); }

private:
  UPCAMODEL *m;
  QList<UPCAPREDICTION *> prediction;
  QStringList objname, varname;
  QString name;
  int did, xscaling, npc, modelid;
  QString hash;
};

#endif
