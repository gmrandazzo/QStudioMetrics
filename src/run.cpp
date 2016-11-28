
#include "run.h"
#include "qstudiometricstypes.h"

#include <QtCore>
#include <QThread>
#include <QThreadPool>
#include <QtConcurrent/QtConcurrent>

#include <scientific.h>

#include <cstdlib>
#include <ctime>

void RUN::DoClusterValidation()
{
  if(vt == RANDOMGROUP){
    KMeansRandomGroupsCV(m, nmaxclusters, clusteralgoritm, ngroup, niter, &dv, &scientifisignal);
  }
  else{
    KMeansJumpMethod(m, nmaxclusters, clusteralgoritm, &dv, &scientifisignal);
  }
}

void RUN::DoClustering()
{
  if(objselectiontype == 3){ /*get all objects...*/
    if(clusteralgoritm < 4){
      KMeans(m, nclusters, clusteralgoritm, &uiv, NULL, &scientifisignal);
    }
    else{
      HierarchicalClustering(m, nclusters, &uiv, NULL, &dendogram, (enum LinkageType)linktype, &scientifisignal);
    }
  }
  else{
    matrix *centroids;
    initMatrix(&centroids);
    KMeans(m, nclusters, clusteralgoritm, &uiv, &centroids, &scientifisignal);
    PruneResults(m, centroids, nmaxobjects, objselectiontype, uiv);
    DelMatrix(&centroids);
  }
}

void RUN::DoRandomSelection()
{
  srand(time(0));
  uint i = 0;
  while(i < nobjects){
    if(scientifisignal == SIGSCIENTIFICSTOP){
      break;
    }
    else{
      uint id = rand() % maxnobjects;
      if(id < maxnobjects){
        bool getid = true;
        for(uint j = 0; j < uiv->size; j++){
          if(getUIVectorValue(uiv, j) != id){
            continue;
          }
          else{
            getid = false;
          }
        }

        if(getid == true){
          UIVectorAppend(&uiv, id);
          i++;
        }
        else{
          continue;
        }
      }
      else{
        continue;
      }
    }
  }
}

void RUN::DoMaxDisSelection()
{
  MaxDis(m, nobjects, metric, &uiv, &scientifisignal);
}

void RUN::DoMDCSelection()
{
  MDC(m, nobjects, metric, &uiv, &scientifisignal);
}

void RUN::DoLDAPrediction()
{
  uivector *py = ldamodel->getLastLDAPrediction()->getPredClasses();
  matrix *pfeatures = ldamodel->getLastLDAPrediction()->getPredFeatures();
  matrix *probpred = ldamodel->getLastLDAPrediction()->getProbPred();
  matrix *mvnpfd = ldamodel->getLastLDAPrediction()->getMVNProbDistrib();
  LDAPrediction(x, ldamodel->Model(), &pfeatures, &probpred, &mvnpfd, &py);
}

void RUN::DoLDAValidation()
{
  DelDVector(&ldamodel->Model()->sens);
  DelDVector(&ldamodel->Model()->spec);
  DelDVector(&ldamodel->Model()->ppv);
  DelDVector(&ldamodel->Model()->npv);
  DelDVector(&ldamodel->Model()->acc);

  initDVector(&ldamodel->Model()->sens);
  initDVector(&ldamodel->Model()->spec);
  initDVector(&ldamodel->Model()->ppv);
  initDVector(&ldamodel->Model()->npv);
  initDVector(&ldamodel->Model()->acc);

  if(vt == LOO){ // Leave One Out
    LDALOOCV(x, uiv,
                      &ldamodel->Model()->sens,
                      &ldamodel->Model()->spec,
                      &ldamodel->Model()->ppv,
                      &ldamodel->Model()->npv,
                      &ldamodel->Model()->acc,
                      QThread::idealThreadCount(), &scientifisignal);
  }
  else if(vt == RANDOMGROUP){ // Cross Validation
    LDARandomGroupsCV(x, uiv,
                      ngroup, niter,
                      &ldamodel->Model()->sens,
                      &ldamodel->Model()->spec,
                      &ldamodel->Model()->ppv,
                      &ldamodel->Model()->npv,
                      &ldamodel->Model()->acc,
                      QThread::idealThreadCount(), &scientifisignal);
  }
}

