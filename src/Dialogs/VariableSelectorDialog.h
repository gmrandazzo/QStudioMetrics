#ifndef VARIABLESELECTORDIALOG_H
#define VARIABLESELECTORDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>
#include "qsmdata.h"

#include "ui_VariableSelectorDialog.h"

class VariableSelectorDialog: public QDialog
{
  Q_OBJECT

public:  
  VariableSelectorDialog(QStringList &objnames, QStringList &variablenames, QList < MATRIX* > m_, QList< int > id_, LABELS *varlabels_);
  VariableSelectorDialog(QStringList &variablenames, LABELS *varlabels_);
  QStringList getSelectedVariables();
  
private slots:
  void Select();
  void InvertSelection();
  void AppendSelection();
  void Unselect();
  void find();
  void OK();
  void EnableDisable();
  void UpdateLabelSelection();
  
private:
  Ui::VariableSelectorDialog ui;
  LABELS *objlabels, *varlabels;
  QList < MATRIX *> m;
  QList < int > id;
    
  void DoSelection(QItemSelection &selection);
  QStringList getObjList();
  int getVariable();
};

#endif

