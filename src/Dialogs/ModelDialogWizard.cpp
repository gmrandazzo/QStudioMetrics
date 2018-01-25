#include "ModelDialogWizard.h"

#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>
#include "qstudiometricstypes.h"

void ModelDialogWizard::genListView(QModelIndex current)
{
  if(current.isValid()){

    selectedproject_ = pids[current.row()];

    tab2->clear();
    if(projects_->keys().contains(selectedproject_) == true){
      if(type == PCA_ || type == PLS_ || type == PLS_DA_ || type == EPLS_ || type == EPLS_DA_ || type == MLR_ || type == LDA_){
        if(projects_->value(selectedproject_)->MatrixCount() > 0){
          for(int i = 0; i < projects_->value(selectedproject_)->MatrixCount(); i++){
            QList<QStandardItem*> tab2matrix;
            tab2matrix.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(i)->getName()));
            tab2->appendRow(tab2matrix);
          }
        }
      }
      ui.objSelectByLabel->clear();

      ui.objSelectByLabel->addItem("Select by label...");
      for(int i = 0; i < projects_->value(selectedproject_)->getObjectLabels().size(); i++){
        ui.objSelectByLabel->addItem(projects_->value(selectedproject_)->getObjectLabels()[i].name);
      }

      ui.xvarSelectByLabel->clear();
      ui.yvarSelectByLabel->clear();
      ui.xvarSelectByLabel->addItem("Select by label...");
      ui.yvarSelectByLabel->addItem("Select by label...");
      for(int i = 0; i < projects_->value(selectedproject_)->getVariableLabels().size(); i++){
        ui.xvarSelectByLabel->addItem(projects_->value(selectedproject_)->getVariableLabels()[i].name);
        ui.yvarSelectByLabel->addItem(projects_->value(selectedproject_)->getVariableLabels()[i].name);
      }
    }
  }
}

void ModelDialogWizard::EnableDisableButtons()
{
  if(ui.modelname->text().isEmpty() == true ||
     ui.listView_1->selectionModel()->selectedRows(0).size() == 0 ||
     ui.listView_2->selectionModel()->selectedRows(0).size() == 0){
    this->button(QWizard::NextButton)->setEnabled(false);
    this->button(QWizard::FinishButton)->setEnabled(false);
  }
  else{
    this->button(QWizard::NextButton)->setEnabled(true);
    this->button(QWizard::FinishButton)->setEnabled(false);

    if(ui.listView_3->selectionModel()->selectedRows(0).size() > 0 &&
      ui.listView_4->selectionModel()->selectedRows(0).size() > 0){
      if(type == PCA_){
        this->button(QWizard::FinishButton)->setEnabled(true);
      }
      else{
        if(ui.listView_5->selectionModel()->selectedRows(0).size() > 0){
          this->button(QWizard::FinishButton)->setEnabled(true);
        }
        else{
          this->button(QWizard::FinishButton)->setEnabled(false);
        }
      }
    }
  }
}

void ModelDialogWizard::ELmethodChanged(int indx)
{
  if(indx == 0){
    ui.label_2->hide();
    ui.nvarsSpinBox->hide();
    ui.label->show();
    ui.trainSizeSpinBox->show();
  }
  else if(indx == 1){
    ui.label_2->show();
    ui.nvarsSpinBox->show();
    ui.label->hide();
    ui.trainSizeSpinBox->hide();
  }
  else{
    ui.label_2->show();
    ui.nvarsSpinBox->show();
    ui.label->show();
    ui.trainSizeSpinBox->show();
  }
}

void ModelDialogWizard::ObjSelectAll()
{
  ui.listView_3->selectAll();
  EnableDisableButtons();
}

void ModelDialogWizard::ObjInvertSelection()
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

void ModelDialogWizard::ObjSelectBy()
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

void ModelDialogWizard::ObjUnselectAll()
{
  ui.listView_3->selectionModel()->clear();
  EnableDisableButtons();
}

