#include "VariableSelectorDialog.h"

#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <QStringListModel>

QStringList VariableSelectorDialog::getSelectedVariables() {
  QStringList varselected;
  for (int i = 0; i < ui.varnamelistView->model()->rowCount(); i++) {
    if (ui.varnamelistView->selectionModel()->isSelected(
            ui.varnamelistView->model()->index(i, 0)) == true) {
      varselected.append(ui.varnamelistView->model()
                             ->index(i, 0)
                             .data(Qt::DisplayRole)
                             .toString());
    } else {
      continue;
    }
  }
  return varselected;
}

void VariableSelectorDialog::DoSelection(QItemSelection &selection) {
  if (ui.selectbyregexpButton->isChecked()) {
    for (int i = 0; i < ui.varnamelistView->model()->rowCount(); i++) {
      if (ui.varnamelistView->model()
              ->index(i, 0)
              .data(Qt::DisplayRole)
              .toString()
              .contains(ui.regexp->text(), Qt::CaseSensitive) == true) {
        QModelIndex topLeft = ui.varnamelistView->model()->index(i, 0);
        QModelIndex bottomRight = ui.varnamelistView->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      } else {
        continue;
      }
    }
  } else if (ui.selectbyvarlabelButton->isChecked()) {
    int index = ui.varlabelslst->currentIndex();
    if (index > -1) {
      for (int i = 0; i < ui.varnamelistView->model()->rowCount(); i++) {
        bool select = false;
        for (int j = 0; j < (*varlabels)[index].objects.size(); j++) {
          if (ui.varnamelistView->model()
                  ->index(i, 0)
                  .data(Qt::DisplayRole)
                  .toString()
                  .contains((*varlabels)[index].objects[j],
                            Qt::CaseSensitive) == true) {
            select = true;
            break;
          } else {
            continue;
          }
        }

        if (select == true) {
          QModelIndex topLeft = ui.varnamelistView->model()->index(i, 0);
          QModelIndex bottomRight = ui.varnamelistView->model()->index(i, 0);
          selection << QItemSelectionRange(topLeft, bottomRight);
        } else {
          continue;
        }
      }
    }
  } else {

    QStringList objlst = getObjList();

    int var = getVariable();
    int objselectiontype;
    if (ui.valueinlimits->isChecked())
      objselectiontype = 0;
    else
      objselectiontype = 1;

    QStringList selvar;

    double limitinf = ui.limitinf->text().toDouble();
    double limitsup = ui.limitsup->text().toDouble();

    double min = ui.min->text().toDouble();
    double max = ui.max->text().toDouble();

    if (m.size() > 0 && id.size() > 0) {
      for (int i = 0; i < objlst.size(); i++) {
        for (int k = 0; k < id.size(); k++) {
          auto indx = m[id[k]]->getObjName().indexOf(objlst[i]);
          if (indx != -1) {
            if (var == -1) { // All variables
              for (int j = 0; j < (int)m[id[k]]->Matrix()->col; j++) {
                if (objselectiontype == 0) {
                  if (getMatrixValue(m[id[k]]->Matrix(), indx, j) < limitinf ||
                      getMatrixValue(m[id[k]]->Matrix(), indx, j) > limitsup) {
                    selvar.append(m[id[k]]->getVarName()[j + 1]);
                  } else {
                    continue;
                  }
                } else {
                  if (getMatrixValue(m[id[k]]->Matrix(), indx, j) > min &&
                      getMatrixValue(m[id[k]]->Matrix(), indx, j) < max) {
                    selvar.append(m[id[k]]->getVarName()[j + 1]);
                  } else {
                    continue;
                  }
                }
              }
            } else { // Specified variable
              if (objselectiontype == 0) {
                if (getMatrixValue(m[id[k]]->Matrix(), indx, var) < limitinf ||
                    getMatrixValue(m[id[k]]->Matrix(), indx, var) > limitsup) {
                  selvar.append(m[id[k]]->getVarName()[var + 1]);
                } else {
                  continue;
                }
              } else {
                if (getMatrixValue(m[id[k]]->Matrix(), indx, var) > min &&
                    getMatrixValue(m[id[k]]->Matrix(), indx, var) < max) {
                  selvar.append(m[id[k]]->getVarName()[var + 1]);
                } else {
                  continue;
                }
              }
            }
          } else {
            continue;
          }
        }
      }

      for (int i = 0; i < ui.varnamelistView->model()->rowCount(); i++) {
        if (selvar.indexOf(ui.varnamelistView->model()
                               ->index(i, 0)
                               .data(Qt::DisplayRole)
                               .toString()) > -1) {
          QModelIndex topLeft = ui.varnamelistView->model()->index(i, 0);
          QModelIndex bottomRight = ui.varnamelistView->model()->index(i, 0);
          selection << QItemSelectionRange(topLeft, bottomRight);
        } else {
          continue;
        }
      }
    } else {
      return;
    }
  }
}