void RUN::DoLDA()
{
  LDA(x, uiv, ldamodel->Model());
}

void RUN::DoMLRPrediction()
{
  if(y != 0 && y->row > 0 && y->col > 0){
    MLRPredictY(x, y, mlrmodel->Model(),
                mlrmodel->getLastMLRPrediction()->YDipVarPointer(),
                NULL,
                mlrmodel->getLastMLRPrediction()->R2YPointer(),
                mlrmodel->getLastMLRPrediction()->SDECPointer());
  }
  else{
    MLRPredictY(x, NULL, mlrmodel->Model(),
                mlrmodel->getLastMLRPrediction()->YDipVarPointer(),
                NULL,
                NULL,
                NULL);
  }
}

void RUN::DoMLRValidation()
{
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

  if(vt == LOO){ // Leave One Out
    MLRLOOCV(x, y,
                      &mlrmodel->Model()->q2y,
                      &mlrmodel->Model()->sdep,
                      &mlrmodel->Model()->bias,
                      &mlrmodel->Model()->predicted_y,
                      &mlrmodel->Model()->pred_residuals, &scientifisignal);
  }
  else if(vt == RANDOMGROUP){ // Cross Validation
    MLRRandomGroupsCV(x, y,
                      ngroup, niter,
                      &mlrmodel->Model()->q2y,
                      &mlrmodel->Model()->sdep,
                      &mlrmodel->Model()->bias,
                      &mlrmodel->Model()->predicted_y,
                      &mlrmodel->Model()->pred_residuals, &scientifisignal);
  }

  if(yscrambling == true){
    if(vt == LOO){
      MLRYScrambling(x, y,
                          block, 0, 0, 0,
                          &mlrmodel->Model()->r2q2scrambling, &scientifisignal);
    }
    else{
      MLRYScrambling(x, y,
                        block, 1, ngroup, niter,
                        &mlrmodel->Model()->r2q2scrambling, &scientifisignal);
    }
  }
}

void RUN::DoMLR()
{
  MLR(x, y, mlrmodel->Model(), &scientifisignal);
}

void RUN::DoUPLSVariableSelection()
{
  int validtype;
  if(vt == LOO){
    validtype = 1;
  }
  else if(vt == RANDOMGROUP){
    validtype = 2;
  }
  else{
    validtype = 0;
  }

  if(vselalgorithm == GA){
    UPLSGAVariableSelection(ax, ay, NULL, NULL,
                            xscaling, yscaling, pc, validtype, ngroup, niter,
                            population_size, fraction_of_population, mutation_rate, crossovertype, nswapping, populationconvergence,
                            vselmod->SelectedVablesPointer(), vselmod->MapPointer(), vselmod->VariableDistributionPointer(), &scientifisignal);
  }
  else{
    UPSLPSOVariableSelection(ax, ay, NULL, NULL,
                            xscaling, yscaling, pc, validtype, ngroup, niter,
                            population_size, randomness,
                             vselmod->SelectedVablesPointer(), vselmod->MapPointer(), vselmod->VariableDistributionPointer(), &scientifisignal);
  }
}

void RUN::DoUPLSPrediction()
{
  matrix *m = uplsmod->getLastUPLSPrediction()->getXPredScores();

  UPLSScorePredictor(ax,
                     uplsmod->Model(),
                     uplsmod->getNPC(),
                     &m);

  array *a = uplsmod->getLastUPLSPrediction()->getYDipVar();
  // we go from 1 because for 0 component no value can be calculated.
  for(pc = 1; pc <= uplsmod->getNPC(); pc++){

    array *py;
    initArray(&py);
    UPLSYPredictor(uplsmod->getLastUPLSPrediction()->getXPredScores(),
                  uplsmod->Model(),
                  pc,
                  &py);

    for(uint k = 0; k < py->order; k++){
      ArrayAppendMatrix(&a, py->m[k]);
    }

    DelArray(&py);
  }

  if(ay != 0 && ay->order > 0 && ay->m[0]->col > 0){
    array *r2y = uplsmod->getLastUPLSPrediction()->getR2Y();
    array *sdec = uplsmod->getLastUPLSPrediction()->getSDEC();

    UPLSRSquared(ax, ay,
                 uplsmod->Model(),
                 uplsmod->getNPC(),
                 NULL, &r2y, &sdec);
  }
}

