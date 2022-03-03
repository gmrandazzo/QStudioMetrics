#ifndef CLUSTERMODEL_H
#define CLUSTERMODEL_H

#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class ClusterModel
{
public:
  ClusterModel();
  ~ClusterModel();
  
  void ImportClusterModel(QString path, QString name_);
  void WriteClusterModel(QString path);
  
  void setName(QString name_){ name = name_; }
  QString getName(){ return name; }
  
  int getModelID(){ return mid; }
  void setModelID(int mid_){ mid = mid_; }
  
  matrix *getCentroids(){ return centroids; }
  uivector *getClustersVector(){ return clusters; }
  matrix *getValidation(){ return validation; }
  
  void setHash(QString hash_){ hash = hash_; }
  QString &getHash(){ return hash; }
  
  void setNumberOfClusters(int nclusters_){ nclusters = nclusters_; };
  int getNumberOfClusters(){ return nclusters; };
  void setClusterAlgorithm(QString clusteralgo_){ clusteralgo_ = clusteralgo_; }
  int setClusterAlgorithm(){ return clusteralgo; }
  
private:
  void ImportClusterModelInfo(QString path);
  QString hash, name;
  int mid;
  uivector *clusters;
  matrix *centroids;
  matrix *validation;
  int pid, modelid, dataid, nclusters, clusteralgo;
};

#endif