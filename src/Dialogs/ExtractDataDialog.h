#ifndef EXTRACTDATADIALOG_H
#define EXTRACTDATADIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "ui_ExtractDataDialog.h"
#include "qsmdata.h"
#include "qstudiometricstypes.h"
#include "GenericProgressDialog.h"

class ExtractDataDialog : public QDialog
{
  Q_OBJECT
  
public:  
  explicit ExtractDataDialog(PROJECTS *projects);
  ~ExtractDataDialog();
  
  int getProjectID(){ return pid; }
  int getDataType(){ return type; }
  MATRIX *getMatrix(){ return mx; }

private slots:
  void ObjectsSelectAll();
  void ObjectsInvertSelection();
  void ObjectsSelectBy();
  void ObjectsUnselect();
  void VariablesSelectAll();
  void VariablesInvertSelection();
  void VariablesSelectBy();
  void VariablesUnselect();
  void UpdateSelectedObjectCounter();
  void UpdateSelectedVariableCounter();
  void OK();
  void genListView();
  void setProjectID(QModelIndex current);
  void genObjVarView(QModelIndex current);
  
private:
  Ui::ExtractDataDialog ui;
  
  PROJECTS *projects_;
  QList<int> pids;
  QStringList hash;
  QString selectedhash_;
  int pid;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4;
  int type;
  
  MATRIX *mx;
  
  GenericProgressDialog pdialog;
  bool stoprun;
  void StartSelectionRun();
  void StopSelectionRun();
};

#endif
