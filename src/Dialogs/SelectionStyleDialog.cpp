#include "SelectionStyleDialog.h"

#include <QColorDialog>

void SelectionStyleDialog::CheckUncheck()
{

  if(ui.SizeGroupBox->isChecked() == true){
    ui.actionSymbolType->setEnabled(true);
  }
  else{
    ui.actionSymbolType->setEnabled(false);
  }

  if(ui.ColorGroupBox->isChecked() == true){
    if(ui.solidcolor->isChecked() == true){
      ui.actionSelectColor->setEnabled(true);
      ui.colorLabel->setEnabled(true);
      ui.colorvarlist->setEnabled(false);
      ui.actionSelectColorMin->setEnabled(false);
      ui.actionSelectColorMax->setEnabled(false);

    }
    else{
      ui.actionSelectColor->setEnabled(false);
      ui.colorLabel->setEnabled(false);
      ui.colorvarlist->setEnabled(true);
      ui.actionSelectColorMin->setEnabled(true);
      ui.actionSelectColorMax->setEnabled(true);
    }
  }
  else{
    ui.actionSelectColor->setEnabled(false);
    ui.colorLabel->setEnabled(false);
    ui.colorvarlist->setEnabled(false);
    ui.actionSelectColorMin->setEnabled(false);
    ui.actionSelectColorMax->setEnabled(false);
  }

  if(ui.SizeGroupBox->isChecked() == true){
    ui.ColorGroupBox->setEnabled(true);
    if(ui.fixedsize->isChecked() == true){
      ui.actionSymbolSize->setEnabled(true);
      ui.sizevarlist->setEnabled(false);
      ui.actionSymbolSizeMin->setEnabled(false);
      ui.actionSymbolSizeMax->setEnabled(false);
    }
    else{
      ui.actionSymbolSize->setEnabled(false);
      ui.sizevarlist->setEnabled(true);
      ui.actionSymbolSizeMin->setEnabled(true);
      ui.actionSymbolSizeMax->setEnabled(true);
    }
  }
  else{
    ui.actionSymbolSize->setEnabled(false);
    ui.sizevarlist->setEnabled(false);
    ui.actionSymbolSizeMin->setEnabled(false);
    ui.actionSymbolSizeMax->setEnabled(false);
  }
}

void SelectionStyleDialog::SetMaxSymbolColor()
{
  color3 = QColorDialog::getColor(color3, this);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color3.red()).arg(color3.green()).arg(color3.blue()));
}

void SelectionStyleDialog::SetMinSymbolColor()
{
  color2 = QColorDialog::getColor(color2, this);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color2.red()).arg(color2.green()).arg(color2.blue()));
}

void SelectionStyleDialog::SetSolidSymbolColor()
{
  color1 = QColorDialog::getColor(color1, this);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));
}

void SelectionStyleDialog::Cancel()
{
  reject();
}

void SelectionStyleDialog::OK()
{
  accept();
}

QString SelectionStyleDialog::getVariableSymbolSize()
{

  return ui.sizevarlist->currentText();
}

int SelectionStyleDialog::getMaxSymbolSize()
{
  return ui.actionSymbolSizeMax->value();
}

int SelectionStyleDialog::getMinSymbolSize()
{
  return ui.actionSymbolSizeMin->value();
}

int SelectionStyleDialog::getFixedSymbolSize()
{
  return ui.actionSymbolSize->value();
}

QString SelectionStyleDialog::getVariableSymbolColor()
{
  return ui.colorvarlist->currentText();
}

QColor& SelectionStyleDialog::getMaxSymbolColor()
{
  return color3;
}

QColor& SelectionStyleDialog::getMinSymbolColor()
{
  return color2;
}

QColor& SelectionStyleDialog::getSolidSymbolColor()
{
  return color1;
}

int SelectionStyleDialog::getSymbolType()
{
  return ui.actionSymbolType->currentIndex();
}

SelectionStyleDialog::ChangeType SelectionStyleDialog::ChangeSize()
{
  if(ui.SizeGroupBox->isChecked()){
    if(ui.fixedsize->isChecked()){
      return FIXED;
    }
    else{
      return GRADIENT;
    }
  }
  else{
    return NOCHANGE;
  }
}

SelectionStyleDialog::ChangeType SelectionStyleDialog::ChangeColor()
{
  if(ui.ColorGroupBox->isChecked()){
    if(ui.solidcolor->isChecked()){
      return FIXED;
    }
    else{
      return GRADIENT;
    }
  }
  else{
    return NOCHANGE;
  }
}

int SelectionStyleDialog::ChangeShape()
{
  if(ui.ShapeGroupBox->isChecked()){
    return 1;
  }
  else{
    return 0;
  }
}

SelectionStyleDialog::SelectionStyleDialog(): QDialog()
{
  ui.setupUi(this);
  ui.ShapeGroupBox->hide();

  ui.solidcolor->setChecked(true);
  color1 = QColor(Qt::black);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));
  ui.fixedsize->setChecked(true);
  ui.ShapeGroupBox->setCheckable(false);
  ui.ColorGroupBox->setCheckable(false);
  ui.SizeGroupBox->setCheckable(false);
  ui.solidcolor->hide();
  ui.variablecolor->hide();
  ui.colorvarlist->hide();
  ui.label->hide();
  ui.actionSelectColorMin->hide();
  ui.label_2->hide();
  ui.actionSelectColorMax->hide();
  ui.fixedsize->hide();
  ui.variablesize->hide();
  ui.sizevarlist->hide();
  ui.label_3->hide();
  ui.actionSymbolSizeMin->hide();
  ui.label_4->hide();
  ui.actionSymbolSizeMax->hide();

  adjustSize();

  connect(ui.actionSelectColor, SIGNAL(clicked(bool)), SLOT(SetSolidSymbolColor()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
}

SelectionStyleDialog::SelectionStyleDialog(QStringList &varnames_): QDialog()
{
  ui.setupUi(this);
  ui.ShapeGroupBox->hide();

  ui.solidcolor->setChecked(true);
  color1 = QColor(Qt::black);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));
  color2 = QColor(Qt::green);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color2.red()).arg(color2.green()).arg(color2.blue()));
  color3 = QColor(Qt::blue);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color3.red()).arg(color3.green()).arg(color3.blue()));
  ui.fixedsize->setChecked(true);
  ui.colorvarlist->clear();
  ui.sizevarlist->clear();
  varnames = varnames_;

  for(int i = 0; i < varnames.size(); i++){
    if(varnames[i].compare("Object Names",  Qt::CaseInsensitive) == 0){
      continue;
    }
    else{
      ui.sizevarlist->addItem(varnames[i]);
      ui.colorvarlist->addItem(varnames[i]);
    }
  }

  CheckUncheck();

  adjustSize();

  connect(ui.actionSelectColor, SIGNAL(clicked(bool)), SLOT(SetSolidSymbolColor()));
  connect(ui.actionSelectColorMin, SIGNAL(clicked(bool)), SLOT(SetMinSymbolColor()));
  connect(ui.actionSelectColorMax, SIGNAL(clicked(bool)), SLOT(SetMaxSymbolColor()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));

  connect(ui.ShapeGroupBox, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
  connect(ui.ColorGroupBox, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
  connect(ui.solidcolor, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
  connect(ui.variablecolor, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
  connect(ui.SizeGroupBox, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
  connect(ui.fixedsize, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
  connect(ui.variablesize, SIGNAL(clicked(bool)), SLOT(CheckUncheck()));
}
