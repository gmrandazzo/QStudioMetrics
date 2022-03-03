#ifndef QSMDATA_H
#define QSMDATA_H

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QList>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <cmath>
#include <QCryptographicHash>
#include <QTreeWidget>

#ifdef DEBUG
#include <QDebug>
#endif

#include "qstudiometricstypes.h"
#include "qstudiometricsdataoperations.h"
#include "DATAIO.h"
#include "PCA/pcamodel.h"
#include "PLS/plsmodel.h"
#include "EPLS/eplsmodel.h"
#include "MLR/mlrmodel.h"
#include "LDA/ldamodel.h"

#include <scientific.h>

struct FILEDATA{
  QString filename; // filepath
  QString separator;
  QString skiplineby;
  QString fileobjname;
  QString filevarname;
  QString label; // name of matrix/aray
  uint datatype; // 1 is matrix, 2 is tensor
};

struct QPairComparerAscending
{
  template<typename T1, typename T2>
  bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const{
    return a.first < b.first;
  }
};

struct QPairComparerDescending
{
  template<typename T1, typename T2>
  bool operator()(const QPair<T1,T2> & a, const QPair<T1,T2> & b) const{
    return a.first > b.first;
  }
};

class DVECTOR
{
public:
  DVECTOR(){ initDVector(&v); }
  DVECTOR(uint n){ NewDVector(&v, n); }
  ~DVECTOR(){
    #ifdef DEBUG
    qDebug() << "Delete DVector";
    #endif
    DelDVector(&v); }

  void SortByName(){
    QList<QPair<QString, uint> > array_;
    for (uint i = 0; i < v->size; i++){
      array_.append(qMakePair(objname[i], i));
    }
    // Ordering ascending
    //qSort(array_.begin(), array_.end(), QPairComparerAscending()); DEPRECATED
    std::sort(array_.begin(), array_.end(), QPairComparerAscending());

    dvector *tmp;
    initDVector(&tmp);
    objname.clear();
    DVectorCopy(v, &tmp);
    for(int i = 0; i < array_.size(); i++){
      setDVectorValue(v, i, getDVectorValue(tmp, array_[i].second));
      objname.append(array_[i].first);
    }
    DelDVector(&tmp);
  }

  dvector* &DVector(){ return v; }
  void setName(QString name_){ name = name_; }
  QString getName(){ return name.toUtf8(); }
  QStringList& getObjName(){ return objname; }
  QString& getHash(){ if(hash.size() == 0){ hash = GenDVectorHash(v); } return hash; }

private:
  dvector *v;
  QStringList objname;
  QString name;
  QString hash;
};

class MATRIX
{
public:
  MATRIX(){ initMatrix(&m); }
  ~MATRIX(){
    #ifdef DEBUG
    qDebug() << "Delete Matrix";
    #endif
    varname.clear();
    objname.clear();
    DelMatrix(&m);
  }

  void SortByName(){
    QList<QPair<QString, uint> > array_;
    for(uint i = 0; i < m->row; i++){
      array_.append(qMakePair(objname[i], i));
    }
    // Ordering ascending
    //qSort(array_.begin(), array_.end(), QPairComparerAscending()); DEPRECATED
    std::sort(array_.begin(), array_.end(), QPairComparerAscending());

    matrix *tmp;
    initMatrix(&tmp);
    objname.clear();
    MatrixCopy(m, &tmp);
    for(int i = 0; i < array_.size(); i++){
      for(uint j = 0; j < tmp->col; j++){
        setMatrixValue(m, i, j, getMatrixValue(tmp, array_[i].second, j));
      }
      objname.append(array_[i].first);
    }
    DelMatrix(&tmp);
  }

  void MatrixResize(size_t nrow, size_t ncol){ ResizeMatrix(&m, nrow, ncol); }
  matrix* &Matrix(){ return m; } //Access to the matrix
  void setName(QString name_){ name = name_; }
  QString getName(){ return name.toUtf8(); }
  QStringList& getObjName(){ return objname; }
  QStringList& getVarName(){ return varname; }
  QString& getHash(){ if(hash.size() == 0){ hash = GenMatrixHash(m); } return hash; }

private:
  QStringList objname, varname;
  QString name;
  matrix *m;
  QString hash;
};

class ARRAY
{
public:
  ARRAY(){ initTensor(&a); }
  ~ARRAY(){
    #ifdef DEBUG
    qDebug()<< "Delete Array";
    #endif
    varname.clear();
    objname.clear();
    DelTensor(&a);
  }

  void SortByName(){
    tensor *atmp;
    QList<QPair<QString, uint> > array_;
    for(uint i = 0; i < a->m[0]->row; i++){
      array_.append(qMakePair(objname[i], i));
    }
    // Ordering ascending
    //qSort(array_.begin(), array_.end(), QPairComparerAscending()); DEPRECATED
    std::sort(array_.begin(), array_.end(), QPairComparerAscending());

    initTensor(&atmp);
    TensorCopy(a, &atmp);
    objname.clear();
    for(int i = 0; i < array_.size(); i++){
      for(uint k = 0; k < atmp->order; k++){
        for(uint j = 0; j < atmp->m[k]->col; j++){
          setTensorValue(a, k, i, j, getTensorValue(atmp, k, array_[i].second, j));
        }
      }
      objname.append(array_[i].first);
    }
    DelTensor(&atmp);
  }

