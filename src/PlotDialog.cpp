#include "PlotDialog.h"
#include <QModelIndex>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QDebug>
#include "qstudiometricstypes.h"

void PlotDialog::CheckPlot()
{
  if(selectedproject_ != -1){
    if(type == PCA_
      || type == UPCA_
      || type == PLS_
      || type == UPLS_
      || type == MLR_
      || type == LDA_
      || type == VarSel
      || type == PLSValidation
      || type == UPLSValidation
      || type == PLSRecalcVSExperimental
      || type == UPLSRecalcVSExperimental
      || type == PLSPredictedVSExperimental
      || type == UPLSPredictedVSExperimental
      || type == MLRRecalcVSExperimental
      || type == MLRPredictedVSExperimental
      || type == PLSYSCRAMBLING
      || type == MLRYSCRAMBLING
      || type == PLSQ2SDEPSTATICSAMPLEVALIDATION
      || type == PLSQ2SDEPDYNAMICSAMPLEVALIDATION){
      if(modelid != -1){
        ui.plotButton->setEnabled(true);
      }
      else{
        ui.plotButton->setEnabled(false);
      }
    }
    else if(type == PCAPrediction
      || type == UPCAPrediction
      || type == PLSPrediction
      || type == MLRPrediction
      || type == LDAPrediction_
      || type == PLSR2R2Plot
      || type == UPLSPrediction
      || type == UPLSR2R2Plot
      || type == PLSRecalcVSExperimentalWithPrediction
      || type == PLSPredictedVSExperimentalWithPrediction
      || type == UPLSRecalcVSExperimentalWithPrediction
      || type == UPLSPredictedVSExperimentalWithPrediction
      || type == MLRRecalcVSExperimentalWithPrediction
      || type == MLRPredictedVSExperimentalWithPrediction){
      if(modelid != -1 && predid != -1){
        ui.plotButton->setEnabled(true);
      }
      else{
        ui.plotButton->setEnabled(false);
      }
    }
  }
  else{
    ui.plotButton->setEnabled(false);
  }

}

void PlotDialog::setNPC()
{
  npc = ui.numpc->value();
}

void PlotDialog::setPredictionID(QModelIndex current)
{
  if(current.isValid()){
    predid = predids[current.row()];
  }
  else{
    predid = -1;
  }
  CheckPlot();
}


