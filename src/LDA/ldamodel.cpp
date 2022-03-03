#include "ldamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#ifdef DEBUG
#include <QDebug>
#endif

#include <QTextStream>
#include <QFile>
#include <QString>

void LDAPREDICTION::ImportLDAPredictionInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  size_t row = 0;
    QTextStream in(&file);
    while(!in.atEnd()){
    QString line = in.readLine();
    if(line.contains("#") == true || line.isEmpty()){ // skip line
      continue;
    }
    else{
      if(row == 0){
        hash = line.split("\t")[1];
      }
      else{
        continue;
      }
      row++;
    }
  }
  file.close();
}

void LDAPREDICTION::WriteLDAPredictionInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
    out << "PREDICTION ARRAY HASH\t" << hash << Qt::endl;
  file.close();
}

void LDAPREDICTION::WriteLDAPrediction(char *path, char* dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString prediction = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::MakeDir(prediction.toUtf8().data());

  for(int i = 0; i < classes.size(); i++){
    DATA::WriteList(classes[i], QString("%1/Class-%2.txt").arg(QString::fromUtf8(prediction.toUtf8())).arg(nameclasses[i]));
  }

  DATAIO::WriteUIvector(QString(QString("%1/PredClass.txt").arg(QString::fromUtf8(prediction.toUtf8()))).toUtf8().data(), pclass);
  DATAIO::WriteMatrix(QString(QString::fromUtf8(prediction.toUtf8())+"/PredFeatures.txt").toUtf8().data(), pfeatures);
  DATAIO::WriteMatrix(QString(QString::fromUtf8(prediction.toUtf8())+"/Probability.txt").toUtf8().data(), probpred);
  DATA::WriteList(getObjName(), QString::fromUtf8(prediction.toUtf8())+"/label.txt");

  WriteLDAPredictionInfo(prediction.toUtf8().data());
}

void LDAPREDICTION::ImportLDAPrediction(char *path, char *name_)
{
  name = name_;
  ImportLDAPredictionInfo(path);

  QDir ldadir = QDir(path);
  QStringList ldalst = ldadir.entryList();
  ldalst.removeAll(".");
  ldalst.removeAll("..");

  for(int i = 0; i < ldalst.size(); i++){
    if(ldalst[i].contains("Class") == true){
      nameclasses.append(ldalst[i].split("-").last().replace(".txt", ""));
      classes.append(QStringList());
      DATA::ImportRows(QString("%1/%2").arg(QString::fromUtf8(path)).arg(nameclasses.last()), classes.last());
    }
    else{
      continue;
    }
   }

  DATAIO::ImportUIvector(QString(QString("%1/PredClass.txt").arg(QString::fromUtf8(path))).toUtf8().data(), pclass);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/PredFeatures.txt").toUtf8().data(), std::string("\t").c_str(), pfeatures);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/Probability.txt").toUtf8().data(), std::string("\t").c_str(), probpred);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
}

LDAPREDICTION::LDAPREDICTION()
{
  initUIVector(&pclass);
  initMatrix(&pfeatures);
  initMatrix(&probpred);
  initMatrix(&mnpdf);
}

LDAPREDICTION::~LDAPREDICTION(){
  #ifdef DEBUG
  qDebug() << "Delete LDAPREDICTION: " << name;
  #endif
  DelUIVector(&pclass);
  DelMatrix(&pfeatures);
  DelMatrix(&probpred);
  DelMatrix(&mnpdf);
}

void LDAModel::ImportLDAModelInfo(char *path)
{
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  size_t row = 0;
  QTextStream in(&file);
  while(!in.atEnd()){
    QString line = in.readLine();
    if(line.contains("#") == true || line.isEmpty()){ // skip line
      continue;
    }
    else{
      if(row == 0){
        hash = line.split("\t")[1];
      }
      else if(row == 1){
        validation = line.split("\t")[1].toInt();
      }
      else{
        continue;
      }
      row++;
    }
  }
  file.close();
}

void LDAModel::WriteLDAModelInfo(char *path)
{
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "INPUT MATRIX HASH\t" << hash << Qt::endl;
  out << "VALIDATION\t" << validation << Qt::endl;
  file.close();
}


void LDAModel::WriteLDAModel(char *path, char *dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString modelpath = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::WriteLDAModel(modelpath.toUtf8().data(), Model());
  DATA::WriteList(getObjName(), QString::fromUtf8(modelpath.toUtf8())+"/label.txt");
  DATA::WriteList(getVarName(), QString::fromUtf8(modelpath.toUtf8())+"/varname.txt");

  for(int i = 0; i < classes.size(); i++){
    DATA::WriteList(classes[i], QString("%1/Class-%2.txt").arg(QString::fromUtf8(modelpath.toUtf8())).arg(nameclasses[i]));
  }

//   DATA::WriteList(nameclasses, QString("%1/NameClasses.txt").arg(QString::fromUtf8(modelpath.toUtf8())));

  WriteLDAModelInfo(modelpath.toUtf8().data());
}

void LDAModel::ImportLDAModel(char *path, char *name_)
{
  name = name_;
  ImportLDAModelInfo(path);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
  DATA::ImportRows(QString::fromUtf8(path)+"/varname.txt", varname);


  QDir ldadir = QDir(path);
  QStringList ldalst = ldadir.entryList();
  ldalst.removeAll(".");
  ldalst.removeAll("..");

  #ifdef DEBUG
  qDebug() << ldalst;
  #endif
  for(int i = 0; i < ldalst.size(); i++){
    if(ldalst[i].contains("Class") == true){
      QString nclass = ldalst[i].split("-").last().replace(".txt", "");
      nameclasses.append(nclass);
      classes.append(QStringList());
      DATA::ImportRows(QString("%1/%2").arg(QString::fromUtf8(path)).arg(nclass), classes.last());
    }
    else{
      continue;
    }
  }

  DATAIO::ImportLDAModel(path, m);
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
