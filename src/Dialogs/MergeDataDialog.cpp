#include "MergeDataDialog.h"

/*
 * Merge along columns
 * This means that the two matrix share the 
 * same columns
 *
 * OR
 * 
 * Merge along rows
 * This means that the matrix do not share same column
 * but share same object names
 */
void MergeDataDialog::OK()
{
 // Merge and Exit
  QStringList objnames;
  QStringList varnames;
  for(int i = 0; i < mxids.size(); i++){
    objnames.append(projects->value(pid)->getMatrix(mxids[i])->getObjName());
    /* we start from 1 because at 0 there is the standard sample name "Objects" */
    for(int j = 1; j < projects->value(pid)->getMatrix(mxids[i])->getVarName().size(); j++){
        varnames.append(projects->value(pid)->getMatrix(mxids[i])->getVarName()[j]);
    }
  }

  objnames.removeDuplicates();
  varnames.removeDuplicates();
  

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
  for(int i = 0; i < objnames.size(); i++){
    for(int j = 0; j < varnames.size(); j++){
      for(int k = 0; k < mxids.size(); k++){
        int indxobj = projects->value(pid)->getMatrix(mxids[k])->getObjName().indexOf(objnames[i]);
        if(indxobj > -1){
          int colindx = projects->value(pid)->getMatrix(mxids[k])->getVarName().indexOf(varnames[j])-1;
          if(colindx > -1){
            mx->Matrix()->data[i][j] = projects->value(pid)->getMatrix(mxids[k])->Matrix()->data[indxobj][colindx];
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

  mx->getVarName().append("Object Names");
  mx->getVarName().append(varnames);
  mx->getObjName().append(objnames);
  mx->setName(ui.dataname->text());
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
