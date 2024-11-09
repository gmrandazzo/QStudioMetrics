
#include "run.h"
#include "qstudiometricstypes.h"

#include <QThread>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>
#include <QtCore>

#include <scientific.h>

#include <cstdlib>
#include <ctime>

void RUN::DoClusterValidation() {
  if (vt == BOOTSTRAPRGCV_) {
    KMeansRandomGroupsCV(m, nmaxclusters, clusteralgoritm, ngroup, niter, dv,
                         QThread::idealThreadCount());
  } else {
    KMeansJumpMethod(m, nmaxclusters, clusteralgoritm, dv,
                     QThread::idealThreadCount());
  }
}

void RUN::DoClustering() {
  if (objselectiontype == 3) { /*get all objects...*/
    if (clusteralgoritm < 4) {
      KMeans(m, nclusters + 1, clusteralgoritm, uiv, NULL,
             QThread::idealThreadCount());
    } else {
      HierarchicalClustering(m, nclusters + 1, uiv, dendogram,
                             (enum LinkageType)linktype,
                             QThread::idealThreadCount());
    }
  } else {
    matrix *centroids;
    initMatrix(&centroids);
    KMeans(m, nclusters + 1, clusteralgoritm, uiv, centroids,
           QThread::idealThreadCount());
    PruneResults(m, centroids, nmaxobjects, objselectiontype, uiv,
                 QThread::idealThreadCount());
    DelMatrix(&centroids);
  }
}

void RUN::DoRandomSelection() {
  srand(time(0));
  int i = 0;
  while (i < nobjects) {
    if (scientifisignal == SIGSCIENTIFICSTOP) {
      break;
    } else {
      int id = rand() % maxnobjects;
      if (id < maxnobjects) {
        bool getid = true;
        for (uint j = 0; j < uiv->size; j++) {
          if ((int)getUIVectorValue(uiv, j) != id) {
            continue;
          } else {
            getid = false;
          }
        }

        if (getid == true) {
          UIVectorAppend(uiv, id);
          i++;
        } else {
          continue;
        }
      } else {
        continue;
      }
    }
  }
}

void RUN::DoMaxDisSelection() {
  MaxDis_Fast(m, nobjects, metric, uiv, QThread::idealThreadCount());
}

void RUN::DoMDCSelection() {
  MDC(m, nobjects, metric, uiv, QThread::idealThreadCount());
}

void RUN::DoLDAPrediction() {
  matrix *py = ldamodel->getLastLDAPrediction()->getPredClasses();
  matrix *pfeatures = ldamodel->getLastLDAPrediction()->getPredFeatures();
  matrix *probpred = ldamodel->getLastLDAPrediction()->getProbPred();
  matrix *mvnpfd = ldamodel->getLastLDAPrediction()->getMVNProbDistrib();
  LDAPrediction(x, ldamodel->Model(), pfeatures, probpred, mvnpfd, py);
}

void RUN::DoLDAValidation() {
  DelTensor(&ldamodel->Model()->roc);
  DelDVector(&ldamodel->Model()->roc_aucs);
  DelTensor(&ldamodel->Model()->pr);
  DelDVector(&ldamodel->Model()->pr_aucs);
  DelMatrix(&ldamodel->Model()->predicted_y);
  DelMatrix(&ldamodel->Model()->predicted_residuals);

  initTensor(&ldamodel->Model()->roc);
  initDVector(&ldamodel->Model()->roc_aucs);
  initTensor(&ldamodel->Model()->pr);
  initDVector(&ldamodel->Model()->pr_aucs);
  initMatrix(&ldamodel->Model()->predicted_y);
  initMatrix(&ldamodel->Model()->predicted_residuals);

  MODELINPUT minpt;
  minpt.mx = x;
  minpt.my = y;
  minpt.nlv = 0;
  minpt.xautoscaling = 0;
  minpt.yautoscaling = 0;

  if (vt == LOO_) { // Leave One Out
    LeaveOneOut(&minpt, _LDA_, ldamodel->Model()->predicted_y,
                ldamodel->Model()->predicted_residuals,
                QThread::idealThreadCount(), &scientifisignal, 0);
  } else if (vt == KFOLDCV_) {
    KFoldCV(&minpt, uiv, _LDA_, ldamodel->Model()->predicted_y,
            ldamodel->Model()->predicted_residuals, QThread::idealThreadCount(),
            &scientifisignal, 0);
  } else {
    BootstrapRandomGroupsCV(&minpt, ngroup, niter, _LDA_,
                            ldamodel->Model()->predicted_y,
                            ldamodel->Model()->predicted_residuals,
                            QThread::idealThreadCount(), &scientifisignal, 0);
  }

  LDAMulticlassStatistics(x, ldamodel->Model()->predicted_y,
                          ldamodel->Model()->roc, ldamodel->Model()->roc_aucs,
                          ldamodel->Model()->pr, ldamodel->Model()->pr_aucs);
}

