//SaveDialog.h
#ifndef SAVEDIALOG_H
#define SAVEDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QList>
#include <QString>


#include "qsmdata.h"
#include "ui_SaveDialog.h"

class SaveDialog: public QDialog
{
  Q_OBJECT

public:
  explicit SaveDialog(PROJECTS *projects);
  int getProjectID(){ return pid; };
  QString getPathToSave(){ return savepath; };
  void setPath(const QString &path_){ path = path_; }

private slots:
  void setProjectID(QModelIndex current);
  void setSaveIn();
  void OK();
  
private:
  Ui::SaveDialog ui;
  QString path;
  QList<int>pids;
  
  QString savepath;
  int pid;
};

#endif

