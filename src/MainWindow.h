#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMap>
#include <QString>

#include "ui_MainWindow.h"

#include "MDIChild.h"
#include "qsmdata.h"

// Version
#define major_ 2
#define minor_ 4
#define patch_ 2

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QString confdir_, QString key_);
  ~MainWindow();

  /*used by data in OpenData*/
  static QTreeWidgetItem *getProjectItem(int pid, QTreeWidget *treeWidget);
  static QTreeWidgetItem *getModelItem(int pid, int mid, QTreeWidget *treeWidget);

protected:
  void changeEvent(QEvent *e);

private slots:
  void Test();
  void NewProject();
  void OpenProject();
  void OpenRecent1();
  void OpenRecent2();
  void OpenRecent3();
  void OpenRecent4();
  void SaveAs();
  void SaveAllProjects();
  void Quit();

  void AddRemoveObjLabel();
  void AddRemoveVarLabel();
  void ShowObjVarList(ListSignal);
  void ExtractData();
  void MergeData();
  void AdvancedPretreatment();

  void DoPCA();
  void DoPCAPrediction();
  void DoCPCA();
  void DoCPCAPrediction();
  void DoPLS(int algtype);
  void DoPLSRegression();
  void DoPLSDA();
  void DoPLSValidation();
  void DoPLSPrediction();
  void DoEPLS(int algtype);
  void DoEPLSRegression();
  void DoEPLSDA();
  void DoEPLSValidation();
  void DoEPLSPrediction();
  void DoMLR();
  void DoMLRValidation();
  void DoMLRPrediction();
  void DoLDA();
  void DoLDAValidation();
  void DoLDAPrediction();

  void PlotVariableDistribution();
  void PlotVariableVSVariable();

  void PCA2DScorePlot();
  void PCA2DLoadingsPlot();
  void PCA2DExpVarPlot();
  void PCA2DLoadingsMVANDPlot();
  void PCA2DScorePlotPrediction();

  void CPCA2DSuperScorePlot();
  void CPCA2DSuperWeightsPlot();
  void CPCA2DBlockScoresPlot();
  void CPCA2DBlockLoadingsPlot();
  void CPCA2DExpVarPlot();
  void CPCA2DSuperScoresPlotPrediction();
  void CPCA2DBlockScoresPlotPrediction();

  void PLS2DPlot();
  void PLS2DTTScorePlot();
  void PLS2DPPLoadingsPlot();
  void PLS2DWWWeightsPlot();
  void PLS2DUUScorePlot();
  void PLS2DQQLoadingsPlot();
  void PLS2DPQLoadingsPlot();
  void PLS2DTTScorePlotPrediction();
  void PLSPlotBetaCoefficients();
  void PLSPlotBetaCoeffDWPlot();
  void PLSRecalcVSExpPlotPrediction();
  void PLSPredictedVSExpAndPredictionPlot();
  void PLSRecalcVSExpPlot();
  void PLSRecalcResidualsVSExpPlot();
  void PLSPredVSExpPlot();
  void PLSPredResidualsVSExpPlot();
  void PLSPlotR2Q2();
  void PLSPlotRMSE();
  void PLSPlotROCAucs();
  void PLSPlotROCCurves();
  void PLSPlotPRAucs();
  void PLSPlotPRCurves();
  void PLSPlotR2R2Predicted();
  void PLSPlotRMSEPredicted();
  void PLSPlotYScrambling();

  void EPLSRecalcVSExpPlot();
  void EPLSRecalcResidualsVSExpPlot();
  void EPLSPredVSExpPlot();
  void EPLSPredResidualsVSExpPlot();
  void EPLSPlotR2Q2();
  void EPLSPlotSDECSDEP();
  void EPLSPlotROCAucs();
  void EPLSPlotROCCurves();
  void EPLSPlotPRAucs();
  void EPLSPlotPRCurves();

  void MLRRecalcVSExpPlot();
  void MLRRecalcResidualsVSExpPlot();
  void MLRBetaCoefficients();
  void MLRPredVSExpPlot();
  void MLRPredResidualsVSExpPlot();
  void MLRRecalcVSExpAndPredictionPlot();
  void MLRPredictedVSExpAndPredictionPlot();
  void MLRPlotYScrambling();

  void LDAFeaturePlot2D();
  void LDAProbabilityDistribution();
  void LDAROCPlot();
  void LDAPRPlot();
  void LDAFeaturePlotAndPrediction2D();
  void LDAProbabilityDistributionWithPredictions();

  void SubWindowsCascade();
  void SubWindowsTiled();
  void About();
  void ProjectWindow();
  void LogWindow();
  void ImageWindow();

  void removeProject();
  void showData();
  void showDescrpitiveStatistics();
  void addData();
  void removeData();
  void DowngradeDataID();

  void ShowContextMenu(const QPoint& pos);

  void ModelInfo();

  void showPCAScore();
  void showPCALoadings();
  void showPCAExpVar();
  void showPCAPredScore();

  void showCPCASuperScore();
  void showCPCASuperWeights();
  void showCPCABlockScores();
  void showCPCABlockLoadings();
  void showCPCAExpVar();
  void showCPCASuperScorePred();
  void showCPCABlockScoresPred();

  void showPLSTScores();
  void showPLSUSCores();
  void showPLSPLoadings();
  void showPLSQLoadings();
  void showPLSWWeights();
  void showPLSRegCoeff();
  void showPLSExpVar();
  void showPLSRecalcY();
  void showPLSValidatedPrediction();
  void showPLSValidation();
  void showPLSPredScore();
  void showPLSPrediction();
  void showPLSPredictionRSquared();

  void showEPLSRecalcY();
  void showEPLSValidatedPrediction();
  void showEPLSValidation();
  void showEPLSPrediction();

  void showMLRCoeff();
  void showMLRRecalcY();
  void showMLRValidatedPrediction();
  void showMLRValidation();
  void showMLRPrediction();
  void showMLRPredictionRSquared();

  void showLDACovarianceGroupMatrix();
  void showLDAPriorProbabilities();
  void showLDAFeatures();
  void showLDAMVNormDistrib();
  /*void showLDAProbability();*/
  void showLDAValidation();
  void showLDAPrediction();
  void showLDAPredictionFeatures();
  /*void showLDAPredictionMVNormDistrib(); */
  /*void showLDAPredictionProbability();*/

  void removeModel();
  void DowngradeModelID();
  void removePrediction();
  void DowngradePredictionID();

  void StartRun();
  void WaitRun();
  void StopRun();
  void UpdateImageWindow(ImageSignal is);
  void PlotVariableVSVariableBis(vvplotSignal vvs);