void RUN::DoLDA() { LDA(x, y, ldamodel->Model()); }

void RUN::DoMLRPrediction() {
  if (y != 0 && y->row > 0 && y->col > 0) {
    MLRPredictY(x, y, mlrmodel->Model(),
                mlrmodel->getLastMLRPrediction()->getYDipVar(), NULL,
                mlrmodel->getLastMLRPrediction()->getR2Y(),
                mlrmodel->getLastMLRPrediction()->getSDEC());
  } else {
    MLRPredictY(x, NULL, mlrmodel->Model(),
                mlrmodel->getLastMLRPrediction()->getYDipVar(), NULL, NULL,
                NULL);
  }
}

void RUN::DoMLRValidation() {
  DelDVector(&mlrmodel->Model()->q2y);
  DelDVector(&mlrmodel->Model()->sdep);
  DelDVector(&mlrmodel->Model()->bias);
  DelMatrix(&mlrmodel->Model()->predicted_y);
  DelMatrix(&mlrmodel->Model()->r2q2scrambling);

  initDVector(&mlrmodel->Model()->q2y);
  initDVector(&mlrmodel->Model()->sdep);
  initDVector(&mlrmodel->Model()->bias);
  initMatrix(&mlrmodel->Model()->predicted_y);
  initMatrix(&mlrmodel->Model()->r2q2scrambling);


  MODELINPUT minpt = {
    .mx = x,
    .my = y,
    .nlv = 0,
    .xautoscaling = 0,
    .yautoscaling = 0
  };

  if (vt == LOO_) { // Leave One Out
    LeaveOneOut(&minpt, _MLR_, mlrmodel->Model()->predicted_y,
                mlrmodel->Model()->pred_residuals, QThread::idealThreadCount(),
                &scientifisignal, 0);
  } else if (vt == KFOLDCV_) {
    KFoldCV(&minpt, uiv, _MLR_, mlrmodel->Model()->predicted_y,
            mlrmodel->Model()->pred_residuals, QThread::idealThreadCount(),
            &scientifisignal, 0);
  } else {
    BootstrapRandomGroupsCV(&minpt, ngroup, niter, _MLR_,
                            mlrmodel->Model()->predicted_y,
                            mlrmodel->Model()->pred_residuals,
                            QThread::idealThreadCount(), &scientifisignal, 0);
  }

  MLRRegressionStatistics(y, mlrmodel->Model()->predicted_y,
                          mlrmodel->Model()->q2y, mlrmodel->Model()->sdep,
                          mlrmodel->Model()->bias);

  if (yscrambling == true) {

    ValidationArg varg;
    varg.vtype = (vt == LOO_) ? LOO : BootstrapRGCV;
    varg.rgcv_group = (vt == LOO_) ? 0 : ngroup;
    varg.rgcv_iterations = (vt == LOO_) ? 0 : niter;
    YScrambling(&minpt, _MLR_, varg, n_yscrambling,
                mlrmodel->Model()->r2q2scrambling, QThread::idealThreadCount(),
                &scientifisignal);
  }
}

void RUN::DoMLR() { MLR(x, y, mlrmodel->Model(), &scientifisignal); }

