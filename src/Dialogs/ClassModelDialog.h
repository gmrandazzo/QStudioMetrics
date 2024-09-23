// ClassModelDialog.h

#ifndef CLASSMODELDIALOG_H
#define CLASSMODELDIALOG_H

#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QStandardItemModel>
#include <QString>

#include "qsmdata.h"
#include "qstudiometricstypes.h"
#include "ui_ClassModelDialog.h"

class ClassModelDialog : public QDialog {
  Q_OBJECT

public:
  explicit ClassModelDialog(PROJECTS *projects);
  ~ClassModelDialog();

  bool compute() { return compute_; }
  int getselectedProject() { return selectedproject_; }
  QString &getModelName() { return modelname_; }
  int getselectedData() { return selecteddata_; }
  QList<QStringList> getClasses() { return classes; }
  QStringList getNameClasses() { return nameclasses; }
  QStringList getVarSelected() { return varsel; }

private slots:
  void next();
  void previous();
  void OK();
  void genListView(QModelIndex current);
  void setData(QModelIndex current);
  void EnableDisableButtons();

  void AddClass();
  void RemoveClass();

  void ObjSelectAll();
  void ObjInvertSelection();
  void ObjUnselectAll();
  void ObjSelectBy();

  void VarSelectAll();
  void VarInvertSelection();
  void VarUnselectAll();
  void VarSelectBy();

private:
  Ui::ClassModelDialog ui;
  int state;

  PROJECTS *projects_;
  QList<int> pids;
  QList<QStringList> classes;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4, *tab5;
  QStringList nameclasses, varsel;
  QString modelname_;
  int selectedproject_, selecteddata_;
  bool compute_;
};

#endif
