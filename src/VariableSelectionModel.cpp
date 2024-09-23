#include "VariableSelectionModel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

void VariableSelectionModel::ImportVarSelModel(QString path, QString name_) {
  varselname = name_;
  std::string sep = " \t";
  DATAIO::ImportMatrix(QString(path + "/map.txt").toUtf8().data(), sep, map);
  DATAIO::ImportUIvector(QString(path + "/selectedvars.txt").toUtf8().data(),
                         selectedvariables);
  DATAIO::ImportUIvector(QString(path + "/vardistribution.txt").toUtf8().data(),
                         vardistribution);
  DATA::ImportRows(path + "/varnames.txt", variablenames);
}

void VariableSelectionModel::WriteVarSelModel(QString path) {
  if (DATAIO::DirExists(path.toUtf8().data()) == false) {
    DATAIO::MakeDir(path.toUtf8().data());
  }

  DATAIO::WriteMatrix(QString(path + "/map.txt").toUtf8().data(), getMap());
  DATAIO::WriteUIvector(QString(path + "/selectedvars.txt").toUtf8().data(),
                        getSelectedVariables());
  DATAIO::WriteUIvector(QString(path + "/vardistribution.txt").toUtf8().data(),
                        getVariableDistribution());
  DATA::WriteList(getVariableNames(), path + "/varnames.txt");

  QStringList info;
  info.append(getVariableSelectionAlgorithm());
  info.append(getVariableSelectionAlgorithmOptions());
  DATA::WriteList(info, path + "/info.txt");
}

VariableSelectionModel::VariableSelectionModel() {
  initMatrix(&map);
  initUIVector(&selectedvariables);
  initUIVector(&vardistribution);
}

VariableSelectionModel::~VariableSelectionModel() {
#ifdef DEBUG
  qDebug() << "Delete Variable Selection Model";
#endif
  DelMatrix(&map);
  DelUIVector(&selectedvariables);
  DelUIVector(&vardistribution);
}
