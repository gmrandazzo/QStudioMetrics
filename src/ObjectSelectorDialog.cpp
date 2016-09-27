#include "ObjectSelectorDialog.h"
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include "qstudiometricstypes.h"

int ObjectSelectorDialog::getSelectionType()
{
  if(ui.mdcButton->isChecked()){
    return MOSTDESCRIPTIVECOMPOUND;
  }
  else if(ui.maxdissButton->isChecked()){
    return MAXIMUMDISSIMILARITYMAXMIN;
  }
  else if(ui.randomButton->isChecked()){
    return RANDOMSELECTION;
  }
  else{
    return LABELSELECTION;
  }
}

QString ObjectSelectorDialog::getLabel()
{
  if(ui.objlabelslst->count() > 0 && ui.objlabelslst->currentIndex() > -1){
    return (*objlabels_)[ui.objlabelslst->currentIndex()].name;
  }
  else{
    return QString("");
  }
}

int ObjectSelectorDialog::getMetric()
{
  if(ui.metricspacelist->currentIndex() == 0){
    return EUCLIDEANDISTANCE;
  }
  else if(ui.metricspacelist->currentIndex() == 1){
    return MANHATTANDISTANCE;
  }
  else{
    return COSINESSIMILARITY;
  }
}
int ObjectSelectorDialog::getNumberOfObjects()
{
  return ui.nobjectsBox->value();
}

QStringList ObjectSelectorDialog::getVariableList()
{
  QStringList selectedvars;
  if(windowtype == MenuSelection){ /* no coordinate selection... */
    if(ui.listView->model()){
      for(int i = 0; i < ui.listView->model()->rowCount(); i++){
        if(ui.listView->selectionModel()->isSelected(ui.listView->model()->index(i, 0)) == true){
          selectedvars.append(ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString());
        }
        else{
          continue;
        }
      }
    }
    else{
      return selectedvars;
    }
  }
  else if(windowtype == PlotSelection){
    if(getDataType() == 0){
      return selectedvars;
    }
    else{
      if(ui.listView->model()){
        for(int i = 0; i < ui.listView->model()->rowCount(); i++){
          if(ui.listView->selectionModel()->isSelected(ui.listView->model()->index(i, 0)) == true){
            selectedvars.append(ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString());
          }
          else{
            continue;
          }
        }
      }
      else{
        return selectedvars;
      }
    }
  }
  return selectedvars;
}

int ObjectSelectorDialog::getDataType()
{
  if(windowtype == PlotSelection){
    if(ui.plotcoordinatesButton->isChecked()){
      return 0;
    }
    else{
      return 1;
    }
  }
  else{
    return 1;
  }
}

int ObjectSelectorDialog::getDataID()
{
  return dataindex;
}

void ObjectSelectorDialog::setVariablesView(QModelIndex current)
{
  if(current.isValid()){
    dataindex = current.row();
    if(mxlst_ != 0){
      QStringList variablelst = (*mxlst_)[current.row()]->getVarName();
      variablelst.removeAll("Object Names");
      ui.listView->setModel(new QStringListModel(variablelst));
    }
    else{
      return;
    }
  }
  else{
    return;
  }
}

void ObjectSelectorDialog::EnableDisable()
{

  if(ui.mdcButton->isChecked()){
    ui.nobjectsBox->setMinimum(0);
    ui.nobjectsBox->setValue(0);
  }
  else{
    ui.nobjectsBox->setMinimum(1);
  }

  if(ui.randomButton->isChecked()){
    ui.dataGroupBox->setEnabled(false);
    ui.varspaceGroupBox->setEnabled(false);
  }
  else if(ui.mdcButton->isChecked() || ui.maxdissButton->isChecked()){
    ui.dataGroupBox->setEnabled(true);
    ui.varspaceGroupBox->setEnabled(true);

    if(getDataType() == 0){
      ui.listView_2->setEnabled(false);
      ui.listView->setEnabled(false);
    }
    else{
      ui.listView_2->setEnabled(true);
      ui.listView->setEnabled(true);
    }
  }
  else{ /*label button*/
    ui.dataGroupBox->setEnabled(false);
    ui.varspaceGroupBox->setEnabled(false);
  }
}

void ObjectSelectorDialog::SelectAll()
{
  ui.listView->selectAll();
  /*
  for(int i = 0; i < ui.listView->model()->rowCount(); i++){
    ui.listView->selectionModel()->select(ui.listView->model()->index(i, 0), QItemSelectionModel::Select);
  }
  */
}

void ObjectSelectorDialog::InvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView->selectionModel();
  QAbstractItemModel *model = ui.listView->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
}
void ObjectSelectorDialog::Unselect()
{
  ui.listView->selectionModel()->clear();
}

