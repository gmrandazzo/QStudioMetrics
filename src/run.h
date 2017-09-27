#ifndef RUN_H
#define RUN_H

#include <QFuture>
#include "qsmdata.h"

#include "scientific.h"

class RUN
{
public:
  RUN();
  void AbortRun();
  void Test();
  void setXMatrix(matrix *x_);
  void setYMatrix(matrix *y_);
  void setXArray(array *ax_);
  void setYArray(array *ay_);
  void setPCAModel(PCAModel *pcamod_);
  void setPLSModel(PLSModel *plsmod_);
  void setUPCAModel(UPCAModel *upcamod_);
  void setUPLSModel(UPLSModel *uplsmod_);
  void setVarSel(VariableSelectionModel* vselmod_);
  void setMLRModel(MLRModel *mlrmodel_);
  void setLDAModel(LDAModel *ldamodel_);
  
  void setXScalingType(int xscaling_);
  void setYScalingType(int yscaling_);
  void setNumberPC(int pc_);
  void setValidationType(int vt_);
  void setNumberOfGroups(int ngroup_);
  void setNumberOfIterations(int niter_);
  void setModelYScrambling(bool yscrambling_);
  void setModelYScramblingBlock(int block);

  void setModelSampleValidator(int samplevalidator_);
  void setModelSampleValidatorClasses(uivector *obj_class);
  void setModelSampleValidatorIncObj(int deltaobj_);
  void setModelSampleValidatorMaxObj(int maxobj_);
  void setModelSampleValidatorSampleSize(int sample_size_);
  void setModelSampleValidatorIterations(int niters_);
  void setModelSampleValidatorBestID(bool status);
  QList <int> getModelSampleValidatorBestID();

  void setVariableSelectionAlgorithm(int vselalgorithm_);
  void setPopulationSize(int population_size_);
  void setCrossoverFraction(double fraction_of_population_);
  void setMutationRate(double mutation_rate_);
  void setCrossoverType(int crossovertype_);
  void setNumbersOfSwappiness(double nswapping_);
  void setPopulationConvergence(double populationconvergence_);
  
  void setRandomizedVariables(int randomness_);
  
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
  QFuture<void> RunPLS();
  QFuture<void> RunPLSValidation();
  QFuture<void> RunPLSPrediction();
  QFuture<void> RunPLSVariableSelection();
  QFuture<void> RunUPCA();
  QFuture<void> RunUPCAPrediction();
  QFuture<void> RunUPLS();
  QFuture<void> RunUPLSValidation();
  QFuture<void> RunUPLSPrediction();
  QFuture<void> RunUPLSVariableSelection();
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
  array *ax, *ay;
  uivector *uiv;
  dvector *dv;
  
  PCAModel *pcamod;
  PLSModel *plsmod;
  UPCAModel *upcamod;
  UPLSModel *uplsmod;
  VariableSelectionModel* vselmod;
  MLRModel *mlrmodel;
  LDAModel *ldamodel;
  
  ssignal scientifisignal;
  
  int xscaling;
  int yscaling;
  int pc;
  int vt;
  int ngroup;
  int niter;
  int samplevalidator_niters;
  int samplevalidator_samplesize;
  int samplevalidator_incobj;
  int samplevalidator_maxobj;
  bool yscrambling;
  int samplevalidator;
  int block;
  
  int vselalgorithm;
  
  int population_size;
  double fraction_of_population;
  double mutation_rate;
  int crossovertype;
  double nswapping;
  double populationconvergence;
  bool bestsampleid;
  QList <int> bestid_;
  double randomness;
  
  double threshold; //used for Spearman's Selection
  //Used for MDC, MaxMinDis, Random Selection
  int metric;
  uint nobjects;
  uint maxnobjects;
  
  uint nclusters;
  int clusteralgoritm;
  int linktype;
  int objselectiontype;
  int nmaxobjects;
  int nmaxclusters;
  strvector *dendogram;
  
  void DoPCA();
  void DoPCAPrediction();
  void DoPLS();
  void DoPLSValidation();
  void DoPLSPrediction();
  void DoPLSVariableSelection();
  void DoUPCA();
  void DoUPCAPrediction();
  void DoUPLS();
  void DoUPLSValidation();
  void DoUPLSPrediction();
  void DoUPLSVariableSelection();
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

