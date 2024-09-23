#include "VariablePlotDialog.h"
#include <QMessageBox>

void VariablePlotDialog::GenDataViewAndLabelView(QModelIndex current) {
  if (current.isValid() && current.row() < projects->keys().size()) {
    // set project
    pid = pids[current.row()];

    tab2->clear();

    QList<QStandardItem *> dataname;
    for (int i = 0; i < projects->value(pid)->MatrixCount(); i++) {
      dataname.clear();
      dataname.append(
          new QStandardItem(projects->value(pid)->getMatrix(i)->getName()));
      dataname.append(new QStandardItem(QString("Matrix")));
      tab2->appendRow(dataname);
    }

    for (int i = 0; i < projects->value(pid)->ArrayCount(); i++) {
      dataname.clear();
      dataname.append(
          new QStandardItem(projects->value(pid)->getArray(i)->getName()));
      dataname.append(new QStandardItem(QString("Array")));
      tab2->appendRow(dataname);
    }

    tab3->clear();

    QList<QStandardItem *> labelaname;
    labelaname.append(new QStandardItem("All Objects"));
    tab3->appendRow(labelaname);

    for (int i = 0; i < projects->value(pid)->getObjectLabels().size(); i++) {
      labelaname.clear();
      labelaname.append(
          new QStandardItem(projects->value(pid)->getObjectLabels()[i].name));
      tab3->appendRow(labelaname);
    }
    // ui.okButton->setEnabled(EnableOKButton());
  } else {
    // ui.okButton->setEnabled(EnableOKButton());
    return;
  }
}

void VariablePlotDialog::GenVariableViewAndSetHash() {
  bool status = true;

  QModelIndexList indexes = ui.dataView->selectionModel()->selectedIndexes();

  if (pid > -1 && projects->keys().contains(pid) == true &&
      indexes.size() > 0) {
    hash.clear();
    QModelIndex current;
    int nmx = 0;
    Q_FOREACH (current, indexes) {
      if (ui.dataView->model()
              ->index(current.row(), 1)
              .data(Qt::DisplayRole)
              .toString()
              .compare("Matrix") == 0) {
        hash.append(projects->value(pid)->getMatrix(current.row())->getHash());
        nmx++;
      } else
        continue;
    }

    // check if the number of variable differ betweeen more matrix/array differ;
    if (nmx > 0) { // are matrix
      vtype = MATRIXDATA;
      auto ncol = projects->value(pid)->getMatrix(hash[0])->Matrix()->col;
      for (int i = 1; i < hash.size(); i++) {
        if (projects->value(pid)->getMatrix(hash[i])->Matrix()->col != ncol) {
          QMessageBox::warning(this, tr("Variable Distribution Error"),
                               tr("Data differ in number of columns."),
                               QMessageBox::Ok);
          status = false;
          break;
        } else {
          continue;
        }
      }

      if (status == false) {
        ui.okButton->setEnabled(status);
      } else {
        // then create the variable list!!
        if (tab5) {
          tab5->clear();
        }
        tab4->clear();

        for (int i = 1;
             i < projects->value(pid)->getMatrix(hash[0])->getVarName().size();
             i++) { // First item is aways the name of the object that we do not
                    // whant to plot...
                    /*
                    if(projects->value(pid)->getMatrix(hash[0])->getVarName()[i].compare(firstcol_name)
                    == 0){           continue;
                    }
                    else{*/
          QList<QStandardItem *> varname1, varname2;
          if (tab5) {
            varname1.clear();
            varname1.append(new QStandardItem(QString("%1").arg(
                projects->value(pid)->getMatrix(hash[0])->getVarName()[i])));
            varname2.clear();
            varname2.append(new QStandardItem(QString("%1").arg(
                projects->value(pid)->getMatrix(hash[0])->getVarName()[i])));
            tab4->appendRow(varname1);
            tab5->appendRow(varname2);
          } else {
            varname1.clear();
            varname1.append(new QStandardItem(QString("%1").arg(
                projects->value(pid)->getMatrix(hash[0])->getVarName()[i])));
            tab4->appendRow(varname1);
          }
          /*}*/
        }
        //           ui.okButton->setEnabled(status);
      }
    }
  } else {
    return;
  }
  // ui.okButton->setEnabled(EnableOKButton());
}

