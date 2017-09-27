#ifndef MDICHILD_H
#define MDICHILD_H

#include <QString>
#include <QStandardItem>
#include <QMdiSubWindow>
#include "qsmdata.h"
#include "Table.h"
#include "ListView.h"
#include "ModelInfo.h"

#include <scientific.h>



class MDIChild : public QMdiSubWindow
{
  Q_OBJECT

public:
  MDIChild();

  void newTable(QString& tabname); // used in order to create a new matrix inside the model
  void newTable(QString& tabname, matrix *m); // show in this table an'allocated matrix
  void newTable(QString& tabname, matrix *m, LABELS *objlabels_, LABELS *varlabels_); // show in this table an'allocated matrix
  void newTable(QString& tabname, QList<QStringList> tab, LABELS *objlabels_, LABELS *varlabels_); // show in this table an'allocated matrix
  void newListView(QString& tabname, QStringList lst);
  void newModelInfo();
  
  void setWindowID(int id_){ id = id_; }
  int getWindowID(){ return id; }
  Table* getTable(){ return table; }
  Model *getTableModel(){ return table->model(); }
  ModelInfo * getModelInfoWindow(){ return modinfo; }
  ListView* getListView(){ return listview; }

private:
  Table *table;
  ModelInfo *modinfo;
  ListView *listview;
  int id;
};
#endif
