#include "FindCorrelationWidget.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>

void FindCorrelationWidget::SelectAll()
{
  ui.listView->selectAll();
}

void FindCorrelationWidget::InvertSelection()
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

void FindCorrelationWidget::UnselectAll()
{
  ui.listView->selectionModel()->clear();
}

void FindCorrelationWidget::PlotVarVSVar()
{
  /*
  QString propstr;
  propstr = QString::fromUtf8("\u221D");
  for(int i = 0; i < ui.listView->model()->rowCount(); i++){
    if(ui.listView->selectionModel()->isSelected(ui.listView->model()->index(i, 0)) == true){
      QStringList vars = ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString().split(propstr);
      if(vars.size() == 2){
        vvplotSignal vvs;
        vvs.vid1 = varname.indexOf(vars[0].trimmed().remove("1/"));
        vvs.vid2 = varname.indexOf(vars[1].trimmed().remove("1/"));
        vvs.pid = pid;
        vvs.mid = mid;
        vvs.mtype = mtype;
        emit VVPlotSignal(vvs);
        //   emit pid, mid, vid1, vid2, mtype;
      }
      else{
        continue;
      }
    }
    else{
      continue;
    }
  }
  */
  
  QModelIndexList list = ui.listView->selectionModel()->selectedIndexes();

  QString propstr;
  propstr = QString::fromUtf8("\u221D");
  foreach(const QModelIndex &index, list){
    QStringList vars = index.data(Qt::DisplayRole).toString().split(propstr);
    if(vars.size() == 2){
      vvplotSignal vvs;
      vvs.vname1 = vars[0].trimmed().remove("1/");
      vvs.vname2 = vars[1].trimmed().remove("1/");
      vvs.pid = pid;
      vvs.mid = mid;
      vvs.mtype = mtype;
      emit VVPlotSignal(vvs);
      //   emit pid, mid, vid1, vid2, mtype;
    }
    else{
      continue;
    }
  }

  /*
  int vid1;
  int vid2;
  
  QString propstr;
  propstr = QString::fromUtf8("\u221D");
  for(int i = 0; i < selectionModel->selectedIndexes().size(); i++){
    QStringList vars = selectionModel->selectedIndexes()[i].data(Qt::DisplayRole).toString().split(propstr);
    if(vars.size() == 2){
      vid1 = varname.indexOf(vars[0].trimmed().remove("1/"));
      vid2 = varname.indexOf(vars[1].trimmed().remove("1/"));
      vvplotSignal vvs;
      vvs.pid = pid;
      vvs.mid = mid;
      vvs.mtype = mtype;
      vvs.vid1 = vid1;
      vvs.vid2 = vid2;
      emit VVPlotSignal(vvs);
      //   emit pid, mid, vid1, vid2, mtype;
    }
    else{
      continue;
    }
  }
  */
}

void FindCorrelationWidget::Higlight()
{
  QModelIndexList list = ui.listView->selectionModel()->selectedIndexes();
  highlightSignal hhs;
  QString propstr;
  propstr = QString::fromUtf8("\u221D");
  foreach(const QModelIndex &index, list){
    QStringList vars = index.data(Qt::DisplayRole).toString().split(propstr);
    if(vars.size() == 2){
      hhs.vid1.append(varname.indexOf(vars[0].trimmed().remove("1/")));
      hhs.vid2.append(varname.indexOf(vars[1].trimmed().remove("1/")));
    }
    else{
      continue;
    }
  }
  emit HighlightSignal(hhs);
  
  /*
  QString propstr;
  propstr = QString::fromUtf8("\u221D");
  for(int i = 0; i < ui.listView->selectionModel()->selectedIndexes().size(); i++){
    QStringList vars = ui.listView->selectionModel()->selectedIndexes()[i].data(Qt::DisplayRole).toString().split(propstr);
    if(vars.size() == 2){
      highlightSignal hhs;
      hhs.vid1 = varname.indexOf(vars[0].trimmed().remove("1/"));
      hhs.vid2 = varname.indexOf(vars[1].trimmed().remove("1/"));
      emit HighlightSignal(hhs);
    }
    else{
      continue;
    }
  }
  */
}


