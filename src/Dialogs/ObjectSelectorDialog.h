#ifndef OBJECTSELECTORDIALOG_H
#define OBJECTSELECTORDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringList>
#include <QString>
#include "qsmdata.h"

#include "ui_ObjectSelectorDialog.h"

class ObjectSelectorDialog: public QDialog
{
  Q_OBJECT

public:
  enum {
    MenuSelection = 0,
    PlotSelection = 1
  };
  
  ObjectSelectorDialog(QList<MATRIX*> *mxlst, LABELS *objlabels, LABELS *varlabels, int windowtype);
  ObjectSelectorDialog(QStringList& currentvariables, LABELS *objlabels, LABELS *varlabels);
  int getSelectionType();
  QString getLabel();
  int getMetric();
  QStringList getVariableList(); 
  int getNumberOfObjects();
  
  int getDataType(); /*0 for current plot coordinates; 1 for Source Data*/
  int getDataID();

private slots:
  void EnableDisable();
  void setVariablesView(QModelIndex current);
  void SelectAll();
  void InvertSelection();
  void Unselect();
  void SelectByVarLabel();
  void OK();
  
private:
  Ui::ObjectSelectorDialog ui;
  int windowtype;
  LABELS *objlabels_, *varlabels_;
  int dataindex;
  QList<MATRIX*> *mxlst_;
};

#endif

