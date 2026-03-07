/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for ClusterModel.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef CLUSTERMODEL_H
#define CLUSTERMODEL_H

#include "qstudiometricstypes.h"
#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <scientific.h>

class ClusterModel {
public:
  ClusterModel();
  ~ClusterModel();

  void ImportClusterModel(QString path, QString name_);
  void WriteClusterModel(QString path);

  void setName(QString name_) { name = name_; }
  QString getName() { return name; }

  int getModelID() { return mid; }
  void setModelID(int mid_) { mid = mid_; }

  matrix *getCentroids() { return centroids; }
  uivector *getClustersVector() { return clusters; }
  matrix *getValidation() { return validation; }

  void setHash(QString hash_) { hash = hash_; }
  QString &getHash() { return hash; }

  void setNumberOfClusters(int nclusters_) { nclusters = nclusters_; };
  int getNumberOfClusters() { return nclusters; };
  void setClusterAlgorithm(int clusteralgo_) { clusteralgo = clusteralgo_; }
  int getClusterAlgorithm() { return clusteralgo; }

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