void ModelDialogWizard::XVarSelectAll()
{
  ui.listView_4->selectAll();
  EnableDisableButtons();
}

void ModelDialogWizard::XVarInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_4->selectionModel();
  QAbstractItemModel *model = ui.listView_4->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);

  EnableDisableButtons();
}

void ModelDialogWizard::XVarSelectBy()
{
  if(ui.xvarSelectByLabel->currentIndex() > 0){
    int labelindex = ui.xvarSelectByLabel->currentIndex()-1;
    QItemSelection selection;
    for(int i = 0; i < ui.listView_4->model()->rowCount(); i++){
      if(projects_->value(selectedproject_)->getVariableLabels()[labelindex].objects.contains(ui.listView_4->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_4->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_4->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_4->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.xvarSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.xvarSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void ModelDialogWizard::XVarUnselectAll()
{
  ui.listView_4->selectionModel()->clear();
  EnableDisableButtons();
}

void ModelDialogWizard::YVarSelectAll()
{
  ui.listView_5->selectAll();
  EnableDisableButtons();
}

void ModelDialogWizard::YVarInvertSelection()
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

void ModelDialogWizard::YVarSelectBy()
{
  if(ui.yvarSelectByLabel->currentIndex() > 0){
    int labelindex = ui.yvarSelectByLabel->currentIndex()-1;
    QItemSelection selection;
    for(int i = 0; i < ui.listView_5->model()->rowCount(); i++){
      if(projects_->value(selectedproject_)->getVariableLabels()[labelindex].objects.contains(ui.listView_5->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_5->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_5->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_5->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.yvarSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.yvarSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void ModelDialogWizard::YVarUnselectAll()
{
  ui.listView_5->selectionModel()->clear();
  EnableDisableButtons();
}

void ModelDialogWizard::setYData(QModelIndex current)
{
  if(current.isValid()){
    ydata = current.row();
  }
}

void ModelDialogWizard::setData(QModelIndex current)
{
  if(current.isValid()){
    selecteddata_ = current.row();
  }
}

void ModelDialogWizard::next()
{
  adjustSize();
  if(type == PCA_){
    if(selecteddata_ > -1 && projects_->keys().contains(selectedproject_) == true && !ui.modelname->text().isEmpty()){
      // Generate Object listview and Variable listview
      tab3->clear();
      tab4->clear();
      tab5->clear();
      for(int i = 0; i < projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName().size(); i++){
        QList<QStandardItem*> oname;
        oname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName()[i]));
        tab3->appendRow(oname);
      }

      for(int i = 1; i < projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName().size(); i++){
        QList<QStandardItem*> xvname;
        xvname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName()[i]));
        tab4->appendRow(xvname);
      }
    }
    else{
      QMessageBox::warning(this, tr("Warning!"), tr("No Project or data selected or PCA model name specified!\nPlease select project and data and check the PCA model name.\n"), QMessageBox::Close);
      return;
    }
  }
  else if(type == PLS_ || type == PLS_DA_ || type == EPLS_ || type == EPLS_DA_ || type == MLR_ || type == LDA_){
    if(selecteddata_ > -1 && projects_->keys().contains(selectedproject_) == true && !ui.modelname->text().isEmpty()){
      // Generate Object listview and Variable listview
      tab3->clear();
      tab4->clear();
      tab5->clear();

      for(int i = 0; i < projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName().size(); i++){
        QList<QStandardItem*> oname;
        oname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName()[i]));
        tab3->appendRow(oname);
      }

      for(int i = 1; i < projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName().size(); i++){
        QList<QStandardItem*> xvname, yvname;
        xvname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName()[i]));
        yvname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName()[i]));
        tab4->appendRow(xvname);
        tab5->appendRow(yvname);
      }
    }
    else{
      if(type == PLS_ || type == PLS_DA_ || type == EPLS_ || type == EPLS_DA_){
        QMessageBox::warning(this, tr("Warning!"), tr("No Project or data selected or PLS model name specified!\nPlease select project and data and check the PLS model name.\n"), QMessageBox::Close);
        return;
      }
      if(type == LDA_){
        QMessageBox::warning(this, tr("Warning!"), tr("No Project or data selected or LDA model name specified!\nPlease select project and data and check the LDA model name.\n"), QMessageBox::Close);
        return;
      }
      else{
        QMessageBox::warning(this, tr("Warning!"), tr("No Project or data selected or MLR model name specified!\nPlease select project and data and check the MLR model name.\n"), QMessageBox::Close);
        return;
      }
    }
  }
  else{
    return;
  }
}