void RUN::DoUPLSValidation()
{
  DelDVector(&uplsmod->Model()->r2x_validation);
  DelArray(&uplsmod->Model()->q2y);
  DelArray(&uplsmod->Model()->sdep);
  DelArray(&uplsmod->Model()->predicted_y);
  DelArray(&uplsmod->Model()->q2y_yscrambling);
  DelArray(&uplsmod->Model()->sdep_yscrambling);

  initDVector(&uplsmod->Model()->r2x_validation);
  initArray(&uplsmod->Model()->q2y);
  initArray(&uplsmod->Model()->sdep);
  initArray(&uplsmod->Model()->predicted_y);
  initArray(&uplsmod->Model()->q2y_yscrambling);
  initArray(&uplsmod->Model()->sdep_yscrambling);

  if(vt == LOO){ // Leave One Out
    UPLSLOOCV(ax, ay,
                      uplsmod->getXScaling(),
                      uplsmod->getYScaling(),
                      uplsmod->getNPC(),
                      NULL/*&uplsmod->Model()->r2x_validation*/,
                      &uplsmod->Model()->q2y,
                      &uplsmod->Model()->sdep,
                      &uplsmod->Model()->predicted_y,
                      &uplsmod->Model()->pred_residuals, &scientifisignal);
  }
  else if(vt == RANDOMGROUP){ // Cross Validation
    UPLSRandomGroupsCV(ax, ay,
                      uplsmod->getXScaling(),
                      uplsmod->getYScaling(),
                      uplsmod->getNPC(), ngroup, niter,
                      NULL/*&uplsmod->Model()->r2x_validation*/,
                      &uplsmod->Model()->q2y,
                      &uplsmod->Model()->sdep,
                      &uplsmod->Model()->predicted_y,
                      &uplsmod->Model()->pred_residuals, &scientifisignal);
  }

  if(yscrambling == true){
    if(vt == LOO){
      UPLSYScrambling(ax, ay,
                          uplsmod->getXScaling(),
                          uplsmod->getYScaling(),
                          uplsmod->getNPC(), block, 0, 0, 0,
                          &uplsmod->Model()->q2y_yscrambling,
                          &uplsmod->Model()->sdep_yscrambling, &scientifisignal);
    }
    else{
      UPLSYScrambling(ax, ay,
                        uplsmod->getXScaling(),
                        uplsmod->getYScaling(),
                        uplsmod->getNPC(), block, 1, ngroup, niter,
                        &uplsmod->Model()->q2y_yscrambling,
                        &uplsmod->Model()->sdep_yscrambling, &scientifisignal);
    }
  }
}

void RUN::DoUPLS()
{
  UPLS(ax, ay,
       pc, xscaling, yscaling,
       uplsmod->Model(), &scientifisignal);
  UPLSRSquared(ax,
               ay,
               uplsmod->Model(), pc,
               NULL/*&(uplsmod->Model()->r2x_model)*/,
               &(uplsmod->Model()->r2y_model),
               &(uplsmod->Model()->sdec));
}

void RUN::DoUPCAPrediction()
{
  matrix *m = upcamod->getLastUPCAPrediction()->getPredScores();
  UPCAScorePredictor(ax, upcamod->Model(), upcamod->getNPC(), &m);
}

void RUN::DoUPCA()
{
  UPCA(ax, pc, xscaling, upcamod->Model(), &scientifisignal);
}

