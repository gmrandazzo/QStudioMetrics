#include "ListView.h"

#include <QAction>
#include <QClipboard>
#include <QFile>
#include <QFileDialog>
#include <QMenu>
#include <QStringListModel>
#include <QTextStream>

void ListView::contextMenuEvent(QContextMenuEvent *event) {
  QWidget::contextMenuEvent(event);
  QAction *copyAct = 0, *exportAct = 0, *searchAct = 0;

  QMenu menu(this);
  copyAct = new QAction(tr("&Copy"), this);
  //   copyAct->setShortcuts(QKeySequence::Copy);
  copyAct->setStatusTip(tr("Copy the current selection's contents to the "
                           "clipboard"));
  connect(copyAct, SIGNAL(triggered()), this, SLOT(copy()));

  menu.addAction(copyAct);

  exportAct = new QAction(tr("&Export.."), this);
  //   exportAct->setShortcuts(QKeySequence::Save);
  exportAct->setStatusTip(tr("Export the current list content"));
  connect(exportAct, SIGNAL(triggered()), this, SLOT(exportlst()));

  menu.addAction(exportAct);

  /*
  searchAct = new QAction(tr("&Search.."), this);
//   searchAct->setShortcuts(QKeySequence::Find);
  searchAct->setStatusTip(tr("Search in the current list contents"));
  connect(searchAct, SIGNAL(triggered()), this, SLOT(search()));

  menu.addAction(searchAct);
  */
  menu.exec(event->globalPos());

  delete copyAct;
  delete searchAct;
  delete exportAct;
}

void ListView::copy() {
  QStringList strings;
  for (int i = 0; i < ui.listView->selectionModel()->selectedRows().size(); ++i)
    strings << ui.listView->selectionModel()
                   ->selectedRows()[i]
                   .data(Qt::DisplayRole)
                   .toString();
  QApplication::clipboard()->setText(strings.join("\n"));
}

void ListView::exportlst() {
  QString fileName = QFileDialog::getSaveFileName(
      this, tr("Export list to file..."), "", tr("TXT (*.txt);;All Files (*)"));

  if (!fileName.isEmpty()) {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

    QTextStream out(&file);
    for (int i = 0; i < ui.listView->model()->rowCount(); i++) {
      out << ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString()
          << "\n";
    }
    file.close();
  }
}

void ListView::search() {}

ListView::ListView(QStringList lst) : QWidget(0) {
  ui.setupUi(this);
  ui.listView->setModel(new QStringListModel(lst));
}
