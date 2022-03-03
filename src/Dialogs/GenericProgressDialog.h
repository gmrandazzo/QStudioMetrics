#ifndef GENERICPROGRESSDIALOG_H
#define GENERICPROGRESSDIALOG_H

#include <QDialog>

#include "ui_GenericProgressDialog.h"

class GenericProgressDialog: public QDialog
{
  Q_OBJECT

public:
  GenericProgressDialog();
  void setRange(int min, int max);
  void setMin(int min);
  void setMax(int max);
  void setValue(int value);
  
  void showcancel();
  void hideCancel();
  
private slots:
  void Cancel();

signals:
  void runCancelled();
  
private:
  Ui::GenericProgressDialog ui;

};

#endif

