#include "DoPredictionDialog.h"
#include <QMessageBox>
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>

void DoPredictionDialog::CheckDataForPrediction() {
  if (selectedmodel_ > -1 && selecteddata_ > -1) {
    QStringList varsel, vardata;
    if (type == PCA_) {
      varsel = projects_->value(selectedproject_)
                   ->getPCAModel(selectedmodel_)
                   ->getVarName();
      vardata = projects_->value(selectedproject_)
                    ->getMatrix(selecteddata_)
                    ->getVarName();
    } else if (type == CPCA_) {
      for (int k = 0; k < projects_->value(selectedproject_)
                              ->getCPCAModel(selectedmodel_)
                              ->getVarName()
                              .size();
           k++) {
        varsel << projects_->value(selectedproject_)
                      ->getCPCAModel(selectedmodel_)
                      ->getVarName()[k]
                      .objects;
      }
      vardata = projects_->value(selectedproject_)
                    ->getMatrix(selecteddata_)
                    ->getVarName();
    } else if (type == PLS_ || type == PLS_DA_) {
      varsel = projects_->value(selectedproject_)
                   ->getPLSModel(selectedmodel_)
                   ->getXVarName();
      vardata = projects_->value(selectedproject_)
                    ->getMatrix(selecteddata_)
                    ->getVarName();
    } else if (type == MLR_) {
      varsel = projects_->value(selectedproject_)
                   ->getMLRModel(selectedmodel_)
                   ->getXVarName();
      vardata = projects_->value(selectedproject_)
                    ->getMatrix(selecteddata_)
                    ->getVarName();
    } else if (type == LDA_) {
      varsel = projects_->value(selectedproject_)
                   ->getLDAModel(selectedmodel_)
                   ->getVarName();
      vardata = projects_->value(selectedproject_)
                    ->getMatrix(selecteddata_)
                    ->getVarName();
    }

    int nvarmatch = 0;
    for (int i = 0; i < varsel.size(); i++) {
      if (vardata.indexOf(varsel[i]) > -1) {
        nvarmatch++;
      } else {
        continue;
      }
    }

    if (nvarmatch == varsel.size()) {
      ui.nextButton->setEnabled(true);
    } else {
      QMessageBox::warning(
          this, tr("Warnig"),
          tr("The selected data have different number of descriptors with the "
             "selected model. \n\nNo prediction can be calculated."),
          QMessageBox::Ok);
      ui.nextButton->setEnabled(false);
    }
  } else {
    ui.nextButton->setEnabled(true);
  }
}

void DoPredictionDialog::VarUnselectAll() {
  ui.listView_5->selectionModel()->clear();
}

