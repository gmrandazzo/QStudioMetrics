#ifndef VARIABLESELECTIONMODEL_H
#define VARIABLESELECTIONMODEL_H

#include <scientific.h>
#include <QFile>
#include <QString>
#include <QList>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

class VariableSelectionModel
{
public:
  VariableSelectionModel();
  ~VariableSelectionModel();

  void ImportVarSelModel(QString path, QString name_);
  void WriteVarSelModel(QString path);

  void setName(QString varselname_){ varselname = varselname_; }
  QString getName(){ return varselname.toUtf8(); }

  QStringList &getVariableNames(){ return variablenames; }
  matrix **MapPointer(){ return &map; }
  matrix *getMap(){ return map; }
  uivector **SelectedVablesPointer(){ return &selectedvariables; }
  uivector *getSelectedVariables(){ return selectedvariables; }
  uivector **VariableDistributionPointer(){ return &vardistribution; }
  uivector *getVariableDistribution(){ return vardistribution; }

  void setProjectID(int pid_){ pid = pid_; }
  int getProjectID(){ return pid; }
  void setModelID(int modelid_){ modelid = modelid_; }
  int getModelID(){ return modelid; }

  void setXHash(QString hash_){ xhash = hash_; }
  QString &getXHash(){ return xhash; }
  void setYHash(QString hash_){ yhash = hash_; }
  QString &getYHash(){ return yhash; }

  void setXScaling(int xscaling_){ xscaling = xscaling_; }
  int getXScaling(){ return xscaling; }
  void setYScaling(int yscaling_){ yscaling = yscaling_; }
  int getYScaling(){ return yscaling; }

  void setNumberOfComponents(int npc_){ npc = npc_; };
  int getNumberOfComponents(){ return npc; };

  void setVariableSelectionAlgorithm(QString varselalgo_){ varselalgo = varselalgo_; }
  QString getVariableSelectionAlgorithm(){ return varselalgo; }

  void setVariableSelectionAlgorithmOptions(QString varseloptions_){ varseloptions = varseloptions_; }
  QString getVariableSelectionAlgorithmOptions(){ return varseloptions; }

private:
  void ImportVarSelModelInfo(QString path);
  QStringList variablenames;
  QString varselname, varselalgo, varseloptions;
  QString xhash, yhash;
  matrix *map;
  uivector *selectedvariables, *vardistribution;
  int pid, modelid, xscaling, yscaling, npc;
};

#endif
