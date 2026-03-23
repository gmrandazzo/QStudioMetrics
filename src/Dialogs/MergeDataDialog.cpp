/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Dialog for mergedata operations.
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

#include "MergeDataDialog.h"

// //
/*
 * Merge along rows
 * This means that the matrix do not share same column
 * but share same object names (firstcol_name)
 */

#include <QHash>

void MergeDataDialog::MergeDialogPrepare() {
  ui.okButton->hide();
  ui.cancelButton->hide();
  ui.progressBar->show();
  ui.progressBar->setRange(0, 100);
  ui.progressBar->setValue(0);
  ui.listView->setSelectionMode(QAbstractItemView::NoSelection);
  ui.listView_2->setSelectionMode(QAbstractItemView::NoSelection);
  ui.dataname->setReadOnly(true);
  ui.mergeallcol->hide();
  ui.mergematchcol->hide();
}

void MergeDataDialog::MergeType0() {
  // Merge along rows (different variables, shared objects)
  QHash<QString, QList<QPair<int, int>>> objmap;
  QStringList varnames;
  
  DATA* project = projects->value(pid);
  for (int i = 0; i < mxids.size(); ++i) {
    MATRIX* currentMx = project->getMatrix(mxids[i]);
    const QStringList& objNames = currentMx->getObjName();
    const QStringList& varNames = currentMx->getVarName();

    for (int j = 0; j < objNames.size(); ++j) {
      objmap[objNames[j]].append({i, j});
    }
    
    // Add variables, excluding the first column (Object Names)
    for (int j = 1; j < varNames.size(); ++j) {
      varnames.append(varNames[j]);
    }
  }

  // Filter objects that don't exist in all matrices
  QStringList intersectionKeys;
  for (auto it = objmap.begin(); it != objmap.end(); ++it) {
    if (it.value().size() == mxids.size()) {
      intersectionKeys.append(it.key());
    }
  }
  
  if (intersectionKeys.isEmpty()) return;

  ui.progressBar->setMaximum(intersectionKeys.size());
  mx->MatrixResize(intersectionKeys.size(), varnames.size());
  QStringList finalObjNames;
  finalObjNames.reserve(intersectionKeys.size());

  for (int i = 0; i < intersectionKeys.size(); ++i) {
    if (i % 100 == 0) QApplication::processEvents();
    ui.progressBar->setValue(i + 1);
    
    const QString& key = intersectionKeys[i];
    finalObjNames.append(key);
    
    int current_col = 0;
    const auto& occurrences = objmap[key];
    for (const auto& pair : occurrences) {
      int mxIdx = pair.first;
      int rowIdx = pair.second;
      matrix* srcMx = project->getMatrix(mxids[mxIdx])->Matrix();
      
      for (uint j = 0; j < srcMx->col; ++j) {
        setMatrixValue(mx->Matrix(), i, current_col++, getMatrixValue(srcMx, rowIdx, j));
      }
    }
  }

  mx->getObjName() = finalObjNames;
  QStringList finalVarNames;
  finalVarNames.reserve(varnames.size() + 1);
  finalVarNames.append(firstcol_name);
  finalVarNames.append(varnames);
  mx->getVarName() = finalVarNames;
  mx->setName(ui.dataname->text());
}

void MergeDataDialog::MergeType1() {
  // Merge along columns (same variables, shared columns)
  QStringList allObjNames;
  QStringList candidateVarnames;
  DATA* project = projects->value(pid);

  for (int mId : mxids) {
    MATRIX* m = project->getMatrix(mId);
    allObjNames << m->getObjName();
    candidateVarnames << m->getVarName();
  }
  candidateVarnames.removeDuplicates();
  candidateVarnames.removeAll(firstcol_name);

  struct VarMapping {
    QString name;
    QVector<int> colIndices;
  };
  QVector<VarMapping> validMappings;

  for (const QString& vName : candidateVarnames) {
    QVector<int> indices;
    bool foundInAll = true;
    for (int mId : mxids) {
      int idx = project->getMatrix(mId)->getVarName().indexOf(vName);
      if (idx > 0) { // Found and not the first column
        indices.append(idx - 1);
      } else {
        foundInAll = false;
        break;
      }
    }
    if (foundInAll) {
      validMappings.append({vName, indices});
    }
  }

  if (validMappings.isEmpty()) return;

  mx->MatrixResize(allObjNames.size(), validMappings.size());
  int currentRow = 0;
  for (int k = 0; k < mxids.size(); ++k) {
    matrix* src = project->getMatrix(mxids[k])->Matrix();
    for (uint i = 0; i < src->row; ++i) {
      if (currentRow % 100 == 0) QApplication::processEvents();
      for (int j = 0; j < validMappings.size(); ++j) {
        setMatrixValue(mx->Matrix(), currentRow, j, getMatrixValue(src, i, validMappings[j].colIndices[k]));
      }
      currentRow++;
    }
  }

  mx->getObjName() = allObjNames;
  QStringList finalVarNames;
  finalVarNames.reserve(validMappings.size() + 1);
  finalVarNames.append(firstcol_name);
  for (const auto& mapping : validMappings) finalVarNames.append(mapping.name);
  mx->getVarName() = finalVarNames;
  mx->setName(ui.dataname->text());
}

void MergeDataDialog::OK() {
  MergeDialogPrepare();
  if (ui.mergematchcol->isChecked()) {
    MergeType1();
  } else {
    MergeType0();
  }
  accept();
}

void MergeDataDialog::genListView() {
  if (pid != -1) {
    tab2->clear();
    DATA* project = projects->value(pid);
    for (int i = 0; i < project->MatrixCount(); ++i) {
      tab2->appendRow(new QStandardItem(project->getMatrix(i)->getName()));
    }
  }
}

void MergeDataDialog::set_unset_MatrixID(QModelIndex current) {
  if (current.isValid()) {
    int row = current.row();
    int indx = mxids.indexOf(row);
    if (indx > -1) {
      mxids.removeAt(indx);
    } else {
      mxids.append(row);
    }
    EnableOKButton();
  }
}

void MergeDataDialog::setProjectID(QModelIndex current) {
  if (current.isValid()) {
    pid = pids[current.row()];
    mxids.clear(); // Clear matrix selection when project changes
    genListView();
    EnableOKButton();
  } else {
    pid = -1;
  }
}

void MergeDataDialog::EnableOKButton() {
  ui.okButton->setEnabled(mxids.size() > 1 && !ui.dataname->text().trimmed().isEmpty());
}

MergeDataDialog::MergeDataDialog(PROJECTS *projects_) : QDialog() {
  ui.setupUi(this);
  setWindowTitle("Merge Datasets");
  projects = projects_;
  mx = new MATRIX();
  pid = -1;
  
  tab1 = new QStandardItemModel(this);
  tab2 = new QStandardItemModel(this);
  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.progressBar->hide();

  for (int key : projects->keys()) {
    tab1->appendRow(new QStandardItem(projects->value(key)->getProjectName()));
    pids.append(key);
  }

  connect(ui.listView->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &MergeDataDialog::setProjectID);
  connect(ui.listView_2->selectionModel(), &QItemSelectionModel::currentChanged,
          this, &MergeDataDialog::set_unset_MatrixID);
  connect(ui.cancelButton, &QPushButton::clicked, this, &QDialog::reject);
  connect(ui.dataname, &QLineEdit::textChanged, this, &MergeDataDialog::EnableOKButton);
  connect(ui.okButton, &QPushButton::clicked, this, &MergeDataDialog::OK);
  
  EnableOKButton();
}

MergeDataDialog::~MergeDataDialog() {
  delete mx;
}
