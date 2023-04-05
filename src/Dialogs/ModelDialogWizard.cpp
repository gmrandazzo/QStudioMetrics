#include "ModelDialogWizard.h"
#include "addLabelDialog.h"

#include <QScreen>
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QStandardPaths>
#include <QFile>
#include <QString>
#include <QByteArray>
#include <QFileDialog>

#ifdef DEBUG
#include <QDebug>
#endif

#include "qstudiometricstypes.h"

void ModelDialogWizard::WindowAdjust()
{
  adjustSize();
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  this->move(screenGeometry.center() - this->rect().center());
}

int ModelDialogWizard::CheckLabelNameInLabels(QString label, LABELS lbl)
{
  int id = -1;
  for(int i = 0; i < lbl.size(); i++){
    if(lbl[i].name.compare(label) == 0){
      id = i;
      break;
    }
    else{
      continue;
    }
  }
  return id;
}

int ModelDialogWizard::CheckNameInLabel(QString name, LABEL lbl)
{
  if(lbl.objects.contains(name) == true)
    return lbl.objects.indexOf(name);
  else
    return -1;
}

void ModelDialogWizard::AddObject2Class(QString class_name, QString objname)
{
  QAbstractItemModel *model = ui.listView_6->model();
  int lbl_indx = CheckLabelNameInLabels(class_name, classes);
  if(lbl_indx > -1){
    // Class found then add object if exist
    int obj_indx = CheckNameInLabel(objname, classes[lbl_indx]);
    if(obj_indx > -1){
      //Notthing to do...
      return;
    }
    else{
      for(int i = 0; i < model->rowCount(); i++){
        if(model->index(i, 0).data(Qt::DisplayRole).toString().compare(objname) == 0){
          classes[lbl_indx].objects.append(objname);
          model->removeRow(i);
          break;
        }
        else{
          continue;
        }
      }
    }
  }
  else{
    // Class not found then if object exist add class.
    for(int i = 0; i < model->rowCount(); i++){
      if(model->index(i, 0).data(Qt::DisplayRole).toString().compare(objname) == 0){
        classes.append(LABEL());
        classes.last().objects.append(objname);
        classes.last().name = class_name;
        QList<QStandardItem*> row;
        row.append(new QStandardItem(class_name));
        tab7->appendRow(row);
        model->removeRow(i);
        break;
      }
      else{
        continue;
      }
    }
  }
}

void ModelDialogWizard::AddVariable2Block(QString block_name, QString varname)
{
  QAbstractItemModel *model = ui.listView_8->model();
  int lbl_indx = CheckLabelNameInLabels(block_name, xblocks);
  if(lbl_indx > -1){
    // Class found then add object if exist
    int obj_indx = CheckNameInLabel(varname, xblocks[lbl_indx]);
    if(obj_indx > -1){
      //Notthing to do...
      return;
    }
    else{
      for(int i = 0; i < model->rowCount(); i++){
        if(model->index(i, 0).data(Qt::DisplayRole).toString().compare(varname) == 0){
          xblocks[lbl_indx].objects.append(varname);
          model->removeRow(i);
          break;
        }
        else{
          continue;
        }
      }
    }
  }
  else{
    // Class not found then if object exist add class.
    for(int i = 0; i < model->rowCount(); i++){
      if(model->index(i, 0).data(Qt::DisplayRole).toString().compare(varname) == 0){
        xblocks.append(LABEL());
        xblocks.last().objects.append(varname);
        xblocks.last().name = block_name;
        QList<QStandardItem*> row;
        row.append(new QStandardItem(block_name));
        tab9->appendRow(row);
        model->removeRow(i);
        break;
      }
      else{
        continue;
      }
    }
  }
}

void ModelDialogWizard::BlockByLabel()
{
  if(ui.xblock_SelectByLabel->currentIndex() > 0){
    int labelindex = ui.xblock_SelectByLabel->currentIndex()-1;
    QStringList lstvars =  projects_->value(selectedproject_)->getVariableLabels()[labelindex].objects;
    QString block_name = projects_->value(selectedproject_)->getVariableLabels()[labelindex].name;
    for(int i = 0; i < lstvars.size(); i++){
      AddVariable2Block(block_name,  lstvars[i]);
    }
    ui.xblock_SelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.xblock_SelectByLabel->setCurrentIndex(0);
    return;
  }

}

