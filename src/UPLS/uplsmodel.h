#ifndef UPLSMODEL_H
#define UPLSMODEL_H

#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>

class UPLSPREDICTION
{
public:
  UPLSPREDICTION();
  ~UPLSPREDICTION();
  void ImportUPLSPrediction(char *path, char *name_);
  void WriteUPLSPrediction(char *path, char *dirname_);
  void ImportUPLSPredictionInfo(char *path);
  void WriteUPLSPredictionInfo(char *path);
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setPredID(int id_){ id = id_; }
  int getPredID(){ return id; }
  void setXID(int id){ xid = id; }
  void setYID(int id){ yid = id; }
  int getXID(){ return xid; }
  int getYID(){ return yid; }
  void setXHash(QString hash_){ xhash = hash_; }
  void setYHash(QString hash_){ yhash = hash_; }
  QString &getXHash(){ return xhash; }
  QString &getYHash(){ return yhash; }
  matrix *getXPredScores(){ return pxscores; }
  array *getYDipVar(){ return py; }
  array *getR2Y(){ return r2y; }
  array *getSDEC(){ return sdec; }
private:
  QStringList objname;
  QString name;
  QString xhash, yhash;
  int id;
  int xid, yid;
  matrix *pxscores;
  array *py;
  array *r2y;
  array *sdec;
};

class UPLSModel
{
public:
  UPLSModel();
  ~UPLSModel();
  UPLSMODEL *Model(){ return m; }
  void ImportUPLSModel(char *path, char *name_);
  void WriteUPLSModel(char *path, char *dirname);
  void ImportUPLSModelInfo(char *path);
  void WriteUPLSModelInfo(char *path);
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setXVarName(QStringList &varname_){ xvarname = varname_; }
  QStringList &getXVarName(){ return xvarname; }
  void setYVarName(QStringList &varname_){ yvarname = varname_; }
  QStringList &getYVarName(){ return yvarname; }
  void setValidation(int v){ validation = v; }
  int getValidation(){ return validation; }
  void setXID(int xid_){ xid = xid_; }
  void setYID(int yid_){ yid = yid_; }
  void setXHash(QString xhash_){ xhash = xhash_; }
  void setYHash(QString yhash_){ yhash = yhash_; } 
  void setXScaling(int xscaling_){ xscaling = xscaling_; }
  void setYScaling(int yscaling_){ yscaling = yscaling_; }
  void setNPC(int npc_){ npc = npc_; }
  void setModelID(int modelid_){ modelid = modelid_; }
  int getXID(){ return xid; }
  int getYID(){ return yid; }
  QString &getXHash(){ return xhash; } // used in order to get the identity matrix provenience usefull for pls cross validation 
  QString &getYHash(){ return yhash; } // used in order to get the identity matrix provenience usefull for pls cross validation 
  int getXScaling(){ return xscaling; }
  int getYScaling(){ return xscaling; }
  int getNPC(){ return npc; }
  int getModelID(){ return modelid; }
  void addUPLSPrediction(){ prediction.append(new UPLSPREDICTION); };
  void delUPLSPredictionAt(int id){ delete prediction[id]; prediction.removeAt(id); }
  void delUPLSPredictions(){ 
    for(int i = 0; i < prediction.size(); i++){
      delete prediction[i];
    }
    prediction.clear(); 
  }
  UPLSPREDICTION *getUPLSPrediction(int id){ Q_ASSERT(id < prediction.size()); return prediction[id]; }
  UPLSPREDICTION *getLastUPLSPrediction(){ return prediction.last(); }
  int UPLSPredictionCount(){ return prediction.size(); }
  
private:
  UPLSMODEL *m;
  QList<UPLSPREDICTION*> prediction;
  QStringList objname, xvarname, yvarname;
  QString name;
  int xid, yid, xscaling, yscaling, npc, modelid, validation;
  QString xhash, yhash;
};

#endif
