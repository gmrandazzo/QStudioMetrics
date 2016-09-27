#include "SearchOnTableDialog.h"
#include <QString>

void SearchOnTableDialog::showForm()
{
  if(ui.selectionlist->currentIndex() == 0){
    ui.variableframe->hide();
    ui.labelframe->show();
  }
  else{
    ui.variableframe->show();
    ui.labelframe->hide();
  }
  
  adjustSize();
}
void SearchOnTableDialog::Ok()
{
  accept();
}

void SearchOnTableDialog::Cancel()
{
  reject();
}

double SearchOnTableDialog::getMin()
{
  return ui.minvaledit->text().toDouble();
}

double SearchOnTableDialog::getMax()
{
  return ui.maxvaledit->text().toDouble();
}

QString SearchOnTableDialog::getLabel()
{
  return ui.lineEdit->text();
}

int SearchOnTableDialog::getSelectionType()
{
  return ui.selectionlist->currentIndex();
}

SearchOnTableDialog::SearchOnTableDialog(QStringList optlist)
{
  ui.setupUi(this);
  
  for(int i = 0; i < optlist.size(); i++){
    ui.selectionlist->addItem(optlist[i]);
  }  
  
  ui.variableframe->hide();
  adjustSize();
  
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(Ok()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.selectionlist, SIGNAL(currentIndexChanged(int)), SLOT(showForm()));
}