private:
  Ui::MainWindow ui;
  MDIChild *createMdiChild();
  void closeMDI(const int &id);
  void closeEvent(QCloseEvent *bar);

  QString key;
  QString confdir;
  QList<RECENTMODELS> recents;
  PROJECTS *projects;
  QString lastpath;

  int tabcount_;
  int pid_; // Prediction ID that is unique for each project and increment always
  int mid_; // model id that is unique for each model and increment always
  int calcpid; // used when a calculation is running on a pid

  bool stoprun;

  void updateLog(QString);
  void RecentsProjectSwap(RECENTMODELS m);
  QList<RECENTMODELS> GetRecentModels();
  void LoadRecentsModelsFile();
  void WriteRecentsModelsFile();
  void DebugProjectTree(ProjectTree pjtree);
  void GetPCAProjects(ProjectTree *pjtree);
  void GetCPCAProjects(ProjectTree *pjtree);
  void GetPLSProjects(ProjectTree *pjtree);
  void GetEPLSProjects(ProjectTree *pjtree);
  void GetMLRProjects(ProjectTree *pjtree);
  void GetLDAProjects(ProjectTree *pjtree);

  int ProjectOpen(QString);
  void CheckProjects();
  bool havepca,
       havepcapred,
       havecpca,
       havecpcapred,
       havepls,
       haveplspred,
       haveplsvalid,
       haveplsyscrambling,
       haveepls,
       haveeplspred,
       haveeplsvalid,
       haveeplsyscrambling,
       havemlr,
       havemlrpred,
       havemlrvalid,
       havemlryscrambling,
       havelda,
       haveldapred,
       haveldavalid;

  void TopMenuEnableDisable();
  void CalculationMenuEnable();
  void CalculationMenuDisable(int calcpid_);

  bool PrepareTensor(MATRIX *indata,
                     QStringList objnames,
                     LABELS block_varsel,
                     tensor *x);

  bool PrepareMatrix(MATRIX *indata,
                     QStringList objnames,
                     QStringList xvarsel,
                     LABELS classes,
                     matrix *x,
                     matrix *y);

  bool PrepareMatrix(MATRIX *indata,
                     QStringList objnames,
                     QStringList xvarsel,
                     QStringList yvarsel,
                     matrix *x,
                     matrix *y);

  bool PrepareMatrix(MATRIX *indata,
                     QStringList objnames,
                     QStringList varsel,
                     matrix *x);

  void PrepareKFoldClasses(QStringList objects,
                           LABELS kfclasses,
                           uivector *classes);

  bool ProjectsHavePCA(){ return havepca; }
  bool ProjectsHavePCAPrediction(){ return havepcapred; }
  bool ProjectsHaveCPCA(){ return havecpca; }
  bool ProjectsHaveCPCAPrediction(){ return havecpcapred; }
  bool ProjectsHavePLS(){ return havepls; }
  bool ProjectsHavePLSValidated(){ return haveplsvalid; }
  bool ProjectsHavePLSYScrambling(){ return haveplsyscrambling; }
  bool ProjectsHavePLSPrediction(){ return haveplspred; }

  bool ProjectsHaveEPLS(){ return haveepls; }
  bool ProjectsHaveEPLSValidated(){ return haveeplsvalid; }
  bool ProjectsHaveEPLSYScrambling(){ return haveeplsyscrambling; }
  bool ProjectsHaveEPLSPrediction(){ return haveeplspred; }

  bool ProjectsHaveMLR(){ return havemlr; }
  bool ProjectsHaveMLRValidated(){ return havemlrvalid; };
  bool ProjectsHaveMLRPrediction(){ return havemlrpred; };
  bool ProjectsHaveMLRYScrambling(){ return havemlryscrambling; }
  bool ProjectsHaveLDA(){ return havelda; }
  bool ProjectsHaveLDAValidated(){ return haveldavalid; };
  bool ProjectsHaveLDAPrediction(){ return haveldapred; };


