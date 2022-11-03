#ifndef MLRMODEL_H
#define MLRMODEL_H

#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>

#include "../qstudiometricsdataoperations.h"

class MLRPREDICTION
{
public:
  MLRPREDICTION();
  ~MLRPREDICTION();
  void ImportMLRPrediction(char *path, char *name_);
  void WriteMLRPrediction(char *path, char *dirname_);
  void ImportMLRPredictionInfo(char *path);
  void WriteMLRPredictionInfo(char *path);
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(const QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setYVarName(const QStringList &varname_){ yvarname = varname_; }
  QStringList &getYVarName(){ return yvarname; }
  void setPredID(int id_){ id = id_; }
  int getPredID(){ return id; }
  void setDID(int id){ did = id; }
  int getDID(){ return did; }
  void setDataHash(QString hash_){ hash = hash_; }
  QString &getDataHash(){ return hash; }
  matrix *getYDipVar(){ return py; }
  dvector *getR2Y(){ return r2y; }
  dvector *getSDEC(){ return sdec; }
private:
  QStringList objname, yvarname;
  QString name;
  QString hash;
  int id;
  int did;
  matrix *py;
  dvector *r2y;
  dvector *sdec;
};

class MLRModel
{
public:
  MLRModel();
  ~MLRModel();
  MLRMODEL *Model(){ return m; }
  void ImportMLRModel(char *path, char *name_);
  void WriteMLRModel(char *path, char *dirname);
  void ImportMLRModelInfo(char *path);
  void WriteMLRModelInfo(char *path);
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(const QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setXVarName(const QStringList &varname_){ xvarname = varname_; }
  QStringList &getXVarName(){ return xvarname; }
  void setYVarName(const QStringList &varname_){ yvarname = varname_; }
  QStringList &getYVarName(){ return yvarname; }
  void setValidation(int v){ validation = v; }
  int getValidation(){ return validation; }
  void setDID(int did_){ did = did_; }
  void setDataHash(QString hash_){ hash = hash_; }
  void setModelID(int modelid_){ modelid = modelid_; }
  int getDID(){ return did; }
  QString &getDataHash(){ return hash; } // used in order to get the identity matrix provenience usefull for pls cross validation
  int getModelID(){ return modelid; }
  void addMLRPrediction(){ prediction.append(new MLRPREDICTION); };
  void delMLRPredictionAt(int id){ delete prediction[id]; prediction.removeAt(id); }
  void delMLRPredictions(){
    for(int i = 0; i < prediction.size(); i++){
      delete prediction[i];
    }
    prediction.clear();
  }
  MLRPREDICTION *getMLRPrediction(int id){ Q_ASSERT(id < prediction.size()); return prediction[id]; }
  MLRPREDICTION *getLastMLRPrediction(){ return prediction.last(); }
  int MLRPredictionCount(){ return prediction.size(); }
  QString& getHash(){ if(mlrhash.size() == 0){ mlrhash = GenHashFromStrlst((QStringList() << name << "mlrmodel_type")+objname+xvarname+yvarname); } return mlrhash; }

private:
  MLRMODEL *m;
  QList<MLRPREDICTION*> prediction;
  QStringList objname, xvarname, yvarname;
  QString name;
  int did, modelid, validation;
  QString hash, mlrhash;
};

#endif
