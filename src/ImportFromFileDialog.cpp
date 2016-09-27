#include "ImportFromFileDialog.h"
#include "qstudiometricstypes.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringListModel>
#include <QTextStream>

void StringListModel::AppendRow(QString row)
{
  int position  = stringList.size();
  beginInsertRows(QModelIndex(), position, position+1);
  stringList.append(row);
  endInsertRows();
}

void StringListModel::AppendRows(QStringList rows)
{
  int position  = stringList.size();
  beginInsertRows(QModelIndex(), position, position+rows.size()-1);

  stringList.append(rows);

  endInsertRows();
}

void StringListModel::Clear()
{
  stringList.clear();
}

int StringListModel::rowCount(const QModelIndex &parent) const
{
  return stringList.count();
}

QVariant StringListModel::data(const QModelIndex &index, int role) const
{
  if(!index.isValid())
    return QVariant();

  if (index.row() >= stringList.size())
    return QVariant();

  if (role == Qt::DisplayRole)
    return stringList.at(index.row());
  else
    return QVariant();
}

QVariant StringListModel::headerData(int section, Qt::Orientation orientation,  int role) const
{
  if (role != Qt::DisplayRole)
      return QVariant();

  if (orientation == Qt::Horizontal)
      return QString("Column %1").arg(section);
  else
      return QString("Row %1").arg(section);
}

Qt::ItemFlags StringListModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
      return Qt::ItemIsEnabled;

  return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool StringListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
  if (index.isValid() && role == Qt::EditRole) {
    stringList.replace(index.row(), value.toString());
    emit dataChanged(index, index);
    return true;
  }

  return false;
}

bool StringListModel::insertRows(int position, int rows, const QModelIndex &parent)
{
  beginInsertRows(QModelIndex(), position, position+rows-1);

  for (int row = 0; row < rows; ++row) {
    stringList.insert(position, "");
  }

  endInsertRows();
  return true;

}

bool StringListModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  beginRemoveRows(QModelIndex(), position, position+rows-1);

  for (int row = 0; row < rows; ++row) {
    stringList.removeAt(position);
  }

  endRemoveRows();
  return true;
}

