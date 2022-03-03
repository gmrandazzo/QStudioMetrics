#include "upcamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

void UPCAPREDICTION::ImportUPCAPredictionInfo(char *path){
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

void UPCAPREDICTION::WriteUPCAPredictionInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "PREDICTION ARRAY HASH\t" << hash << endl;
  file.close();
}

void UPCAPREDICTION::WriteUPCAPrediction(char *path, char *dirname)
{

  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString predpath = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::MakeDir(predpath.toUtf8().data());

  DATAIO::WriteMatrix(QString(QString::fromUtf8(predpath.toUtf8())+"/T-Scores-Pred.txt").toUtf8().data(), getPredScores());
  DATA::WriteList(getObjName(), QString::fromUtf8(predpath.toUtf8())+"/label.txt");
  WriteUPCAPredictionInfo(predpath.toUtf8().data());
}


void UPCAPREDICTION::ImportUPCAPrediction(char *path, char *name_)
{
  name = QString::fromUtf8(name_);
  ImportUPCAPredictionInfo(path);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/T-Scores-Pred.txt").toUtf8().data(), std::string("\t").c_str(), pscores);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
}


UPCAPREDICTION::UPCAPREDICTION(){
  initMatrix(&pscores);
}

UPCAPREDICTION::~UPCAPREDICTION()
{
  #ifdef DEBUG
  qDebug() << "Delete UPCAPREDICTION: " << name;
  #endif
  DelMatrix(&pscores);
}


void UPCAModel::ImportUPCAModelInfo(char *path){
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
        npc = line.split("\t")[1].toInt();
      }
      else if(row == 1){
        xscaling = line.split("\t")[1].toInt();
      }
      else if(row == 2){
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

void UPCAModel::WriteUPCAModelInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "NUMBER OF COMPONENT\t" << npc << endl;
  out << "SCALINGTYPE\t" << xscaling << endl;
  out << "INPUT ARRAY HASH\t" << hash << endl;
  file.close();
}


void UPCAModel::WriteUPCAModel(char *path, char *dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString modelpath = QString::fromUtf8(path)+"/"+QString::fromUtf8(name.toUtf8());
  DATAIO::WriteUPCAModel(modelpath.toUtf8().data(), Model());
  DATA::WriteList(getObjName(), QString("%1/label.txt").arg(QString::fromUtf8(modelpath.toUtf8())));
  DATA::WriteList(getVarName(), QString("%1/varname.txt").arg(QString::fromUtf8(modelpath.toUtf8())));
  WriteUPCAModelInfo(modelpath.toUtf8().data());
}


void UPCAModel::ImportUPCAModel(char *path, char *name_)
{
  name = name_;
  ImportUPCAModelInfo(path);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
  DATA::ImportRows(QString::fromUtf8(path)+"/varname.txt", varname);
  DATAIO::ImportUPCAModel(path, m);
}

UPCAModel::UPCAModel()
{
  NewUPCAModel(&m);
  did = xscaling = npc = modelid = -1;
}

UPCAModel::~UPCAModel()
{
  #ifdef DEBUG
  qDebug() << "Delete UPCA Model: " << name;
  #endif
  delUPCAPredictions();
  DelUPCAModel(&m);
}
