#include "MergeDataDialog.h"


void MergeDataDialog::OK()
{
 // Merge and Exit
  QStringList objnames;
  QStringList varnames;
  for(int i = 0; i < mxids.size(); i++){
    objnames.append(projects->value(pid)->getMatrix(mxids[i])->getObjName());
    varnames.append(projects->value(pid)->getMatrix(mxids[i])->getVarName());
  }
  
  objnames.removeDuplicates();
  varnames.removeDuplicates();
  varnames.removeFirst();
  
  if(ui.mergematchcol->isChecked()){
    QStringList matchvarnames;
    for(int i = 0; i < varnames.size(); i++){
      bool add = true;
      for(int j = 0; j < mxids.size(); j++){
        if(projects->value(pid)->getMatrix(mxids[j])->getVarName().contains(varnames[i]) == true ){
          continue;
        }
        else{
          add = false;
          break;
        }
      }
      
      if(add == true){
        matchvarnames.append(varnames[i]);
      }
      else{
        continue;
      }
    }
    varnames.clear();
    varnames.append(matchvarnames);
  }

  mx->MatrixResize(objnames.size(), varnames.size());
  int cc = 0;
  for(int k = 0; k < objnames.size(); k++){
    for(int i = 0; i < mxids.size(); i++){
      int indxobj = projects->value(pid)->getMatrix(mxids[i])->getObjName().indexOf(objnames[k]);
      if(indxobj > -1){
        for(int j = 0; j < varnames.size(); j++){
          int colindx = projects->value(pid)->getMatrix(mxids[i])->getVarName().indexOf(varnames[j])-1; // -1 because first column is the default column name...
          if(colindx > -1){
            mx->Matrix()->data[cc][j] = projects->value(pid)->getMatrix(mxids[i])->Matrix()->data[indxobj][colindx];
          }
          else
            continue;
        }
        cc++;
        break;
      }
      else{
        continue;
      }
    }
  }
  
  mx->getVarName().append("Object Names");
  mx->getVarName().append(varnames);
  mx->getObjName().append(objnames);
  mx->setName(ui.dataname->text());
  mx->GenHash();
  accept();
}

void MergeDataDialog::genListView()
{
  if(pid != -1){
    tab2->clear();
    if(projects->value(pid)->MatrixCount() > 0){
      for(int i = 0; i < projects->value(pid)->MatrixCount(); i++){
        QList<QStandardItem*> tab2matrix;
        tab2matrix.append(new QStandardItem(projects->value(pid)->getMatrix(i)->getName()));
        tab2->appendRow(tab2matrix);
      }
    }
  }
  else{
    return;
  }
}

void MergeDataDialog::set_unset_MatrixID(QModelIndex current)
{
  if(current.isValid()){
    int indx = mxids.indexOf(current.row());
    if(indx > -1){
      mxids.removeAt(indx);
      EnableOKButton();
    }
    else{
      mxids.append(current.row());
      EnableOKButton();
    }
  }
  else{
    EnableOKButton();
    return;
  }
}

void MergeDataDialog::setProjectID(QModelIndex current)
{
  if(current.isValid()){
    pid = pids[current.row()];
    genListView();
  }
  else{
    pid = -1;
  }
}

void MergeDataDialog::EnableOKButton()
{
  if(mxids.size() > 1 && !ui.dataname->text().isEmpty()){
    ui.okButton->setEnabled(true);
  }
  else{
    ui.okButton->setEnabled(false);
  }
}

MergeDataDialog::MergeDataDialog (PROJECTS *projects_)
{
  ui.setupUi(this);
  projects = projects_;
  mx = new MATRIX();
  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();
  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);
  
  QList<QStandardItem*> projectsname;
  for(int i = 0; i < projects->keys().size(); i++){
    projectsname.append(new QStandardItem(projects->value(projects->keys()[i])->getProjectName()));
    pids.append(projects->keys()[i]);
  }
  tab1->appendColumn(projectsname);
  
  connect(ui.listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setProjectID(QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(set_unset_MatrixID(QModelIndex)));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.dataname, SIGNAL(textChanged(QString)), SLOT(EnableOKButton()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(OK()));
  EnableOKButton();
}

MergeDataDialog::~MergeDataDialog()
{
  delete tab1;
  delete tab2;
  delete mx;

}
