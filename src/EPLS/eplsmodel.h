#ifndef EPLSMODEL_H
#define EPLSMODEL_H
#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"
#include "qstudiometricsdataoperations.h"

class EPLSPREDICTION
{
public:
  EPLSPREDICTION();
  ~EPLSPREDICTION();
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
  tensor *pxscores;
  matrix *py;
  matrix *r2;
  matrix *sdec;
  matrix *bias;
  tensor *roc;
  matrix *roc_auc;
  tensor *precision_recall;
  matrix *precision_recall_ap;
private:
  QStringList objname, yvarname;
  QString name;
  QString hash;
  int id;
  int did;
};

class EPLSModel
{
public:
  EPLSModel();
  ~EPLSModel();
  EPLSMODEL *Model(){ return m; }

  /*Regression*/
  matrix *r2;
  matrix *sdec;
  matrix *q2;
  matrix *sdep;
  matrix *bias;
  /*Discriminant Analysis*/
  tensor *roc_recalculated;
  matrix *roc_auc_recalculated;
  tensor *precision_recall_recalculated;
  matrix *precision_recall_ap_recalculated;
  tensor *roc_predicted;
  matrix *roc_auc_predicted;
  tensor *precision_recall_predicted;
  matrix *precision_recall_ap_predicted;

  matrix *yscrambling;

  matrix *y_recalculated;
  matrix *y_recalculated_residuals;
  matrix *y_predicted;
  matrix *y_predicted_residuals;

  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setXVarName(QStringList &varname_){ xvarname = varname_; }
  QStringList &getXVarName(){ return xvarname; }
  void setYVarName(QStringList &varname_){ yvarname = varname_; }
  void setClasses(LABELS &classes_){ classes = classes_; cnames.clear(); for(int i = 0; i < classes.size(); i++){cnames << classes[i].name;}}
  LABELS getClasses(){ return classes; }
  QStringList &getYVarName(){if(algtype == EPLS_){return yvarname;}else{return cnames;} }
  void setAlgorithm(int algtype_){ algtype = algtype_; };
  int getAlgorithm(){ return algtype; }
  void setElearningParm(ELearningParameters eparm_) { eparm = eparm_; }
  ELearningParameters getElearningParm(){ return eparm; }
  void setCombinationRule(CombinationRule crule_) { crule = crule_; }
  CombinationRule getCombinationRule(){ return crule; }
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
  void addEPLSPrediction(){ prediction.append(new EPLSPREDICTION); };
  void delEPLSPredictionAt(int id){ delete prediction[id]; prediction.removeAt(id); }
  void delEPLSPredictions(){
    for(int i = 0; i < prediction.size(); i++){
      delete prediction[i];
    }
    prediction.clear();
  }

  EPLSPREDICTION *getEPLSPrediction(int id){ Q_ASSERT(id < prediction.size()); return prediction[id]; }
  EPLSPREDICTION *getLastEPLSPrediction(){ return prediction.last(); }
  int EPLSPredictionCount(){ return prediction.size(); }

  QString& getHash(){ if(eplshash.size() == 0){ eplshash = GenMatrixHash(m->models[0]->xscores); } return eplshash; }

private:
  EPLSMODEL *m;

  QList<EPLSPREDICTION*> prediction;
  QStringList objname, xvarname, yvarname, cnames;
  QString name;
  int did, xscaling, yscaling, npc, modelid, validation, algtype;
  LABELS classes;
  ELearningParameters eparm;
  CombinationRule crule;
  QString hash, eplshash;
};

#endif
