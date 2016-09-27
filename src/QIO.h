#ifndef QIO_H
#define QIO_H
#include <QFile>
#include <QString>
#include <QStringList>
#include <ImportDataDialog.h>

extern "C" {
  #include "scientific.h"
}

namespace QIO
{
  void QImportLabels(FILES f, QStringList &objname);
  void QImportMatrix(FILES f, matrix *data, QStringList &varname);
  void QImportArray(FILES f, array *data, QStringList &varname);
  
}

#endif