void DoPredictionDialog::VarSelectBy() {
  if (ui.yvarSelectByLabel->currentIndex() > 0) {
    int labelindex = ui.yvarSelectByLabel->currentIndex() - 1;
    QItemSelection selection;
    for (int i = 0; i < ui.listView_5->model()->rowCount(); i++) {
      if (projects_->value(selectedproject_)
              ->getVariableLabels()[labelindex]
              .objects.contains(ui.listView_5->model()
                                    ->index(i, 0)
                                    .data(Qt::DisplayRole)
                                    .toString()) == true) {
        QModelIndex topLeft = ui.listView_5->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_5->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      } else {
        continue;
      }
    }
    ui.yvarSelectByLabel->setCurrentIndex(0);
    ui.listView_5->selectionModel()->select(selection,
                                            QItemSelectionModel::Select);
  } else {
    ui.yvarSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void DoPredictionDialog::VarInvertSelection() {
  QItemSelection selection;
  for (int i = 0; i < ui.listView_5->model()->rowCount(); i++) {
    if (ui.listView_5->selectionModel()->isSelected(
            ui.listView_5->model()->index(i, 0)) == true) {
      continue;
    } else {
      QModelIndex topLeft = ui.listView_5->model()->index(i, 0);
      QModelIndex bottomRight = ui.listView_5->model()->index(i, 0);
      selection << QItemSelectionRange(topLeft, bottomRight);
    }
  }
  ui.listView_5->selectionModel()->select(selection,
                                          QItemSelectionModel::ClearAndSelect);
}

void DoPredictionDialog::VarSelectAll() { ui.listView_5->selectAll(); }

void DoPredictionDialog::ObjUnselectAll() {
  ui.listView_4->selectionModel()->clear();
}

void DoPredictionDialog::ObjSelectBy() {
  if (ui.objSelectByLabel->currentIndex() > 0) {
    int labelindex = ui.objSelectByLabel->currentIndex() - 1;
    QItemSelection selection;
    for (int i = 0; i < ui.listView_4->model()->rowCount(); i++) {
      if (projects_->value(selectedproject_)
              ->getObjectLabels()[labelindex]
              .objects.contains(ui.listView_4->model()
                                    ->index(i, 0)
                                    .data(Qt::DisplayRole)
                                    .toString()) == true) {
        QModelIndex topLeft = ui.listView_4->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_4->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      } else {
        continue;
      }
    }
    ui.listView_4->selectionModel()->select(
        selection, QItemSelectionModel::ClearAndSelect);
    ui.objSelectByLabel->setCurrentIndex(0);
  } else {
    ui.objSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void DoPredictionDialog::ObjInvertSelection() {
  QItemSelectionModel *selectionModel = ui.listView_4->selectionModel();
  QAbstractItemModel *model = ui.listView_4->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount() - 1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

void DoPredictionDialog::ObjSelectAll() { ui.listView_4->selectAll(); }

void DoPredictionDialog::EnableOKButton() {
  if (type == PCA_ || type == CPCA_ || type == LDA_) {
    if (ui.listView_4->selectionModel()->selectedRows(0).size() > 0) {
      ui.okButton->setEnabled(true);
    } else {
      ui.okButton->setEnabled(false);
    }
  } else {
    if (ui.listView_4->selectionModel()->selectedRows(0).size() > 0) {
      if (ui.YvariableGroupBox->isChecked() == true) {
        if (ui.listView_5->selectionModel()->selectedRows(0).size() > 0) {
          ui.okButton->setEnabled(true);
        } else {
          ui.okButton->setEnabled(false);
        }
      } else {
        ui.okButton->setEnabled(true);
      }
    } else {
      ui.okButton->setEnabled(false);
    }
  }
}

void DoPredictionDialog::setselectedYData(QModelIndex current) {
  if (current.isValid()) {
    selectedydata_ = current.row();
  }
}

void DoPredictionDialog::setselectedData(QModelIndex current) {
  if (current.isValid()) {
    selecteddata_ = current.row();
  }
}

void DoPredictionDialog::setselectedModel(QModelIndex current) {
  if (current.isValid())
    selectedmodel_ = mids[current.row()];
}

void DoPredictionDialog::setProject(QModelIndex current) {
  if (current.isValid()) {
    // set project
    selectedproject_ = pids[current.row()];

    // ui.objSelectByLabel->clear();
    ui.objSelectByLabel->addItem("Select by label...");
    for (int i = 0;
         i < projects_->value(selectedproject_)->getObjectLabels().size();
         i++) {
      ui.objSelectByLabel->addItem(
          projects_->value(selectedproject_)->getObjectLabels()[i].name);
    }

    // ui.yvarSelectByLabel->clear();
    ui.yvarSelectByLabel->addItem("Select by label...");
    for (int i = 0;
         i < projects_->value(selectedproject_)->getVariableLabels().size();
         i++) {
      ui.yvarSelectByLabel->addItem(
          projects_->value(selectedproject_)->getVariableLabels()[i].name);
    }

    // create the value for the second tab
    tab2->clear();
    tab3->clear();
    mids.clear();

    if (type == PCA_) {
      for (int i = 0; i < projects_->value(selectedproject_)->PCACount(); i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getPCAModelAt(i)->getName()));
        mids.append(
            projects_->value(selectedproject_)->getPCAModelAt(i)->getModelID());
        tab2->appendRow(row);
      }

      for (int i = 0; i < projects_->value(selectedproject_)->MatrixCount();
           i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getMatrix(i)->getName()));
        tab3->appendRow(row);
      }
    } else if (type == CPCA_) {
      for (int i = 0; i < projects_->value(selectedproject_)->CPCACount();
           i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getCPCAModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)
                        ->getCPCAModelAt(i)
                        ->getModelID());
        tab2->appendRow(row);
      }

      for (int i = 0; i < projects_->value(selectedproject_)->MatrixCount();
           i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getMatrix(i)->getName()));
        tab3->appendRow(row);
      }
    } else if (type == PLS_ || type == PLS_DA_) { // PLS
      for (int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
        mids.append(
            projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
        tab2->appendRow(row);
      }

      for (int i = 0; i < projects_->value(selectedproject_)->MatrixCount();
           i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getMatrix(i)->getName()));
        tab3->appendRow(row);
      }
    } else if (type == MLR_) { // MLR
      for (int i = 0; i < projects_->value(selectedproject_)->MLRCount(); i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getMLRModelAt(i)->getName()));
        mids.append(
            projects_->value(selectedproject_)->getMLRModelAt(i)->getModelID());
        tab2->appendRow(row);
      }

      for (int i = 0; i < projects_->value(selectedproject_)->MatrixCount();
           i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getMatrix(i)->getName()));
        tab3->appendRow(row);
      }
    } else if (type == LDA_) { // LDA
      for (int i = 0; i < projects_->value(selectedproject_)->LDACount(); i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getLDAModelAt(i)->getName()));
        mids.append(
            projects_->value(selectedproject_)->getLDAModelAt(i)->getModelID());
        tab2->appendRow(row);
      }

      for (int i = 0; i < projects_->value(selectedproject_)->MatrixCount();
           i++) {
        QList<QStandardItem *> row;
        row.append(new QStandardItem(
            projects_->value(selectedproject_)->getMatrix(i)->getName()));
        tab3->appendRow(row);
      }
    }
  }
}

