#include "QDATAIO.h"
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QStringList>
#include "ImportDataDialog.h"
#include <QDebug>

#include "scientific.h"

void QDATAIO::QImportLabels(FILES f, QStringList &objname)
{
  QFile file(f.filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  while(!in.atEnd()){
   objname.append(in.readLine().trimmed());
  }

  file.close();
}

void ImportVarNames(FILES f, QStringList &varname)
{
  
}

void QDATAIO::QImportMatrix(FILES f, matrix *data, QStringList &varname)
{
  QFile file(f.filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  size_t row, col;
  row = col = 0;
  while(!file.atEnd()){
    QString line = file.readLine();
    if(line.isEmpty() || line.contains(f.skiplineby, Qt::CaseInsensitive) == true){
      continue;
    }
    else{
      QStringList list = line.split(f.separator, QString::SkipEmptyParts);
      col = list.size();
      row++;
    }
  }
  
  ResizeMatrix(&data, (row - f.skiplineby), col);
  
  file.seek(0);
  
  bool getvarname = true;
  
  row = 0;
  uint skipnline = 0;
  while(!file.atEnd()){
    
    QString line = file.readLine().trimmed();
   
    if(line.contains(f.skiplineby, Qt::CaseInsensitive) == true){
      continue;
    }
    else{
      if(f.nline > 0 && skipnline < f.nline){
        skipnline++;
      }
      else{
        if(getvarname == true){
          if(f.firstrowvarname == true && f.filevarname.isEmpty()){
            varname = line.split(f.separator, QString::SkipEmptyParts);
          }
          else if(f.firstrowvarname == false && !f.filevarname.isEmpty()){
            QFile file1(f.filevarname);
            if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
              return;

            QTextStream in1(&file1);
            while(!in1.atEnd()){
              QStringList list = in1.readLine().trimmed().split(f.separator, QString::SkipEmptyParts);
              for(int i = 0; i < list.size(); i++)
                varname.append(list[i]);
            }
            file1.close();
          }
          else{ // generating 
            varname.append(QString("Label"));
            for(int i = 0; i < line.split(f.separator, QString::SkipEmptyParts).size(); i++){
              varname.append(QString("var%0").arg(i+1));
            }
          }
          getvarname = false;
        }
        QStringList list = line.split(f.separator, QString::SkipEmptyParts);
        for(col = 0; col < (size_t)list.size(); col++){
          setMatrixValue(data, row, col, list[col].toFloat());
        }
        row++;
      }
    }
  }

  file.close();
}

void QDATAIO::QImportArray(FILES f, array *data, QStringList &varname)
{
  QFile file(f.filename);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  size_t row, col, col_t;
  row = col = 0;
  while(!file.atEnd()){
    QString line = file.readLine();
    if(line.isEmpty() || line.contains(f.skiplineby, Qt::CaseInsensitive) == true){
      continue;
    }
    else{
      if(line.compare("-") == 0 && line.size() == 1){
        break;
      }
      else{
        QStringList list = line.split(f.separator, QString::SkipEmptyParts);
        col_t = list.size()-1; // -1 because the first column is the object name
        row++;
        if(col_t > col)
          col = col_t;
        else
          continue;
      }
    }
  }

  
  file.seek(0);
  
  matrix *m;
  NewMatrix(&m, row, col);
  
  
  bool getvarname = true;
  
  row = col = 0;
  size_t skipnline = 0;
  while(!file.atEnd()){
    QString line = file.readLine().trimmed();
    
    if(getvarname == true){
      if(f.firstrowvarname == true && f.filevarname.isEmpty()){
        varname = line.split(f.separator, QString::SkipEmptyParts);
      }
      else if(f.firstrowvarname == false && !f.filevarname.isEmpty()){
        QFile file1(f.filevarname);
        if (!file1.open(QIODevice::ReadOnly | QIODevice::Text))
          return;

        QTextStream in1(&file1);
        while(!in1.atEnd()){
          QStringList list = in1.readLine().trimmed().split(f.separator, QString::SkipEmptyParts);
          for(int i = 0; i < list.size(); i++)
            varname.append(list[i]);
        }
        file1.close();
      }
      else{ // generating 
        varname.append(QString("Label"));
        for(int i = 0; i < line.split(f.separator, QString::SkipEmptyParts).size(); i++){
          varname.append(QString("var%0").arg(i+1));
        }
      }
      getvarname = false;
    }
    else{    
      if(line.isEmpty() || line.contains(f.skiplineby, Qt::CaseInsensitive) == true){
        continue;
      }
      else{
        if(f.nline > 0 && skipnline < f.nline){
          skipnline++;
        }
        else{
          if(line.compare("-") == 0 && line.size() == 1){
            ArrayAppendMatrix(&data, m);
            MatrixSet(m, 0.f);
            row = 0;
          }
          else{
            if(f.nline > 0 && skipnline < f.nline){
              skipnline++;
            }
            else{
              QStringList list = line.split(f.separator, QString::SkipEmptyParts);
              for(col = 0; col < (size_t)list.size(); col++){
                setMatrixValue(m, row, col, list[col].toFloat());
              }
              row++;
            }
          }
          
        }
      }
    }
  }
  
  ArrayAppendMatrix(&data, m);
  DelMatrix(&m);
  file.close();
}
