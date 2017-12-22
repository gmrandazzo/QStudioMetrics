#include "qstudiometricsdataoperations.h"
#include <iostream>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <string>
#include <cmath>
#include <scientific.h>


QStringList DeserializeQStringList(QString serialized_lst)
{
  return serialized_lst.split("//");
}

QString SerializeQStringList(QStringList lst)
{
  if(lst.size() > 0){
  QString serialized_lst;
    for(int i = 0; i < lst.size()-1; i++)
      serialized_lst += lst[i]+"//";
    serialized_lst += lst.last();
    return serialized_lst.toUtf8();
  }
  else{
    return QString("NULL");
  }
}

void DeserializeDVector(QString serialized_dvector, dvector **v)
{
  if(serialized_dvector.compare("NULL", Qt::CaseInsensitive) == 0 || serialized_dvector.size() == 0){
    return;
  }
  else{
    QStringList strvct = serialized_dvector.split(";");
    DVectorResize(v, strvct.size());
    for(int i = 0; i < strvct.size(); i++){
      (*v)->data[i] = std::stod(strvct[i].toStdString());
    }
  }
}

QString SerializeDVector(dvector *v)
{
  if(v->size > 0){
    std::string serialized_dvector;
    for(int i = 0; i < v->size-1; i++)
      serialized_dvector += std::to_string(v->data[i])+";";
    serialized_dvector += std::to_string(v->data[v->size-1]);
    return QString(serialized_dvector.c_str()).toUtf8();
  }
  else{
    return QString("NULL");
  }
}

void DeserializeUIVector(QString serialized_uivector, uivector **v)
{
  if(serialized_uivector.compare("NULL", Qt::CaseInsensitive) == 0 || serialized_uivector.size() == 0){
    return;
  }
  else{
    QStringList strvct = serialized_uivector.split(";");
    UIVectorResize(v, strvct.size());
    for(int i = 0; i < strvct.size(); i++){
      (*v)->data[i] = std::stoi(strvct[i].toStdString());
    }
  }
}

QString SerializeUIVector(uivector *v)
{
  if(v->size > 0){
    std::string serialized_uivector;
    for(int i = 0; i < v->size-1; i++)
      serialized_uivector += std::to_string(v->data[i])+";";
    serialized_uivector += std::to_string(v->data[v->size-1]);
    return QString(serialized_uivector.c_str()).toUtf8();
  }
  else{
    return QString("NULL");
  }
}

void DeserializeMatrix(QString serialized_mx, matrix **mx)
{
  if(serialized_mx.compare("NULL", Qt::CaseInsensitive) == 0 || serialized_mx.size() == 0){
    return;
  }
  else{
    QList< QList<QString> > _mx_;
    QStringList row = serialized_mx.split("//");
    for(int i = 0; i < row.size(); i++){
      _mx_.append(row[i].trimmed().split(";"));
    }

    ResizeMatrix(mx, _mx_.size(), _mx_[0].size());
    for(int i = 0; i < _mx_.size(); i++){
      for(int j = 0; j < _mx_[i].size(); j++){
        (*mx)->data[i][j] = std::stod(_mx_[i][j].toStdString());
      }
    }
  }
}

QString SerializeMatrix(matrix *mx)
{
  if(mx->row > 0 && mx->col > 0){
    std::string serialized_mx;
    for(int i = 0; i < mx->row-1; i++){
      //start a row and concatenate values
      for(int j = 0; j < mx->col-1; j++){
        serialized_mx += std::to_string(mx->data[i][j])+";";
      }
      serialized_mx += std::to_string(mx->data[i][mx->col-1])+"//"; //end row
    }

    int lrow = mx->row-1;
    for(int j = 0; j < mx->col-1; j++){
      serialized_mx += std::to_string(mx->data[lrow][j])+";";
    }
    serialized_mx += std::to_string(mx->data[lrow][mx->col-1]); //end row

    return QString(serialized_mx.c_str()).toUtf8();
  }
  else{
    return QString("NULL");
  }
}

