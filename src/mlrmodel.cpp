#include "mlrmodel.h"
#include "DATAIO.h"
#include "data.h"

#include <QDebug>

void MLRPREDICTION::ImportMLRPredictionInfo(char *path){
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

void MLRPREDICTION::WriteMLRPredictionInfo(char *path){
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
    out << "PREDICTION ARRAY HASH\t" << hash << endl;
  file.close();
}

void MLRPREDICTION::WriteMLRPrediction(char *path, char* dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString prediction = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::MakeDir(prediction.toUtf8().data());
  DATAIO::WriteMatrix(QString(QString::fromUtf8(prediction.toUtf8())+"/Y-Pred.txt").toUtf8().data(), py);
  DATAIO::WriteDvector(QString(QString::fromUtf8(prediction.toUtf8())+"/R2Y.txt").toUtf8().data(), r2y);
  DATAIO::WriteDvector(QString(QString::fromUtf8(prediction.toUtf8())+"/SDEC.txt").toUtf8().data(), sdec);
  DATA::WriteList(getObjName(), QString::fromUtf8(prediction.toUtf8())+"/label.txt");
  WriteMLRPredictionInfo(prediction.toUtf8().data());
}

void MLRPREDICTION::ImportMLRPrediction(char *path, char *name_)
{
  name = name_;
  ImportMLRPredictionInfo(path);
  DATAIO::ImportMatrix(QString(QString::fromUtf8(path)+"/Y-Pred.txt").toUtf8().data(), std::string("\t").c_str(), py);
  DATAIO::ImportDvector(QString(QString::fromUtf8(path)+"/R2Y.txt").toUtf8().data(), r2y);
  DATAIO::ImportDvector(QString(QString::fromUtf8(path)+"/SDEC.txt").toUtf8().data(), sdec);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
}

MLRPREDICTION::MLRPREDICTION()
{
  initMatrix(&py);
  initDVector(&r2y);
  initDVector(&sdec);
}

MLRPREDICTION::~MLRPREDICTION(){
  qDebug() << "Delete MLRPREDICTION: " << name;
  DelMatrix(&py);
  DelDVector(&r2y);
  DelDVector(&sdec);
}

void MLRModel::ImportMLRModelInfo(char *path)
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

void MLRModel::WriteMLRModelInfo(char *path)
{
  QFile file(QString::fromUtf8(path)+"/info.txt");
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  out << "INPUT MATRIX HASH\t" << hash << endl;
  out << "VALIDATION\t" << validation << endl;
  file.close();
}


void MLRModel::WriteMLRModel(char *path, char *dirname)
{
  if(DATAIO::DirExists(path) == false){
    DATAIO::MakeDir(path);
  }

  QString modelpath = QString::fromUtf8(path)+"/"+QString::fromUtf8(dirname);
  DATAIO::WriteMLRModel(modelpath.toUtf8().data(), Model());
  DATA::WriteList(getObjName(), QString::fromUtf8(modelpath.toUtf8())+"/label.txt");
  DATA::WriteList(getXVarName(), QString::fromUtf8(modelpath.toUtf8())+"/xvarname.txt");
  DATA::WriteList(getYVarName(), QString::fromUtf8(modelpath.toUtf8())+"/yvarname.txt");

  WriteMLRModelInfo(modelpath.toUtf8().data());
}

void MLRModel::ImportMLRModel(char *path, char *name_)
{
  name = name_;
  ImportMLRModelInfo(path);
  DATA::ImportRows(QString::fromUtf8(path)+"/label.txt", objname);
  DATA::ImportRows(QString::fromUtf8(path)+"/xvarname.txt", xvarname);
  DATA::ImportRows(QString::fromUtf8(path)+"/yvarname.txt", yvarname);
  DATAIO::ImportMLRModel(path, m);
}

MLRModel::MLRModel(){
  NewMLRModel(&m);
  did = modelid = -1;
  validation = 0;
}

MLRModel::~MLRModel()
{
  qDebug() << "Delete MLR Model: " << name;
  delMLRPredictions();
  DelMLRModel(&m);
}
