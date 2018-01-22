#include <scientific.h>
#include "eplsmodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif


EPLSPrediction::EPLSPrediction()
{
  initArray(&pxscores);
  initMatrix(&py);
  initMatrix(&r2y);
  initMatrix(&sdec);
}

EPLSPrediction::~EPLSPrediction()
{
  #ifdef DEBUG
  qDebug() << "Delete EPLSPrediction: " << name;
  #endif

  DelArray(&pxscores);
  DelMatrix(&py);
  DelMatrix(&r2y);
  DelMatrix(&sdec);
}

EPLSModel::EPLSModel()
{
  NewEPLSModel(&m);
  did = xscaling = yscaling = npc = modelid = -1;
  validation = 0;
}

EPLSModel::~EPLSModel()
{
  #ifdef DEBUG
  qDebug() << "Delete PLS Model: "<< name;
  #endif
  delEPLSPredictions();
  DelEPLSModel(&m);
}
