#include "VariableSelectionModel.h"
#include "DATAIO.h"
#include "data.h"

void VariableSelectionModel::ImportVarSelModelInfo(QString path)
{
  QFile file(path+"/info.txt");
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
        varselalgo = line.trimmed();
      }
      else if(row == 6){
        varseloptions = line.trimmed();
      }
      else{
        continue;
      }
      row++;
    }
  }
  file.close();
}

void VariableSelectionModel::ImportVarSelModel(QString path, QString name_)
{
  varselname = name_;
  std::string sep = " \t";
  DATAIO::ImportMatrix(QString(path+"/map.txt").toUtf8().data(), sep, map);
  DATAIO::ImportUIvector(QString(path+"/selectedvars.txt").toUtf8().data(), selectedvariables);
  DATAIO::ImportUIvector(QString(path+"/vardistribution.txt").toUtf8().data(), vardistribution);
  DATA::ImportRows(path+"/varnames.txt", variablenames);
  ImportVarSelModelInfo(path);
}

void VariableSelectionModel::WriteVarSelModel(QString path)
{
  if(DATAIO::DirExists(path.toUtf8().data()) == false){
    DATAIO::MakeDir(path.toUtf8().data());
  }

  DATAIO::WriteMatrix(QString(path+"/map.txt").toUtf8().data(), getMap());
  DATAIO::WriteUIvector(QString(path+"/selectedvars.txt").toUtf8().data(), getSelectedVariables());
  DATAIO::WriteUIvector(QString(path+"/vardistribution.txt").toUtf8().data(), getVariableDistribution());
  DATA::WriteList(getVariableNames(), path+"/varnames.txt");

  QStringList info;
  info.append(QString("NUMBER OF COMPONENT\t%1").arg(QString::number(getNumberOfComponents())));
  info.append(QString("XSCALINGTYPE\t%1").arg(QString::number(getXScaling())));
  info.append(QString("YSCALINGTYPE\t%1").arg(QString::number(getYScaling())));
  info.append(QString("X INPUT MATRIX HASH\t%1").arg(getXHash()));
  info.append(QString("Y INPUT MATRIX HASH\t%1").arg(getYHash()));
  info.append(getVariableSelectionAlgorithm());
  info.append(getVariableSelectionAlgorithmOptions());
  DATA::WriteList(info, path+"/info.txt");
}

VariableSelectionModel::VariableSelectionModel()
{
  initMatrix(&map);
  initUIVector(&selectedvariables);
  initUIVector(&vardistribution);
}


VariableSelectionModel::~VariableSelectionModel()
{
  qDebug() << "Delete Variable Selection Model";
  DelMatrix(&map);
  DelUIVector(&selectedvariables);
  DelUIVector(&vardistribution);
}
