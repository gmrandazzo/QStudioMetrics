#include "SelectByDialog.h"
#include "ObjectSelectorDialog.h"
#include <QStringList>
#include <QStringListModel>
#include <QApplication>
#include <QMessageBox>

#include <scientific.h>

#include "run.h"

void SelectByDialog::StopSelectionRun()
{
  abort = true;
  ui.progressBar->setMinimum(0);
  ui.progressBar->setMaximum(100);
  ui.progressBar->setValue(20);
  ui.label_3->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();
}

void SelectByDialog::WaitSelectionRun()
{
  ui.abortButton->setEnabled(false);
}

void SelectByDialog::StartSelectionRun()
{
  abort = false;
  ui.abortButton->setEnabled(true);
  ui.progressBar->setMinimum(0);
  ui.progressBar->setMaximum(0);
  ui.progressBar->setValue(0);
  ui.label_3->show();
  ui.progressBar->show();
  ui.abortButton->show();
}

bool SelectByDialog::CheckIDSmxlst(QList<int> *ids)
{
  if(mxlst != 0 && (*ids).size() > 0){
    for(int i = 0; i < (*ids).size(); i++){
      if((*ids)[i] < mxlst->size()){
        continue;
      }
      else{
        return false;
      }
    }
    return true;
  }
  else{
    return false;
  }
}

void SelectByDialog::SetSelectionView()
{
  QList<QString> selection;

  if(ui.variableSelector->isChecked()){
    if(mxid.size() > 0){ // Select by Variable
      bool minok, maxok;
      double min = ui.minvaledit->text().toDouble(&minok);
      double max = ui.maxvaledit->text().toDouble(&maxok);
      int j = ui.varlist->currentIndex();
      if(mxlst != 0 && (*mxlst).size() > 0){
        if(minok == true && maxok == true){
          for(int k = 0; k < mxid.size(); k++){
            int id_ = mxid[k];
            for(uint i = 0; i < (*mxlst)[id_ ]->Matrix()->row; i++){
              double x = (*mxlst)[id_ ]->Matrix()->data[i][j];
              if( (x > min || FLOAT_EQ(x, min, EPSILON)) && (x < max || FLOAT_EQ(x, max, EPSILON)) ){
                selection.append((*mxlst)[id_ ]->getObjName()[i]);
              }
              else{
                continue;
              }
            }
          }
        }
        else{
          QMessageBox::warning(this, tr("Select by matrix value"),
                               tr("Unable to select by matrix value\n"
                                  "Wrong min/max value. Please type valid numbers."),
                               QMessageBox::Ok);
        }
      }
      else{
        QMessageBox::warning(this, tr("Select by matrix value"),
                             tr("Unable to select by matrix value\n"
                                "No input data matrix found."),
                             QMessageBox::Ok);
      }
    }
  }
  else{ // Select by Advanced Selector
    if(selectiontype == MOSTDESCRIPTIVECOMPOUND || selectiontype == MAXIMUMDISSIMILARITYMAXMIN){
      if(metric != -1 && nobjects != -1 && ((datatype == 1 && dataid != -1 && varlist.size() > 0) || datatype == 0)){
        matrix *m;
        uivector *selected;
        RUN obj;

        if(datatype == 1){
          NewMatrix(&m, (*mxlst)[dataid]->Matrix()->row, varlist.size());
          QStringList varnames = (*mxlst)[dataid]->getVarName();
          varnames.removeAll("Object Names");

          int col = 0;
          for(int j = 0; j < varlist.size(); j++){
            int colindex = varnames.indexOf(varlist[j]);
            if(colindex !=  -1){
              for(uint i = 0; i < (*mxlst)[dataid]->Matrix()->row; i++){
                setMatrixValue(m, i, col, getMatrixValue((*mxlst)[dataid]->Matrix(), i, colindex));
              }
              col++;
            }
            else{
              qDebug() << "Error in selection columnindex this variable " << varlist[j] << "seems to be not present.";
            }
          }
          obj.setMatrix(m);
        }
        else{
          obj.setMatrix(coordinate);
        }

        initUIVector(&selected);

        obj.setUIVector(selected);
        obj.setMetric(metric);
        obj.setNumberOfObject(nobjects);

        StartSelectionRun();
        QFuture<void> future;
        if(selectiontype == MOSTDESCRIPTIVECOMPOUND){
          future = obj.RunMDCSelection();
        }
        else{
          future = obj.RunMaxDisSelection();

        }

        while(!future.isFinished()){
          if(abort == true){
            obj.AbortRun();
            QApplication::processEvents();
          }
          else{
            QApplication::processEvents();
          }
        }

        if(abort == false){
          WaitSelectionRun();
          if(datatype == 1){
            for(uint i = 0; i < selected->size; i++){
              selection.append((*mxlst)[dataid]->getObjName()[getUIVectorValue(selected, i)]);
            }
            DelMatrix(&m);
          }
          else{
            for(uint i = 0; i < selected->size; i++){
              selection.append(objname[getUIVectorValue(selected, i)]);
            }
          }
        }

        DelUIVector(&selected);
        StopSelectionRun();
      }
      else{
        return;
      }
    }
    else if(selectiontype == RANDOMSELECTION){
      if(nobjects != -1 && ((datatype == 1 && dataid != -1) || datatype == 0)){
        uivector *selected;
        RUN obj;

        initUIVector(&selected);

        obj.setUIVector(selected);
        obj.setNumberOfObject(nobjects);
        if(datatype == 1){
          obj.setNumberMaxOfObject((*mxlst)[dataid]->Matrix()->row);
        }
        else{
          obj.setNumberMaxOfObject(coordinate->row);
        }

        StartSelectionRun();
        QFuture<void> future = obj.RunRandomSelection();;

        while(!future.isFinished()){
          if(abort == true){
            obj.AbortRun();
            QApplication::processEvents();
          }
          else{
            QApplication::processEvents();
          }
        }

        if(abort == false){
          WaitSelectionRun();
          for(uint i = 0; i < selected->size; i++){
            selection.append(objname[getUIVectorValue(selected, i)]);
          }
        }

        DelUIVector(&selected);
        StopSelectionRun();
      }
      else{
        return;
      }
    }
    else{
      qDebug() << "Label Selection";
      if(label.size() > 0){
        int lindex = -1;
        for(int i = 0; i < (*objlabel).size(); i++){
          if((*objlabel)[i].name.compare(label) == 0){
            lindex = i;
            break;
          }
          else{
            continue;
          }
        }
        if(lindex > -1){
          selection.append((*objlabel)[lindex].objects);
        }
        else{
          return;
        }
      }
    }
  }

  if(append == true){
    for(int i = 0; i < selection.size(); i++){
      int id = objname.indexOf(selection[i]);
      if(id != -1){
        ui.objnamelistView->selectionModel()->select(ui.objnamelistView->model()->index(id, 0), QItemSelectionModel::Select);
      }
    }
  }
  else{
    ui.objnamelistView->selectionModel()->clear();
    for(int i = 0; i < selection.size(); i++){
      int id = objname.indexOf(selection[i]);
      if(id != -1){
        ui.objnamelistView->selectionModel()->select(ui.objnamelistView->model()->index(id, 0), QItemSelectionModel::Select);
      }
    }
  }
}

