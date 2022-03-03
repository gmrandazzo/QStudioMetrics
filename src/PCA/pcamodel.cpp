#include "pcamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

void PCAPREDICTION::ImportPCAPredictionInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
    return;
  }

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

void PCAPREDICTION::WritePCAPredictionInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "PREDICTION MATRIX HASH\t" << hash << Qt::endl;
  file.close();
}

void PCAPREDICTION::WritePCAPrediction(char *path, char *dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString predpath = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::MakeDir(predpath.toUtf8().data());

  DATAIO::WriteMatrix(QString(QString::fromUtf8(predpath.toUtf8())+"/T-Scores-Pred.txt").toUtf8().data(), getPredScores());
  DATA::WriteList(getObjName(), QString::fromUtf8(predpath.toUtf8())+"/label.txt");
  WritePCAPredictionInfo(predpath.toUtf8().data());
}


void PCAPREDICTION::ImportPCAPrediction(char *path, char *name_)
{
  name = name_;
  ImportPCAPredictionInfo(path);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/T-Scores-Pred.txt").toUtf8().data(), std::string("\t").c_str(), pscores);
  DATA::ImportRows(QString("%1/label.txt").arg(QString::fromUtf8(path)), objname);
}


PCAPREDICTION::PCAPREDICTION()
{
  initMatrix(&pscores);
}

PCAPREDICTION::~PCAPREDICTION()
{
  #ifdef DEBUG
  qDebug() << "Delete PCAPREDICTION: " << name;
  #endif
  DelMatrix(&pscores);
}

void PCAModel::ImportPCAModelInfo(char *path)
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

void PCAModel::WritePCAModelInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "NUMBER OF COMPONENT\t" << npc << Qt::endl;
  out << "SCALINGTYPE\t" << xscaling << Qt::endl;
  out << "INPUT MATRIX HASH\t" << hash << Qt::endl;
  file.close();
}

void PCAModel::WritePCAModel(char *path, char *name)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString modelpath = QString::fromUtf8(path)+"/"+QString::fromUtf8(name);

  DATAIO::WritePCAModel(modelpath.toUtf8().data(), Model());
  DATA::WriteList(getObjName(), QString::fromUtf8(modelpath.toUtf8())+"/label.txt");
  DATA::WriteList(getVarName(), QString::fromUtf8(modelpath.toUtf8())+"/varname.txt");

  WritePCAModelInfo(modelpath.toUtf8().data());
}

void PCAModel::ImportPCAModel(char *path, char *name_)
{
  name = name_;
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
  DATA::ImportRows(QString::fromUtf8(path)+"/varname.txt", varname);
  ImportPCAModelInfo(path);
  DATAIO::ImportPCAModel(path, m);
}

PCAModel::~PCAModel()
{
  #ifdef DEBUG
  qDebug() << "Delete PCA Model: " << name;
  #endif
  delPCAPredictions();
  DelPCAModel(&m);
}

PCAModel::PCAModel()
{
  NewPCAModel(&m);
  did = xscaling = npc = modelid = -1;
}
