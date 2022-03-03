#include "ModelDialog.h"
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include "qstudiometricstypes.h"

void ModelDialog::genListView(QModelIndex current)
{
  if(current.isValid()){

    selectedproject_ = pids[current.row()];

    tab2->clear();
    if(projects_->keys().contains(selectedproject_) == true){
      if(type == PCA_ || type == PLS_ || type == MLR_ || type == LDA_){
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


void ModelDialog::EnableOKButton()
{
  if(type == PCA_){
    if(ui.listView_3->selectionModel()->selectedRows(0).size() > 0
      && ui.listView_4->selectionModel()->selectedRows(0).size() > 0){
      ui.okButton->setEnabled(true);
    }
    else{
      ui.okButton->setEnabled(false);
    }
  }
  else{
    if(ui.listView_3->selectionModel()->selectedRows(0).size() > 0
      && ui.listView_4->selectionModel()->selectedRows(0).size() > 0
      && ui.listView_5->selectionModel()->selectedRows(0).size() > 0){
      ui.okButton->setEnabled(true);
    }
    else{
      ui.okButton->setEnabled(false);
    }
  }
}

void ModelDialog::ObjSelectAll()
{
  ui.listView_3->selectAll();
  EnableOKButton();
}

void ModelDialog::ObjInvertSelection()
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
  EnableOKButton();
}

void ModelDialog::ObjSelectBy()
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

void ModelDialog::ObjUnselectAll()
{
  ui.listView_3->selectionModel()->clear();
  EnableOKButton();
}

void ModelDialog::XVarSelectAll()
{
  ui.listView_4->selectAll();
  EnableOKButton();
}

void ModelDialog::XVarInvertSelection()
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

  EnableOKButton();
}

void ModelDialog::XVarSelectBy()
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

void ModelDialog::XVarUnselectAll()
{
  ui.listView_4->selectionModel()->clear();
  EnableOKButton();
}

void ModelDialog::YVarSelectAll()
{
  ui.listView_5->selectAll();
  EnableOKButton();
}

void ModelDialog::YVarInvertSelection()
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
  EnableOKButton();
}

void ModelDialog::YVarSelectBy()
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

void ModelDialog::YVarUnselectAll()
{
  ui.listView_5->selectionModel()->clear();
  EnableOKButton();
}

void ModelDialog::setYData(QModelIndex current)
{
  if(current.isValid()){
    ydata = current.row();
  }
}

void ModelDialog::setData(QModelIndex current)
{
  if(current.isValid()){
    selecteddata_ = current.row();
  }
}

void ModelDialog::next()
{
  if(type == PCA_){
    if(state == 0 && selecteddata_ > -1 && projects_->keys().contains(selectedproject_) == true && !ui.modelname->text().isEmpty()){
      state++;
      ui.prevButton->setEnabled(true);
      ui.nextButton->setEnabled(false);

      ui.objectGroupBox->show();
      ui.XvariableGroupBox->show();
      ui.YvariableGroupBox->hide();
      ui.modnameFrame->hide();
      ui.projectsGroupBox->hide();
      ui.dataGroupBox->hide();
      ui.OptGroupBox->hide();

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
      adjustSize();
    }
    else{
      QMessageBox::warning(this, tr("Warning!"), tr("No Project or data selected or PCA model name specified!\nPlease select project and data and check the PCA model name.\n"), QMessageBox::Close);
      return;
    }
  }
  else if(type == PLS_ || type == MLR_ || type == LDA_){
    if(state == 0 && selecteddata_ > -1 && projects_->keys().contains(selectedproject_) == true && !ui.modelname->text().isEmpty()){
      state++;
      ui.prevButton->setEnabled(true);
      ui.nextButton->setEnabled(false);

      ui.objectGroupBox->show();
      ui.XvariableGroupBox->show();
      ui.YvariableGroupBox->show();
      ui.modnameFrame->hide();
      ui.projectsGroupBox->hide();
      ui.dataGroupBox->hide();
      ui.OptGroupBox->hide();

      if(type == LDA_){
        ui.yvarSelectAllButton->hide();
        ui.yvarSelectByLabel->hide();
        ui.yvarInvertSelectionButton->hide();
        ui.yvarUnselectButton->hide();
      }

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
      adjustSize();
    }
    else{
      if(type == PLS_){
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
    if(state == 0 && selecteddata_ > -1 && ydata > -1 &&  projects_->keys().contains(selectedproject_) == true && !ui.modelname->text().isEmpty()){
      state++;
      ui.prevButton->setEnabled(true);
      ui.nextButton->setEnabled(false);

      ui.objectGroupBox->show();
      ui.XvariableGroupBox->show();
      ui.YvariableGroupBox->show();
      ui.modnameFrame->hide();
      ui.projectsGroupBox->hide();
      ui.dataGroupBox->hide();
      ui.OptGroupBox->hide();

      // Generate Object listview and Variable listview
      tab3->clear();
      tab4->clear();
      tab5->clear();

      for(int i = 0; i < projects_->value(selectedproject_)->getArray(selecteddata_)->getObjName().size(); i++){
        QList<QStandardItem*> oname;
        oname.append(new QStandardItem(projects_->value(selectedproject_)->getArray(selecteddata_)->getObjName()[i]));
        tab3->appendRow(oname);
      }

      for(int i = 1; i < projects_->value(selectedproject_)->getArray(selecteddata_)->getVarName().size(); i++){
        QList<QStandardItem*> xvname, yvname;
        xvname.append(new QStandardItem(projects_->value(selectedproject_)->getArray(selecteddata_)->getVarName()[i]));
        yvname.append(new QStandardItem(projects_->value(selectedproject_)->getArray(selecteddata_)->getVarName()[i]));
        tab4->appendRow(xvname);

      }

      for(int i = 1; i < projects_->value(selectedproject_)->getArray(ydata)->getVarName().size(); i++){
        QList<QStandardItem*> xvname, yvname;
        xvname.append(new QStandardItem(projects_->value(selectedproject_)->getArray(ydata)->getVarName()[i]));
        yvname.append(new QStandardItem(projects_->value(selectedproject_)->getArray(ydata)->getVarName()[i]));
        tab5->appendRow(xvname);
      }
      adjustSize();
    }
    else{
      return;
    }
  }
}


void ModelDialog::previous()
{
  if(state == 1){
    state--;
    if(type == PCA_){
      ui.prevButton->setEnabled(false);
      ui.nextButton->setEnabled(true);
      ui.okButton->setEnabled(false);
      ui.objectGroupBox->hide();
      ui.XvariableGroupBox->hide();
      ui.YvariableGroupBox->hide();
      ui.modnameFrame->show();
      ui.projectsGroupBox->show();
      ui.dataGroupBox->show();
      ui.OptGroupBox->show();
      ui.xScalingFrame->show();
      ui.yScalingFrame->hide();
      adjustSize();
    }
    else if(type == PLS_){
      ui.prevButton->setEnabled(false);
      ui.nextButton->setEnabled(true);
      ui.okButton->setEnabled(false);
      ui.objectGroupBox->hide();
      ui.XvariableGroupBox->hide();
      ui.YvariableGroupBox->hide();
      ui.modnameFrame->show();
      ui.projectsGroupBox->show();
      ui.dataGroupBox->show();
      ui.OptGroupBox->show();
      adjustSize();
    }
    else{ //MLR
      ui.prevButton->setEnabled(false);
      ui.nextButton->setEnabled(true);
      ui.okButton->setEnabled(false);
      ui.objectGroupBox->hide();
      ui.XvariableGroupBox->hide();
      ui.YvariableGroupBox->hide();
      ui.modnameFrame->show();
      ui.projectsGroupBox->show();
      ui.dataGroupBox->show();
      ui.OptGroupBox->hide();
      adjustSize();
    }
  }
  else{
    adjustSize();
    return;
  }
}

void ModelDialog::OK()
{
  if(selectedproject_ == -1 || selecteddata_ == -1 || ui.modelname->text().isEmpty()){
    QMessageBox::warning(this, tr("Warning!"), tr("No project, data selected or no model name setted!\nPlease check your settings.\n"), QMessageBox::Close);
    return;
  }
  else{
    modelname_ = ui.modelname->text();

    if(type == PCA_){
      if(type == PCA_){
        if(ui.NPrincipalComponent->value() > (int)projects_->value(selectedproject_)->getMatrix(selecteddata_)->Matrix()->col){
          n_pc = projects_->value(selectedproject_)->getMatrix(selecteddata_)->Matrix()->col;
        }
        else{
          n_pc = ui.NPrincipalComponent->value();
        }
      }
      else{
        if(ui.NPrincipalComponent->value() > (int)projects_->value(selectedproject_)->getArray(selecteddata_)->Array()->m[0]->col){
          n_pc = projects_->value(selectedproject_)->getArray(selecteddata_)->Array()->m[0]->col;
        }
        else{
          n_pc = ui.NPrincipalComponent->value();
        }
      }
      xscaling = ui.xscalinglist->currentIndex();
    }
    else if(type == PLS_){

      if(ui.NPrincipalComponent->value() > (int)projects_->value(selectedproject_)->getMatrix(selecteddata_)->Matrix()->col){
        n_pc = projects_->value(selectedproject_)->getMatrix(selecteddata_)->Matrix()->col;
      }
      else{
        n_pc = ui.NPrincipalComponent->value();
      }
    // set the autoscaling type
      xscaling = ui.xscalinglist->currentIndex();
      yscaling = ui.yscalinglist->currentIndex();
    }
    // else MLR no other variable to set...

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

    compute_ = true;
    accept();
  }
}

ModelDialog::ModelDialog(PROJECTS *projects, int type_)
{
  ui.setupUi(this);

  state = 0;
  type = type_;
  if(type == PCA_){
    setWindowTitle("Compute PCA");
    ui.yScalingFrame->hide();
  }
  else if(type == PLS_){
    ui.OptGroupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute PLS");
  }
  else if(type == MLR_){
    setWindowTitle("Compute MLR");
    ui.OptGroupBox->hide();
  }
  else if(type == LDA_){
    setWindowTitle("Compute LDA");
    ui.OptGroupBox->hide();
    ui.listView_5->setSelectionMode(QAbstractItemView::SingleSelection);
  }

  ui.okButton->setEnabled(false);
  ui.prevButton->setEnabled(false);
  ui.objectGroupBox->hide();
  ui.XvariableGroupBox->hide();
  ui.YvariableGroupBox->hide();


  selecteddata_ = ydata = -1;
  n_pc = ui.NPrincipalComponent->value();
  xscaling = yscaling = 0;

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


  connect(ui.nextButton, SIGNAL(clicked(bool)), SLOT(next()));
  connect(ui.prevButton, SIGNAL(clicked(bool)), SLOT(previous()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(OK()));

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

  connect(ui.listView_3->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableOKButton()));
  connect(ui.listView_4->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableOKButton()));
  connect(ui.listView_5->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableOKButton()));



  adjustSize();
}

ModelDialog::~ModelDialog()
{
  delete tab1;
  delete tab2;
  delete tab3;
  delete tab4;
  delete tab5;
}
