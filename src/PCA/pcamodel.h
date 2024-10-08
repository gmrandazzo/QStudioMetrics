#ifndef PCAMODEL_H
#define PCAMODEL_H

#include "../qstudiometricsdataoperations.h"
#include <QCryptographicHash>
#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <cmath>
#include <scientific.h>

class PCAPREDICTION {
public:
  PCAPREDICTION();
  ~PCAPREDICTION();

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

class PCAModel {
public:
  PCAModel();
  ~PCAModel();
  PCAMODEL *Model() { return m; }

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
  void setNPC(int npc_) { npc = npc_; }
  void setModelID(int modelid_) { modelid = modelid_; }
  int getDID() { return did; }
  int getXScaling() { return xscaling; }
  int getNPC() { return npc; }
  int getModelID() { return modelid; }
  void addPCAPrediction() { prediction.append(new PCAPREDICTION); };
  void delPCAPredictionAt(int id) {
    delete prediction[id];
    prediction.removeAt(id);
  }
  void delPCAPredictions() {
    for (int i = 0; i < prediction.size(); i++) {
      delete prediction[i];
    }
    prediction.clear();
  }
  PCAPREDICTION *getPCAPrediction(int id) {
    Q_ASSERT(id < prediction.size());
    return prediction[id];
  }
  PCAPREDICTION *getLastPCAPrediction() { return prediction.last(); }
  int PCAPredictionCount() { return prediction.size(); }

  QString &getHash() {
    if (pcahash.size() == 0) {
      pcahash = GenHashFromStrlst((QStringList() << name << "pcamodel_type") +
                                  objname + varname);
    }
    return pcahash;
  }

private:
  PCAMODEL *m;
  QList<PCAPREDICTION *> prediction;
  QStringList objname, varname;
  QString name;
  int did, xscaling, npc, modelid;
  QString hash, pcahash;
};

#endif
