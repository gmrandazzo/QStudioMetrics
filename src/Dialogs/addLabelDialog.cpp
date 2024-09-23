#include "addLabelDialog.h"

QString addLabelDialog::getLabel() { return ui.lineEdit->text(); }

addLabelDialog::addLabelDialog(QString windowname) : QDialog() {
  ui.setupUi(this);
  setWindowTitle(windowname);
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
}
