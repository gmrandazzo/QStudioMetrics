#include "ClassModelDialog.h"
#include "data.h"
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include "qstudiometricstypes.h"

#include "addLabelDialog.h"

void ClassModelDialog::genListView(QModelIndex current)
{
  if(current.isValid()){
    
    selectedproject_ = pids[current.row()];
    
    tab2->clear();
    if(projects_->keys().contains(selectedproject_) == true){
      for(int i = 0; i < projects_->value(selectedproject_)->MatrixCount(); i++){
        QList<QStandardItem*> mname;
        mname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(i)->getName()));
        tab2->appendRow(mname);
      }
      
      ui.objSelectByLabel->clear();
      
      ui.objSelectByLabel->addItem("Select by label...");
      for(int i = 0; i < projects_->value(selectedproject_)->getObjectLabels().size(); i++){
        ui.objSelectByLabel->addItem(projects_->value(selectedproject_)->getObjectLabels()[i].name);
      }
      
      ui.varSelectByLabel->clear();
      ui.varSelectByLabel->addItem("Select by label...");
      for(int i = 0; i < projects_->value(selectedproject_)->getVariableLabels().size(); i++){
        ui.varSelectByLabel->addItem(projects_->value(selectedproject_)->getVariableLabels()[i].name);
      }
    }
  }
}


void ClassModelDialog::EnableDisableButtons()
{
  if(state == 0){
    if(ui.listView_1->selectionModel()->selectedRows().size() > 0
      && ui.listView_2->selectionModel()->selectedRows().size() > 0){
      ui.nextButton->setEnabled(true);
      ui.prevButton->setEnabled(false);
      ui.okButton->setEnabled(false);
    }
    else{
      ui.nextButton->setEnabled(false);
      ui.prevButton->setEnabled(false);
      ui.okButton->setEnabled(false);
    }
  }
  else if(state == 1){
    if(ui.listView_4->model()->rowCount() > 0){
      ui.nextButton->setEnabled(true);
      ui.prevButton->setEnabled(true);
      ui.okButton->setEnabled(false);
    }
    else{
      ui.nextButton->setEnabled(false);
      ui.prevButton->setEnabled(true);
      ui.okButton->setEnabled(false);
    }
  }
  else if(state == 2){
    if(ui.listView_5->selectionModel()->selectedRows().size() > 0){
      ui.okButton->setEnabled(true);
      ui.nextButton->setEnabled(false);
      ui.prevButton->setEnabled(true);
    }
    else{
      ui.okButton->setEnabled(false);
      ui.nextButton->setEnabled(false);
      ui.prevButton->setEnabled(true);
    }
  }
  else{
    return;
  }
}

void ClassModelDialog::AddClass()
{
  addLabelDialog groupnamedialog("Group Name");
  if(groupnamedialog.exec() == QDialog::Accepted){
    QModelIndexList selected = ui.listView_3->selectionModel()->selectedRows();
    if(selected.size() > 0){
      classes.append(QStringList());
      
      QList<QStandardItem*> row;
      row.append(new QStandardItem(groupnamedialog.getLabel()));
      tab4->appendRow(row);
      nameclasses.append(groupnamedialog.getLabel());
      
      for(int i = 0; i < selected.size(); i++){
        QString obj = selected[i].data(Qt::DisplayRole).toString();
        classes.last().append(obj);
      }
      
      // Remove selected index from the view
      QModelIndexList indexes;
      while((indexes = ui.listView_3->selectionModel()->selectedIndexes()).size()) { 
        ui.listView_3->model()->removeRow(indexes.first().row()); 
      }
      EnableDisableButtons();
    }
    else{
      EnableDisableButtons();
      return;
    }
  }
  else{
    EnableDisableButtons();
    return;
  }
}

void ClassModelDialog::RemoveClass()
{
  QModelIndexList indexes = ui.listView_4->selectionModel()->selectedIndexes();
  
  if(indexes.size() < 1)
    return;
  
  for(int i = 0; i < indexes.size(); i++){
    int indx = indexes[i].row();
    if(indx > -1 && indx < classes.size()){
      classes.removeAt(indx);
      nameclasses.removeAt(indx);
      ui.listView_4->model()->removeRow(indx);
    }
    else{
      EnableDisableButtons();
      return;
    }
  }
  EnableDisableButtons();
}

