#include "cpcamodel.h"
#include "DATAIO.h"
#include "qsmdata.h"

#include <QString>
#include <QTextStream>

#ifdef DEBUG
#include <QDebug>
#endif

CPCAPREDICTION::CPCAPREDICTION() {
  initMatrix(&p_super_scores);
  initTensor(&p_block_scores);
  id = did = -1;
}

CPCAPREDICTION::~CPCAPREDICTION() {
#ifdef DEBUG
  qDebug() << "Delete CPCAPREDICTION: " << name;
#endif
  DelMatrix(&p_super_scores);
  DelTensor(&p_block_scores);
}

CPCAModel::~CPCAModel() {
#ifdef DEBUG
  qDebug() << "Delete CPCA Model: " << name;
#endif
  delCPCAPredictions();
  DelCPCAModel(&m);
}

CPCAModel::CPCAModel() {
  NewCPCAModel(&m);
  did = xscaling = npc = modelid = -1;
}
