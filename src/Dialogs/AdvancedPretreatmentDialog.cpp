#include "AdvancedPretreatmentDialog.h"
#include <scientific.h>

void AdvancedPretreatmentDialog::OK()
{
 // Pretreat and Exit
  QStringList varnamestomod;

  for(int i = 0; i < ui.listView_3->selectionModel()->selectedRows().size(); i++){
    varnamestomod.append(ui.listView_3->selectionModel()->selectedRows()[i].data(Qt::DisplayRole).toString());
  }


  size_t nrow = projects->value(pid)->getMatrix(mxid)->Matrix()->row;
  size_t ncol = varnamestomod.size();
  matrix *mx_in;
  NewMatrix(&mx_in, nrow, ncol);

  for(size_t j = 0; j < ncol; j++){
    size_t colindx = projects->value(pid)->getMatrix(mxid)->getVarName().indexOf(varnamestomod[j]) - 1;
    for(size_t i = 0; i < nrow; i++){
      mx_in->data[i][j] = projects->value(pid)->getMatrix(mxid)->Matrix()->data[i][colindx];
    }
  }

  mx->getVarName().append(firstcol_name);
  // Set the object names in the pretreated matrix
  for(size_t i = 0; i < nrow; i++){
    mx->getObjName().append(projects->value(pid)->getMatrix(mxid)->getObjName()[i]);
  }

  matrix *mx_out;
  initMatrix(&mx_out);
  if(ui.logTransform->isChecked()){
    Matrix2LogMatrix(mx_in, mx_out);
  }
  else if(ui.squareTransform->isChecked()){
    Matrix2SquareMatrix(mx_in, mx_out);
  }
  else if(ui.sqrtTransform->isChecked()){
    Matrix2SQRTMatrix(mx_in, mx_out);
  }
  else if(ui.absTransform->isChecked()){
  Matrix2ABSMatrix(mx_in, mx_out);
  }
  else if(ui.rowcenteringScaling->isChecked()){
    MatrixRowCenterScaling(mx_in, mx_out);
  }
  else{
    /*else if(ui.SVNScaling->isChecked()){*/
    MatrixSVNScaling(mx_in, mx_out);
  }

  /* Now MX cpontain the pretreated matrix... */
  if(ui.addNoModVars->isChecked()){
    mx->MatrixResize(projects->value(pid)->getMatrix(mxid)->getObjName().size(), projects->value(pid)->getMatrix(mxid)->getVarName().size()-1);
    for(int j = 1; j < projects->value(pid)->getMatrix(mxid)->getVarName().size(); j++){
      auto indx = varnamestomod.indexOf(projects->value(pid)->getMatrix(mxid)->getVarName()[j]);
      mx->getVarName().append(projects->value(pid)->getMatrix(mxid)->getVarName()[j]);
      if(indx > -1){
        for(size_t i = 0; i < nrow; i++){
          mx->Matrix()->data[i][j-1] = mx_out->data[i][indx];
        }
      }
      else{
        for(size_t i = 0; i < nrow; i++){
          mx->Matrix()->data[i][j-1] = projects->value(pid)->getMatrix(mxid)->Matrix()->data[i][j-1];
        }
      }
    }
  }
  else{
    for(int i = 0; i < varnamestomod.size(); i++)
      mx->getVarName().append(varnamestomod[i]);

    mx->MatrixResize(mx_out->row, mx_out->col);
    for(size_t i = 0; i < mx_out->row; i++)
      for(size_t j = 0; j < mx_out->col; j++)
        mx->Matrix()->data[i][j] = mx_out->data[i][j];
  }

  mx->setName(ui.dataname->text());
  DelMatrix(&mx_in);
  DelMatrix(&mx_out);
  accept();
}

void AdvancedPretreatmentDialog::setMatrixID(QModelIndex current)
{
  if(current.isValid()){
    mxid = current.row();
    if(mxid > -1){
      tab3->clear();
        for(int i = 1; i < projects->value(pid)->getMatrix(mxid)->getVarName().size(); i++){
        QList<QStandardItem*> tab3row;
        tab3row.append(new QStandardItem(projects->value(pid)->getMatrix(mxid)->getVarName()[i]));
        tab3->appendRow(tab3row);
      }
      EnableOKButton();
    }
    else{
      EnableOKButton();
    }
  }
  else{
    EnableOKButton();
    return;
  }
}

