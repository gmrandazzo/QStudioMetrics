#ifndef ADDFILEDIALOG_H
#define ADDFILEDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>
// #include "Table.h"

#include "ui_addfileDialog.h"

class AddFileDialog: public QDialog
{
  Q_OBJECT

public:
  AddFileDialog();
  ~AddFileDialog();
  QString getFileName();
  QString getLabel();
  QString getSeparator();
  QString getSkipChar();
  QString getFileObjName();
  QString getFileVarName();
  void setPath(QString &path_){ path = path_; }
  void setSimpleOpen();
  
private slots:
  void Open();
  void OpenFileObjName();
  void OpenFileVarName();
  void Accept();
  void Preview();

  
private:
  Ui::AddFileDialog ui;
  QStandardItemModel* model;
  QString path;
  QStringList ListRead(QString file_);
  auto CountNumberRow(QString file_);
  auto CountNumberColum(QString file_);
};

#endif
