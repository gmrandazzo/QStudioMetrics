#ifndef MODELDIALOGWIZARD_H
#define MODELDIALOGWIZARD_H

#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QString>
#include <QWizard>

#include "qsmdata.h"
#include "qstudiometricstypes.h"
#include "ui_ModelDialogWizard.h"

class ModelDialogWizard : public QWizard {
  Q_OBJECT

public:
  ModelDialogWizard(PROJECTS *projects, int type_, QWidget *parent = 0);
  ~ModelDialogWizard();

  bool compute() { return compute_; }
  int getselectedProject() { return selectedproject_; }
  QString &getModelName() { return modelname_; }
  int getselectedData() { return selecteddata_; }
  int getselectedYData() { return ydata; }
  QStringList getObjectSelected() { return objsel; }
  QStringList getXVarSelected() { return xvarsel; }
  LABELS getBlockXVarSelected() { return xblocks; }
  QStringList getYVarSelected() { return yvarsel; }
  LABELS getClasses() { return classes; }
  int getXScalingType() { return xscaling; }
  int getYScalingType() { return yscaling; }
  int getNumberOfComponent() { return n_pc; }
  ELearningParameters getElearningParm() { return eparm; }

private slots:
  void next();
  void OK();
  void genListView(QModelIndex current, QModelIndex previous);
  void setData(QModelIndex current, QModelIndex previous);
  void setYData(QModelIndex current, QModelIndex previous);
  void ClassByLabel();
  void importClass();
  void addClass();
  void removeClass();

  void BlockByLabel();
  void importBlock();
  void addBlock();
  void removeBlock();
  void EnableDisableButtons();

  void ELmethodChanged(int);

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
  Ui::ModelDialogWizard ui;
  PROJECTS *projects_;
  ELearningParameters eparm;
  QList<int> pids;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5, *tab6, *tab7, *tab8,
      *tab9;
  QStringList objsel, xvarsel, yvarsel;
  QString modelname_;
  int selectedproject_;
  int selecteddata_, ydata; // ydata for UPLS
  int type;                 // 0 PLS
  int elmethod;
  bool compute_;
  int xscaling, yscaling;
  uint n_pc; // Number of Principal Component
  LABELS classes;
  LABELS xblocks;
  int CheckLabelNameInLabels(QString label, LABELS lbl);
  int CheckNameInLabel(QString name, LABEL lbl);
  void AddObject2Class(QString class_label, QString objname);
  void AddVariable2Block(QString block_name, QString varname);
  void WindowAdjust();
};

#endif