void PlotDialog::setModelID(QModelIndex current)
{
  if(current.isValid() && selectedproject_ != -1){
    modelid = mids[current.row()];
    if(tab3){
      tab3->clear();
      predids.clear();
      if(type == PCAPrediction){ // PCA Prediction
        for(int i = 0; i < projects_->value(selectedproject_)->getPCAModel(modelid)->PCAPredictionCount(); i++){
          predids.append(projects_->value(selectedproject_)->getPCAModel(modelid)->getPCAPrediction(i)->getPredID());
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPCAModel(modelid)->getPCAPrediction(i)->getName()));
          tab3->appendRow(row);
        }
      }
      else if(type == PLSPrediction){ // PLS Prediction
        for(int i = 0; i < projects_->value(selectedproject_)->getPLSModel(modelid)->PLSPredictionCount(); i++){
          predids.append(projects_->value(selectedproject_)->getPLSModel(modelid)->getPLSPrediction(i)->getPredID());
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModel(modelid)->getPLSPrediction(i)->getName()));
          tab3->appendRow(row);
        }
      }
      else if(type == PLSR2R2Plot){ // PLS Prediction R2R2
        for(int i = 0; i < projects_->value(selectedproject_)->getPLSModel(modelid)->PLSPredictionCount(); i++){
          if(projects_->value(selectedproject_)->getPLSModel(modelid)->getPLSPrediction(i)->getR2Y()->row > 0){
            predids.append(projects_->value(selectedproject_)->getPLSModel(modelid)->getPLSPrediction(i)->getPredID());
            QList<QStandardItem*> row;
            row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModel(modelid)->getPLSPrediction(i)->getName()));
            tab3->appendRow(row);
          }
          else{
            continue;
          }
        }
      }
      else if(type == PLSRecalcVSExperimentalWithPrediction || type == PLSPredictedVSExperimentalWithPrediction){ // PLS Recalcualted VS Experimental with Predictions
        for(int i = 0; i < projects_->value(selectedproject_)->getPLSModel(modelid)->PLSPredictionCount(); i++){
          if(projects_->value(selectedproject_)->getPLSModel(modelid)->getPLSPrediction(i)->getR2Y()->row > 0){
            QList<QStandardItem*> row;
            row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModel(modelid)->getPLSPrediction(i)->getName()));
            predids.append(projects_->value(selectedproject_)->getPLSModel(modelid)->getPLSPrediction(i)->getPredID());
            tab3->appendRow(row);
          }
          else{
            continue;
          }
        }
      }
      else if(type == UPCAPrediction){ // UPCA Prediction
        for(int i = 0; i < projects_->value(selectedproject_)->getUPCAModel(modelid)->UPCAPredictionCount(); i++){
          predids.append(projects_->value(selectedproject_)->getUPCAModel(modelid)->getUPCAPrediction(i)->getPredID());
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getUPCAModel(modelid)->getUPCAPrediction(i)->getName()));
          tab3->appendRow(row);
        }
      }
      else if(type == UPLSPrediction){ // UPLS Prediction
        for(int i = 0; i < projects_->value(selectedproject_)->getUPLSModel(modelid)->UPLSPredictionCount(); i++){
          predids.append(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getPredID());
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getName()));
          tab3->appendRow(row);
        }
      }
      else if(type == UPLSR2R2Plot){ // PLS Prediction
        for(int i = 0; i < projects_->value(selectedproject_)->getUPLSModel(modelid)->UPLSPredictionCount(); i++){
          if(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getR2Y()->order > 0){
            predids.append(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getPredID());
            QList<QStandardItem*> row;
            row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getName()));
            tab3->appendRow(row);
          }
          else{
            continue;
          }
        }
      }
      else if(type == UPLSRecalcVSExperimental){ // UPLS Predicted VS Experimental
        for(int i = 0; i < projects_->value(selectedproject_)->getUPLSModel(modelid)->UPLSPredictionCount(); i++){
          if(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getR2Y()->order > 0){
            QList<QStandardItem*> row;
            row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getName()));
            predids.append(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getPredID());
            tab3->appendRow(row);
          }
          else{
            continue;
          }
        }
      }
      else if(type == UPLSRecalcVSExperimentalWithPrediction || type == UPLSPredictedVSExperimentalWithPrediction){
        for(int i = 0; i < projects_->value(selectedproject_)->getUPLSModel(modelid)->UPLSPredictionCount(); i++){
          if(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getR2Y()->order > 0){
            QList<QStandardItem*> row;
            row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getName()));
            predids.append(projects_->value(selectedproject_)->getUPLSModel(modelid)->getUPLSPrediction(i)->getPredID());
            tab3->appendRow(row);
          }
          else{
            continue;
          }
        }
      }
      else if(type == MLRPrediction){ // MLR Prediction
        for(int i = 0; i < projects_->value(selectedproject_)->getMLRModel(modelid)->MLRPredictionCount(); i++){
          predids.append(projects_->value(selectedproject_)->getMLRModel(modelid)->getMLRPrediction(i)->getPredID());
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getMLRModel(modelid)->getMLRPrediction(i)->getName()));
          tab3->appendRow(row);
        }
      }
      else if(type == MLRRecalcVSExperimentalWithPrediction || type == MLRPredictedVSExperimentalWithPrediction){ // MLR Recalcualted VS Experimental with Predictions
        for(int i = 0; i < projects_->value(selectedproject_)->getMLRModel(modelid)->MLRPredictionCount(); i++){
          if(projects_->value(selectedproject_)->getMLRModel(modelid)->getMLRPrediction(i)->getYDipVar()->row > 0){
            QList<QStandardItem*> row;
            row.append(new QStandardItem(projects_->value(selectedproject_)->getMLRModel(modelid)->getMLRPrediction(i)->getName()));
            predids.append(projects_->value(selectedproject_)->getMLRModel(modelid)->getMLRPrediction(i)->getPredID());
            tab3->appendRow(row);
          }
          else{
            continue;
          }
        }
      }
      else if(type == LDAPrediction_){ // LDA Prediction
        for(int i = 0; i < projects_->value(selectedproject_)->getLDAModel(modelid)->LDAPredictionCount(); i++){
          predids.append(projects_->value(selectedproject_)->getLDAModel(modelid)->getLDAPrediction(i)->getPredID());
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getLDAModel(modelid)->getLDAPrediction(i)->getName()));
          tab3->appendRow(row);
        }
      }
    }
  }
  else{
    modelid = -1;
  }
  CheckPlot();
}

