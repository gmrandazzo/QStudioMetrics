// qstudiomtypes
#ifndef QSTUDIOMETRICSTYPES_H
#define QSTUDIOMETRICSTYPES_H

#include <QString>
#include <QList>
#include <QPixmap>
#include <scientific.h>

enum{
  MATRIXDATA = 0,
  ARRAYDATA = 1,

  // Calculations
  PCA_ = 2,
  PLS_ = 3,
  UPCA_ = 4,
  UPLS_ = 5,
  PLSValidation = 6,
  UPLSValidation = 7,
  MLR_ = 8,
  MLRValidation = 9,
  LDA_ = 10,
  LDAValidation = 11,

  PLSVariableSelection = 12,
  UPLSVariableSelection = 13,

  PCAPrediction = 14,
  PLSPrediction = 15,
  UPCAPrediction = 16,
  UPLSPrediction = 17,
  MLRPrediction = 18,
  LDAPrediction_ = 19,

  PLSR2R2Plot = 20,
  UPLSR2R2Plot = 21,
  PLSRecalcVSExperimental = 22,
  UPLSRecalcVSExperimental = 23,
  PLSPredictedVSExperimental = 24,
  UPLSPredictedVSExperimental = 25,
  MLRRecalcVSExperimental = 26,
  MLRPredictedVSExperimental = 27,

  PLSRecalcVSExperimentalWithPrediction = 28,
  PLSPredictedVSExperimentalWithPrediction = 29,
  UPLSRecalcVSExperimentalWithPrediction = 29,
  UPLSPredictedVSExperimentalWithPrediction = 29,
  MLRRecalcVSExperimentalWithPrediction = 30,
  MLRPredictedVSExperimentalWithPrediction = 30,

  PLSYSCRAMBLING = 31,
  MLRYSCRAMBLING = 32,
  PLSQ2SDEPSTATICSAMPLEVALIDATION = 33,
  PLSQ2SDEPDYNAMICSAMPLEVALIDATION = 34,

  VarSel = 35,

  // Variables Types
  LOO = 36,
  RANDOMGROUP = 37,
  EXTERNALTESTSET = 38,
  GA = 39,
  PSO = 40,
  SPEARMAN = 41,
  JUMPMETHOD = 42,

  MOSTDESCRIPTIVECOMPOUND = 43,
  MAXIMUMDISSIMILARITYMAXMIN = 44,
  RANDOMSELECTION = 45,
  LABELSELECTION = 46,

  EUCLIDEANDISTANCE = 47,
  MANHATTANDISTANCE = 48,
  COSINESSIMILARITY = 49,

  OBJLABELST = 50,
  VARLABELST = 51,
  TABLABELST = 52
};

/*highlight signal used in 2D plot to highlight objects (Used by FindCorrelationDialog)*/
struct highlightSignal
{
  QList <int> vid1;
  QList <int> vid2;
};

/*varvarplot signals used from loadings to plot correlations between variables*/
struct vvplotSignal
{
  int pid;
  int mid;
  int mtype;
  QString vname1;
  QString vname2;
};

/*used to connect a signal from table/plot to the mainwindow*/
struct ImageSignal
{
  int pid;
  QStringList imgname;
};

/*used to connect a signal from the mainwindow to the mdi....*/
struct ListSignal{
  int pid;
  int id;
  int type;
};

/*used to connect a signal from the FindCorrelationDialog to the mainwindow in order to open a variable vs variable plot */
struct CorrVarVarPlotSignal{
  int varid1, varid2;
};

struct LABEL{
  QString name;
  QStringList objects;
};

typedef QList<LABEL> LABELS; // QString is the labelname QStringList is the objectname selected

struct IMAGE{
  QString name;
  QString filepath;
  QPixmap image;
};

class TABLABEL{
public:
  TABLABEL(){ initMatrix(&m); }
  ~TABLABEL(){ DelMatrix(&m); }

  QString getName(){ return name; }
  void setName(QString name_){ name = name_; }
  QStringList &getObjectsName(){ return objectsname; }
  QStringList &getFeaturesName(){ return featuresname; }
  matrix *getMatrix(){ return m; }

private:
  QString name;
  matrix *m;
  QStringList objectsname, featuresname;
};

typedef QList<TABLABEL*> TABLABELS;

#endif
