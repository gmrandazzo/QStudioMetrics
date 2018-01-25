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
  initMatrix(&r2);
  initMatrix(&sdec);
  initMatrix(&bias);
  initArray(&roc);
  initMatrix(&roc_auc);
  initArray(&precision_recall);
  initMatrix(&precision_recall_ap);
}

EPLSPrediction::~EPLSPrediction()
{
  #ifdef DEBUG
  qDebug() << "Delete EPLSPrediction: " << name;
  #endif

  DelArray(&pxscores);
  DelMatrix(&py);
  DelMatrix(&r2);
  DelMatrix(&sdec);
  DelMatrix(&bias);
  DelArray(&roc);
  DelMatrix(&roc_auc);
  DelArray(&precision_recall);
  DelMatrix(&precision_recall_ap);
}

EPLSModel::EPLSModel()
{
  NewEPLSModel(&m);
  initMatrix(&r2);
  initMatrix(&sdec);
  initMatrix(&q2);
  initMatrix(&sdep);
  initMatrix(&bias);
  initArray(&roc_recalculated);
  initMatrix(&roc_auc_recalculated);
  initArray(&precision_recall_recalculated);
  initMatrix(&precision_recall_ap_recalculated);
  initArray(&roc_predicted);
  initMatrix(&roc_auc_predicted);
  initArray(&precision_recall_predicted);
  initMatrix(&precision_recall_ap_predicted);
  initMatrix(&y_recalculated);
  initMatrix(&y_recalculated_residuals);
  initMatrix(&y_predicted);
  initMatrix(&y_predicted_residuals);
  initMatrix(&yscrambling);
  did = xscaling = yscaling = npc = modelid = -1;
  validation = 0;
}

EPLSModel::~EPLSModel()
{
  #ifdef DEBUG
  qDebug() << "Delete PLS Model: "<< name;
  #endif
  DelMatrix(&r2);
  DelMatrix(&sdec);
  DelMatrix(&q2);
  DelMatrix(&sdep);
  DelMatrix(&bias);
  DelArray(&roc_recalculated);
  DelMatrix(&roc_auc_recalculated);
  DelArray(&precision_recall_recalculated);
  DelMatrix(&precision_recall_ap_recalculated);
  DelArray(&roc_predicted);
  DelMatrix(&roc_auc_predicted);
  DelArray(&precision_recall_predicted);
  DelMatrix(&precision_recall_ap_predicted);
  DelMatrix(&y_recalculated);
  DelMatrix(&y_recalculated_residuals);
  DelMatrix(&y_predicted);
  DelMatrix(&y_predicted_residuals);
  DelMatrix(&yscrambling);
  delEPLSPredictions();
  DelEPLSModel(&m);
}
