#include "qstudiometricsdataoperations.h"
#include "qstudiometricstypes.h"
#include <iostream>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <string>
#include <cmath>
#include <scientific.h>

LABELS DeserializeLABELS(QString serialized_l)
{
  LABELS l;
  if(serialized_l.compare("NULL", Qt::CaseInsensitive) != 0){
    QStringList r = serialized_l.split("/#");
    for(int i = 0; i < r.size(); i++){
      l.append(LABEL());
      QStringList lb = r[i].split(";");
      l.last().name = lb[0];
      l.last().objects = lb[1].split("//");
    }
  }
  return l;
}

QString SerializeLABELS(LABELS l)
{
  if(l.size() > 0){
    QString serialized_l;
    for(int i = 0; i < l.size()-1; i++){
      serialized_l +=  l[i].name+";"+SerializeQStringList(l[i].objects)+"/#";
    }
    serialized_l += l.last().name+";"+SerializeQStringList(l.last().objects);
    return serialized_l.toUtf8();
  }
  else{
    return QString("NULL");
  }
}

QStringList DeserializeQStringList(QString serialized_lst)
{
  if(serialized_lst.compare("NULL", Qt::CaseInsensitive) != 0)
    return serialized_lst.split("//");
  else
    return QStringList();
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

void DeserializeDVector(QString serialized_dvector, dvector *v)
{
  if(serialized_dvector.compare("NULL", Qt::CaseInsensitive) == 0 || serialized_dvector.size() == 0){
    return;
  }
  else{
    QStringList strvct = serialized_dvector.split(";");
    DVectorResize(v, strvct.size());
    for(int i = 0; i < strvct.size(); i++){
      v->data[i] = std::stod(strvct[i].toStdString());
    }
  }
}

QString SerializeDVector(dvector *v)
{
  if(v->size > 0){
    std::string serialized_dvector;
    for(size_t i = 0; i < v->size-1; i++)
      serialized_dvector += std::to_string(v->data[i])+";";
    serialized_dvector += std::to_string(v->data[v->size-1]);
    return QString(serialized_dvector.c_str()).toUtf8();
  }
  else{
    return QString("NULL");
  }
}

void DeserializeUIVector(QString serialized_uivector, uivector *v)
{
  if(serialized_uivector.compare("NULL", Qt::CaseInsensitive) == 0 || serialized_uivector.size() == 0){
    return;
  }
  else{
    QStringList strvct = serialized_uivector.split(";");
    UIVectorResize(v, strvct.size());
    for(int i = 0; i < strvct.size(); i++){
      v->data[i] = std::stoi(strvct[i].toStdString());
    }
  }
}

QString SerializeUIVector(uivector *v)
{
  if(v->size > 0){
    std::string serialized_uivector;
    for(size_t i = 0; i < v->size-1; i++)
      serialized_uivector += std::to_string(v->data[i])+";";
    serialized_uivector += std::to_string(v->data[v->size-1]);
    return QString(serialized_uivector.c_str()).toUtf8();
  }
  else{
    return QString("NULL");
  }
}

void DeserializeMatrix(QString serialized_mx, matrix *mx)
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
        mx->data[i][j] = std::stod(_mx_[i][j].toStdString());
      }
    }
  }
}

QString SerializeMatrix(matrix *mx)
{
  if(mx->row > 0 && mx->col > 0){
    std::string serialized_mx;
    for(size_t i = 0; i < mx->row-1; i++){
      //start a row and concatenate values
      for(size_t j = 0; j < mx->col-1; j++){
        serialized_mx += std::to_string(mx->data[i][j])+";";
      }
      serialized_mx += std::to_string(mx->data[i][mx->col-1])+"//"; //end row
    }

    int lrow = mx->row-1;
    for(size_t j = 0; j < mx->col-1; j++){
      serialized_mx += std::to_string(mx->data[lrow][j])+";";
    }
    serialized_mx += std::to_string(mx->data[lrow][mx->col-1]); //end row

    return QString(serialized_mx.c_str()).toUtf8();
  }
  else{
    return QString("NULL");
  }
}

void DeserializeTensor(QString serialized_ar, tensor *ar)
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
      AddTensorMatrix(&ar, _ar_[k].size(), _ar_[k][0].size());
      for(int i = 0; i < _ar_[k].size(); i++){
        for(int j = 0; j < _ar_[k][i].size(); j++){
          ar->m[k]->data[i][j] = std::stod(_ar_[k][i][j].toStdString());
        }
      }
    }
  }
}

QString SerializeTensor(tensor *ar)
{
  if(ar->order > 0){
    if(ar->m[0]->row > 0 && ar->m[0]->col > 0){
      std::string serialized_ar;
      for(size_t k = 0; k < ar->order; k++){
        for(size_t i = 0; i < ar->m[k]->row-1; i++){
          //start a row and concatenate values
          for(size_t j = 0; j < ar->m[k]->col-1; j++){
            serialized_ar += std::to_string(ar->m[k]->data[i][j])+";";
          }
          serialized_ar += std::to_string(ar->m[k]->data[i][ar->m[k]->col-1])+"//"; // end row
        }

        int lrow = ar->m[k]->row-1;
        for(size_t j = 0; j < ar->m[k]->col-1; j++){
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

QString GenHashFromStrlst(QStringList slst)
{

  slst.sort(Qt::CaseSensitive);
  QCryptographicHash hash_(QCryptographicHash::Sha256);
  hash_.addData(slst.join("").toUtf8());
  return QString(hash_.result().toHex());
}
