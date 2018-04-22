#ifndef MODELDIALOGWIZARD_H
#define MODELDIALOGWIZARD_H

#include <QWizard>
#include <QPushButton>
#include <QString>
#include <QDialogButtonBox>
#include <QStandardItemModel>

#include "ui_ModelDialogWizard.h"
#include "qsmdata.h"
#include "qstudiometricstypes.h"

class ModelDialogWizard : public QWizard
{
  Q_OBJECT

public:
  ModelDialogWizard(PROJECTS *projects, int type_, QWidget *parent = 0);
  ~ModelDialogWizard();

  bool compute(){ return compute_; }
  int getselectedProject(){ return selectedproject_; }
  QString& getModelName(){ return modelname_; }
  int getselectedData(){ return selecteddata_; }
  int getselectedYData(){ return ydata; }
  QStringList getObjectSelected(){ return objsel; }
  QStringList getXVarSelected(){ return xvarsel; }
  QStringList getYVarSelected(){ return yvarsel; }
  LABELS getClasses(){ return classes; }
  int getXScalingType(){ return xscaling; }
  int getYScalingType(){ return yscaling; }
  int getNumberOfComponent(){ return n_pc; }
  ELearningParameters getElearningParm(){ return eparm; }

private slots:
  void next();
  void OK();
  void genListView(QModelIndex current, QModelIndex previous);
  void setData(QModelIndex current, QModelIndex previous);
  void setYData(QModelIndex current, QModelIndex previous);
  void importClass();
  void addClass();
  void removeClass();
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
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5, *tab6, *tab7;
  QStringList objsel, xvarsel, yvarsel;
  QString modelname_;
  int selectedproject_;
  int selecteddata_, ydata; // ydata for UPLS
  int type; // 0 PLS
  int elmethod;
  bool compute_;
  int xscaling, yscaling;
  uint n_pc; // Number of Principal Component
  LABELS classes;
  int CheckClassLabelAndObject(QString label, QString objectname);
  void WindowAdjust();
};

#endif
