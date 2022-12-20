#include "ClusterModel.h"
#include <QFile>
#include <QTextStream>
#include "DATAIO.h"
#include "data.h"

void ClusterModel::ImportClusterModelInfo(QString path)
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
        nclusters = line.split("\t")[1].toInt();
      }
      else if(row == 1){
        hash = line.split("\t")[1];
      }
      else if(row == 2){
        clusteralgo = line.split("\t")[1].toInt();
      }
      else{
        continue;
      }
      row++;
    }
  }
  file.close();
}

void ClusterModel::ImportClusterModel(QString path, QString name_)
{
  name = name_;
  string sep = " \t";
  DATAIO::ImportUIvector(QString(path+"/clusters.txt").toUtf8().data(), clusters);
  DATAIO::ImportMatrix(QString(path+"/centroids.txt").toUtf8().data(), sep, centroids);
  DATAIO::ImportMatrix(QString(path+"/validation.txt").toUtf8().data(), sep, validation);
  ImportClusterModelInfo(path);
}

void ClusterModel::WriteClusterModel(QString path)
{
  if(DATAIO::CheckDir(path.toUtf8().data()) == false){
    DATAIO::MakeDir(path.toUtf8().data());
  }

  DATAIO::WriteUIvector(QString(path+"/clusters.txt").toUtf8().data(), getClustersVector());
  DATAIO::WriteMatrix(QString(path+"/centroids.txt").toUtf8().data(), getCentroids());
  DATAIO::WriteMatrix(QString(path+"/validation.txt").toUtf8().data(), getValidation());
  
  QStringList info;
  info.append(QString("NUMBER OF CLUSTERS\t%1").arg(QString::number(getNumberOfClusters())));
  info.append(QString("INPUT MATRIX HASH\t%1").arg(getHash()));
  info.append(QString("CLUSTERING ALGORITHM\t%1").arg(QString::number(clusteralgo)));
  DATA::WriteList(info, path+"/info.txt");
}

ClusterModel::ClusterModel()
{
  initUIVector(&clusters);
  initMatrix(&centroids);
  initMatrix(&validation);
  pid = mid = modelid = dataid = nclusters = clusteralgo = -1;
}

ClusterModel::~ClusterModel()
{
  DelMatrix(&centroids);
  DelUIVector(&clusters);
  DelMatrix(&validation);
}