void VariablePlotDialog::setVariableID2(QModelIndex current) {
  if (current.isValid()) {
    varid2 = current.row();
    // ui.okButton->setEnabled(EnableOKButton());
  }
}

void VariablePlotDialog::setVariableID1(QModelIndex current) {
  if (current.isValid()) {
    varid1 = current.row();
    // ui.okButton->setEnabled(EnableOKButton());
  }
}

QStringList VariablePlotDialog::getObjLabelSelected() {
  QModelIndexList indexes = ui.objectsView->selectionModel()->selectedRows();
  if (indexes.size() > 0) {
    QModelIndex current;
    Q_FOREACH (current, indexes) {
      labelobjsel.append(
          ui.objectsView->model()->data(current, Qt::DisplayRole).toString());
    }
  }
  return labelobjsel;
}

QString VariablePlotDialog::getVariableName1() {
  return ui.variableView->model()
      ->data(ui.variableView->selectionModel()->currentIndex(), Qt::DisplayRole)
      .toString();
}

QString VariablePlotDialog::getVariableName2() {
  return ui.variableView_2->model()
      ->data(ui.variableView_2->selectionModel()->currentIndex(),
             Qt::DisplayRole)
      .toString();
}

void VariablePlotDialog::getObjectNames() {
  objects.clear();
  QModelIndexList indexes = ui.objectsView->selectionModel()->selectedRows();
  if (indexes.size() > 0) {
    QModelIndex current;
    Q_FOREACH (current, indexes) {
      if (current.row() == 0) { // append all objects
        QStringList obj;
        if (hash.size() > 0) {
          for (int i = 0; i < hash.size(); i++) {
            for (int j = 0;
                 j <
                 projects->value(pid)->getMatrix(hash[i])->getObjName().size();
                 j++) {
              obj.append(
                  projects->value(pid)->getMatrix(hash[i])->getObjName()[j]);
            }
          }
        } else {
          for (int i = 0; i < hash.size(); i++) {
            for (int j = 0;
                 j <
                 projects->value(pid)->getArray(hash[i])->getObjName().size();
                 j++) {
              obj.append(
                  projects->value(pid)->getArray(hash[i])->getObjName()[j]);
            }
          }
        }
        objects.append(obj);
      } else {
        objects.append(
            projects->value(pid)->getObjectLabels()[current.row() - 1].objects);
      }
    }
  }
  // ui.okButton->setEnabled(EnableOKButton());
}

bool VariablePlotDialog::checkOK() {
  if (projects->size() == 0) {
    QMessageBox::warning(this, tr("Warning!"), tr("No Project found!\n"),
                         QMessageBox::Close);
    return false;
  }

  if (projects->keys().contains(pid) == false) {
    QMessageBox::warning(this, tr("Warning!"), tr("No Project found!\n"),
                         QMessageBox::Close);
    return false;
  }

  int nobj = 0;
  for (int i = 0; i < objects.size(); i++) {
    nobj += objects[i].size();
  }

  if (nobj == 0) {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("The selected object lists are empty!\n"),
                         QMessageBox::Close);
    return false;
  }

  if (hash.size() == 0) {
    QMessageBox::warning(this, tr("Warning!"), tr("No matrix Selected!\n"),
                         QMessageBox::Close);
    return false;
  }

  if (windowtype == VariableDistribution && varid1 == -1) {
    QMessageBox::warning(this, tr("Warning!"), tr("No variable selected!\n"),
                         QMessageBox::Close);
    return false;
  } else if (windowtype == VariableVSVariable && varid1 == -1 && varid2 == -1) {
    QMessageBox::warning(this, tr("Warning!"), tr("No variable selected!\n"),
                         QMessageBox::Close);
    return false;
  }

  return true;
}