void RUN::DoPLSVariableSelection()
{
  int validtype;
  if(vt == LOO){
    validtype = 1;
  }
  else if(vt == RANDOMGROUP){
    validtype = 2;
  }
  else{
    validtype = 0;
  }

  if(vselalgorithm == GA){
    PLSGAVariableSelection(x, y, NULL, NULL,
                       xscaling, yscaling, pc, validtype, ngroup, niter,
                       population_size, fraction_of_population, mutation_rate, crossovertype, nswapping, populationconvergence,
                       vselmod->SelectedVablesPointer(), vselmod->MapPointer(), vselmod->VariableDistributionPointer(), QThread::idealThreadCount(), &scientifisignal);
  }
  else if(vselalgorithm == PSO){
    PSLPSOVariableSelection(x, y, NULL, NULL,
                            xscaling, yscaling, pc, validtype, ngroup, niter,
                            population_size, randomness,
                            vselmod->SelectedVablesPointer(), vselmod->MapPointer(), vselmod->VariableDistributionPointer(), QThread::idealThreadCount(), &scientifisignal);
  }
  else{
    PLSSpearmannVariableSelection(x, y, NULL, NULL,
                            xscaling, yscaling, pc, validtype, ngroup, niter,
                            threshold,
                            vselmod->SelectedVablesPointer(), vselmod->MapPointer(), vselmod->VariableDistributionPointer(), QThread::idealThreadCount(), &scientifisignal);
  }
}

void RUN::DoPLSPrediction()
{
  PLSScorePredictor(x,
                    plsmod->Model(),
                    plsmod->getNPC(),
                    plsmod->getLastPLSPrediction()->XPredScoresPointer());


  // we go from 1 because for 0 component no value can be calculated.
  for(pc = 1; pc <= plsmod->getNPC(); pc++){
    matrix *y;
    initMatrix(&y);
    PLSYPredictor(plsmod->getLastPLSPrediction()->getXPredScores(),
                  plsmod->Model(),
                  pc,
                  &y);

    for(uint col = 0; col < y->col; col++){
      MatrixAppendCol(plsmod->getLastPLSPrediction()->YDipVarPointer() , getMatrixColumn(y, col));
    }

    DelMatrix(&y);
  }

  if(y != 0 && y->col > 0){ // calculate the R2 for the prediction
    PLSRSquared(x, y,
                plsmod->Model(),
                plsmod->getNPC(),
                plsmod->getLastPLSPrediction()->R2YPointer(), plsmod->getLastPLSPrediction()->SDECPointer());
  }
}

