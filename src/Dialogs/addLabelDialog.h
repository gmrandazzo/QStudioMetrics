#ifndef ADDLABELDIALOG_H
#define ADDLABELDIALOG_H

#include <QDialog>
#include <QString>

#include "ui_addLabelDialog.h"

class addLabelDialog : public QDialog {
  Q_OBJECT

public:
  addLabelDialog(QString windowname);
  QString getLabel();

private:
  Ui::addLabelDialog ui;
};

#endif
