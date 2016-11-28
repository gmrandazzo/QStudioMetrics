#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiSubWindow>
#include <QMap>
#include <QString>

#include "ui_MainWindow.h"

#include "MDIChild.h"
#include "data.h"

// Version
#define major_ 1
#define minor_ 9
#define patch_ 0

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
  void Save();
  void Quit();

  void AddRemoveObjLabel();
  void AddRemoveVarLabel();
  void ShowObjVarList(ListSignal);
  void ExtractData();
  void MergeData();
  void AdvancedPretreatment();

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

  void PlotVariableDistribution();
  void PlotVariableVSVariable();

  void PCA2DScorePlot();
  void PCA2DLoadingsPlot();
  void PCA2DLoadingsMVANDPlot();
  void PCA2DScorePlotPrediction();
  void PCA3DScorePlot();
  void PCA3DLoadingsPlot();
  void PCA3DScorePlotPrediction();

  void PLS2DPlot();
  void PLS2DTTScorePlot();
  void PLS2DPPLoadingsPlot();
  void PLS2DWWWeightsPlot();
  void PLS2DUUScorePlot();
  void PLS2DQQLoadingsPlot();
  void PLS2DTTScorePlotPrediction();
  void PLSPlotBetaCoefficients();
  void PLSPlotBetaCoeffDWPlot();
  void PLSRecalcVSExpPlotPrediction();
  void PLSPredictedVSExpAndPredictionPlot();
  void PLSRecalcVSExpPlot();
  void PLSRecalcResidualsVSExpPlot();
  void PLSPredVSExpPlot();
  void PLSPredResidualsVSExpPlot();
  void PLSPlotQ2R2();
  void PLSPlotR2R2Predicted();
  void PLSPlotYScrambling();
  void PLSPlotQ2SampleValidator();
  void PLSPlotSDEPSampleValidator();

  void PLS3DTTTScorePlot();
  void PLS3DPPPLoadingsPlot();
  void PLS3DWWWLoadingsPlot();
  void PLS3DUUUScorePlot();
  void PLS3DQQQLoadingsPlot();
  void PLS3DScorePlotPrediction();
  void PLS3DPlotQ2SampleValidator();
  void PLS3DPlotSDEPSampleValidator();

  void UPCA2DScorePlot();
  void UPCA2DLoadingsPlot();
  void UPCA2DScorePlotPrediction();
  void UPCA3DScorePlot();
  void UPCA3DLoadingsPlot();
  void UPCA3DScorePlotPrediction();

  void UPLS2DPlot();
  void UPLS2DTTScorePlot();
  void UPLS2DPPLoadingsPlot();
  void UPLS2DWWWeightsPlot();
  void UPLS2DUUScorePlot();
  void UPLS2DQQLoadingsPlot();
  void UPLS2DTTScorePlotPrediction();
  void UPLSRecalcVSExpPlotPrediction();
  void UPLSPredVSExpPlotPrediction();
  void UPLSRecalcVSExpPlot();
  void UPLSRecalcResidualsVSExpPlot();
  void UPLSPredVSExpPlot();
  void UPLSPredResidualsVSExpPlot();
  void UPLSPlotQ2R2();
  void UPLSPlotR2R2Predicted();
  void UPLS3DTTTScorePlot();
  void UPLS3DPPPLoadingsPlot();
  void UPLS3DWWWLoadingsPlot();
  void UPLS3DUUUScorePlot();
  void UPLS3DQQQLoadingsPlot();
  void UPLS3DScorePlotPrediction();

  void PlotVarSelR2Q2();
  void PlotVarSelFTestNVar();
  void PlotVarSelFTestQ2();
  void PlotModIncVarVar();

  void MLRRecalcVSExpPlot();
  void MLRRecalcResidualsVSExpPlot();
  void MLRBetaCoefficients();
  void MLRPredVSExpPlot();
  void MLRPredResidualsVSExpPlot();
  void MLRRecalcVSExpAndPredictionPlot();
  void MLRPredictedVSExpAndPredictionPlot();
  void MLRPlotYScrambling();

  void LDAFeaturePlot2D();
  void LDAFeaturePlot3D();
  void LDAProbabilityDistribution();
  void LDAFeaturePlotAndPrediction2D();
  void LDAFeaturePlotAndPrediction3D();
  void LDAProbabilityDistributionWithPredictions();

  void SubWindowsCascade();
  void SubWindowsTiled();
  void About();
  void ProjectWindow();
  void LogWindow();
  void ImageWindow();

  void removeProject();
  void DowngradeProjectID();
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
  void showPLSVarSelMap();

  void showUPCAScore();
  void showUPCALoadings();
  void showUPCAExpVar();
  void showUPCAPredScore();

  void showUPLSTScores();
  void showUPLSUSCores();
  void showUPLSPLoadings();
  void showUPLSQLoadings();
  void showUPLSWWeights();
  void showUPLSRegCoeff();
  void showUPLSExpVar();
  void showUPLSValidation();
  void showUPLSValidatedPrediction();
  void showUPLSRecalcY();
  void showUPLSPredScore();
  void showUPLSPrediction();
  void showUPLSPredictionRSquared();
  void showUPLSVarSelMap();

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

  QString key;
  QString confdir;
  QStringList recents;
  PROJECTS *projects;
