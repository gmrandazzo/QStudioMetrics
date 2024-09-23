#ifndef MODELINFO_H
#define MODELINFO_H

#include "ui_ModelInfo.h"
#include <QString>
#include <QStringList>
#include <QWidget>

class ModelInfo : public QWidget {
  Q_OBJECT

public:
  explicit ModelInfo(QWidget *parent = 0);
  void Adjust();
  void setText(QStringList textlst);

private:
  Ui::ModelInfo ui;
};

#endif