/* QMAP <int pid, PROJECT()>
 *
 * pid - pid  (2)
 *    |
 *    | Data   // child 0
 *    |   |
 *    |   Name - Type - Tab Count - Data Project ID (Data Position) - pid  (5)
 *    |
 *    | Models  // child 1
 *        |
 *        Name - Tab Count - pid - xhash - yhash - x Scaling type - y Scaling type - number of components - Model Type - Model ID (Model Position)  (10)
 *          |
 *         ModelPrediction Name - Tab Count - pid - Model ID - xhash - yhash - Data Position - Data Type (PCA Prediction, PLS Prediction, ...) (8)
 *
 * Type could be "Matrix" or "Array"
 * Tab count is an id used to know what tables are opened and to close the deleted table
 * Data Position is used to retrieve the Data Position inside the QList<MATRIX>,  QList<Array> QList<PCAModel> etc...
 * Model Type can be PCA Model, PLS Model, LDA Model, MLR Model
 *
 * Each time that implement a model yuou have to add also other routines in:
 * - OpenProject
 * - SaveProject
 * - DowngradeModel
 * - RemoveModel
 */

  // Project Section
  QTreeWidgetItem *getCurrentProjectItem();
  QTreeWidgetItem *getProjectItem(int pid);
  QString getCurrentProjectName();
  int getCurrentProjectID();
  bool CurrentIsProject();

  // Data Section
  int getDataCount(int pid);
  QTreeWidgetItem *getDataItem(int pid, int did);
  QString getDataType(int pid, int did);
  int getDataTableID(int pid, int did);

  QString getCurrentDataName();
  QString getCurrentDataType();
  int getCurrentDataTableID();
  int getCurrentDataID();
  int getCurrentDataProjectID();
  bool CurrentIsData();

  // Model Section
  // pid is the project unique id,    mid is the model unique id
  int getModelCount(int pid);
  QTreeWidgetItem *getModelItem(int pid, int mid);
  QString getModelName(int pid, int mid);
  int getModelTableID(int pid, int mid);
  int getModelProjectID(int pid, int mid);
  QString getModelXhash(int pid, int mid);
  QString getModelYhash(int pid, int mid);
  int getModelXScalingType(int pid, int mid);
  int getModelYScalingType(int pid, int mid);
  int getModelNComponents(int pid, int mid);
  QString getModelType(int pid, int mid);
  int getModelID(int pid, int mid);
  bool IsModel(int pid, int mid);

  // used during iteration along the treeview
  // id is the array position
  QString getModelNameAt(int pid, int id);
  int getModelTableIDAt(int pid, int id);
  int getModelProjectIDAt(int pid, int id);
  int getModelXIDAt(int pid, int id);
  int getModelYIDAt(int pid, int id);
  int getModelXScalingTypeAt(int pid, int id);
  int getModelYScalingTypeAt(int pid, int id);
  int getModelNComponentsAt(int pid, int id);
  QString getModelTypeAt(int pid, int id);
  int getModelIDAt(int pid, int id);
  bool IsModelAt(int pid, int id);


  QString getCurrentModelName();
  int getCurrentModelTableID();
  int getCurrentModelProjectID();
  QString getCurrentModelXhash();
  QString getCurrentModelYhash();
  int getCurrentModelXScalingType();
  int getCurrentModelYScalingType();
  int getCurrentModelNComponents();
  QString getCurrentModelType();
  int getCurrentModelID();
  bool CurrentIsModel();

  // Prediction Section
  QString getCurrentPredictionName();
  int getCurrentPredictionTableID();
  int getCurrentPredictionProjectID();
  int getCurrentPredictionModelID();
  QString getCurrentPredictionXhash();
  QString getCurrentPredictionYhash();
  int getCurrentPredictionID();
  QString getCurrentPredictionType();
  bool CurrentIsPrediction();

  // predid is the prediction unique id
  QTreeWidgetItem *getPredictionItem(int pid, int mid, int preid);
  int getPredictionCount(int pid, int mid);
  QString getPredictionName(int pid, int mid, int predid);
  int getPredictionTableID(int pid, int mid, int predid);
  int getPredictionProjectID(int pid, int mid, int predid);
  int getPredictionModelID(int pid, int mid, int predid);
  QString getPredictionXhash(int pid, int mid, int predid);
  QString getPredictionYhash(int pid, int mid, int predid);
  int getPredictionID(int pid, int mid, int predid);
  QString getPredictionType(int pid, int mid, int predid);
  bool IsPrediction(int pid, int mid, int predid);

  // id is the model id position in the array that is different infront of mid model unique id
  // function used during iteration along the menu
  int getPredictionCountAt(int pid, int id);
  QString getPredictionNameAt(int pid, int id, int predid);
  int getPredictionTableIDAt(int pid, int id, int predid);
  int getPredictionProjectIDAt(int pid, int id, int predid);
  int getPredictionModelIDAt(int pid, int id, int predid);
  QString getPredictionXhashAt(int pid, int id, int predid);
  QString getPredictionYhashAt(int pid, int id, int predid);
  int getPredictionIDAt(int pid, int id, int predid);
  QString getPredictionTypeAt(int pid, int id, int predid);
  bool IsPredictionAt(int pid, int id, int predid);
};


#endif
