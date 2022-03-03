#ifndef PLSMODEL_H
#define PLSMODEL_H
#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>

#include "qstudiometricstypes.h"
#include "qstudiometricsdataoperations.h"

class PLSPREDICTION
{
public:
  PLSPREDICTION();
  ~PLSPREDICTION();
  void ImportPLSPrediction(char *path, char *name_);
  void WritePLSPrediction(char *path, char *dirname);
  void ImportPLSPredictionInfo(char *path);
  void WritePLSPredictionInfo(char *path);
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  void setYVarName(QStringList &yvarname_){ yvarname = yvarname_; }
  QStringList &getObjName(){ return objname; }
  QStringList &getYVarName(){ return yvarname; }
  void setPredID(int id_){ id = id_; }
  int getPredID(){ return id; }
  void setDID(int id){ did = id; }
  int getDID(){ return did; }
  void setDataHash(QString hash_){ hash = hash_; }
  QString &getDataHash(){ return hash; }
  matrix **XPredScoresPointer(){ return &pxscores; }
  matrix *getXPredScores(){ return pxscores; }
  matrix **YDipVarPointer(){ return &py; }
  matrix *getYDipVar(){ return py; }
  matrix **R2YPointer(){ return &r2y; }
  matrix *getR2Y(){ return r2y; }
  matrix **SDECPointer(){ return &sdec; }
  matrix *getSDEC(){ return sdec; }
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

class PLSModel
{
public:
  PLSModel();
   ~PLSModel();
  PLSMODEL *Model(){ return m; }
  void ImportPLSModel(char *path, char *name_);
  void WritePLSModel(char *path, char *dirname);
  void ImportPLSModelInfo(char *path);
  void WritePLSModelInfo(char *path);
  void setAlgorithm(int algtype_){ algtype = algtype_; }
  int getAlgorithm(){ return algtype; }
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setXVarName(QStringList &varname_){ xvarname = varname_; }
  QStringList &getXVarName(){ return xvarname; }
  void setYVarName(QStringList &varname_){ yvarname = varname_; }
  QStringList &getYVarName(){if(algtype == PLS_){return yvarname;}else{return cnames;} }
  void setClasses(LABELS classes_){ classes = classes_; cnames.clear(); for(int i = 0; i < classes.size(); i++){cnames << classes[i].name;} }
  LABELS getClasses(){ return classes; }
  void setValidation(int v){ validation = v; }
  int getValidation(){ return validation; }
  void setDID(int did_){ did = did_; }
  void setDataHash(QString hash_){ hash = hash_; }
  void setXScaling(int xscaling_){ xscaling = xscaling_; } // used in order to set the identity matrix provenience usefull for pls cross validation
  void setYScaling(int yscaling_){ yscaling = yscaling_; } // used in order to set the identity matrix provenience usefull for pls cross validation
  void setNPC(int npc_){ npc = npc_; }
  int getNPC(){ return npc; }
  void setModelID(int modelid_){ modelid = modelid_; }
  int getDID(){ return did; }
  QString &getDataHash(){ return hash; } // used in order to get the identity matrix provenience usefull for pls cross validation
  int getXScaling(){ return xscaling; }
  int getYScaling(){ return yscaling; }
  int getModelID(){ return modelid; }
  void addPLSPrediction(){ prediction.append(new PLSPREDICTION); };
  void delPLSPredictionAt(int id){ delete prediction[id]; prediction.removeAt(id); }
  void delPLSPredictions(){
    for(int i = 0; i < prediction.size(); i++){
      delete prediction[i];
    }
    prediction.clear();
  }

  PLSPREDICTION *getPLSPrediction(int id){ Q_ASSERT(id < prediction.size()); return prediction[id]; }
  PLSPREDICTION *getLastPLSPrediction(){ return prediction.last(); }
  int PLSPredictionCount(){ return prediction.size(); }

  QString& getHash(){ if(plshash.size() == 0){ plshash = GenMatrixHash(m->xscores); } return plshash; }

private:
  PLSMODEL *m;
  QList<PLSPREDICTION*> prediction;
  QStringList objname, xvarname, yvarname, cnames;
  QString name;
  LABELS classes;
  int did, xscaling, yscaling, npc, modelid, validation, algtype;
  QString hash, plshash;
};

#endif
