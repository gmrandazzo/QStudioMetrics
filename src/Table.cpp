#include "Table.h"
#include <QList>
#include <QString>
#include <QStringList>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QMessageBox>
#include <QMenu>
#include <QList>
#include <QApplication>
#include <QTextStream>
#include <QPainter>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QFileDialog>

#ifdef DEBUG
#include <QDebug>
#endif



#include "qsmdata.h"
#include "run.h"

#include "Dialogs/LabelDialog.h"
#include "Dialogs/SearchOnTableDialog.h"
#include "Dialogs/HighlightTableVarDialog.h"
#include "Dialogs/ObjectSelectorDialog.h"
#include "Dialogs/ExportTableDialog.h"


#include <fstream>
#include <iostream>

QColor Model::makeColor(double val, const double& min, const double& max, const QColor& color1, const QColor& color2) const
{
  /*
  colorA = [0, 0, 255] # blue
  colorB = [255, 0, 0] # red
  function get_gradient_color(val):
  # 'val' must be between 0 and 1
  for i in [1,2,3]:
  color[i] = colorA[i] + val * (colorB[i] - colorA[i])
  return color1
  */
  double normval = (val - min) / (max -min);
  QColor ncolor;
  ncolor.setRed(color1.red() + (normval * (color2.red() - color1.red())));
  ncolor.setGreen(color1.green() + (normval * (color2.green() - color1.green())));
  ncolor.setBlue(color1.blue() + (normval * (color2.blue() - color1.blue())));
  return ncolor;
}

int Model::columnCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  if(m == 0){
    if(tab.size() > 0){
      return tab.last().size()+1;
    }
    else{
      return 0;
    }
  }
  else{
    return (int)m->col+1;
  }
}

int Model::rowCount(const QModelIndex& parent) const
{
  Q_UNUSED(parent);
  if(m == 0){
    if(tab.size() > 0){
      return tab.size();
    }
    else{
      return 0;
    }
  }
  else
    return (int)m->row;
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
  if(role != Qt::DisplayRole)
    return QVariant();

  if(orientation == Qt::Horizontal){
    if(section > header.size() || section < 0){
//       return QVariant();
      return QString("[%1] ~ NO VAR NAME FOUND").arg(QString::number(section+1));
    }
    else{
      if(section == 0){
        return header.at(section);
      }
      else{
        if(section < header.size()){
          return QString("[%1] ~ %2").arg(QString::number(section)).arg(header.at(section));
        }
        else{
          return QString("[%1] ~ Error!! No column name!!").arg(QString::number(section));
        }
      }
    }
  }

  if(orientation == Qt::Vertical){
    if(section > id.size()){
      return QVariant();
    }
    else{
      return id.at(section);
    }
  }
  return QVariant();
}

QVariant Model::data(const QModelIndex& index, int role) const
{
  if(!index.isValid()){
    return QVariant();
  }
  else{
    if(role == Qt::DisplayRole){
      if(m != 0){
        if(index.row() > (int)m->row || index.row() < 0 || index.column() > (int)m->col+1 || index.column() < 0) // +1 for the label column
          return QVariant();
        else{
          if(index.column() == 0){
            if(index.row() < labels.size()){
              return labels.at(index.row());
            }
            else{
              QString val = "####";
              return val;
            }
          }
          else{
            QString val = QString::number(getMatrixValue(m, index.row(), index.column()-1)); // -1 for the label column
            return val;
          }
        }
      }
      else if(tab.size() > 0){
        if(index.row() > tab.size() || index.row() < 0 || index.column() > tab.first().size()+1 || index.column() < 0){ // +1 for the label column
          return QVariant();
        }
        else{
          if(index.column() == 0){
            if(index.row() < labels.size()){
              return labels.at(index.row());
            }
            else{
              QString val = "####";
              return val;
            }
          }
          else{
            QString val = tab[index.row()][index.column()-1]; // -1 for the label column
            return val;
          }
        }
      }
      else{
        return QVariant();
      }
    }
    else if(role == Qt::BackgroundColorRole){
      if(m != 0){
        if(std::abs(minval) == std::abs(maxval)){
          return QVariant();
        }
        else{
          if((uint)index.column()-1 == variable){
            QColor color = makeColor(getMatrixValue(m, index.row(),variable), minval, maxval, mincolor, maxcolor);
            return QVariant(color);
          }
          else{
            return QVariant();
          }
        }
      }
      else{
        return QVariant();
      }
    }
    else{
      return QVariant();
    }
  }
}
/*
QVariant Model::data(const QModelIndex& index, int role) const
{
  if(!index.isValid()){
    return QVariant();
  }
  else{
    if(m != 0){
      if(role == Qt::DisplayRole){
        if(index.row() > (int)m->row || index.row() < 0 || index.column() > (int)m->col+1 || index.column() < 0) // +1 for the label column
          return QVariant();
        else{
          if(index.column() == 0){
            if(index.row() < labels.size()){
              return labels.at(index.row());
            }
            else{
              QString val = "####";
              return val;
            }
          }
          else{
            QString val = QString::number(getMatrixValue(m, index.row(), index.column()-1)); // -1 for the label column
            return val;
          }
        }
      }
      else if(role == Qt::BackgroundColorRole){
        if(abs(minval) == abs(maxval)){
          return QVariant();
        }
        else{
          if((uint)index.column()-1 == variable){
            QColor color = makeColor(getMatrixValue(m, index.row(),variable), minval, maxval, mincolor, maxcolor);
            return QVariant(color);
          }
          else{
            return QVariant();
          }
        }
      }
      else{
        return QVariant();
      }
    }
    else if(tab.size() > 0){
      qDebug() << "Role " << role << Qt::DisplayRole;
      if(role == Qt::DisplayRole){
        if(index.row() > (int)tab.size() || index.row() < 0 || index.column() > (int)tab.first().size()+1 || index.column() < 0){ // +1 for the label column
          qDebug() << "Esco Qua 1";
          return QVariant();
        }
        else{
          if(index.column() == 0){
            if(index.row() < labels.size()){
              return labels.at(index.row());
            }
            else{
              QString val = "####";
              return val;
            }
          }
          else{
            QString val = tab[index.row()][index.column()-1]; // -1 for the label column
            return val;
          }
        }
      }
      else{
        qDebug() << "Esco Qua 3";
        return QVariant();
      }
    }
    else{
      qDebug() << "Esco Qua 4";
      return QVariant();
    }
  }
}
*/