bool VariablePlotDialog::EnableOKButton() {
  if (projects->size() > 0) {
    if (projects->keys().contains(pid) == true &&
        (vtype == MATRIXDATA || vtype == ARRAYDATA) && objects.size() > 0 &&
        objects.first().size() > 0 && hash.size() > 0) {
      if (windowtype == VariableDistribution && varid1 > -1) {
        return true;
      } else if (windowtype == VariableVSVariable && varid1 > -1 &&
                 varid2 > -1) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
}

void VariablePlotDialog::Cancel() { reject(); }

void VariablePlotDialog::OK() {
  if (checkOK() == true) {
    if (windowtype == VariableDistribution && varid1 > -1) {
      int nhashok = 0;
      for (int i = 0; i < hash.size(); i++) {
        if (varid1 <
            (int)projects->value(pid)->getMatrix(hash[i])->Matrix()->col) {
          nhashok++;
        } else {
          QMessageBox::warning(
              this, tr("Warning!"),
              tr("Please the following matrix %1 present different variable "
                 "types. Please select data with the same variables.\n"),
              QMessageBox::Close);
          return;
        }
      }

      if (nhashok == hash.size() &&
          ui.variableView->selectionModel()->selectedIndexes().size() > 0) {
        accept();
      } else {
        return;
      }
    } else if (windowtype == VariableVSVariable && hash.size() > 0) {
      int nhashok = 0;
      for (int i = 0; i < hash.size(); i++) {
        if (varid1 <
                (int)projects->value(pid)->getMatrix(hash[i])->Matrix()->col &&
            varid2 <
                (int)projects->value(pid)->getMatrix(hash[i])->Matrix()->col) {
          nhashok++;
        } else {
          QMessageBox::warning(
              this, tr("Warning!"),
              tr("Please the following matrix %1 present different variable "
                 "types. Please select data with the same variables.\n"),
              QMessageBox::Close);
          return;
        }
      }

      if (nhashok == hash.size() &&
          ui.variableView->selectionModel()->selectedIndexes().size() > 0 &&
          ui.variableView_2->selectionModel()->selectedIndexes().size() > 0) {
        accept();
      } else {
        return;
      }
    } else {
      return;
    }
  } else {
    //     QMessageBox::warning(this, tr("Warning!"), tr("Please select a
    //     project.\n"), QMessageBox::Close);
    return;
  }
}

VariablePlotDialog::VariablePlotDialog(PROJECTS *projects_, int windowtype_)
    : QDialog() {
  ui.setupUi(this);

  projects = projects_;

  windowtype = windowtype_;

  if (projects->size() > 0)
    pid = 0;
  else
    pid = -1;

  vtype = varid1 = varid2 = -1;

  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();
  tab3 = new QStandardItemModel();
  tab4 = new QStandardItemModel();

  ui.projectView->setModel(tab1);
  ui.dataView->setModel(tab2);
  ui.objectsView->setModel(tab3);
  ui.variableView->setModel(tab4);

  if (windowtype == 0) {
    ui.variablegroupBox_2->hide();
    tab5 = 0;
  } else {
    tab5 = new QStandardItemModel();
    ui.variableView_2->setModel(tab5);
  }

  resize(minimumSize());

  // Fill the table with data
  QList<QStandardItem *> projectsname;
  for (int i = 0; i < projects_->keys().size(); i++) {
    projectsname.append(new QStandardItem(
        projects_->value(projects_->keys()[i])->getProjectName()));
    pids.append(projects_->keys()[i]);
  }
  tab1->appendColumn(projectsname);

  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.projectView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(GenDataViewAndLabelView(QModelIndex)));
  connect(ui.dataView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          SLOT(GenVariableViewAndSetHash()));

  connect(ui.objectsView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          SLOT(getObjectNames()));

  if (windowtype == 0) {
    connect(ui.variableView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            SLOT(setVariableID1(QModelIndex)));
  } else {
    connect(ui.variableView->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            SLOT(setVariableID1(QModelIndex)));
    connect(ui.variableView_2->selectionModel(),
            SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            SLOT(setVariableID2(QModelIndex)));
  }
  // ui.okButton->setEnabled(EnableOKButton());
}

VariablePlotDialog::~VariablePlotDialog() {
  delete tab1;
  delete tab2;
  delete tab3;

  if (tab5) {
    delete tab4;
    delete tab5;
  } else {
    delete tab4;
  }
}
