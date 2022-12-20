#include "plsmodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

PLSPREDICTION::PLSPREDICTION()
{
  initMatrix(&pxscores);
  initMatrix(&py);
  initMatrix(&r2y);
  initMatrix(&sdec);
  id = did = -1;
}

PLSPREDICTION::~PLSPREDICTION()
{
  #ifdef DEBUG
  qDebug() << "Delete PLSPREDICTION: " << name;
  #endif

  DelMatrix(&pxscores);
  DelMatrix(&py);
  DelMatrix(&r2y);
  DelMatrix(&sdec);
}

PLSModel::PLSModel()
{
  NewPLSModel(&m);
  did = xscaling = yscaling = npc = modelid = -1;
  validation = 0;
  algtype = PLS_;
}

PLSModel::~PLSModel()
{
  #ifdef DEBUG
  qDebug() << "Delete PLS Model: "<< name;
  #endif
  delPLSPredictions();
  DelPLSModel(&m);
}
