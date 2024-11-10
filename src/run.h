#ifndef RUN_H
#define RUN_H

#include "qsmdata.h"
#include <QFuture>

#include "scientific.h"

class RUN {
public:
  RUN()
        : scientifisignal(SIGSCIENTIFICRUN), // Initialize scientifisignal
          xscaling(0),                       // Initialize xscaling
          yscaling(0),                       // Initialize yscaling
          pc(5),                             // Initialize pc
          ngroup(5),                         // Initialize ngroup
          niter(-1),                         // Initialize niter
          n_yscrambling(-1),                 // Initialize n_yscrambling
          metric(-1),                        // Initialize metric
          nobjects(-1),                      // Initialize nobjects
          maxnobjects(-1),                   // Initialize maxnobjects
          nclusters(-1),                     // Initialize nclusters
          clusteralgoritm(-1),               // Initialize clusteralgoritm
          linktype(-1),                      // Initialize linktype
          objselectiontype(-1),              // Initialize objselectiontype
          nmaxobjects(-1),                   // Initialize nmaxobjects
          nmaxclusters(-1),                  // Initialize nmaxclusters
          threshold(0.f),                    // Initialize threshold
          yscrambling(false),                // Initialize yscrambling
          m(nullptr),                        // Initialize m to nullptr
          uiv(nullptr),                      // Initialize uiv to nullptr
          x(nullptr),                        // Initialize x to nullptr
          y(nullptr),                        // Initialize y to nullptr
          ax(nullptr),                       // Initialize ax to nullptr
          ay(nullptr),                       // Initialize ay to nullptr
          dv(nullptr),                       // Initialize dv to nullptr
          pcamod(nullptr),                   // Initialize pcamod to nullptr
          cpcamod(nullptr),                  // Initialize cpcamod to nullptr
          plsmod(nullptr),                   // Initialize plsmod to nullptr
          mlrmodel(nullptr),                 // Initialize mlrmodel to nullptr
          ldamodel(nullptr)                  // Initialize ldamodel to nullptr 
    {};
  void AbortRun();
  void Test();
  void setXMatrix(matrix *x_);
  void setYMatrix(matrix *y_);
  void setXTensor(tensor *ax_);
  void setYTensor(tensor *ay_);
  void setPCAModel(PCAModel *pcamod_);
  void setCPCAModel(CPCAModel *cpcamod_);
  void setPLSModel(PLSModel *plsmod_);
  void setMLRModel(MLRModel *mlrmodel_);
  void setLDAModel(LDAModel *ldamodel_);

  void setXScalingType(int xscaling_);
  void setYScalingType(int yscaling_);
  void setNumberPC(int pc_);
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
  int scientifisignal;
  int xscaling;
  int yscaling;
  int pc;
  int ngroup;
  int niter;
  int n_yscrambling;
  int metric;
  int nobjects;
  int maxnobjects;
  int nclusters;
  int clusteralgoritm;
  int linktype;
  int objselectiontype;
  int nmaxobjects;
  int nmaxclusters;
  float threshold;  // Assuming threshold is a float based on initialization.
  bool yscrambling;

  matrix* m;         
  uivector* uiv;       
  matrix* x;         
  matrix* y;         
  tensor* ax;        
  tensor* ay;        
  dvector* dv;        
  PCAModel* pcamod;    
  CPCAModel* cpcamod;   
  PLSModel* plsmod;     
  MLRModel* mlrmodel;  
  LDAModel* ldamodel;

  int algtype; // PLS_, PLS_DA_,
  int vt;
  int linkype;
  strvector *dendogram;

  void DoPCA();
  void DoPCAPrediction();
  void DoCPCA();
  void DoCPCAPrediction();
  void DoPLS();
  void DoPLSValidation();
  void DoPLSPrediction();

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
