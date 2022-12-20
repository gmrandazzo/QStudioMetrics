#ifndef DOCLUSTERANALYSISDIALOG_H
#define DOCLUSTERANALYSISDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>
#include "qsmdata.h"

#include "ui_DoClusterAnalysisDialog.h"

class DoClusterAnalysisDialog: public QDialog
{
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
  
  DoClusterAnalysisDialog(QList<MATRIX*> *mxlst, QList<ARRAY*> *arlst, int windowtype);

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
  QList<MATRIX*> mxlst;
  QList<ARRAY*> arlst;
  QStringList hash;
  QString selectedhash;
};

#endif

