#include "ValidatorDialog.h"
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>

#include "ClassDialog.h"

void ValidatorDialog::setYScramblingBlock()
{
  block = ui.block->value();
}

void ValidatorDialog::setYScrambling()
{
  if(ui.YScramblingGroupBox->isChecked()){
    yscrambling = true;
  }
  else{
    yscrambling= false;
  }

  if(selectedproject_ > -1 && projects_->keys().contains(selectedproject_) == true && modelid > -1){
     int yscrambling_blocks = 0;
    if(type == PLSValidation){
      if(modelid < projects_->value(selectedproject_)->PLSCount())
        yscrambling_blocks = (int)ceil((projects_->value(selectedproject_)->getPLSModel(modelid)->getObjName().size() * 5) / 17.);
    }
    else if(type == MLRValidation){
      if(modelid < projects_->value(selectedproject_)->MLRCount())
        yscrambling_blocks = (int)ceil((projects_->value(selectedproject_)->getMLRModel(modelid)->getObjName().size() * 5)/17.);
    }
    else if(type == LDAValidation){
      return;
    }
    ui.block->setValue(yscrambling_blocks);
  }
}

void ValidatorDialog::setNIterations()
{
  niter = ui.iterations->value();
}

void ValidatorDialog::setNGroup()
{
  ngroup = ui.groupnumber->value();
}

void ValidatorDialog::setValidationType()
{
  if(ui.leaveoneout->isChecked()){
    validtype = LOO_; // cross validation
  }
  else{
    validtype = BOOTSTRAPRGCV_; // cross validation
  }
}

void ValidatorDialog::setModelID(QModelIndex current)
{
  if(current.isValid() && projects_->keys().contains(selectedproject_) == true && selectedproject_ > -1){
    modelid = mids[current.row()];
  }
}

void ValidatorDialog::setProject(QModelIndex current)
{
  if(current.isValid()){
    // set project
    selectedproject_ = pids[current.row()];

    // create the value for the second tab
    tab2->clear();
    mids.clear();
    if(projects_->keys().contains(selectedproject_) == true){
      if(type == PLSValidation){
        for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
      else if(type == MLRValidation){
        for(int i = 0; i < projects_->value(selectedproject_)->MLRCount(); i++){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getMLRModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getMLRModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
      else if(type == LDAValidation){
        for(int i = 0; i < projects_->value(selectedproject_)->LDACount(); i++){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getLDAModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getLDAModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
  }
}

void ValidatorDialog::OK()
{

  if(selectedproject_ == -1 || modelid == -1){
    QMessageBox::warning(this, tr("Warning!"), tr("Please select a project and a model to validate.\n"), QMessageBox::Close);
  }
  else{
    compute_ = true;
    accept();
  }
}

ValidatorDialog::ValidatorDialog(PROJECTS *projects, int type_)
{
  ui.setupUi(this);

  type = type_;

  projects_ = projects;

  if(type == PLSValidation)
    setWindowTitle("Compute PLS Validation");
  else if(type == MLRValidation)
    setWindowTitle("Compute MLR Validation");
  else if(type == LDAValidation){
    setWindowTitle("Compute LDA Validation");
    ui.YScramblingGroupBox->hide();
    ui.label->hide();
    ui.block->hide();
  }


  if(projects_->size() > 0){
    selectedproject_ = 0;
  }
  else{
    selectedproject_ = -1;
  }

  modelid = -1;
  niter = ui.iterations->value();
  ngroup = ui.groupnumber->value();
  yscrambling = false;
  block = ui.block->value();

  setValidationType();
  compute_ = false;

  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();

  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);


 //Fill the table with data
  QList<QStandardItem*> projectsname;
  for(int i = 0; i < projects_->keys().size(); i++){
    int pid = projects_->keys()[i];
    projectsname.append(new QStandardItem(projects_->value(pid)->getProjectName()));
    pids.append(pid);
  }
  tab1->appendColumn(projectsname);

  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(OK()));

  connect(ui.leaveoneout, SIGNAL(clicked(bool)), SLOT(setValidationType()));
  connect(ui.crossvalid, SIGNAL(clicked(bool)), SLOT(setValidationType()));

  connect(ui.listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setProject(QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setModelID(QModelIndex)));

  connect(ui.groupnumber, SIGNAL(valueChanged(int)), SLOT(setNGroup()));
  connect(ui.iterations, SIGNAL(valueChanged(int)), SLOT(setNIterations()));

  connect(ui.YScramblingGroupBox, SIGNAL(clicked(bool)), SLOT(setYScrambling()));
  connect(ui.block, SIGNAL(valueChanged(int)), SLOT(setYScramblingBlock()));

  adjustSize();
  ui.listView->selectionModel()->clear();
  ui.listView_2->selectionModel()->clear();
}

ValidatorDialog::~ValidatorDialog()
{
  delete tab1;
  delete tab2;
}
