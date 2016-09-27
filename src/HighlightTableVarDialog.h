#ifndef HIGHLIGHTTABLEVARDIALOG_H
#define HIGHLIGHTTABLEVARDIALOG_H

#include <QDialog>
#include <QColor>

#include "ui_HighlightTableVarDialog.h"

#include <QDebug>

extern "C" {
  #include "scientific.h"
}

class HighlightTableVarDialog: public QDialog
{
  Q_OBJECT
  
public:
  HighlightTableVarDialog(QStringList varname);  
  
  int getVariableType(){ return var; }
  QColor& getMinColor(){ return colormin; }
  QColor& getMaxColor(){ return colormax; }
  
public slots:
  void Cancel();
  void OK();
  void SetVariable();
  void SetMinColor();
  void SetMaxColor();

private:
  Ui::HighlightTableVarDialog ui;
  QColor colormin, colormax;
  int var;
};

#endif

