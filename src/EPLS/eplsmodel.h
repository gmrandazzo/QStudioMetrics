#ifndef EPLSMODEL_H
#define EPLSMODEL_H
#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>

#include "../qstudiometricsdataoperations.h"

class EPLSPrediction
{
public:
  EPLSPrediction();
  ~EPLSPrediction();
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
  array **XPredScoresPointer(){ return &pxscores; }
  array *getXPredScores(){ return pxscores; }
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
  array *pxscores;
  matrix *py;
  matrix *r2y;
  matrix *sdec;
};

class EPLSModel
{
public:
  EPLSModel();
   ~EPLSModel();
  EPLSMODEL *Model(){ return m; }
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setXVarName(QStringList &varname_){ xvarname = varname_; }
  QStringList &getXVarName(){ return xvarname; }
  void setYVarName(QStringList &varname_){ yvarname = varname_; }
  QStringList &getYVarName(){ return yvarname; }
  void setElearningParm(ELearningParameters eparm_) { eparm = eparm_; }
  ELearningParameters getElearningParm(){ return eparm; }
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
  void addEPLSPrediction(){ prediction.append(new EPLSPrediction); };
  void delEPLSPredictionAt(int id){ delete prediction[id]; prediction.removeAt(id); }
  void delEPLSPredictions(){
    for(int i = 0; i < prediction.size(); i++){
      delete prediction[i];
    }
    prediction.clear();
  }

  EPLSPrediction *getEPLSPrediction(int id){ Q_ASSERT(id < prediction.size()); return prediction[id]; }
  EPLSPrediction *getLastEPLSPrediction(){ return prediction.last(); }
  int EPLSPredictionCount(){ return prediction.size(); }

  QString& getHash(){ if(eplshash.size() == 0){ eplshash = GenMatrixHash(m->models[0]->xscores); } return eplshash; }

private:
  EPLSMODEL *m;
  QList<EPLSPrediction*> prediction;
  QStringList objname, xvarname, yvarname;
  QString name;
  int did, xscaling, yscaling, npc, modelid, validation;
  ELearningParameters eparm;
  QString hash, eplshash;
};

#endif