void RUN::DoPLSValidation()
{
  DelMatrix(&plsmod->Model()->q2y);
  DelMatrix(&plsmod->Model()->sdep);
  DelMatrix(&plsmod->Model()->bias);
  DelMatrix(&plsmod->Model()->predicted_y);
  DelMatrix(&plsmod->Model()->pred_residuals);

  initMatrix(&plsmod->Model()->q2y);
  initMatrix(&plsmod->Model()->sdep);
  initMatrix(&plsmod->Model()->bias);
  initMatrix(&plsmod->Model()->predicted_y);
  initMatrix(&plsmod->Model()->pred_residuals);

  if(vt == LOO){ // Leave One Out
    PLSLOOCV(x, y,
                       plsmod->getXScaling(), plsmod->getYScaling(),
                       plsmod->getNPC(),
                       &plsmod->Model()->q2y,
                       &plsmod->Model()->sdep,
                       &plsmod->Model()->bias,
                       &plsmod->Model()->predicted_y,
                       &plsmod->Model()->pred_residuals, QThread::idealThreadCount(), &scientifisignal);
  }
  else if(vt == RANDOMGROUP){ // Cross Validation
    PLSRandomGroupsCV(x, y,
                       plsmod->getXScaling(), plsmod->getYScaling(),
                       plsmod->getNPC(), ngroup, niter,
                       &plsmod->Model()->q2y,
                       &plsmod->Model()->sdep,
                       &plsmod->Model()->bias,
                       &plsmod->Model()->predicted_y,
                       &plsmod->Model()->pred_residuals,
                       QThread::idealThreadCount(),
                       &scientifisignal);
  }

  if(yscrambling == true){
    DelMatrix(&plsmod->Model()->r2q2scrambling);
    initMatrix(&plsmod->Model()->r2q2scrambling);
    if(vt == LOO){
      PLSYScrambling(x, y,
                      plsmod->getXScaling(), plsmod->getYScaling(),
                      plsmod->getNPC(), block,
                      0, 0,  0,
                      &plsmod->Model()->r2q2scrambling, QThread::idealThreadCount(), &scientifisignal);
    }
    else{
      PLSYScrambling(x, y,
                      plsmod->getXScaling(), plsmod->getYScaling(),
                      plsmod->getNPC(), block,
                      1, ngroup,  niter,
                      &plsmod->Model()->r2q2scrambling, QThread::idealThreadCount(), &scientifisignal);
    }
  }

  if(samplevalidator == 1){
    DelMatrix(&plsmod->Model()->q2_sample_validation);
    DelMatrix(&plsmod->Model()->sdep_sample_validation);
    initMatrix(&plsmod->Model()->q2_sample_validation);
    initMatrix(&plsmod->Model()->sdep_sample_validation);

    if(vt == LOO){
      if(bestsampleid == true){
        uivector *bestid;
        initUIVector(&bestid);
        PLSStaticSampleValidator(x, y, uiv,
                          plsmod->getXScaling(), plsmod->getYScaling(),
                          plsmod->getNPC(), samplevalidator_samplesize, samplevalidator_niters,
                          0, 0, QThread::idealThreadCount(),
                          &plsmod->Model()->q2_sample_validation, &plsmod->Model()->sdep_sample_validation, &bestid, &scientifisignal);
        for(size_t i = 0; i < bestid->size; i++)
          bestid_.append(bestid->data[i]);

        DelUIVector(&bestid);
      }
      else{
        PLSStaticSampleValidator(x, y, uiv,
                          plsmod->getXScaling(), plsmod->getYScaling(),
                          plsmod->getNPC(), samplevalidator_samplesize, samplevalidator_niters,
                          0, 0, QThread::idealThreadCount(),
                          &plsmod->Model()->q2_sample_validation, &plsmod->Model()->sdep_sample_validation, NULL, &scientifisignal);
      }
    }
    else{
      if(bestsampleid == true){
        uivector *bestid;
        initUIVector(&bestid);
        PLSStaticSampleValidator(x, y, uiv,
                          plsmod->getXScaling(), plsmod->getYScaling(),
                          plsmod->getNPC(), samplevalidator_samplesize, samplevalidator_niters,
                          ngroup,  niter, QThread::idealThreadCount(),
                          &plsmod->Model()->q2_sample_validation, &plsmod->Model()->sdep_sample_validation, &bestid, &scientifisignal);
        for(size_t i = 0; i < bestid->size; i++)
          bestid_.append(bestid->data[i]);

        DelUIVector(&bestid);
      }
      else{
        PLSStaticSampleValidator(x, y, uiv,
                  plsmod->getXScaling(), plsmod->getYScaling(),
                  plsmod->getNPC(), samplevalidator_samplesize, samplevalidator_niters,
                  ngroup,  niter, QThread::idealThreadCount(),
                  &plsmod->Model()->q2_sample_validation, &plsmod->Model()->sdep_sample_validation, NULL, &scientifisignal);
      }
    }
  }
  else if(samplevalidator == 0){// incremental sample validation
    DelMatrix(&plsmod->Model()->q2_sample_validation_surface);
    DelMatrix(&plsmod->Model()->sdep_sample_validation_surface);
    initMatrix(&plsmod->Model()->q2_sample_validation_surface);
    initMatrix(&plsmod->Model()->sdep_sample_validation_surface);
    if(vt == LOO){
      if(bestsampleid == true){
        uivector *bestid;
        initUIVector(&bestid);
        PLSDynamicSampleValidator(x, y,
                          plsmod->getXScaling(), plsmod->getYScaling(),
                          plsmod->getNPC(), samplevalidator_niters,
                          uiv, samplevalidator_incobj, samplevalidator_maxobj,
                          0, 0, QThread::idealThreadCount(),
                          &plsmod->Model()->q2_sample_validation_surface, &plsmod->Model()->sdep_sample_validation_surface, &bestid, &scientifisignal);

        for(size_t i = 0; i < bestid->size; i++)
          bestid_.append(bestid->data[i]);

        DelUIVector(&bestid);
      }
      else{
        PLSDynamicSampleValidator(x, y,
                          plsmod->getXScaling(), plsmod->getYScaling(),
                          plsmod->getNPC(), samplevalidator_niters,
                          uiv, samplevalidator_incobj, samplevalidator_maxobj,
                          0, 0, QThread::idealThreadCount(),
                          &plsmod->Model()->q2_sample_validation_surface, &plsmod->Model()->sdep_sample_validation_surface, NULL, &scientifisignal);
      }
    }
    else{
      if(bestsampleid == true){
        uivector *bestid;
        initUIVector(&bestid);
        PLSDynamicSampleValidator(x, y,
                  plsmod->getXScaling(), plsmod->getYScaling(),
                  plsmod->getNPC(), samplevalidator_niters,
                  uiv, samplevalidator_incobj, samplevalidator_maxobj,
                  ngroup, niter, QThread::idealThreadCount(),
                  &plsmod->Model()->q2_sample_validation_surface, &plsmod->Model()->sdep_sample_validation_surface, &bestid, &scientifisignal);
        for(size_t i = 0; i < bestid->size; i++)
          bestid_.append(bestid->data[i]);

        DelUIVector(&bestid);
      }
      else{
        PLSDynamicSampleValidator(x, y,
                  plsmod->getXScaling(), plsmod->getYScaling(),
                  plsmod->getNPC(), samplevalidator_niters,
                  uiv, samplevalidator_incobj, samplevalidator_maxobj,
                  ngroup, niter, QThread::idealThreadCount(),
                  &plsmod->Model()->q2_sample_validation_surface, &plsmod->Model()->sdep_sample_validation_surface, NULL, &scientifisignal);
      }
    }
  }

}

