/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for ldamodel.
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

#ifndef LDAMODEL_H
#define LDAMODEL_H

#include "../qstudiometricsdataoperations.h"
#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <scientific.h>

class LDAPREDICTION {
public:
  LDAPREDICTION();
  ~LDAPREDICTION();
  void setName(QString name_) { name = name_; }
  QString &getName() { return name; }
  void setObjName(const QStringList &objname_) { objname = objname_; }
  QStringList &getVarName() { return varname; }
  void setVarName(QStringList varname_) { varname = varname_; }
  QStringList &getObjName() { return objname; }
  void setClasses(const QList<QStringList> &classes_) { classes = classes_; }
  void setNameClasses(QStringList nameclasses_) { nameclasses = nameclasses_; }
  QList<QStringList> &getClasses() { return classes; }
  QStringList &getNameClasses() { return nameclasses; }
  matrix *getPredClasses() { return pclass; }
  void setPredID(int id_) { id = id_; }
  int getPredID() { return id; }
  void setDID(int id) { did = id; }
  int getDID() { return did; }
  void setDataHash(QString hash_) { hash = hash_; }
  QString &getDataHash() { return hash; }
  matrix *getPredFeatures() { return pfeatures; }
  matrix *getProbPred() { return probpred; }
  matrix *getMVNProbDistrib() { return mnpdf; }
  matrix **getMVNProbDistribPointer() { return &mnpdf; }

private:
  QStringList objname;
  QStringList varname;
  QList<QStringList> classes;
  QStringList nameclasses;
  QString name;
  QString hash;
  int id;
  int did;
  matrix *pfeatures;
  matrix *probpred;
  matrix *mnpdf;
  matrix *pclass;
};

class LDAModel {
public:
  LDAModel();
  ~LDAModel();
  LDAMODEL *Model() { return m; }
  void setName(QString name_) { name = name_; }
  QString &getName() { return name; }
  void setObjName(const QStringList &objname_) { objname = objname_; }
  QStringList &getObjName() { return objname; }
  void setVarName(const QStringList &varname_) { varname = varname_; }
  void setClasses(QList<QStringList> classes_) { classes = classes_; }
  void setNameClasses(QStringList nameclasses_) { nameclasses = nameclasses_; }
  QStringList &getVarName() { return varname; }
  QList<QStringList> getClasses() { return classes; }
  QStringList &getNameClasses() { return nameclasses; }
  void setValidation(int v) { validation = v; }
  int getValidation() { return validation; }
  void setDID(int did_) { did = did_; }
  void setDataHash(QString hash_) { hash = hash_; }
  void setModelID(int modelid_) { modelid = modelid_; }
  int getDID() { return did; }
  QString &getDataHash() {
    return hash;
  } // used in order to get the identity matrix provenience usefull for pls
    // cross validation
  int getModelID() { return modelid; }
  void addLDAPrediction() { prediction.append(new LDAPREDICTION); };
  void delLDAPredictionAt(int id) {
    delete prediction[id];
    prediction.removeAt(id);
  }
  void delLDAPredictions() {
    for (int i = 0; i < prediction.size(); i++) {
      delete prediction[i];
    }
    prediction.clear();
  }
  LDAPREDICTION *getLDAPrediction(int id) {
    Q_ASSERT(id < prediction.size());
    return prediction[id];
  }
  LDAPREDICTION *getLastLDAPrediction() { return prediction.last(); }
  auto LDAPredictionCount() { return prediction.size(); }
  QString &getHash() {
    if (ldahash.size() == 0) {
      ldahash = GenHashFromStrlst((QStringList() << name << "ldamodel_type") +
                                  objname + varname);
    }
    return ldahash;
  }

private:
  LDAMODEL *m;
  QList<LDAPREDICTION *> prediction;
  QStringList objname, varname;
  QList<QStringList> classes;
  QStringList nameclasses;
  QString name;
  int did, modelid, validation;
  QString hash, ldahash;
};

#endif
