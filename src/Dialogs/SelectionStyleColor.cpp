#include "SelectionStyleColor.h"

#include <QColorDialog>

void SelectionStyleColor::CheckUncheck()
{
  if(ui.solidcolor->isChecked() == true){
    ui.actionSelectColor->setEnabled(true);
    ui.colorLabel->setEnabled(true);
    
    ui.colorvarvaltype->setEnabled(false);
    ui.colorvarlist->setEnabled(false);
    ui.actionSelectColorMin->setEnabled(false);
    ui.actionSelectColorMax->setEnabled(false);
    
    if(xlevels == 0){
      ui.colorvarlevels->hide();
    }
    else{
      ui.colorvarlevels->setEnabled(false);
    }
  }
  else{
    ui.actionSelectColor->setEnabled(false);
    ui.colorLabel->setEnabled(false);
    
    ui.colorvarvaltype->setEnabled(true);
    ui.colorvarlist->setEnabled(true);
    ui.actionSelectColorMin->setEnabled(true);
    ui.actionSelectColorMax->setEnabled(true);
    
    if(xlevels == 0){
      ui.colorvarlevels->hide();
    }
    else{
      ui.colorvarlevels->setEnabled(true);
    }
  }
}

void SelectionStyleColor::SetVariableLevelSymbolColor()
{
  clevel = ui.colorvarlevels->currentIndex();
}

void SelectionStyleColor::SetVariableSymbolColor()
{
  cvar = ui.colorvarlist->currentIndex();
}

void SelectionStyleColor::SetMaxSymbolColor()
{
  color3 = QColorDialog::getColor(color3, this);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color3.red()).arg(color3.green()).arg(color3.blue()));
}

void SelectionStyleColor::SetMinSymbolColor()
{
  color2 = QColorDialog::getColor(color2, this);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color2.red()).arg(color2.green()).arg(color2.blue()));
}

void SelectionStyleColor::SetVariableTypeSymbolColor()
{
  if(ui.colorvarvaltype->currentIndex() == 0){
    ui.colorvarlist->clear();
    for(int i = 0; i < xvarname.size(); i++){
      if(xvarname[i].compare(firstcol_name,  Qt::CaseInsensitive) == 0){
        continue;
      }
      else{
        ui.colorvarlist->addItem(xvarname[i]);
      }
    }
    
    ui.colorvarlevels->clear();
    for(int i = 0; i < xlevels; i++){
      ui.colorvarlevels->addItem(QString::number(i+1));
    }
  }
  else{ // set y
    ui.colorvarlist->clear();
    for(int i = 0; i < yvarname.size(); i++){
      if(yvarname[i].compare(firstcol_name,  Qt::CaseInsensitive) == 0){
        continue;
      }
      else{
        ui.colorvarlist->addItem(yvarname[i]);
      }
    }
    
    ui.colorvarlevels->clear();
    for(int i = 0; i < ylevels; i++){
      ui.colorvarlevels->addItem(QString::number(i+1));
    }
  }
  varcolor = ui.colorvarvaltype->currentIndex();
}

void SelectionStyleColor::SetSolidSymbolColor()
{
  color1 = QColorDialog::getColor(color1, this);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));
}

void SelectionStyleColor::Cancel()
{
  reject();
}

void SelectionStyleColor::OK()
{
  accept();
}

int SelectionStyleColor::ChangeColorType()
{
  if(ui.solidcolor->isChecked()){
    return 0;
  }
  else{
    return 1;
  }
}

