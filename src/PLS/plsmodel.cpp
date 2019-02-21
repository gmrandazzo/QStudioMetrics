#include "plsmodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

void PLSPREDICTION::ImportPLSPredictionInfo(char *path){
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

void PLSPREDICTION::WritePLSPredictionInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "PREDICTION DATA MATRIX HASH\t" << hash << endl;
  file.close();
}

void PLSPREDICTION::WritePLSPrediction(char *path, char *dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString prediction = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::MakeDir(prediction.toUtf8().data());
  DATAIO::WriteMatrix(QString(QString::fromUtf8(prediction.toUtf8())+"/T-Scores-Pred.txt").toUtf8().data(), pxscores);
  DATAIO::WriteMatrix(QString(QString::fromUtf8(prediction.toUtf8())+"/Y-Pred.txt").toUtf8().data(), py);
  DATAIO::WriteMatrix(QString(QString::fromUtf8(prediction.toUtf8())+"/R2Y.txt").toUtf8().data(), r2y);
  DATAIO::WriteMatrix(QString(QString::fromUtf8(prediction.toUtf8())+"/SDEC.txt").toUtf8().data(), sdec);
  DATA::WriteList(getObjName(), QString::fromUtf8(prediction.toUtf8())+"/label.txt");
  DATA::WriteList(getYVarName(), QString::fromUtf8(prediction.toUtf8())+"/yvarname.txt");
  WritePLSPredictionInfo(prediction.toUtf8().data());
}

void PLSPREDICTION::ImportPLSPrediction(char *path, char *name_)
{
  name = name_;
  ImportPLSPredictionInfo(path);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/T-Scores-Pred.txt").toUtf8().data(), std::string("\t").c_str(), pxscores);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/Y-Pred.txt").toUtf8().data(), std::string("\t").c_str(), py);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/R2Y.txt").toUtf8().data(), std::string("\t").c_str(), r2y);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/SDEC.txt").toUtf8().data(), std::string("\t").c_str(), sdec);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
  DATA::ImportRows(QString::fromUtf8(path)+"/yvarname.txt", yvarname);
}


PLSPREDICTION::PLSPREDICTION()
{
  initMatrix(&pxscores);
  initMatrix(&py);
  initMatrix(&r2y);
  initMatrix(&sdec);
}

PLSPREDICTION::~PLSPREDICTION()
{
  #ifdef DEBUG
  qDebug() << "Delete PLSPREDICTION: " << name;
  #endif

  DelMatrix(&pxscores);
  DelMatrix(&py);
  DelMatrix(&r2y);
  DelMatrix(&sdec);
}

void PLSModel::ImportPLSModelInfo(char *path){
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
        yscaling = line.split("\t")[1].toInt();
      }
      else if(row == 3){
        hash = line.split("\t")[1];
      }
      else if(row == 4){
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

void PLSModel::WritePLSModelInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "NUMBER OF COMPONENT\t" << npc << endl;
  out << "XSCALINGTYPE\t" << xscaling << endl;
  out << "YSCALINGTYPE\t" << yscaling << endl;
  out << "INPUT MATRIX HASH\t" << hash << endl;
  out << "VALIDATION\t" << validation << endl;
  file.close();
}

void PLSModel::WritePLSModel(char *path, char *dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString modelpath = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::WritePLSModel(modelpath.toUtf8().data(), Model());
  DATA::WriteList(getObjName(), QString::fromUtf8(modelpath.toUtf8())+"/label.txt");
  DATA::WriteList(getXVarName(), QString::fromUtf8(modelpath.toUtf8())+"/xvarname.txt");
  DATA::WriteList(getYVarName(), QString::fromUtf8(modelpath.toUtf8())+"/yvarname.txt");

  WritePLSModelInfo(modelpath.toUtf8().data());
}

void PLSModel::ImportPLSModel(char *path, char *name_)
{
  name = name_;
  ImportPLSModelInfo(path);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
  DATA::ImportRows(QString::fromUtf8(path)+"/xvarname.txt", xvarname);
  DATA::ImportRows(QString::fromUtf8(path)+"/yvarname.txt", yvarname);
  DATAIO::ImportPLSModel(path, m);
}

PLSModel::PLSModel()
{
  NewPLSModel(&m);
  did = xscaling = yscaling = npc = modelid = -1;
  validation = 0;
  algtype = PLS_;
}

PLSModel::~PLSModel()
{
  #ifdef DEBUG
  qDebug() << "Delete PLS Model: "<< name;
  #endif
  delPLSPredictions();
  DelPLSModel(&m);
}
