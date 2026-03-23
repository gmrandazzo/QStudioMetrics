/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for extractdata operations.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "ExtractDataDialog.h"
#include "ObjectSelectorDialog.h"
#include "VariableSelectorDialog.h"
#include "qstudiometricstypes.h"
#include "run.h"
#include <QMessageBox>
#include <QString>

#include <QSet>

void ExtractDataDialog::StartSelectionRun() {
  stoprun = false;
  pdialog.setRange(0, 0); // Indeterminate
  pdialog.setValue(0);
  pdialog.show();
}

void ExtractDataDialog::StopSelectionRun() {
  stoprun = true;
  pdialog.hide();
}

void ExtractDataDialog::setProjectID(QModelIndex current) {
  if (current.isValid()) {
    pid = pids[current.row()];
    genListView();
  } else {
    pid = -1;
  }
}

void ExtractDataDialog::genObjVarView(QModelIndex current) {
  if (current.isValid()) {
    selectedhash_ = hash[current.row()];

    tab3->clear();
    tab4->clear();
    tab3->setHorizontalHeaderLabels(QStringList() << firstcol_name);
    tab4->setHorizontalHeaderLabels(QStringList() << "Variable Names");

    MATRIX* sourceMx = projects_->value(pid)->getMatrix(selectedhash_);
    const QStringList& objNames = sourceMx->getObjName();
    const QStringList& varNames = sourceMx->getVarName();

    for (const QString& name : objNames) {
      tab3->appendRow(new QStandardItem(name));
    }

    for (const QString& name : varNames) {
      if (name != firstcol_name) {
        tab4->appendRow(new QStandardItem(name));
      }
    }
    UpdateSelectedObjectCounter();
    UpdateSelectedVariableCounter();
  }
}

void ExtractDataDialog::genListView() {
  if (pid != -1) {
    tab2->clear();
    hash.clear();
    DATA* project = projects_->value(pid);
    int count = project->MatrixCount();
    if (count > 0) {
      for (int i = 0; i < count; i++) {
        tab2->appendRow(new QStandardItem(project->getMatrix(i)->getName()));
        hash.append(project->getMatrix(i)->getHash());
      }
    }
  }
}

void ExtractDataDialog::VariablesUnselect() {
  ui.tableView_4->selectionModel()->clearSelection();
}

void ExtractDataDialog::VariablesSelectBy() {
  if (pid != -1 && !selectedhash_.isEmpty()) {
    MATRIX* sourceMx = projects_->value(pid)->getMatrix(selectedhash_);
    VariableSelectorDialog vseldialog(sourceMx->getVarName(),
                                      &projects_->value(pid)->getVariableLabels());

    if (vseldialog.exec() == QDialog::Accepted) {
      QStringList varselected = vseldialog.getSelectedVariables();
      QSet<QString> selectedSet(varselected.begin(), varselected.end());
      
      QItemSelection selection;
      QAbstractItemModel* model = ui.tableView_4->model();
      for (int i = 0; i < model->rowCount(); i++) {
        if (selectedSet.contains(model->index(i, 0).data().toString())) {
          selection.select(model->index(i, 0), model->index(i, 0));
        }
      }
      ui.tableView_4->selectionModel()->select(selection, QItemSelectionModel::Select);
    }
  }
}

