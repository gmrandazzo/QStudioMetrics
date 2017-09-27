#ifndef SELECTBYDIALOG_H
#define SELECTBYDIALOG_H

#include <QDialog>
#include <QList>
#include <QString>
#include <QStringList>

#include "ui_SelectByDialog.h"
#include "qsmdata.h"

class SelectByDialog: public QDialog
{
  Q_OBJECT

public:
  SelectByDialog(QStringList objname_, matrix* coordinate_, QList<MATRIX*> *mxlst_, QList<int> mxid_, LABELS *objlabel_, LABELS *varlabel_);
  ~SelectByDialog();
  QList< int > GetSelectedObjID(){ return selectedobjid; }
  
private slots:
  void UpdateSelectionCounter();
  void selectButton();
  void invertselectButton();
  void appendselectionButton();
  void unselectButton();
  void SetSelectionList();
  void OpenSelectorButton();
  void FindName();
  void Abort();
  void OK();
  void Cancel();
  
private:
  Ui::SelectByDialog ui;
  QStringList selectedobjname;
  QList< int > selectedobjid;
  QStringList objname;
  matrix* coordinate;
  QList<MATRIX*> *mxlst;
  QList<int> mxid;
  LABELS *objlabel, *varlabel;
  bool append;
  bool abort;
  //Used for other selection...
  int selectiontype, metric, nobjects, datatype, dataid, layerid;
  QString label;
  QStringList varlist;
  
  void SetSelectionView();
  bool CheckIDSmxlst(QList<int> *ids);
  
  void StartSelectionRun();
  void WaitSelectionRun();
  void StopSelectionRun();
};

#endif