  tensor* &Array(){ return a; }
  void setName(QString name_){ name = name_; }
  QString getName(){ return name.toUtf8(); }
  QStringList& getObjName(){ return objname; }
  QStringList& getVarName(){ return varname; }
  QString& getHash(){ if(hash.size() == 0){ hash = GenArrayHash(a); } return hash; }

private:
  QStringList objname, varname;
  QString name;
  tensor *a;
  QString hash;
};

class DATA
{
public:
  DATA();
  ~DATA();
  void setProjectPath(QString projectpath_){ projectpath = projectpath_; }
  void setProjectName(QString projectname_){ projectname = projectname_; }
  QString &getProjectName(){ return projectname; }
  QString &getProjectPath(){ return projectpath; }
  void ImportFileMatrix(const FILEDATA &f);
  void ImportFileArray(const FILEDATA &f);
  void OpenData(QString dir, QTreeWidget *treeWidget, int *tabcount_, int *mid_, QStringList *log);
  bool isSQLDatabase(QString sqlfile);
  void OpenSQLData(QString sqlfile, QTreeWidget *treeWidget, int *tabcount_, int *mid_, QStringList *log);
  QString SaveData(QString savepath); // savepath is the path to save the project
  QString SaveSQLData(QString dbName);
  void AutoSave(); // To work first SaveData.
  QString getAutoSaveFile(){ return projectfautosave; }
  void addMatrix();
  void addMatrix(MATRIX *mx);
  void addArray();
  void addArray(ARRAY *ar);
  void delMatrixAt(int i);
  void delArrayAt(int i);
  void delMatrix();
  void delArray();

  void addPCAModel();
  void delPCAModel(int mid);
  void delPCAModelAt(int id);
  void delPCAModels();

  void addPLSModel();
  void delPLSModel(int mid);
  void delPLSModelAt(int id);
  void delPLSModels();

  void addEPLSModel();
  void delEPLSModel(int mid);
  void delEPLSModelAt(int id);
  void delEPLSModels();

  void addMLRModel();
  void delMLRModel(int mid);
  void delMLRModelAt(int id);
  void delMLRModels();

  void addLDAModel();
  void delLDAModel(int mid);
  void delLDAModelAt(int id);
  void delLDAModels();

  void setProjectID(int id_);
  int getProjectID();

  int getMatrixID(QString hash);
  int getArrayID(QString hash);

  MATRIX *getMatrix(int id);
  MATRIX *getMatrix(QString hash);
  ARRAY *getArray(int id);
  ARRAY *getArray(QString hash);
  QList<MATRIX*> &getMATRIXList(){ return matrix_; }
  QList<ARRAY*> &getARRAYList(){ return array_; }

  // return the last model
  PCAModel *getLastPCAModel();
  PLSModel *getLastPLSModel();
  EPLSModel *getLastEPLSModel();
  MLRModel *getLastMLRModel();
  LDAModel *getLastLDAModel();

  // return the model at given position id in list
  PCAModel *getPCAModelAt(int id);
  PLSModel *getPLSModelAt(int id);
  EPLSModel *getEPLSModelAt(int id);
  MLRModel *getMLRModelAt(int id);
  LDAModel *getLDAModelAt(int id);

  // return the model with a special ModelID
  PCAModel *getPCAModel(int mid);
  PLSModel *getPLSModel(int mid);
  PLSModel *getPLSModel(QString hash);
  EPLSModel *getEPLSModel(int mid);
  EPLSModel *getEPLSModel(QString hash);
  MLRModel *getMLRModel(int mid);
  LDAModel *getLDAModel(int mid);

  LABELS &getObjectLabels(){ return objlabels; } // Get/Set new label that collect objects
  LABELS &getVariableLabels(){ return varlabels; }
  TABLABELS &getVariableTabLabels(){ return vartablabels; }
  QList<IMAGE> &getImages(){ return images; }

  int MatrixCount();
  int ArrayCount();
  int PCACount();
  int PLSCount();
  int EPLSCount();
  int MLRCount();
  int LDACount();

  static void WriteList(QStringList &lst, QString fname);
  static void GenNameLst(uint n, QString name, QStringList &namelst);
  static void ImportRows(QString fname, QStringList &rowlst);
  static void ImportColumns(QString fname, QString separator, QStringList &collst);
  static bool CopyFile(const QString& sourceFile, const QString& destinationDir);
private:
  QString projectpath, projectname, projectfautosave;
  QList<MATRIX*> matrix_;
  QList<ARRAY*> array_;
  QList<PCAModel*> pcamodel;
  QList<PLSModel*> plsmodel;
  QList<EPLSModel*> eplsmodel;
  QList<MLRModel*> mlrmodel;
  QList<LDAModel*> ldamodel;
  LABELS objlabels, varlabels;
  TABLABELS vartablabels;
  QList<IMAGE> images;
  int uniqueid;
};


typedef QMap<int, DATA*> PROJECTS; // ID and PROJECT

#endif