void AdvancedPretreatmentDialog::setProjectID(QModelIndex current)
{
  if(current.isValid()){
    pid = pids[current.row()];
    tab2->clear();
    for(int i = 0; i < projects->value(pid)->MatrixCount(); i++){
      QList<QStandardItem*> tab2matrix;
      tab2matrix.append(new QStandardItem(projects->value(pid)->getMatrix(i)->getName()));
      tab2->appendRow(tab2matrix);
    }


    ui.selectByLabelButton->addItem("Select by label...");
    for(int i = 0; i < projects->value(pid)->getVariableLabels().size(); i++){
      ui.selectByLabelButton->addItem(projects->value(pid)->getVariableLabels()[i].name);
    }

  }
  else{
    pid = -1;
  }
  EnableOKButton();
}

void AdvancedPretreatmentDialog::EnableOKButton()
{
  if(pid > -1
    && mxid > -1
    && !ui.dataname->text().isEmpty()
    && ui.listView_3->selectionModel()->selectedRows().size() > 0){
    ui.okButton->setEnabled(true);
  }
  else{
    ui.okButton->setEnabled(false);
  }
}

void AdvancedPretreatmentDialog::SelectAllVars()
{
  ui.listView_3->selectAll();
  EnableOKButton();
}

void AdvancedPretreatmentDialog::InvertVarSelection()
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
  EnableOKButton();
}

void AdvancedPretreatmentDialog::UnselectAllVars()
{
  ui.listView_3->selectionModel()->clear();
  EnableOKButton();
}

void AdvancedPretreatmentDialog::SelectVarsBy(int indx)
{
  if(pid > -1 && mxid > -1){
    QItemSelection selection;
    for(int i = 0; i < ui.listView_3->model()->rowCount(); i++){
      if(projects->value(pid)->getVariableLabels()[indx].objects.contains(ui.listView_3->model()->index(i, 0).data(Qt::DisplayRole).toString()) == true){
        QModelIndex topLeft = ui.listView_3->model()->index(i, 0);
        QModelIndex bottomRight = ui.listView_3->model()->index(i, 0);
        selection << QItemSelectionRange(topLeft, bottomRight);
      }
      else{
        continue;
      }
    }
    ui.listView_3->selectionModel()->select(selection, QItemSelectionModel::Select);
    ui.selectByLabelButton->setCurrentIndex(0);

  }
}

AdvancedPretreatmentDialog::AdvancedPretreatmentDialog (PROJECTS *projects_)
{
  ui.setupUi(this);
  projects = projects_;
  mx = new MATRIX();
  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();
  tab3 = new QStandardItemModel();
  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);
  ui.listView_3->setModel(tab3);

  QList<QStandardItem*> projectsname;
  for(int i = 0; i < projects->keys().size(); i++){
    projectsname.append(new QStandardItem(projects->value(projects->keys()[i])->getProjectName()));
    pids.append(projects->keys()[i]);
  }
  tab1->appendColumn(projectsname);

  connect(ui.listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setProjectID(QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setMatrixID(QModelIndex)));
  connect(ui.listView_3->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(EnableOKButton()));

  connect(ui.selectAllButton, SIGNAL(clicked()), SLOT(SelectAllVars()));
  connect(ui.invertSelectionButton, SIGNAL(clicked()), SLOT(InvertVarSelection()));
  connect(ui.unselectButton, SIGNAL(clicked()), SLOT(UnselectAllVars()));
  connect(ui.selectByLabelButton, SIGNAL(currentIndexChanged(int)), SLOT(SelectVarsBy(int)));

  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.dataname, SIGNAL(textChanged(QString)), SLOT(EnableOKButton()));
  connect(ui.okButton, SIGNAL(clicked()), SLOT(OK()));

  EnableOKButton();
}

AdvancedPretreatmentDialog::~AdvancedPretreatmentDialog()
{
  delete tab1;
  delete tab2;
  delete tab3;
  delete mx;

}