void RUN::DoPLS()
{
  PLS(x, y, pc, xscaling, yscaling, plsmod->Model(), &scientifisignal);

  PLSRSquared(x, y, plsmod->Model(), pc,
              &(plsmod->Model()->r2y_model),
              &(plsmod->Model()->sdec));
}

void RUN::DoPCAPrediction()
{
  PCAScorePredictor(x,
                    pcamod->Model(),
                    pcamod->getNPC(),
                    pcamod->getLastPCAPrediction()->PredScoresPointer());
}

void RUN::DoPCA()
{
  PCA(x, xscaling, pc, pcamod->Model(), &scientifisignal);
}

QFuture< void > RUN::RunClusterValidation()
{
  return QtConcurrent::run(this, &RUN::DoClusterValidation);
}

QFuture< void > RUN::RunClustering()
{
  return QtConcurrent::run(this, &RUN::DoClustering);
}

QFuture< void > RUN::RunRandomSelection()
{
  return QtConcurrent::run(this, &RUN::DoRandomSelection);
}

QFuture< void > RUN::RunMaxDisSelection()
{
  return QtConcurrent::run(this, &RUN::DoMaxDisSelection);
}

QFuture< void > RUN::RunMDCSelection()
{
  return QtConcurrent::run(this, &RUN::DoMDCSelection);
}

QFuture< void > RUN::RunLDAPrediction()
{
  return QtConcurrent::run(this, &RUN::DoLDAPrediction);
}

QFuture< void > RUN::RunLDAValidation()
{
  return QtConcurrent::run(this, &RUN::DoLDAValidation);
}

QFuture< void > RUN::RunLDA()
{
  return QtConcurrent::run(this, &RUN::DoLDA);
}

QFuture< void > RUN::RunMLRPrediction()
{
  return QtConcurrent::run(this, &RUN::DoMLRPrediction);
}

