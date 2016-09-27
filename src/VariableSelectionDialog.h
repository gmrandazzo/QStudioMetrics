#ifndef VARIABLESELECTIONDIALOG_H
#define VARIABLESELECTIONDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QString>
#include <QDialogButtonBox>
#include <QStandardItemModel>

#include "ui_VariableSelectionDialog.h"
#include "data.h"

class VariableSelectionDialog : public QDialog
{
  Q_OBJECT
  
public:
  VariableSelectionDialog(PROJECTS *projects, int type_);
  ~VariableSelectionDialog();
  
  bool compute(){ return compute_; }
  
  QString& getVariableSelectionName(){ return varselname; }
  
  int getselectedProject(){ return selectedproject_; }
  int getselectedModel(){ return selectedmodel_; }
  
  int getValidType(){ return validtype; } /* 0 leave one out; 1 random group; 2 external test set*/
  
  // Random Group Cross Validation
  int getNumberOfGroup(){ return ngroup; }
  int getNumberOfIteration(){ return niter; }
  
  //get variable selectioon algorithm...
  int getVarSelectionAlgorithm(){ return varselalgo; } /* GA; PSO or SPEARMAN */
  
  int getPopulationSize(){ return popsize; }
  
  //genetic algorithm 
  double getCrossoverFraction(){ return crossover; }
  int getCrossoverType(){ return crossovertype; }
  double getCrossoverSwappiness(){ return nswappiness; }
  double getMutationRate(){ return mutationrate; }
  double getPopulationConverngence(){ return popconverge; }
  
  //PSO
  double getRandomVariables(){ return randomvar; }
  
  //SPEARMAN
  double getThreshold(){ return threshold; }
  
private slots:
  void OK();
  void setProject(QModelIndex current);
  void setModel(QModelIndex current);
  void UpdateCrossoverType();
  
private:
  Ui::VariableSelectionDialog ui;
  QString varselname;
  PROJECTS *projects_;
  QList<int> pids, mids;
  QStandardItemModel *tab1, *tab2;
  int selectedproject_, selectedmodel_;
  int type; // 0 PLSVariableSelection, 1 UPLSVariableSelection
  int validtype; // 0 LOO 1 RG
  int ngroup, niter; // Number of Principal Component
  int varselalgo;
  int popsize;
  int crossovertype;
  double crossover, nswappiness, mutationrate, popconverge, randomvar, threshold;
  bool compute_;
};

#endif