void RUN::DoPLSPrediction() {

  // we go from 1 because for 0 component no value can be calculated.
  PLSYPredictorAllLV(x, plsmod->Model(),
                     plsmod->getLastPLSPrediction()->getXPredScores(),
                     plsmod->getLastPLSPrediction()->getYDipVar());

  if (y != 0 && y->col > 0) { // calculate the R2 for the prediction
    PLSRegressionStatistics(y, plsmod->getLastPLSPrediction()->getYDipVar(),
                            plsmod->getLastPLSPrediction()->getR2Y(),
                            plsmod->getLastPLSPrediction()->getSDEC(), NULL);
  }
}

void RUN::DoPLSValidation() {
  // DelMatrix(&plsmod->Model()->r2y_recalculated);
  // DelMatrix(&plsmod->Model()->sdec);
  DelMatrix(&plsmod->Model()->q2y);
  DelMatrix(&plsmod->Model()->sdep);
  DelMatrix(&plsmod->Model()->bias);
  DelMatrix(&plsmod->Model()->predicted_y);
  DelMatrix(&plsmod->Model()->pred_residuals);
  DelTensor(&plsmod->Model()->roc_validation);
  DelMatrix(&plsmod->Model()->roc_auc_validation);
  DelTensor(&plsmod->Model()->precision_recall_validation);
  DelMatrix(&plsmod->Model()->precision_recall_ap_validation);

  // initMatrix(&plsmod->Model()->r2y_recalculated);
  // initMatrix(&plsmod->Model()->sdec);
  initMatrix(&plsmod->Model()->q2y);
  initMatrix(&plsmod->Model()->sdep);
  initMatrix(&plsmod->Model()->bias);
  initMatrix(&plsmod->Model()->predicted_y);
  initMatrix(&plsmod->Model()->pred_residuals);
  initTensor(&plsmod->Model()->roc_validation);
  initMatrix(&plsmod->Model()->roc_auc_validation);
  initTensor(&plsmod->Model()->precision_recall_validation);
  initMatrix(&plsmod->Model()->precision_recall_ap_validation);

  /*PLSYPredictorAllLV(x, plsmod->Model(), NULL,
  &plsmod->Model()->recalculated_y);
  */

  MODELINPUT minpt = {
    .mx = x,
    .my = y,
    .nlv = static_cast<size_t>(plsmod->getNPC()),
    .xautoscaling = static_cast<size_t>(plsmod->getXScaling()),
    .yautoscaling = static_cast<size_t>(plsmod->getYScaling())
  };

  if (vt == LOO_) { // Leave One Out
    LeaveOneOut(&minpt, _PLS_, plsmod->Model()->predicted_y,
                plsmod->Model()->pred_residuals, QThread::idealThreadCount(),
                &scientifisignal, 0);
  } else if (vt == KFOLDCV_) {
    KFoldCV(&minpt, uiv, _PLS_, plsmod->Model()->predicted_y,
            plsmod->Model()->pred_residuals, QThread::idealThreadCount(),
            &scientifisignal, 0);
  } else {
    BootstrapRandomGroupsCV(&minpt, ngroup, niter, _PLS_,
                            plsmod->Model()->predicted_y,
                            plsmod->Model()->pred_residuals,
                            QThread::idealThreadCount(), &scientifisignal, 0);
  }
  
  if (algtype == PLS_) {
    PLSRegressionStatistics(y, plsmod->Model()->predicted_y,
                              plsmod->Model()->q2y, plsmod->Model()->sdep,
                              plsmod->Model()->bias);
  } else {
    PLSDiscriminantAnalysisStatistics(
      y, plsmod->Model()->predicted_y, plsmod->Model()->roc_validation,
      plsmod->Model()->roc_auc_validation,
      plsmod->Model()->precision_recall_validation,
      plsmod->Model()->precision_recall_ap_validation);
  }

  if (yscrambling == true) {
    DelMatrix(&plsmod->Model()->yscrambling);
    initMatrix(&plsmod->Model()->yscrambling);

    ValidationArg varg;
    varg.vtype = (vt == LOO_) ? LOO : BootstrapRGCV;
    varg.rgcv_group = (vt == LOO_) ? 0 : ngroup;
    varg.rgcv_iterations = (vt == LOO_) ? 0 : niter;
    YScrambling(&minpt, _PLS_, varg, n_yscrambling, plsmod->Model()->yscrambling,
                    QThread::idealThreadCount(), &scientifisignal);
  }
}

