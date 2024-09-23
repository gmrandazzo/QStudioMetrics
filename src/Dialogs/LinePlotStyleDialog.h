#ifndef LINEPLOTSTYLEDIALOG_H
#define LINEPLOTSTYLEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "ui_LinePlotStyleDialog.h"

class LinePlotStyleDialog : public QDialog {
  Q_OBJECT

public:
  LinePlotStyleDialog();
  int ChangeShape(); // 0 is no color change, 1 change color
  int ChangeColor(); // 0 is no color change, 1 change color
  int ChangeSize();  // 0 is no color change, 1 change color

  int getLineType() { return ltype; }
  QColor getLineColor() { return color; }
  int getLineSize() { return lsize; }

private slots:
  void Cancel();
  void OK();
  void SetLineType();
  void SetLineColor();
  void SetLineSize();

private:
  Ui::LinePlotStyleDialog ui;
  int lsize, ltype;
  QColor color;
};

#endif