QFuture< void > RUN::RunMLRValidation()
{
  return QtConcurrent::run(this, &RUN::DoMLRValidation);
}

QFuture< void > RUN::RunMLR()
{
  return QtConcurrent::run(this, &RUN::DoMLR);
}

QFuture< void > RUN::RunUPLSVariableSelection()
{
  return QtConcurrent::run(this, &RUN::DoUPLSVariableSelection);
}

QFuture< void > RUN::RunUPLSPrediction()
{
  return QtConcurrent::run(this, &RUN::DoUPLSPrediction);
}

QFuture< void > RUN::RunUPLSValidation()
{
  return QtConcurrent::run(this, &RUN::DoUPLSValidation);
}

QFuture< void > RUN::RunUPLS()
{
  return QtConcurrent::run(this, &RUN::DoUPLS);
}

QFuture< void > RUN::RunUPCAPrediction()
{
  return QtConcurrent::run(this, &RUN::DoUPCAPrediction);
}

QFuture< void > RUN::RunUPCA()
{
  return QtConcurrent::run(this, &RUN::DoUPCA);
}

QFuture< void > RUN::RunPLSVariableSelection()
{
  return QtConcurrent::run(this, &RUN::DoPLSVariableSelection);
}

QFuture< void > RUN::RunPLSPrediction()
{
  return QtConcurrent::run(this, &RUN::DoPLSPrediction);
}

QFuture< void > RUN::RunPLSValidation()
{
  return QtConcurrent::run(this, &RUN::DoPLSValidation);
}

QFuture< void > RUN::RunPLS()
{
  return QtConcurrent::run(this, &RUN::DoPLS);
}

QFuture< void > RUN::RunPCAPrediction()
{
  return QtConcurrent::run(this, &RUN::DoPCAPrediction);
}

QFuture< void > RUN::RunPCA()
{
  return QtConcurrent::run(this, &RUN::DoPCA);
}

void RUN::setNMaxClusters(int nmaxclusters_)
{
  nmaxclusters = nmaxclusters_;
}

void RUN::setNMaxObjects(int nmaxobjects_)
{
  nmaxobjects = nmaxobjects_;
}

void RUN::setObjectSelectionType(int objselectiontype_)
{
  objselectiontype = objselectiontype_;
}

void RUN::setLinkType(int linktype_)
{
  linktype = linktype_;
}

void RUN::setClusteringAlgorithm(int clusteralgoritm_)
{
  clusteralgoritm = clusteralgoritm_;
}

void RUN::setNumberOfCluster(uint nclusters_)
{
  nclusters = nclusters_;
}

void RUN::setNumberMaxOfObject(uint maxnobjects_)
{
  maxnobjects = maxnobjects_;
}

void RUN::setNumberOfObject(uint nobjects_)
{
  nobjects = nobjects_;
}

void RUN::setMetric(int metric_)
{
  if(metric_ == EUCLIDEANDISTANCE){
    metric = 0;
  }
  else if(metric_ == MANHATTANDISTANCE){
    metric = 1;
  }
  else{
    metric = 2;
  }
}

void RUN::setDendogramVector(strvector* dendogram_)
{
  dendogram = dendogram_;
}

void RUN::setDVector(dvector* dv_)
{
  dv = dv_;
}

void RUN::setUIVector(uivector* uiv_)
{
  uiv = uiv_;
}

void RUN::setMatrix(matrix* m_)
{
  m = m_;
}

void RUN::setThreshold(double threshold_)
{
  threshold = threshold_;
}

void RUN::setRandomizedVariables(int randomness_){
  randomness = randomness_;
}

void RUN::setPopulationConvergence(double populationconvergence_)
{
  populationconvergence = populationconvergence_;
}

void RUN::setNumbersOfSwappiness(double nswapping_)
{
  nswapping = nswapping_;
}

void RUN::setCrossoverType(int crossovertype_)
{
  crossovertype = crossovertype_;
}

void RUN::setMutationRate(double mutation_rate_)
{
  mutation_rate = mutation_rate_;
}