void RUN::DoPLS() {
  PLS(x, y, pc, xscaling, yscaling, plsmod->Model(), &scientifisignal);
  if (algtype == PLS_)
    PLSRegressionStatistics(y, plsmod->Model()->recalculated_y,
                            plsmod->Model()->r2y_recalculated,
                            plsmod->Model()->sdec, NULL);
  else
    PLSDiscriminantAnalysisStatistics(
        y, plsmod->Model()->recalculated_y, plsmod->Model()->roc_recalculated,
        plsmod->Model()->roc_auc_recalculated,
        plsmod->Model()->precision_recall_recalculated,
        plsmod->Model()->precision_recall_ap_recalculated);
}

void RUN::DoPCAPrediction() {
  PCAScorePredictor(x, pcamod->Model(), pcamod->getNPC(),
                    pcamod->getLastPCAPrediction()->getPredScores());
}

void RUN::DoPCA() { PCA(x, xscaling, pc, pcamod->Model(), &scientifisignal); }

void RUN::DoCPCAPrediction() {
  CPCAScorePredictor(ax, cpcamod->Model(), cpcamod->getNPC(),
                     cpcamod->getLastCPCAPrediction()->getPredSuperScores(),
                     cpcamod->getLastCPCAPrediction()->getPredBlockScores());
}

void RUN::DoCPCA() { CPCA(ax, xscaling, pc, cpcamod->Model()); }

QFuture<void> RUN::RunClusterValidation() {
  return QtConcurrent::run([this] { RUN::DoClusterValidation(); });
}

QFuture<void> RUN::RunClustering() {
  return QtConcurrent::run([this] { RUN::DoClustering(); });
}

QFuture<void> RUN::RunRandomSelection() {
  return QtConcurrent::run([this] { RUN::DoRandomSelection(); });
}

QFuture<void> RUN::RunMaxDisSelection() {
  return QtConcurrent::run([this] { RUN::DoMaxDisSelection(); });
}

QFuture<void> RUN::RunMDCSelection() {
  return QtConcurrent::run([this] { RUN::DoMDCSelection(); });
}

QFuture<void> RUN::RunLDAPrediction() {
  return QtConcurrent::run([this] { RUN::DoLDAPrediction(); });
}

QFuture<void> RUN::RunLDAValidation() {
  return QtConcurrent::run([this] { RUN::DoLDAValidation(); });
}

QFuture<void> RUN::RunLDA() {
  return QtConcurrent::run([this] { RUN::DoLDA(); });
}

QFuture<void> RUN::RunMLRPrediction() {
  return QtConcurrent::run([this] { RUN::DoMLRPrediction(); });
}

QFuture<void> RUN::RunMLRValidation() {
  return QtConcurrent::run([this] { RUN::DoMLRValidation(); });
}

QFuture<void> RUN::RunMLR() {
  return QtConcurrent::run([this] { RUN::DoMLR(); });
}

QFuture<void> RUN::RunPLSPrediction() {
  return QtConcurrent::run([this] { RUN::DoPLSPrediction(); });
}

QFuture<void> RUN::RunPLSValidation(int algtype_) {
  algtype = algtype_;
  return QtConcurrent::run([this] { RUN::DoPLSValidation(); });
}

QFuture<void> RUN::RunPLS(int algtype_) {
  algtype = algtype_;
  return QtConcurrent::run([this] { RUN::DoPLS(); });
}

QFuture<void> RUN::RunCPCAPrediction() {
  return QtConcurrent::run([this] { RUN::DoCPCAPrediction(); });
}

QFuture<void> RUN::RunCPCA() {
  return QtConcurrent::run([this] { RUN::DoCPCA(); });
}

QFuture<void> RUN::RunPCAPrediction() {
  return QtConcurrent::run([this] { RUN::DoPCAPrediction(); });
}

QFuture<void> RUN::RunPCA() {
  return QtConcurrent::run([this] { RUN::DoPCA(); });
}

void RUN::setNMaxClusters(int nmaxclusters_) { nmaxclusters = nmaxclusters_; }

