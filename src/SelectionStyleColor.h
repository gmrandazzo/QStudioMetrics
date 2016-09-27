#ifndef SELECTIONSTYLECOLOR_H
#define SELECTIONSTYLECOLOR_H

#include <scientific.h>
#include <QDialog>
#include <QColor>

#include "ui_SelectionStyleColor.h"
#include "data.h"

#include <QDebug>

class SelectionStyleColor: public QDialog
{
  Q_OBJECT
  
public:
  SelectionStyleColor();
  SelectionStyleColor(QStringList &xvarname_, QStringList &yvarname_);
  SelectionStyleColor(QStringList &xvarname_, QStringList &yvarname_, int &xlevels_, int &ylevels_); // used for array multiway where could be more than one layer
  
  int ChangeColorType();
  
  QColor& getSolidSymbolColor(){ return color1; }
  int getVariableTypeSymbolColor(){ return varcolor; }
  QColor& getMinSymbolColor(){ return color2; }
  QColor& getMaxSymbolColor(){ return color3; }
  int getVariableSymbolColor(){ return cvar; }
  int getVariableLevelSymbolColor(){ return clevel; }
  
public slots:
  void Cancel();
  void OK();
  void SetSolidSymbolColor();
  void SetVariableTypeSymbolColor();
  void SetMinSymbolColor();
  void SetMaxSymbolColor();
  void SetVariableSymbolColor();
  void SetVariableLevelSymbolColor();
  void CheckUncheck();

private:
  Ui::SelectionStyleColor ui;
  QStringList xvarname, yvarname;
  int xlevels, ylevels;
  QColor color1, color2, color3;
  int cvar, varcolor, clevel;
};

#endif

