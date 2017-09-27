#include "ExtractDataDialog.h"
#include <QString>
#include <QMessageBox>
#include "qstudiometricstypes.h"
#include "ObjectSelectorDialog.h"
#include "run.h"
#include "VariableSelectorDialog.h"

void ExtractDataDialog::StartSelectionRun()
{
  stoprun = false;
  pdialog.setMin(0);
  pdialog.setMax(0);
  pdialog.setValue(0);
  pdialog.show();
}

void ExtractDataDialog::StopSelectionRun()
{
  stoprun = true;
  pdialog.setMin(0);
  pdialog.setMax(100);
  pdialog.setValue(20);
  pdialog.hide();
}

void ExtractDataDialog::setProjectID(QModelIndex current)
{
  if(current.isValid()){
    pid = pids[current.row()];
    genListView();
  }
  else{
    pid = -1;
  }
}

void ExtractDataDialog::genObjVarView(QModelIndex current)
{
  if(current.isValid()){
    selectedhash_ = hash[current.row()];
    
    tab3->clear();
    tab4->clear();
    tab3->setHorizontalHeaderItem(0, new QStandardItem("Object Names"));
    tab4->setHorizontalHeaderItem(0, new QStandardItem("Variable Names"));
    for(int i = 0; i < projects_->value(pid)->getMatrix(selectedhash_)->getObjName().size(); i++){
//       for(int i = 0; i < (int)projects_->value(pid)->getMatrix(selectedhash_)->Matrix()->row; i++){
      QList<QStandardItem*> row;
      row.append(new QStandardItem(projects_->value(pid)->getMatrix(selectedhash_)->getObjName()[i]));
      tab3->appendRow(row);
    }
    
    for(int i = 0; i < projects_->value(pid)->getMatrix(selectedhash_)->getVarName().size(); i++){
      if(projects_->value(pid)->getMatrix(selectedhash_)->getVarName()[i].compare("Object Names") == 0){
        continue;
      }
      else{
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(pid)->getMatrix(selectedhash_)->getVarName()[i]));
        tab4->appendRow(row);
      }
    }
    UpdateSelectedObjectCounter();
    UpdateSelectedVariableCounter();
  }
  else{
    return;
  }
}

void ExtractDataDialog::genListView()
{  
  if(pid != -1){
    tab2->clear();
    hash.clear();
    if(projects_->value(pid)->MatrixCount() > 0){
      for(int i = 0; i < projects_->value(pid)->MatrixCount(); i++){
        QList<QStandardItem*> tab2matrix;
        tab2matrix.append(new QStandardItem(projects_->value(pid)->getMatrix(i)->getName()));
        hash.append(projects_->value(pid)->getMatrix(i)->getHash());
        tab2->appendRow(tab2matrix);
      }
    }
  }
  else{
    return;
  }
}

void ExtractDataDialog::VariablesUnselect()
{
  ui.tableView_4->selectionModel()->clear();
}

void ExtractDataDialog::VariablesSelectBy()
{
  if(pid != -1 && selectedhash_.size() > 0){
    QItemSelection selection;
    VariableSelectorDialog vseldialog(projects_->value(pid)->getMatrix(selectedhash_)->getVarName(),
                                      &projects_->value(pid)->getVariableLabels());
    
    if(vseldialog.exec() == QDialog::Accepted){
      QStringList varselected = vseldialog.getSelectedVariables();
      
      for(int i = 0; i < ui.tableView_4->model()->rowCount(); i++){
        if(varselected.indexOf(ui.tableView_4->model()->index(i, 0).data(Qt::DisplayRole).toString()) > -1){
          QModelIndex topLeft = ui.tableView_4->model()->index(i, 0);
          QModelIndex bottomRight = ui.tableView_4->model()->index(i, 0);
          selection << QItemSelectionRange(topLeft, bottomRight);
        }
        else{
          continue;
        }
      }
    }
    else{
      return;
    }
    ui.tableView_4->selectionModel()->select(selection, QItemSelectionModel::Select);
  }
  else{
    return;
  }
}

void ExtractDataDialog::VariablesInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.tableView_4->selectionModel();
  QAbstractItemModel *model = ui.tableView_4->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

void ExtractDataDialog::VariablesSelectAll()
{
  ui.tableView_4->selectAll();
  /*
  for(int i = 0; i < ui.tableView_4->model()->rowCount(); i++){
    ui.tableView_4->selectionModel()->select(ui.tableView_4->model()->index(i, 0), QItemSelectionModel::Select);
  }
  */
}

void ExtractDataDialog::ObjectsUnselect()
{
  ui.tableView_3->selectionModel()->clear();
}

