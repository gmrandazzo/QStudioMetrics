#ifndef RUN_H
#define RUN_H

#include "qsmdata.h"
#include <QFuture>

#include "scientific.h"

class RUN {
public:
  RUN();
  void AbortRun();
  void Test();
  void setXMatrix(matrix *x_);
  void setYMatrix(matrix *y_);
  void setXTensor(tensor *ax_);
  void setYTensor(tensor *ay_);
  void setPCAModel(PCAModel *pcamod_);
  void setCPCAModel(CPCAModel *cpcamod_);
  void setPLSModel(PLSModel *plsmod_);
  void setEPLSModel(EPLSModel *eplsmod_);
  void setMLRModel(MLRModel *mlrmodel_);
  void setLDAModel(LDAModel *ldamodel_);

  void setXScalingType(int xscaling_);
  void setYScalingType(int yscaling_);
  void setNumberPC(int pc_);
  void setElearningParm(ELearningParameters eparm_);
  void setValidationType(int vt_);
  void setNumberOfGroups(int ngroup_);
  void setNumberOfIterations(int niter_);
  void setModelYScrambling(bool yscrambling_);
  void setModelYScramblingModels(int n_yscrambling);

  void setThreshold(double threshold_);

  void setMatrix(matrix *m_);
  void setUIVector(uivector *uiv_);
  void setDVector(dvector *dv_);
  void setDendogramVector(strvector *dendogram_);
  void setMetric(int metric_);
  void setNumberOfObject(uint nobjects_);
  void setNumberMaxOfObject(uint maxnobjects_);

  void setNumberOfCluster(uint nclusters_);
  void setClusteringAlgorithm(int clusteralgoritm_);
  void setLinkType(int linktype_); /*only for hierarchical clustering*/
  void setObjectSelectionType(int objselectiontype_);
  void setNMaxObjects(int nmaxobjects_);
  void setNMaxClusters(int nmaxclusters_);

  QFuture<void> RunPCA();
  QFuture<void> RunPCAPrediction();
  QFuture<void> RunCPCA();
  QFuture<void> RunCPCAPrediction();
  QFuture<void> RunPLS(int algtype_);
  QFuture<void> RunPLSValidation(int algtype_);
  QFuture<void> RunPLSPrediction();
  QFuture<void> RunEPLS(int algtype_);
  QFuture<void> RunEPLSValidation(int algtype_, CombinationRule crule);
  QFuture<void> RunEPLSPrediction(CombinationRule crule);
  QFuture<void> RunMLR();
  QFuture<void> RunMLRValidation();
  QFuture<void> RunMLRPrediction();
  QFuture<void> RunLDA();
  QFuture<void> RunLDAValidation();
  QFuture<void> RunLDAPrediction();

  QFuture<void> RunMDCSelection();
  QFuture<void> RunMaxDisSelection();
  QFuture<void> RunRandomSelection();

  QFuture<void> RunClustering();
  QFuture<void> RunClusterValidation();

private:
  matrix *m, *x, *y;
  tensor *ax, *ay;
  uivector *uiv;
  dvector *dv;

  PCAModel *pcamod;
  CPCAModel *cpcamod;
  PLSModel *plsmod;
  EPLSModel *eplsmod;
  MLRModel *mlrmodel;
  LDAModel *ldamodel;

  ssignal scientifisignal;

  int algtype; // PLS_, PLS_DA_, EPLS_, EPLS_DA_
  ELearningParameters eparm;
  CombinationRule crule;
  int xscaling;
  int yscaling;
  int pc;
  int vt;
  int ngroup;
  int niter;
  bool yscrambling;
  int n_yscrambling;

  double threshold; // used for Spearman's Selection
  // Used for MDC, MaxMinDis, Random Selection
  int metric;
  int nobjects;
  int maxnobjects;

  int nclusters;
  int clusteralgoritm;
  int linktype;
  int objselectiontype;
  int nmaxobjects;
  int nmaxclusters;
  strvector *dendogram;

  void DoPCA();
  void DoPCAPrediction();
  void DoCPCA();
  void DoCPCAPrediction();
  void DoPLS();
  void DoPLSValidation();
  void DoPLSPrediction();

  void DoEPLS();
  void DoEPLSValidation();
  void DoEPLSPrediction();

  void DoMLR();
  void DoMLRValidation();
  void DoMLRPrediction();
  void DoLDA();
  void DoLDAValidation();
  void DoLDAPrediction();

  void DoMDCSelection();
  void DoMaxDisSelection();
  void DoRandomSelection();

  void DoClustering();
  void DoClusterValidation();
};

#endif
