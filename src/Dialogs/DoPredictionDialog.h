#ifndef DOPREDICTIONDIALOG_H
#define DOPREDICTIONDIALOG_H

#include <QDialog>
#include <QString>
#include <QStandardItemModel>

#include "ui_DoPredictionDialog.h"
#include "qsmdata.h"
#include "qstudiometricstypes.h"

class DoPredictionDialog : public QDialog
{
  Q_OBJECT
  
public:
  DoPredictionDialog(PROJECTS *projects, int type_); // 0 pca, 1 pls, 2 upca, 3 upls
  ~DoPredictionDialog();
  
  bool compute(){ return compute_; }
  int getselectedProject(){ return selectedproject_; }
  QString getPredictionName(){ return ui.predname->text(); }
  int getselectedData(){ return selecteddata_; }
  int getselectedYData(){ return selectedydata_; }
  int getselectedModel(){ return selectedmodel_; }
  QStringList getObjectSelected(){ return objsel; }
  QStringList getYVariableSelected(){ return yvarsel; }
  
private slots:
  void previous();
  void next();
  void OK();
  void setProject(QModelIndex current);
  void setselectedModel(QModelIndex current);
  void setselectedData(QModelIndex current);
  void setselectedYData(QModelIndex current); // used for upls prediction
  void EnableOKButton();
  
  void ObjSelectAll();
  void ObjInvertSelection();
  void ObjSelectBy();
  void ObjUnselectAll();
  
  void VarSelectAll();
  void VarInvertSelection();
  void VarSelectBy();
  void VarUnselectAll();
  
private:
  Ui::DoPredictionDialog ui;
  int state;
  
  PROJECTS *projects_;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5, *tab6;
  QList<int> pids;
  QList<int> mids;
  QStringList objsel, yvarsel;
  int selectedproject_, selecteddata_, selectedydata_, selectedmodel_;
  int type; // 0 pca, 1 pls, 2 upca, 3 upls
  bool compute_;
  
  void CheckDataForPrediction();
};

#endif
