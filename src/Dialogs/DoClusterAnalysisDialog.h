/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for doclusteranalysis operations.
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

#ifndef DOCLUSTERANALYSISDIALOG_H
#define DOCLUSTERANALYSISDIALOG_H

#include "qsmdata.h"
#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "ui_DoClusterAnalysisDialog.h"

class DoClusterAnalysisDialog : public QDialog {
  Q_OBJECT

public:
  enum {
    MenuSelection = 0,
    PlotSelection = 1,
    KMEANSRANDOM = 0,
    KMEANSPP = 1,
    KMEANSMDC = 2,
    KMEANSMAXMINDIS = 3,
    HCLSINGLELINK = 4,
    HCLCOMPLETELINK = 5,
    HCLAVERAGELINK = 6,
    HCLWARDLINK = 7,
    NEAROBJECTS = 0,
    FAROBJECTS = 1,
    ALLOBJECTS = 3
  };

  DoClusterAnalysisDialog(QList<MATRIX *> *mxlst, QList<ARRAY *> *arlst,
                          int windowtype);

  int getAlgorithmType();
  int getNumberOfClusters();

  int getDataType(); /*0 for current plot coordinates; 1 for Source Data*/
  QString getDataHash();
  int getLayerID(); /*used for array*/

  bool SaveClusterLabels();
  QString getClusterLabelSufix();

  int getExtractObjects();
  int getNMaxObjects();

  bool ValidateCluster();
  int getVaidationType();
  int getMaxClustersNumber();
  int getNGroups();
  int getNIterations();

private slots:
  void EnableDisable();
  void GenLayerList();
  void OK();

private:
  Ui::DoClusterAnalysisDialog ui;
  int windowtype;
  QList<MATRIX *> mxlst;
  QList<ARRAY *> arlst;
  QStringList hash;
  QString selectedhash;
};

#endif
