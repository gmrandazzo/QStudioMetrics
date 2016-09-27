#include "GenericProgressDialog.h"
#include <QDesktopWidget>

void GenericProgressDialog::setValue(int value)
{
  ui.progressBar->setValue(value);
}

void GenericProgressDialog::setMax(int max)
{
  ui.progressBar->setMaximum(max);
}

void GenericProgressDialog::setMin(int min)
{
  ui.progressBar->setMinimum(min);
}

void GenericProgressDialog::setRange(int min, int max)
{
  ui.progressBar->setRange(min, max);
}

void GenericProgressDialog::hideCancel()
{
  ui.cancelButton->hide();
}

void GenericProgressDialog::showcancel()
{
  ui.cancelButton->show();
}

void GenericProgressDialog::Cancel()
{
  emit runCancelled();
}

GenericProgressDialog::GenericProgressDialog(): QDialog()
{
  ui.setupUi(this);
  setWindowFlags(((windowFlags() | Qt::CustomizeWindowHint) & ~Qt::WindowCloseButtonHint));
  setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
  const QRect screen = QApplication::desktop()->screenGeometry();
  this->move(screen.center() - this->rect().center());
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
}
