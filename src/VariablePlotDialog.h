#ifndef VARIABLEPLOTDIALOG_H
#define VARIABLEPLOTDIALOG_H

#include <QDialog>
#include <QString>
#include <QStandardItemModel>
#include <QStringList>

#include "data.h"

#include "ui_VariablePlotDialog.h"

class VariablePlotDialog: public QDialog
{
  Q_OBJECT

public:
  enum WindowType{
      VariableDistribution = 0,
      VariableVSVariable = 1
  };
  VariablePlotDialog(PROJECTS *projects_, int windowtype_); // used in mainwindow
  ~VariablePlotDialog();

  int getVarType(){ return vtype; }
  int getProjectID(){ return pid; }
  QStringList getDataHash(){ return hash; }
  QList<QStringList> getObjects(){ return objects; }
  int getVariableID1(){ return varid1; }
  int getVariableID2(){ return varid2; }
  QString getVariableName1();
  QString getVariableName2();
  QStringList getObjLabelSelected();
  
private slots:
  void setVariableID1(QModelIndex);
  void setVariableID2(QModelIndex);
  void GenVariableViewAndSetHash();
  void GenDataViewAndLabelView(QModelIndex);
  void getObjectNames();
  void OK();
  void Cancel();
  
private:
  Ui::VariablePlotDialog ui;
  int windowtype;
  int vtype; // 0 matrix; 1 array
  int pid;
  int varid1, varid2;
  QList<uint> pids;
  QStringList hash;
  QStringList labelobjsel;
  QList<QStringList> objects;
  PROJECTS *projects;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5;
  
  bool EnableOKButton();
  bool checkOK();
};

#endif
