#include "VariableSelectionDialog.h"
#include <QMessageBox>
#include "qstudiometricstypes.h"
/*
void VariableSelectionDialog::CheckDataCompatibility()
{
  QStringList xnamlst, ynamlst;
  QByteArray xnamestr, ynamestr;
  
  if(selectedproject_ != -1 && selectedmodel_ != -1 ){
    if(type == PLSVariableSelection){
      xnamlst = projects_->value(selectedproject_)->getMatrix(selectedxdata_)->getObjName();
      ynamlst = projects_->value(selectedproject_)->getMatrix(selectedydata_)->getObjName();
    }
    else{
      xnamlst = projects_->value(selectedproject_)->getArray(selectedxdata_)->getObjName();
      ynamlst = projects_->value(selectedproject_)->getArray(selectedydata_)->getObjName();
    }
    
    qSort(xnamlst.begin(), xnamlst.end());
    qSort(ynamlst.begin(), ynamlst.end());
    
    for(int i = 0; i < xnamlst.size(); i++){
      xnamestr.append(xnamlst[i]);
    }
    
    for(int i = 0; i < ynamlst.size(); i++){
      ynamestr.append(ynamlst[i]);
    }
    
    QString xhashname = QString(QCryptographicHash::hash(xnamestr,QCryptographicHash::Md5).toHex());
    QString yhashname = QString(QCryptographicHash::hash(ynamestr,QCryptographicHash::Md5).toHex());
    
    if(xhashname.compare(yhashname) == 0){
      ui.okButton->setEnabled(true);
    }
    else{
      QMessageBox::critical(this, tr("Variable Selection Error"),
      tr("Unable to compute Variable Selection.\n"
      "The data seems to be incompatible by checking name... Please check your data."),
      QMessageBox::Ok);
      ui.okButton->setEnabled(false);
    }
  }
  else{
    ui.okButton->setEnabled(false);
  }
}
*/
void VariableSelectionDialog::UpdateCrossoverType()
{
  if(ui.GAcrossovertype->currentIndex() == 2){
    ui.GAcrossowerSwappinesBox->show();
    ui.label_15->show();
  }
  else{
    ui.GAcrossowerSwappinesBox->hide();
    ui.label_15->hide();
  }
  crossovertype = ui.GAcrossovertype->currentIndex();
}

void VariableSelectionDialog::setModel(QModelIndex current)
{
  if(current.isValid()){
    selectedmodel_ = mids[current.row()];
  }
  else{
    selectedmodel_ = -1;
  }
}

void VariableSelectionDialog::setProject(QModelIndex current)
{
  if(current.isValid()){
    // set project 
    selectedproject_ = pids[current.row()];
    // create the value for the second tab
    tab2->clear();
    
    selectedmodel_ = -1;
    mids.clear();
    if(selectedproject_ > -1){
      if(type == PLSVariableSelection){
        for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
          tab2->appendRow(row);
          mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
        }
      }
      else{
        for(int i = 0; i < projects_->value(selectedproject_)->UPLSCount(); i++){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModelAt(i)->getName()));
          tab2->appendRow(row);
          mids.append(projects_->value(selectedproject_)->getUPLSModelAt(i)->getModelID());
        } 
      }
    }
  }
  else{
    selectedproject_ = -1;
  }
}

void VariableSelectionDialog::OK()
{
  if(selectedproject_ > -1 && selectedmodel_ > -1 && !ui.varselname->text().isEmpty()){
    
    varselname = ui.varselname->text();
    /*check and set the validation type*/
    if(ui.leaveoneout->isChecked()){
      validtype = LOO;
    }
//     else if(ui.randomgroup->isChecked()){
    else{
      validtype = RANDOMGROUP;
    }
    
    if(ui.gaButton->isChecked()){
      varselalgo = GA;
      popsize = ui.GApopulationBox->value();
      crossover = ui.GAcrossoverBox->value();
      crossovertype = ui.GAcrossovertype->currentIndex();
      nswappiness = (double)ui.GAcrossowerSwappinesBox->value()/100.f;
      mutationrate = ui.GAmutationBox->value();
      popconverge = ui.GApopulationConvergenceBox->value()/100.f;
    }
    else if(ui.psoButton->isChecked()){ /*PSO*/
      varselalgo = PSO;
      randomvar = ui.PSOrandomnesBox->value()/100.f;
    }
    else{
      varselalgo = SPEARMAN;
      threshold = ui.threshold->value();
    }
    
    accept();
  }
  else{
    QMessageBox::warning(this, tr("Warning!"), tr("Please select project, data and set a variable selection name.\n"), QMessageBox::Close);
    return;
  }
}

VariableSelectionDialog::VariableSelectionDialog(PROJECTS* projects, int type_): QDialog()
{
  ui.setupUi(this);
  
  type = type_;
  
  projects_ = projects;
  
  if(type == PLSVariableSelection)
    setWindowTitle("PLS Variable Selection");
  else{
    setWindowTitle("UPLS Variable Selection");
    ui.spearmanButton->hide();;
    ui.threshold->hide();
    ui.label_3->hide();
  }

  selectedproject_ = selectedmodel_ = -1;
  
  validtype = LOO; // leave one out
  ui.leaveoneout->setChecked(true);
  
  niter = ui.iterations->value();
  ngroup = ui.groupnumber->value();

  varselalgo = GA;
  popsize = ui.GApopulationBox->value();
  crossover = ui.GAcrossoverBox->value();
  crossovertype = ui.GAcrossovertype->currentIndex();
  UpdateCrossoverType();
  nswappiness = (double)ui.GAcrossowerSwappinesBox->value()/100.f;
  mutationrate = ui.GAmutationBox->value();
  
  randomvar = ui.PSOrandomnesBox->value();
  
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
  
  connect(ui.listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setProject(QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setModel(QModelIndex)));
  connect(ui.GAcrossovertype, SIGNAL(currentIndexChanged(int)), SLOT(UpdateCrossoverType()));
  
}

VariableSelectionDialog::~VariableSelectionDialog()
{
  delete tab1;
  delete tab2;
}

