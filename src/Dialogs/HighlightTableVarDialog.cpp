#include "HighlightTableVarDialog.h"
#include <QColorDialog>

void HighlightTableVarDialog::Cancel()
{
  reject();
}

void HighlightTableVarDialog::OK()
{
  accept();
}

void HighlightTableVarDialog::SetMaxColor()
{
  colormax = QColorDialog::getColor(colormax, this);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(colormax.red()).arg(colormax.green()).arg(colormax.blue()));
}

void HighlightTableVarDialog::SetMinColor()
{
  colormin = QColorDialog::getColor(colormin, this);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(colormin.red()).arg(colormin.green()).arg(colormin.blue()));
}

void HighlightTableVarDialog::SetVariable()
{
  var = ui.varlist->currentIndex();
}

HighlightTableVarDialog::HighlightTableVarDialog(QStringList varname): QDialog()
{
  ui.setupUi(this);
  
  var = 0;
  
  colormin = QColor(Qt::green);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(colormin.red()).arg(colormin.green()).arg(colormin.blue()));
  
  colormax = QColor(Qt::red);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(colormax.red()).arg(colormax.green()).arg(colormax.blue()));
  
  for(int i = 0; i < varname.size(); i++){
    if(varname[i].compare("Object Names",  Qt::CaseInsensitive) == 0 || 
      varname[i].compare("Principal Component",  Qt::CaseInsensitive) == 0 || 
      varname[i].compare("Variables",  Qt::CaseInsensitive) == 0 ||
      varname[i].compare("Models Name",  Qt::CaseInsensitive) == 0){
      continue;
    }
    else{
      ui.varlist->addItem(varname[i]);
    }
  }
  
  connect(ui.varlist, SIGNAL(currentIndexChanged(int)), SLOT(SetVariable()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.actionSelectColorMin, SIGNAL(clicked(bool)), SLOT(SetMinColor()));
  connect(ui.actionSelectColorMax, SIGNAL(clicked(bool)), SLOT(SetMaxColor()));
}