void VariableSelectorDialog::UpdateLabelSelection() {
  ui.selectedlabel->setText(
      QString("%1 / %2")
          .arg(ui.varnamelistView->selectionModel()->selectedRows().size())
          .arg(ui.varnamelistView->model()->rowCount()));
}

int VariableSelectorDialog::getVariable() {
  if (ui.variablelst->currentIndex() == 0) {
    return -1;
  } else {
    return ui.variablelst->currentIndex() - 1;
  }
}

QStringList VariableSelectorDialog::getObjList() {
  QStringList selected;
  for (int i = 0; i < ui.objectlistView->model()->rowCount(); i++) {
    if (ui.objectlistView->selectionModel()->isSelected(
            ui.objectlistView->model()->index(i, 0)) == true) {
      selected.append(ui.objectlistView->model()
                          ->index(i, 0)
                          .data(Qt::DisplayRole)
                          .toString());
    } else {
      continue;
    }
  }
  return selected;
}

void VariableSelectorDialog::find() {
  for (int i = 0; i < ui.varnamelistView->model()->rowCount(); i++) {
    if (ui.varnamelistView->model()
                ->index(i, 0)
                .data(Qt::DisplayRole)
                .toString()
                .contains(ui.searchline->text(), Qt::CaseSensitive) == true &&
        ui.varnamelistView->selectionModel()->isSelected(
            ui.varnamelistView->model()->index(i, 0)) == false) {
      ui.varnamelistView->scrollTo(ui.varnamelistView->model()->index(i, 0));
      ui.varnamelistView->selectionModel()->select(
          ui.varnamelistView->model()->index(i, 0),
          QItemSelectionModel::Select);
      break;
    } else {
      continue;
    }
  }
}

void VariableSelectorDialog::Select() {
  ui.varnamelistView->selectionModel()->clear();
  QItemSelection selection;
  DoSelection(selection);
  ui.varnamelistView->selectionModel()->select(selection,
                                               QItemSelectionModel::Select);
}