void DoPredictionDialog::next() {
  if (state == 0 && selecteddata_ != -1 && selectedproject_ != -1 &&
      !ui.predname->text().isEmpty()) {
    state++;
    ui.prevButton->setEnabled(true);
    ui.nextButton->setEnabled(false);

    ui.objectGroupBox->show();
    ui.prednameFrame->hide();
    ui.projectGroupBox->hide();
    ui.dataGroupBox->hide();
    ui.modelGroupBox->hide();

    // Generate Object listview and Variable listview
    tab4->clear();
    tab5->clear();
    if (type == PCA_ || type == CPCA_ || type == PLS_ || type == PLS_DA_ ||
       type == MLR_ || type == LDA_) {
      for (int i = 0; i < projects_->value(selectedproject_)
                              ->getMatrix(selecteddata_)
                              ->getObjName()
                              .size();
           i++) {
        QList<QStandardItem *> oname;
        oname.append(new QStandardItem(projects_->value(selectedproject_)
                                           ->getMatrix(selecteddata_)
                                           ->getObjName()[i]));
        tab4->appendRow(oname);
      }

      if (type == PLS_ || type == MLR_) {
        ui.YvariableGroupBox->show();
        for (int i = 1; i < projects_->value(selectedproject_)
                                ->getMatrix(selecteddata_)
                                ->getVarName()
                                .size();
             i++) {
          QList<QStandardItem *> vname;
          vname.append(new QStandardItem(projects_->value(selectedproject_)
                                             ->getMatrix(selecteddata_)
                                             ->getVarName()[i]));
          tab5->appendRow(vname);
        }
      }
    } else {
      ui.YvariableGroupBox->hide();
    }
    adjustSize();
  } else {
    QMessageBox::warning(
        this, tr("Warning!"),
        tr("No Project, data, model selected or model name specified!\nPlease "
           "select project and data and check the model name.\n"),
        QMessageBox::Close);
    return;
  }
}

void DoPredictionDialog::previous() {
  if (state == 1) {
    state--;
    ui.prevButton->setEnabled(false);
    ui.nextButton->setEnabled(true);
    ui.okButton->setEnabled(false);
    ui.YvariableGroupBox->hide();
    ui.objectGroupBox->hide();
    ui.modelGroupBox->show();
    ui.projectGroupBox->show();
    ui.dataGroupBox->show();
    ui.prednameFrame->show();

    adjustSize();
  } else {
    adjustSize();
    return;
  }
}

