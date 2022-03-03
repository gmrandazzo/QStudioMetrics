//PlotSelectionStyleDialog.h
#ifndef SELECTIONSTYLEDIALOG_H
#define SELECTIONSTYLEDIALOG_H

#include <scientific.h>
#include <QDialog>
#include <QColor>

#include "ui_SelectionStyleDialog.h"
#include "qsmdata.h"


class SelectionStyleDialog: public QDialog
{
  Q_OBJECT

public:
  enum ChangeType { NOCHANGE, FIXED, GRADIENT };

  SelectionStyleDialog();
  SelectionStyleDialog(QStringList &varname);
  void setSymbolNames(QStringList);
  
  int ChangeShape();
  ChangeType ChangeColor(); // 0 is no color change, 1 is solid change color, 2 is from min to max change color
  ChangeType ChangeSize(); // 0 is no size change, 1 is solid change size, 2 is from min to max change size

  int getSymbolType(); // 0 = circle; 1 = square; 2 = triangle;
  QColor& getSolidSymbolColor();
  QColor& getMinSymbolColor();
  QColor& getMaxSymbolColor();
  QString getVariableSymbolColor();

  int getFixedSymbolSize();
  int getMinSymbolSize();
  int getMaxSymbolSize();
  QString getVariableSymbolSize();

public slots:
  void Cancel();
  void OK();
  void SetSolidSymbolColor();
  void SetMinSymbolColor();
  void SetMaxSymbolColor();
  void CheckUncheck();

private:
  Ui::SelectionStyleDialog ui;
  QStringList varnames;
  QColor color1, color2, color3;
};

#endif
