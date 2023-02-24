#ifndef CPCAMODEL_H
#define CPCAMODEL_H

#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <cmath>
#include "../qstudiometricsdataoperations.h"

class CPCAPREDICTION
{
public:
  CPCAPREDICTION();
  ~CPCAPREDICTION();

  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(const QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setPredID(int id_){ id = id_; }
  int getPredID(){ return id; }
  void setDID(int id){ did = id; }
  int getDID(){ return did; }
  void setDataHash(QString hash_){ hash = hash_; }
  QString &getDataHash(){ return hash; }
  matrix *getPredSuperScores(){ return p_super_scores; }
  tensor *getPredBlockScores(){ return p_block_scores; }
private:
  QStringList objname;
  QString name;
  QString hash;
  int id, did;
  matrix *p_super_scores;
  tensor *p_block_scores;
};

class CPCAModel
{
public:
  CPCAModel();
  ~CPCAModel();
  CPCAMODEL *Model(){ return m; }
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(const QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setVarName(const LABELS &varname_){ varname = varname_; }
  LABELS &getVarName(){ return varname; }
  void setDID(int did_){ did = did_; }
  void setDataHash(QString hash_){ hash = hash_; }
  QString &getDataHash(){ return hash; }
  void setXScaling(int xscaling_){ xscaling = xscaling_; }
  void setNPC(int npc_){ npc = npc_; }
  void setModelID(int modelid_){ modelid = modelid_; }
  int getDID(){ return did; }
  int getXScaling(){ return xscaling; }
  int getNPC(){ return npc; }
  int getModelID(){ return modelid; }
  void addCPCAPrediction(){ prediction.append(new CPCAPREDICTION); };
  void delCPCAPredictionAt(int id){ delete prediction[id]; prediction.removeAt(id); }
  void delCPCAPredictions(){
    for(int i = 0; i < prediction.size(); i++){
      delete prediction[i];
    }
    prediction.clear();
  }
  CPCAPREDICTION *getCPCAPrediction(int id){ Q_ASSERT(id < prediction.size()); return prediction[id]; }
  CPCAPREDICTION *getLastCPCAPrediction(){ return prediction.last(); }
  auto CPCAPredictionCount(){ return prediction.size(); }

  QString& getHash(){
    if(cpcahash.size() == 0){
      LABEL v;
      foreach(v, varname)
        cpcahash = GenHashFromStrlst((QStringList() << name << "cpcamodel_type")+objname+v.objects);
    }
    return cpcahash;

  }


private:
  CPCAMODEL *m;
  QList<CPCAPREDICTION*> prediction;
  QStringList objname;
  LABELS varname;
  QString name;
  int did, xscaling, npc, modelid;
  QString hash, cpcahash;
};

#endif
