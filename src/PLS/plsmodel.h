#ifndef PLSMODEL_H
#define PLSMODEL_H
#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <scientific.h>

#include "qstudiometricsdataoperations.h"
#include "qstudiometricstypes.h"
#include <QDebug>

class PLSPREDICTION {
public:
  PLSPREDICTION();
  ~PLSPREDICTION();
  void setName(QString name_) { name = name_; }
  QString &getName() { return name; }
  void setObjName(const QStringList &objname_) { objname = objname_; }
  void setYVarName(const QStringList &yvarname_) { yvarname = yvarname_; }
  QStringList &getObjName() { return objname; }
  QStringList &getYVarName() { return yvarname; }
  void setPredID(int id_) { id = id_; }
  int getPredID() { return id; }
  void setDID(int id) { did = id; }
  int getDID() { return did; }
  void setDataHash(QString hash_) { hash = hash_; }
  QString &getDataHash() { return hash; }
  matrix *getXPredScores() { return pxscores; }
  matrix *getYDipVar() { return py; }
  matrix *getR2Y() { return r2y; }
  matrix *getSDEC() { return sdec; }

private:
  QStringList objname, yvarname;
  QString name;
  QString hash;
  int id;
  int did;
  matrix *pxscores;
  matrix *py;
  matrix *r2y;
  matrix *sdec;
};

class PLSModel {
public:
  PLSModel();
  ~PLSModel();
  PLSMODEL *Model() { return m; }
  void setAlgorithm(int algtype_) { algtype = algtype_; }
  int getAlgorithm() { return algtype; }
  void setName(QString name_) { name = name_; }
  QString &getName() { return name; }
  void setObjName(const QStringList &objname_) { objname = objname_; }
  QStringList &getObjName() { return objname; }
  void setXVarName(const QStringList &varname_) { xvarname = varname_; }
  QStringList &getXVarName() { return xvarname; }
  void setYVarName(const QStringList &varname_) { yvarname = varname_; }
  QStringList &getYVarName() { return yvarname; }
  void setClasses(LABELS classes_) {
    classes = classes_;
    if (classes.size() > 0) {
      yvarname.clear();
      for (int i = 0; i < classes.size(); i++)
        yvarname << classes[i].name;
    }
  }
  LABELS getClasses() { return classes; }
  void setValidation(int v) { validation = v; }
  int getValidation() { return validation; }
  void setDID(int did_) { did = did_; }
  void setDataHash(QString hash_) { hash = hash_; }
  void setXScaling(int xscaling_) {
    xscaling = xscaling_;
  } // used in order to set the identity matrix provenience usefull for pls
    // cross validation
  void setYScaling(int yscaling_) {
    yscaling = yscaling_;
  } // used in order to set the identity matrix provenience usefull for pls
    // cross validation
  void setNPC(int npc_) { npc = npc_; }
  int getNPC() { return npc; }
  void setModelID(int modelid_) { modelid = modelid_; }
  int getDID() { return did; }
  QString &getDataHash() {
    return hash;
  } // used in order to get the identity matrix provenience usefull for pls
    // cross validation
  int getXScaling() { return xscaling; }
  int getYScaling() { return yscaling; }
  int getModelID() { return modelid; }
  void addPLSPrediction() { prediction.append(new PLSPREDICTION); };
  void delPLSPredictionAt(int id) {
    delete prediction[id];
    prediction.removeAt(id);
  }
  void delPLSPredictions() {
    for (int i = 0; i < prediction.size(); i++) {
      delete prediction[i];
    }
    prediction.clear();
  }

  PLSPREDICTION *getPLSPrediction(int id) {
    Q_ASSERT(id < prediction.size());
    return prediction[id];
  }
  PLSPREDICTION *getLastPLSPrediction() { return prediction.last(); }
  int PLSPredictionCount() { return prediction.size(); }

  QString &getHash() {
    if (plshash.size() == 0) {
      QStringList hashprep;
      hashprep << name;
      hashprep << "plsmodel_type";
      hashprep += objname;
      hashprep += xvarname;
      hashprep += yvarname;
      for (int i = 0; i < classes.size(); i++) {
        hashprep << classes[i].name;
        hashprep += classes[i].objects;
      }
      plshash = GenHashFromStrlst(hashprep);
      // plshash = GenHashFromStrlst((QStringList() << name <<
      // "plsmodel_type")+objname+xvarname+yvarname);
    }
    return plshash;
  }

private:
  PLSMODEL *m;
  QList<PLSPREDICTION *> prediction;
  QStringList objname, xvarname, yvarname;
  QString name;
  LABELS classes;
  int did, xscaling, yscaling, npc, modelid, validation, algtype;
  QString hash, plshash;
};

#endif
