#include "LinePlotStyleDialog.h"

#include <QColorDialog>

void LinePlotStyleDialog::Cancel()
{
  reject();
}

void LinePlotStyleDialog::OK()
{
  accept();
}

int LinePlotStyleDialog::ChangeShape()
{
  if(ui.ShapeGroupBox->isChecked()){
    return 1;
  }
  else{
    return 0;
  }
}

int LinePlotStyleDialog::ChangeColor()
{
  if(ui.ColorGroupBox->isChecked()){
    return 1;
  }
  else{
    return 0;
  }
}

int LinePlotStyleDialog::ChangeSize()
{
  if(ui.SizeGroupBox->isChecked()){
    return 1;
  }
  else{
    return 0;
  }
}

void LinePlotStyleDialog::SetLineType()
{
  ltype = ui.actionSelectShape->currentIndex();
}

void LinePlotStyleDialog::SetLineColor()
{
  color = QColorDialog::getColor(color, this);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
}

void LinePlotStyleDialog::SetLineSize()
{
  lsize = ui.actionSelectSize->value();
}

LinePlotStyleDialog::LinePlotStyleDialog(): QDialog(0)
{
  ui.setupUi(this);
  
  color = QColor(Qt::red);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color.red()).arg(color.green()).arg(color.blue()));
  
  lsize = ui.actionSelectSize->value();
  ltype = ui.actionSelectShape->currentIndex();
  
  connect(ui.actionSelectShape, SIGNAL(currentIndexChanged(int)), SLOT(SetLineType()));
  connect(ui.actionSelectColor, SIGNAL(clicked(bool)), SLOT(SetLineColor()));
  connect(ui.actionSelectSize, SIGNAL(valueChanged(int)), SLOT(SetLineSize()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
}