void RUN::setCrossoverFraction(double fraction_of_population_)
{
  fraction_of_population = fraction_of_population_;
}

void RUN::setPopulationSize(int population_size_)
{
  population_size = population_size_;
}

void RUN::setVariableSelectionAlgorithm(int vselalgorithm_)
{
  vselalgorithm = vselalgorithm_;
}

void RUN::setModelYScrambling(bool yscrambling_)
{
  yscrambling = yscrambling_;
}

void RUN::setModelYScramblingBlock(int block_)
{
  block = block_;
}

void RUN::setModelSampleValidator(int samplevalidator_)
{
  samplevalidator = samplevalidator_;
}

void RUN::setModelSampleValidatorClasses ( uivector* obj_class )
{
  uiv = obj_class;
}

void RUN::setModelSampleValidatorIncObj ( int incobj )
{
  samplevalidator_incobj = incobj;
}

void RUN::setModelSampleValidatorMaxObj ( int maxobj_ )
{
  samplevalidator_maxobj = maxobj_;
}

void RUN::setModelSampleValidatorBestID ( bool status )
{
  bestsampleid = status;
}

QList< int > RUN::getModelSampleValidatorBestID()
{
  return bestid_;
}

void RUN::setModelSampleValidatorIterations ( int niters_ )
{
  samplevalidator_niters = niters_;
}

void RUN::setModelSampleValidatorSampleSize ( int sample_size_ )
{
  samplevalidator_samplesize = sample_size_;
}

void RUN::setNumberOfIterations(int niter_)
{
  niter = niter_;
}

void RUN::setNumberOfGroups(int ngroup_)
{
  ngroup = ngroup_;
}

void RUN::setValidationType(int vt_)
{
  vt = vt_;
}

void RUN::setNumberPC(int pc_)
{
  pc = pc_;
}

void RUN::setYScalingType(int yscaling_)
{
  yscaling = yscaling_;
}
void RUN::setXScalingType(int xscaling_)
{
  xscaling = xscaling_;
}

void RUN::setLDAModel(LDAModel* ldamodel_)
{
  ldamodel = ldamodel_;
}

void RUN::setMLRModel(MLRModel* mlrmodel_)
{
  mlrmodel = mlrmodel_;
}


void RUN::setVarSel(VariableSelectionModel* vselmod_)
{
  vselmod = vselmod_;
}

void RUN::setUPLSModel(UPLSModel* uplsmod_)
{
  uplsmod = uplsmod_;
}

void RUN::setUPCAModel(UPCAModel* upcamod_)
{
  upcamod = upcamod_;
}

void RUN::setPLSModel(PLSModel* plsmod_)
{
  plsmod = plsmod_;
}

void RUN::setPCAModel(PCAModel * pcamod_)
{
  pcamod = pcamod_;
}

void RUN::setYArray(array* ay_)
{
  ay = ay_;
}

void RUN::setXArray(array* ax_)
{
  ax = ax_;
}

void RUN::setYMatrix(matrix* y_)
{
  y = y_;
}

void RUN::setXMatrix(matrix* x_)
{
  x = x_;
}

void RUN::Test()
{
  qDebug() << " XSCALING " << xscaling << " YSCALING " << yscaling << " PC " << pc << " VT " << vt << " GROUP " << ngroup << " ITER " << niter;
}

void RUN::AbortRun()
{
  scientifisignal = SIGSCIENTIFICSTOP;
//   QThreadPool::globalInstance()->waitForDone();
}

RUN::RUN()
{
  scientifisignal = SIGSCIENTIFICRUN;

  pc = vt = ngroup = niter = block = nclusters = clusteralgoritm = -1;
  xscaling = yscaling = 0;
  yscrambling = false;
  samplevalidator = -1;
  samplevalidator_niters = 0;
  samplevalidator_samplesize = 0;
  samplevalidator_incobj = 1;
  m = 0;
  uiv = 0;
  x = 0;
  y = 0;
  ax = 0;
  ay = 0;
  bestsampleid = false;
  uiv = NULL;
}
