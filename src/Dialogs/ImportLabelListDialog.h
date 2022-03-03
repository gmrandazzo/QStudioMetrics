#ifndef IMPORTLABELLISTDIALOG_H
#define IMPORTLABELLISTDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_ImportLabelListDialog.h"

class ImportLabelListDialog: public QDialog
{
  Q_OBJECT

public:
  ImportLabelListDialog(QString path_);
  QString getFileName();
  QString getLabel();
  void setPath(QString &path_){ path = path_; }
  
private slots:
  void Open();
  
private:
  Ui::ImportLabelListDialog ui;
  QString path;
};

#endif

