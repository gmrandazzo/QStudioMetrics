#include "ClassPlotDialog.h"
#include "PlotDialog.h"
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"
#include "addLabelDialog.h"

#ifdef DEBUG
#include <QDebug>
#endif 

void ClassPlotDialog::CheckPlot()
{
  if(selectedproject_ != -1){
    if(modelid != -1){
      ui.plotButton->setEnabled(true);
    }
    else{
      ui.plotButton->setEnabled(false);
    }
  }
  else{
    ui.plotButton->setEnabled(false);
  }
}


void ClassPlotDialog::Find()
{
  for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
    if(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString().contains(ui.lineEdit->text(), Qt::CaseSensitive) == true
      && ui.listView_3->selectionModel()->isSelected(ui.listView_3->model()->index(i, 0)) == false){
      ui.listView_3->scrollTo(ui.listView_3->model()->index(i, 0));
      ui.listView_3->selectionModel()->select(ui.listView_3->model()->index(i, 0), QItemSelectionModel::Select);
      break;
    }
    else{
      continue;
    }
  }
}

void ClassPlotDialog::FindAll()
{
  for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
    if(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString().contains(ui.lineEdit->text(), Qt::CaseSensitive) == true
      && ui.listView_3->selectionModel()->isSelected(ui.listView_3->model()->index(i, 0)) == false){
      ui.listView_3->scrollTo(ui.listView_3->model()->index(i, 0));
      ui.listView_3->selectionModel()->select(ui.listView_3->model()->index(i, 0), QItemSelectionModel::Select);
    }
    else{
      continue;
    }
  }

}


void ClassPlotDialog::AddGroup()
{
  addLabelDialog groupnamedialog("Group Name");
  if(groupnamedialog.exec() == QDialog::Accepted){
    QModelIndexList selected = ui.listView_3->selectionModel()->selectedRows();
    if(selected.size() > 0){
      #ifdef DEBUG
      qDebug() << " AddGroup Selected " << selected.size();
      #endif
      g.append(LABEL());
      g.last().name = groupnamedialog.getLabel();
      
      for(int i = 0; i < selected.size(); i++){
//         QString name = ui.listView_3->selectionModel()->selectedRows()[i].data(Qt::DisplayRole).toString();
        QString obj = selected[i].data(Qt::DisplayRole).toString();
//         g.last().gname += QString("%1,").arg(obj );
        g.last().objects.append(obj );
      }
      
      // Remove selected index from the view
      QModelIndexList indexes;
      while((indexes = ui.listView_3->selectionModel()->selectedIndexes()).size()) { 
        ui.listView_3->model()->removeRow(indexes.first().row()); 
      }
      
      QList<QStandardItem*> row;
      row.append(new QStandardItem(g.last().name));
      tab4->appendRow(row);
    }
    else{
      return;
    }
  }
  else{
    return;
  }
}

void ClassPlotDialog::SelectAll()
{
  ui.listView_3->selectAll();
}

void ClassPlotDialog::InvertSelection()
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
}

void ClassPlotDialog::SelectBy()
{
  if(ui.selbycomboBox->currentIndex()-1 < labels.size()){
    QItemSelection selection;
    for(int i = 0; i < ui.listView_3->model()->rowCount(); i++) {
      if(labels[i].objects.indexOf(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString()) > -1){
        QModelIndex topLeft = ui.listView_3->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_3->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
    }
    ui.listView_3->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.selbycomboBox->setCurrentIndex(0);
  }
  else{
    ui.selbycomboBox->setCurrentIndex(0);
    return;
  }
}

void ClassPlotDialog::UnselectAll()
{
  ui.listView_3->selectionModel()->clear();
}

void ClassPlotDialog::RemoveGroup()
{
  QModelIndexList indexes = ui.listView_4->selectionModel()->selectedRows();
  if(indexes.size() < 1)
    return;
  
  for(int j = 0; j < indexes.size(); j++){
    int indx = -1;
    for(int i = 0; i < g.size(); i++){
      if(g[i].name.compare(indexes[j].data(Qt::DisplayRole).toString()) == 0){
        indx = i;
        break;
      }
      else{
        continue;
      }
    }
    
    if(indx > -1){
      for(int i = 0; i < g[indx].objects.size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(g[indx].objects[i]));
        tab3->appendRow(row);
      }
      g.removeAt(indx);
      ui.listView_4->model()->removeRow(indexes[j].row());
    }
  }
}

