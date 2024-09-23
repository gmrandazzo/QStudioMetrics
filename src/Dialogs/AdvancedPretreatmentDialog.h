#ifndef ADVANCEDPRETREATMENTDIALOG_H
#define ADVANCEDPRETREATMENTDIALOG_H

#include <QDialog>
#include <QStandardItemModel>

#include "qsmdata.h"
#include "ui_AdvancedPretreatmentDialog.h"

class AdvancedPretreatmentDialog : public QDialog {
  Q_OBJECT

public:
  explicit AdvancedPretreatmentDialog(PROJECTS *projects_);
  ~AdvancedPretreatmentDialog();
  MATRIX *getMergedMatrix() { return mx; }
  int getProjectID() { return pid; }
private slots:
  void OK();
  void EnableOKButton();
  void setMatrixID(QModelIndex current);
  void setProjectID(QModelIndex current);
  void SelectAllVars();
  void InvertVarSelection();
  void UnselectAllVars();
  void SelectVarsBy(int current);

private:
  Ui::AdvancedPretreatmentDialog ui;
  PROJECTS *projects;
  MATRIX *mx;
  QStandardItemModel *tab1, *tab2, *tab3;
  QList<int> pids;
  int pid, mxid;
};

#endif
