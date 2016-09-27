#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QWidget>
#include <QStringList>
#include <QContextMenuEvent>

#include "ui_ListView.h"


class ListView : public QWidget
{
  Q_OBJECT

public:
  ListView(QStringList);

private slots:
  void copy();
  void exportlst();
  void search();

private:
  Ui::ListView ui;

protected:
   void contextMenuEvent(QContextMenuEvent *event);
};

#endif