void PlotDialog::setProject(QModelIndex current)
{
  if(current.isValid()){
    // set project
    selectedproject_ = pids[current.row()];
    // create the value for the second tab
    tab2->clear();
    mids.clear();

    if(type == PCA_ || type == PCAPrediction){ // pca
      for(int i = 0; i < projects_->value(selectedproject_)->PCACount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getPCAModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getPCAModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == PLS_ || type == PLSPrediction || type == PLSR2R2Plot){ // pls
      for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == PLSValidation){ // pls validation
      for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
        if(projects_->value(selectedproject_)->getPLSModelAt(i)->getValidation() > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == PLSYSCRAMBLING){
      for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
        if(projects_->value(selectedproject_)->getPLSModelAt(i)->Model()->r2q2scrambling->row > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == PLSQ2SDEPSTATICSAMPLEVALIDATION){
      for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
        if(projects_->value(selectedproject_)->getPLSModelAt(i)->Model()->q2_sample_validation->row > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == PLSQ2SDEPDYNAMICSAMPLEVALIDATION){
      for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
        if(projects_->value(selectedproject_)->getPLSModelAt(i)->Model()->q2_sample_validation_surface->row > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == PLSRecalcVSExperimental || type == PLSRecalcVSExperimentalWithPrediction){
      for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == PLSPredictedVSExperimental || type == PLSPredictedVSExperimentalWithPrediction){ // PLS Predicted  VS Experimental
      for(int i = 0; i < projects_->value(selectedproject_)->PLSCount(); i++){
        if(projects_->value(selectedproject_)->getPLSModelAt(i)->getValidation() > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getPLSModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == UPCA_ || type == UPCAPrediction){ // upca
      for(int i = 0; i < projects_->value(selectedproject_)->UPCACount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getUPCAModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getUPCAModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == UPLS_ || type == UPLSPrediction){ // upls
      for(int i = 0; i < projects_->value(selectedproject_)->UPLSCount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getUPLSModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == UPLSRecalcVSExperimental || type == UPLSRecalcVSExperimentalWithPrediction){
      for(int i = 0; i < projects_->value(selectedproject_)->UPLSCount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getUPLSModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == UPLSPredictedVSExperimental || type == UPLSPredictedVSExperimentalWithPrediction || type == UPLSR2R2Plot){ // UPLS Predicted VS Experimental
      for(int i = 0; i < projects_->value(selectedproject_)->UPLSCount(); i++){
        if(projects_->value(selectedproject_)->getUPLSModelAt(i)->getValidation() > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getUPLSModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == UPLSValidation){ // upls validation
      for(int i = 0; i < projects_->value(selectedproject_)->UPLSCount(); i++){
        if(projects_->value(selectedproject_)->getUPLSModelAt(i)->getValidation() > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getUPLSModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getUPLSModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == VarSel){ // Variable Selection Plot
      for(int i = 0; i < projects_->value(selectedproject_)->VarSelCount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getVarSelModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getVarSelModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == MLRRecalcVSExperimental || type == MLRRecalcVSExperimentalWithPrediction){
      for(int i = 0; i < projects_->value(selectedproject_)->MLRCount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getMLRModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getMLRModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
    else if(type == MLRPredictedVSExperimental || type == MLRPredictedVSExperimentalWithPrediction){ // MLR Predicted  VS Experimental
      for(int i = 0; i < projects_->value(selectedproject_)->MLRCount(); i++){
        if(projects_->value(selectedproject_)->getMLRModelAt(i)->getValidation() > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getMLRModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getMLRModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == MLRYSCRAMBLING){
      for(int i = 0; i < projects_->value(selectedproject_)->MLRCount(); i++){
        if(projects_->value(selectedproject_)->getMLRModelAt(i)->Model()->r2q2scrambling->row > 0){
          QList<QStandardItem*> row;
          row.append(new QStandardItem(projects_->value(selectedproject_)->getMLRModelAt(i)->getName()));
          mids.append(projects_->value(selectedproject_)->getMLRModelAt(i)->getModelID());
          tab2->appendRow(row);
        }
      }
    }
    else if(type == LDA_ || type == LDAPrediction_){ // pca
      for(int i = 0; i < projects_->value(selectedproject_)->LDACount(); i++){
        QList<QStandardItem*> row;
        row.append(new QStandardItem(projects_->value(selectedproject_)->getLDAModelAt(i)->getName()));
        mids.append(projects_->value(selectedproject_)->getLDAModelAt(i)->getModelID());
        tab2->appendRow(row);
      }
    }
  }
  else{
    selectedproject_ = -1;
  }
  CheckPlot();
}

void PlotDialog::actionPlot()
{
  if(type == PCA_ ||
     type == PLS_ ||
     type == UPCA_ ||
     type == UPLS_ ||
     type == MLR_ ||
     type == LDA_ ||
     type == PLSValidation ||
     type == UPLSValidation ||
     type == MLRValidation ||
     type == LDAValidation ||
     type == PLSRecalcVSExperimental ||
     type == UPLSRecalcVSExperimental ||
     type == PLSPredictedVSExperimental ||
     type == UPLSPredictedVSExperimental ||
     type == VarSel ||
     type == MLRRecalcVSExperimental ||
     type == MLRPredictedVSExperimental ||
     type == PLSYSCRAMBLING ||
     type == MLRYSCRAMBLING ||
     type == PLSQ2SDEPSTATICSAMPLEVALIDATION ||
     type == PLSQ2SDEPDYNAMICSAMPLEVALIDATION){
    if(selectedproject_ == -1 || modelid == -1){
      QMessageBox::warning(this, tr("Warning!"), tr("Please select project and model to plot.\n"), QMessageBox::Close);
      return;
    }
    else{
      plot_ = true;
      accept();
    }
  }
  else{
    if(selectedproject_ == -1|| modelid == -1 || predid == -1){
      QMessageBox::warning(this, tr("Warning!"), tr("Please select project, model and prediction to plot.\n"), QMessageBox::Close);
      return;
    }
    else{
      plot_ = true;
      accept();
    }
  }
}


PlotDialog::PlotDialog(PROJECTS *projects, int type_)
{
  ui.setupUi(this);

  ui.listView->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui.listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

  projects_ = projects;
  plot_ = false;
  type = type_;
  selectedproject_ = modelid = predid = -1;
  npc = ui.numpc->value();

  tab1 = new QStandardItemModel();
  tab2 = new QStandardItemModel();

  ui.listView->setModel(tab1);
  ui.listView_2->setModel(tab2);

  if(type == PCA_ ||
     type == PLS_ ||
     type == UPCA_ ||
     type == UPLS_ ||
     type == MLR_ ||
     type == LDA_ ||
     type == PLSValidation ||
     type == UPLSValidation ||
     type == VarSel ||
     type == MLRRecalcVSExperimental ||
     type == MLRPredictedVSExperimental){

    ui.predictionGroupBox->hide();
    ui.label_4->hide();
    ui.numpc->hide();
    tab3 = 0;
  }
  else if(type == PCAPrediction ||
          type == PLSPrediction ||
          type == MLRPrediction ||
          type == LDAPrediction_ ||
          type == PLSR2R2Plot ||
          type == UPCAPrediction ||
          type == UPLSPrediction ||
          type == UPLSR2R2Plot ||
          type == MLRRecalcVSExperimentalWithPrediction ||
          type == MLRPredictedVSExperimentalWithPrediction){

    ui.label_4->hide();
    ui.numpc->hide();
    tab3 = new QStandardItemModel();
    ui.listView_3->setModel(tab3);
  }
  else if(type == PLSRecalcVSExperimental ||
          type == UPLSRecalcVSExperimental ||
          type == PLSPredictedVSExperimental ||
          type == UPLSPredictedVSExperimental){

    ui.predictionGroupBox->hide();
    ui.label_4->show();
    ui.numpc->show();
    tab3 = 0;
  }
  else if(type == PLSYSCRAMBLING ||
          type == MLRYSCRAMBLING ||
          type == PLSQ2SDEPSTATICSAMPLEVALIDATION ||
          type == PLSQ2SDEPDYNAMICSAMPLEVALIDATION){
    ui.predictionGroupBox->hide();
    ui.label_4->hide();
    ui.numpc->hide();
    tab3 = 0;
  }
  else{
    tab3 = new QStandardItemModel();
    ui.listView_3->setModel(tab3);
  }

  for(int i = 0; i < projects_->keys().size(); i++){
    int pid = projects_->keys()[i];
    bool acquire = false;
    if((type == PCA_ || type == PCAPrediction) && projects_->value(pid)->PCACount() > 0){ // PCA
      acquire = true;
    }
    else if((type == PLS_ ||
            type == PLSValidation ||
            type == PLSPrediction ||
            type == PLSR2R2Plot ||
            type == PLSYSCRAMBLING ||
            type == PLSQ2SDEPSTATICSAMPLEVALIDATION ||
            type == PLSQ2SDEPDYNAMICSAMPLEVALIDATION) && projects_->value(pid)->PLSCount() > 0){ // PLS
      if(type == PLSYSCRAMBLING){
        for(int i = 0; i < projects_->value(pid)->PLSCount(); i++){
          if(projects_->value(pid)->getPLSModelAt(i)->Model()->r2q2scrambling->row > 0){
            acquire = true;
            break;
          }
          else{
            continue;
          }
        }
      }
      else if(type == PLSQ2SDEPSTATICSAMPLEVALIDATION){
        for(int i = 0; i < projects_->value(pid)->PLSCount(); i++){
          if(projects_->value(pid)->getPLSModelAt(i)->Model()->q2_sample_validation->row > 0){
            acquire = true;
            break;
          }
          else{
            continue;
          }
        }
      }
      else if(type == PLSQ2SDEPDYNAMICSAMPLEVALIDATION){
        for(int i = 0; i < projects_->value(pid)->PLSCount(); i++){
          if(projects_->value(pid)->getPLSModelAt(i)->Model()->q2_sample_validation_surface->row > 0){
            acquire = true;
            break;
          }
          else{
            continue;
          }
        }
      }
      else{
        acquire = true;
      }
    }
    else if(type == PLSRecalcVSExperimental && projects_->value(pid)->PLSCount() > 0){ // PLS
      acquire = true;
    }
    else if(type == PLSPredictedVSExperimental && projects_->value(pid)->PLSCount() > 0){ // PLS
      for(int j = 0; j < projects_->value(pid)->PLSCount(); j++){ // almost one model must be validated!
        if(projects_->value(pid)->getPLSModelAt(j)->getValidation() > 0){
          acquire = true;
          break;
        }
        else{
          continue;
        }
      }
    }
    else if(type == PLSRecalcVSExperimentalWithPrediction || type == PLSPredictedVSExperimentalWithPrediction){
      for(int j = 0; j < projects_->value(pid)->PLSCount(); j++){ // almost one model must ave a prediction
        if(projects_->value(pid)->getPLSModelAt(j)->PLSPredictionCount() > 0){
          acquire = true;
          break;
        }
        else{
          continue;
        }
      }
    }
    else if((type == UPCA_ || type == UPCAPrediction) && projects_->value(pid)->UPCACount() > 0){ // UPCA
      acquire = true;
    }
    else if((type == UPLS_ ||
             type == UPLSValidation ||
             type == UPLSPrediction ||
             type == UPLSR2R2Plot) && projects_->value(pid)->UPLSCount() > 0){ // UPLS
      acquire = true;
    }
    else if(type == UPLSRecalcVSExperimental && projects_->value(pid)->UPLSCount() > 0){ // UPLS
      acquire = true;
    }
    else if(type == UPLSPredictedVSExperimental && projects_->value(pid)->UPLSCount() > 0){ // UPLS
      for(int j = 0; j < projects_->value(pid)->UPLSCount(); j++){ // almost one model must be validated!
        if(projects_->value(pid)->getUPLSModelAt(j)->getValidation() > 0){
          acquire = true;
          break;
        }
        else{
          continue;
        }
      }
    }
    else if(type == UPLSRecalcVSExperimentalWithPrediction || type == UPLSPredictedVSExperimentalWithPrediction){
      for(int j = 0; j < projects_->value(pid)->UPLSCount(); j++){ // almost one model must ave a prediction!
        if(projects_->value(pid)->getUPLSModelAt(j)->UPLSPredictionCount() > 0){
          acquire = true;
          break;
        }
        else{
          continue;
        }
      }
    }
    else if(type == VarSel && projects_->value(pid)->VarSelCount() > 0){ // Variable Selection Models
      acquire = true;
    }
    else if((type == MLR_ ||
             type == MLRValidation ||
             type == MLRPrediction ||
             type == MLRYSCRAMBLING) && projects_->value(pid)->MLRCount() > 0){ // MLR
      if(type == MLRYSCRAMBLING){
        for(int i = 0; i < projects_->value(pid)->MLRCount(); i++){
          if(projects_->value(pid)->getMLRModelAt(i)->Model()->r2q2scrambling->row > 0){
            acquire = true;
            break;
          }
          else{
            continue;
          }
        }
      }
      else{
        acquire = true;
      }
    }
    else if((type == LDA_ || type == LDAValidation || type == LDAPrediction_) && projects_->value(pid)->LDACount() > 0){ // LDA
      acquire = true;
    }
    else if(type == MLRRecalcVSExperimental && projects_->value(pid)->MLRCount() > 0){ // MLR
      acquire = true;
    }
    else if(type == MLRPredictedVSExperimental && projects_->value(pid)->MLRCount() > 0){ // MLR
      for(int j = 0; j < projects_->value(pid)->MLRCount(); j++){ // almost one model must be validated!
        if(projects_->value(pid)->getMLRModelAt(j)->getValidation() > 0){
          acquire = true;
          break;
        }
        else{
          continue;
        }
      }
    }
    else if(type == MLRRecalcVSExperimentalWithPrediction || type == MLRPredictedVSExperimentalWithPrediction){
      for(int j = 0; j < projects_->value(pid)->MLRCount(); j++){ // almost one model must ave a prediction
        if(projects_->value(pid)->getMLRModelAt(j)->MLRPredictionCount() > 0){
          acquire = true;
          break;
        }
        else{
          continue;
        }
      }
    }

    if(acquire == true){
      QList<QStandardItem*> row;
      row.append(new QStandardItem(projects_->value(pid)->getProjectName()));
      tab1->appendRow(row);
      pids.append(pid);
    }
    else{
      continue;
    }
  }

  connect(ui.plotButton, SIGNAL(clicked()), SLOT(actionPlot()));
  connect(ui.cancelButton, SIGNAL(clicked()), SLOT(reject()));
  connect(ui.listView->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setProject(QModelIndex)));
  connect(ui.listView_2->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setModelID(QModelIndex)));

  if(tab3){
    connect(ui.listView_3->selectionModel(), SIGNAL(currentChanged(QModelIndex,QModelIndex)), SLOT(setPredictionID(QModelIndex)));
  }

  connect(ui.numpc, SIGNAL(valueChanged(int)), SLOT(setNPC()));
  CheckPlot();
}

PlotDialog::~PlotDialog()
{
  delete tab1;
  delete tab2;
  if(tab3){
    delete tab3;
  }
}
