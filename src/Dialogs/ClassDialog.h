#ifndef CLASSDIALOG_H
#define CLASSDIALOG_H

#include <QDialog>
#include <QList>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "ui_ClassDialog.h"

struct CLASSES {
  QString name;
  QStringList objlst;
};

class ClassDialog : public QDialog {
  Q_OBJECT

public:
  ClassDialog(QStringList objenames, QList<CLASSES> *classes_);
  ~ClassDialog();

private slots:
  void Import();
  void Add();
  void Remove();

private:
  Ui::ClassDialog ui;
  QList<CLASSES> *classes;
  QStandardItemModel *tab1, *tab2;
  int ClassesHaveLabelName(QString label);
};

#endif