void ModelDialogWizard::importBlock()
{

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Variable Name List"), "", tr("Files (*.txt, *.csv)"));

  if(!fileName.isEmpty()){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    //QAbstractItemModel *model = ui.listView_6->model();

    QTextStream in(&file);
    while(!in.atEnd()){
      QString line = in.readLine();
      QStringList linesplit = line.split(",");
      if(linesplit.size() != 2)
        continue;
      else{
        // linesplit[0]; objname
        // linesplit[1]; Classname
        AddVariable2Block(linesplit[1], linesplit[0]);
      }
    }
    EnableDisableButtons();
  }
  else{
    return;
  }
}

void ModelDialogWizard::addBlock()
{
  addLabelDialog groupnamedialog("Class Name");
  if(groupnamedialog.exec() == QDialog::Accepted){
    QModelIndexList selected = ui.listView_8->selectionModel()->selectedRows();
    if(selected.size() > 0){
      xblocks.append(LABEL());
      xblocks.last().name = groupnamedialog.getLabel();

      QList<QStandardItem*> row;
      row.append(new QStandardItem(groupnamedialog.getLabel()));
      tab9->appendRow(row);

      for(int i = 0; i < selected.size(); i++){
        QString obj = selected[i].data(Qt::DisplayRole).toString();
        xblocks.last().objects.append(obj);
      }

      // Remove selected index from the view
      QModelIndexList indexes;
      while((indexes = ui.listView_8->selectionModel()->selectedIndexes()).size()) {
        ui.listView_8->model()->removeRow(indexes.first().row());
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

void ModelDialogWizard::removeBlock()
{
  QModelIndexList indexes = ui.listView_9->selectionModel()->selectedIndexes();

  if(indexes.size() < 1)
    return;

  for(int i = 0; i < indexes.size(); i++){
    int indx = indexes[i].row();
    if(indx > -1 && indx < xblocks.size()){
      for(int i = 0; i < xblocks[indx].objects.size(); i++){
        QList<QStandardItem*> mname;
        mname.append(new QStandardItem(xblocks[indx].objects[i]));
        tab6->appendRow(mname);
      }
      xblocks.removeAt(indx);
      ui.listView_9->model()->removeRow(indx);
    }
    else{
      return;
    }
  }
}

void ModelDialogWizard::ClassByLabel()
{
  if(ui.class_SelectByLabel->currentIndex() > 0){
    int labelindex = ui.class_SelectByLabel->currentIndex()-1;
    QStringList lstobjects =  projects_->value(selectedproject_)->getObjectLabels()[labelindex].objects;
    QString class_name = projects_->value(selectedproject_)->getObjectLabels()[labelindex].name;
    for(int i = 0; i < lstobjects.size(); i++){
      AddObject2Class(class_name,  lstobjects[i]);
    }
    ui.class_SelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.class_SelectByLabel->setCurrentIndex(0);
    return;
  }

}

void ModelDialogWizard::importClass()
{

  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Object Name List"), "", tr("Files (*.txt, *.csv)"));

  if(!fileName.isEmpty()){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    //QAbstractItemModel *model = ui.listView_6->model();

    QTextStream in(&file);
    while(!in.atEnd()){
      QString line = in.readLine();
      QStringList linesplit = line.split(",");
      if(linesplit.size() < 2)
        continue;
      else{
        // linesplit[0]; objname
        //linesplit[1]; Classname
        AddObject2Class(linesplit[1], linesplit[0]);
      }
    }
  }
  else{
    return;
  }
}

void ModelDialogWizard::addClass()
{
  addLabelDialog groupnamedialog("Class Name");
  if(groupnamedialog.exec() == QDialog::Accepted){
    QModelIndexList selected = ui.listView_6->selectionModel()->selectedRows();
    if(selected.size() > 0){
      classes.append(LABEL());
      classes.last().name = groupnamedialog.getLabel();

      QList<QStandardItem*> row;
      row.append(new QStandardItem(groupnamedialog.getLabel()));
      tab7->appendRow(row);

      for(int i = 0; i < selected.size(); i++){
        QString obj = selected[i].data(Qt::DisplayRole).toString();
        classes.last().objects.append(obj);
      }

      // Remove selected index from the view
      QModelIndexList indexes;
      while((indexes = ui.listView_6->selectionModel()->selectedIndexes()).size()) {
        ui.listView_6->model()->removeRow(indexes.first().row());
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

void ModelDialogWizard::removeClass()
{
  QModelIndexList indexes = ui.listView_7->selectionModel()->selectedIndexes();

  if(indexes.size() < 1)
    return;

  for(int i = 0; i < indexes.size(); i++){
    int indx = indexes[i].row();
    if(indx > -1 && indx < classes.size()){
      for(int i = 0; i < classes[indx].objects.size(); i++){
        QList<QStandardItem*> mname;
        mname.append(new QStandardItem(classes[indx].objects[i]));
        tab6->appendRow(mname);
      }
      classes.removeAt(indx);
      ui.listView_7->model()->removeRow(indx);
    }
    else{
      return;
    }
  }
}

void ModelDialogWizard::genListView(QModelIndex current, QModelIndex previous)
{
  if(current.isValid()){

    selectedproject_ = pids[current.row()];

    tab2->clear();
    if(projects_->keys().contains(selectedproject_) == true){
      if(projects_->value(selectedproject_)->MatrixCount() > 0){
        for(int i = 0; i < projects_->value(selectedproject_)->MatrixCount(); i++){
          QList<QStandardItem*> row_tab2, row_tab6;
          row_tab2.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(i)->getName()));
          tab2->appendRow(row_tab2);
        }
      }

      for(int i = 0; i < projects_->value(selectedproject_)->getObjectLabels().size(); i++){
        ui.objSelectByLabel->addItem(projects_->value(selectedproject_)->getObjectLabels()[i].name);
        ui.class_SelectByLabel->addItem(projects_->value(selectedproject_)->getObjectLabels()[i].name);
      }

      for(int i = 0; i < projects_->value(selectedproject_)->getVariableLabels().size(); i++){
        ui.xvarSelectByLabel->addItem(projects_->value(selectedproject_)->getVariableLabels()[i].name);
        ui.yvarSelectByLabel->addItem(projects_->value(selectedproject_)->getVariableLabels()[i].name);
      }
    }
  }
}

void ModelDialogWizard::EnableDisableButtons()
{
  if(ui.modelname->text().isEmpty() == true ||
     ui.listView_1->selectionModel()->selectedRows(0).size() == 0 ||
     ui.listView_2->selectionModel()->selectedRows(0).size() == 0){
    this->button(QWizard::NextButton)->setEnabled(false);
    this->button(QWizard::FinishButton)->setEnabled(false);
  }
  else{
    this->button(QWizard::NextButton)->setEnabled(true);
    this->button(QWizard::FinishButton)->setEnabled(false);
    if(ui.listView_3->selectionModel()->selectedRows(0).size() > 0){
      // PCA_ CPCA_ PLS_ EPLS_ MLR_ PLS_DA_ EPLS_DA_ LDA_ MLR_
      if(type == PCA_ ||
         type == PLS_ ||
         type == PLS_DA_ ||
         type == EPLS_ ||
         type == EPLS_DA_ ||
         type == MLR_ ||
         type == LDA_){
           if(ui.listView_4->selectionModel()->selectedRows(0).size() > 0){
             if(type == PCA_){
               this->button(QWizard::FinishButton)->setEnabled(true);
             }
             else{
               if(type == PLS_ || type == EPLS_ || type == MLR_){
                 if(ui.listView_5->selectionModel()->selectedRows(0).size() > 0){
                   this->button(QWizard::FinishButton)->setEnabled(true);
                 }
                 else{
                   this->button(QWizard::FinishButton)->setEnabled(false);
                 }
               }
               else if(type == LDA_ || type == PLS_DA_ || type == EPLS_DA_){
                 if(ui.listView_7->model()->rowCount() > 0){
                   this->button(QWizard::FinishButton)->setEnabled(true);
                 }
                 else{
                   this->button(QWizard::FinishButton)->setEnabled(false);
                 }
              }
              else{
                // finish button disabled by default!
                this->button(QWizard::FinishButton)->setEnabled(false);
              }
           }
         }
         else{
           this->button(QWizard::FinishButton)->setEnabled(false);
         }
       }
       //Multiblock methods
       else if(type == CPCA_){
         if(type == CPCA_){
           if(ui.listView_9->model()->rowCount() > 0){
             this->button(QWizard::FinishButton)->setEnabled(true);
           }
           else{
             this->button(QWizard::FinishButton)->setEnabled(false);
           }
         }
         else{
           // finish button disabled by default!
           this->button(QWizard::FinishButton)->setEnabled(false);
         }
       }
       else{
         this->button(QWizard::FinishButton)->setEnabled(false);
       }
    }
    else{
      // finish button disabled by default!
      this->button(QWizard::FinishButton)->setEnabled(false);
    }
  }
}

void ModelDialogWizard::ELmethodChanged(int indx)
{
  if(indx == 0){
    ui.label_2->hide();
    ui.nvarsSpinBox->hide();
    ui.label->show();
    ui.trainSizeSpinBox->show();
  }
  else if(indx == 1){
    ui.label_2->show();
    ui.nvarsSpinBox->show();
    ui.label->hide();
    ui.trainSizeSpinBox->hide();
  }
  else{
    ui.label_2->show();
    ui.nvarsSpinBox->show();
    ui.label->show();
    ui.trainSizeSpinBox->show();
  }
}

void ModelDialogWizard::ObjSelectAll()
{
  ui.listView_3->selectAll();
  EnableDisableButtons();
}

void ModelDialogWizard::ObjInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_3->selectionModel();
  QAbstractItemModel *model = ui.listView_3->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  EnableDisableButtons();
}

void ModelDialogWizard::ObjSelectBy()
{
  if(ui.objSelectByLabel->currentIndex() > 0){
    int labelindex = ui.objSelectByLabel->currentIndex()-1;
    QItemSelection selection;
    for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
      if(projects_->value(selectedproject_)->getObjectLabels()[labelindex].objects.contains(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_3->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_3->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_3->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.objSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.objSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void ModelDialogWizard::ObjUnselectAll()
{
  ui.listView_3->selectionModel()->clear();
  EnableDisableButtons();
}

void ModelDialogWizard::XVarSelectAll()
{
  ui.listView_4->selectAll();
  EnableDisableButtons();
}

void ModelDialogWizard::XVarInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_4->selectionModel();
  QAbstractItemModel *model = ui.listView_4->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);

  EnableDisableButtons();
}

void ModelDialogWizard::XVarSelectBy()
{
  if(ui.xvarSelectByLabel->currentIndex() > 0){
    int labelindex = ui.xvarSelectByLabel->currentIndex()-1;
    QItemSelection selection;
    for(int i = 0; i < ui.listView_4->model()->rowCount(); i++){
      if(projects_->value(selectedproject_)->getVariableLabels()[labelindex].objects.contains(ui.listView_4->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_4->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_4->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_4->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.xvarSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.xvarSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void ModelDialogWizard::XVarUnselectAll()
{
  ui.listView_4->selectionModel()->clear();
  EnableDisableButtons();
}

void ModelDialogWizard::YVarSelectAll()
{
  ui.listView_5->selectAll();
  EnableDisableButtons();
}

void ModelDialogWizard::YVarInvertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView_5->selectionModel();
  QAbstractItemModel *model = ui.listView_5->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  EnableDisableButtons();
}

void ModelDialogWizard::YVarSelectBy()
{
  if(ui.yvarSelectByLabel->currentIndex() > 0){
    int labelindex = ui.yvarSelectByLabel->currentIndex()-1;
    QItemSelection selection;
    for(int i = 0; i < ui.listView_5->model()->rowCount(); i++){
      if(projects_->value(selectedproject_)->getVariableLabels()[labelindex].objects.contains(ui.listView_5->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_5->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_5->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_5->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.yvarSelectByLabel->setCurrentIndex(0);
  }
  else{
    ui.yvarSelectByLabel->setCurrentIndex(0);
    return;
  }
}

void ModelDialogWizard::YVarUnselectAll()
{
  ui.listView_5->selectionModel()->clear();
  EnableDisableButtons();
}

void ModelDialogWizard::setYData(QModelIndex current, QModelIndex previous)
{
  if(current.isValid()){
    ydata = current.row();
  }
}

void ModelDialogWizard::setData(QModelIndex current, QModelIndex previous)
{
  if(current.isValid()){
    selecteddata_ = current.row();
  }
}

void ModelDialogWizard::next()
{
  WindowAdjust();
  if(selecteddata_ > -1 && projects_->keys().contains(selectedproject_) == true && !ui.modelname->text().isEmpty()){
    // Generate Object listview and Variable listview
    tab3->clear();
    tab4->clear();
    tab5->clear();
    tab6->clear();
    tab8->clear();
    for(int i = 0; i < projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName().size(); i++){
      QList<QStandardItem*> row_tab3, row_tab6;
      row_tab3.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName()[i]));
      row_tab6.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getObjName()[i]));
      tab3->appendRow(row_tab3);
      tab6->appendRow(row_tab6);
    }

    for(int i = 1; i < projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName().size(); i++){
      QList<QStandardItem*> xvname, yvname, xblockvname;
      xvname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName()[i]));
      xblockvname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName()[i]));
      yvname.append(new QStandardItem(projects_->value(selectedproject_)->getMatrix(selecteddata_)->getVarName()[i]));
      tab4->appendRow(xvname);
      tab8->appendRow(xblockvname);
      tab5->appendRow(yvname);
    }
    EnableDisableButtons();
  }
  else{
    return;
  }
}

void ModelDialogWizard::OK()
{
  if(selectedproject_ == -1 || selecteddata_ == -1 || ui.modelname->text().isEmpty()){
    QMessageBox::warning(this, tr("Warning!"), tr("No project, data selected or no model name setted!\nPlease check your settings.\n"), QMessageBox::Close);
    reject();
    return;
  }
  else{
    modelname_ = ui.modelname->text();

    if(ui.NPrincipalComponent->value() > (int)projects_->value(selectedproject_)->getMatrix(selecteddata_)->Matrix()->col){
      n_pc = projects_->value(selectedproject_)->getMatrix(selecteddata_)->Matrix()->col;
    }
    else{
      n_pc = ui.NPrincipalComponent->value();
    }

    xscaling = ui.xscalinglist->currentIndex();
    yscaling = ui.yscalinglist->currentIndex();

    elmethod = ui.elmethodComboBox->currentIndex();

    eparm.n_models = (int)ui.nmodelsSpinBox->value();

    if(elmethod == 0){
      eparm.algorithm = Bagging;
      eparm.trainsize = ui.trainSizeSpinBox->value()/100.;
      eparm.r_fix = 0;
    }
    else if(elmethod == 1){
      eparm.algorithm = FixedRandomSubspaceMethod;
      eparm.trainsize = 0;
      eparm.r_fix = ui.nvarsSpinBox->value();
    }
    else{
      eparm.algorithm = BaggingRandomSubspaceMethod;
      eparm.trainsize = ui.trainSizeSpinBox->value()/100.;
      eparm.r_fix = ui.nvarsSpinBox->value();
    }
    
    // Here we have a selection of objects on the panel of objects
    // Plus we have a selection from class list from an external list.
    // We need to be sure that the objects in the final class list are part
    // of the object selected in the first panel!
    if(type == LDA_ || type == PLS_DA_ || type == EPLS_DA_){
      for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
        if(ui.listView_3->selectionModel()->isSelected(ui.listView_3->model()->index(i, 0)) == true){
          QString objname = ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString();
          for(int j = 0; j < classes.size(); j++){
            if(classes[j].objects.contains(objname)){
              objsel.append(objname);
              break;
            }
            else{
              continue;
            }
          }
        }
        else{
          continue;
        }
      }
    }
    else{
      for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
        if(ui.listView_3->selectionModel()->isSelected(ui.listView_3->model()->index(i, 0)) == true){
          objsel.append(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString());
        }
        else{
          continue;
        }
      }
    }

    for(int i = 0; i < ui.listView_4->model()->rowCount(); i++){
      if(ui.listView_4->selectionModel()->isSelected(ui.listView_4->model()->index(i, 0)) == true){
        xvarsel.append(ui.listView_4->model()->index(i, 0).data(Qt::DisplayRole).toString());
      }
      else{
        continue;
      }
    }

    for(int i = 0; i < ui.listView_5->model()->rowCount(); i++){
      if(ui.listView_5->selectionModel()->isSelected(ui.listView_5->model()->index(i, 0)) == true){
        yvarsel.append(ui.listView_5->model()->index(i, 0).data(Qt::DisplayRole).toString());
      }
      else{
        continue;
      }
    }


    if(objsel.size() > 0){
      if(xvarsel.size() > 0){
        if(type == PCA_){
          compute_ = true;
          accept();
        }
        else{
          if(yvarsel.size() > 0 && classes.size() == 0){
            compute_ = true;
            accept();
          }
          else if(yvarsel.size() == 0 && classes.size() >= 2){ // at list a binary classification!
            compute_ = true;
            accept();
          }
          else{
            compute_ = false;
            reject();
          }
        }
      }
      else{
        if(xblocks.size() > 0){
          if(type == CPCA_){
            xscaling = ui.xblockscalinglist->currentIndex();
            compute_ = true;
            accept();
          }
          else{
            compute_ = false;
            reject();
          }
        }
      }
    }
  }
}

ModelDialogWizard::ModelDialogWizard(PROJECTS *projects, int type_, QWidget *parent)
{
  ui.setupUi(this);

  type = type_;
  if(type == PCA_){
    setWindowTitle("Compute PCA");
    ui.yScaling->hide();
    ui.YvariableGroupBox->hide();
    ui.YclassGroupBox->hide();
    ui.XBlockGroupBox->hide();
    ui.ELearningMethodGroupBox->hide();
  }
  else if(type == CPCA_){
    setWindowTitle("Compute CPCA");
    ui.xScaling->hide();
    ui.XvariableGroupBox->hide();
    ui.yScaling->hide();
    ui.YvariableGroupBox->hide();
    ui.YclassGroupBox->hide();
    ui.ELearningMethodGroupBox->hide();
  }
  else if(type == PLS_){
    ui.groupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute PLS Regression");
    ui.YclassGroupBox->hide();
    ui.YvariableGroupBox->show();
    ui.XBlockGroupBox->hide();
    ui.ELearningMethodGroupBox->hide();
  }
  else if(type == PLS_DA_){
    ui.groupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute PLS Discriminant Analysis");
    ui.YclassGroupBox->show();
    ui.YvariableGroupBox->hide();
    ui.XBlockGroupBox->hide();
    ui.ELearningMethodGroupBox->hide();
    ui.yScaling->hide();
  }
  else if(type == EPLS_){
    ui.groupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute Ensemble PLS Regression");
    ui.YclassGroupBox->hide();
    ui.YvariableGroupBox->show();
    ui.XBlockGroupBox->hide();
  }
  else if(type == EPLS_DA_){
    ui.groupBox->setTitle("N. of Latent Variables");
    setWindowTitle("Compute Ensemble PLS Discriminant Analysis");
    ui.YclassGroupBox->show();
    ui.YvariableGroupBox->hide();
    ui.yScaling->hide();
    ui.XBlockGroupBox->hide();
  }
  else if(type == MLR_){
    setWindowTitle("Compute MLR");
    ui.groupBox->hide();
    ui.YclassGroupBox->hide();
    ui.ELearningMethodGroupBox->hide();
    ui.XBlockGroupBox->hide();
  }
  else if(type == LDA_){
    setWindowTitle("Compute LDA");
    ui.groupBox->hide();
    ui.YclassGroupBox->show();
    ui.yvarSelectAllButton->hide();
    ui.yvarSelectByLabel->hide();
    ui.yvarInvertSelectionButton->hide();
    ui.yvarUnselectButton->hide();
    ui.listView_5->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.ELearningMethodGroupBox->hide();
    ui.XBlockGroupBox->hide();
  }

  selecteddata_ = ydata = -1;
  n_pc = ui.NPrincipalComponent->value();
  xscaling = yscaling = 0;
  elmethod = 0;

  compute_ = false;

  projects_ = projects;

  if(projects_->size() > 0){
    selectedproject_ = 0;
  }
  else{
    selectedproject_ = -1;
  }

  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();
  tab3 = new QStandardItemModel();
  tab4 = new QStandardItemModel();
  tab5 = new QStandardItemModel();
  tab6 = new QStandardItemModel();
  tab7 = new QStandardItemModel();
  tab8 = new QStandardItemModel();
  tab9 = new QStandardItemModel();

  ui.listView_1->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.listView_3->setModel(tab3);
  ui.listView_4->setModel(tab4);
  ui.listView_5->setModel(tab5);
  ui.listView_6->setModel(tab6);
  ui.listView_7->setModel(tab7);
  ui.listView_8->setModel(tab8);
  ui.listView_9->setModel(tab9);

 //Fill the table with data
  QList<QStandardItem*> projectsname;
  for(int i = 0; i < projects_->keys().size(); i++){
    int pid = projects_->keys()[i];
    projectsname.append(new QStandardItem(projects_->value(pid)->getProjectName()));
    pids.append(pid);
  }
  tab1->appendColumn(projectsname);


  connect(ui.listView_1->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(genListView(QModelIndex,QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setData(QModelIndex,QModelIndex)));

  connect(ui.objSelectAllButton, SIGNAL(clicked(bool)), SLOT(ObjSelectAll()));
  connect(ui.objInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(ObjInvertSelection()));
  connect(ui.objUnselectButton, SIGNAL(clicked(bool)), SLOT(ObjUnselectAll()));
  connect(ui.objSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(ObjSelectBy()));

  connect(ui.xvarSelectAllButton, SIGNAL(clicked(bool)), SLOT(XVarSelectAll()));
  connect(ui.xvarInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(XVarInvertSelection()));
  connect(ui.xvarUnselectButton, SIGNAL(clicked(bool)), SLOT(XVarUnselectAll()));
  connect(ui.xvarSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(XVarSelectBy()));

  connect(ui.yvarSelectAllButton, SIGNAL(clicked(bool)), SLOT(YVarSelectAll()));
  connect(ui.yvarInvertSelectionButton, SIGNAL(clicked(bool)), SLOT(YVarInvertSelection()));
  connect(ui.yvarUnselectButton, SIGNAL(clicked(bool)), SLOT(YVarUnselectAll()));
  connect(ui.yvarSelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(YVarSelectBy()));

  connect(ui.modelname, SIGNAL(textChanged(QString)), SLOT(EnableDisableButtons()));
  connect(ui.listView_1->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_2->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_3->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_4->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_5->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_6->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_7->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_8->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));
  connect(ui.listView_9->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButtons()));

  connect(ui.class_SelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(ClassByLabel()));
  connect(ui.class_import, SIGNAL(clicked(bool)), SLOT(importClass()));
  connect(ui.class_addButton, SIGNAL(clicked(bool)), SLOT(addClass()));
  connect(ui.class_removeButton, SIGNAL(clicked(bool)), SLOT(removeClass()));

  connect(ui.xblock_SelectByLabel, SIGNAL(currentIndexChanged(int)), SLOT(BlockByLabel()));
  connect(ui.xblock_import, SIGNAL(clicked(bool)), SLOT(importBlock()));
  connect(ui.xblock_addButton, SIGNAL(clicked(bool)), SLOT(addBlock()));
  connect(ui.xblock_removeButton, SIGNAL(clicked(bool)), SLOT(removeBlock()));

  connect(ui.elmethodComboBox, SIGNAL(currentIndexChanged(int)), SLOT(ELmethodChanged(int)));

  connect(this->button(QWizard::NextButton), SIGNAL(clicked()), this, SLOT(next()));
  connect(this->button(QWizard::FinishButton),SIGNAL(clicked()), this, SLOT(OK()));

  this->button(QWizard::NextButton)->setEnabled(false);
  this->button(QWizard::FinishButton)->setEnabled(false);
  ELmethodChanged(0);
  WindowAdjust();
}

ModelDialogWizard::~ModelDialogWizard()
{
  delete tab1;
  delete tab2;
  delete tab3;
  delete tab4;
  delete tab5;
  delete tab6;
  delete tab7;
  delete tab8;
  delete tab9;
}