void RUN::setNMaxObjects(int nmaxobjects_) { nmaxobjects = nmaxobjects_; }

void RUN::setObjectSelectionType(int objselectiontype_) {
  objselectiontype = objselectiontype_;
}

void RUN::setLinkType(int linktype_) { linktype = linktype_; }

void RUN::setClusteringAlgorithm(int clusteralgoritm_) {
  clusteralgoritm = clusteralgoritm_;
}

void RUN::setNumberOfCluster(uint nclusters_) { nclusters = nclusters_; }

void RUN::setNumberMaxOfObject(uint maxnobjects_) {
  maxnobjects = maxnobjects_;
}

void RUN::setNumberOfObject(uint nobjects_) { nobjects = nobjects_; }

void RUN::setMetric(int metric_) {
  if (metric_ == EUCLIDEANDISTANCE) {
    metric = 0;
  } else if (metric_ == MANHATTANDISTANCE) {
    metric = 1;
  } else {
    metric = 2;
  }
}

void RUN::setDendogramVector(strvector *dendogram_) { dendogram = dendogram_; }

void RUN::setDVector(dvector *dv_) { dv = dv_; }

void RUN::setUIVector(uivector *uiv_) { uiv = uiv_; }

void RUN::setMatrix(matrix *m_) { m = m_; }

void RUN::setThreshold(double threshold_) { threshold = threshold_; }

void RUN::setModelYScrambling(bool yscrambling_) { yscrambling = yscrambling_; }

void RUN::setModelYScramblingModels(int n_yscrambling_) {
  n_yscrambling = n_yscrambling_;
}

void RUN::setNumberOfIterations(int niter_) { niter = niter_; }

void RUN::setNumberOfGroups(int ngroup_) { ngroup = ngroup_; }

void RUN::setNumberPC(int pc_) { pc = pc_; }
void RUN::setValidationType(int vt_) { vt = vt_; }

void RUN::setYScalingType(int yscaling_) { yscaling = yscaling_; }
void RUN::setXScalingType(int xscaling_) { xscaling = xscaling_; }

void RUN::setLDAModel(LDAModel *ldamodel_) { ldamodel = ldamodel_; }

void RUN::setMLRModel(MLRModel *mlrmodel_) { mlrmodel = mlrmodel_; }

void RUN::setPLSModel(PLSModel *plsmod_) { plsmod = plsmod_; }

void RUN::setPCAModel(PCAModel *pcamod_) { pcamod = pcamod_; }

void RUN::setCPCAModel(CPCAModel *cpcamod_) { cpcamod = cpcamod_; }

void RUN::setYTensor(tensor *ay_) { ay = ay_; }

void RUN::setXTensor(tensor *ax_) { ax = ax_; }

void RUN::setYMatrix(matrix *y_) { y = y_; }

void RUN::setXMatrix(matrix *x_) { x = x_; }

void RUN::Test() {
#ifdef DEBUG
  qDebug() << " XSCALING " << xscaling << " YSCALING " << yscaling << " PC "
           << pc << " VT " << vt << " GROUP " << ngroup << " ITER " << niter;
#endif
}

void RUN::AbortRun() {
  scientifisignal = SIGSCIENTIFICSTOP;
  //   QThreadPool::globalInstance()->waitForDone();
}

RUN::RUN() {
  scientifisignal = SIGSCIENTIFICRUN;
  xscaling = 0;
  yscaling = 0;
  pc = 5;
  ngroup = 5;
  niter = -1;
  n_yscrambling = -1;
  metric = -1;
  nobjects = -1;
  maxnobjects = -1;
  nclusters = -1;
  clusteralgoritm = -1;
  linktype = -1;
  objselectiontype = -1;
  nmaxobjects = -1;
  nmaxclusters = -1;
  threshold = 0.f;
  yscrambling = false;
  m = NULL;
  uiv = NULL;
  x = NULL;
  y = NULL;
  ax = NULL;
  ay = NULL;
  dv = NULL;
  uiv = NULL;
  pcamod = NULL;
  cpcamod = NULL;
  plsmod = NULL;
  mlrmodel = NULL;
  ldamodel = NULL;
}
