#include "ImportFileDialog.h"
#include "qstudiometricstypes.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QtCore/qvarlengtharray.h>
#include <QString>
#include <QTextStream>
#include <QtConcurrent/QtConcurrentRun>
#include <QFutureWatcher>

/*
void ImportFileDialog::BuildArray()
{
  FSIZE sz = GetSize();
  size_t ncols;
  if(ui.firstcolobjname->isChecked() == true){
    ncols = (sz.col-1)/ui.levelBox->value();
  }
  else{
    ncols = sz.col/ui.levelBox->value();
  }

  array *_a_ = a->Array();
  for(int i = 0; i < ui.levelBox->value(); i++){
    AddArrayMatrix(&_a_, sz.row, ncols);
  }

  a->setName(ui.filename->text());

  matrix *_m_ = m->Matrix();
  ResizeMatrix(&_m_, sz.row, sz.col);
  a->setName(ui.filename->text());

  QFile f(ui.file->text());
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  a->getVarName().append("Object Names");

  size_t row = 0;
  QTextStream in(&f);
  while(!in.atEnd()){
    QString line = in.readLine();
    if(QString(line[0]).compare(getSkipChar(), Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line starting with
      continue;
    }
    else{
      QStringList items = line.split(getSeparator());
      if(row == 0 && ui.firstrowvarname->isChecked() == true){
        items.removeFirst();
        a->getVarName().append(items);
        row++;
      }
      else{
        if(ui.firstcolobjname->isChecked() == true && ui.firstrowvarname->isChecked() == true){
          a->getObjName().append(items.first());
          size_t col = 0;
          int layer = 0;
          for(int j = 1; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            if(col < ncols){
              if(converted == true){
                setArrayValue(a->Array(), layer, row-1, col, val);
              }
              else{
                setArrayValue(a->Array(), layer, row-1, col, -9999);
              }
              col++;
            }
            else{
              layer++;
              col = 0;
              if(converted == true){
                setArrayValue(a->Array(), layer, row-1, col, val);
              }
              else{
                setArrayValue(a->Array(), layer, row-1, col, -9999);
              }
            }
          }
        }
        else if(ui.firstcolobjname->isChecked() == true && ui.firstrowvarname->isChecked() == false){
          a->getObjName().append(items.first());
          size_t col = 0;
          int layer = 0;
          for(int j = 1; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            if(col < ncols){
              if(converted == true){
                setArrayValue(a->Array(), layer, row, col, val);
              }
              else{
                setArrayValue(a->Array(), layer, row, col, -9999);
              }
              col++;
            }
            else{
              layer++;
              col = 0;
              if(converted == true){
                setArrayValue(a->Array(), layer, row, col, val);
              }
              else{
                setArrayValue(a->Array(), layer, row, col, -9999);
              }
            }
          }
        }
        else if(ui.firstcolobjname->isChecked() == false && ui.firstrowvarname->isChecked() == true){
          a->getObjName().append(items.first());
          size_t col = 0;
          int layer = 0;
          for(int j = 0; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            if(col < ncols){
              if(converted == true){
                setArrayValue(a->Array(), layer, row-1, col, val);
              }
              else{
                setArrayValue(a->Array(), layer, row-1, col, -9999);
              }
              col++;
            }
            else{
              layer++;
              col = 0;
              if(converted == true){
                setArrayValue(a->Array(), layer, row-1, col, val);
              }
              else{
                setArrayValue(a->Array(), layer, row-1, col, -9999);
              }
            }
          }
        }
        else{
          size_t col = 0;
          int layer = 0;
          for(int j = 0; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            if(col < ncols){
              if(converted == true){
                setArrayValue(a->Array(), layer, row, col, val);
              }
              else{
                setArrayValue(a->Array(), layer, row, col, -9999);
              }
              col++;
            }
            else{
              layer++;
              col = 0;
              if(converted == true){
                setArrayValue(a->Array(), layer, row, col, val);
              }
              else{
                setArrayValue(a->Array(), layer, row, col, -9999);
              }
            }
          }
        }
        row++;
      }
    }
  }
  f.close();


  if(a->getVarName().size() == 1){
    for(size_t i = 0; i < a->Array()->m[0]->col; i++)
      a->getVarName().append(QString("Var %1").arg(i+1));
  }

  if(a->getObjName().size() == 0){
    for(size_t i = 0; i < a->Array()->m[0]->row; i++)
      a->getObjName().append(QString("Objects %1").arg(i+1));
  }

  if(ui.transpose->isChecked() == true){
    array *at;
    array *_a_ = a->Array();
    NewArray(&at, _a_->order);
    for(uint i = 0; i < _a_->order; i++){
      NewArrayMatrix(&_a_, i, _a_->m[i]->col, _a_->m[i]->row);
    }
    ArrayTranspose(_a_, at);

    ArrayCopy(at, &_a_);
    DelArray(&at);

    QStringList tmp = a->getObjName();
    a->getVarName().removeFirst();
    a->getObjName() = a->getVarName();
    a->getVarName().clear();
    a->getVarName().append("Object Names");
    a->getVarName().append(tmp);
  }

  for(size_t j = 0; j < ncols; j++){
    for(size_t k = 0; k < a->Array()->order; k++){
      bool make_correction = false;
      int empty = 0;
      double mean = 0.f;
      for(size_t i = 0; i < a->Array()->m[k]->row; i++){
        if(FLOAT_EQ(getArrayValue(a->Array(), k, i, j), -9999, EPSILON)){
          make_correction = true;
          empty++;
        }
        else{
          mean += getArrayValue(a->Array(), k, i, j);
        }
        // QApplication::processEvents();
      }

      if(make_correction == true){
        mean /= (a->Array()->m[k]->row - empty);
        for(size_t i = 0; i < a->Array()->m[k]->row; i++){
          if(FLOAT_EQ(getArrayValue(a->Array(), k, i, j), -9999, EPSILON)){
            setArrayValue(a->Array(), k, i, j, mean);
          }
          else{
            continue;
          }
          // QApplication::processEvents();
        }
      }
      else{
        continue;
      }
    }
  }
  a->GenHash();
}
*/