void ModelDialogWizard::OK()
{
  if(selectedproject_ == -1 || selecteddata_ == -1 || ui.modelname->text().isEmpty()){
    QMessageBox::warning(this, tr("Warning!"), tr("No project, data selected or no model name setted!\nPlease check your settings.\n"), QMessageBox::Close);
    reject();
    return;
  }
  else{
    modelname_ = ui.modelname->text();

    if(ui.NPrincipalComponent->value() > (int)projects_->value(selectedproject_)->getMatrix(selecteddata_)->Matrix()->col){
      n_pc = projects_->value(selectedproject_)->getMatrix(selecteddata_)->Matrix()->col;
    }
    else{
      n_pc = ui.NPrincipalComponent->value();
    }

    /*if(ui.NPrincipalComponent->value() > (int)projects_->value(selectedproject_)->getArray(selecteddata_)->Array()->m[0]->col){
      n_pc = projects_->value(selectedproject_)->getArray(selecteddata_)->Array()->m[0]->col;
    }
    else{
      n_pc = ui.NPrincipalComponent->value();
    }
    */
    xscaling = ui.xscalinglist->currentIndex();
    yscaling = ui.yscalinglist->currentIndex();


    elmethod = ui.elmethodComboBox->currentIndex();

    eparm.n_models = (int)ui.nmodelsSpinBox->value();

    if(elmethod == 0){
      eparm.algorithm = Bagging;
      eparm.trainsize = ui.trainSizeSpinBox->value()/100.;
      eparm.r_fix = 0;
    }
    else if(elmethod == 1){
      eparm.algorithm = FixedRandomSubspaceMethod;
      eparm.trainsize = 0;
      eparm.r_fix = ui.nvarsSpinBox->value();
    }
    else{
      eparm.algorithm = BaggingRandomSubspaceMethod;
      eparm.trainsize = ui.trainSizeSpinBox->value()/100.;
      eparm.r_fix = ui.nvarsSpinBox->value();
    }

    for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
      if(ui.listView_3->selectionModel()->isSelected(ui.listView_3->model()->index(i, 0)) == true){
        objsel.append(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString());
      }
      else{
        continue;
      }
    }

    for(int i = 0; i < ui.listView_4->model()->rowCount(); i++){
      if(ui.listView_4->selectionModel()->isSelected(ui.listView_4->model()->index(i, 0)) == true){
        xvarsel.append(ui.listView_4->model()->index(i, 0).data(Qt::DisplayRole).toString());
      }
      else{
        continue;
      }
    }

    for(int i = 0; i < ui.listView_5->model()->rowCount(); i++){
      if(ui.listView_5->selectionModel()->isSelected(ui.listView_5->model()->index(i, 0)) == true){
        yvarsel.append(ui.listView_5->model()->index(i, 0).data(Qt::DisplayRole).toString());
      }
      else{
        continue;
      }
    }

    if(objsel.size() > 0 && xvarsel.size() > 0){
      if(type == PCA_){
        compute_ = true;
        accept();
      }
      else{
        if(yvarsel.size() > 0){
          compute_ = true;
          accept();
        }
        else{
          compute_ = false;
          reject();
        }
      }
    }
  }
}

