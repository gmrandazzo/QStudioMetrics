#ifndef LDAMODEL_H
#define LDAMODEL_H

#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>


class LDAPREDICTION
{
public:
  LDAPREDICTION();
  ~LDAPREDICTION();
  void ImportLDAPrediction(char *path, char *name_);
  void WriteLDAPrediction(char *path, char *dirname_);
  void ImportLDAPredictionInfo(char *path);
  void WriteLDAPredictionInfo(char *path);
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setClasses(QList < QStringList >  &classes_){ classes = classes_; }
  void setNameClasses(QStringList nameclasses_){ nameclasses = nameclasses_; }
  QList < QStringList >  &getClasses(){ return classes; }
  QStringList &getNameClasses(){ return nameclasses; }
  uivector *getPredClasses(){ return pclass; }
  void setPredID(int id_){ id = id_; }
  int getPredID(){ return id; }
  void setDID(int id){ did = id; }
  int getDID(){ return did; }
  void setDataHash(QString hash_){ hash = hash_; }
  QString &getDataHash(){ return hash; }
  matrix *getPredFeatures(){ return pfeatures; }
  matrix *getProbPred(){ return probpred; }
  matrix *getMVNProbDistrib(){ return mvnpfd; }
private:
  QStringList objname;
  QList < QStringList > classes;
  QStringList nameclasses;
  QString name;
  QString hash;
  int id;
  int did;
  matrix *pfeatures;
  matrix *probpred;
  matrix *mvnpfd;
  uivector *pclass;
};

class LDAModel
{
public:
  LDAModel();
  ~LDAModel();
  LDAMODEL *Model(){ return m; }
  void ImportLDAModel(char *path, char *name_);
  void WriteLDAModel(char *path, char *dirname);
  void ImportLDAModelInfo(char *path);
  void WriteLDAModelInfo(char *path);
  void setName(QString name_){ name = name_; }
  QString &getName(){ return name; }
  void setObjName(QStringList &objname_){ objname = objname_; }
  QStringList &getObjName(){ return objname; }
  void setVarName(QStringList &varname_){ varname = varname_; }
  void setClasses(QList < QStringList > classes_){ classes = classes_; }
  void setNameClasses(QStringList nameclasses_){ nameclasses = nameclasses_; }
  QStringList &getVarName(){ return varname; }
  QList < QStringList > getClasses(){ return classes; }
  QStringList &getNameClasses(){ return nameclasses; }
  void setValidation(int v){ validation = v; }
  int getValidation(){ return validation; }
  void setDID(int did_){ did = did_; }
  void setDataHash(QString hash_){ hash = hash_; }
  void setModelID(int modelid_){ modelid = modelid_; }
  int getDID(){ return did; }
  QString &getDataHash(){ return hash; } // used in order to get the identity matrix provenience usefull for pls cross validation
  int getModelID(){ return modelid; }
  void addLDAPrediction(){ prediction.append(new LDAPREDICTION); };
  void delLDAPredictionAt(int id){ delete prediction[id]; prediction.removeAt(id); }
  void delLDAPredictions(){
    for(int i = 0; i < prediction.size(); i++){
      delete prediction[i];
    }
    prediction.clear();
  }
  LDAPREDICTION *getLDAPrediction(int id){ Q_ASSERT(id < prediction.size()); return prediction[id]; }
  LDAPREDICTION *getLastLDAPrediction(){ return prediction.last(); }
  int LDAPredictionCount(){ return prediction.size(); }

private:
  LDAMODEL *m;
  QList<LDAPREDICTION*> prediction;
  QStringList objname, varname;
  QList < QStringList > classes;
  QStringList nameclasses;
  QString name;
  int did, modelid, validation;
  QString hash;
};

#endif