SelectionStyleColor::SelectionStyleColor(): QDialog()
{
  ui.setupUi(this);
  
  xlevels = ylevels = 0;

  ui.solidcolor->setChecked(true);
  color1 = QColor(Qt::white);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));
  
  ui.solidcolor->hide();
  ui.variablecolor->hide();
  ui.colorvarlist->hide();
  ui.colorvarvaltype->hide();
  ui.label->hide();
  ui.actionSelectColorMin->hide();
  ui.label_2->hide();
  ui.actionSelectColorMax->hide();
  ui.colorvarlevels->hide();
  
  adjustSize();
  
  connect(ui.actionSelectColor, SIGNAL(clicked(bool)), SLOT(SetSolidSymbolColor()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
}

SelectionStyleColor::SelectionStyleColor(QStringList &xvarname_, QStringList &yvarname_): QDialog()
{
  ui.setupUi(this);
  
  ui.solidcolor->setChecked(true);
  color1 = QColor(Qt::white);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));
  
  color2 = QColor(Qt::green);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color2.red()).arg(color2.green()).arg(color2.blue()));

  color3 = QColor(Qt::blue);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color3.red()).arg(color3.green()).arg(color3.blue()));
  
  cvar = 0;
  
  ui.colorvarlist->clear();
  
  xvarname = xvarname_;
  yvarname = yvarname_;
  
  if(yvarname.size() == 0 )
    ui.colorvarvaltype->hide();
  
  varcolor = 0;
  
  xlevels = ylevels = 0;
  
  for(int i = 0; i < xvarname.size(); i++){
    if(xvarname[i].compare(firstcol_name,  Qt::CaseInsensitive) == 0){
      continue;
    }
    else{
      ui.colorvarlist->addItem(xvarname[i]);
    }
  }
  
  CheckUncheck();
  
  adjustSize();
  
  connect(ui.actionSelectColor, SIGNAL(clicked(bool)), SLOT(SetSolidSymbolColor()));
  connect(ui.colorvarvaltype, SIGNAL(currentIndexChanged(int)), SLOT(SetVariableTypeSymbolColor()));
  connect(ui.actionSelectColorMin, SIGNAL(clicked(bool)), SLOT(SetMinSymbolColor()));
  connect(ui.actionSelectColorMax, SIGNAL(clicked(bool)), SLOT(SetMaxSymbolColor()));
  connect(ui.colorvarlist, SIGNAL(currentIndexChanged(int)), SLOT(SetVariableSymbolColor()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.solidcolor, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
  connect(ui.variablecolor, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
}

SelectionStyleColor::SelectionStyleColor(QStringList& xvarname_, QStringList& yvarname_, int &xlevels_, int &ylevels_): QDialog()
{
  ui.setupUi(this);

  ui.solidcolor->setChecked(true);
  color1 = QColor(Qt::white);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));

  color2 = QColor(Qt::green);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color2.red()).arg(color2.green()).arg(color2.blue()));
  
  color3 = QColor(Qt::blue);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color3.red()).arg(color3.green()).arg(color3.blue()));
  
  cvar = 0;
  
  ui.colorvarlist->clear();
  
  xvarname = xvarname_;
  yvarname = yvarname_;
  
  if(yvarname.size() == 0 )
    ui.colorvarvaltype->hide();
  
  xlevels = xlevels_;
  ylevels = ylevels_;
  varcolor = 0;
  clevel = 0;
  
  for(int i = 0; i < xvarname.size(); i++){
    if(xvarname[i].compare(firstcol_name,  Qt::CaseInsensitive) == 0){
      continue;
    }
    else{
      ui.colorvarlist->addItem(xvarname[i]);
    }
  }
  
  ui.colorvarlevels->clear();
  for(int i = 0; i < xlevels; i++){
    ui.colorvarlevels->addItem(QString::number(i+1));
  }
  
  CheckUncheck();
  
  adjustSize();
  
  connect(ui.actionSelectColor, SIGNAL(clicked(bool)), SLOT(SetSolidSymbolColor()));
  connect(ui.colorvarvaltype, SIGNAL(currentIndexChanged(int)), SLOT(SetVariableTypeSymbolColor()));
  connect(ui.actionSelectColorMin, SIGNAL(clicked(bool)), SLOT(SetMinSymbolColor()));
  connect(ui.actionSelectColorMax, SIGNAL(clicked(bool)), SLOT(SetMaxSymbolColor()));
  connect(ui.colorvarlist, SIGNAL(currentIndexChanged(int)), SLOT(SetVariableSymbolColor()));
  connect(ui.colorvarlevels, SIGNAL(currentIndexChanged(int)), SLOT(SetVariableLevelSymbolColor()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.solidcolor, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
  connect(ui.variablecolor, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
}
