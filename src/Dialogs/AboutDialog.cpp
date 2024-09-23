#include "AboutDialog.h"
#include "scientific.h"
#include <QString>
AboutDialog::AboutDialog(int major, int minor, int patch) {
  ui.setupUi(this);

  QString scientificversion = QString::fromLatin1(GetScientificVersion());
  ui.label_version->setText(QString("Version: %1.%2.%3\nLibscientific: %4")
                                .arg(QString::number(major))
                                .arg(QString::number(minor))
                                .arg(QString::number(patch))
                                .arg(scientificversion));

  connect(ui.closeButton, SIGNAL(clicked(bool)), SLOT(accept()));
}
