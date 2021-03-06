#ifndef MERGEDATADIALOG_H
#define MERGEDATADIALOG_H


#include <QDialog>
#include <QStandardItemModel>

#include "ui_MergeDataDialog.h"
#include "qsmdata.h"


class MergeDataDialog : public QDialog
{
  Q_OBJECT
  
public:
  MergeDataDialog(PROJECTS *projects_);
  ~MergeDataDialog();
  MATRIX *getMergedMatrix(){ return mx; }
  int getProjectID(){ return pid; }
private slots:
  void OK();
  void EnableOKButton();
  void genListView();
  void setProjectID(QModelIndex current);
  void set_unset_MatrixID(QModelIndex current);
  
private:
  Ui::MergeDataDialog ui;
  PROJECTS *projects;
  MATRIX *mx;
  QStandardItemModel *tab1, *tab2;
  QList <int> pids, mxids;
  int pid;
  void MergeType0();
  void MergeType1();
};


#endif