/*
bool Model::setData(matrix* m_, int role)
{
  if(role == Qt::EditRole){
    MatrixCopy(m_, &m);
    if(header.size() == 0){
    // Set the generic column name
      for(uint i = 0; i < m->col; i++)
        header.append(QString::number(i));
    }
    return true;
  }
  else
    return false;
}
*/

void Model::setObjNames(QStringList labels_)
{
  labels.clear();
  labels.append(labels_);

  /*
  if(labels_.size() < (int)m->row){
    labels.clear();
    for(int i = 0; i < labels_.size(); i++){
      labels.append(labels_[i]);
    }
    for(int i = 0; i < (int)(m->row - labels_.size()); i++){
      labels.append("####");
    }
  }
  else if(labels_.size() == (int)m->row){
    labels.clear();
    labels = labels_;
  }
  else{
    labels.clear();
    for(uint i = 0; i < m->row; i++){
      labels.append(labels_[i]);
    }
  }
  */
}

void Model::setHorizontalHeaderLabels(QStringList& headerlabels)
{
  header = headerlabels;
}

void Model::setItem(uint row, uint col, double value)
{
  if(row < m->row && col < m->col)
    setMatrixValue(m, row, col, value);
}

void Model::init_Matrix()
{
  isallocatedmatrix = true;
  initMatrix(&m);
}

void Model::newMatrix(uint row, uint col)
{
  isallocatedmatrix = true;
  NewMatrix(&m, row, col);

  labels.clear();
  for(uint i = 0; i < m->row; i++){
    labels << "####";
    id.append(i+1);
  }

  header.clear();
  header.append("Object Names");
  for(uint j = 0; j < m->col; j++){
    header.append(QString::number(j+1));
  }

  UpdateModel();
}

void Model::newMatrix(uint row, uint col, QStringList objnames, QStringList headernames)
{
  isallocatedmatrix = true;
  NewMatrix(&m, row, col);

  labels.clear();
  if(objnames.size() < (int)m->row){
    int c = 0;
    for(int i = 0; i < (int)m->row; i++){
      if(i < objnames.size()){
        labels << objnames[i];
      }
      else{
        labels << QString("Obj %1").arg(c+1);
        c++;
      }
      id.append(i+1);
    }
  }
  else{
    for(int i = 0; i < (int)m->row; i++){
      labels << objnames[i];
      id.append(i+1);
    }
  }

  header.clear();

  if(headernames.size() < (int)m->col){
    int c = 0;
    for(int j = 0; j < (int)m->col; j++){
      if(j < headernames.size()){
        header.append(headernames[j]);
      }
      else{
        header.append(QString("Col %1").arg(c+1));
        c++;
      }
    }
  }
  else{
    for(int j = 0; j < (int)m->col; j++){
      header.append(headernames[j]);
    }
  }
  UpdateModel();
}


void Model::delMatrix()
{
  DelMatrix(&m);
  isallocatedmatrix = false;
}

void Model::UpdateModel()
{
 emit layoutChanged();
}

Model::Model(QObject* parent): QAbstractTableModel(parent)
{
  isallocatedmatrix = false;
  m = 0;
  minval = maxval = variable = 0;
  mincolor = QColor(Qt::green);
  maxcolor = QColor(Qt::red);
}

Model::Model(matrix* m_, QObject* parent): QAbstractTableModel(parent)
{
  isallocatedmatrix = true;
  initMatrix(&m);
  MatrixCopy(m_, &m);
//   m = m_;
  minval = maxval = variable = 0;
  mincolor = QColor(Qt::green);
  maxcolor = QColor(Qt::red);
  for(uint i = 0; i < m->row; i++){
    labels << "####";
    id.append(i+1);
  }
  header.append("Object Names");
  for(uint j = 0; j < m->col; j++)
    header.append(QString::number(j+1));
}

