#include "ClassDialog.h"
#include "addLabelDialog.h"

#include <QString>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>
int ClassDialog::ClassesHaveLabelName(QString label)
{
  for(int i = 0; i < (*classes).size(); i++)
    if((*classes)[i].name.compare(label) == 0)
      return i;
    else
      continue;

  return -1;
}

void ClassDialog::Import()
{
  QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.csv)"));
  
  if(!fileName.isEmpty()){
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    
    QAbstractItemModel *model = ui.listView->model();
    
    QTextStream in(&file);
    while(!in.atEnd()){
      QString line = in.readLine();
      QStringList linesplit = line.split(";");
      if(linesplit.size() < 2)
        continue;
      else{
        // linesplit[0]; objname
        //linesplit[1]; Classname
        int indx = ClassesHaveLabelName(linesplit[1]);
        if(indx > -1){
          // Class found then add object if exist
          for(int i = 0; i < model->rowCount(); i++){
            if(model->index(i, 0).data(Qt::DisplayRole).toString().compare(linesplit[0]) == 0){
              (*classes)[indx].objlst.append(linesplit[0]);
              model->removeRow(i);
              break;
            }
            else{
              continue;
            }
          }
        }
        else{
          //Class not found then if object exist add class.
          for(int i = 0; i < model->rowCount(); i++){
            if(model->index(i, 0).data(Qt::DisplayRole).toString().compare(linesplit[0]) == 0){
              (*classes).append(CLASSES());
              (*classes).last().objlst.append(linesplit[0]);
              (*classes).last().name = linesplit[1];
              QList<QStandardItem*> row;
              row.append(new QStandardItem(linesplit[1]));
              tab2->appendRow(row);
              model->removeRow(i);
              break;
            }
            else{
              continue;
            }
          }
        }
      }
    }
  }
  else
    return;
}


void ClassDialog::Add()
{
  addLabelDialog groupnamedialog("Class Name");
  if(groupnamedialog.exec() == QDialog::Accepted){
    QModelIndexList selected = ui.listView->selectionModel()->selectedRows();
    if(selected.size() > 0){
      (*classes).append(CLASSES());
      (*classes).last().name = groupnamedialog.getLabel();
      
      QList<QStandardItem*> row;
      row.append(new QStandardItem(groupnamedialog.getLabel()));
      tab2->appendRow(row);
      
      for(int i = 0; i < selected.size(); i++){
        QString obj = selected[i].data(Qt::DisplayRole).toString();
       (*classes).last().objlst.append(obj);
      }
      
      // Remove selected index from the view
      QModelIndexList indexes;
      while((indexes = ui.listView->selectionModel()->selectedIndexes()).size()) { 
        ui.listView->model()->removeRow(indexes.first().row()); 
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

void ClassDialog::Remove()
{
  QModelIndexList indexes = ui.listView_2->selectionModel()->selectedIndexes();
  
  if(indexes.size() < 1)
    return;
  
  for(int i = 0; i < indexes.size(); i++){
    int indx = indexes[i].row();
    if(indx > -1 && indx < (*classes).size()){
      for(int i = 0; i < (*classes)[indx].objlst.size(); i++){
        QList<QStandardItem*> mname;
        mname.append(new QStandardItem((*classes)[indx].objlst[i]));
        tab1->appendRow(mname);
      }
      (*classes).removeAt(indx);
      ui.listView_2->model()->removeRow(indx);
    }
    else{
      return;
    }
  }
}

ClassDialog::ClassDialog(QStringList objenames, QList <CLASSES> *classes_)
{
  ui.setupUi(this);
  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();
  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);
  
  classes = classes_;
  
  tab1->clear();
  tab2->clear();
  QStringList objtoadd = objenames;
  for(int i = 0; i < (*classes_).size(); i++){
    for(int j = 0; j < (*classes_)[i].objlst.size(); j++){
      int indx = objtoadd.indexOf((*classes_)[i].objlst[j]);
      if(indx > 0)
        objtoadd.removeAt(indx);
    }
    QList<QStandardItem*> classname;
    classname.append(new QStandardItem((*classes_)[i].name));
    tab2->appendRow(classname);
  }
  
  for(int i = 0; i < objtoadd.size(); i++){
    QList<QStandardItem*> mname;
    mname.append(new QStandardItem(objtoadd[i]));
    tab1->appendRow(mname);
  }
  
  
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(accept()));
  connect(ui.addButton, SIGNAL(clicked()), SLOT(Add()));
  connect(ui.removeButton, SIGNAL(clicked()), SLOT(Remove()));
  connect(ui.importButton, SIGNAL(clicked()), SLOT(Import()));
}

ClassDialog::~ClassDialog()
{
  delete tab1;
  delete tab2;
}