void ClassModelDialog::ObjSelectAll()
{
  ui.listView_3->selectAll();
  EnableDisableButtons();
}

void ClassModelDialog::ObjInvertSelection()
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
  EnableDisableButtons();
}

void ClassModelDialog::ObjSelectBy()
{
  if(ui.objSelectByLabel->currentIndex() > 0){
    int labelindex = ui.objSelectByLabel->currentIndex()-1;
    QItemSelection selection;
    for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
      if(projects_->value(selectedproject_)->getObjectLabels()[labelindex].objects.contains(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_3->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_3->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_3->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.objSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.objSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void ClassModelDialog::ObjUnselectAll()
{
  ui.listView_3->selectionModel()->clear();
  EnableDisableButtons();
}

void ClassModelDialog::VarSelectAll()
{
  ui.listView_5->selectAll();
  EnableDisableButtons();
}

void ClassModelDialog::VarInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_5->selectionModel();
  QAbstractItemModel *model = ui.listView_5->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  EnableDisableButtons();
}

void ClassModelDialog::VarSelectBy()
{
  if(ui.varSelectByLabel->currentIndex() > 0){
    int labelindex = ui.varSelectByLabel->currentIndex()-1;
    QItemSelection selection;
    for(int i = 0; i < ui.listView_5->model()->rowCount(); i++){
      if(projects_->value(selectedproject_)->getVariableLabels()[labelindex].objects.contains(ui.listView_4->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_5->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_5->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_5->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.varSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.varSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void ClassModelDialog::VarUnselectAll()
{
  ui.listView_5->selectionModel()->clear();
  EnableDisableButtons();
}

void ClassModelDialog::setData(QModelIndex current)
{
  if(current.isValid()){
    selecteddata_ = current.row();
  }
}

void ClassModelDialog::next()
{
  if(state == 0 && selecteddata_ > -1 && projects_->keys().contains(selectedproject_) == true && !ui.modelname->text().isEmpty()){
    state++;
//     ui.prevButton->setEnabled(true);
//     ui.nextButton->setEnabled(true);
    ui.objectGroupBox->show();
    ui.variableGroupBox->hide();
    ui.modnameFrame->hide();
    ui.projectsGroupBox->hide();
    ui.dataGroupBox->hide();
    
    tab3->clear();
    for(int i = 0; i < projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName().size(); i++){
      QList<QStandardItem*> mname;
      mname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName()[i]));
      tab3->appendRow(mname);
    }
    resize(ui.objectGroupBox->minimumSize());
    adjustSize();
  }
  else if (state == 1 && classes.size() > 0){
    state++;
//     ui.prevButton->setEnabled(true);
//     ui.nextButton->setEnabled(false);
    ui.objectGroupBox->hide();
    ui.variableGroupBox->show();
    ui.modnameFrame->hide();
    ui.projectsGroupBox->hide();
    ui.dataGroupBox->hide();
    
    tab5->clear();
    for(int i = 1; i < projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName().size(); i++){
      QList<QStandardItem*> name;
      name.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName()[i]));
      tab5->appendRow(name);
    }
    resize(ui.variableGroupBox->minimumSize());
  }
  else{
    return;
  }
  EnableDisableButtons();
}


void ClassModelDialog::previous()
{
  if(state == 1){
    state--;
//     ui.prevButton->setEnabled(false);
//     ui.nextButton->setEnabled(true);
//     ui.okButton->setEnabled(false);
    ui.objectGroupBox->hide();
    ui.variableGroupBox->hide();
    ui.modnameFrame->show();
    ui.projectsGroupBox->show();
    ui.dataGroupBox->show();
    adjustSize();
  }
  else if(state == 2){
//     ui.prevButton->setEnabled(true);
//     ui.nextButton->setEnabled(true);
//     ui.okButton->setEnabled(false);
    ui.objectGroupBox->show();
    ui.variableGroupBox->hide();
    ui.modnameFrame->hide();
    ui.projectsGroupBox->hide();
    ui.dataGroupBox->hide();
    resize(ui.objectGroupBox->minimumSize());
  }
  else{
    return;
  }
  EnableDisableButtons();
}

void ClassModelDialog::OK()
{ 
  if(selectedproject_ == -1 || selecteddata_ == -1 || ui.modelname->text().isEmpty()){
    QMessageBox::warning(this, tr("Warning!"), tr("No project, data selected or no model name setted!\nPlease check your settings.\n"), QMessageBox::Close);
    return;
  }
  else{
    modelname_ = ui.modelname->text();
    
    for(int i = 0; i < ui.listView_4->model()->rowCount(); i++){
      if(ui.listView_4->selectionModel()->isSelected(ui.listView_4->model()->index(i, 0)) == true){
        ui.listView_4->model()->index(i, 0).data(Qt::DisplayRole).toString();
        
//         classes.append();
      }
      else{
        continue;
      }
    }
    
    for(int i = 0; i < ui.listView_5->model()->rowCount(); i++){
      if(ui.listView_5->selectionModel()->isSelected(ui.listView_5->model()->index(i, 0)) == true){
        varsel.append(ui.listView_5->model()->index(i, 0).data(Qt::DisplayRole).toString());
      }
      else{
        continue;
      }
    }
    
    compute_ = true;
    accept();
  }
}

ClassModelDialog::ClassModelDialog(PROJECTS *projects)
{
  ui.setupUi(this);
  
  state = 0;
  
  setWindowTitle("Compute LDA");
//   ui.okButton->setEnabled(false);
//   ui.prevButton->setEnabled(false);
  ui.objectGroupBox->hide();
  ui.variableGroupBox->hide();
  
  selecteddata_ = -1;
  
  compute_ = false;
  
  projects_ = projects;
  
  if(projects_->size() > 0){
    selectedproject_ = 0;
  }
  else{
    selectedproject_ = -1;
  }
  
  tab1 = new QStandardItemModel();   
  tab2 = new QStandardItemModel();
  tab3 = new QStandardItemModel();
  tab4 = new QStandardItemModel();
  tab5 = new QStandardItemModel();
  
  ui.listView_1->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.listView_3->setModel(tab3);
  ui.listView_4->setModel(tab4);
  ui.listView_5->setModel(tab5);
   
 //Fill the table with data
  QList<QStandardItem*> projectsname;
  for(int i = 0; i < projects_->keys().size(); i++){
    int pid = projects_->keys()[i];
    projectsname.append(new QStandardItem(projects_->value(pid)->getProjectName()));
    pids.append(pid);
  }
  tab1->appendColumn(projectsname);
  
  connect(ui.listView_1->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(genListView(QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setData(QModelIndex)));
  
  connect(ui.objSelectAllButton, SIGNAL(clicked(bool)), SLOT(ObjSelectAll()));
  connect(ui.objInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(ObjInvertSelection()));
  connect(ui.objUnselectButton, SIGNAL(clicked(bool)), SLOT(ObjUnselectAll()));
  connect(ui.objSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(ObjSelectBy()));

  connect(ui.varSelectAllButton, SIGNAL(clicked(bool)), SLOT(VarSelectAll()));
  connect(ui.varInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(VarInvertSelection()));
  connect(ui.varUnselectButton, SIGNAL(clicked(bool)), SLOT(VarUnselectAll()));
  connect(ui.varSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(VarSelectBy()));
  
  connect(ui.listView_1->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_2->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_4->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_5->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  adjustSize();
  
  
  connect(ui.nextButton, SIGNAL(clicked(bool)), SLOT(next()));
  connect(ui.prevButton, SIGNAL(clicked(bool)), SLOT(previous()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.addClass, SIGNAL(clicked()), SLOT(AddClass()));
  connect(ui.removeClass, SIGNAL(clicked()), SLOT(RemoveClass()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(OK()));
  EnableDisableButtons();
}

ClassModelDialog::~ClassModelDialog()
{
  delete tab1;
  delete tab2;
  delete tab3;
  delete tab4;
  delete tab5;
}