void DeserializeArray(QString serialized_ar, array **ar)
{
  if(serialized_ar.compare("NULL", Qt::CaseInsensitive) == 0 || serialized_ar.size() == 0){
    return;
  }
  else{
    QList< QList< QStringList > > _ar_;
    QStringList orders = serialized_ar.split("o");
    for(int k = 0; k < orders.size(); k++){
      QList<QStringList> tmp;
      QStringList row = orders[k].split("//");
      for(int i = 0; i < row.size(); i++){
        tmp.append(row[i].split(";"));
      }
      _ar_.append(tmp);
    }

    //NewArray(ar, _ar_.size());
    for(int k = 0; k < _ar_.size(); k++){
      AddArrayMatrix(ar, _ar_[k].size(), _ar_[k][0].size());
      for(int i = 0; i < _ar_[k].size(); i++){
        for(int j = 0; j < _ar_[k][i].size(); j++){
          (*ar)->m[k]->data[i][j] = std::stod(_ar_[k][i][j].toStdString());
        }
      }
    }
  }
}

QString SerializeArray(array *ar)
{
  if(ar->order > 0){
    if(ar->m[0]->row > 0 && ar->m[0]->col > 0){
      std::string serialized_ar;
      for(int k = 0; k < ar->order; k++){
        for(int i = 0; i < ar->m[k]->row-1; i++){
          //start a row and concatenate values
          for(int j = 0; j < ar->m[k]->col-1; j++){
            serialized_ar += std::to_string(ar->m[k]->data[i][j])+";";
          }
          serialized_ar += std::to_string(ar->m[k]->data[i][ar->m[k]->col-1])+"//"; // end row
        }

        int lrow = ar->m[k]->row-1;
        for(int j = 0; j < ar->m[k]->col-1; j++){
          serialized_ar += std::to_string(ar->m[k]->data[lrow][j])+";";
        }
        serialized_ar += std::to_string(ar->m[k]->data[lrow][ar->m[k]->col-1]);

        if( k < ar->order-1)
          serialized_ar += "o";
      }
      return QString(serialized_ar.c_str()).toUtf8();
    }
    else{
      return QString("NULL");
    }
  }
  else{
    return QString("NULL");
  }
}

QString GenDVectorHash(dvector *v)
{
  QString hash;
  if(v->size> 0){
    hash.append(QString::number(v->size)); // get the size
    hash.append(QString::number((int)ceil(DvectorModule(v)))); // get the vector module
    hash.append(QString::number((int)ceil(getDVectorValue(v, 0)))); // get the element 0
    hash.append(QString::number((int)ceil(getDVectorValue(v, (size_t)ceil((v->size-1)/2))))); // get the middle element
    hash.append(QString::number((int)ceil(getDVectorValue(v, v->size-1)))); // get the last element
    QCryptographicHash hash_(QCryptographicHash::Md5);
    hash_.addData(hash.toUtf8());
    hash = QString(hash_.result().toHex());
  }
  else{
    hash = "Vector Empty!";
  }
  return hash;
}

QString GenMatrixHash(matrix *m)
{
  QString hash;
  if(m->row > 0 && m->col > 0){
    QString vectorizedform;
    vectorizedform.append(QString::number(m->row)); // get the row
    vectorizedform.append(QString::number(m->col)); // get the col
    for(uint i = 0; i < m->row; i++){
      for(uint j = 0; j < m->col; j++){
        //vectorizedform.append(QString::number(((int)getMatrixValue(m, i, j)*100)/100.0));
        vectorizedform.append(QString::number((int)ceil(getMatrixValue(m, i, j))));
      }
    }

    QCryptographicHash hash_(QCryptographicHash::Md5);
    hash_.addData(vectorizedform.toUtf8());
    hash = QString(hash_.result().toHex());
  }
  else{
    hash = "Matrix Empty!";
  }
  return hash;
}

QString GenArrayHash(array *a)
{
  QString hash;
  if(a->order > 0){
    hash.append(QString::number(a->order)); // get the order
    QString vectorizedform;
    for(uint k = 0; k < a->order; k++){
      vectorizedform.append(QString::number(a->m[k]->row)); // get the row
      vectorizedform.append(QString::number(a->m[k]->col)); // get the col
      for(uint i = 0; i < a->m[k]->row; i++){
        for(uint j = 0; j < a->m[k]->col; j++){
          //vectorizedform.append(QString::number(((int)getArrayValue(a, k, i, j)*100)/100.0));
          vectorizedform.append(QString::number((int)ceil(getArrayValue(a, k, i, j))));
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
  return hash;
}