void ExtractDataDialog::VariablesInvertSelection() {
  QItemSelectionModel *selectionModel = ui.tableView_4->selectionModel();
  QAbstractItemModel *model = ui.tableView_4->model();
  QItemSelection toggleSelection(model->index(0, 0), model->index(model->rowCount() - 1, 0));
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

void ExtractDataDialog::VariablesSelectAll() {
  ui.tableView_4->selectAll();
}

void ExtractDataDialog::ObjectsUnselect() {
  ui.tableView_3->selectionModel()->clearSelection();
}

void ExtractDataDialog::ObjectsSelectBy() {
  if (pid == -1) return;

  ObjectSelectorDialog objseldialog(&projects_->value(pid)->getMATRIXList(),
                                    &projects_->value(pid)->getObjectLabels(),
                                    &projects_->value(pid)->getVariableLabels(),
                                    ObjectSelectorDialog::MenuSelection);
  if (objseldialog.exec() != QDialog::Accepted) return;

  int selectiontype = objseldialog.getSelectionType();
  int metric = objseldialog.getMetric();
  int nobjects = objseldialog.getNumberOfObjects();
  int dataid = objseldialog.getDataID();
  QString label = objseldialog.getLabel();
  QStringList varlist = objseldialog.getVariableList();

  if ((selectiontype == MOSTDESCRIPTIVECOMPOUND || selectiontype == MAXIMUMDISSIMILARITYMAXMIN) &&
      metric != -1 && nobjects != -1 && dataid != -1 && !varlist.isEmpty()) {
    
    MATRIX* sourceMatrix = projects_->value(pid)->getMatrix(dataid);
    matrix *m;
    NewMatrix(&m, sourceMatrix->Matrix()->row, varlist.size());
    QStringList varnames = sourceMatrix->getVarName();
    varnames.removeAll(firstcol_name);

    for (int j = 0; j < varlist.size(); j++) {
      int colindex = varnames.indexOf(varlist[j]);
      if (colindex != -1) {
        for (uint i = 0; i < sourceMatrix->Matrix()->row; i++) {
          setMatrixValue(m, i, j, getMatrixValue(sourceMatrix->Matrix(), i, colindex));
        }
      }
    }

    uivector *selected;
    initUIVector(&selected);
    RUN obj;
    obj.setMatrix(m);
    obj.setUIVector(selected);
    obj.setMetric(metric);
    obj.setNumberOfObject(nobjects);

    StartSelectionRun();
    QFuture<void> future = (selectiontype == MOSTDESCRIPTIVECOMPOUND) ? obj.RunMDCSelection() : obj.RunMaxDisSelection();

    while (!future.isFinished()) {
      if (stoprun) { future.cancel(); break; }
      QApplication::processEvents();
    }

    if (!stoprun) {
      ui.tableView_3->selectionModel()->clearSelection();
      for (uint i = 0; i < selected->size; i++) {
        uint idx = getUIVectorValue(selected, i);
        if (idx < sourceMatrix->Matrix()->row) {
          ui.tableView_3->selectionModel()->select(ui.tableView_3->model()->index(idx, 0),
                                                   QItemSelectionModel::Select | QItemSelectionModel::Rows);
        }
      }
    }
    DelMatrix(&m);
    DelUIVector(&selected);
    StopSelectionRun();

  } else if (selectiontype == RANDOMSELECTION && nobjects != -1 && dataid != -1) {
    MATRIX* sourceMatrix = projects_->value(pid)->getMatrix(dataid);
    uivector *selected;
    initUIVector(&selected);
    RUN obj;
    obj.setUIVector(selected);
    obj.setNumberOfObject(nobjects);
    obj.setNumberMaxOfObject(sourceMatrix->Matrix()->row);

    StartSelectionRun();
    QFuture<void> future = obj.RunRandomSelection();
    while (!future.isFinished()) {
      if (stoprun) { future.cancel(); break; }
      QApplication::processEvents();
    }

    if (!stoprun) {
      ui.tableView_3->selectionModel()->clearSelection();
      for (uint i = 0; i < selected->size; i++) {
        uint idx = getUIVectorValue(selected, i);
        ui.tableView_3->selectionModel()->select(ui.tableView_3->model()->index(idx, 0),
                                                 QItemSelectionModel::Select | QItemSelectionModel::Rows);
      }
    }
    DelUIVector(&selected);
    StopSelectionRun();

  } else if (!label.isEmpty()) {
    const LABELS& objLabels = projects_->value(pid)->getObjectLabels();
    int lindex = -1;
    for (int i = 0; i < objLabels.size(); ++i) {
      if (objLabels[i].name == label) { lindex = i; break; }
    }

    if (lindex > -1) {
      QSet<QString> targetObjects(objLabels[lindex].objects.begin(), objLabels[lindex].objects.end());
      QItemSelectionModel* selModel = ui.tableView_3->selectionModel();
      QAbstractItemModel* model = ui.tableView_3->model();
      selModel->clearSelection();
      
      QItemSelection selection;
      for (int j = 0; j < model->rowCount(); j++) {
        if (targetObjects.contains(model->index(j, 0).data().toString())) {
          selection.select(model->index(j, 0), model->index(j, 0));
        }
      }
      selModel->select(selection, QItemSelectionModel::Select);
    }
  }
}

void ExtractDataDialog::ObjectsInvertSelection() {
  QItemSelectionModel *selectionModel = ui.tableView_3->selectionModel();
  QAbstractItemModel *model = ui.tableView_3->model();
  QItemSelection toggleSelection(model->index(0, 0), model->index(model->rowCount() - 1, 0));
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}

void ExtractDataDialog::ObjectsSelectAll() {
  ui.tableView_3->selectAll();
}

void ExtractDataDialog::UpdateSelectedObjectCounter() {
  ui.nselobj->setText(QString("%1/%2")
      .arg(ui.tableView_3->selectionModel()->selectedRows().size())
      .arg(ui.tableView_3->model()->rowCount()));
}

void ExtractDataDialog::UpdateSelectedVariableCounter() {
  ui.nselvar->setText(QString("%1/%2")
      .arg(ui.tableView_4->selectionModel()->selectedRows().size())
      .arg(ui.tableView_4->model()->rowCount()));
}

void ExtractDataDialog::OK() {
  QModelIndexList objectIndexes = ui.tableView_3->selectionModel()->selectedRows();
  QModelIndexList variableIndexes = ui.tableView_4->selectionModel()->selectedRows();

  if (objectIndexes.isEmpty() || variableIndexes.isEmpty() || ui.dataname->text().isEmpty()) {
    QMessageBox::warning(this, tr("Extract Data Error"),
        tr("Please select data and specify the new data name."), QMessageBox::Ok);
    return;
  }

  MATRIX* sourceMx = projects_->value(pid)->getMatrix(selectedhash_);
  matrix* src = sourceMx->Matrix();
  matrix* dst = mx->Matrix();

  ResizeMatrix(dst, objectIndexes.size(), variableIndexes.size());

  QStringList objnames, varnames;
  objnames.reserve(objectIndexes.size());
  varnames.reserve(variableIndexes.size() + 1);
  varnames.append(firstcol_name);

  // Map variable selection to source column indices
  QVector<int> colMap;
  colMap.reserve(variableIndexes.size());
  for (const QModelIndex& idx : variableIndexes) {
    int vIdx = idx.row();
    colMap.append(vIdx);
    varnames.append(sourceMx->getVarName()[vIdx + 1]);
  }

  for (int r = 0; r < objectIndexes.size(); ++r) {
    int srcRow = objectIndexes[r].row();
    objnames.append(sourceMx->getObjName()[srcRow]);
    for (int c = 0; r == 0 && c < colMap.size(); ++c) { /* Already handled above */ }
    for (int c = 0; c < colMap.size(); ++c) {
      setMatrixValue(dst, r, c, getMatrixValue(src, srcRow, colMap[c]));
    }
  }

  mx->setName(ui.dataname->text());
  mx->getObjName() = objnames;
  mx->getVarName() = varnames;

  accept();
}

ExtractDataDialog::ExtractDataDialog(PROJECTS *projects) : QDialog() {
  ui.setupUi(this);
  setWindowTitle("Extract Sub-dataset");
  mx = new MATRIX();
  stoprun = false;
  projects_ = projects;
  pid = -1;

  tab1 = new QStandardItemModel(this);
  tab2 = new QStandardItemModel(this);
  tab3 = new QStandardItemModel(this);
  tab4 = new QStandardItemModel(this);

  ui.nselobj->setText("0/0");
  ui.nselvar->setText("0/0");

  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.tableView_3->setModel(tab3);
  ui.tableView_4->setModel(tab4);

  QList<int> keys = projects_->keys();
  for (int key : keys) {
    tab1->appendRow(new QStandardItem(projects_->value(key)->getProjectName()));
    pids.append(key);
  }

  connect(ui.cancelButton, &QPushButton::clicked, this, &QDialog::reject);
  connect(ui.okButton, &QPushButton::clicked, this, &ExtractDataDialog::OK);

  connect(ui.listView->selectionModel(), &QItemSelectionModel::currentChanged,
          [this](const QModelIndex &current) { setProjectID(current); });
  connect(ui.listView_2->selectionModel(), &QItemSelectionModel::currentChanged,
          [this](const QModelIndex &current) { genObjVarView(current); });
  
  auto updateCounters = [this]() {
    UpdateSelectedObjectCounter();
    UpdateSelectedVariableCounter();
  };

  connect(ui.tableView_3->selectionModel(), &QItemSelectionModel::selectionChanged, updateCounters);
  connect(ui.tableView_4->selectionModel(), &QItemSelectionModel::selectionChanged, updateCounters);

  connect(ui.objSelectAllButton, &QPushButton::clicked, this, &ExtractDataDialog::ObjectsSelectAll);
  connect(ui.objInvertSelectionButton, &QPushButton::clicked, this, &ExtractDataDialog::ObjectsInvertSelection);
  connect(ui.objSelectByButton, &QPushButton::clicked, this, &ExtractDataDialog::ObjectsSelectBy);
  connect(ui.objUnselectButton, &QPushButton::clicked, this, &ExtractDataDialog::ObjectsUnselect);
  
  connect(ui.varSelectAllButton, &QPushButton::clicked, this, &ExtractDataDialog::VariablesSelectAll);
  connect(ui.varInvertSelectionButton, &QPushButton::clicked, this, &ExtractDataDialog::VariablesInvertSelection);
  connect(ui.varSelectByButton, &QPushButton::clicked, this, &ExtractDataDialog::VariablesSelectBy);
  connect(ui.varUnselectButton, &QPushButton::clicked, this, &ExtractDataDialog::VariablesUnselect);
}

ExtractDataDialog::~ExtractDataDialog() {
  delete mx;
}