Model::Model(QList< QStringList > tab_, QObject* parent): QAbstractTableModel(parent)
{
  isallocatedmatrix = false;
  m = 0;
  tab = tab_;
  minval = maxval = variable = 0;
  mincolor = QColor(Qt::green);
  maxcolor = QColor(Qt::red);

  for(int i = 0; i < tab.size(); i++){
    labels << "####";
    id.append(i+1);
  }
  header.append("Object Names");
  for(int j = 0; j < tab.last().size(); j++)
    header.append(QString::number(j+1));
}


Model::~Model()
{
  if(isallocatedmatrix == true){
    delMatrix();
    #ifdef DEBUG
    qDebug()<< "~Model() Deleting Matrix";
    #endif
  }
}

// Function used by qStableSort in copy
static bool indexesByRow(const QModelIndex& one, const QModelIndex& two )
{
  return one.row()<two.row();
}

void Table::copy()
{
  QModelIndexList indexes = ui.tableView->selectionModel()->selectedIndexes();
  if(indexes.size() < 1)
    return;
  else if( indexes.size() == 1){ // copy one cell
//     selected_cell;
    QString selected_cell(model_->data(indexes.first()).toString());
    QApplication::clipboard()->setText( selected_cell );
//     qDebug()<<  selected_cell;
  }
  else{ // copy more cell and row
    //Sort the indexes list maintain the relative order of column by row ascening
    qStableSort(indexes.begin(), indexes.end(), indexesByRow );
  //   qSort(indexes.begin(), indexes.end());

    // get a pair of indexes to find the row changes
    QModelIndex previous = indexes.first();
    indexes.removeFirst();
    QString selected_text;
    QModelIndex current;
    Q_FOREACH(current, indexes){
      QVariant _data = model_->data(previous);
      QString text = _data.toString();
      // Append the text in one in one cell
      selected_text.append(text);
      // If you are at the start of the row the row number of the previous index
      // isn't the same.  Text is followed by a row separator, which is a newline.
      if ( current.row() != previous.row() ){
        selected_text.append(QLatin1Char('\n'));
      }
      // Otherwise it's the same row, so append a column separator, which is a tab.
      else{
        selected_text.append(QLatin1Char('\t'));
      }
      previous = current;
    }

    // add last element
    selected_text.append(model_->data(current).toString());
    selected_text.append(QLatin1Char('\n'));
    #ifdef DEBUG
    qDebug()<< "Copy Action";
    qDebug()<< selected_text<<endl;
    #endif
    QApplication::clipboard()->setText( selected_text );
  }
}

void Table::addObjectLabel()
{
  QModelIndexList rowindexes = ui.tableView->selectionModel()->selectedRows();

  if(rowindexes.size() < 1)
    return;
  else{
    QStringList selectedobj;
    QModelIndex current;
    Q_FOREACH(current, rowindexes){
      selectedobj.append(model_->getObjNames()[current.row()]);
    }

    LabelDialog ldialog(objlabels, selectedobj, LabelDialog::OBJLABELS);
    ldialog.exec();
  }
}

void Table::addVariableLabel()
{
  QModelIndexList colindexes = ui.tableView->selectionModel()->selectedColumns();

  if(colindexes.size() < 1){
    return;
  }
  else{
    QStringList selectevars;
    QModelIndex current;
    Q_FOREACH(current, colindexes){
      selectevars.append(model_->getHorizontalHeaderLabels()[current.column()]);
    }

    #ifdef DEBUG
    qDebug() << "Selected Variables... " << selectevars;
    #endif

    LabelDialog ldialog(varlabels, selectevars, LabelDialog::VARLABELS);
    ldialog.exec();
  }
}


