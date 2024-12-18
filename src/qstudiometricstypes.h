// qstudiomtypes
#ifndef QSTUDIOMETRICSTYPES_H
#define QSTUDIOMETRICSTYPES_H

#include <QList>
#include <QPixmap>
#include <QString>
#include <scientific.h>

static QString firstcol_name = "Object Names";

#define DEFAULT_EMTPY_VALUE missing_value()

enum {
  MATRIXDATA = 0,
  ARRAYDATA,

  // Calculations
  PCA_,
  CPCA_,
  PLS_,
  PLS_DA_,
  PLSValidation,
  // UPCA_,
  // UPLS_,
  // UPLSValidation,
  MLR_,
  MLRValidation,

  LDA_,
  LDAValidation,
  LDAPrediction_,

  PCAPrediction,
  // UPCAPrediction,

  PLSPrediction,
  PLSR2R2Plot,
  PLSRecalcVSExperimental,
  PLSPredictedVSExperimental,
  PLSRecalcVSExperimentalWithPrediction,
  PLSPredictedVSExperimentalWithPrediction,
  PLSYSCRAMBLING,

  // UPLSPrediction,
  // UPLSR2R2Plot,
  // UPLSRecalcVSExperimental,
  // UPLSPredictedVSExperimental,
  // UPLSRecalcVSExperimentalWithPrediction,
  // UPLSPredictedVSExperimentalWithPrediction,
  MLRPrediction,
  MLRRecalcVSExperimental,
  MLRPredictedVSExperimental,
  MLRRecalcVSExperimentalWithPrediction,
  MLRPredictedVSExperimentalWithPrediction,
  MLRYSCRAMBLING,

  LOO_,
  BOOTSTRAPRGCV_,
  KFOLDCV_,
  EXTERNALTESTSET,
  JUMPMETHOD,

  MOSTDESCRIPTIVECOMPOUND,
  MAXIMUMDISSIMILARITYMAXMIN,
  RANDOMSELECTION,
  LABELSELECTION,

  EUCLIDEANDISTANCE,
  MANHATTANDISTANCE,
  COSINESSIMILARITY,

  OBJLABELST,
  VARLABELST,
  TABLABELST
};

/*Project tree for plot dialog*/
typedef struct {
  int id;
  QString name;
} PREDICTIONTREE;

typedef struct {
  QList<PREDICTIONTREE> ptree;
  int id;
  QString name;
} MODELTREE;

typedef struct {
  QList<MODELTREE> mtree;
  int id;
  QString name;
} PROJECT;

#define ProjectTree QList<PROJECT>

/*highlight signal used in 2D plot to highlight objects (Used by
 * FindCorrelationDialog)*/
struct highlightSignal {
  QList<ssize_t> vid1;
  QList<ssize_t> vid2;
};

/*varvarplot signals used from loadings to plot correlations between variables*/
struct vvplotSignal {
  int pid;
  int mid;
  int mtype;
  QString vname1;
  QString vname2;
};

/*used to connect a signal from table/plot to the mainwindow*/
struct ImageSignal {
  int pid;
  QStringList imgname;
};

/*used to connect a signal from the mainwindow to the mdi....*/
struct ListSignal {
  int pid;
  int id;
  int type;
};

/*used to connect a signal from the FindCorrelationDialog to the mainwindow in
 * order to open a variable vs variable plot */
struct CorrVarVarPlotSignal {
  ssize_t varid1, varid2;
};

struct LABEL {
  QString name;
  QStringList objects;
};

typedef QList<LABEL>
    LABELS; // QString is the labelname QStringList is the objectname selected

struct RECENTMODELS {
  QString name;
  QString path;

  bool operator==(const RECENTMODELS &omod) const {
    if (name.compare(omod.name) == 0 && path.compare(omod.path) == 0)
      return true;
    return false;
  }
};

struct IMAGE {
  QString name;
  QString filepath;
  QPixmap image;
};

class TABLABEL {
public:
  TABLABEL() { initMatrix(&m); }
  ~TABLABEL() { DelMatrix(&m); }

  QString getName() { return name; }
  void setName(QString name_) { name = name_; }
  QStringList &getObjectsName() { return objectsname; }
  QStringList &getFeaturesName() { return featuresname; }
  matrix *getMatrix() { return m; }

private:
  QString name;
  matrix *m;
  QStringList objectsname, featuresname;
};

typedef QList<TABLABEL *> TABLABELS;

#endif