void ExtractDataDialog::ObjectsSelectBy()
{
  bool doselectionfrommatrix = false;
  int selectiontype, metric, nobjects, dataid, layerid;
  QString label;
  QStringList varlist;
  
  selectiontype = metric = nobjects = dataid = layerid = -1;
  
  ObjectSelectorDialog objseldialog(&projects_->value(pid)->getMATRIXList(), &projects_->value(pid)->getObjectLabels(), &projects_->value(pid)->getVariableLabels(), ObjectSelectorDialog::MenuSelection);
  if(objseldialog.exec() == QDialog::Accepted){
    selectiontype = objseldialog.getSelectionType();
    metric = objseldialog.getMetric();
    nobjects = objseldialog.getNumberOfObjects();
    dataid = objseldialog.getDataID();
    label = objseldialog.getLabel();
    varlist = objseldialog.getVariableList();
    doselectionfrommatrix = true;
  }
  else{
    return;
  }
  
  if(doselectionfrommatrix == true){
    if((selectiontype == MOSTDESCRIPTIVECOMPOUND || selectiontype == MAXIMUMDISSIMILARITYMAXMIN) && metric != -1 && nobjects != -1 && dataid != -1 && varlist.size() > 0){
      matrix *m;
      uivector *selected;
      RUN obj;
      
      NewMatrix(&m, projects_->value(pid)->getMatrix(dataid)->Matrix()->row, varlist.size());
      QStringList varnames = projects_->value(pid)->getMatrix(dataid)->getVarName();
      varnames.removeAll("Object Names");
      
      int col = 0;
      for(int j = 0; j < varlist.size(); j++){
        int colindex = varnames.indexOf(varlist[j]);
        if(colindex !=  -1){
          for(uint i = 0; i < projects_->value(pid)->getMatrix(dataid)->Matrix()->row; i++){
            setMatrixValue(m, i, col, getMatrixValue(projects_->value(pid)->getMatrix(dataid)->Matrix(), i, colindex));
          }
          col++;
        }
        else{
          qDebug() << "Error in selection columnindex this variable " << varlist[j] << "seems to be not present.";
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
      
      ui.tableView_3->selectionModel()->clearSelection();
  
      for(uint i = 0; i < selected->size; i++){
        if(getUIVectorValue(selected, i) < projects_->value(pid)->getMatrix(dataid)->Matrix()->row){
          ui.tableView_3->selectionModel()->select(ui.tableView_3->model()->index(getUIVectorValue(selected, i), 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
      }
      
      DelMatrix(&m);
      DelUIVector(&selected);
      StopSelectionRun();
    }
    else if(selectiontype == RANDOMSELECTION && nobjects != -1 && dataid != -1){
      uivector *selected;
      RUN obj;
      
      qDebug() << "dataid " << dataid;
      initUIVector(&selected);
      
      obj.setUIVector(selected);
      obj.setNumberOfObject(nobjects);
      obj.setNumberMaxOfObject(projects_->value(pid)->getMatrix(dataid)->Matrix()->row);
      
      StartSelectionRun();
      QFuture<void> future = obj.RunRandomSelection();
      
      while(!future.isFinished()){
        if(stoprun == true){
          future.cancel();
        }
        else{
          QApplication::processEvents();
        }
      }
      
      ui.tableView_3->selectionModel()->clearSelection();
  
      for(uint i = 0; i < selected->size; i++){
        if(getUIVectorValue(selected, i) < projects_->value(pid)->getMatrix(dataid)->Matrix()->row){
          ui.tableView_3->selectionModel()->select(ui.tableView_3->model()->index(getUIVectorValue(selected, i), 0), QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
      }
      
      DelUIVector(&selected);
      StopSelectionRun();
    }
    else{
      if(label.size() > 0){
        int lindex = -1;
        for(int i = 0; i < projects_->value(pid)->getObjectLabels().size(); i++){
          if(projects_->value(pid)->getObjectLabels()[i].name.compare(label) == 0){
            lindex = i;
            break;
          }
          else{
            continue;
          }
        }
        if(lindex > -1){
          ui.tableView_3->selectionModel()->clearSelection();
          
          for(int i = 0; i < projects_->value(pid)->getObjectLabels()[lindex].objects.size(); i++){
            for(int j = 0; j < ui.tableView_3->model()->rowCount(); j++){
              if(ui.tableView_3->model()->index(j, 0).data(Qt::DisplayRole).toString().compare(projects_->value(pid)->getObjectLabels()[lindex].objects[i]) == 0){
               ui.tableView_3->selectionModel()->select(ui.tableView_3->model()->index(j, 0), QItemSelectionModel::Select); 
               break;
              }
              else{
                continue;
              }
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
  else{
    return;
  }
}

void ExtractDataDialog::ObjectsInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.tableView_3->selectionModel();
  QAbstractItemModel *model = ui.tableView_3->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

void ExtractDataDialog::ObjectsSelectAll()
{
  ui.tableView_3->selectAll();
  /*
  for(int i = 0; i < ui.tableView_3->model()->rowCount(); i++){
    ui.tableView_3->selectionModel()->select(ui.tableView_3->model()->index(i, 0), QItemSelectionModel::Select);
  }
  */
}

void ExtractDataDialog::UpdateSelectedObjectCounter()
{
  ui.nselobj->setText(QString("%1/%2").arg(QString::number(ui.tableView_3->selectionModel()->selectedIndexes().size())).arg(QString::number(ui.tableView_3->model()->rowCount())));
}

void ExtractDataDialog::UpdateSelectedVariableCounter()
{
  ui.nselvar->setText(QString("%1/%2").arg(QString::number(ui.tableView_4->selectionModel()->selectedIndexes().size())).arg(QString::number(ui.tableView_4->model()->rowCount())));
}

void ExtractDataDialog::OK()
{
  QModelIndexList objectselected = ui.tableView_3->selectionModel()->selectedIndexes();
  QModelIndexList variableselected = ui.tableView_4->selectionModel()->selectedIndexes();
  
  if(objectselected.size() > 0 && variableselected.size() > 0 && !ui.dataname->text().isEmpty()){
    matrix *_m_ = mx->Matrix();
    
    ResizeMatrix(&_m_, objectselected.size(), variableselected.size());
    
    QStringList objnames, varnames;
    
    varnames.append("Object Names");
    uint row = 0, col = 0;
    for(int i = 0; i < ui.tableView_3->model()->rowCount(); i++){
      if(i == 0){ /*Fill the variable names*/
        for(int j = 0; j < ui.tableView_4->model()->rowCount(); j++){
          if(ui.tableView_4->selectionModel()->isSelected(ui.tableView_4->model()->index(j, 0)) == true){
            varnames.append(projects_->value(pid)->getMatrix(selectedhash_)->getVarName()[j+1]);
          }
          else{
            continue;
          }
        }
      }
      
      if(ui.tableView_3->selectionModel()->isSelected(ui.tableView_3->model()->index(i, 0)) == true){
        objnames.append(projects_->value(pid)->getMatrix(selectedhash_)->getObjName()[i]);
        col = 0;
        for(int j = 0; j < ui.tableView_4->model()->rowCount(); j++){
          if(ui.tableView_4->selectionModel()->isSelected(ui.tableView_4->model()->index(j, 0)) == true){
            setMatrixValue(_m_, row, col, getMatrixValue(projects_->value(pid)->getMatrix(selectedhash_)->Matrix(), i, j));
            col++;
          }
          else{
            continue;
          }
        }
        row++;
      }
      else{
        continue;
      }
    }
    
    mx->setName(ui.dataname->text());
    mx->getObjName().append(objnames);
    mx->getVarName().append(varnames);
    mx->GenHash();
    
    return accept();
  }
  else{
    QMessageBox::warning(this, tr("Merge Data Error"), tr("Please select data and/or specify the new data name."), QMessageBox::Ok);
  }
}

ExtractDataDialog::ExtractDataDialog(PROJECTS* projects): QDialog()
{
  ui.setupUi(this);
  mx = new MATRIX();
  
  stoprun = false;
  projects_ = projects;
  
  pid = -1;
  
  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();
  tab3 = new QStandardItemModel();   
  tab4 = new QStandardItemModel();
  
  ui.nselobj->setText("0/0");
  ui.nselvar->setText("0/0");
  
  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.tableView_3->setModel(tab3);
  ui.tableView_4->setModel(tab4);
  
 //Fill the table with data
  QList<QStandardItem*> projectsname;
  for(int i = 0; i < projects_->keys().size(); i++){
    projectsname.append(new QStandardItem(projects_->value(projects_->keys()[i])->getProjectName()));
    pids.append(projects_->keys()[i]);
  }
  tab1->appendColumn(projectsname);  
  
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(OK()));
  
  connect(ui.listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setProjectID(QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(genObjVarView(QModelIndex)));
  connect(ui.tableView_3->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(UpdateSelectedObjectCounter()));
  connect(ui.tableView_4->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(UpdateSelectedVariableCounter()));
  
  connect(ui.objSelectAllButton, SIGNAL(clicked(bool)), SLOT(ObjectsSelectAll()));
  connect(ui.objInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(ObjectsInvertSelection()));
  connect(ui.objSelectByButton, SIGNAL(clicked(bool)), SLOT(ObjectsSelectBy()));
  connect(ui.objUnselectButton, SIGNAL(clicked(bool)), SLOT(ObjectsUnselect()));
  connect(ui.varSelectAllButton, SIGNAL(clicked(bool)), SLOT(VariablesSelectAll()));
  connect(ui.varInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(VariablesInvertSelection()));
  connect(ui.varSelectByButton, SIGNAL(clicked(bool)), SLOT(VariablesSelectBy()));
  connect(ui.varUnselectButton, SIGNAL(clicked(bool)), SLOT(VariablesUnselect()));  
}

ExtractDataDialog::~ExtractDataDialog()
{
  qDebug() << "Delete ExtractDataDialog Data";
  delete tab1;
  delete tab2;  
  delete tab3;
  delete tab4;
  delete mx;
}
