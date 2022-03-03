#ifndef MODELINFO_H
#define MODELINFO_H

#include <QWidget>
#include <QString>
#include <QStringList>
#include "ui_ModelInfo.h"

class ModelInfo : public QWidget
{
  Q_OBJECT

public:
  ModelInfo(QWidget *parent = 0);
  void Adjust();
  void setText(QStringList textlst);

private:
  Ui::ModelInfo ui;
};

#endif