void SelectByDialog::UpdateSelectionCounter()
{
  ui.selobjectlabel->setText(QString("%1/%2").arg(QString::number(ui.objnamelistView->selectionModel()->selectedIndexes().size())).arg(QString::number(ui.objnamelistView->model()->rowCount())));
}

void SelectByDialog::selectButton()
{
  append = false;
  SetSelectionView();
  UpdateSelectionCounter();
}

void SelectByDialog::invertselectButton()
{
  QItemSelectionModel *selectionModel =  ui.objnamelistView->selectionModel();
  QAbstractItemModel *model = ui.objnamelistView->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  UpdateSelectionCounter();
}

void SelectByDialog::appendselectionButton()
{
  append = true;
  SetSelectionView();
  UpdateSelectionCounter();
}

void SelectByDialog::unselectButton()
{
  ui.objnamelistView->selectionModel()->clear();
  UpdateSelectionCounter();
}

void SelectByDialog::OpenSelectorButton()
{
  ObjectSelectorDialog objseldialog(mxlst, objlabel, varlabel, ObjectSelectorDialog::PlotSelection);
  if(objseldialog.exec() == QDialog::Accepted){
    ui.buttonframe->setEnabled(true);
    selectiontype = objseldialog.getSelectionType();
    metric = objseldialog.getMetric();
    nobjects = objseldialog.getNumberOfObjects();
    datatype = objseldialog.getDataType();
    dataid = objseldialog.getDataID();
    label = objseldialog.getLabel();
    varlist = objseldialog.getVariableList();
  }
  else{
    selectiontype = metric = datatype = dataid = layerid =  nobjects = -1;
    varlist.clear();
    label.clear();
    ui.buttonframe->setEnabled(false);
  }
}

