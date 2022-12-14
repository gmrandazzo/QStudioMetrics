#include "upcamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

UPCAPREDICTION::UPCAPREDICTION(){
  initMatrix(&pscores);
}

UPCAPREDICTION::~UPCAPREDICTION()
{
  #ifdef DEBUG
  qDebug() << "Delete UPCAPREDICTION: " << name;
  #endif
  DelMatrix(&pscores);
}

UPCAModel::UPCAModel()
{
  NewUPCAModel(&m);
  did = xscaling = npc = modelid = -1;
}

UPCAModel::~UPCAModel()
{
  #ifdef DEBUG
  qDebug() << "Delete UPCA Model: " << name;
  #endif
  delUPCAPredictions();
  DelUPCAModel(&m);
}