void ImportFromFileDialog::BuildArray()
{
  if(file.size() > 0){
    
    QList< int > rows, cols; // rows and cols to get...
    QList< int > empty_cols;
    
    if(ui.listView_2->model()->rowCount() == ui.listView_2->selectionModel()->selectedRows().size()){
      for(int i = 0; i < ui.listView_2->model()->rowCount(); i++){
        rows.append(i);
        a->getObjName().append(tab1->getStringList());
        // QApplication::processEvents();
      }
    }
    else{
      for(int i = 0; i < ui.listView_2->selectionModel()->selectedRows().size(); i++){
        rows.append(ui.listView_2->selectionModel()->selectedRows()[i].row());
        a->getObjName().append(ui.listView_2->selectionModel()->selectedRows()[i].data(Qt::DisplayRole).toString());
        // QApplication::processEvents();
      }
    }
    
    a->getVarName().append("Object Names");
    if(ui.listView_3->model()->rowCount() == ui.listView_3->selectionModel()->selectedRows().size()){
      for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
        cols.append(i);
        a->getVarName().append(tab2->getStringList());
        // QApplication::processEvents();
      }
    }
    else{
      for(int i = 0; i < ui.listView_3->selectionModel()->selectedRows().size(); i++){
        cols.append(ui.listView_3->selectionModel()->selectedRows()[i].row());
        a->getVarName().append(ui.listView_3->selectionModel()->selectedRows()[i].data(Qt::DisplayRole).toString());
        // QApplication::processEvents();
      }
    }
    
    array *_a_ = a->Array();
    for(int i = 0; i < ui.levelBox->value(); i++){
      AddArrayMatrix(&_a_, rows.size(), cols.size());
    }
    
    a->setName(ui.filename->text());
    
    size_t ncols;
    if(FirstColumnObjNames() == true){
      if(file.size() > 1){
        ncols = (file[1].size()-1)/ui.levelBox->value();
      }
      else{
        ncols = (file[0].size()-1)/ui.levelBox->value();
      }
    }
    else{
      if(file.size() > 1){
        ncols = file[1].size()/ui.levelBox->value();
      }
      else{
        ncols = file[0].size()/ui.levelBox->value();
      }
    }
    
    if(FirstRowVarNames() == true){
      for(int i = 0; i < rows.size(); i++){
        if(FirstColumnObjNames() == true){
          for(int c = 0; c < ui.levelBox->value(); c++){
            int l = 0;
            for(int j = 0; j < cols.size(); j++){
              bool converted;
              int col = (cols[j]+1)+(ncols*c);
              /*
              qDebug() << QString("i %1").arg(i);
              qDebug() << QString("ncols %1").arg(ncols);
              qDebug() << QString("c %1").arg(c);
              
              qDebug() << QString("file_row %1").arg(QString::number(file.size()));
              qDebug() << QString("file_col %1").arg(QString::number(file.first().size()));
              qDebug() << QString("row_to_get %1").arg(QString::number(rows[i]+1));
              qDebug() << QString("col_to_get %1").arg(QString::number(col));
              qDebug() << QString("Contains %1").arg(file[rows[i]+1][col]);
              qDebug() << "##############################";
              */
              double val = file[rows[i]+1][col].toDouble(&converted);
              if(converted == true){
                setArrayValue(a->Array(), c, i, l, val);
              }
              else{
                if(empty_cols.contains(l) == false){
                  empty_cols.append(l);
                  setArrayValue(a->Array(), c, i, l, -9999);
                }
                else{
                  setArrayValue(a->Array(), c, i, l, -9999);
                }
              }
              l++;
              // QApplication::processEvents();
            }
          }
        }
        else{
          for(int c = 0; c < ui.levelBox->value(); c++){
            int l = 0;
            for(int j = 0; j < cols.size(); j++){
              bool converted;
              int col = cols[j]+(ncols*c);
              double val = file[rows[i]+1][col].toDouble(&converted);
              
              if(converted == true){
                setArrayValue(a->Array(), c, i, l, val);
              }
              else{
                if(empty_cols.contains(l) == false){
                  empty_cols.append(l);
                  setArrayValue(a->Array(), c, i, l, -9999);
                }
                else{
                  setArrayValue(a->Array(), c, i, l, -9999);
                }
              }
              l++;
              // QApplication::processEvents();
            }
          }
        }
      }
    }
    else{
      for(int i = 0; i < rows.size(); i++){
        if(FirstColumnObjNames() == true){
          for(int c = 0; c < ui.levelBox->value(); c++){
            int l = 0;
            for(int j = 0; j < cols.size(); j++){
              bool converted;
              int col = (cols[j]+1)+(ncols*c);
              double val = file[rows[i]][col].toDouble(&converted);
              
              if(converted == true){
                setArrayValue(a->Array(), c, i, l, val);
              }
              else{
                if(empty_cols.contains(l) == false){
                  empty_cols.append(l);
                  setArrayValue(a->Array(), c, i, l, -9999);
                }
                else{
                  setArrayValue(a->Array(), c, i, l, -9999);
                }
              }
              l++;
              // QApplication::processEvents();
            }
          }
        }
        else{
          for(int c = 0; c < ui.levelBox->value(); c++){
            int l = 0;
            for(int j = 0; j < cols.size(); j++){
              bool converted;
              int col = cols[j]+(ncols*c);
              double val = file[rows[i]][col].toDouble(&converted);
              
              if(converted == true){
                setArrayValue(a->Array(), c, i, l, val);
              }
              else{
                if(empty_cols.contains(l) == false){
                  empty_cols.append(l);
                  setArrayValue(a->Array(), c, i, l, -9999);
                }
                else{
                  setArrayValue(a->Array(), c, i, l, -9999);
                }
              }
              l++;
              // QApplication::processEvents();
            }
          }
        }
      }
    }
    
    for(int j = 0; j < empty_cols.size(); j++){
      for(size_t k = 0; k < a->Array()->order; k++){
        bool make_correction = false;      
        int empty = 0;
        double mean = 0.f;
        for(size_t i = 0; i < a->Array()->m[k]->row; i++){
          if(FLOAT_EQ(getArrayValue(a->Array(), k, i, empty_cols[j]), -9999, EPSILON)){
            make_correction = true;
            empty++;
          }
          else{
            mean += getArrayValue(a->Array(), k, i, empty_cols[j]);
          }
          // QApplication::processEvents();
        }
        
        if(make_correction == true){
          mean /= (a->Array()->m[k]->row - empty);
          for(size_t i = 0; i < a->Array()->m[k]->row; i++){
            if(FLOAT_EQ(getArrayValue(a->Array(), k, i, empty_cols[j]), -9999, EPSILON)){
              setArrayValue(a->Array(), k, i, empty_cols[j], mean);
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
  }
  a->GenHash();
}

void ImportFromFileDialog::BuildMatrix()
{
  if(file.size() > 0){
    ui.progressBar->show();
    ui.progressBar->setRange(0, 0);
    QList< int > rows, cols; // rows and cols to get...
    QList< int > empty_cols;
    
    
    if(ui.listView_2->model()->rowCount() == ui.listView_2->selectionModel()->selectedRows().size()){
      m->getObjName().append(tab1->getStringList());
      
      for(int i = 0; i < ui.listView_2->model()->rowCount(); i++){
        rows.append(i);
        // QApplication::processEvents();
      }
    }
    else{
      for(int i = 0; i < ui.listView_2->selectionModel()->selectedRows().size(); i++){
        rows.append(ui.listView_2->selectionModel()->selectedRows()[i].row());
        m->getObjName().append(ui.listView_2->selectionModel()->selectedRows()[i].data(Qt::DisplayRole).toString());
        // QApplication::processEvents();
      }
    }
    
    m->getVarName().append("Object Names");
    if(ui.listView_3->model()->rowCount() == ui.listView_3->selectionModel()->selectedRows().size()){
      m->getVarName().append(tab2->getStringList());
      for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
        cols.append(i);
        // QApplication::processEvents();
      }
    }
    else{
      for(int i = 0; i < ui.listView_3->selectionModel()->selectedRows().size(); i++){
        cols.append(ui.listView_3->selectionModel()->selectedRows()[i].row());
        m->getVarName().append(ui.listView_3->selectionModel()->selectedRows()[i].data(Qt::DisplayRole).toString());
        // QApplication::processEvents();
      }
    }
    
    /*
    for(int i = 0; i < ui.listView_2->model()->rowCount(); i++){
      if(ui.listView_2->selectionModel()->isSelected(ui.listView_2->model()->index(i, 0)) == true){
        rows.append(i);
        m->getObjName().append(ui.listView_2->model()->index(i, 0).data(Qt::DisplayRole).toString());
      }
      else{
        continue;
      }
      // QApplication::processEvents();
    }
    
    m->getVarName().append("Object Names");
    for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
      if(ui.listView_3->selectionModel()->isSelected(ui.listView_3->model()->index(i, 0)) == true){
        cols.append(i);
        m->getVarName().append(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString());
      }
      else{
        continue;
      }
      // QApplication::processEvents();
    }
    */
    
    matrix *_m_ = m->Matrix();
    ResizeMatrix(&_m_, rows.size(), cols.size()); 
    m->setName(ui.filename->text());
    
    if(FirstRowVarNames() == true){
      for(int i = 0; i < rows.size(); i++){
        if(FirstColumnObjNames() == true){
          for(int j = 0; j < cols.size(); j++){
//                         qDebug() << QString("row %1  col %2").arg(QString::number(rows[i]+1)).arg(QString::number(cols[j]+1));
            bool converted;
            double val = -9999;
            
            if(rows[i]+1 < file.size()){
              if(cols[j]+1 < file[rows[i]+1].size()){
                file[rows[i]+1][cols[j]+1].replace(",", ".");
                val = file[rows[i]+1][cols[j]+1].toDouble(&converted);
              }
              else{
                converted = false;
              }
            }
            else{
              converted = false;
            }
            
            if(converted == true){
              setMatrixValue(m->Matrix(), i, j, val);
            }
            else{
              if(empty_cols.contains(j) == false){
                empty_cols.append(j);
                setMatrixValue(m->Matrix(), i, j, -9999);
              }
              else{
                setMatrixValue(m->Matrix(), i, j, -9999);
              }
            }
            // QApplication::processEvents();
          }
        }
        else{
          for(int j = 0; j < cols.size(); j++){
            bool converted;
            double val = -9999;
            
            if(rows[i]+1 < file.size()){
              if(cols[j] < file[rows[i]+1].size()){
                file[rows[i]+1][cols[j]].replace(",",".");
                val = file[rows[i]+1][cols[j]].toDouble(&converted);
              }
              else{
                converted = false;
              }
            }
            else{
              converted = false;
            }
            
            if(converted == true){
              setMatrixValue(m->Matrix(), i, j, val);
            }
            else{
              if(empty_cols.contains(j) == true){
                setMatrixValue(m->Matrix(), i, j, -9999);
              }
              else{
                empty_cols.append(j);
                setMatrixValue(m->Matrix(), i, j, -9999);
              }
            }
            // QApplication::processEvents();
          }
        }
      }
    }
    else{
      for(int i = 0; i < rows.size(); i++){  
        if(FirstColumnObjNames() == true){
          for(int j = 0; j < cols.size(); j++){
            bool converted;
            double val = -9999;
            
            if(rows[i] < file.size()){
              if(cols[j]+1 < file[rows[i]].size()){
                file[rows[i]][cols[j]+1].replace(",",".");
                val = file[rows[i]][cols[j]+1].toDouble(&converted);
              }
              else{
                converted = false;
              }
            }
            else{
              converted = false;
            }
            
            if(converted == true){
              setMatrixValue(m->Matrix(), i, j, val);
            }
            else{
              if(empty_cols.contains(j) == true){
                setMatrixValue(m->Matrix(), i, j, -9999);
              }
              else{
                empty_cols.append(j);
                setMatrixValue(m->Matrix(), i, j, -9999);
              }
            }
            // QApplication::processEvents();
          }
        }
        else{
          for(int j = 0; j < cols.size(); j++){
            bool converted;
            double val = -9999;
            
            if(rows[i] < file.size()){
              if(cols[j] < file[rows[i]].size()){
                file[rows[i]][cols[j]].replace(",",".");
                val = file[rows[i]][cols[j]].toDouble(&converted);
              }
              else{
                converted = false;
              }
            }
            else{
              converted = false;
            }
            
            if(converted == true){
              setMatrixValue(m->Matrix(), i, j, val);
            }
            else{
              if(empty_cols.contains(j) == true){
                setMatrixValue(m->Matrix(), i, j, -9999);
              }
              else{
                empty_cols.append(j);
                setMatrixValue(m->Matrix(), i, j, -9999);
              }
            }
            // QApplication::processEvents();
          }
        }
      }
    }
    
    for(int j = 0; j < empty_cols.size(); j++){
      double mean = 0.f;
      int empty = 0;
      for(int i = 0; i < (int)m->Matrix()->row; i++){
        if(FLOAT_EQ(getMatrixValue(m->Matrix(), i, empty_cols[j]), -9999, EPSILON)){
          empty++;
        }
        else{
          mean += getMatrixValue(m->Matrix(), i, empty_cols[j]);
        }
        // QApplication::processEvents();
      }
      
      mean /= (m->Matrix()->row - empty);
      
      for(int i = 0; i < (int)m->Matrix()->row; i++){
        if(FLOAT_EQ(getMatrixValue(m->Matrix(), i, empty_cols[j]), -9999, EPSILON)){
          setMatrixValue(m->Matrix(), i, empty_cols[j], mean);
        }
        else{
          continue;
        }
        // QApplication::processEvents();
      }
    }
  }
  m->GenHash();
}

void ImportFromFileDialog::GetColumtoDiscard()
{
  QList<int> col_check;
  int row = 0;
  
  for(int k = 0; k < file.size(); k++){
    if(row == 0){
      for(int i = 0; i < file[k].size(); i++){
        col_check.append(0);
      }
    }
    
    for(int i = 0; i < file[k].size(); i++){
      bool ok;
      file[k][i].toDouble(&ok);
      if(ok == true){
        if(i < col_check.size()){
          col_check[i] += 1;
        }
        else{ // column out of range... so add others column until the column i is not reached and then plus 1
          for(int j = col_check.size(); j < i+1; j++){
            col_check.append(0);
          }
          col_check[i] += 1;
        }
      }
      else{
        continue;
      }
    }
    row++;
  }
  
  for(int i = 0; i < col_check.size(); i++){
    
    if((100 - ((col_check[i] * 100) / row)) > ui.colmissvalueBox->value()){ // skip this column... too much empty line or converted lines....
      columntoskip.append(i); 
    }
    else{
      continue;
    }
  }
}

void ImportFromFileDialog::LoadVariableNames()
{
  varnames.clear();
  ui.progressBar->show();
  ui.progressBar->setRange(0 ,0);
  
  if(FirstColumnObjNames() == true){
    if(FirstRowVarNames() == true){
      for(int j = 1; j < file.first().size(); j++){
        if(ui.skipcolmissingvalue->isChecked()){
          if(columntoskip.contains(j) == true){
            continue;
          }
          else{
            varnames.append(file.first()[j]);
          }
        }
        else{
          varnames.append(file.first()[j]);
        }
        // QApplication::processEvents();
      }
    }
    else{
      for(int j = 1; j < file.first().size(); j++){
        if(ui.skipcolmissingvalue->isChecked()){
          if(columntoskip.contains(j) == true){
            continue;
          }
          else{
            varnames.append(QString("Var %1").arg(QString::number(j)));
          }
        }
        else{
          varnames.append(QString("Var %1").arg(QString::number(j)));
        }
        // QApplication::processEvents();
      }
    }
  }
  else{
   if(FirstRowVarNames() == true){
      for(int j = 0; j < file.first().size(); j++){
        if(ui.skipcolmissingvalue->isChecked()){
          if(columntoskip.contains(j) == true){
            continue;
          }
          else{
            varnames.append(file.first()[j]);
          }
        }
        else{
          varnames.append(file.first()[j]);
        }
        // QApplication::processEvents();
      }
    }
    else{
      for(int j = 0; j < file.first().size(); j++){
        if(ui.skipcolmissingvalue->isChecked()){
          if(columntoskip.contains(j) == true){
            continue;
          }
          else{
            varnames.append(QString("Var %1").arg(QString::number(j+1)));
          }
        }
        else{
          varnames.append(QString("Var %1").arg(QString::number(j+1)));
        }
        // QApplication::processEvents();
      }
    } 
  }
  
  tab2->Clear();
  
  if(ui.matrixButton->isChecked()){
    tab2->AppendRows(varnames);
    /*
    for(int i = 0; i < varnames.size(); i++){
      QList<QStandardItem*> row;
      row.append(new QStandardItem(varnames[i]));
      tab2->appendRow(row); 
      // QApplication::processEvents();
    }
    */
  }
  else{
    int ncols = file[0].size()/ui.levelBox->value();
    for(int i = 0; i < varnames.size(); i++){
      if(i < ncols){
        tab2->AppendRow(varnames[i]);
        /*
        QList<QStandardItem*> row;
        row.append(new QStandardItem(varnames[i]));
        tab2->appendRow(row);*/
      }
      else{
        continue;
      }
      // QApplication::processEvents();
    }
  }
  ui.progressBar->hide();
  ui.progressBar->setRange(0, 1);
}

void ImportFromFileDialog::LoadObjectNames()
{
  objnames.clear();
  ui.progressBar->show();
  ui.progressBar->setRange(0 ,0);
  
  if(FirstRowVarNames() == true){
    if(FirstColumnObjNames() == true){
      for(int i = 1; i < file.size(); i++){
        objnames.append(file[i].first());
        // QApplication::processEvents();
      }
    }
    else{
      for(int i = 1; i < file.size(); i++){
        objnames.append(QString("Object %1").arg(QString::number(i)));
        // QApplication::processEvents();
      }
    }
  }
  else{
    if(FirstColumnObjNames() == true){
      for(int i = 0; i < file.size(); i++){
        objnames.append(file[i].first());
        // QApplication::processEvents();
      }
    }
    else{
      for(int i = 0; i < file.size(); i++){
        objnames.append(QString("Object %1").arg(QString::number(i+1)));
        // QApplication::processEvents();
      }
    }
  }
  
  tab1->Clear();
  
  tab1->AppendRows(objnames);
  /*
  for(int i = 0; i < objnames.size(); i++){
    tab1->appendRow(new QStandardItem(objnames[i]));
    // QApplication::processEvents();
  }
  */
  
  ui.progressBar->hide();
  ui.progressBar->setRange(0 ,1);
}


void ImportFromFileDialog::LoadFile(QString file_)
{
  QFileInfo info(file_);
  if(info.exists()){
    ui.progressBar->show();
    ui.progressBar->setRange(0, 0);
    
    for(int i = 0; i < file.size(); i++){
      file[i].clear();
    }
    file.clear();
    
    QFile f(ui.file->text());
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
      return;
    
    QTextStream in(&f);
    if(ui.startlineBox->value() != ui.stoplineBox->value() && ui.stoplineBox->value() > ui.startlineBox->value()){
      int c = 0;
      while(!in.atEnd()){
        QString line = in.readLine();
        if(c >= ui.startlineBox->value() && c <= ui.stoplineBox->value()){
          if(QString(line[0]).compare(getSkipChar(), Qt::CaseInsensitive) == 0){ // skip line starting with 
            continue;
          }
          else{
            file.append(line.split(getSeparator()));
          }
          c++;
        }
        else{
          c++;
        }
        // QApplication::processEvents();
      }
    }
    else{
      while(!in.atEnd()){
        QString line = in.readLine();
        if(QString(line[0]).compare(getSkipChar(), Qt::CaseInsensitive) == 0){ // skip line starting with 
          continue;
        }
        else{
          file.append(line.split(getSeparator()));
        }
        // QApplication::processEvents();
      }
    }
    f.close();
  }
  
  ui.progressBar->hide();
  ui.progressBar->setRange(0, 1);
}

bool ImportFromFileDialog::FirstColumnObjNames()
{
  if(ui.firstcolobjname->isChecked()){
    return true;
  }
  else{
    return false;
  }
}

bool ImportFromFileDialog::FirstRowVarNames()
{
  if(ui.firstrowvarname->isChecked()){
    return true;
  }
  else{
    return false;
  }
}

int ImportFromFileDialog::getDataType()
{
  if(ui.matrixButton->isChecked()){
    return MATRIXDATA;
  }
  else{
    return ARRAYDATA;
  }
}

QString ImportFromFileDialog::getFileName()
{
  return ui.file->text();
}

QString ImportFromFileDialog::getLabel()
{
  if(ui.filename->text().compare("project") != 0) // project name is a reserved name in order to recognize a speicific kind of data in the qtreeview
    return ui.filename->text();
  else
    return ui.filename->text().append("_");
}

QString ImportFromFileDialog::getSeparator()
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

QString ImportFromFileDialog::getSkipChar()
{
  return ui.ignorelinebychar->text();
}

int ImportFromFileDialog::getStartLine()
{
  return ui.startlineBox->value();
}

int ImportFromFileDialog::getStopLine()
{
  return ui.stoplineBox->value();
}

void ImportFromFileDialog::UpdateView()
{
  bool LoadFileInMem_ = false;
  bool Transpose_ = false;
  
  if(ui.splitlineby->currentIndex() != idsplitlinebyindex){
    idsplitlinebyindex = ui.splitlineby->currentIndex();
    LoadFileInMem_ = true;
  }
  
  if(ui.ignorelinebychar->text().compare("#") == false){
    LoadFileInMem_ = true;
  }
  
  if(ui.startlineBox->value() != idstartlinebox){
    LoadFileInMem_ = true;
    idstartlinebox = ui.startlineBox->value();
  }
  
  if(ui.stoplineBox->value() != idstoplinebox){
    LoadFileInMem_ = true;
    idstoplinebox = ui.startlineBox->value();
  }
  
  if(ui.transpose->isChecked() != transposestatus){
    Transpose_ = true;
    transposestatus = ui.transpose->isChecked();
  }
  
  if(LoadFileInMem_ == true)
    LoadFileInMem();
  
  if(Transpose_ == true)
    Transpose();
  
  Preview();
}

void ImportFromFileDialog::OK()
{
  if(!ui.file->text().isEmpty()){
    QFileInfo info(ui.file->text());
    if(getDataType() == MATRIXDATA && info.exists() == true){
      BuildMatrix();
      accept();
    }
    else if(getDataType() == ARRAYDATA && info.exists() == true){
      BuildArray();
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

void ImportFromFileDialog::Open()
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

  LoadFileInMem();
  Preview();
}

void ImportFromFileDialog::LoadFileInMem()
{
  columntoskip.clear();
  LoadFile(ui.file->text());
  
  /*
  if(ui.transpose->isChecked()){
    Transpose();
  }
  */
  if(ui.skipcolmissingvalue->isChecked()){
    GetColumtoDiscard();
  }
}

void ImportFromFileDialog::Transpose()
{
  if(file.size() > 0 && file.first().size() > 1){
    QList<QStringList> file_;
    // deep copy
    for(int j = 0; j < file.first().size(); j++){
      file_.append(QStringList());
      for(int i = 0; i < file.size(); i++){
        if(j < file[i].size()){
          file_.last().append(file[i][j]);
        }
        else{
          continue;
        }
      }
    }
    
    for(int i = 0; i < file.size(); i++){
      file[i].clear();
    }
    
    file.clear();
    file = file_;
  }
}

void ImportFromFileDialog::ObjSelectAll()
{
  ui.listView_2->selectAll();
  CountNObjects();
}

void ImportFromFileDialog::ObjInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_2->selectionModel();
  QAbstractItemModel *model = ui.listView_2->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  
  CountNObjects();
}

void ImportFromFileDialog::ObjUnselectAll()
{
  ui.listView_2->selectionModel()->clear();
  CountNObjects();
}

void ImportFromFileDialog::ObjSelectBy()
{
  CountNObjects();
}

void ImportFromFileDialog::VarSelectAll()
{
  ui.listView_3->selectAll();
  CountNVariables();
}

void ImportFromFileDialog::VarInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_3->selectionModel();
  QAbstractItemModel *model = ui.listView_3->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  CountNVariables();
}

void ImportFromFileDialog::VarUnselectAll()
{
  ui.listView_3->selectionModel()->clear();
  CountNVariables();
}

void ImportFromFileDialog::VarSelectBy()
{
  CountNVariables();
}

void ImportFromFileDialog::CountNObjects()
{
  uint nobj = 0;
  for(int i = 0; i < ui.listView_2->model()->rowCount(); i++){
    if(ui.listView_2->selectionModel()->isSelected(ui.listView_2->model()->index(i, 0)) == true){
      nobj++;
    }
    else{
      continue;
    }
  }
  ui.nobjectsLabel->setText(QString("Number of Objects: %1").arg(QString::number(nobj)));
}

void ImportFromFileDialog::CountNVariables()
{
  uint nvar = 0;
  for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
    if(ui.listView_3->selectionModel()->isSelected(ui.listView_3->model()->index(i, 0)) == true){
      nvar++;
    }
    else{
      continue;
    }
  }
  ui.nvariableLabel->setText(QString("Number of Variables: %1").arg(QString::number(nvar)));
}

void ImportFromFileDialog::EnableDisable()
{
  if(ui.listView_2->selectionModel()->selectedRows().size() > 0 && ui.listView_3->selectionModel()->selectedRows().size() > 0){
    ui.okButton->setEnabled(true);
  }
  else{
    ui.okButton->setEnabled(false);
  }
  
  CountNObjects();
  CountNVariables();
}

void ImportFromFileDialog::Preview()
{
  model->clear();
  if(file.size() > 0 && file.first().size() > 0){
    LoadObjectNames();
    GetColumtoDiscard();
    LoadVariableNames();
    
    int maxrowtoget = 10;
    int maxcoltoget = 10;
    
    // Import the temporary file in mem...
    QList<QStringList> previewfile;
    
    if(ui.matrixButton->isChecked()){
      if(maxrowtoget  < file.size()){
        for(int i = 0; i < maxrowtoget; i++){
          QStringList lst;
          if(maxcoltoget < file[i].size()){
            for(int j = 0; j < maxcoltoget; j++){
              lst.append(file[i][j]);
            }
          }
          else{
            for(int j = 0; j < file[i].size(); j++){
              lst.append(file[i][j]);
            }
          }
          previewfile.append(lst);
        }
      }
      else{
        for(int i = 0; i < file.size(); i++){
          QStringList lst;
          if(maxcoltoget < file[i].size()){
            for(int j = 0; j < maxcoltoget; j++){
              lst.append(file[i][j]);
            }
          }
          else{
            for(int j = 0; j < file[i].size(); j++){
              lst.append(file[i][j]);
            }
          }
          previewfile.append(lst);
        }
      }
      
      if(varnames.size() < maxcoltoget){
        maxcoltoget = varnames.size();
      }
      
      if(objnames.size() < maxrowtoget){
        maxrowtoget = objnames.size();
      }
      
      if(previewfile.size() > 0 && previewfile.last().size() > 0){
        if(FirstRowVarNames() == true){
          for(int i = 0; i < previewfile.first().size(); i++){
            if(i == 0){
              model->setHorizontalHeaderItem(i, new QStandardItem(QString("Object Names")));
            }
            else{
              model->setHorizontalHeaderItem(i, new QStandardItem(QString("[%1] ~ %2").arg(QString::number(i)).arg(previewfile.first()[i])));
            }
          }
          
          model->setHorizontalHeaderItem(maxcoltoget, new QStandardItem(QString("...")));
          previewfile.removeFirst();
        }
        else{
          for(int i = 0; i < previewfile.first().size(); i++){
            if(i == 0){
              model->setHorizontalHeaderItem(i, new QStandardItem(QString("Object Names")));
            }
            else{
              model->setHorizontalHeaderItem(i, new QStandardItem(QString("[%1] ~ Var %2").arg(QString::number(i)).arg(QString::number(i+1))));
            }
          }
          
          model->setHorizontalHeaderItem(maxcoltoget, new QStandardItem(QString("...")));
        }
        
        // load previewfile for preview.......
        for(int i = 0; i < previewfile.size(); i++){
          QList<QStandardItem*> row;
          
          if(FirstColumnObjNames() == true){
            row.append(new QStandardItem(previewfile[i].first()));
            
            for(int j = 1; j < previewfile[i].size(); j++){
              row.append(new QStandardItem(previewfile[i][j]));
            }
            
            row.append(new QStandardItem("..."));
            model->appendRow(row);
          }
          else{
            row.append(new QStandardItem(QString("Object %1").arg(QString::number(i+1)))); 
            for(int j = 0; j < maxcoltoget-1; j++){
              if(j < previewfile[i].size()){
                row.append(new QStandardItem(previewfile[i][j]));
              }
              else{
                continue;
              }
            }
            row.append(new QStandardItem("..."));
            model->appendRow(row);
          }
        }
        
        // last empty row
        QList<QStandardItem*> lastrow;
        if(FirstColumnObjNames() == true){
          lastrow.append(new QStandardItem(previewfile.last().first()));
          
          for(int j = 1; j < previewfile.last().size()+1; j++){
            lastrow.append(new QStandardItem("..."));
          }
          model->appendRow(lastrow);
        }
        else{
          lastrow.append(new QStandardItem(QString("Object %1").arg(QString::number(maxrowtoget)))); 
          for(int j = 0; j < maxcoltoget; j++){
            lastrow.append(new QStandardItem("..."));
          }
          model->appendRow(lastrow);
        }
      }
      else{
        model->clear();
        tab1->Clear();
        tab2->Clear();
        QString errormsg = "Please Check Your File";
        tab1->AppendRow(errormsg);
        tab2->AppendRow(errormsg);
        /*
        QList<QStandardItem*> objrow, varrow;
        objrow.append(new QStandardItem("Please Check Your File"));
        varrow.append(new QStandardItem("Please Check Your File"));
        tab1->appendRow(objrow);
        tab2->appendRow(varrow);
        */
      }
    }
    else{
      if(maxrowtoget  < file.size()){
        for(int i = 0; i < maxrowtoget; i++){
          QStringList lst;
          if(maxcoltoget < file[i].size()){
            for(int j = 0; j < maxcoltoget; j++){
              lst.append(file[i][j]);
            }
          }
          else{
            for(int j = 0; j < file[i].size(); j++){
              lst.append(file[i][j]);
            }
          }
          previewfile.append(lst);
        }
      }
      else{
        for(int i = 0; i < file.size(); i++){
          QStringList lst;
          if(maxcoltoget < file[i].size()){
            for(int j = 0; j < maxcoltoget; j++){
              lst.append(file[i][j]);
            }
          }
          else{
            for(int j = 0; j < file[i].size(); j++){
              lst.append(file[i][j]);
            }
          }
          previewfile.append(lst);
        }
      }
      
      if(varnames.size() < maxcoltoget){
        maxcoltoget = varnames.size();
      }
      
      if(objnames.size() < maxrowtoget){
        maxrowtoget = objnames.size();
      }
      
      if(previewfile.size() > 0 && previewfile.last().size() > 0){
        int ncols;
        if(FirstColumnObjNames() == true){
          if(file.size() > 1){
            ncols = (file[1].size()-1)/ui.levelBox->value();
          }
          else{
            ncols = (file[0].size()-1)/ui.levelBox->value();
          }
        }
        else{
          if(file.size() > 1){
            ncols = (file[1].size())/ui.levelBox->value();
          }
          else{
            ncols = (file[0].size())/ui.levelBox->value();
          }
        }
        
        if(FirstRowVarNames() == true){
          for(int i = 0; i < previewfile.first().size(); i++){
            if(i == 0){
              model->setHorizontalHeaderItem(i, new QStandardItem(QString("Object Names")));
            }
            else{
              if(i < ncols){
                model->setHorizontalHeaderItem(i, new QStandardItem(QString("[%1] ~ %2").arg(QString::number(i)).arg(previewfile.first()[i])));
              }
              else{
                // here init the second layer so stop.........
                break;
              }
            }
          }
          
          model->setHorizontalHeaderItem(maxcoltoget, new QStandardItem(QString("...")));
          previewfile.removeFirst();
        }
        else{
          for(int i = 0; i < previewfile.first().size(); i++){
            if(i == 0){
              model->setHorizontalHeaderItem(i, new QStandardItem(QString("Object Names")));
            }
            else{
              if(i < ncols){
                model->setHorizontalHeaderItem(i, new QStandardItem(QString("[%1] ~ Var %2").arg(QString::number(i)).arg(QString::number(i+1))));
              }
              else{
                break;
              }
            }
          }
          
          model->setHorizontalHeaderItem(maxcoltoget, new QStandardItem(QString("...")));
        }
        
        // load previewfile for preview.......
        for(int i = 0; i < previewfile.size(); i++){
          QList<QStandardItem*> row;
          
          if(FirstColumnObjNames() == true){
            row.append(new QStandardItem(previewfile[i].first()));
            
            for(int j = 1; j < previewfile[i].size(); j++){
              if(j < ncols){
                row.append(new QStandardItem(previewfile[i][j]));
              }
              else{
                break;
              }
            }
            
            row.append(new QStandardItem("..."));
            model->appendRow(row);
          }
          else{
            row.append(new QStandardItem(QString("Object %1").arg(QString::number(i+1)))); 
            for(int j = 0; j < maxcoltoget-1; j++){
              if(j < ncols){
                row.append(new QStandardItem(previewfile[i][j]));
              }
              else{
                break;
              }
            }
            
            row.append(new QStandardItem("..."));
            model->appendRow(row);
          }
        }
        
        // last empty row
        QList<QStandardItem*> lastrow;
        if(FirstColumnObjNames() == true){
          lastrow.append(new QStandardItem(previewfile.last().first()));
          
          for(int j = 1; j < previewfile.last().size()+1; j++){
            lastrow.append(new QStandardItem("..."));
          }
          model->appendRow(lastrow);
        }
        else{
          lastrow.append(new QStandardItem(QString("Object %1").arg(QString::number(maxrowtoget)))); 
          for(int j = 0; j < maxcoltoget; j++){
            lastrow.append(new QStandardItem("..."));
          }
          model->appendRow(lastrow);
        }
      }
      else{
        model->clear();
        tab1->Clear();
        tab2->Clear();
        QString errormsg = "Please Check Your File";
        tab1->AppendRow(errormsg);
        tab2->AppendRow(errormsg);
        /*
        QList<QStandardItem*> objrow, varrow;
        objrow.append(new QStandardItem("Please Check Your File"));
        varrow.append(new QStandardItem("Please Check Your File"));
        tab1->appendRow(objrow);
        tab2->appendRow(varrow);*/
      }
    }
  }
  else{
    tab1->Clear();
    tab2->Clear();
  }
}

ImportFromFileDialog::ImportFromFileDialog(): QDialog()
{
  ui.setupUi(this);
  
  model = new QStandardItemModel();
  
  ui.tableView->setModel(model);
  
  m = new MATRIX;
  a = new ARRAY;
  
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui.openButton, SIGNAL(clicked(bool)), this, SLOT(Open()));
  
  idsplitlinebyindex = ui.splitlineby->currentIndex();
  idstartlinebox = ui.startlineBox->value();
  idstoplinebox = ui.startlineBox->value();
  transposestatus = ui.transpose->isChecked();
  
  /*
  connect(ui.file, SIGNAL(textChanged(QString)), SLOT(LoadFileInMem()));
  connect(ui.splitlineby, SIGNAL(currentIndexChanged(int)), SLOT(LoadFileInMem()));
  connect(ui.ignorelinebychar, SIGNAL(textChanged(QString)), SLOT(LoadFileInMem()));
  connect(ui.startlineBox, SIGNAL(valueChanged(int)), SLOT(LoadFileInMem()));
  connect(ui.stoplineBox, SIGNAL(valueChanged(int)), SLOT(LoadFileInMem()));
  connect(ui.firstcolobjname, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.firstrowvarname, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.transpose, SIGNAL(clicked(bool)), SLOT(Transpose()));
  connect(ui.skipcolmissingvalue, SIGNAL(clicked(bool)), SLOT(Preview()));
  connect(ui.colmissvalueBox, SIGNAL(valueChanged(int)), SLOT(Preview()));
  connect(ui.levelBox, SIGNAL(valueChanged(int)), SLOT(Preview()));
  */
  
  connect(ui.updateView, SIGNAL(clicked(bool)), SLOT(UpdateView()));
  
  connect(ui.objselectAllButton, SIGNAL(clicked(bool)), SLOT(ObjSelectAll()));
  connect(ui.objinvertselectionButton, SIGNAL(clicked(bool)), SLOT(ObjInvertSelection()));
  connect(ui.objunselectAllButton, SIGNAL(clicked(bool)), SLOT(ObjUnselectAll()));
//   connect(ui.objselectbymatchButton, SIGNAL(clicked(bool)), SLOT(ObjSelectBy()));
  
  connect(ui.varselectAllButton, SIGNAL(clicked(bool)), SLOT(VarSelectAll()));
  connect(ui.varinvertselectionButton, SIGNAL(clicked(bool)), SLOT(VarInvertSelection()));
  connect(ui.varunselectAllButton, SIGNAL(clicked(bool)), SLOT(VarUnselectAll()));
//   connect(ui.varselectbymatchButton, SIGNAL(clicked(bool)), SLOT(VarSelectBy()));
  
  ui.varselectbymatchButton->hide();
  ui.objselectbymatchButton->hide();
  
  tab1 = new StringListModel();
  tab2 = new StringListModel();
  
  ui.listView_2->setModel(tab1);
  ui.listView_3->setModel(tab2);
  
  /*ui.listView_2->horizontalHeader()->setDefaultSectionSize(200);
  ui.listView_3->horizontalHeader()->setDefaultSectionSize(200);*/
  
  connect(ui.listView_2->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisable()));
  connect(ui.listView_3->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisable()));
  
  
  CountNObjects();
  CountNVariables();
  EnableDisable();
  ui.progressBar->hide();
}

ImportFromFileDialog::~ImportFromFileDialog()
{
  delete tab1;
  delete tab2;
  delete m;
  delete a;
}
