#ifndef DATA_H
#define DATA_H

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QList>
#include <QPixmap>
#include <QString>
#include <QStringList>
#include <cmath>
#include <QDebug>
#include <QCryptographicHash>
#include <QTreeWidget>

#include "qstudiometricstypes.h"
#include "DATAIO.h"
#include "pcamodel.h"
#include "plsmodel.h"
#include "upcamodel.h"
#include "uplsmodel.h"
#include "mlrmodel.h"
#include "ldamodel.h"
#include "VariableSelectionModel.h"

#include <scientific.h>

struct FILEDATA{
  QString filename; // filepath
  QString separator;
  QString skiplineby;
  QString fileobjname;
  QString filevarname;
  QString label; // name of matrix/aray
  uint datatype; // 1 is matrix, 2 is array
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
    qDebug() << "Delete DVector";
    DelDVector(&v); }

  void SortByName(){
    QList<QPair<QString, uint> > array_;
    for (uint i = 0; i < v->size; i++){
      array_.append(qMakePair(objname[i], i));
    }
    // Ordering ascending
    qSort(array_.begin(), array_.end(), QPairComparerAscending());

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
  QString getName(){ return name; }
  QStringList& getObjName(){ return objname; }
  void GenHash(){
    if(v->size> 0 && !name.isEmpty()){
      /*SortByName(); */
      hash.clear();
      hash.append(QString::number(v->size)); // get the size
      hash.append(QString::number(ceil(DvectorModule(v)))); // get the vector module
      hash.append(QString::number(ceil(getDVectorValue(v, 0)))); // get the element 0
      hash.append(QString::number(ceil(getDVectorValue(v, (size_t)ceil((v->size-1)/2))))); // get the middle element
      hash.append(QString::number(ceil(getDVectorValue(v, v->size-1)))); // get the last element
      hash.append(name.toLatin1().toHex()); // get the last element
    }
    else{
      hash = "Vector Empty!";
    }
  }
  QString& getHash(){ return hash; }

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
    qDebug() << "Delete Matrix";
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
    qSort(array_.begin(), array_.end(), QPairComparerAscending());

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
  QString getName(){ return name; }
  QStringList& getObjName(){ return objname; }
  QStringList& getVarName(){ return varname; }
  void GenHash(){
    if(m->row > 0 && m->col > 0 && objname.size() > 0){
      /*SortByName(); */
      hash.clear();
      QString vectorizedform;
      vectorizedform.append(QString::number(m->row)); // get the row
      vectorizedform.append(QString::number(m->col)); // get the col

      for(uint i = 0; i < m->row; i++){
        for(uint j = 0; j < m->col; j++){
          vectorizedform.append(QString::number(((int)getMatrixValue(m, i, j)*100)/100.0));
        }
      }

      QCryptographicHash hash_(QCryptographicHash::Md5);
      hash_.addData(vectorizedform.toUtf8());
      hash = QString(hash_.result().toHex());
    }
    else{
      hash = "Matrix Empty!";
    }
  }
  QString& getHash(){ return hash; }

private:
  QStringList objname, varname;
  QString name;
  matrix *m;
  QString hash;
};

class ARRAY
{
public:
  ARRAY(){ initArray(&a); }
  ~ARRAY(){
    qDebug()<< "Delete Array";
    varname.clear();
    objname.clear();
    DelArray(&a);
  }

  void SortByName(){
    array *atmp;
    QList<QPair<QString, uint> > array_;
    for(uint i = 0; i < a->m[0]->row; i++){
      array_.append(qMakePair(objname[i], i));
    }
    // Ordering ascending
    qSort(array_.begin(), array_.end(), QPairComparerAscending());


    initArray(&atmp);
    ArrayCopy(a, &atmp);
    objname.clear();
    for(int i = 0; i < array_.size(); i++){
      for(uint k = 0; k < atmp->order; k++){
        for(uint j = 0; j < atmp->m[k]->col; j++){
          setArrayValue(a, k, i, j, getArrayValue(atmp, k, array_[i].second, j));
        }
      }
      objname.append(array_[i].first);
    }
    DelArray(&atmp);
  }

