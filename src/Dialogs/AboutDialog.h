#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include "ui_AboutDialog.h"
#include <QDialog>

class AboutDialog : public QDialog {
  Q_OBJECT

public:
  AboutDialog(int major, int minor, int patch);

private:
  Ui::AboutDialog ui;
};

#endif