void Table::selectBy()
{
  QStringList currentvariables = model()->getHorizontalHeaderLabels();
  currentvariables.removeAll("Object Names");
  currentvariables.removeAll("Object Labels");
  currentvariables.removeAll("Principal Component");
  currentvariables.removeAll("Variables");

  ObjectSelectorDialog objseldialog(currentvariables, objlabels, varlabels);
  if(objseldialog.exec() == QDialog::Accepted){
    int selectiontype = objseldialog.getSelectionType();
    int metric = objseldialog.getMetric();
    int nobjects = objseldialog.getNumberOfObjects();
    QString label = objseldialog.getLabel();
    QStringList varlist = objseldialog.getVariableList();

    if(selectiontype == MOSTDESCRIPTIVECOMPOUND || selectiontype == MAXIMUMDISSIMILARITYMAXMIN){
      if(metric != -1 && nobjects != -1 && varlist.size() > 0){
        matrix *m;
        uivector *selected;
        RUN obj;

        NewMatrix(&m, model()->Matrix()->row, varlist.size());

        int col = 0;
        for(int j = 0; j < varlist.size(); j++){
          int colindex = currentvariables.indexOf(varlist[j]);
          if(colindex !=  -1){
            for(uint i = 0; i < model()->Matrix()->row; i++){
              setMatrixValue(m, i, col, getMatrixValue(model()->Matrix(), i, colindex));
            }
            col++;
          }
          else{
            #ifdef DEBUG
            qDebug() << "Error in selection columnindex this variable " << varlist[j] << "seems to be not present.";
            #endif
	    continue;
	  }
        }
        obj.setMatrix(m);

        initUIVector(&selected);

        obj.setUIVector(selected);
        obj.setMetric(metric);
        obj.setNumberOfObject(nobjects);

        StartSelectionRun();
        QFuture<void> future;
        if(selectiontype == MOSTDESCRIPTIVECOMPOUND){
          future = obj.RunMDCSelection();
        }
        else{
          future = obj.RunMaxDisSelection();

        }

        while(!future.isFinished()){
          if(stoprun == true){
            future.cancel();
          }
          else{
            QApplication::processEvents();
          }
        }

        ui.tableView->selectionModel()->clearSelection();

        for(uint i = 0; i < selected->size; i++){
          if(getUIVectorValue(selected, i) < model()->Matrix()->row){
            ui.tableView->selectionModel()->select(ui.tableView->model()->index(getUIVectorValue(selected, i), 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
          }
        }

        DelMatrix(&m);
        DelUIVector(&selected);
        StopSelectionRun();
      }
      else{
        return;
      }
    }
    else if(selectiontype == RANDOMSELECTION){
      if(nobjects != -1){
        uivector *selected;
        RUN obj;

        initUIVector(&selected);

        obj.setUIVector(selected);
        obj.setNumberOfObject(nobjects);
        obj.setNumberMaxOfObject(model()->Matrix()->row);

        StartSelectionRun();
        QFuture<void> future = obj.RunRandomSelection();;

        while(!future.isFinished())
          QApplication::processEvents();

        ui.tableView->selectionModel()->clearSelection();

        for(uint i = 0; i < selected->size; i++){
          if(getUIVectorValue(selected, i) < model()->Matrix()->row){
            ui.tableView->selectionModel()->select(ui.tableView->model()->index(getUIVectorValue(selected, i), 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
          }
        }

        DelUIVector(&selected);
        StopSelectionRun();
      }
      else{
        return;
      }
    }
    else{
      if(label.size() > 0){
        int lindex = -1;
        for(int i = 0; i < (*objlabels).size(); i++){
          if((*objlabels)[i].name.compare(label) == 0){
            lindex = i;
            break;
          }
          else{
            continue;
          }
        }

        if(lindex > -1){

          ui.tableView->selectionModel()->clearSelection();

          for(int i = 0; i < (*objlabels)[lindex].objects.size(); i++){
            int index = model()->getObjNames().indexOf((*objlabels)[lindex].objects[i]);
            if(index > -1){
              ui.tableView->selectionModel()->select(ui.tableView->model()->index(index, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
            }
            else{
              continue;
            }
          }

        }
        else{
          return;
        }
      }
      else{
        return;
      }
    }
  }
}

void Table::searchBy()
{
  SearchOnTableDialog sotd(model()->getHorizontalHeaderLabels());
  if(sotd.exec() == QDialog::Accepted){
    if(sotd.getSelectionType() == 0){ // select by objname
      QString label = sotd.getLabel();

      ui.tableView->selectionModel()->clearSelection();

      for(int i = 0; i < model()->getObjNames().size(); i++){
        if(model()->getObjNames()[i].contains(label, Qt::CaseInsensitive)== true){
          if(i < (int)model()->Matrix()->row){
            ui.tableView->selectionModel()->select(ui.tableView->model()->index(i, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
            ui.tableView->scrollTo(ui.tableView->model()->index(i, 0));
          }
          else{
            continue;
          }
        }
      }
    }
    else{ // select by descriptor
      int descriptor = sotd.getSelectionType() - 1;

      double min = sotd.getMin(), max = sotd.getMax();

      if(descriptor < (int)model()->Matrix()->col){
        ui.tableView->selectionModel()->clearSelection();
        for(uint i = 0; i < model()->Matrix()->row; i++){
          double val = getMatrixValue(model()->Matrix(), i, descriptor);
          if(val >= min && val <= max){
            ui.tableView->selectionModel()->select(ui.tableView->model()->index(i, 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
            ui.tableView->scrollTo(ui.tableView->model()->index(i, 0));
          }
          else{
            continue;
          }
        }
      }
    }
  }
}

void Table::highlitingCell()
{
  HighlightTableVarDialog hdialog(model()->getHorizontalHeaderLabels());
  if(hdialog.exec() == QDialog::Accepted){
    QColor colormin, colormax;
    colormin = hdialog.getMinColor();
    colormax = hdialog.getMaxColor();
    int var = hdialog.getVariableType();

    double min, max;
    MatrixColumnMinMax(model()->Matrix(), var, &min, &max);

    model()->setVariable(var);
    model()->setMinColumnVal(min);
    model()->setMaxColumnVal(max);
    model()->setMinColumnColor(colormin);
    model()->setMaxColumnColor(colormax);

    ui.tableView->scrollTo(ui.tableView->model()->index(ui.tableView->currentIndex().row(), var));
  }
}

void Table::ExportTable()
{
  ExportTableDialog exptabdialog(model()->getObjNames(), model()->getHorizontalHeaderLabels(), objlabels, varlabels);

  if(exptabdialog.exec() == QDialog::Accepted){

    pdialog.setRange(0, 0);
    pdialog.hideCancel();
    pdialog.show();
    int barvalue = 0;

    QString fname = exptabdialog.getFileName();
    QString sep = exptabdialog.getSeparator();
    if(fname.contains(".txt", Qt::CaseInsensitive) == false || fname.contains(".csv", Qt::CaseInsensitive) == false)
      fname.append(".csv");

    std::ofstream out;
    out.open(fname.toStdString().c_str());
    if (out.is_open()){
      QStringList selectedobj = exptabdialog.getSelectedObjects();
      QStringList selectedvar = exptabdialog.getSelectedVariables();

      std::string sep_ = sep.toStdString();
      QStringList objname = model()->getObjNames();
      QStringList varnames = model()->getHorizontalHeaderLabels();
      matrix *mxtowrite = model()->Matrix();

      out.setf(std::ios_base::right, std::ios_base::adjustfield);
      out.setf(std::ios::fixed, std::ios::floatfield);

      if(selectedobj.size() == model()->getObjNames().size()
        && selectedvar.size() == model()->getHorizontalHeaderLabels().size()){
        // write the entire matrix
        int nrow = objname.size();
        int ncol = varnames.size();


        pdialog.setRange(0, selectedvar.size() + selectedobj.size());
        // write the header
        for(int j = 0; j < ncol-1; j++){
          out << varnames[j].toStdString() << sep_;
        }
        out << varnames.last().toStdString() <<  std::endl;

        for(int i = 0; i < nrow; i++){
          out << objname[i].toStdString() << sep_;
          for(int j = 0; j < ncol-2; j++){ // -2 because the first col is the object name and the last column is writed with an excape char
            out << mxtowrite->data[i][j] << sep_;
            pdialog.setValue(barvalue);
            barvalue++;
            QApplication::processEvents();
          }
          out << mxtowrite->data[i][mxtowrite->col-1] <<  std::endl;
        }
      }
      else{
        pdialog.setRange(0, selectedvar.size() + selectedobj.size()*2);

        // select object and variables to write and write it!
        QList< uint > selectedvarid;
        for(int i = 0; i < varnames.size(); i++){
          int j = selectedvar.indexOf(varnames[i]);
          if(j > -1){
            selectedvarid.append(i);
            selectedvar.removeAt(j);
          }
          pdialog.setValue(barvalue);
          barvalue++;
          QApplication::processEvents();

          if(selectedvar.size() == 0)
            break;
          else
            continue;
        }

        QList<int> selobjid;
        for(int i = 0; i < objname.size(); i++){
          int j = selectedobj.indexOf(objname[i]);
          if(j > -1){
            selobjid.append(i);
            selectedobj.removeAt(j);
          }
          pdialog.setValue(barvalue);
          barvalue++;
          QApplication::processEvents();

          if(selectedobj.size() == 0)
            break;
          else
            continue;
        }



        // write the header
        for(int j = 0; j < selectedvarid.size()-1; j++){
          out << varnames[selectedvarid[j]].toStdString() << sep_;
        }
        out << varnames[selectedvarid.last()].toStdString() <<  std::endl;

        if(selectedvarid[0] == 0){ // write the object name
          for(int i = 0; i < selobjid.size(); i++){
            uint row =  selobjid[i];
            if(selectedvarid.size() > 1){
              out << objname[row].toStdString() << sep_;
            // 1 because is the object and the possible data to write...
              for(int j = 1; j < selectedvarid.size()-1; j++){
                out << mxtowrite->data[row][selectedvarid[j]-1] << sep_;
              }
              out << mxtowrite->data[row][selectedvarid.last()-1] <<  std::endl;
            }
            else{
              out << objname[row].toStdString() <<  std::endl;
            }
            pdialog.setValue(barvalue);
            barvalue++;
            QApplication::processEvents();
          }
        }
        else{
          for(int i = 0; i < selobjid.size(); i++){
            uint row =  selobjid[i];
            for(int j = 0; j < selectedvarid.size()-1; j++){
              out << mxtowrite->data[row][selectedvarid[j]-1] << sep_;
            }
            out << mxtowrite->data[row][selectedvarid.last()-1] <<  std::endl;

          pdialog.setValue(barvalue);
          barvalue++;
          QApplication::processEvents();
          }
        }
      }
      out.close();
    }
    pdialog.hide();
  }
}

void Table::resetHighliting()
{
  model()->setMinColumnVal(0.f);
  model()->setMaxColumnVal(0.f);
  model()->setMinColumnVal(0);
}

#include <QPrinter>
void Table::SaveAsImage()
{
  QString fname = QFileDialog::getSaveFileName(this, tr("Save table as image..."), QString(), tr("PNG files(*.png)"));
  if(!fname.isEmpty()){

    QPixmap pixmap(ui.tableView->sizeHint());
    ui.tableView->render(&pixmap);
    pixmap.save(fname, "PNG", 100);

    /*QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fname);
    printer.setResolution(300); //override to 300dpi
    printer.setFontEmbeddingEnabled(true);
    printer.setColorMode(QPrinter::Color);
    printer.setOrientation(QPrinter::Portrait);
    printer.setPaperSize(size(), QPrinter::DevicePixel);
    printer.setFullPage(true);
    QPainter painter(&printer);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setRenderHint(QPainter::TextAntialiasing, true);
    painter.drawPixmap(rect(), pixmap);*/


  }
}

void Table::SortByColumn(int col)
{

  #ifdef DEBUG
  qDebug() << "Sort by column " << col;
  #endif
  if(col == 0){
    matrix *m = model()->Matrix();

    QStringList objnames, new_objnames;
    objnames.append(model()->getObjNames());

    QList<QPair<QString, uint> > array_;
    for(uint i = 0; i < (uint)objnames.size(); i++){
      array_.append(qMakePair(objnames[i], i));
    }
    // Ordering ascending
    if(ui.tableView->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
      qSort(array_.begin(), array_.end(), QPairComparerAscending());
    else
      qSort(array_.begin(), array_.end(), QPairComparerDescending());


    matrix *tmp;
    initMatrix(&tmp);
    model()->getObjNames().clear();
    MatrixCopy(m, &tmp);
    for(int i = 0; i < array_.size(); i++){
      for(uint j = 0; j < tmp->col; j++){
        setMatrixValue(m, i, j, getMatrixValue(tmp, array_[i].second, j));
      }
      new_objnames.append(array_[i].first);
    }
    DelMatrix(&tmp);
    model()->setObjNames(new_objnames);
  }
  else{
    matrix *m = model()->Matrix();

    QStringList objnames, new_objnames;
    objnames.append(model()->getObjNames());

    QList<QPair<double, uint> > array_;
    for(uint i = 0; i < m->row; i++){
      array_.append(qMakePair(getMatrixValue(m, i, col-1), i));
    }

    if(ui.tableView->horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder)
      qSort(array_.begin(), array_.end(), QPairComparerAscending());
    else
      qSort(array_.begin(), array_.end(), QPairComparerDescending());

    matrix *tmp;
    initMatrix(&tmp);
    model()->getObjNames().clear();
    MatrixCopy(m, &tmp);
    for(int i = 0; i < array_.size(); i++){
      for(uint j = 0; j < tmp->col; j++){
        setMatrixValue(m, i, j, getMatrixValue(tmp, array_[i].second, j));
      }
      new_objnames.append(objnames[array_[i].second]);
    }
    DelMatrix(&tmp);

    model()->setObjNames(new_objnames);
  }
  model()->UpdateModel();
}


void Table::SetSelectionName()
{

  int colindexessize = ui.tableView->selectionModel()->selectedColumns().size();
  int rowindexessize = ui.tableView->selectionModel()->selectedRows().size();
  int indexessize = ui.tableView->selectionModel()->selectedIndexes().size() - 1;

  /*
  qDebug() << "indexes " << indexessize;
  qDebug() << "col" << colindexessize << " " << (int)model()->Matrix()->col;
  qDebug() << "row" << rowindexessize;
  qDebug() << "Selected: " << ((ceil(indexessize / (int)(model()->Matrix()->col+1)))+1);
  qDebug() << "#############################################";
 */

//   if(((ceil(indexessize / (int)(model()->Matrix()->col+1)))+1) == rowindexessize && rowindexessize > 0){
  if(((ceil(indexessize / (int)(model()->columnCount()+1)))+1) == rowindexessize && rowindexessize > 0){
    ts.pid = pid;
    ts.imgname.clear();
    ts.imgname.append(ui.tableView->model()->index(ui.tableView->currentIndex().row(), 0).data(Qt::DisplayRole).toString());
    emit TabImageSignalChanged(ts);
  }
  else{
    if(colindexessize > 0){
      ts.pid = pid;
      if(ui.tableView->currentIndex().column() < model()->getHorizontalHeaderLabels().size()){
        ts.imgname.clear();
        ts.imgname.append(model()->getHorizontalHeaderLabels()[ui.tableView->currentIndex().column()]);
        emit TabImageSignalChanged(ts);
      }
      else{
        ts.pid = -1;
        ts.imgname.clear();
        emit TabImageSignalChanged(ts);
      }
    }
    else{
      ts.pid = -1;
      ts.imgname.clear();
      emit TabImageSignalChanged(ts);
    }
  }
}

void Table::stopRun()
{
  StopSelectionRun();
}

void Table::contextMenuEvent(QContextMenuEvent *event)
{
  if(model_){
    QAction *copyAct = 0,
            *addObjLabel = 0,
            *addVarLabel = 0,
            *searchAct = 0,
            *selectByAct = 0,
            *highlitingCell = 0,
            *resethighliting = 0,
            *exportTable = 0;

    copyAct = new QAction(tr("&Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                              "clipboard"));
    connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    QMenu menu(this);
    menu.addAction(copyAct);
    menu.addSeparator();

    int colindexessize = ui.tableView->selectionModel()->selectedColumns().size();
    int rowindexessize = ui.tableView->selectionModel()->selectedRows().size();
    int indexessize = ui.tableView->selectionModel()->selectedIndexes().size() - 1;
    int selected = ((ceil(indexessize / (int)(model()->Matrix()->col+1)))+1); //(ceil(indexessize / (int)(model()->columnCount()+1)))+1)
    /*
    qDebug() << "indexes " << indexessize;
    qDebug() << "col" << colindexessize << " " << (int)model()->Matrix()->col;
    qDebug() << "row" << rowindexessize;
    qDebug() << "Selected: " << selected;
    qDebug() << "#############################################";
    */

    if(objlabels != 0){
  //     if(((ceil(indexessize / (int)(model()->Matrix()->col+1)))+1) == rowindexessize && rowindexessize > 0){
      if(selected == rowindexessize && rowindexessize > 0){
        addObjLabel = new QAction(("&Add Object Label"), this);
        addObjLabel->setStatusTip(tr("Add Label to selected objects"));
        connect(addObjLabel, SIGNAL(triggered()), this, SLOT(addObjectLabel()));
        menu.addAction(addObjLabel);
      }
    }

    if(varlabels != 0){
      if(colindexessize > 0){
        addVarLabel = new QAction(("&Add Variable Label"), this);
        addVarLabel->setStatusTip(tr("Add Label to selected Variables"));
        connect(addVarLabel, SIGNAL(triggered()), this, SLOT(addVariableLabel()));
        menu.addAction(addVarLabel);
      }
    }

    if(model()->Matrix() != 0){
      searchAct = new QAction(tr("&Search by column.."), this);
      searchAct->setStatusTip(tr("Search objects in the table by column..."));
      connect(searchAct, SIGNAL(triggered()), this, SLOT(searchBy()));
      menu.addAction(searchAct);

      if(varlabels != 0){
        selectByAct = new QAction(tr("&Select objects by"), this);
        selectByAct->setStatusTip(tr("Select objects in the table by..."));
        connect(selectByAct, SIGNAL(triggered()), this, SLOT(selectBy()));
        menu.addAction(selectByAct);
      }

      menu.addSeparator();

      highlitingCell = new QAction(tr("&Highliting Column"), this);
      highlitingCell->setStatusTip(tr("Highliting Cell from min to max"));
      connect(highlitingCell, SIGNAL(triggered()), this, SLOT(highlitingCell()));
      menu.addAction(highlitingCell);

      resethighliting = new QAction(tr("&Reset Highliting"), this);
      resethighliting->setStatusTip(tr("Reset the table Highliting"));
      connect(resethighliting, SIGNAL(triggered()), this, SLOT(resetHighliting()));
      menu.addAction(resethighliting);
      menu.addSeparator();
    }

    exportTable = new QAction(tr("&Export table..."), this);
    exportTable->setStatusTip(tr("Export table to file..."));
    connect(exportTable, SIGNAL(triggered()), this, SLOT(ExportTable()));
    menu.addAction(exportTable);

    menu.exec(event->globalPos());

    delete copyAct;
    delete searchAct;
    delete selectByAct;
    delete highlitingCell;
    delete resethighliting;

    if(addObjLabel != 0){
      delete addObjLabel;
    }

    if(addVarLabel != 0){
      delete addVarLabel;
    }
  }
  else{
    QAction *saveAsImage = 0;

    saveAsImage = new QAction(tr("&Save As Image..."), this);
    saveAsImage->setStatusTip(tr("Save the table as image."));
    connect(saveAsImage, SIGNAL(triggered()), this, SLOT(SaveAsImage()));

    QMenu menu(this);
    menu.addAction(saveAsImage);
    menu.exec(event->globalPos());

    delete saveAsImage;

  }
}

void Table::StartSelectionRun()
{
  stoprun = false;
  pdialog.setMin(0);
  pdialog.setMax(0);
  pdialog.setValue(0);
  pdialog.show();
}

void Table::StopSelectionRun()
{
  stoprun = true;
  pdialog.setMin(0);
  pdialog.setMax(100);
  pdialog.setValue(20);
  pdialog.hide();
}

Table::Table(QWidget *parent) : QWidget(parent)
{
  ui.setupUi(this);
  model_ = new Model(this);
  imgtabmodel_ = 0;
  pid = -1;
  ui.tableView->setModel(model_);
  ui.tableView->adjustSize();
  ui.tableView->installEventFilter(this);
//   ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  ui.tableView->horizontalHeader()->setDefaultSectionSize(200);
  objlabels = 0;
  varlabels = 0;
  connect(&pdialog, SIGNAL(runCancelled()), SLOT(stopRun()));
  connect(ui.tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(SetSelectionName()));
  connect(ui.tableView->horizontalHeader(),SIGNAL(sectionClicked(int)), SLOT(SortByColumn(int)));
}

Table::Table(matrix* m, QWidget* parent): QWidget(parent)
{
  ui.setupUi(this);
  model_ = new Model(m, this);
  imgtabmodel_ = 0;
  pid = -1;
  ui.tableView->setModel(model_);
  ui.tableView->adjustSize();
  ui.tableView->installEventFilter(this);
//   ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  ui.tableView->horizontalHeader()->setDefaultSectionSize(200);
  objlabels = 0;
  varlabels = 0;
  connect(&pdialog, SIGNAL(runCancelled()), SLOT(stopRun()));
  connect(ui.tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(SetSelectionName()));
  connect(ui.tableView->horizontalHeader(),SIGNAL(sectionClicked(int)), SLOT(SortByColumn(int)));
}

Table::Table(matrix* m, LABELS* objlabels_, LABELS* varlabels_, QWidget* parent): QWidget(parent)
{
  ui.setupUi(this);
  model_ = new Model(m, this);
  imgtabmodel_ = 0;
  pid = -1;
  objlabels = objlabels_;
  varlabels = varlabels_;
  stoprun = false;
  ui.tableView->setModel(model_);
  ui.tableView->adjustSize();
  ui.tableView->installEventFilter(this);
//   ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  ui.tableView->horizontalHeader()->setDefaultSectionSize(200);
  connect(&pdialog, SIGNAL(runCancelled()), SLOT(stopRun()));
  connect(ui.tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(SetSelectionName()));
  connect(ui.tableView->horizontalHeader(),SIGNAL(sectionClicked(int)), SLOT(SortByColumn(int)));
}

Table::Table(QList< QStringList > tab, LABELS* objlabels_, LABELS* varlabels_, QWidget* parent): QWidget(parent)
{
  ui.setupUi(this);
  model_ = new Model(tab, this);
  imgtabmodel_ = 0;
  pid = -1;
  objlabels = objlabels_;
  varlabels = varlabels_;
  stoprun = false;
  ui.tableView->setModel(model_);
  ui.tableView->adjustSize();
  ui.tableView->installEventFilter(this);
  ui.tableView->setSortingEnabled(false);
//   ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  ui.tableView->horizontalHeader()->setDefaultSectionSize(200);
  connect(&pdialog, SIGNAL(runCancelled()), SLOT(stopRun()));
  connect(ui.tableView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(SetSelectionName()));

//   connect(ui.tableView->horizontalHeader(),SIGNAL(sectionClicked(int)), SLOT(SortByColumn(int)));
}

Table::Table(QStringList names, QList<QPixmap> images, QList<QColor> colors, QWidget *parent): QWidget(parent)
{
  ui.setupUi(this);
  model_ = 0;
  int nrows = (int)ceil(names.size()/10.f);
  imgtabmodel_ = new QStandardItemModel(nrows, 10, this);
  ui.tableView->setModel(imgtabmodel_);

  int k = 0;
  for(int i = 0; i < nrows; i++){
    for(int j = 0; j < 10; j++){
      if(k < names.size()){

        QLabel *image = new QLabel();
        image->setMinimumSize(50, 50);
        image->resize(50, 50);
        image->setPixmap(images[k]);
        image->setScaledContents(true);
        image->pixmap()->scaled(50, 50, Qt::KeepAspectRatioByExpanding);

        QLabel *label = new QLabel;
        /*QPalette pal = label->palette();
        pal.setColor(QPalette::Window,colors[k]);
        pal.setColor(QPalette::WindowText, colors[k]);
        label->setAutoFillBackground(true);
        label->setPalette(pal);*/
        label->setText(names[k]);
        QVariant variant= colors[k];
        QString colcode = variant.toString();
        label->setStyleSheet("QLabel { background-color :"+colcode+" ; border-style: outset; border-width: 2px; border-radius: 10px; border-color: beige; font: bold 9px; min-width: 10em; padding: 6px; color : black; }");
        QVBoxLayout *layout = new QVBoxLayout;
        if(images[k].isNull() == false)
          layout->addWidget(image);

        layout->addWidget(label);
        QWidget *cell = new QWidget;
        cell->setLayout(layout);

        imgtabmodel_->setItem(i, j, new QStandardItem(QString("")));
        ui.tableView->setIndexWidget(imgtabmodel_->index(i , j), cell);
        k++;
      }
      else{
        continue;
      }
    }
  }

  pid = -1;
  //ui.tableView->setModel(imgtabmodel_);

  ui.tableView->adjustSize();
  ui.tableView->installEventFilter(this);
  ui.tableView->setSortingEnabled(false);
//   ui.tableView->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
  ui.tableView->verticalHeader()->setDefaultSectionSize(150);
  ui.tableView->horizontalHeader()->setDefaultSectionSize(150);
  //ui.tableView->horizontalHeader()->sectionResizeMode(QHeaderView::Stretch);
  //ui.tableView->verticalHeader()->sectionResizeMode(QHeaderView::Stretch);
//   connect(ui.tableView->horizontalHeader(),SIGNAL(sectionClicked(int)), SLOT(SortByColumn(int)));
}

Table::~Table()
{
  #ifdef DEBUG
  qDebug()<<"~Table() Delete model_\n";
  #endif
  if(model_)
    delete model_;
  else if(imgtabmodel_)
    delete imgtabmodel_;
}
