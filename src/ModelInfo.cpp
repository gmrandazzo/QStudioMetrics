#include "ModelInfo.h"
#include "qstudiometricstypes.h"
#include <QApplication>
#include <QString>
#include <QStringList>

void ModelInfo::setText(QStringList textlst) {
  for (int i = 0; i < textlst.size(); i++)
    ui.textEdit->append(textlst[i]);
}

void ModelInfo::Adjust() {
  resize(minimumSize());
  //   adjustSize();
}

ModelInfo::ModelInfo(QWidget *parent) : QWidget(parent) { ui.setupUi(this); }
