#ifndef UPCAMODEL_H
#define UPCAMODEL_H

#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>

class UPCAPREDICTION
{
public:
  UPCAPREDICTION();
  ~UPCAPREDICTION();
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setPredID(int id_){ id = id_; }
  int getPredID(){ return id; }
  void setDID(int id){ did = id; }
  int getDID(){ return did; }
  void setDataHash(QString hash_){ hash = hash_; }
  QString &getDataHash(){ return hash; }
  matrix *getPredScores(){ return pscores; }
private:
  QStringList objname;
  QString name;
  QString hash;
  int id;
  int did;
  matrix *pscores;
};

class UPCAModel
{
public:
  UPCAModel();
  ~UPCAModel();
  UPCAMODEL *Model(){ return m; }
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setVarName(QStringList &varname_){ varname = varname_; }
  QStringList &getVarName(){ return varname; }
  void setDID(int did_){ did = did_; }
  void setDataHash(QString hash_){ hash = hash_; }
  void setXScaling(int xscaling_){ xscaling = xscaling_; }
  void setNPC(int npc_){ npc = npc_; }
  void setModelID(int modelid_){ modelid = modelid_; }
  int getDID(){ return did; }
  QString &getDataHash(){ return hash; }
  int getXScaling(){ return xscaling; }
  int getNPC(){ return npc; }
  int getModelID(){ return modelid; }
  void addUPCAPrediction(){ prediction.append(new UPCAPREDICTION); };
  void delUPCAPredictionAt(int id){ delete prediction[id]; prediction.removeAt(id); }
  void delUPCAPredictions(){
    for(int i = 0; i < prediction.size(); i++){
      delete prediction[i];
    }
    prediction.clear();
  }
  UPCAPREDICTION *getUPCAPrediction(int id){ Q_ASSERT(id < prediction.size()); return prediction[id]; }
  UPCAPREDICTION *getLastUPCAPrediction(){ return prediction.last(); }
  int UPCAPredictionCount(){ return prediction.size(); }

private:
  UPCAMODEL *m;
  QList<UPCAPREDICTION*> prediction;
  QStringList objname, varname;
  QString name;
  int did, xscaling, npc, modelid;
  QString hash;
};

#endif
