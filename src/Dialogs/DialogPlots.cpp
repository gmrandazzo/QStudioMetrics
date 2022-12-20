#include "DialogPlots.h"
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include "qstudiometricstypes.h"

int DialogPlots::getNLV(){ return ui.numpc->value(); }

int DialogPlots::getPredictionID(){
  if(pid > -1 && mid > -1 && predid > -1){
    return pjtree[pid].mtree[mid].ptree[predid].id;
  }
  else{
    return -1;
  }
}


int DialogPlots::getModelID()
{
  if(pid > -1){
    return pjtree[pid].mtree[mid].id;
  }
  else{
    return -1;
  }
}

int DialogPlots::getProjectID()
{
  if(pid > -1){
    return pjtree[pid].id;
  }
  else{
    return -1;
  }
}

void DialogPlots::EnableDisableButtons()
{
  if(dtype == TwoColumns){
    if(pid > -1 && mid > -1){
      ui.plotButton->setEnabled(true);
    }
    else{
      ui.plotButton->setEnabled(false);
    }
  }
  else{
    if(pid > -1 && mid > -1 && predid > -1){
      ui.plotButton->setEnabled(true);
    }
    else{
      ui.plotButton->setEnabled(false);
    }
  }
}

void DialogPlots::setPrediction(QModelIndex current, QModelIndex previous)
{
  predid = current.row();
  EnableDisableButtons();
}

void DialogPlots::setModel(QModelIndex current, QModelIndex previous)
{
  mid = current.row();
  if(dtype == ThreeColumns){
    tab3->clear();
    for(int i = 0; i < pjtree[pid].mtree[mid].ptree.size(); i++){
      QList<QStandardItem*> row;
      row.append(new QStandardItem(pjtree[pid].mtree[mid].ptree[i].name));
      tab3->appendRow(row);
    }
  }
  EnableDisableButtons();
}

void DialogPlots::setProject(QModelIndex current, QModelIndex previous)
{
  pid = current.row();
  tab2->clear();
  for(int i = 0; i < pjtree[pid].mtree.size(); i++){
    QList<QStandardItem*> row;
    row.append(new QStandardItem(pjtree[pid].mtree[i].name));
    tab2->appendRow(row);
  }
  EnableDisableButtons();
}

void DialogPlots::actionPlot()
{
  if(dtype == TwoColumns){
    if(pid > -1 && mid > -1){
      return accept();
    }
    else{
      return reject();
    }
  }
  else{
    if(pid > -1 && mid > -1 && predid > -1){
      return accept();
    }
    else{
      return reject();
    }
  }
}

void DialogPlots::hideOptions(bool opt)
{
  if(opt == true){
    ui.OptionGroupBox->hide();
  }
  else{
    ui.OptionGroupBox->show();
  }
}

DialogPlots::DialogPlots(ProjectTree pjtree_, int dtype_)
{
  int i;
  ui.setupUi(this);
  ui.listView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui.listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

  pjtree = pjtree_;
  dtype = dtype_;
  pid = mid = predid = -1;
  tab1 = new QStandardItemModel();
  ui.listView->setModel(tab1);

  tab2 = new QStandardItemModel();
  ui.listView_2->setModel(tab2);

  if(dtype == ThreeColumns){
    tab3 = new QStandardItemModel();
    ui.listView_3->setModel(tab3);
  }
  else{
    ui.predictionGroupBox->hide();
  }

  for(i = 0; i < pjtree.size(); i++){
    QList<QStandardItem*> row;
    row.append(new QStandardItem(pjtree[i].name));
    tab1->appendRow(row);
  }

  connect(ui.plotButton, SIGNAL(clicked()), SLOT(actionPlot()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.listView->selectionModel(),
          SIGNAL(currentChanged(QModelIndex, QModelIndex)),
          SLOT(setProject(QModelIndex, QModelIndex)));
  connect(ui.listView_2->selectionModel(),
          SIGNAL(currentChanged(QModelIndex,QModelIndex)),
          SLOT(setModel(QModelIndex, QModelIndex)));
  if(dtype == ThreeColumns){
    connect(ui.listView_3->selectionModel(),
            SIGNAL(currentChanged(QModelIndex,QModelIndex)),
            SLOT(setPrediction(QModelIndex, QModelIndex)));
  }
  EnableDisableButtons();
}

DialogPlots::~DialogPlots()
{
  delete tab1;
  delete tab2;
  if(dtype == ThreeColumns)
    delete tab3;
}
