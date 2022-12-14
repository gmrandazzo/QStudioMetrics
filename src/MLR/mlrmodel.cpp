#include "mlrmodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

MLRPREDICTION::MLRPREDICTION()
{
  initMatrix(&py);
  initDVector(&r2y);
  initDVector(&sdec);
}

MLRPREDICTION::~MLRPREDICTION(){
  #ifdef DEBUG
  qDebug() << "Delete MLRPREDICTION: " << name;
  #endif
  DelMatrix(&py);
  DelDVector(&r2y);
  DelDVector(&sdec);
}

MLRModel::MLRModel(){
  NewMLRModel(&m);
  did = modelid = -1;
  validation = 0;
}

MLRModel::~MLRModel()
{
  #ifdef DEBUG
  qDebug() << "Delete MLR Model: " << name;
  #endif
  delMLRPredictions();
  DelMLRModel(&m);
}
