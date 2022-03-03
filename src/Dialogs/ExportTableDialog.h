#ifndef EXPORTTABLEDIALOG_H
#define EXPORTTABLEDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QString>
#include <QStringList>

#include "qsmdata.h"

#include "ui_ExportTableDialog.h"

class ExportTableDialog : public QDialog
{
  Q_OBJECT
  
public:
  ExportTableDialog(QStringList objname, QStringList varname, LABELS *objectlabels_, LABELS *varlabels_);
  
  void setPath(QString path_){ path = path_; }
  QString getFileName();
  QString getSeparator();
  QStringList getSelectedObjects();
  QStringList getSelectedVariables();
  
private slots:
  void SaveAs();
  void EnableOKButton();
  
  void ObjSelectAll();
  void ObjInvertSelection();
  void ObjUnselectAll();
  void ObjSelectBy();
  
  void VarSelectAll();
  void VarInvertSelection();
  void VarUnselectAll();
  void VarSelectBy();
  
private:
  Ui::ExportTableDialog ui;
  QString path;
  LABELS objectlabels;
  LABELS varlabels;
  QString filename;
  QString separator;
  QStringList selected_objects;
  QStringList selected_variables;
};


#endif

