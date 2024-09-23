#ifndef LISTVIEW_H
#define LISTVIEW_H

#include <QContextMenuEvent>
#include <QStringList>
#include <QWidget>

#include "ui_ListView.h"

class ListView : public QWidget {
  Q_OBJECT

public:
  explicit ListView(QStringList);

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