void DoPredictionDialog::OK() {

  if (selectedproject_ == -1 || selectedmodel_ == -1 || selecteddata_ == -1 ||
      ui.predname->text().isEmpty()) {
    reject();
  } else {
    for (int i = 0; i < ui.listView_4->model()->rowCount(); i++) {
      if (ui.listView_4->selectionModel()->isSelected(
              ui.listView_4->model()->index(i, 0)) == true) {
        objsel.append(ui.listView_4->model()
                          ->index(i, 0)
                          .data(Qt::DisplayRole)
                          .toString());
      } else {
        continue;
      }
    }

    if (type == PLS_ || type == MLR_) {
      if (ui.YvariableGroupBox->isChecked() == true) {
        for (int i = 0; i < ui.listView_5->model()->rowCount(); i++) {
          if (ui.listView_5->selectionModel()->isSelected(
                  ui.listView_5->model()->index(i, 0)) == true) {
            yvarsel.append(ui.listView_5->model()
                               ->index(i, 0)
                               .data(Qt::DisplayRole)
                               .toString());
          } else {
            continue;
          }
        }
      }
    }
    compute_ = true;
    accept();
  }
}

DoPredictionDialog::DoPredictionDialog(PROJECTS *projects, int type_) {
  ui.setupUi(this);

  type = type_;

  projects_ = projects;

  state = 0;

  ui.YvariableGroupBox->hide();
  ui.objectGroupBox->hide();
  ui.prevButton->setEnabled(false);
  ui.okButton->setEnabled(false);

  if (type == PCA_) {
    setWindowTitle("Compute PCA Prediction");
  } else if (type == CPCA_) {
    setWindowTitle("Compute CPCA Prediction");
  } else if (type == PLS_ || type == PLS_DA_) {
    setWindowTitle("Compute PLS Prediction");
  } else if (type == MLR_) {
    setWindowTitle("Compute MLR Prediction");
  } else if (type == LDA_) {
    setWindowTitle("Compute LDA Prediction");
    ui.listView_5->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.yvarInvertSelectionButton->hide();
    ui.yvarSelectAllButton->hide();
    ui.yvarSelectByLabel->hide();
    ui.yvarUnselectButton->hide();
  }

  selectedmodel_ = selecteddata_ = -1;

  compute_ = false;

  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();
  tab3 = new QStandardItemModel();
  tab4 = new QStandardItemModel();
  tab5 = new QStandardItemModel();

  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.listView_3->setModel(tab3);
  ui.listView_4->setModel(tab4);
  ui.listView_5->setModel(tab5);

  // Fill the table with data
  QList<QStandardItem *> projectsname;
  for (int i = 0; i < projects_->keys().size(); i++) {
    int pid = projects_->keys()[i];
    projectsname.append(
        new QStandardItem(projects->value(pid)->getProjectName()));
    pids.append(pid);
  }
  tab1->appendColumn(projectsname);

  connect(ui.nextButton, SIGNAL(clicked(bool)), SLOT(next()));
  connect(ui.prevButton, SIGNAL(clicked(bool)), SLOT(previous()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(OK()));

  connect(ui.listView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setProject(QModelIndex)));
  connect(ui.listView_2->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setselectedModel(QModelIndex)));
  connect(ui.listView_3->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setselectedData(QModelIndex)));

  connect(ui.listView_4->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          SLOT(EnableOKButton()));
  connect(ui.listView_5->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          SLOT(EnableOKButton()));
  connect(ui.YvariableGroupBox, SIGNAL(clicked(bool)), SLOT(EnableOKButton()));

  connect(ui.objSelectAllButton, SIGNAL(clicked(bool)), SLOT(ObjSelectAll()));
  connect(ui.objInvertSelectionButton, SIGNAL(clicked(bool)),
          SLOT(ObjInvertSelection()));
  connect(ui.objUnselectButton, SIGNAL(clicked(bool)), SLOT(ObjUnselectAll()));
  connect(ui.objSelectByLabel, SIGNAL(currentIndexChanged(int)),
          SLOT(ObjSelectBy()));

  connect(ui.yvarSelectAllButton, SIGNAL(clicked(bool)), SLOT(VarSelectAll()));
  connect(ui.yvarInvertSelectionButton, SIGNAL(clicked(bool)),
          SLOT(VarInvertSelection()));
  connect(ui.yvarUnselectButton, SIGNAL(clicked(bool)), SLOT(VarUnselectAll()));
  connect(ui.yvarSelectByLabel, SIGNAL(currentIndexChanged(int)),
          SLOT(VarSelectBy()));

  adjustSize();
}

DoPredictionDialog::~DoPredictionDialog() {
  delete tab1;
  delete tab2;
  delete tab3;
  delete tab4;
  delete tab5;
}
