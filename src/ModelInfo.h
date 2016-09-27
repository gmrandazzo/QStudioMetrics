#ifndef MODELINFO_H
#define MODELINFO_H

#include <QWidget>
#include <QString>
#include "ui_ModelInfo.h"

class ModelInfo : public QWidget
{
  Q_OBJECT
  
public:
  ModelInfo(QWidget *parent = 0);
  void Adjust();
  void setModelName(QString modname_);
  void setModelType(QString modtype_);
  void setModelComponentNumber(int modnpc_);
  void setNObjects(int nobj);
  void setNVariables(int nvar);
  void setNY(int ny);
  void setXDataOrigin(QString modxdataorig_);
  void setYDataOrigin(QString modydataorig_);
  void setModelXScalingType(int modxscalingtype_);
  void setModelYScalingType(int modyscalingtype_);
  void setModelValidationType(int modvalidtype_);
  void setVariableSelectionAlgorithm(QString varselalgorithm);
  void setVariableSelectionOptions(QString varseloption);
  
private:
  Ui::ModelInfo ui;
};

#endif