void ImportFileDialog::AssignName(QStringList &list, QString name)
{
  list.append(name);
  /* TOO SLOW for big datasets!!!!
  if(list.contains(name) == true){
    int i = 1;
    while(list.contains(QString("%1_%2").arg(name).arg(QString::number(i)), Qt::CaseSensitive) == true){
      i++;
    }
    list.append(QString("%1_%2").arg(name).arg(QString::number(i)));
  }
  else{
    list.append(name);
  }*/
}

//if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == true)
void ImportFileDialog::ImportType0()
{
  QFileInfo info(ui.file->text());
  if(info.exists()){
    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    AssignName(m->getVarName(), "Object Names");
    size_t row = 0;
    QString skipchar = getSkipChar();
    QString sep = getSeparator();
    QTextStream in(&f);
    while(!in.atEnd()){
      QString line = in.readLine();
      if(QString(line[0]).compare(skipchar, Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line empty or starting with a skip char
        continue;
      }
      else{
        if(row == 0){
          QStringList items = line.split(sep);
          for(int j = 1; j < items.size(); j++){
            AssignName(m->getVarName(), items[j]);
          }
        }
        else{
          QStringList items = line.split(sep);
          AssignName(m->getObjName(), items[0]);
          for(int j = 1; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            m->Matrix()->data[row-1][j-1] = (converted == true) ? val : -9999.;
          }
        }
        row++;
      }
    }
    f.close();
  }
}


// if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == false)
void ImportFileDialog::ImportType1()
{
  QFileInfo info(ui.file->text());
  if(info.exists()){
    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    AssignName(m->getVarName(), "Objects");
    size_t row = 0;
    QString skipchar = getSkipChar();
    QString sep = getSeparator();
    QTextStream in(&f);
    while(!in.atEnd()){
      QString line = in.readLine();
      if(QString(line[0]).compare(skipchar, Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line empty or starting with a skip char
        continue;
      }
      else{
        if(row == 0){
          QStringList items = line.split(sep);
          for(int j = 1; j < items.size(); j++){
            AssignName(m->getVarName(), items[j]);
          }
        }
        else{
          QStringList items = line.split(sep);
          for(int j = 0; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            m->Matrix()->data[row-1][j] = (converted == true) ? val : -9999.;
          }
        }
        row++;
      }
    }
    f.close();
  }
}

//else if(ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == true){
void ImportFileDialog::ImportType2()
{
  QFileInfo info(ui.file->text());
  if(info.exists()){
    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    AssignName(m->getVarName(), "Objects");
    size_t row = 0;
    QString skipchar = getSkipChar();
    QString sep = getSeparator();
    QTextStream in(&f);
    while(!in.atEnd()){
      QString line = in.readLine();
      if(QString(line[0]).compare(skipchar, Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line empty or starting with a skip char
        continue;
      }
      else{
        QStringList items = line.split(sep);
        AssignName(m->getObjName(), items[0]);
        for(int j = 1; j < items.size(); j++){
          bool converted;
          double val = items[j].replace(",",".").toDouble(&converted);
          m->Matrix()->data[row][j-1] = (converted == true) ? val : -9999.;
        }
        row++;
      }
    }
    f.close();
  }
}

// ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == false
void ImportFileDialog::ImportType3()
{
  QFileInfo info(ui.file->text());
  if(info.exists()){
    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    AssignName(m->getVarName(), "Objects");
    size_t row = 0;
    QString skipchar = getSkipChar();
    QString sep = getSeparator();
    QTextStream in(&f);
    while(!in.atEnd()){
      QString line = in.readLine();
      if(QString(line[0]).compare(skipchar, Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line empty or starting with a skip char
        continue;
      }
      else{
        QStringList items = line.split(sep);
        for(int j = 0; j < items.size(); j++){
          bool converted;
          double val = items[j].replace(",",".").toDouble(&converted);
          m->Matrix()->data[row][j] = (converted == true) ? val : -9999.;
        }
        row++;
      }
    }
    f.close();
  }
}

void ImportFileDialog::BuildMatrix()
{
  // Get the file size and build the empty matrix
  FSIZE sz = GetSize();
  ResizeMatrix(&m->Matrix(), sz.row, sz.col);
  // Assign the matrix name
  m->setName(ui.filename->text());

  // Import the matrix
  if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == true){
    ImportType0();
  }
  else if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == false){
    ImportType1();
  }
  else if(ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == true){
    ImportType2();
  }
  else{ //ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == false
    ImportType3();
  }

  // verify the variable names
  if(m->getVarName().size() == 1){
    for(size_t i = 0; i < m->Matrix()->col; i++)
      m->getVarName().append(QString("Var %1").arg(i+1));
  }

  // verify the object names
  if(m->getObjName().size() == 0){
    for(size_t i = 0; i < m->Matrix()->row; i++)
      m->getObjName().append(QString("Objects %1").arg(i+1));
  }

  // Do some other operation
  if(ui.transpose->isChecked()){
    matrix *mt;
    NewMatrix(&mt, m->Matrix()->col, m->Matrix()->row);
    MatrixTranspose(m->Matrix(), mt);
    MatrixCopy(mt, &m->Matrix());
    DelMatrix(&mt);
    QStringList tmp = m->getObjName();
    m->getVarName().removeFirst();
    m->getObjName() = m->getVarName();
    m->getVarName().clear();
    m->getVarName().append("Object Names");
    m->getVarName().append(tmp);
  }

  // Check if there are nan value and substitute with the average.
  for(size_t j = 0; j < m->Matrix()->col; j++){
    double mean = 0.f;
    int empty = 0;
    for(int i = 0; i < (int)m->Matrix()->row; i++){
      if(FLOAT_EQ(getMatrixValue(m->Matrix(), i, j), -9999, EPSILON)){
        empty++;
      }
      else{
        mean += getMatrixValue(m->Matrix(), i, j);
      }
    }

    mean /= (m->Matrix()->row - empty);

    if(_isnan_(mean))
      mean = 0.f;

    for(int i = 0; i < (int)m->Matrix()->row; i++){
      if(FLOAT_EQ(getMatrixValue(m->Matrix(), i, j), -9999, EPSILON)){
        setMatrixValue(m->Matrix(), i, j, mean);
      }
      else{
        continue;
      }
    }
  }
}

/*
void ImportFileDialog::BuildMatrix()
{
  FSIZE sz = GetSize();
  matrix *_m_ = m->Matrix();
  ResizeMatrix(&_m_, sz.row, sz.col);
  m->setName(ui.filename->text());

  int import_type;
  if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == true){
    import_type = 0;
  }
  else if(ui.firstrowvarname->isChecked() == true && ui.firstcolobjname->isChecked() == false){
    import_type = 1;
  }
  else if(ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == true){
    import_type = 2;
  }
  else{ //ui.firstrowvarname->isChecked() == false && ui.firstcolobjname->isChecked() == false
    import_type = 3;
  }

  FILE *file = NULL;

  QString delims = getSeparator();
  QString skipchar = getSkipChar();
  size_t row = 0;

  char *filename = (char*)ui.file->text().toLatin1().data();
  if ((file = fopen(filename, "r")) == NULL){
    fprintf(stdout, "Error: Can't open file %s!\n", filename);
    return;
  }

  char *buffer = new char(sz.linelenght);
  int field = 0;
  char *tl = NULL;

  switch (import_type) {
    case(0):
      m->getVarName().append("Object Names");
      while(fgets(buffer, sz.linelenght, file)){
        if(QString(buffer[0]).compare(skipchar, Qt::CaseInsensitive) == 0){
          continue;
        }
        else{
          if(row > sz.row){
            break;
          }
          else{
            field = 0;
            if(row == 0){
              for(tl = strtok(buffer, delims.toStdString().c_str()), field = 0; tl; tl = strtok (NULL, delims.toStdString().c_str()), field++){
                // Set no of fields according to your requirement
                if(field > sz.col)
                  break;
                else if(field == 0)
                  continue;
                else{
                  AssignName(m->getVarName(), QString(tl));
                }
              }
            }
            else{
              for(tl = strtok(buffer, delims.toStdString().c_str()), field = 0; tl; tl = strtok (NULL, delims.toStdString().c_str()), field++){
                // Set no of fields according to your requirement
                if(field > sz.col)
                  break;
                else if(field == 0)
                  AssignName(m->getObjName(),  QString(tl));
                else{
                  bool converted;
                  double val = QString(tl).replace(",",".").toDouble(&converted);
                  //qDebug() << val << row-1 << field-1;
                  _m_->data[row-1][field-1] = (converted == true) ? val : -9999.;

                  //_m_->data[row-1][field-1] = atof(tl);
                }
              }
            }
            row++;
          }
        }
      }
      break;
    case(1): // only first row var names
      m->getVarName().append("Object Names");
      while(fgets(buffer, sz.linelenght, file)){
        if(QString(buffer[0]).compare(skipchar, Qt::CaseInsensitive) == 0){
          continue;
        }
        else{
          if(row > sz.row){
            break;
          }
          else{
            field = 0;
            if(row == 0){
              for(tl = strtok(buffer, " \t"), field = 0; tl; tl = strtok (NULL, " \t"), field++){
                // Set no of fields according to your requirement
                if(field > sz.col)
                  break;
                else if(field == 0)
                  continue;
                else{
                  AssignName(m->getVarName(), QString(tl));
                }
              }
            }
            else{
              for(tl = strtok(buffer, delims.toStdString().c_str()), field = 0; tl; tl = strtok (NULL, delims.toStdString().c_str()), field++){
                // Set no of fields according to your requirement
                if(field > sz.col)
                  break;
                else{
                  bool converted;
                  double val = QString(tl).replace(",",".").toDouble(&converted);
                  _m_->data[row-1][field] = (converted == true) ? val : -9999;
                }
              }
            }
            row++;
          }
        }
      }
      break;
    case(2): // only first column obj name
      m->getVarName().append("Object Names");
      while(fgets(buffer, sz.linelenght, file)){
        if(QString(buffer[0]).compare(skipchar, Qt::CaseInsensitive) == 0){
          continue;
        }
        else{
          if(row > sz.row){
            break;
          }
          else{
            for(tl = strtok(buffer, delims.toStdString().c_str()), field = 0; tl; tl = strtok (NULL, delims.toStdString().c_str()), field++){
              // Set no of fields according to your requirement
              if(field > sz.col){
                break;
              }
              else if(field == 0){
                AssignName(m->getObjName(),  QString(tl));
              }
              else{
                bool converted;
                double val = QString(tl).replace(",",".").toDouble(&converted);
                _m_->data[row][field-1] = (converted == true) ? val : -9999;
              }
            }
            row++;
          }
        }
      }
      break;
    case(3):
      m->getVarName().append("Object Names");
      while(fgets(buffer, sz.linelenght, file) != NULL){
        if (strstr(buffer, skipchar.toStdString().c_str()) != NULL){
          continue;
        }
        else{
          if(row > sz.row){
            break;
          }
          else{
            for(tl = strtok(buffer, delims.toStdString().c_str()), field = 0; tl; tl = strtok (NULL, delims.toStdString().c_str()), field++){
              // Set no of fields according to your requirement
              if(field > sz.col){
                break;
              }
              else{
                bool converted;
                double val = QString(tl).replace(",",".").toDouble(&converted);
                _m_->data[row][field] = (converted == true) ? val : -9999;
              }
            }
            row++;
          }
        }
      }
      break;
  }
  delete [] buffer;
  fclose(file);

  if(m->getVarName().size() == 1){
    for(size_t i = 0; i < m->Matrix()->col; i++)
      m->getVarName().append(QString("Var %1").arg(i+1));
  }

  if(m->getObjName().size() == 0){
    for(size_t i = 0; i < m->Matrix()->row; i++)
      m->getObjName().append(QString("Objects %1").arg(i+1));
  }

  if(ui.transpose->isChecked()){
    matrix *mt;
    NewMatrix(&mt, _m_->col, _m_->row);
    MatrixTranspose(_m_, mt);
    MatrixCopy(mt, &_m_);
    DelMatrix(&mt);
    QStringList tmp = m->getObjName();
    m->getVarName().removeFirst();
    m->getObjName() = m->getVarName();
    m->getVarName().clear();
    m->getVarName().append("Object Names");
    m->getVarName().append(tmp);
  }

  for(size_t j = 0; j < _m_->col; j++){
    double mean = 0.f;
    int empty = 0;
    for(int i = 0; i < (int)_m_->row; i++){
      if(FLOAT_EQ(getMatrixValue(_m_, i, j), -9999, EPSILON)){
        empty++;
      }
      else{
        mean += getMatrixValue(_m_, i, j);
      }
      // QApplication::processEvents();
    }

    mean /= (_m_->row - empty);

    if(_isnan_(mean))
      mean = 0.f;

    for(int i = 0; i < (int)_m_->row; i++){
      if(FLOAT_EQ(getMatrixValue(_m_, i, j), -9999, EPSILON)){
        setMatrixValue(_m_, i, j, mean);
      }
      else{
        continue;
      }
      // QApplication::processEvents();
    }
  }
  m->GenHash();
}
*/

/*
void ImportFileDialog::BuildMatrix()
{
  FSIZE sz = GetSize();

  matrix *_m_ = m->Matrix();
  ResizeMatrix(&_m_, sz.row, sz.col);
  m->setName(ui.filename->text());

  QFile f(ui.file->text());
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  m->getVarName().append("Object Names");

  size_t row = 0, row_ = 0;
  QTextStream in(&f);
  while(!in.atEnd()){
    QString line = in.readLine();
    if(QString(line[0]).compare(getSkipChar(), Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line starting with
      continue;
    }
    else{
      QStringList items = line.split(getSeparator());
      if(row == 0){
        if(ui.firstrowvarname->isChecked() == true){
          items.removeFirst();
          for(int i = 0; i < items.size(); i++){
            AssignName(m->getVarName(), items[i]);
          }
        }
        else{
        if(ui.firstcolobjname->isChecked() == true){
          AssignName(m->getObjName(), items.first());
          for(int j = 1; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            if(converted == true){
              m->Matrix()->data[row_][j-1] = val;
            }
            else{
              m->Matrix()->data[row_][j-1] = -9999;
            }
          }
        }
        else{
          for(int j = 0; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            if(converted == true){
              m->Matrix()->data[row_][j] = val;
            }
            else{
              m->Matrix()->data[row_][j] = val;
            }
          }
        }
        row_++;
        }
      }
      else{
        if(ui.firstcolobjname->isChecked() == true){
          AssignName(m->getObjName(), items.first());
          for(int j = 1; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            if(converted == true){
              setMatrixValue(m->Matrix(), row_, j-1, val);
//               m->Matrix()->data[row_][j-1] = val;
            }
            else{
              m->Matrix()->data[row_][j-1] = -9999;
            }
          }
        }
        else{
          for(int j = 0; j < items.size(); j++){
            bool converted;
            double val = items[j].replace(",",".").toDouble(&converted);
            if(converted == true){
              m->Matrix()->data[row_][j] = val;
            }
            else{
              m->Matrix()->data[row_][j] = val;
            }
          }
        }
        row_++;
      }
      row++;
    }
  }
  f.close();

  if(m->getVarName().size() == 1){
    for(size_t i = 0; i < m->Matrix()->col; i++)
      m->getVarName().append(QString("Var %1").arg(i+1));
  }

  if(m->getObjName().size() == 0){
    for(size_t i = 0; i < m->Matrix()->row; i++)
      m->getObjName().append(QString("Objects %1").arg(i+1));
  }

  if(ui.transpose->isChecked()){
    matrix *mt;
    NewMatrix(&mt, _m_->col, _m_->row);
    MatrixTranspose(_m_, mt);
    MatrixCopy(mt, &_m_);
    DelMatrix(&mt);
    QStringList tmp = m->getObjName();
    m->getVarName().removeFirst();
    m->getObjName() = m->getVarName();
    m->getVarName().clear();
    m->getVarName().append("Object Names");
    m->getVarName().append(tmp);
  }

  for(size_t j = 0; j < _m_->col; j++){
    double mean = 0.f;
    int empty = 0;
    for(int i = 0; i < (int)_m_->row; i++){
      if(FLOAT_EQ(getMatrixValue(_m_, i, j), -9999, EPSILON)){
        empty++;
      }
      else{
        mean += getMatrixValue(_m_, i, j);
      }
      // QApplication::processEvents();
    }

    mean /= (_m_->row - empty);

    if(_isnan_(mean))
      mean = 0.f;

    for(int i = 0; i < (int)_m_->row; i++){
      if(FLOAT_EQ(getMatrixValue(_m_, i, j), -9999, EPSILON)){
        setMatrixValue(_m_, i, j, mean);
      }
      else{
        continue;
      }
      // QApplication::processEvents();
    }
  }
  m->GenHash();
}
*/

void ImportFileDialog::Preview()
{
  QList < QStringList > fpreview;

  QFile f(ui.file->text());
  if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QStringList objnames, varnames;
  varnames << "Objects";

  size_t row = 0, max_row = 10;
  QTextStream in(&f);
  while(!in.atEnd()){

    if(row == max_row){
      break;
    }

    QString line = in.readLine();
    if(QString(line[0]).compare(getSkipChar(), Qt::CaseInsensitive) == 0 || line.isEmpty()){ // skip line starting with
      continue;
    }
    else{
      QStringList items = line.split(getSeparator());
      if(row == 0){
        if(ui.firstrowvarname->isChecked() == true){
          items.removeFirst();
          for(int i = 0; i < items.size(); i++){
            AssignName(varnames, items[i]);
          }
        }
        else{
          if(ui.firstcolobjname->isChecked() == true){
            AssignName(objnames, items.first());
            fpreview.append(QStringList());
            for(int j = 1; j < items.size(); j++){
              fpreview.last().append(items[j]);
            }
          }
          else{
            fpreview.append(QStringList());
            for(int j = 0; j < items.size(); j++){
              fpreview.last().append(items[j]);
            }
          }
        }
      }
      else{
        if(ui.firstcolobjname->isChecked() == true){
          AssignName(objnames, items.first());
          fpreview.append(QStringList());
          for(int j = 1; j < items.size(); j++){
            fpreview.last().append(items[j]);
          }
        }
        else{
          fpreview.append(QStringList());
          for(int j = 0; j < items.size(); j++){
            fpreview.last().append(items[j]);
          }
        }
      }
      row++;
    }
  }
  f.close();

  if(varnames.size() == 1 && fpreview.size() > 0){
    for(int i = 0; i < fpreview.first().size(); i++)
      varnames.append(QString("Var %1").arg(i+1));
  }

  if(objnames.size() == 0){
    for(int i = 0; i < fpreview.size(); i++)
      objnames.append(QString("Objects %1").arg(i+1));
  }

  if(ui.transpose->isChecked() && fpreview.size() > 0){
    QList<QStringList> fpreview_t;
    // deep copy
    for(int j = 0; j < fpreview.first().size(); j++){
      fpreview_t.append(QStringList());
      for(int i = 0; i < fpreview.size(); i++){
        if(j < fpreview[i].size()){
          fpreview_t.last().append(fpreview[i][j]);
        }
        else{
          continue;
        }
      }
    }
    fpreview.clear();
    fpreview = fpreview_t;

    QStringList tmp = objnames;
    varnames.removeFirst();
    objnames = varnames;
    varnames.clear();
    varnames.append("Object Names");
    varnames.append(tmp);
  }

  model->clear();

  model->setHorizontalHeaderLabels(varnames);
  for(int i = 0; i < fpreview.size(); i++){
    QList<QStandardItem*> row;
    row.append(new QStandardItem(objnames[i]));
    for(int j = 0; j < fpreview[i].size(); j++){
      row.append(new QStandardItem(fpreview[i][j]));
    }
    model->appendRow(row);
  }

  if(row == max_row && fpreview.size() > 0){
    QList<QStandardItem*> row;
    row.append(new QStandardItem("...."));
    for(int j = 0; j < fpreview.last().size(); j++){
      row.append(new QStandardItem("...."));
    }
  }
}

FSIZE ImportFileDialog::GetSize()
{
  FSIZE sz;
  sz.row = sz.col = sz.linelenght = 0;
  QFileInfo info(ui.file->text());
  if(info.exists()){

    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return sz;

    QString skipchar = getSkipChar();
    QString sep = getSeparator();
    QTextStream in(&f);
    while(!in.atEnd()){
      QString line = in.readLine();
      if(QString(line[0]).compare(skipchar) == 0 || line.isEmpty()){ // skip line empty or starting with a skip char
        continue;
      }
      else{
        sz.row++;
        size_t col = line.split(sep).size();
        size_t linelenght = line.size();
        if(col > sz.col)
          sz.col = col;

        if(linelenght+3 > sz.linelenght)
          sz.linelenght = linelenght+3;
      }
    }
    f.close();
  }

  if(ui.firstcolobjname->isChecked() == true)
    sz.col -= 1;

  if(ui.firstrowvarname->isChecked() == true)
    sz.row -= 1;

  return sz;
}

QString ImportFileDialog::getFileName()
{
  return ui.file->text();
}

QString ImportFileDialog::getLabel()
{
  if(ui.filename->text().compare("project") != 0) // project name is a reserved name in order to recognize a speicific kind of data in the qtreeview
    return ui.filename->text();
  else
    return ui.filename->text().append("_");
}

QString ImportFileDialog::getSeparator()
{
  if(ui.splitlineby->currentIndex() == 0) // tab
    return "\t";
  else if(ui.splitlineby->currentIndex() == 1) // space
    return " ";
  else if(ui.splitlineby->currentIndex() == 2) // ,
    return ",";
  else if(ui.splitlineby->currentIndex() == 3) // ;
    return ";";
  else{ // PERSONAL
    return ui.splitlineby->currentText();
  }
}

QString ImportFileDialog::getSkipChar()
{
  return ui.ignorelinebychar->text();
}

void ImportFileDialog::Cancel()
{
  return reject();
}

void ImportFileDialog::OK()
{
  if(!ui.file->text().isEmpty()){
    QFileInfo info(ui.file->text());
    if(info.exists() == true){
      /*QFutureWatcher<void> futureWatcher;
      connect(&futureWatcher, SIGNAL(finished()), this,SLOT(accept()));
      futureWatcher.setFuture(QtConcurrent::run(this, &ImportFileDialog::BuildMatrix));
      futureWatcher.waitForFinished();*/

      BuildMatrix();
      accept();
    }
    else{
      QMessageBox::warning(this, tr("Warnig"), tr("The selected file could not be used because the file does not exist. Please select a file.."), QMessageBox::Ok);
      return;
    }
  }
  else{
    QMessageBox::warning(this, tr("Warnig"), tr("Please select a file.."), QMessageBox::Ok);
  }
}

void ImportFileDialog::Open()
{
  if(path.isEmpty()){
    ui.file->setText(QFileDialog::getOpenFileName( this, tr("Open File"), QDir::currentPath(), tr("Text Files (*.txt *.csv);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }
  else{
    ui.file->setText(QFileDialog::getOpenFileName( this, tr("Open File"), path, tr("Text Files (*.txt *.csv);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog ));
    QFileInfo last(ui.file->text());
    path = last.absoluteFilePath();
  }

  QStringList list = ui.file->text().split("/", QString::SkipEmptyParts);
  if(list.size() > 0){
    list.last().remove(".txt", Qt::CaseInsensitive);
    list.last().remove(".csv", Qt::CaseInsensitive);
    ui.filename->setText(list.last());
  }
  Preview();
}

ImportFileDialog::ImportFileDialog(): QDialog()
{
  ui.setupUi(this);

  m = new MATRIX;
  a = new ARRAY;

  model = new QStandardItemModel;
  ui.tableView->setModel(model);

  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui.openButton, SIGNAL(clicked(bool)), this, SLOT(Open()));
  connect(ui.firstcolobjname, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.firstrowvarname, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.transpose, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.splitlineby, SIGNAL(currentIndexChanged(int)), SLOT(Preview()));
  connect(ui.ignorelinebychar, SIGNAL(textChanged(QString)), SLOT(Preview()));
}

ImportFileDialog::~ImportFileDialog()
{
  delete m;
  delete a;
  delete model;
}