ModelDialogWizard::ModelDialogWizard(PROJECTS *projects, int type_, QWidget *parent)
{
  ui.setupUi(this);

  type = type_;
  if(type == PCA_){
    setWindowTitle("Compute PCA");
    ui.yScaling->hide();
    ui.YvariableGroupBox->hide();
    ui.YclassGroupBox->hide();
    ui.ELearningMethodGroupBox->hide();
  }
  else if(type == PLS_){
    ui.groupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute PLS Regression");
    ui.YclassGroupBox->hide();
    ui.YvariableGroupBox->show();
    ui.ELearningMethodGroupBox->hide();
  }
  else if(type == PLS_DA_){
    ui.groupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute PLS Discriminant Analysis");
    ui.YclassGroupBox->show();
    ui.YvariableGroupBox->hide();
    ui.ELearningMethodGroupBox->hide();
  }
  else if(type == EPLS_){
    ui.groupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute Ensemble PLS Regression");
    ui.YclassGroupBox->hide();
    ui.YvariableGroupBox->show();
  }
  else if(type == EPLS_DA_){
    ui.groupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute Ensemble PLS Discriminant Analysis");
    ui.YclassGroupBox->show();
    ui.YvariableGroupBox->hide();
  }
  else if(type == MLR_){
    setWindowTitle("Compute MLR");
    ui.groupBox->hide();
    ui.YclassGroupBox->hide();
    ui.ELearningMethodGroupBox->hide();
  }
  else if(type == LDA_){
    setWindowTitle("Compute LDA");
    ui.groupBox->hide();
    ui.yvarSelectAllButton->hide();
    ui.yvarSelectByLabel->hide();
    ui.yvarInvertSelectionButton->hide();
    ui.yvarUnselectButton->hide();
    ui.listView_5->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.ELearningMethodGroupBox->hide();
  }

  selecteddata_ = ydata = -1;
  n_pc = ui.NPrincipalComponent->value();
  xscaling = yscaling = 0;
  elmethod = 0;

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
  tab6 = new QStandardItemModel();
  tab7 = new QStandardItemModel();

  ui.listView_1->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.listView_3->setModel(tab3);
  ui.listView_4->setModel(tab4);
  ui.listView_5->setModel(tab5);
  ui.listView_6->setModel(tab6);
  ui.listView_7->setModel(tab7);

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

  connect(ui.xvarSelectAllButton, SIGNAL(clicked(bool)), SLOT(XVarSelectAll()));
  connect(ui.xvarInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(XVarInvertSelection()));
  connect(ui.xvarUnselectButton, SIGNAL(clicked(bool)), SLOT(XVarUnselectAll()));
  connect(ui.xvarSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(XVarSelectBy()));

  connect(ui.yvarSelectAllButton, SIGNAL(clicked(bool)), SLOT(YVarSelectAll()));
  connect(ui.yvarInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(YVarInvertSelection()));
  connect(ui.yvarUnselectButton, SIGNAL(clicked(bool)), SLOT(YVarUnselectAll()));
  connect(ui.yvarSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(YVarSelectBy()));

  connect(ui.modelname, SIGNAL(textChanged(QString)), SLOT(EnableDisableButtons()));
  connect(ui.listView_1->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_2->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_3->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_4->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_5->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));

  connect(ui.elmethodComboBox, SIGNAL(currentIndexChanged(int)), SLOT(ELmethodChanged(int)));

  connect(this->button(QWizard::NextButton), SIGNAL(clicked()), this, SLOT(next()));
  connect(this->button(QWizard::FinishButton),SIGNAL(clicked()), this, SLOT(OK()));

  this->button(QWizard::NextButton)->setEnabled(false);
  this->button(QWizard::FinishButton)->setEnabled(false);
  ELmethodChanged(0);

  adjustSize();
}

ModelDialogWizard::~ModelDialogWizard()
{
  delete tab1;
  delete tab2;
  delete tab3;
  delete tab4;
  delete tab5;
  delete tab6;
  delete tab7;
}
