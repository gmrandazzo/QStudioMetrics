#include "MergeDataDialog.h"
// // 
/* 
* Merge along rows
* This means that the matrix do not share same column
* but share same object names
*/

void MergeDataDialog::MergeType0()
{
  // Merge and Exit
  /*Create a map of objectname, matrix id, and row position*/
  QMap<QString, QList<QPair<int, int>>> objmap;
  QStringList varnames;
  for(int i = 0; i < mxids.size(); i++){
    for(int j = 0; j < projects->value(pid)->getMatrix(mxids[i])->getObjName().size(); j++){
      QString objname = projects->value(pid)->getMatrix(mxids[i])->getObjName()[j];
      QPair<int, int> p;
      p.first = i;
      p.second = j;
      objmap[objname].append(p);
    }
    /* we start from 1 because at 0 there is the standard sample name "Objects" */
    for(int j = 1; j < projects->value(pid)->getMatrix(mxids[i])->getVarName().size(); j++){
      varnames.append(projects->value(pid)->getMatrix(mxids[i])->getVarName()[j]);
    }
  }
  
  /*
   * Remove items that do not contains the row object in all the matrix in question
   */
  int i = 0;
  while(i < objmap.size()){
    if(objmap.values()[i].size() == mxids.size()){
      i++;
    }
    else{
      QString key = objmap.keys()[i];
      objmap.remove(key);
    }
  }
  
  mx->MatrixResize(objmap.size(), varnames.size());
  for(int i = 0; i < objmap.size(); i++){
    mx->getObjName() << objmap.keys()[i];
    int c = 0;
    for(int k = 0; k < objmap.values()[i].size(); k++){
      int mxpos = objmap.values()[i][k].first;
      int mxrow = objmap.values()[i][k].second;
      for(int j = 1; j < projects->value(pid)->getMatrix(mxpos)->getVarName().size(); j++){
        mx->Matrix()->data[i][c] = projects->value(pid)->getMatrix(mxpos)->Matrix()->data[mxrow][j-1];
        c+=1;
      }
    }
  }
  
  mx->getVarName().append("Object Names");
  mx->getVarName().append(varnames);
  mx->setName(ui.dataname->text());
}

/*
 * Merge along columns
 * This means that the two matrix share the 
 * same columns
 */
void MergeDataDialog::MergeType1()
{
  QStringList varnames;
  QStringList objnames;
  for(int i = 0; i < mxids.size(); i++){
    objnames << projects->value(pid)->getMatrix(mxids[i])->getObjName();
    /* we start from 1 because at 0 there is the standard sample name "Objects" */
    for(int j = 1; j < projects->value(pid)->getMatrix(mxids[i])->getVarName().size(); j++){
      varnames.append(projects->value(pid)->getMatrix(mxids[i])->getVarName()[j]);
    }
    QApplication::processEvents();
  }
  varnames.removeDuplicates();
  
  QMap<QString, QList<int>> varmap;
  for(int i = 0; i < varnames.size(); i++){
    QList<int> cids;
    for(int j = 0; j < mxids.size(); j++){
      /*-1 because first name of getVarName() is "Object Names" */
      int vindx = projects->value(pid)->getMatrix(mxids[j])->getVarName().indexOf(varnames[i])-1;
      if(vindx > -1){
        cids.append(vindx);
      }
      else{
        break; // Not all matrix contains this column and we can skip
      }
      if(cids.size() == mxids.size()){
        varmap[varnames[i]] = cids;
      }
      else{
        continue;
      }
    }
    QApplication::processEvents();
  }
  
  mx->MatrixResize(objnames.size(), varmap.size());
  int r = 0;
  for(int k = 0; k < mxids.size(); k++){
    for(int i = 0; i < projects->value(pid)->getMatrix(mxids[k])->getObjName().size(); i++){
      for(int j = 0; j < varmap.size(); j++){
        QString key = varmap.keys()[j];
        int c = varmap[key][k];
        mx->Matrix()->data[r][j] = projects->value(pid)->getMatrix(mxids[k])->Matrix()->data[i][c];
      }
      r++;
    }
  }
  
  mx->getObjName().append(objnames);
  mx->getVarName().append("Object Names");
  for(int j = 0; j < varmap.size(); j++){
    mx->getVarName().append(varmap.keys()[j]);
  }
  mx->setName(ui.dataname->text());
}

void MergeDataDialog::OK()
{
  // Merge and Exit
  if(ui.mergematchcol->isChecked()){
    MergeType1();
  }
  else{
    MergeType0();
  }
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