void VariableSelectorDialog::InvertSelection() {
  QItemSelectionModel *selectionModel = ui.varnamelistView->selectionModel();
  QAbstractItemModel *model = ui.varnamelistView->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount() - 1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

void VariableSelectorDialog::AppendSelection() {
  QItemSelection selection;
  DoSelection(selection);
  ui.varnamelistView->selectionModel()->select(selection,
                                               QItemSelectionModel::Select);
}

void VariableSelectorDialog::Unselect() {
  ui.varnamelistView->selectionModel()->clear();
}

void VariableSelectorDialog::OK() { accept(); }

void VariableSelectorDialog::EnableDisable() {
  if (m.size() > 0 && id.size() > 0) {
    if (ui.selectbyvarlabelButton->isChecked()) {
      ui.varlabelslst->setEnabled(true);
      ui.regexp->setEnabled(false);
      ui.objectGroupBox->setEnabled(false);
    } else if (ui.selectbyregexpButton->isChecked()) {
      ui.varlabelslst->setEnabled(false);
      ui.regexp->setEnabled(true);
      ui.objectGroupBox->setEnabled(false);
    } else {
      ui.varlabelslst->setEnabled(false);
      ui.regexp->setEnabled(false);
      ui.objectGroupBox->setEnabled(true);
    }
  } else {
    if (ui.selectbyvarlabelButton->isChecked()) {
      ui.varlabelslst->setEnabled(true);
      ui.regexp->setEnabled(false);
      ui.objectGroupBox->setEnabled(false);
    } else {
      //     else if(ui.selectbyregexpButton->isChecked()){
      ui.varlabelslst->setEnabled(false);
      ui.regexp->setEnabled(true);
      ui.objectGroupBox->setEnabled(false);
    }
  }
}

VariableSelectorDialog::VariableSelectorDialog(QStringList &objnames,
                                               QStringList &variablenames,
                                               QList<MATRIX *> m_,
                                               QList<int> id_,
                                               LABELS *varlabels_) {
  ui.setupUi(this);

  objlabels = 0;
  varlabels = varlabels_;

  ui.label_13->hide();
  ui.objlayerBox->hide();

  for (int i = 0; i < (*varlabels).size(); i++) {
    ui.varlabelslst->addItem((*varlabels)[i].name);
  }

  m = m_;
  id = id_;

  ui.varnamelistView->setModel(new QStringListModel(variablenames));
  ui.objectlistView->setModel(new QStringListModel(objnames));

  ui.variablelst->addItem("All Variables");
  for (int i = 0; i < variablenames.size(); i++) {
    ui.variablelst->addItem(variablenames[i]);
  }

  connect(ui.varnamelistView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          SLOT(UpdateLabelSelection()));
  connect(ui.selectButton, SIGNAL(clicked(bool)), SLOT(Select()));
  connect(ui.invertselectButton, SIGNAL(clicked(bool)),
          SLOT(InvertSelection()));
  connect(ui.appendselectionButton, SIGNAL(clicked(bool)),
          SLOT(AppendSelection()));
  connect(ui.unselectButton, SIGNAL(clicked(bool)), SLOT(Unselect()));
  connect(ui.findButton, SIGNAL(clicked(bool)), SLOT(find()));

  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui.selectbyvarlabelButton, SIGNAL(clicked(bool)),
          SLOT(EnableDisable()));
  connect(ui.selectbyregexpButton, SIGNAL(clicked(bool)),
          SLOT(EnableDisable()));
  connect(ui.selectbyObj, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  EnableDisable();
  UpdateLabelSelection();
}

VariableSelectorDialog::VariableSelectorDialog(QStringList &variablenames,
                                               LABELS *varlabels_) {
  ui.setupUi(this);

  objlabels = 0;
  varlabels = varlabels_;

  ui.label_13->hide();
  ui.objlayerBox->hide();

  for (int i = 0; i < (*varlabels).size(); i++) {
    ui.varlabelslst->addItem((*varlabels)[i].name);
  }

  ui.varnamelistView->setModel(new QStringListModel(variablenames));

  ui.variablelst->addItem("All Variables");
  for (int i = 0; i < variablenames.size(); i++) {
    ui.variablelst->addItem(variablenames[i]);
  }

  ui.selectbyObj->hide();
  ui.objectGroupBox->hide();

  connect(ui.varnamelistView->selectionModel(),
          SIGNAL(selectionChanged(QItemSelection, QItemSelection)),
          SLOT(UpdateLabelSelection()));
  connect(ui.selectButton, SIGNAL(clicked(bool)), SLOT(Select()));
  connect(ui.invertselectButton, SIGNAL(clicked(bool)),
          SLOT(InvertSelection()));
  connect(ui.appendselectionButton, SIGNAL(clicked(bool)),
          SLOT(AppendSelection()));
  connect(ui.unselectButton, SIGNAL(clicked(bool)), SLOT(Unselect()));
  connect(ui.findButton, SIGNAL(clicked(bool)), SLOT(find()));

  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui.selectbyvarlabelButton, SIGNAL(clicked(bool)),
          SLOT(EnableDisable()));
  connect(ui.selectbyregexpButton, SIGNAL(clicked(bool)),
          SLOT(EnableDisable()));
  connect(ui.selectbyObj, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  EnableDisable();
  UpdateLabelSelection();
}