void FindCorrelationWidget::Find()
{
  if(cmx->row > 0){
    double aerror = ui.angle_error->value();
    double derror = ui.distance_error->value()/100.;
    
    if(FLOAT_EQ(derror, 0, 0))
      derror = 1e-4;
    
    // search angle at 0 ° and angle at 180 ° with a defined error

    corr_0.clear(); corr_180.clear();
    if(ui.allcorrelations->isChecked() == true){
      for(uint i = 0; i < cmx->row; i++){
        for(uint j = i+1; j < cmx->col; j++){
          if(FLOAT_EQ(cmx->data[i][j], 0, aerror) && derr->data[i][j] < derror){
            corr_0.append(CorrVarVarPlotSignal());
            corr_0.last().varid1 = i;
            corr_0.last().varid2 = j;
          }
          else if(FLOAT_EQ(cmx->data[i][j], 180, aerror) && derr->data[i][j] < derror){
            corr_180.append(CorrVarVarPlotSignal());
            corr_180.last().varid1 = i;
            corr_180.last().varid2 = j;
          }
          else{
            continue;
          }
        }
      }
    }
    else if(ui.correlationsto->isChecked() == true){
      uint i = ui.variables->currentIndex();
      for(uint j = 0; j < cmx->col; j++){
        if(j != i){
          if(FLOAT_EQ(cmx->data[i][j], 0, aerror) && derr->data[i][j] < derror){
            corr_0.append(CorrVarVarPlotSignal());
            corr_0.last().varid1 = i;
            corr_0.last().varid2 = j;
          }
          else if(FLOAT_EQ(cmx->data[i][j], 180, aerror) && derr->data[i][j] < derror){
            corr_180.append(CorrVarVarPlotSignal());
            corr_180.last().varid1 = i;
            corr_180.last().varid2 = j;
          }
          else{
            continue;
          }
        }
        else{
          continue;
        }
      }
    }
    else{
      if(ui.varlist->count() > 0){
        QStringList selvarnames  = varlabels->at(ui.varlist->currentIndex()).objects;
        
        QList< int > selvars;
        for(int i = 0; i < selvarnames.size(); i++){
          int id = varname.indexOf(selvarnames[i]);
          if(id > -1 && id < (int)cmx->row){
            selvars.append(id);
          }
          else{
            continue;
          }
        }
        
        for(int i = 0; i < selvars.size(); i++){
          /*for(uint j = selvars[i]+1; j < cmx->col; j++){*/
          for(uint j = 0; j < cmx->col; j++){
            if(selvars[i] != (int)j){
              if(FLOAT_EQ(cmx->data[selvars[i]][j], 0, aerror) && derr->data[selvars[i]][j] < derror){
                corr_0.append(CorrVarVarPlotSignal());
                corr_0.last().varid1 = selvars[i];
                corr_0.last().varid2 = j;
              }
              else if(FLOAT_EQ(cmx->data[selvars[i]][j], 180, aerror) && derr->data[selvars[i]][j] < derror){
                corr_180.append(CorrVarVarPlotSignal());
                corr_180.last().varid1 = selvars[i];
                corr_180.last().varid2 = j;
              }
              else{
                continue;
              }
            }
            else{
              continue;
            }
          }
        }
      }
      else{
        return;
      }
    }
    
    model->clear();
    
    QString propstr;
    propstr = QString::fromUtf8("\u221D");
    
    if(ui.directly_proportional_checkBox->isChecked() == true && ui.inversely_proportional_checkBox->isChecked() == true){
      for(int i = 0; i < corr_0.size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString("%1 %2 %3").arg(varname[corr_0[i].varid1]).arg(propstr).arg(varname[corr_0[i].varid2])));
        model->appendRow(row);
      }
      for(int i = 0; i < corr_180.size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString("%1 %2 1/%3").arg(varname[corr_180[i].varid1]).arg(propstr).arg(varname[corr_180[i].varid2])));
        model->appendRow(row);
      }
    }
    else if(ui.directly_proportional_checkBox->isChecked() == true && ui.inversely_proportional_checkBox->isChecked() == false){
      for(int i = 0; i < corr_0.size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString("%1 %2 %3").arg(varname[corr_0[i].varid1]).arg(propstr).arg(varname[corr_0[i].varid2])));
        model->appendRow(row);
      }
    }
    else if(ui.directly_proportional_checkBox->isChecked() == false && ui.inversely_proportional_checkBox->isChecked() == true){
      for(int i = 0; i < corr_180.size(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(QString("%1 %2 1/%3").arg(varname[corr_180[i].varid1]).arg(propstr).arg(varname[corr_180[i].varid2])));
        model->appendRow(row);
      }
    }
    else{
      return;
    }
  }
  else{
    return;
  }
}

FindCorrelationWidget::FindCorrelationWidget(matrix *cmx_, matrix *derr_, QStringList *varname_, LABELS *varlabels_, int pid_, int mid_, int mtype_)
{
  ui.setupUi(this);
  setAttribute(Qt::WA_DeleteOnClose);
  model = new QStandardItemModel();
  
  ui.listView->setModel(model);
  
  initMatrix(&cmx);
  initMatrix(&derr);
  MatrixCopy(cmx_, &cmx);
  MatrixCopy(derr_, &derr);
  
  varname = (*varname_);
  varlabels = varlabels_;
  
  pid = pid_;
  mid = mid_;
  mtype = mtype_;
  
  ui.variables->addItems(varname);
  
  for(int i = 0; i < varlabels->size(); i++){
    ui.varlist->addItem(varlabels->at(i).name);
  }
  
  connect(ui.closeButton, SIGNAL(clicked(bool)), SLOT(close()));
  connect(ui.plotButton, SIGNAL(clicked(bool)), SLOT(PlotVarVSVar()));
  connect(ui.highlightButton, SIGNAL(clicked(bool)), SLOT(Higlight()));
  connect(ui.selectallButton, SIGNAL(clicked(bool)), this, SLOT(SelectAll()));
  connect(ui.invertselectionButton, SIGNAL(clicked(bool)), this, SLOT(InvertSelection()));
  connect(ui.unselectallButton, SIGNAL(clicked(bool)), SLOT(UnselectAll()));
  connect(ui.findButton, SIGNAL(clicked(bool)), SLOT(Find()));
}


FindCorrelationWidget::~FindCorrelationWidget()
{
  DelMatrix(&cmx);
  DelMatrix(&derr);
  emit CloseFindCorrelationWidgetSignal();
}

