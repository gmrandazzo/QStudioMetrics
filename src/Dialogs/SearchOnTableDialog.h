// SearchOnTableDialog.h
#ifndef SEARCHONTABLEDIALOG_H
#define SEARCHONTABLEDIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <scientific.h>

#include "ui_SearchOnTableDialog.h"

class SearchOnTableDialog : public QDialog {
  Q_OBJECT

public:
  SearchOnTableDialog(QStringList objlist);
  int getSelectionType();
  QString getLabel();
  double getMin();
  double getMax();

private slots:
  void showForm();
  void Ok();
  void Cancel();

private:
  Ui::SearchOnTableDialog ui;
};

#endif
