#ifndef QSTUDIOMETRICSDATAOPERATIONS_H
#define QSTUDIOMETRICSDATAOPERATIONS_H

#include <iostream>
#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <string>
#include <scientific.h>

// Conversion functions for sql export
// N.B.: To serialize vector,matrix and tensor is utilised a std::string
// to avoid double conversion problems and truncated floating points present in QString
QStringList DeserializeQStringList(QString serialized_lst);
QString SerializeQStringList(QStringList lst);
void DeserializeDVector(QString serialized_dvector, dvector **v);
QString SerializeDVector(dvector *v);
void DeserializeUIVector(QString serialized_dvector, uivector **v);
QString SerializeUIVector(uivector *v);
void DeserializeMatrix(QString serialized_mx, matrix **mx);
QString SerializeMatrix(matrix *mx);
void DeserializeArray(QString serialized_ar, tensor **ar);
QString SerializeArray(tensor *ar);

// hash functions
// Serialze the matrix and return a QCryptographicHash in hexadecimal
QString GenDVectorHash(dvector *v);
QString GenMatrixHash(matrix *m);
QString GenArrayHash(tensor *a);

#endif
