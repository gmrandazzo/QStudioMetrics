#ifndef VARIABLESELECTIONMODEL_H
#define VARIABLESELECTIONMODEL_H

#include "qstudiometricstypes.h"
#include <QFile>
#include <QList>
#include <QString>
#include <QStringList>
#include <scientific.h>

class VariableSelectionModel {
public:
  VariableSelectionModel();
  ~VariableSelectionModel();

  void ImportVarSelModel(QString path, QString name_);
  void WriteVarSelModel(QString path);

  void setName(QString varselname_) { varselname = varselname_; }
  QString getName() { return varselname.toUtf8(); }

  QStringList &getVariableNames() { return variablenames; }
  matrix **MapPointer() { return &map; }
  matrix *getMap() { return map; }
  uivector **SelectedVablesPointer() { return &selectedvariables; }
  uivector *getSelectedVariables() { return selectedvariables; }
  uivector **VariableDistributionPointer() { return &vardistribution; }
  uivector *getVariableDistribution() { return vardistribution; }

  void setProjectID(int pid_) { pid = pid_; }
  int getProjectID() { return pid; }
  void setModelID(int modelid_) { modelid = modelid_; }
  int getModelID() { return modelid; }

  void setModelHash(QString hash_) { hash = hash_; }
  QString &getModelHash() { return hash; }

  void setVariableSelectionAlgorithm(QString varselalgo_) {
    varselalgo = varselalgo_;
  }
  QString getVariableSelectionAlgorithm() { return varselalgo; }

  void setVariableSelectionAlgorithmOptions(QString varseloptions_) {
    varseloptions = varseloptions_;
  }
  QString getVariableSelectionAlgorithmOptions() { return varseloptions; }

private:
  QStringList variablenames;
  QString varselname, varselalgo, varseloptions;
  QString hash;
  matrix *map;
  uivector *selectedvariables, *vardistribution;
  int pid, modelid;
};

#endif
