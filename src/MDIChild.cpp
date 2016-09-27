#include "MDIChild.h"
#include <QDebug>
#include <QProgressDialog>
#include <QStringListModel>
#include <QDesktopWidget>
#include "ListView.h"

void MDIChild::newModelInfo()
{
  setWindowTitle("Model Info");
  modinfo = new ModelInfo(this);
  setWidget(modinfo);
  modinfo->Adjust();
  adjustSize();
}

void MDIChild::newListView(QString& tabname, QStringList lst)
{
  setWindowTitle(tabname);
  listview = new ListView(lst);
  setWidget(listview);
  adjustSize();
}

void MDIChild::newTable (QString& tabname_)
{
  setWindowTitle(tabname_);
  table = new Table(this);
  table->setObjectName(tabname_);
  setWidget(table);
  resize(300, 260);
}

void MDIChild::newTable (QString& tabname_, matrix* m)
{
  setWindowTitle(tabname_);
  table = new Table(m, this);
  table->setObjectName(tabname_);
  setWidget(table);
  resize(300, 260);
}

void MDIChild::newTable(QString& tabname_, matrix* m, LABELS *objlabels_, LABELS *varlabels_)
{
  setWindowTitle(tabname_);
  table = new Table(m, objlabels_, varlabels_, this);
  table->setObjectName(tabname_);
  setWidget(table);
  resize(300, 260);
}

void MDIChild::newTable(QString& tabname, QList< QStringList > tab, LABELS* objlabels_, LABELS* varlabels_)
{
  setWindowTitle(tabname);
  table = new Table(tab, objlabels_, varlabels_, this);
  table->setObjectName(tabname);
  setWidget(table);
  resize(300, 260);
}

MDIChild::MDIChild()
{
  table = 0;
  modinfo = 0;
  setAttribute(Qt::WA_DeleteOnClose);
  id = -1;
}

/*
MDIChild::~MDIChild()
{
  qDebug() << "Delete MDIChild with id : " << id;
  if(table != 0)
    delete table;
  
  if(modinfo != 0)
    delete modinfo;
}
*/
