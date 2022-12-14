#include "uplsmodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

UPLSPREDICTION::UPLSPREDICTION()
{
  initMatrix(&pxscores);
  initArray(&py);
  initArray(&r2y);
  initArray(&sdec);
}

UPLSPREDICTION::~UPLSPREDICTION(){
  #ifdef DEBUG
  qDebug() << "Delete UPLSPREDICTION: " << name;
  #endif
  DelMatrix(&pxscores);
  DelArray(&py);
  DelArray(&r2y);
  DelArray(&sdec);
}

UPLSModel::UPLSModel(){
  NewUPLSModel(&m);
  xid = yid = xscaling = yscaling = npc = modelid = -1;
  validation = 0;
}

UPLSModel::~UPLSModel()
{
  #ifdef DEBUG
  qDebug() << "Delete UPLS Model: " << name;
  #endif
  delUPLSPredictions();
  DelUPLSModel(&m);
}