void ClassPlotDialog::setModelID(QModelIndex current)
{
  if(current.isValid() && selectedproject_ != -1){
    modelid = mids[current.row()];
    tab3->clear();
    if(type == PCALOADINGS){
      for(int i = 0; i < projects_->value(selectedproject_)->getPCAModel(modelid)->getVarName().size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getPCAModel(modelid)->getVarName()[i]));
        tab3->appendRow(row);
      }
    }
    else if(type == PLSLOADINGS){
      for(int i = 0; i < projects_->value(selectedproject_)->getPLSModel(modelid)->getXVarName().size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModel(modelid)->getXVarName()[i]));
        tab3->appendRow(row);
      }
      for(int i = 0; i < projects_->value(selectedproject_)->getPLSModel(modelid)->getYVarName().size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModel(modelid)->getYVarName()[i]));
        tab3->appendRow(row);
      }
    }
    else if(type == UPCALOADINGS){
      for(int i = 0; i < projects_->value(selectedproject_)->getUPCAModel(modelid)->getVarName().size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getUPCAModel(modelid)->getVarName()[i]));
        tab3->appendRow(row);
      }
    }
    else if(type == UPLSLOADINGS){
      for(int i = 0; i < projects_->value(selectedproject_)->getUPLSModel(modelid)->getXVarName().size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModel(modelid)->getXVarName()[i]));
        tab3->appendRow(row);
      }
      
      for(int i = 0; i < projects_->value(selectedproject_)->getUPLSModel(modelid)->getYVarName().size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModel(modelid)->getYVarName()[i]));
        tab3->appendRow(row);
      }
    }
    
    if(type == PCALOADINGS || type == PLSLOADINGS || type == UPCALOADINGS || type == UPLSLOADINGS){
      labels = projects_->value(selectedproject_)->getVariableLabels();
    }
    
    for(int i = 0; i < labels.size(); i++){
      ui.selbycomboBox->addItem(projects_->value(selectedproject_)->getVariableLabels()[i].name);
    }
  }
  else{
    modelid = -1;
  }
  CheckPlot();
}

void ClassPlotDialog::setProject(QModelIndex current)
{
  if(current.isValid()){
    // set project
    selectedproject_ = pids[current.row()];
    // create the value for the second tab
    tab2->clear();
    mids.clear();
      
    if(type == PCALOADINGS){ // pca
      for(int i = 0; i < projects_->value(selectedproject_)->PCACount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getPCAModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getPCAModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == PLSLOADINGS){ // pls
      for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == UPCALOADINGS){ // upca
      for(int i = 0; i < projects_->value(selectedproject_)->UPCACount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getUPCAModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getUPCAModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == UPLSLOADINGS){ // upls
      for(int i = 0; i < projects_->value(selectedproject_)->UPLSCount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getUPLSModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
  }
  else{
    selectedproject_ = -1;
  }
  CheckPlot();
}

void ClassPlotDialog::actionPlot()
{  
  if(selectedproject_ == -1 || modelid == -1){
    QMessageBox::warning(this, tr("Warning!"), tr("Please select project and model to plot.\n"), QMessageBox::Close);
    return;
  }
  else{
    #ifdef DEBUG
    qDebug() << "PLOTDIALOG SELECTED PROJECT: " << projects_->value(selectedproject_)->getProjectName() << " Model ID " << modelid;
    #endif
    plot_ = true;
    accept();
  }
}


ClassPlotDialog::ClassPlotDialog(PROJECTS *projects, int type_)
{
  ui.setupUi(this);

  ui.listView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui.listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);  
  
  projects_ = projects;
  plot_ = false;
  type = type_;
  selectedproject_ = modelid = -1;
  
  tab1 = new QStandardItemModel();   
  tab2 = new QStandardItemModel();
  tab3 = new QStandardItemModel();
  tab4 = new QStandardItemModel();
  
  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.listView_3->setModel(tab3);
  ui.listView_4->setModel(tab4);

  for(int i = 0; i < projects_->keys().size(); i++){
    int pid = projects_->keys()[i];
    bool acquire = false;
    if(type == PCALOADINGS && projects_->value(pid)->PCACount() > 0){ // PCA
      acquire = true;
    }
    else if(type == PLSLOADINGS && projects_->value(pid)->PLSCount() > 0){ // PLS
      acquire = true;
    }
    else if(type == UPCALOADINGS && projects_->value(pid)->UPCACount() > 0){ // UPCA
      acquire = true;
    }
    else if(type == UPLSLOADINGS && projects_->value(pid)->UPLSCount() > 0){ // UPLS
      acquire = true;
    }
    else{
      acquire = false;
    }

    if(acquire == true){
      QList<QStandardItem*> row;
      row.append(new QStandardItem(projects_->value(pid)->getProjectName()));
      tab1->appendRow(row);
      pids.append(pid);
    }
    else{
      continue;
    } 
  }

  connect(ui.plotButton, SIGNAL(clicked()), SLOT(actionPlot()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setProject(QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setModelID(QModelIndex)));
  connect(ui.findButton, SIGNAL(clicked(bool)), SLOT(Find()));
  connect(ui.findAllButton, SIGNAL(clicked(bool)), SLOT(FindAll()));
  connect(ui.addgroupButton, SIGNAL(clicked(bool)), SLOT(AddGroup()));
  connect(ui.removegroupButton, SIGNAL(clicked(bool)), SLOT(RemoveGroup()));
  
  
  connect(ui.selectButton, SIGNAL(clicked(bool)), SLOT(SelectAll()));
  connect(ui.invertselectionButton, SIGNAL(clicked(bool)), SLOT(InvertSelection()));
  connect(ui.selbycomboBox, SIGNAL(currentIndexChanged(int)), SLOT(SelectBy()));
  connect(ui.unselectButton, SIGNAL(clicked(bool)), SLOT(UnselectAll()));
  
//   connect(ui.listView_3->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT());

  CheckPlot();
}

ClassPlotDialog::~ClassPlotDialog()
{
  delete tab1;
  delete tab2;
  delete tab3;
  delete tab4;
}
