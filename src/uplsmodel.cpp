#include "uplsmodel.h"
#include "DATAIO.h"
#include "data.h"

#include <QDebug>

void UPLSPREDICTION::ImportUPLSPredictionInfo(char *path){
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
        xhash = line.split("\t")[1];
      }
      else if(row == 1){
        yhash = line.split("\t")[1];
      }
      else{
        continue;
      }
      row++;
    }
  }
  file.close();
}

void UPLSPREDICTION::WriteUPLSPredictionInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
    out << "PREDICTION X ARRAY HASH\t" << xhash << endl;
    out << "PREDICTION Y ARRAY HASH\t" << yhash << endl;
  file.close();
}

void UPLSPREDICTION::WriteUPLSPrediction(char *path, char* dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString prediction = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::MakeDir(prediction.toUtf8().data());
  DATAIO::WriteMatrix(QString(QString::fromUtf8(prediction.toUtf8())+"/T-Scores-Pred.txt").toUtf8().data(), pxscores);
  DATAIO::WriteArray(QString(QString::fromUtf8(prediction.toUtf8())+"/Y-Pred.txt").toUtf8().data(), py);
  DATAIO::WriteArray(QString(QString::fromUtf8(prediction.toUtf8())+"/R2Y.txt").toUtf8().data(), r2y);
  DATAIO::WriteArray(QString(QString::fromUtf8(prediction.toUtf8())+"/SDEC.txt").toUtf8().data(), sdec);
  DATA::WriteList(getObjName(), QString::fromUtf8(prediction.toUtf8())+"/label.txt");
  WriteUPLSPredictionInfo(prediction.toUtf8().data());
}

void UPLSPREDICTION::ImportUPLSPrediction(char *path, char *name_)
{
  name = name_;
  ImportUPLSPredictionInfo(path);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/T-Scores-Pred.txt").toUtf8().data(), std::string("\t").c_str(), pxscores);
  DATAIO::ImportArray(QString(QString::fromUtf8(path)+"/Y-Pred.txt").toUtf8().data(), std::string("\t").c_str(), py);
  DATAIO::ImportArray(QString(QString::fromUtf8(path)+"/R2Y.txt").toUtf8().data(), std::string("\t").c_str(), r2y);
  DATAIO::ImportArray(QString(QString::fromUtf8(path)+"/SDEC.txt").toUtf8().data(), std::string("\t").c_str(), sdec);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
}

UPLSPREDICTION::UPLSPREDICTION()
{
  initMatrix(&pxscores);
  initArray(&py);
  initArray(&r2y);
  initArray(&sdec);
}

UPLSPREDICTION::~UPLSPREDICTION(){
  qDebug() << "Delete UPLSPREDICTION: " << name;
  DelMatrix(&pxscores);
  DelArray(&py);
  DelArray(&r2y);
  DelArray(&sdec);
}

void UPLSModel::ImportUPLSModelInfo(char *path)
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
        yscaling = line.split("\t")[1].toInt();
      }
      else if(row == 3){
        xhash = line.split("\t")[1];
      }
      else if(row == 4){
        yhash = line.split("\t")[1];
      }
      else if(row == 5){
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

void UPLSModel::WriteUPLSModelInfo(char *path)
{
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "NUMBER OF COMPONENT\t" << npc << endl;
  out << "XSCALINGTYPE\t" << xscaling << endl;
  out << "YSCALINGTYPE\t" << yscaling << endl;
  out << "X INPUT ARRAY HASH\t" << xhash << endl;
  out << "Y INPUT ARRAY HASH\t" << yhash << endl;
  out << "VALIDATION\t" << validation << endl;
  file.close();
}


void UPLSModel::WriteUPLSModel(char *path, char *dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString modelpath = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::WriteUPLSModel(modelpath.toUtf8().data(), Model());
  DATA::WriteList(getObjName(), QString::fromUtf8(modelpath.toUtf8())+"/label.txt");
  DATA::WriteList(getXVarName(), QString::fromUtf8(modelpath.toUtf8())+"/xvarname.txt");
  DATA::WriteList(getYVarName(), QString::fromUtf8(modelpath.toUtf8())+"/yvarname.txt");

  WriteUPLSModelInfo(modelpath.toUtf8().data());
}

void UPLSModel::ImportUPLSModel(char *path, char *name_)
{
  name = name_;
  ImportUPLSModelInfo(path);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
  DATA::ImportRows(QString::fromUtf8(path)+"/xvarname.txt", xvarname);
  DATA::ImportRows(QString::fromUtf8(path)+"/yvarname.txt", yvarname);
  DATAIO::ImportUPLSModel(path, m);
}

UPLSModel::UPLSModel(){
  NewUPLSModel(&m);
  xid = yid = xscaling = yscaling = npc = modelid = -1;
  validation = 0;
}

UPLSModel::~UPLSModel()
{
  qDebug() << "Delete UPLS Model: " << name;
  delUPLSPredictions();
  DelUPLSModel(&m);
}