//   QMap<int, DATA*> *projects; // ID and PROJECT
  QString lastpath;

  int tabcount_;
  int pid_; // Prediction ID that is unique for each project and increment always
  int mid_; // model id that is unique for each model and increment always
  int calcpid; // used when a calculation is running on a pid

  bool stoprun;

  void updateLog(QString);

  int ProjectOpen(QString);
  void CheckProjects();
  bool havepca,
       havepcapred,
       havepls,
       haveplspred,
       haveplsvalid,
       haveplsyscrambling,
       haveplsstaticsamplevalidation,
       haveplsdynamicsamplevalidation,
       haveupca,
       haveupcapred,
       haveupls,
       haveuplspred,
       haveuplsvalid,
       havemlr,
       havemlrpred,
       havemlrvalid,
       havemlryscrambling,
       havevarsel,
       havelda,
       haveldapred,
       haveldavalid;

  void TopMenuEnableDisable();
  void CalculationMenuEnable();
  void CalculationMenuDisable(int calcpid_);

  bool ProjectsHavePCA(){ return havepca; };
  bool ProjectsHavePCAPrediction(){ return havepcapred; };
  bool ProjectsHavePLS(){ return havepls; };
  bool ProjectsHavePLSValidated(){ return haveplsvalid; };
  bool ProjectsHavePLSYScrambling(){ return haveplsyscrambling; }
  bool ProjectsHavePLSStaticSampleValidation(){ return haveplsstaticsamplevalidation; }
  bool ProjectsHavePLSDynamicSampleValidation(){ return haveplsdynamicsamplevalidation; }
  bool ProjectsHavePLSPrediction(){ return haveplspred; };
  bool ProjectsHaveUPCA(){ return haveupca; };
  bool ProjectsHaveUPCAPrediction(){ return haveupcapred; };
  bool ProjectsHaveUPLS(){ return haveupls; };
  bool ProjectsHaveUPLSValidated(){ return haveuplsvalid; };
  bool ProjectsHaveUPLSPrediction(){ return haveuplspred; };
  bool ProjectsHaveMLR(){ return havemlr; }
  bool ProjectsHaveMLRValidated(){ return havemlrvalid; };
  bool ProjectsHaveMLRPrediction(){ return havemlrpred; };
  bool ProjectsHaveMLRYScrambling(){ return havemlryscrambling; }
  bool ProjectsHaveVariableSelection(){ return havevarsel; };
  bool ProjectsHaveLDA(){ return havelda; }
  bool ProjectsHaveLDAValidated(){ return haveldavalid; };
  bool ProjectsHaveLDAPrediction(){ return haveldapred; };

  // Used to interact with lateral menù


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
 *         ModelPrediction Name - Tab Count - pid - Model ID - xhash - yhash - Data Position - Data Type (PCA Prediction, UPCA Prediction, ...) (8)
 *
 * Type could be "Matrix" or "Array"
 * Tab count is an id used to know what tables are opened and to close the deleted table
 * Data Position is used to retrieve the Data Position inside the QList<MATRIX>,  QList<Array> QList<PCAModel> etc...
 * Model Type can be PCA Model, PLS Model, UPCA Model, UPLS Model
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