void ObjectSelectorDialog::SelectByVarLabel()
{
  int indx = ui.selectByVarLabel->currentIndex();
  if(indx > 0){
    for(int i = 0; i < ui.listView->model()->rowCount(); i++){
      if((*varlabels_)[indx-1].objects.contains(ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        ui.listView->selectionModel()->select(ui.listView->model()->index(i, 0), QItemSelectionModel::Select);
      }
      else{
        continue;
      }
    }
  }
  else{
    return;
  }
}

void ObjectSelectorDialog::OK()
{
  if((ui.mdcButton->isChecked() || ui.maxdissButton->isChecked())){
    if(ui.plotcoordinatesButton->isChecked()){
      return accept();
    }
    else{
      if(!ui.listView->selectionModel()->selectedIndexes().isEmpty() && ui.listView->selectionModel()->selectedIndexes().size() > 0){
        return accept();
      }
      else{
        return;
      }
    }
  }
  else if(ui.randomButton->isChecked()){
    return accept();
  }
  else if(ui.selectbyobjlabelbutton->isChecked() && ui.objlabelslst->count() > 0){
    return accept();
  }
  else{
    return;
  }
}

ObjectSelectorDialog::ObjectSelectorDialog(QList<MATRIX*> *mxlst, LABELS *objlabels, LABELS *varlabels, int windowtype_): QDialog()
{
  ui.setupUi(this);

  windowtype = windowtype_;

  if(windowtype == MenuSelection){
    ui.plotcoordinatesButton->hide();
    ui.sourcedataButton->hide();
  }

  mxlst_ = mxlst;
  dataindex = -1;
  objlabels_= objlabels;
  varlabels_ = varlabels;

  ui.objlabelslst->clear();

  for(int i = 0; i < (*objlabels_).size(); i++){
    ui.objlabelslst->addItem((*objlabels_)[i].name);
  }

  for(int i = 0; i < (*varlabels_).size(); i++){
    ui.selectByVarLabel->addItem((*varlabels_)[i].name);
  }

  QStringList mxnamelst;
  for(int i = 0; i < (*mxlst_).size(); i++){
    mxnamelst.append((*mxlst_)[i]->getName());
  }
  ui.listView_2->setModel(new QStringListModel(mxnamelst));

  EnableDisable();

  connect(ui.mdcButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.maxdissButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.randomButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.selectbyobjlabelbutton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.plotcoordinatesButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.sourcedataButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setVariablesView(QModelIndex)));
  connect(ui.selectAllButton, SIGNAL(clicked(bool)), SLOT(SelectAll()));
  connect(ui.invertSelectionButton, SIGNAL(clicked(bool)), SLOT(InvertSelection()));
  connect(ui.selectByVarLabel, SIGNAL(currentIndexChanged(int)), SLOT(SelectByVarLabel()));
  connect(ui.unselectButton, SIGNAL(clicked(bool)), SLOT(Unselect()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
}

ObjectSelectorDialog::ObjectSelectorDialog(QStringList &currentvariables, LABELS *objlabels, LABELS *varlabels): QDialog()
{
  ui.setupUi(this);

  windowtype = MenuSelection;
  ui.plotcoordinatesButton->setChecked(false);
  ui.sourcedataButton->setChecked(true);
  ui.dataGroupBox->hide();
  dataindex = -1;
  objlabels_= objlabels;
  varlabels_ = varlabels;

  ui.objlabelslst->clear();

  for(int i = 0; i < (*objlabels_).size(); i++){
    ui.objlabelslst->addItem((*objlabels_)[i].name);
  }

  for(int i = 0; i < (*varlabels_).size(); i++){
    ui.selectByVarLabel->addItem((*varlabels_)[i].name);
  }

  ui.listView->setModel(new QStringListModel(currentvariables));

  adjustSize();
  EnableDisable();

  connect(ui.mdcButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.maxdissButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.randomButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.selectbyobjlabelbutton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.plotcoordinatesButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.sourcedataButton, SIGNAL(clicked(bool)), SLOT(EnableDisable()));
  connect(ui.selectAllButton, SIGNAL(clicked(bool)), SLOT(SelectAll()));
  connect(ui.invertSelectionButton, SIGNAL(clicked(bool)), SLOT(InvertSelection()));
  connect(ui.selectByVarLabel, SIGNAL(currentIndexChanged(int)), SLOT(SelectByVarLabel()));
  connect(ui.unselectButton, SIGNAL(clicked(bool)), SLOT(Unselect()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
}
