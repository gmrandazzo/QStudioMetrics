#include "ExportTableDialog.h"
#include "data.h"
#include <QFileDialog>
#include <QFileInfo>

#include <QStringListModel>

QString ExportTableDialog::getFileName()
{
  return QString::fromUtf8(ui.filetosave->text().toUtf8());
}

QString ExportTableDialog::getSeparator()
{
  if(ui.separator->currentIndex() == 0){
    return QString("\t");
  }
  else if(ui.separator->currentIndex() == 1){
    return QString(" ");
  }
  else if(ui.separator->currentIndex() == 2){
    return QString(",");
  }
  else if(ui.separator->currentIndex() == 3){
    return QString(";");
  }
  else{
    return ui.separator->currentText();
  }
}

QStringList ExportTableDialog::getSelectedObjects()
{
  QStringList selectedobjects;
  for (int i = 0; i < ui.listView_1->model()->rowCount(); i++){
    if(ui.listView_1->selectionModel()->isSelected(ui.listView_1->model()->index(i, 0)) == true){
      selectedobjects.append(ui.listView_1->model()->index(i, 0).data(Qt::DisplayRole).toString());
    }
    else{
      continue;
    }
  }
  return selectedobjects;
}

QStringList ExportTableDialog::getSelectedVariables()
{
  QStringList selectedvariables;
  for (int i = 0; i < ui.listView_2->model()->rowCount(); i++){
    if(ui.listView_2->selectionModel()->isSelected(ui.listView_2->model()->index(i, 0)) == true){
      selectedvariables.append(ui.listView_2->model()->index(i, 0).data(Qt::DisplayRole).toString());
    }
    else{
      continue;
    }
  }
  return selectedvariables;
}

void ExportTableDialog::SaveAs()
{
  if(path.isEmpty()){
    ui.filetosave->setText(QFileDialog::getSaveFileName(this, "Save Project", getenv("HOME")));
    QFileInfo last(ui.filetosave->text());
    path = QString::fromUtf8(last.absoluteFilePath().toUtf8());
  }
  else{
    ui.filetosave->setText(QFileDialog::getSaveFileName(this, "Save Project", path));
    QFileInfo last(ui.filetosave->text());
    path = QString::fromUtf8(last.absoluteFilePath().toUtf8());
  }
}

void ExportTableDialog::EnableOKButton()
{
  if(!ui.filetosave->text().isEmpty() && ui.listView_1->selectionModel()->selectedRows(0).size() > 0
    && ui.listView_2->selectionModel()->selectedRows(0).size() > 0){
    ui.okButton->setEnabled(true);
  }
  else{
    ui.okButton->setEnabled(false);
  }
}

void ExportTableDialog::ObjSelectAll()
{
  ui.listView_1->selectAll();
  EnableOKButton();
}

void ExportTableDialog::ObjInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_1->selectionModel();
  QAbstractItemModel *model = ui.listView_1->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  EnableOKButton();
}

void ExportTableDialog::ObjSelectBy()
{
  if(ui.objSelectByLabel->currentIndex() > 0){
    int labelindex = ui.objSelectByLabel->currentIndex()-1;

    QAbstractItemModel *m = ui.listView_1->model();
    QItemSelection selection;
    for(int i = 0; i < m->rowCount(); i++){
      if(objectlabels[labelindex].objects.contains(m->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = m->index(i, 0);
        QModelIndex bottomRight = m->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_1->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.objSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.objSelectByLabel->setCurrentIndex(0);
  }
  EnableOKButton();
}

void ExportTableDialog::ObjUnselectAll()
{
  ui.listView_1->selectionModel()->clear();
  EnableOKButton();
}

void ExportTableDialog::VarSelectAll()
{
  ui.listView_2->selectAll();
  EnableOKButton();
}

void ExportTableDialog::VarInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_2->selectionModel();
  QAbstractItemModel *model = ui.listView_2->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  EnableOKButton();
}

void ExportTableDialog::VarSelectBy()
{
  if(ui.varSelectByLabel->currentIndex() > 0){
    int labelindex = ui.varSelectByLabel->currentIndex()-1;
    QItemSelection selection;
    for(int i = 0; i < ui.listView_2->model()->rowCount(); i++){
      if(varlabels[labelindex].objects.contains(ui.listView_2->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_2->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_2->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_2->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.varSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.varSelectByLabel->setCurrentIndex(0);
  }
  EnableOKButton();
}

void ExportTableDialog::VarUnselectAll()
{
  ui.listView_2->selectionModel()->clear();
  EnableOKButton();
}

ExportTableDialog::ExportTableDialog(QStringList objname, QStringList varname, LABELS *objectlabels_, LABELS *varlabels_): QDialog(0)
{
  ui.setupUi(this);

  if(objectlabels_ != 0)
    objectlabels = (*objectlabels_);
  else
    ui.objSelectByLabel->hide();

  if(varlabels_ != 0)
    varlabels = (*varlabels_);
  else
    ui.varSelectByLabel->hide();

  ui.listView_1->setModel(new QStringListModel(objname));
  ui.listView_2->setModel(new QStringListModel(varname));

  ui.objSelectByLabel->addItem("Select by label...");
  for(int i = 0; i < objectlabels.size(); i++){
    ui.objSelectByLabel->addItem(objectlabels[i].name);
  }

  ui.varSelectByLabel->addItem("Select by label...");
  for(int i = 0; i < varlabels.size(); i++){
    ui.varSelectByLabel->addItem(varlabels[i].name);
  }

  EnableOKButton();
  connect(ui.saveButton, SIGNAL(clicked(bool)), SLOT(SaveAs()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));

  connect(ui.filetosave, SIGNAL(textChanged(QString)), SLOT(EnableOKButton()));
  connect(ui.listView_1->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableOKButton()));
  connect(ui.listView_2->selectionModel(),SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableOKButton()));
  connect(ui.listView_1->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(EnableOKButton()));
  connect(ui.listView_2->selectionModel(),SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(EnableOKButton()));

  connect(ui.objselectAllButton, SIGNAL(clicked(bool)), SLOT(ObjSelectAll()));
  connect(ui.objinvertselectionButton, SIGNAL(clicked(bool)), SLOT(ObjInvertSelection()));
  connect(ui.objunselectAllButton, SIGNAL(clicked(bool)), SLOT(ObjUnselectAll()));
  connect(ui.objSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(ObjSelectBy()));

  connect(ui.varselectAllButton, SIGNAL(clicked(bool)), SLOT(VarSelectAll()));
  connect(ui.varinvertselectionButton, SIGNAL(clicked(bool)), SLOT(VarInvertSelection()));
  connect(ui.varunselectAllButton, SIGNAL(clicked(bool)), SLOT(VarUnselectAll()));
  connect(ui.varSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(VarSelectBy()));
}