void SelectByDialog::FindName()
{
  QString linetofind = ui.searchline->text();
  if(!linetofind.isEmpty()){
    for(int i = 0; i < ui.objnamelistView->model()->rowCount(); i++){
      if(ui.objnamelistView->model()->index(i,0).data(Qt::DisplayRole).toString().contains(linetofind) == true){
        ui.objnamelistView->selectionModel()->select(ui.objnamelistView->model()->index(i, 0), QItemSelectionModel::Select);
        ui.objnamelistView->scrollTo(ui.objnamelistView->model()->index(i, 0));
      }
      else{
        continue;
      }
    }
  }
  else{
    return;
  }
  UpdateSelectionCounter();
}

void SelectByDialog::Abort()
{
  abort = true;
}

void SelectByDialog::SetSelectionList()
{
  if(ui.variableSelector->isChecked()){ // Select by Variable
    ui.openSelectorButton->setEnabled(false);
    ui.varlist->setEnabled(true);
    ui.minvaledit->setEnabled(true);
    ui.maxvaledit->setEnabled(true);
    ui.varlist->clear();
    if(mxid.size() > 0){
      if(CheckIDSmxlst(&mxid) == true){ // is matrix
        QStringList varname;
        varname = (*mxlst)[mxid[0]]->getVarName();
        varname.removeFirst();
        for(int i = 0; i < varname.size(); i++){
          ui.varlist->addItem(varname[i]);
        }
      }
      else{
        ui.varlist->clear();
      }
    }
  }
  else{ // Select by other..... MDC, MAXMIN, RANDOM, LABELS
    ui.openSelectorButton->setEnabled(true);
    ui.varlist->setEnabled(false);
    ui.minvaledit->setEnabled(false);
    ui.maxvaledit->setEnabled(false);
  }
}

void SelectByDialog::OK()
{
  for(int i = 0; i < ui.objnamelistView->model()->rowCount(); i++){
    if(ui.objnamelistView->selectionModel()->isSelected(ui.objnamelistView->model()->index(i, 0)) == true){
      selectedobjname.append(objname[i]);
      selectedobjid.append(i);
    }
    else{
      continue;
    }
  }
  accept();
}

void SelectByDialog::Cancel()
{
  reject();
}

SelectByDialog::SelectByDialog(QStringList objname_,  matrix* coordinate_, QList<MATRIX*> *mxlst_, QList<int> mxid_, LABELS *objlabel_, LABELS *varlabel_): QDialog()
{
  ui.setupUi(this);

  abort = false;
  objname = objname_;
  initMatrix(&coordinate);
  MatrixCopy(coordinate_, &coordinate);
  mxlst = mxlst_;
  objlabel = objlabel_;
  varlabel = varlabel_;
  mxid = mxid_;

  selectiontype = metric = nobjects = dataid = layerid = -1;

  StopSelectionRun();
  append = false;

  ui.objnamelistView->setModel(new QStringListModel(objname));

  SetSelectionList();
  UpdateSelectionCounter();

  connect(ui.objnamelistView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(UpdateSelectionCounter()));

  connect(ui.variableSelector, SIGNAL(clicked(bool)), SLOT(SetSelectionList()));
  connect(ui.advancedSelector, SIGNAL(clicked(bool)), SLOT(SetSelectionList()));

  connect(ui.openSelectorButton, SIGNAL(clicked(bool)), SLOT(OpenSelectorButton()));
  connect(ui.selectButton, SIGNAL(clicked(bool)), SLOT(selectButton()));
  connect(ui.invertselectButton, SIGNAL(clicked(bool)), SLOT(invertselectButton()));
  connect(ui.appendselecttionButton, SIGNAL(clicked(bool)), SLOT(appendselectionButton()));
  connect(ui.unselectButton, SIGNAL(clicked(bool)), SLOT(unselectButton()));
  connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(Abort()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.findButton, SIGNAL(clicked(bool)), SLOT(FindName()));
}

SelectByDialog::~SelectByDialog()
{
  DelMatrix(&coordinate);
}
