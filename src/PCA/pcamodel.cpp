#include "pcamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

PCAPREDICTION::PCAPREDICTION()
{
  initMatrix(&pscores);
  id = did = -1;
}

PCAPREDICTION::~PCAPREDICTION()
{
  #ifdef DEBUG
  qDebug() << "Delete PCAPREDICTION: " << name;
  #endif
  DelMatrix(&pscores);
}

PCAModel::~PCAModel()
{
  #ifdef DEBUG
  qDebug() << "Delete PCA Model: " << name;
  #endif
  delPCAPredictions();
  DelPCAModel(&m);
}

PCAModel::PCAModel()
{
  NewPCAModel(&m);
  did = xscaling = npc = modelid = -1;
}
