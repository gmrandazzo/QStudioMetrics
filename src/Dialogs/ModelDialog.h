#ifndef MODELDIALOG_H
#define MODELDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QString>
#include <QDialogButtonBox>
#include <QStandardItemModel>

#include "ui_ModelDialog.h"
#include "qsmdata.h"
#include "qstudiometricstypes.h"

class ModelDialog : public QDialog
{
  Q_OBJECT
  
public:
  ModelDialog(PROJECTS *projects, int type_);
  ~ModelDialog();
  
  bool compute(){ return compute_; }
  int getselectedProject(){ return selectedproject_; }
  QString& getModelName(){ return modelname_; }
  int getselectedData(){ return selecteddata_; }
  int getselectedYData(){ return ydata; }
  QStringList getObjectSelected(){ return objsel; }
  QStringList getXVarSelected(){ return xvarsel; }
  QStringList getYVarSelected(){ return yvarsel; }
  int getXScalingType(){ return xscaling; }
  int getYScalingType(){ return yscaling; }
  int getNumberOfComponent(){ return n_pc; }
  
private slots:
  void next();
  void previous();
  void OK();
  void genListView(QModelIndex current);
  void setData(QModelIndex current);
  void setYData(QModelIndex current);
  void EnableOKButton();
  
  void ObjSelectAll();
  void ObjInvertSelection();
  void ObjUnselectAll();
  void ObjSelectBy();
  
  void XVarSelectAll();
  void XVarInvertSelection();
  void XVarUnselectAll();
  void XVarSelectBy();
  
  void YVarSelectAll();
  void YVarInvertSelection();
  void YVarUnselectAll();
  void YVarSelectBy();
private:
  Ui::ModelDialog ui;
  int state;
  
  PROJECTS *projects_;
  QList<int> pids;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5, *tab6;
  QStringList objsel, xvarsel, yvarsel;
  QString modelname_;
  int selectedproject_;
  int selecteddata_, ydata; // ydata for UPLS
  int type; // 0 PLS
  bool compute_;
  int xscaling, yscaling;
  uint n_pc; // Number of Principal Component
};

#endif
