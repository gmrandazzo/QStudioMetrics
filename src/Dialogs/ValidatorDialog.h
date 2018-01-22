#ifndef VALIDATORDIALOG_H
#define DoPLSUPLSValidationDialog_H

#include <QDialog>
#include <QPushButton>
#include <QString>
#include <QDialogButtonBox>
#include <QStandardItemModel>

#include "ui_ValidatorDialog.h"
#include "qsmdata.h"
#include "qstudiometricstypes.h"

#include "ClassDialog.h"

class ValidatorDialog : public QDialog
{
  Q_OBJECT

public:
  ValidatorDialog(PROJECTS *projects, int type_);
  ~ValidatorDialog();

  bool compute(){ return compute_; }
  int getselectedProject(){ return selectedproject_; }
  int getNumberOfGroup(){ return ngroup; }
  int getNumberOfIteration(){ return niter; }
  int getModelID(){ return modelid; }
  int getValidType(){ return validtype; }
  bool ModelYScrambling(){ return yscrambling; };
  int getYSCramblingBlock(){ return block; }

private slots:
  void OK();
  void setProject(QModelIndex current);
  void setModelID(QModelIndex current);
  void setValidationType();
  void setNIterations();
  void setNGroup();
  void setYScrambling();
  void setYScramblingBlock();

private:
  Ui::ValidatorDialog ui;

  PROJECTS *projects_;
  QList<int> pids;
  QList<int> mids;
  QStandardItemModel *tab1, *tab2;
  int selectedproject_;
  int type; // 0 PLS, 1 UPLS
  int validtype; // 0 for cross validation 1 for Leave One Out
  bool compute_, yscrambling;
  int modelid, ngroup, niter, block; // Number of Principal Component

};

#endif
