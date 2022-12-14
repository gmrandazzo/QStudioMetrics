#include "ldamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#ifdef DEBUG
#include <QDebug>
#endif

#include <QTextStream>
#include <QFile>
#include <QString>

LDAPREDICTION::LDAPREDICTION()
{
  initMatrix(&pclass);
  initMatrix(&pfeatures);
  initMatrix(&probpred);
  initMatrix(&mnpdf);
}

LDAPREDICTION::~LDAPREDICTION(){
  #ifdef DEBUG
  qDebug() << "Delete LDAPREDICTION: " << name;
  #endif
  DelMatrix(&pclass);
  DelMatrix(&pfeatures);
  DelMatrix(&probpred);
  DelMatrix(&mnpdf);
}

LDAModel::LDAModel(){
  NewLDAModel(&m);
  did = modelid = -1;
  validation = 0;
}

LDAModel::~LDAModel()
{
  #ifdef DEBUG
  qDebug() << "Delete LDA Model: " << name;
  #endif
  delLDAPredictions();
  DelLDAModel(&m);
}