  array* &Array(){ return a; }
  void setName(QString name_){ name = name_; }
  QString getName(){ return name; }
  QStringList& getObjName(){ return objname; }
  QStringList& getVarName(){ return varname; }
  void GenHash(){
    if(a->order > 0 && objname.size() > 0){
      /*SortByName();*/
      hash.clear();
      hash.append(QString::number(a->order)); // get the order
      QString vectorizedform;
      for(uint k = 0; k < a->order; k++){
        vectorizedform.append(QString::number(a->m[k]->row)); // get the row
        vectorizedform.append(QString::number(a->m[k]->col)); // get the col
        for(uint i = 0; i < a->m[k]->row; i++){
          for(uint j = 0; j < a->m[k]->col; j++){
            vectorizedform.append(QString::number(((int)getArrayValue(a, k, i, j)*100)/100.0));
          }
        }
      }
      QCryptographicHash hash_(QCryptographicHash::Md5);
      hash_.addData(vectorizedform.toUtf8());
      hash = QString(hash_.result().toHex());
    }
    else{
      hash = "Array Empty!";
    }
  }
  QString& getHash(){ return hash; }

private:
  QStringList objname, varname;
  QString name;
  array *a;
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
  QString SaveData(QString savepath); // savepath is the path to save the project
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

  void addUPCAModel();
  void delUPCAModel(int mid);
  void delUPCAModelAt(int id);
  void delUPCAModels();

  void addPLSModel();
  void delPLSModel(int mid);
  void delPLSModelAt(int id);
  void delPLSModels();

  void addUPLSModel();
  void delUPLSModel(int mid);
  void delUPLSModelAt(int id);
  void delUPLSModels();

  void addVarSelModel();
  void delVarSelModel(int mid);
  void delVarSelModelAt(int id);
  void delVarSelModels();

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
  UPCAModel *getLastUPCAModel();
  PLSModel *getLastPLSModel();
  UPLSModel *getLastUPLSModel();
  VariableSelectionModel *getLastVarSelModel();
  MLRModel *getLastMLRModel();
  LDAModel *getLastLDAModel();

  // return the model at given position id in list
  PCAModel *getPCAModelAt(int id);
  UPCAModel *getUPCAModelAt(int id);
  PLSModel *getPLSModelAt(int id);
  UPLSModel *getUPLSModelAt(int id);
  VariableSelectionModel *getVarSelModelAt(int id);
  MLRModel *getMLRModelAt(int id);
  LDAModel *getLDAModelAt(int id);

  // return the model with a special ModelID
  PCAModel *getPCAModel(int mid);
  UPCAModel *getUPCAModel(int mid);
  PLSModel *getPLSModel(int mid);
  UPLSModel *getUPLSModel(int mid);
  VariableSelectionModel *getVarSelModel(int mid);
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
  int UPCACount();
  int UPLSCount();
  int VarSelCount();
  int MLRCount();
  int LDACount();

  static void WriteList(QStringList &lst, QString fname);
  static void GenNameLst(uint n, QString name, QStringList &namelst);
  static void ImportRows(QString fname, QStringList &rowlst);
  static void ImportColumns(QString fname, QString separator, QStringList &collst);
  static bool CopyFile(const QString& sourceFile, const QString& destinationDir);
private:
  QString projectpath, projectname;
  QList<MATRIX*> matrix_;
  QList<ARRAY*> array_;
  QList<PCAModel*> pcamodel;
  QList<UPCAModel*> upcamodel;
  QList<PLSModel*> plsmodel;
  QList<UPLSModel*> uplsmodel;
  QList<VariableSelectionModel*> varselectionmodel;
  QList<MLRModel*> mlrmodel;
  QList<LDAModel*> ldamodel;
  LABELS objlabels, varlabels;
  TABLABELS vartablabels;
  QList<IMAGE> images;
  int uniqueid;
};


typedef QMap<int, DATA*> PROJECTS; // ID and PROJECT

#endif
