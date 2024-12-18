// MainWindow
#include <QCryptographicHash>
#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QMap>
#include <QMessageBox>
#include <QProgressDialog>
#include <QString>
#include <QStringList>
#include <QtGui>

#ifdef DEBUG
#include <QDebug>
#endif

#include <scientific.h>
#include <unistd.h>

#include "MainWindow.h"
#include "qsmdata.h"
#include "run.h"

#include "CPCA/CPCAPlot.h"
#include "Dialogs/AboutDialog.h"
#include "Dialogs/AdvancedPretreatmentDialog.h"
#include "Dialogs/ClassModelDialog.h"
#include "Dialogs/ClassPlotDialog.h"
#include "Dialogs/DialogPlots.h"
#include "Dialogs/DoPredictionDialog.h"
#include "Dialogs/ExtractDataDialog.h"
#include "Dialogs/GenericProgressDialog.h"
#include "Dialogs/LabelDialog.h"
#include "Dialogs/MergeDataDialog.h"
#include "Dialogs/ModelDialog.h"
#include "Dialogs/ModelDialogWizard.h"
#include "Dialogs/ProjectManager.h"
#include "Dialogs/SaveDialog.h"
#include "Dialogs/ValidatorDialog.h"
#include "Dialogs/VariablePlotDialog.h"
#include "LDA/LDAPlot.h"
#include "MLR/MLRPlot.h"
#include "PCA/PCAPlot.h"
#include "PLS/PLSPlot.h"
#include "Plotlib/BarPlot.h"
#include "Plotlib/ScatterPlot.h"
#include "Plotlib/SimpleLine2DPlot.h"
#include "Plotlib/VariablePlot.h"
#include "dircompressor.h"

void MainWindow::CheckProjects() {

  havepca = false;
  havepcapred = false;
  havecpca = false;
  havecpcapred = false;
  havepls = false;
  haveplspred = false;
  haveplsvalid = false;
  haveplsyscrambling = false;
  havemlr = false;
  havemlrpred = false;
  havemlrvalid = false;
  havemlryscrambling = false;
  havelda = false;
  haveldapred = false;
  haveldavalid = false;

  QMap<int, DATA *>::const_iterator i = projects->constBegin();
  while (i != projects->constEnd()) {
    if (i.value()->PCACount() > 0) {
      havepca = true;
      for (int j = 0; j < i.value()->PCACount(); j++) {
        if (i.value()->getPCAModelAt(j)->PCAPredictionCount() > 0) {
          havepcapred = true;
        }
      }
    }

    if (i.value()->CPCACount() > 0) {
      havecpca = true;
      for (int j = 0; j < i.value()->CPCACount(); j++) {
        if (i.value()->getCPCAModelAt(j)->CPCAPredictionCount() > 0) {
          havecpcapred = true;
        }
      }
    }

    if (i.value()->PLSCount() > 0) {
      havepls = true;
      for (int j = 0; j < i.value()->PLSCount(); j++) {
        if (i.value()->getPLSModelAt(j)->getValidation() > 0) {
          haveplsvalid = true;
        }

        if (i.value()->getPLSModelAt(j)->PLSPredictionCount() > 0) {
          haveplspred = true;
        }

        if (i.value()->getPLSModelAt(j)->Model()->yscrambling->row > 0)
          haveplsyscrambling = true;
      }
    }

    if (i.value()->MLRCount() > 0) {
      havemlr = true;
      for (int j = 0; j < i.value()->MLRCount(); j++) {
        if (i.value()->getMLRModelAt(j)->getValidation() > 0) {
          havemlrvalid = true;
        }

        if (i.value()->getMLRModelAt(j)->MLRPredictionCount() > 0) {
          havemlrpred = true;
        }

        if (i.value()->getMLRModelAt(j)->Model()->r2q2scrambling->row > 0) {
          havemlryscrambling = true;
        }
      }
    }

    if (i.value()->LDACount() > 0) {
      havelda = true;
      for (int j = 0; j < i.value()->LDACount(); j++) {
        if (i.value()->getLDAModelAt(j)->getValidation() > 0) {
          haveldavalid = true;
        }

        if (i.value()->getLDAModelAt(j)->LDAPredictionCount() > 0) {
          haveldapred = true;
        }
      }
    }

    ++i;
  }
}

/*Fast implementation of index_of for QStringList*/
static inline int _index_of_(QStringList lst, QString str) {
  int indx = 0;
  QStringMatcher matcher(str);
  foreach (const QString &lstitem, lst) {
    if (matcher.indexIn(lstitem) != -1) {
      return indx;
    } else {
      indx++;
    }
  }
  return -1;
}

bool MainWindow::PrepareMatrix(MATRIX *indata, QStringList objnames,
                               QStringList varsel, matrix *x) {
  ResizeMatrix(x, objnames.size(), varsel.size());

  QMap<QString, int> objmap;
  for (int i = 0; i < indata->getObjName().size(); i++) {
    objmap[indata->getObjName()[i]] = i;
  }

  QMap<QString, int> varmap;
  for (int i = 1; i < indata->getVarName().size(); i++) {
    varmap[indata->getVarName()[i]] = i - 1;
  }

  QList<int> aligned_varid, aligned_objid;
  for (int i = 0; i < objnames.size(); i++) {
    auto it = objmap.find(objnames[i]);
    if (it != objmap.end()) {
      aligned_objid.append(it.value());
    } else {
      continue;
    }
  }

  QStringList varnotfound;
  for (int i = 0; i < varsel.size(); i++) {
    auto it = varmap.find(varsel[i]);
    if (it != varmap.end()) {
      aligned_varid.append(it.value());
    } else {
      varnotfound << varsel[i];
    }
  }

  // Copy the data
  for (int i = 0; i < aligned_objid.size(); i++) {
    int ii = aligned_objid[i];
    for (int j = 0; j < aligned_varid.size(); j++) {
      int jx = aligned_varid[j];
      x->data[i][j] = indata->Matrix()->data[ii][jx];
    }
    QApplication::processEvents();
  }

  if (varnotfound.size() > 0) {
    QString msg = "The following features were not found: \n";
    QString vname;
    foreach (vname, varnotfound)
      msg += QString("%1\n").arg(vname);

    QMessageBox::warning(this, tr("Warning!"), tr(msg.toStdString().c_str()),
                         QMessageBox::Close);
    return false;
  } else {
    return true;
  }
}

bool MainWindow::PrepareMatrix(MATRIX *indata, QStringList objnames,
                               QStringList xvarsel, QStringList yvarsel,
                               matrix *x, matrix *y) {
  ResizeMatrix(x, objnames.size(), xvarsel.size());
  ResizeMatrix(y, objnames.size(), yvarsel.size());

  QMap<QString, int> objmap;
  for (int i = 0; i < indata->getObjName().size(); i++) {
    objmap[indata->getObjName()[i]] = i;
  }

  QMap<QString, int> varmap;
  for (int i = 1; i < indata->getVarName().size(); i++) {
    varmap[indata->getVarName()[i]] = i - 1;
  }

  QList<int> aligned_xvarid, aligned_yvarid, aligned_objid;

  for (int i = 0; i < objnames.size(); i++) {
    auto it = objmap.find(objnames[i]);
    if (it != objmap.end()) {
      aligned_objid.append(it.value());
    } else {
      continue;
    }
  }

  QStringList xvarnotfound;
  for (int i = 0; i < xvarsel.size(); i++) {
    auto it = varmap.find(xvarsel[i]);
    if (it != varmap.end()) {
      aligned_xvarid.append(it.value());
    } else {
      xvarnotfound << xvarsel[i];
    }
  }

  QStringList yvarnotfound;
  for (int i = 0; i < yvarsel.size(); i++) {
    auto it = varmap.find(yvarsel[i]);
    if (it != varmap.end()) {
      aligned_yvarid.append(it.value());
    } else {
      yvarnotfound << yvarsel[i];
    }
  }

  // Copy the data
  for (int i = 0; i < aligned_objid.size(); i++) {
    int ii = aligned_objid[i];
    for (int j = 0; j < aligned_xvarid.size(); j++) {
      int jx = aligned_xvarid[j];
      // printf("%d %d %d %d\n", i, j, ii, jx);
      x->data[i][j] = indata->Matrix()->data[ii][jx];
    }

    for (int j = 0; j < aligned_yvarid.size(); j++) {
      int jy = aligned_yvarid[j];
      y->data[i][j] = indata->Matrix()->data[ii][jy];
    }
    QApplication::processEvents();
  }

  bool retval = true;
  if (xvarnotfound.size() > 0) {
    QString msg = "The following features were not found: \n";
    QString xvname;
    foreach (xvname, xvarnotfound)
      msg += QString("%1\n").arg(xvname);

    QMessageBox::warning(this, tr("Warning!"), tr(msg.toStdString().c_str()),
                         QMessageBox::Close);
    retval = false;
  }

  if (yvarnotfound.size() > 0) {
    QString msg = "The following dependent variables were not found: \n";
    QString yvname;
    foreach (yvname, yvarnotfound)
      msg += QString("%1\n").arg(yvname);

    QMessageBox::warning(this, tr("Warning!"), tr(msg.toStdString().c_str()),
                         QMessageBox::Close);
    retval = false;
  }
  return retval;
}

bool MainWindow::PrepareMatrix(MATRIX *indata, QStringList objnames,
                               QStringList xvarsel, LABELS classes, matrix *x,
                               matrix *y) {

  ResizeMatrix(x, objnames.size(), xvarsel.size());

  if (classes.size() == 2) {
    ResizeMatrix(y, objnames.size(), 1);
  } else {
    ResizeMatrix(y, objnames.size(), classes.size());
  }

  QMap<QString, int> objmap;
  for (int i = 0; i < indata->getObjName().size(); i++) {
    objmap[indata->getObjName()[i]] = i;
  }

  QMap<QString, int> varmap;
  for (int i = 1; i < indata->getVarName().size(); i++) {
    varmap[indata->getVarName()[i]] = i - 1;
  }

  QList<int> aligned_xvarid, aligned_objid;

  for (int i = 0; i < objnames.size(); i++) {
    auto it = objmap.find(objnames[i]);
    if (it != objmap.end()) {
      aligned_objid.append(it.value());
    } else {
      continue;
    }
  }

  QStringList xvarnotfound;
  for (int i = 0; i < xvarsel.size(); i++) {
    auto it = varmap.find(xvarsel[i]);
    if (it != varmap.end()) {
      aligned_xvarid.append(it.value());
    } else {
      xvarnotfound << xvarsel[i];
    }
  }

  // Copy the data
  for (int i = 0; i < aligned_objid.size(); i++) {
    int ii = aligned_objid[i];
    for (int j = 0; j < aligned_xvarid.size(); j++) {
      int jx = aligned_xvarid[j];
      x->data[i][j] = indata->Matrix()->data[ii][jx];
    }

    if (classes.size() == 2) {
      // contains maybe slow...
      if (classes[0].objects.contains(indata->getObjName()[ii]) == true) {
        y->data[i][0] = 1; // TRUE
      } else {
        y->data[i][0] = 0; // FALSE
      }
    } else {
      for (int j = 0; j < classes.size(); j++) {
        if (classes[j].objects.contains(indata->getObjName()[ii]) == true) {
          y->data[i][j] = 1; // TRUE
        } else {
          y->data[i][j] = 0; // FALSE
        }
      }
    }
    QApplication::processEvents();
  }

  if (xvarnotfound.size() > 0) {
    QString msg = "The following features were not found: \n";
    QString xvname;
    foreach (xvname, xvarnotfound)
      msg += QString("%1\n").arg(xvname);

    QMessageBox::warning(this, tr("Warning!"), tr(msg.toStdString().c_str()),
                         QMessageBox::Close);
    return false;
  } else {
    return true;
  }
}

bool MainWindow::PrepareTensor(MATRIX *indata, QStringList objnames,
                               LABELS block_varsel, tensor *x) {

  // ResizeMatrix(x, objnames.size(), varsel.size());

  QMap<QString, int> objmap;
  for (int i = 0; i < indata->getObjName().size(); i++) {
    objmap[indata->getObjName()[i]] = i;
  }

  QMap<QString, int> varmap;
  for (int i = 1; i < indata->getVarName().size(); i++) {
    varmap[indata->getVarName()[i]] = i - 1;
  }

  QList<int> aligned_objid;
  for (int i = 0; i < objnames.size(); i++) {
    auto it = objmap.find(objnames[i]);
    if (it != objmap.end()) {
      aligned_objid.append(it.value());
    } else {
      continue;
    }
  }

  QList<QList<int>> aligned_varid;
  QStringList varnotfound;
  for (int k = 0; k < block_varsel.size(); k++) {
    aligned_varid << QList<int>();
    for (int i = 0; i < block_varsel[k].objects.size(); i++) {
      auto it = varmap.find(block_varsel[k].objects[i]);
      if (it != varmap.end()) {
        aligned_varid.last().append(it.value());
      } else {
        varnotfound << block_varsel[k].objects[i];
      }
    }
  }

  // Prepare the tensor structure
  for (int k = 0; k < aligned_varid.size(); k++) {
    AddTensorMatrix(x, objnames.size(), aligned_varid[k].size());
  }

  // Copy the data
  for (int i = 0; i < aligned_objid.size(); i++) {
    int ii = aligned_objid[i];
    for (int k = 0; k < aligned_varid.size(); k++) {
      for (int j = 0; j < aligned_varid[k].size(); j++) {
        int jx = aligned_varid[k][j];
        x->m[k]->data[i][j] = indata->Matrix()->data[ii][jx];
      }
    }
    QApplication::processEvents();
  }

  if (varnotfound.size() > 0) {
    QString msg = "The following features were not found: \n";
    QString vname;
    foreach (vname, varnotfound)
      msg += QString("%1\n").arg(vname);

    QMessageBox::warning(this, tr("Warning!"), tr(msg.toStdString().c_str()),
                         QMessageBox::Close);
    return false;
  } else {
    return true;
  }
}

void MainWindow::PrepareKFoldClasses(QStringList objects, LABELS kfclasses,
                                     uivector *classes) {
  for (int i = 0; i < objects.size(); i++) {
    size_t class_indx = 0;
    for (int j = 0; j < kfclasses.size(); j++) {
      if (kfclasses[j].objects.contains(objects[i]) == true) {
        class_indx = j;
        break;
      } else {
        continue;
      }
    }
    UIVectorAppend(classes, class_indx);
  }
}

void MainWindow::CalculationMenuDisable(int calcpid_) {
  ui.menuModels->setEnabled(false);
  calcpid = calcpid_;
}

void MainWindow::CalculationMenuEnable() {
  calcpid = -1;
  ui.menuModels->setEnabled(true);
}

void MainWindow::TopMenuEnableDisable() {
  CheckProjects();

  if (ProjectsHavePCA() == false) {
    ui.menuPlot_PCA_Model->setEnabled(false);
  } else {
    ui.menuPlot_PCA_Model->setEnabled(true);
    if (ProjectsHavePCAPrediction() == false) {
      ui.actionPCA2DScore_Plot_Prediction->setEnabled(false);
    } else {
      ui.actionPCA2DScore_Plot_Prediction->setEnabled(true);
    }
  }

  if (ProjectsHaveCPCA() == false) {
    ui.menuPlot_CPCA_Model->setEnabled(false);
  } else {
    ui.menuPlot_CPCA_Model->setEnabled(true);
    if (ProjectsHaveCPCAPrediction() == false) {
      ui.actionCPCA2DSuperScores_Plot_Prediction->setEnabled(false);
      ui.actionCPCA2DBlockScores_Plot_Prediction->setEnabled(false);
    } else {
      ui.actionCPCA2DSuperScores_Plot_Prediction->setEnabled(true);
      ui.actionCPCA2DBlockScores_Plot_Prediction->setEnabled(true);
    }
  }

  if (ProjectsHavePLS() == false) {
    ui.menuPlot_PLS_Model->setEnabled(false);
  } else {
    ui.menuPlot_PLS_Model->setEnabled(true);

    if (ProjectsHavePLSValidated() == false) {
      ui.actionPLSR2_Q2->setEnabled(false);
      ui.actionPLSRMSE->setEnabled(false);
      ui.actionPLSPred_vs_Exp->setEnabled(false);
      ui.actionPLSPred_Residuals_vs_Exp->setEnabled(false);
    } else {
      // Check if is a pls regression or discriminant analysis
      ui.actionPLSR2_Q2->setEnabled(true);
      ui.actionPLSPred_vs_Exp->setEnabled(true);
      ui.actionPLSPred_Residuals_vs_Exp->setEnabled(true);
      ui.actionPLSRMSE->setEnabled(true);
    }

    if (ProjectsHavePLSYScrambling() == false) {
      ui.actionPLSYScrambling->setEnabled(false);
    } else {
      ui.actionPLSYScrambling->setEnabled(true);
    }

    if (ProjectsHavePLSPrediction() == false) {
      ui.actionPLS2DScore_Plot_Prediction->setEnabled(false);
      ui.actionPLSRecalc_vs_Exp_with_Prediction->setEnabled(false);
      ui.actionPLSPred_vs_Exp_with_Prediction->setEnabled(false);
      ui.actionPLSR2_Prediction->setEnabled(false);
    } else {
      ui.actionPLS2DScore_Plot_Prediction->setEnabled(true);
      ui.actionPLSRecalc_vs_Exp_with_Prediction->setEnabled(true);
      if (ProjectsHavePLSValidated() == true)
        ui.actionPLSPred_vs_Exp_with_Prediction->setEnabled(true);
      else
        ui.actionPLSPred_vs_Exp_with_Prediction->setEnabled(false);
      ui.actionPLSR2_Prediction->setEnabled(true);
    }
  }

  if (ProjectsHaveMLR() == false) {
    ui.menuPlot_MLR->setEnabled(false);
  } else {
    ui.menuPlot_MLR->setEnabled(true);
    if (ProjectsHaveMLRValidated() == false) {
      ui.actionMLR_Predict_vs_Exp->setEnabled(false);
      ui.actionMLR_Predict_Residuals_vs_Exp->setEnabled(false);
    } else {
      ui.actionMLR_Predict_vs_Exp->setEnabled(true);
      ui.actionMLR_Predict_Residuals_vs_Exp->setEnabled(true);
    }

    if (ProjectsHaveMLRYScrambling() == false) {
      ui.actionMLR_Y_Scrambling_Plot->setEnabled(false);
    } else {
      ui.actionMLR_Y_Scrambling_Plot->setEnabled(true);
    }

    if (ProjectsHaveMLRPrediction() == false) {
      ui.actionMLR_Recal_vs_Exp_and_Prediction->setEnabled(false);
      ui.actionMLR_Predict_vs_Exp_and_Prediction->setEnabled(false);
    } else {
      ui.actionMLR_Recal_vs_Exp_and_Prediction->setEnabled(true);
      ui.actionMLR_Predict_vs_Exp_and_Prediction->setEnabled(true);
    }
  }

  if (ProjectsHaveLDA() == false) {
    ui.menuPlot_LDA->setEnabled(false);
  } else {
    ui.menuPlot_LDA->setEnabled(true);
    /*
    if(ProjectsHaveLDAValidated() == false){

    }
    else{

    }
    */

    if (ProjectsHaveLDAPrediction() == false) {
      ui.actionLDA2D_Feature_Plot_and_Predictions->setEnabled(false);
      ui.actionLDAProbability_Distribution_Plot_and_Predictions->setEnabled(
          false);
    } else {
      ui.actionLDA2D_Feature_Plot_and_Predictions->setEnabled(true);
      ui.actionLDAProbability_Distribution_Plot_and_Predictions->setEnabled(
          true);
    }
  }

  /*
  if( == false){

    ui.actionVarSel_R2Q2_Plot->setEnabled(false);
    ui.actionVarSel_Ftest_Nvar_Plot->setEnabled(false);
    ui.actionVarSel_Ftest_Q2_Plot->setEnabled(false);
  }
  else{
    ui.actionVarSel_R2Q2_Plot->setEnabled(true);
    ui.actionVarSel_Ftest_Nvar_Plot->setEnabled(true);
    ui.actionVarSel_Ftest_Q2_Plot->setEnabled(true);
  }
  */
}

/* QMAP <int pid, PROJECT()>
 *
 * pid - pid  (2)
 *    |
 *    | Data   // child 0
 *    |   |
 *    |   Name - Type - Tab Count - Data Position - pid  (5)
 *    |
 *    | Models  // child 1
 *        |
 *        Name - Tab Count - pid - x ID - y ID - x Scaling type - y Scaling type
 * - number of components - Model Type - Data Position  (10)
 *          |
 *         ModelPrediction Name - Tab Count - pid - Model ID - xdata id - ydata
 * id - Data Position - Data Type (PCA Prediction, PLS Prediction, ...) (8)
 *
 * Type could be "Matrix" or "Array"
 * Tab count is an id used to know what tables are opened and to close the
 * deleted table Data Position is used to retrieve the Data Position inside the
 * QList<MATRIX>,  QList<Array> QList<PCAModel> etc... Model Type can be PCA
 * Model, PLS Model, LDA Model, MLR Model
 */

bool MainWindow::IsPredictionAt(int pid, int id, int predid) {
  if (getProjectItem(pid)->child(1)->child(id)->child(predid)->childCount() ==
      8) {
    return true;
  } else {
    return false;
  }
}

QString MainWindow::getPredictionTypeAt(int pid, int id, int predid) {
  return getProjectItem(pid)->child(1)->child(id)->child(predid)->text(7);
}

int MainWindow::getPredictionProjectIDAt(int pid, int id, int predid) {
  return getProjectItem(pid)
      ->child(1)
      ->child(id)
      ->child(predid)
      ->text(6)
      .toInt();
}

QString MainWindow::getPredictionYhashAt(int pid, int id, int predid) {
  return getProjectItem(pid)->child(1)->child(id)->child(predid)->text(5);
}

QString MainWindow::getPredictionXhashAt(int pid, int id, int predid) {
  return getProjectItem(pid)->child(1)->child(id)->child(predid)->text(4);
}

int MainWindow::getPredictionModelIDAt(int pid, int id, int predid) {
  return getProjectItem(pid)
      ->child(1)
      ->child(id)
      ->child(predid)
      ->text(3)
      .toInt();
}

int MainWindow::getPredictionIDAt(int pid, int id, int predid) {
  return getProjectItem(pid)
      ->child(1)
      ->child(id)
      ->child(predid)
      ->text(2)
      .toInt();
}

int MainWindow::getPredictionTableIDAt(int pid, int id, int predid) {
  return getProjectItem(pid)
      ->child(1)
      ->child(id)
      ->child(predid)
      ->text(1)
      .toInt();
}

QString MainWindow::getPredictionNameAt(int pid, int id, int predid) {
  return getProjectItem(pid)->child(1)->child(id)->child(predid)->text(0);
}

int MainWindow::getPredictionCountAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->childCount();
}

bool MainWindow::IsPrediction(int pid, int mid, int predid) {
  if (getProjectItem(pid)->child(1)->child(mid)->child(predid)->columnCount() ==
      7) {
    return true;
  } else {
    return false;
  }
}

QString MainWindow::getPredictionType(int pid, int mid, int predid) {
  QTreeWidgetItem *item = getModelItem(pid, mid);
  if (item->child(predid)->columnCount() == 8) {
    return item->child(predid)->text(7);
  } else {
    return QString("None");
  }
}

int MainWindow::getPredictionID(int pid, int mid, int predid) {
  QTreeWidgetItem *item = getModelItem(pid, mid);
  if (item->child(predid)->columnCount() == 8) {
    return item->child(predid)->text(6).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getPredictionYhash(int pid, int mid, int predid) {
  QTreeWidgetItem *item = getModelItem(pid, mid);
  if (item->child(predid)->columnCount() == 8) {
    return item->child(predid)->text(5);
  } else {
    return "";
  }
}

QString MainWindow::getPredictionXhash(int pid, int mid, int predid) {
  QTreeWidgetItem *item = getModelItem(pid, mid);
  if (item->child(predid)->columnCount() == 8) {
    return item->child(predid)->text(4);
  } else {
    return "";
  }
}

int MainWindow::getPredictionModelID(int pid, int mid, int predid) {
  QTreeWidgetItem *item = getModelItem(pid, mid);
  if (item->child(predid)->columnCount() == 8) {
    return item->child(predid)->text(3).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getPredictionProjectID(int pid, int mid, int predid) {
  QTreeWidgetItem *item = getModelItem(pid, mid);
  if (item->child(predid)->columnCount() == 8) {
    return item->child(predid)->text(2).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getPredictionTableID(int pid, int mid, int predid) {
  QTreeWidgetItem *item = getModelItem(pid, mid);
  if (item->child(predid)->columnCount() == 8) {
    return item->child(predid)->text(1).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getPredictionName(int pid, int mid, int predid) {
  QTreeWidgetItem *item = getModelItem(pid, mid);
  if (item->child(predid)->columnCount() == 8) {
    return item->child(predid)->text(0);
  } else {
    return QString("None");
  }
}

int MainWindow::getPredictionCount(int pid, int mid) {
  return getModelItem(pid, mid)->childCount();
}

bool MainWindow::CurrentIsPrediction() {
  if (ui.treeWidget->currentItem()->columnCount() == 8)
    return true;
  else
    return false;
}

QString MainWindow::getCurrentPredictionType() {
  if (ui.treeWidget->currentItem()->columnCount() == 8) {
    return ui.treeWidget->currentItem()->text(7);
  } else {
    return QString("None");
  }
}

int MainWindow::getCurrentPredictionID() {
  if (ui.treeWidget->currentItem()->columnCount() == 8) {
    return ui.treeWidget->currentItem()->text(6).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getCurrentPredictionYhash() {
  if (ui.treeWidget->currentItem()->columnCount() == 8) {
    return ui.treeWidget->currentItem()->text(5);
  } else {
    return "";
  }
}

QString MainWindow::getCurrentPredictionXhash() {
  if (ui.treeWidget->currentItem()->columnCount() == 8) {
    return ui.treeWidget->currentItem()->text(4);
  } else {
    return "";
  }
}

int MainWindow::getCurrentPredictionModelID() {
  if (ui.treeWidget->currentItem()->columnCount() == 8) {
    return ui.treeWidget->currentItem()->text(3).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getCurrentPredictionProjectID() {
  if (ui.treeWidget->currentItem()->columnCount() == 8) {
    return ui.treeWidget->currentItem()->text(2).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getCurrentPredictionTableID() {
  if (ui.treeWidget->currentItem()->columnCount() == 8) {
    return ui.treeWidget->currentItem()->text(1).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getCurrentPredictionName() {
  if (ui.treeWidget->currentItem()->columnCount() == 8) {
    return ui.treeWidget->currentItem()->text(0);
  } else {
    return QString("None");
  }
}

QTreeWidgetItem *MainWindow::getPredictionItem(int pid, int mid, int preid) {
  QTreeWidgetItemIterator it(getProjectItem(pid)->child(1)->child(mid));
  while (*it) {
    if ((*it)->columnCount() == 8 && (*it)->text(6).toInt() == preid) {
      return (*it);
    }
    ++it;
  }
  return 0;
}

bool MainWindow::CurrentIsModel() {
  if (ui.treeWidget->currentItem()->columnCount() == 10)
    return true;
  else
    return false;
}

int MainWindow::getCurrentModelID() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    return ui.treeWidget->currentItem()->text(9).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getCurrentModelType() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    return ui.treeWidget->currentItem()->text(8);
  } else {
    return QString("None");
  }
}

int MainWindow::getCurrentModelNComponents() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    return ui.treeWidget->currentItem()->text(7).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getCurrentModelYScalingType() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    return ui.treeWidget->currentItem()->text(6).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getCurrentModelXScalingType() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    return ui.treeWidget->currentItem()->text(5).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getCurrentModelYhash() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    if (ui.treeWidget->currentItem()->text(4).compare("-") == 0) {
      return "";
    } else {
      return ui.treeWidget->currentItem()->text(4);
    }
  } else {
    return "";
  }
}

QString MainWindow::getCurrentModelXhash() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    if (ui.treeWidget->currentItem()->text(3).compare("-") == 0) {
      return "";
    } else {
      return ui.treeWidget->currentItem()->text(3);
    }
  } else {
    return "";
  }
}

int MainWindow::getCurrentModelProjectID() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    return ui.treeWidget->currentItem()->text(2).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getCurrentModelTableID() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    return ui.treeWidget->currentItem()->text(1).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getCurrentModelName() {
  if (ui.treeWidget->currentItem()->columnCount() == 10) {
    return ui.treeWidget->currentItem()->text(0);
  } else {
    return QString("None");
  }
}

/*    | Models  // child 1
 *        |
 *        Name - Tab Count - pid - x ID - y ID - x Scaling type - y Scaling type
 * - number of components - Model Type - Data Position  (10)
 */

bool MainWindow::IsModelAt(int pid, int id) {
  if (getProjectItem(pid)->child(1)->child(id)->columnCount() == 10) {
    return true;
  } else {
    return false;
  }
}

int MainWindow::getModelIDAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(9).toInt();
}

QString MainWindow::getModelTypeAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(8);
}

int MainWindow::getModelNComponentsAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(7).toInt();
}

int MainWindow::getModelYScalingTypeAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(6).toInt();
}

int MainWindow::getModelXScalingTypeAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(5).toInt();
}

int MainWindow::getModelYIDAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(4).toInt();
}

int MainWindow::getModelXIDAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(3).toInt();
}

int MainWindow::getModelProjectIDAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(2).toInt();
}

int MainWindow::getModelTableIDAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(1).toInt();
}

QString MainWindow::getModelNameAt(int pid, int id) {
  return getProjectItem(pid)->child(1)->child(id)->text(0);
}

bool MainWindow::IsModel(int pid, int mid) {
  if (getModelItem(pid, mid)->columnCount() == 10) {
    return true;
  } else {
    return false;
  }
}

int MainWindow::getModelID(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(9).toInt();
    }
    ++it;
  }
  return -1;
  /*
  QTreeWidgetItem *item = getProjectItem(pid);
  if(item->child(1)->child(mid)->columnCount() == 10){
    return item->child(1)->child(mid)->text(9).toInt();
  }
  else{
    return -1;
  }
  */
}

QString MainWindow::getModelType(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(8);
    }
    ++it;
  }
  return QString("None");
}

int MainWindow::getModelNComponents(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(7).toInt();
    }
    ++it;
  }
  return -1;
}

int MainWindow::getModelYScalingType(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(6).toInt();
    }
    ++it;
  }
  return -1;
}

int MainWindow::getModelXScalingType(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(5).toInt();
    }
    ++it;
  }
  return -1;
}

QString MainWindow::getModelYhash(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(4);
    }
    ++it;
  }
  return "";
}

QString MainWindow::getModelXhash(int pid, int mid) {

  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(3);
    }
    ++it;
  }
  return "";
}

int MainWindow::getModelProjectID(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(2).toInt();
    }
    ++it;
  }
  return -1;
}

int MainWindow::getModelTableID(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(1).toInt();
    }
    ++it;
  }
  return -1;
}

QString MainWindow::getModelName(int pid, int mid) {
  QTreeWidgetItemIterator it(getProjectItem(pid));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it)->text(0);
    }
    ++it;
  }
  return QString("None");
}

QTreeWidgetItem *MainWindow::getModelItem(
    int pid, int mid,
    QTreeWidget *treeWidget) // pid = project id, mid = model id
{
  QTreeWidgetItemIterator it(getProjectItem(pid, treeWidget)->child(1));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it);
    }
    ++it;
  }
  return 0;
}

QTreeWidgetItem *
MainWindow::getModelItem(int pid, int mid) // pid = project id, mid = model id
{
  QTreeWidgetItemIterator it(getProjectItem(pid)->child(1));
  while (*it) {
    if ((*it)->columnCount() == 10 && (*it)->text(9).toInt() == mid) {
      return (*it);
    }
    ++it;
  }
  return 0;
}

int MainWindow::getModelCount(int pid) {
  return getProjectItem(pid)->child(1)->childCount();
}

/*    | Data   // child 0
 *    |   |
 *    |   Name - Type - Tab Count - Data Position - pid  (5)
 */

bool MainWindow::CurrentIsData() {
  if (ui.treeWidget->currentItem()->childCount() == 0 &&
      ui.treeWidget->currentItem()->columnCount() == 5)
    return true;
  else
    return false;
}

int MainWindow::getCurrentDataProjectID() {
#ifdef DEBUG
  qDebug() << "getCurrentDataProjectID() Number of Column: "
           << ui.treeWidget->currentItem()->columnCount()
           << " line: " << ui.treeWidget->currentItem()->text(0)
           << ui.treeWidget->currentItem()->text(1)
           << ui.treeWidget->currentItem()->text(2)
           << ui.treeWidget->currentItem()->text(3)
           << ui.treeWidget->currentItem()->text(4);
#endif

  if (ui.treeWidget->currentItem()->columnCount() == 5) {
    return ui.treeWidget->currentItem()->text(4).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getCurrentDataID() {
  if (ui.treeWidget->currentItem()->columnCount() == 5) {
    return ui.treeWidget->currentItem()->text(3).toInt();
  } else {
    return -1;
  }
}

int MainWindow::getCurrentDataTableID() {
  if (ui.treeWidget->currentItem()->columnCount() == 5) {
    return ui.treeWidget->currentItem()->text(2).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getCurrentDataType() {
  if (ui.treeWidget->currentItem()->columnCount() == 5) {
    return ui.treeWidget->currentItem()->text(1);
  } else {
    return QString("None");
  }
}

QString MainWindow::getCurrentDataName() {
  if (ui.treeWidget->currentItem()->columnCount() == 5) {
    return ui.treeWidget->currentItem()->text(0);
  } else {
    return QString("None");
  }
}

int MainWindow::getDataTableID(int pid, int did) {
  QTreeWidgetItem *item = getProjectItem(pid);
  if (did < item->child(0)->childCount()) {
    return item->child(0)->child(did)->text(2).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getDataType(int pid, int did) {
  QTreeWidgetItem *item = getProjectItem(pid);
  if (did < item->child(0)->childCount()) {
    return item->child(0)->child(did)->text(1);
  } else {
    return "None Type";
  }
}

QTreeWidgetItem *
MainWindow::getDataItem(int pid, int did) // pid = project id, did = data id
{
  QTreeWidgetItemIterator it(getProjectItem(pid)->child(0));
  while (*it) {
    if ((*it)->columnCount() == 5 && (*it)->text(3).toInt() == did) {
      return (*it);
    }
    ++it;
  }
  return 0;
}

int MainWindow::getDataCount(int pid) {
  return getProjectItem(pid)->child(0)->childCount();
}

bool MainWindow::CurrentIsProject() {
  if (ui.treeWidget->model()->rowCount() > 0) {
    if (ui.treeWidget->selectionModel()->selectedIndexes().size() > 0) {
      if (ui.treeWidget->currentItem()->columnCount() == 2) {
        return true;
      } else {
        return false;
      }
    } else {
      return false;
    }
  } else {
    return false;
  }
}

int MainWindow::getCurrentProjectID() {
  if (ui.treeWidget->currentItem()->columnCount() == 2) {
    return ui.treeWidget->currentItem()->text(1).toInt();
  } else {
    return -1;
  }
}

QString MainWindow::getCurrentProjectName() {
  if (ui.treeWidget->currentItem()->columnCount() == 2) {
    return ui.treeWidget->currentItem()->text(0);
  } else {
    return QString("None");
  }
}

QTreeWidgetItem *MainWindow::getCurrentProjectItem() {
  if (ui.treeWidget->currentItem()->columnCount() == 2) {
    return ui.treeWidget->currentItem();
  } else {
    return 0;
  }
}

QTreeWidgetItem *MainWindow::getProjectItem(int pid, QTreeWidget *treeWidget) {
  QTreeWidgetItemIterator it(treeWidget);
  while (*it) {
    if ((*it)->columnCount() == 2 && (*it)->text(1).toInt() == pid) {
      return (*it);
    }
    ++it;
  }
  return 0;
}

QTreeWidgetItem *MainWindow::getProjectItem(int pid) {
  QTreeWidgetItemIterator it(ui.treeWidget);
  while (*it) {
    if ((*it)->columnCount() == 2 && (*it)->text(1).toInt() == pid) {
      return (*it);
    }
    ++it;
  }
  return 0;
}

void MainWindow::StartRun() {
  ui.abortButton->setEnabled(true);
  ui.progressframe->show();
  stoprun = false;
  ui.progressBar->setMinimum(0);
  ui.progressBar->setMaximum(0);
  ui.progressBar->setValue(0);
}

void MainWindow::WaitRun() { ui.abortButton->setEnabled(false); }

void MainWindow::StopRun() {
  ui.progressframe->hide();
  stoprun = true;
  ui.progressBar->setMinimum(0);
  ui.progressBar->setMaximum(100);
  ui.progressBar->setValue(20);
}

void MainWindow::PlotVariableVSVariableBis(vvplotSignal vvs) {
#ifdef DEBUG
  qDebug() << "PlotVariableVSVariableBis(vvplotSignal vvs) " << vvs.pid
           << vvs.mid << vvs.mtype << vvs.vname1 << vvs.vname2;
#endif
  if (vvs.pid > -1 && vvs.mid > -1) {
    QStringList hashlst;
    QStringList objects;
    if (vvs.mtype == PCA_) {
      hashlst.append(
          projects->value(vvs.pid)->getPCAModel(vvs.mid)->getDataHash());
      objects = projects->value(vvs.pid)->getPCAModel(vvs.mid)->getObjName();
    } else if (vvs.mtype == PLS_) {
      hashlst.append(
          projects->value(vvs.pid)->getPLSModel(vvs.mid)->getDataHash());
      objects = projects->value(vvs.pid)->getPLSModel(vvs.mid)->getObjName();
    }

    int mxid = projects->value(vvs.pid)->getMatrixID(hashlst.last());
    if (hashlst.size() > 0 && objects.size() > 0 && mxid > -1) {
      VariablePlot vplot(projects);

      vplot.setPlotType(MATRIXDATA);
      vplot.setPID(vvs.pid);

      vplot.setXHash(hashlst);
      QList<QStringList> objectslst;
      objectslst << objects;
      vplot.setObjects(objectslst);
      // -1 because first varname is a generic name
      vplot.setVarID1(
          projects->value(vvs.pid)->getMatrix(mxid)->getVarName().indexOf(
              vvs.vname1) -
          1);
      vplot.setVarID2(
          projects->value(vvs.pid)->getMatrix(mxid)->getVarName().indexOf(
              vvs.vname2) -
          1);
      vplot.setLayerID(-1);

      ScatterPlot *plot2D = vplot.VariableVSVariable();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));

      MDIChild *graphchild = createMdiChild();
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(vvs.pid);
      graphchild->resize(510, 530);
      graphchild->show();
    } else {
      return;
    }
  }
}

void MainWindow::UpdateImageWindow(ImageSignal is) {
#ifdef DEBUG
  qDebug() << QString("UpdateImageWindow pid: %1").arg(is.pid) << "\n"
           << is.imgname;
#endif

  while (ui.tabWidget->count() > 0) {
    int i = ui.tabWidget->currentIndex();
    delete ui.tabWidget->widget(i);
    ui.tabWidget->removeTab(i);
  }
  ui.tabWidget->clear();

  if (is.pid > -1) {
#ifdef DEBUG
    qDebug() << "IMAGESIZE " << projects->value(is.pid)->getImages().size();
#endif
    for (int i = 0; i < projects->value(is.pid)->getImages().size(); i++) {
      if (is.imgname.indexOf(projects->value(is.pid)->getImages()[i].name) >
          -1) {
        QLabel *img = new QLabel();
        img->setMinimumSize(247, 170);
        img->resize(247, 170);
        //         img->setPixmap(QPixmap(projects->value(is.pid)->getImages()[i].filepath));
        img->setPixmap(projects->value(is.pid)->getImages()[i].image);
        img->setScaledContents(true);
        img->pixmap().scaled(247, 170, Qt::KeepAspectRatioByExpanding);

        ui.tabWidget->addTab(img, projects->value(is.pid)->getImages()[i].name);
      } else {
        continue;
      }
    }
  } else {
    return;
  }
}

/*
 * pid - pid  (2)
 *    |
 *    | Data   // child 0
 *    |   |
 *    |   Name - Type - Tab Count - Data Position - pid  (5)
 *    |
 *    | Models  // child 1
 *        |
 *        Name - Tab Count - pid - x ID - y ID - x Scaling type - y Scaling type
 * - number of components - Model Type - Data Position  (10)
 *          |
 *         ModelPrediction Name - Tab Count - pid - Model ID - xdata id - ydata
 * id - Data Position - Data Type (PCA Prediction, PLS Prediction, ...) (8)
 *
 * Type could be "Matrix" or "Array"
 * Tab count is an id used to know what tables are opened and to close the
 * deleted table Data Position is used to retrieve the Data Position inside the
 * QList<MATRIX>,  QList<Array> QList<PCAModel> etc... Model Type can be PCA
 * Model, PLS Model, LDA Model, MLR Model
 */

void MainWindow::DowngradePredictionID() {
  if (CurrentIsPrediction() == true) {
    int predid = getCurrentPredictionID();
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    if (pid > -1 && mid > -1 && predid > -1) {
      QTreeWidgetItem *modelitem = getModelItem(pid, mid);
      // Downgrading of one all the id of the other data predicted
      for (int i = 0; i < modelitem->childCount(); i++) {
        int childid = getPredictionID(pid, mid, i);
        if (childid > predid) {
          QString childmodeltype = getPredictionType(pid, mid, i);
          if (childmodeltype.compare("PCA Prediction") == 0) {
            ui.treeWidget->currentItem()->parent()->child(i)->setText(
                6, QString::number(childid - 1));
            projects->value(pid)
                ->getPCAModel(mid)
                ->getPCAPrediction(childid)
                ->setPredID(childid - 1);
          } else if (childmodeltype.compare("PLS Prediction") == 0) {
            ui.treeWidget->currentItem()->parent()->child(i)->setText(
                6, QString::number(childid - 1));
            projects->value(pid)
                ->getPLSModel(mid)
                ->getPLSPrediction(childid)
                ->setPredID(childid - 1);
          } else if (childmodeltype.compare("MLR Prediction") == 0) {
            ui.treeWidget->currentItem()->parent()->child(i)->setText(
                6, QString::number(childid - 1));
            projects->value(pid)
                ->getMLRModel(mid)
                ->getMLRPrediction(childid)
                ->setPredID(childid - 1);
          } else if (childmodeltype.compare("LDA Prediction") == 0) {
            ui.treeWidget->currentItem()->parent()->child(i)->setText(
                6, QString::number(childid - 1));
            projects->value(pid)
                ->getLDAModel(mid)
                ->getLDAPrediction(childid)
                ->setPredID(childid - 1);
          } else {
            continue;
          }
        } else {
          continue;
        }
      }
    }
  }
#ifndef DEBUG
  else {
    qDebug() << "Error During DowngradePredictionID. Current Item is not a "
                "Prediction";
  }
#endif
}

void MainWindow::removePrediction() {
  if (CurrentIsPrediction() == true) {

    bool removeprediction;
    if (calcpid != -1) {
      removeprediction = false;
    } else {
      removeprediction = true;
    }

    if (removeprediction == true) {

      closeMDI(getCurrentPredictionTableID());

      int predid = getCurrentPredictionID();
      int pid = getCurrentPredictionProjectID();
      int mid = getCurrentPredictionModelID();
      if (pid > -1 && mid > -1 && predid > -1) {
        DowngradePredictionID();

        if (getCurrentPredictionType().compare("PCA Prediction") == 0) {
          updateLog(QString("Deleting PCA Prediction %1\n")
                        .arg(getCurrentPredictionName()));
          projects->value(pid)->getPCAModel(mid)->delPCAPredictionAt(predid);
        } else if (getCurrentPredictionType().compare("CPCA Prediction") == 0) {
          updateLog(QString("Deleting CPCA Prediction %1\n")
                        .arg(getCurrentPredictionName()));
          projects->value(pid)->getCPCAModel(mid)->delCPCAPredictionAt(predid);
        } else if (getCurrentPredictionType().compare("PLS Prediction") == 0) {
          updateLog(QString("Deleting PLS Prediction %1\n")
                        .arg(getCurrentPredictionName()));
          projects->value(pid)->getPLSModel(mid)->delPLSPredictionAt(predid);
        } else if (getCurrentPredictionType().compare("MLR Prediction") == 0) {
          updateLog(QString("Deleting MLR Prediction %1\n")
                        .arg(getCurrentPredictionName()));
          projects->value(pid)->getMLRModel(mid)->delMLRPredictionAt(predid);
        } else if (getCurrentPredictionType().compare("LDA Prediction") == 0) {
          updateLog(QString("Deleting LDA Prediction %1\n")
                        .arg(getCurrentPredictionName()));
          projects->value(pid)->getLDAModel(mid)->delLDAPredictionAt(predid);
        }

        getModelItem(pid, mid)->removeChild(ui.treeWidget->currentItem());
        //     ui.treeWidget->currentItem()->parent()->removeChild(ui.treeWidget->currentItem());
      }
    } else {
      return;
    }
  }
}

/*
 * pid - pid  (2)
 *    |
 *    | Data   // child 0
 *    |   |
 *    |   Name - Type - Tab Count - Data Position - pid  (5)
 *    |
 *    | Models  // child 1
 *        |
 *        Name - Tab Count - pid - x ID - y ID - x Scaling type - y Scaling type
 * - number of components - Model Type - Data Position  (10)
 *          |
 *         ModelPrediction Name - Tab Count - pid - Model ID - xdata id - ydata
 * id - Data Position - Data Type (PCA Prediction, PLS Prediction, ...) (8)
 *
 * Type could be "Matrix" or "Array"
 * Tab count is an id used to know what tables are opened and to close the
 * deleted table Data Position is used to retrieve the Data Position inside the
 * QList<MATRIX>,  QList<Array> QList<PCAModel> etc... Model Type can be PCA
 * Model, PLS Model, LDA Model, MLR Model
 */

void MainWindow::DowngradeModelID() {
  if (CurrentIsModel() == true) {

    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    // Downgrading of one all the id of the other models
    for (int i = 0; i < getModelCount(pid); i++) {
      int childid = getModelID(pid, i);
      QString childmodeltype = getModelType(pid, i);
      if (childid > mid) {
        if (childmodeltype.compare("PCA Model") == 0) {
          ui.treeWidget->currentItem()->parent()->child(i)->setText(
              9, QString::number(childid - 1));
          projects->value(pid)->getPCAModel(childid)->setModelID(childid - 1);
        } else if (childmodeltype.compare("CPCA Model") == 0) {
          ui.treeWidget->currentItem()->parent()->child(i)->setText(
              9, QString::number(childid - 1));
          projects->value(pid)->getCPCAModel(childid)->setModelID(childid - 1);
        } else if (childmodeltype.compare("PLS Model") == 0) {
          ui.treeWidget->currentItem()->parent()->child(i)->setText(
              9, QString::number(childid - 1));
          projects->value(pid)->getPLSModel(childid)->setModelID(childid - 1);
        } else if (childmodeltype.compare("MLR Model") == 0) {
          ui.treeWidget->currentItem()->parent()->child(i)->setText(
              9, QString::number(childid - 1));
          projects->value(pid)->getMLRModel(childid)->setModelID(childid - 1);
        } else if (childmodeltype.compare("LDA Model") == 0) {
          ui.treeWidget->currentItem()->parent()->child(i)->setText(
              9, QString::number(childid - 1));
          projects->value(pid)->getLDAModel(childid)->setModelID(childid - 1);
        } else {
          continue;
        }
      } else {
        continue;
      }
    }
  }
}

void MainWindow::removeModel() {
  if (CurrentIsModel() == true) {
    bool removemodel;
    if (calcpid != -1) {
      removemodel = false;
    } else {
      removemodel = true;
    }

    if (removemodel == true) {
      closeMDI(getCurrentModelTableID());

      int pid = getCurrentModelProjectID();
      int mid = getCurrentModelID();

      if (pid == -1 || mid == -1)
        return;

      DowngradeModelID();

      // Close all subchild and delete the predictions if are presents
      QTreeWidgetItem *modelitem = getModelItem(pid, mid);
      for (int i = 0; i < modelitem->childCount(); i++) {
        closeMDI(getPredictionTableID(pid, mid, i));
      }

      if (getCurrentModelType().compare("PCA Model") == 0) {
        updateLog(
            QString("Deleting PCA Model %1\n").arg(getCurrentModelName()));
        projects->value(pid)->delPCAModel(mid);
      } else if (ui.treeWidget->currentItem()->text(8).compare("CPCA Model") ==
                 0) {
        updateLog(
            QString("Deleting CPCA Model %1\n").arg(getCurrentModelName()));
        projects->value(pid)->delCPCAModel(mid);
      } else if (ui.treeWidget->currentItem()->text(8).compare("PLS Model") ==
                 0) {
        updateLog(
            QString("Deleting PLS Model %1\n").arg(getCurrentModelName()));
        projects->value(pid)->delPLSModel(mid);
      } else if (ui.treeWidget->currentItem()->text(8).compare("MLR Model") ==
                 0) {
        updateLog(
            QString("Deleting MLR Model %1\n").arg(getCurrentModelName()));
        projects->value(pid)->delMLRModel(mid);
      } else if (ui.treeWidget->currentItem()->text(8).compare("LDA Model") ==
                 0) {
        updateLog(
            QString("Deleting LDA Model %1\n").arg(getCurrentModelName()));
        projects->value(pid)->delLDAModel(mid);
      }

      getProjectItem(pid)->child(1)->removeChild(ui.treeWidget->currentItem());
      //     ui.treeWidget->currentItem()->parent()->removeChild(ui.treeWidget->currentItem());
      TopMenuEnableDisable();
    } else {
      return;
    }
  }
}

void MainWindow::showLDACovarianceGroupMatrix() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - LDA Covariance Group Matrix";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)->getLDAModel(mid)->Model()->inv_cov,
                      &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(
          projects->value(pid)->getLDAModel(mid)->getVarName());
      QStringList headername;
      headername << "Variables";
      headername.append(projects->value(pid)->getLDAModel(mid)->getVarName());
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showLDAPriorProbabilities() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - LDA Covariance Group Matrix";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      matrix *pprob;
      initMatrix(&pprob);
      MatrixAppendCol(pprob,
                      projects->value(pid)->getLDAModel(mid)->Model()->pprob);
      child->newTable(tabname, pprob, &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      QStringList objname;
      for (size_t i = 0; i < pprob->row; i++)
        objname.append(QString("Class %1").arg(QString::number(i + 1)));

      child->getTable()->model()->setObjNames(objname);
      QStringList headername;
      headername << "Classes"
                 << "Prior Probabilities";
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
      DelMatrix(&pprob);
    }
  }
}

void MainWindow::showLDAFeatures() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

      size_t cc = 0;
      uivector *classid =
          projects->value(pid)->getLDAModel(mid)->Model()->classid;
      for (size_t k = 0;
           k < projects->value(pid)->getLDAModel(mid)->Model()->features->order;
           k++) { /* for each class */
        QString tabname =
            projectname + " - " + modelname +
            QString(" - LDA Features Class %1").arg(QString::number(k + 1));
        MDIChild *child = createMdiChild();
        child->setWindowID(tabid);
        QStringList headername, objname;
        matrix *m;
        initMatrix(&m);
        MatrixCopy(
            projects->value(pid)->getLDAModel(mid)->Model()->features->m[k],
            &m);
        child->newTable(tabname, m, &projects->value(pid)->getObjectLabels(),
                        &projects->value(pid)->getVariableLabels());

        for (size_t i = 0; i < m->row; i++) {
          objname.append(projects->value(pid)
                             ->getLDAModel(mid)
                             ->getObjName()[classid->data[cc]]);
          cc++;
        }
        child->getTable()->model()->setObjNames(objname);

        headername << "Object Name";
        for (size_t i = 0; i < m->col; i++) {
          headername.append(QString("LD %1").arg(QString::number(i + 1)));
        }
        child->getTable()->model()->setHorizontalHeaderLabels(headername);
        child->show();
        child->getTable()->setPID(pid);
        connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
        DelMatrix(&m);
      }
    }
  }
}

void MainWindow::showLDAMVNormDistrib() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

      size_t cc = 0;
      uivector *classid =
          projects->value(pid)->getLDAModel(mid)->Model()->classid;
      for (size_t k = 0;
           k < projects->value(pid)->getLDAModel(mid)->Model()->mnpdf->order;
           k++) { /* for each class */
        QString tabname = projectname + " - " + modelname +
                          QString(" -  LDA Multivariate Normal Distribution of "
                                  "Probabilities Class %1")
                              .arg(QString::number(k + 1));
        MDIChild *child = createMdiChild();
        child->setWindowID(tabid);
        QStringList headername, objname;
        matrix *m;
        initMatrix(&m);
        MatrixCopy(projects->value(pid)->getLDAModel(mid)->Model()->mnpdf->m[k],
                   &m);
        child->newTable(tabname, m, &projects->value(pid)->getObjectLabels(),
                        &projects->value(pid)->getVariableLabels());

        for (size_t i = 0; i < m->row; i++) {
          objname.append(projects->value(pid)
                             ->getLDAModel(mid)
                             ->getObjName()[classid->data[cc]]);
          cc++;
        }
        child->getTable()->model()->setObjNames(objname);

        headername << "Object Name";
        for (size_t i = 0; i < m->col; i++) {
          headername.append(QString("LD %1").arg(QString::number(i + 1)));
        }
        child->getTable()->model()->setHorizontalHeaderLabels(headername);
        child->show();
        child->getTable()->setPID(pid);
        connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
        DelMatrix(&m);
      }
    }
  }
}

void MainWindow::showLDAValidation() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

      QString tabname = projectname + " - " + modelname + " - LDA Validation";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      matrix *valid;
      initMatrix(&valid);
      QStringList headername;
      headername << "Classes";
      for (size_t k = 0;
           k < projects->value(pid)->getLDAModel(mid)->Model()->roc->order;
           k++) {
        headername << QString("ROC FP Rate - %1").arg(QString::number(k + 1))
                   << QString("ROC TP Rate - %1").arg(QString::number(k + 1));
        dvector *fpr = getMatrixColumn(
            projects->value(pid)->getLDAModel(mid)->Model()->roc->m[k], 0);
        dvector *tpr = getMatrixColumn(
            projects->value(pid)->getLDAModel(mid)->Model()->roc->m[k], 1);
        MatrixAppendCol(valid, fpr);
        MatrixAppendCol(valid, tpr);
        DelDVector(&fpr);
        DelDVector(&tpr);
      }

      for (size_t k = 0;
           k < projects->value(pid)->getLDAModel(mid)->Model()->roc->order;
           k++) {
        headername
            << QString("PR Recall - %1").arg(QString::number(k + 1))
            << QString("PR Precision Rate - %1").arg(QString::number(k + 1));
        dvector *recall = getMatrixColumn(
            projects->value(pid)->getLDAModel(mid)->Model()->pr->m[k], 0);
        dvector *precision = getMatrixColumn(
            projects->value(pid)->getLDAModel(mid)->Model()->pr->m[k], 1);
        MatrixAppendCol(valid, recall);
        MatrixAppendCol(valid, precision);
        DelDVector(&recall);
        DelDVector(&precision);
      }

      child->newTable(tabname, valid);
      QStringList objname;
      for (size_t i = 0; i < valid->row; i++)
        objname.append(QString("Class %1").arg(QString::number(i + 1)));
      child->getTable()->model()->setObjNames(objname);

      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
      DelMatrix(&valid);
    }
  }
}

void MainWindow::showLDAPrediction() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
      int predid = getCurrentPredictionID();
      QString tabname = projectname + " - " + modelname +
                        " - LDa Predicted Class" + " - " +
                        ui.treeWidget->currentItem()->text(0);
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname, projects->value(pid)
                                   ->getLDAModel(mid)
                                   ->getLDAPrediction(predid)
                                   ->getPredClasses());
      child->getTable()->model()->setObjNames(projects->value(pid)
                                                  ->getPCAModel(mid)
                                                  ->getPCAPrediction(predid)
                                                  ->getObjName());
      QStringList headername;
      headername << firstcol_name << "Predicted Class";
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showLDAPredictionFeatures() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getLDAModel(mid)->getName();
      int predid = getCurrentPredictionID();

      QString tabname = projectname + " - " + modelname +
                        QString(" -  LDA Predicted Feature");
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      QStringList headername, objname;
      child->newTable(tabname, projects->value(pid)
                                   ->getLDAModel(mid)
                                   ->getLDAPrediction(predid)
                                   ->getPredFeatures());
      child->getTable()->model()->setObjNames(projects->value(pid)
                                                  ->getLDAModel(mid)
                                                  ->getLDAPrediction(predid)
                                                  ->getObjName());

      headername << "Object Name";
      for (size_t i = 0; i < projects->value(pid)
                                 ->getLDAModel(mid)
                                 ->getLDAPrediction(predid)
                                 ->getPredFeatures()
                                 ->col;
           i++) {
        headername.append(QString("LD %1").arg(QString::number(i + 1)));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

/*
void MainWindow::showLDAPredictionMVNormDistrib()
{
  if(CurrentIsPrediction() == true){
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();

    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();
    int predid = getCurrentPredictionID();

    QString tabname = projectname + " - " + modelname + QString(" -  LDA
Predicted Feature"); MDIChild *child = createMdiChild();
    child->setWindowID(tabid);
    QStringList headername, objname;
    child->newTable(tabname,
projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getProbPred());
    child->getTable()->model()->setObjNames(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getObjName());

    headername << "Object Name";
    for(size_t i = 0; i <
projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getPredFeatures()->col;
i++){ headername.append(QString("LD%").arg(QString::number(i+1)));
    }
    child->getTable()->model()->setHorizontalHeaderLabels(headername);
    child->show();
    child->getTable()->setPID(pid);
    connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
SLOT(UpdateImageWindow(ImageSignal)));
  }
}
*/
void MainWindow::showMLRCoeff() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getMLRModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - MLR Regression Coefficients";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)->getMLRModel(mid)->Model()->b);

      QStringList varnames =
          projects->value(pid)->getMLRModel(mid)->getXVarName();
      QStringList coeffname;
      coeffname.append("Intercept");
#ifdef DEBUG
      qDebug() << varnames.size()
               << projects->value(pid)->getMLRModel(mid)->Model()->b->row;
#endif
      for (size_t i = 1;
           i < projects->value(pid)->getMLRModel(mid)->Model()->b->row; i++) {
        coeffname.append(varnames[i - 1]);
      }
      child->getTable()->model()->setObjNames(coeffname);

      QStringList headername;
      headername << "Coefficient Name";
      for (size_t c = 0;
           c < projects->value(pid)->getMLRModel(mid)->Model()->b->col; c++) {
        headername << QString("Betas %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showMLRValidation() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getMLRModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - MLR Correlation Coefficient";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname);

      dvector *r2y_model =
          projects->value(pid)->getMLRModel(mid)->Model()->r2y_model;
      dvector *sdec = projects->value(pid)->getMLRModel(mid)->Model()->sdec;
      dvector *q2y_model = projects->value(pid)->getMLRModel(mid)->Model()->q2y;
      dvector *sdep = projects->value(pid)->getMLRModel(mid)->Model()->sdep;
      dvector *bias = projects->value(pid)->getMLRModel(mid)->Model()->bias;

      QStringList labels;
      QStringList header;
      child->getTable()->model()->newMatrix(r2y_model->size, 5);
      for (size_t i = 0; i < r2y_model->size; i++) {
        setMatrixValue(child->getTable()->model()->Matrix(), i, 0,
                       getDVectorValue(r2y_model, i));
        setMatrixValue(child->getTable()->model()->Matrix(), i, 1,
                       getDVectorValue(sdec, i));
        setMatrixValue(child->getTable()->model()->Matrix(), i, 2,
                       getDVectorValue(q2y_model, i));
        setMatrixValue(child->getTable()->model()->Matrix(), i, 3,
                       getDVectorValue(sdep, i));
        setMatrixValue(child->getTable()->model()->Matrix(), i, 4,
                       getDVectorValue(bias, i));
        labels.append("Y " + QString::number(i + 1));
      }
      header << "Y "
             << "R2"
             << "SDEC"
             << "Q2"
             << "SDEP"
             << "BIAS";
      child->getTable()->model()->setObjNames(labels);
      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->getTable()->model()->UpdateModel();
      child->show();
    }
  }
}

void MainWindow::showMLRRecalcY() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getMLRModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - MLR Recalculated Y";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname,
          projects->value(pid)->getMLRModel(mid)->Model()->recalculated_y,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(
          projects->value(pid)->getMLRModel(mid)->getObjName());
      QStringList headername;
      headername << "Object Name";
      for (size_t c = 0;
           c < projects->value(pid)->getMLRModel(mid)->Model()->b->col; c++) {
        headername << QString("Y %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showMLRValidatedPrediction() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getMLRModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - MLR Validated Predicted Y";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname, projects->value(pid)->getMLRModel(mid)->Model()->predicted_y,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(
          projects->value(pid)->getMLRModel(mid)->getObjName());
      QStringList headername;
      headername << "Object Name";
      for (size_t c = 0;
           c < projects->value(pid)->getMLRModel(mid)->Model()->b->col; c++) {
        headername << QString("Y %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showMLRPrediction() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getMLRModel(mid)->getName();
      int predid = getCurrentPredictionID();
      QString tabname = projectname + " - " + modelname +
                        " - MLR Predicted Dipendent Value" + " - " +
                        ui.treeWidget->currentItem()->text(0);
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)
                          ->getMLRModel(mid)
                          ->getMLRPrediction(predid)
                          ->getYDipVar(),
                      &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(projects->value(pid)
                                                  ->getMLRModel(mid)
                                                  ->getMLRPrediction(predid)
                                                  ->getObjName());
      QStringList header;
      header << firstcol_name;

      for (size_t j = 0; j < projects->value(pid)
                                 ->getMLRModel(mid)
                                 ->getMLRPrediction(predid)
                                 ->getYDipVar()
                                 ->col;
           j++) {
        header << QString("Y %1").arg(QString::number(j + 1));
      }

      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showMLRPredictionRSquared() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int predid = getCurrentPredictionID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1 && predid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getMLRModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - MLR Prediction Error";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname);

      dvector *r2pred = projects->value(pid)
                            ->getMLRModel(mid)
                            ->getMLRPrediction(predid)
                            ->getR2Y();
      dvector *sdecpred = projects->value(pid)
                              ->getMLRModel(mid)
                              ->getMLRPrediction(predid)
                              ->getSDEC();

      child->getTable()->model()->newMatrix(r2pred->size, 2);

      QStringList labels;

      for (size_t i = 0; i < r2pred->size; i++) { // PC
        labels.append("Y " + QString::number(i + 1));
        setMatrixValue(child->getTable()->model()->Matrix(), i, 0,
                       getDVectorValue(r2pred, i));
        setMatrixValue(child->getTable()->model()->Matrix(), i, 1,
                       getDVectorValue(sdecpred, i));
      }

      child->getTable()->model()->setObjNames(labels);

      QStringList header;
      header << "Y"
             << "R2"
             << "SDEC";
      child->getTable()->model()->setHorizontalHeaderLabels(header);

      child->getTable()->model()->UpdateModel();
      child->show();
    }
  }
}

void MainWindow::showPLSPredictionRSquared() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int predid = getCurrentPredictionID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && predid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - PLS Prediction Error";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname);

#ifdef DEBUG
      qDebug() << "r2y";
      PrintMatrix(projects->value(pid)
                      ->getPLSModel(mid)
                      ->getPLSPrediction(predid)
                      ->getR2Y());
      qDebug() << "sdec";
      PrintMatrix(projects->value(pid)
                      ->getPLSModel(mid)
                      ->getPLSPrediction(predid)
                      ->getSDEC());
#endif

      size_t row = projects->value(pid)
                       ->getPLSModel(mid)
                       ->getPLSPrediction(predid)
                       ->getR2Y()
                       ->row; // the number of components
      size_t col = projects->value(pid)
                       ->getPLSModel(mid)
                       ->getPLSPrediction(predid)
                       ->getR2Y()
                       ->col +
                   projects->value(pid)
                       ->getPLSModel(mid)
                       ->getPLSPrediction(predid)
                       ->getSDEC()
                       ->col;

      child->getTable()->model()->newMatrix(row, col);

      QStringList labels;

      for (size_t i = 0; i < row; i++) { // PC
        labels.append("PC " + QString::number(i + 1));
        size_t l = 0;
        for (size_t j = 0; j < projects->value(pid)
                                   ->getPLSModel(mid)
                                   ->getPLSPrediction(predid)
                                   ->getR2Y()
                                   ->col;
             j++) { // the q2 for each y
          setMatrixValue(child->getTable()->model()->Matrix(), i, l,
                         getMatrixValue(projects->value(pid)
                                            ->getPLSModel(mid)
                                            ->getPLSPrediction(predid)
                                            ->getR2Y(),
                                        i, j));
          l++;
          setMatrixValue(child->getTable()->model()->Matrix(), i, l,
                         getMatrixValue(projects->value(pid)
                                            ->getPLSModel(mid)
                                            ->getPLSPrediction(predid)
                                            ->getSDEC(),
                                        i, j));
          l++;
        }
      }

      child->getTable()->model()->setObjNames(labels);

      QStringList header;
      header << "Principal Component";

      for (size_t j = 0; j < projects->value(pid)
                                 ->getPLSModel(mid)
                                 ->getPLSPrediction(predid)
                                 ->getR2Y()
                                 ->col;
           j++) {
        header << QString("R^2(y %1)").arg(QString::number(j + 1))
               << QString("SDEC(y %1)").arg(QString::number(j + 1));
      }

      child->getTable()->model()->setHorizontalHeaderLabels(header);

      child->getTable()->model()->UpdateModel();
      child->show();
    }
  }
}

void MainWindow::showPLSPrediction() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
      int predid = getCurrentPredictionID();
      QString tabname = projectname + " - " + modelname +
                        " - PLS Predicted Dipendent Value" + " - " +
                        ui.treeWidget->currentItem()->text(0);
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)
                          ->getPLSModel(mid)
                          ->getPLSPrediction(predid)
                          ->getYDipVar(),
                      &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(projects->value(pid)
                                                  ->getPLSModel(mid)
                                                  ->getPLSPrediction(predid)
                                                  ->getObjName());
      QStringList header;
      header << firstcol_name;

      for (int i = 0; i < projects->value(pid)->getPLSModel(mid)->getNPC();
           i++) {
        for (size_t j = 0;
             j <
             projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row;
             j++) {
          header << QString("y %1 (PC %2)")
                        .arg(QString::number(j + 1))
                        .arg(QString::number(i + 1));
        }
      }

      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPLSPredScore() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
      int predid = getCurrentPredictionID();
      QString tabname = projectname + " - " + modelname +
                        " - PLS Predicted Scores" + " - " +
                        ui.treeWidget->currentItem()->text(0);
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)
                          ->getPLSModel(mid)
                          ->getPLSPrediction(predid)
                          ->getXPredScores(),
                      &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(projects->value(pid)
                                                  ->getPLSModel(mid)
                                                  ->getPLSPrediction(predid)
                                                  ->getObjName());
      QStringList headername;
      headername << firstcol_name;
      for (size_t c = 0; c < projects->value(pid)
                                 ->getPLSModel(mid)
                                 ->getPLSPrediction(predid)
                                 ->getXPredScores()
                                 ->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPLSValidatedPrediction() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - PLS Model Prediction";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname, projects->value(pid)->getPLSModel(mid)->Model()->predicted_y,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels());
      QStringList varname =
          projects->value(pid)->getPLSModel(mid)->getObjName();
      child->getTable()->model()->setObjNames(varname);
      QStringList header;
      header << firstcol_name;

      for (int i = 0; i < projects->value(pid)->getPLSModel(mid)->getNPC();
           i++) {
        for (size_t j = 0;
             j <
             projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row;
             j++) {
          header << QString("y %1 (PC %2)")
                        .arg(QString::number(j + 1))
                        .arg(QString::number(i + 1));
        }
      }

      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPLSValidation() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
      QString tabname = projectname + " - " + modelname + " - PLS Validation";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname);
      size_t row = 0;
      size_t col = 0;
      if (projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_) {
        row = projects->value(pid)
                  ->getPLSModel(mid)
                  ->Model()
                  ->q2y->row; // the number of components
        col = projects->value(pid)
                  ->getPLSModel(mid)
                  ->Model()
                  ->r2y_recalculated->col +
              projects->value(pid)->getPLSModel(mid)->Model()->q2y->col +
              projects->value(pid)->getPLSModel(mid)->Model()->sdep->col +
              projects->value(pid)->getPLSModel(mid)->Model()->bias->col;
      } else {
        row = projects->value(pid)
                  ->getPLSModel(mid)
                  ->Model()
                  ->roc_auc_recalculated->row;
        col = projects->value(pid)
                  ->getPLSModel(mid)
                  ->Model()
                  ->roc_auc_recalculated->col +
              projects->value(pid)
                  ->getPLSModel(mid)
                  ->Model()
                  ->roc_auc_validation->col +
              projects->value(pid)
                  ->getPLSModel(mid)
                  ->Model()
                  ->precision_recall_ap_recalculated->col +
              projects->value(pid)
                  ->getPLSModel(mid)
                  ->Model()
                  ->precision_recall_ap_validation->col;
      }

#ifdef DEBUG
      qDebug() << QString("Create a matrix of %1 %2").arg(row).arg(col);
#endif
      child->getTable()->model()->newMatrix(row, col);

      QStringList labels;
      QStringList header;
      header << "Principal Component";
      if (projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_) {
        for (size_t i = 0; i < row; i++) { // PC
          labels.append("PC " + QString::number(i + 1));
          size_t l = 0;
          for (size_t j = 0;
               j < projects->value(pid)->getPLSModel(mid)->Model()->q2y->col;
               j++) { // the q2 for each y
            setMatrixValue(child->getTable()->model()->Matrix(), i, l,
                           getMatrixValue(projects->value(pid)
                                              ->getPLSModel(mid)
                                              ->Model()
                                              ->r2y_recalculated,
                                          i, j));
            l++;
            setMatrixValue(
                child->getTable()->model()->Matrix(), i, l,
                getMatrixValue(
                    projects->value(pid)->getPLSModel(mid)->Model()->q2y, i,
                    j));
            l++;
            setMatrixValue(
                child->getTable()->model()->Matrix(), i, l,
                getMatrixValue(
                    projects->value(pid)->getPLSModel(mid)->Model()->sdep, i,
                    j));
            l++;
            setMatrixValue(
                child->getTable()->model()->Matrix(), i, l,
                getMatrixValue(
                    projects->value(pid)->getPLSModel(mid)->Model()->bias, i,
                    j));
            l++;
          }
        }

        QStringList yvarnames =
            projects->value(pid)->getPLSModel(mid)->getYVarName();
        for (size_t j = 0;
             j < projects->value(pid)->getPLSModel(mid)->Model()->q2y->col;
             j++) {
          header << QString("R^2(%1)").arg(yvarnames[j])
                 << QString("Q^2(%1)").arg(yvarnames[j])
                 << QString("SDEP(%1)").arg(yvarnames[j])
                 << QString("BIAS(%1)").arg(yvarnames[j]);
        }
      } else {                             // plsda
        for (size_t i = 0; i < row; i++) { // PC
          labels.append("PC " + QString::number(i + 1));
          size_t l = 0;
          for (size_t j = 0; j < projects->value(pid)
                                     ->getPLSModel(mid)
                                     ->Model()
                                     ->roc_auc_recalculated->col;
               j++) { // the q2 for each y
            setMatrixValue(child->getTable()->model()->Matrix(), i, l,
                           getMatrixValue(projects->value(pid)
                                              ->getPLSModel(mid)
                                              ->Model()
                                              ->roc_auc_recalculated,
                                          i, j));
            l++;
            setMatrixValue(child->getTable()->model()->Matrix(), i, l,
                           getMatrixValue(projects->value(pid)
                                              ->getPLSModel(mid)
                                              ->Model()
                                              ->roc_auc_validation,
                                          i, j));
            l++;
            setMatrixValue(
                child->getTable()->model()->Matrix(), i, l,
                getMatrixValue(projects->value(pid)
                                   ->getPLSModel(mid)
                                   ->Model()
                                   ->precision_recall_ap_recalculated,
                               i, j));
            l++;
            setMatrixValue(child->getTable()->model()->Matrix(), i, l,
                           getMatrixValue(projects->value(pid)
                                              ->getPLSModel(mid)
                                              ->Model()
                                              ->precision_recall_ap_validation,
                                          i, j));
            l++;
          }
        }

        LABELS classes = projects->value(pid)->getPLSModel(mid)->getClasses();
        for (size_t j = 0; j < projects->value(pid)
                                   ->getPLSModel(mid)
                                   ->Model()
                                   ->roc_auc_recalculated->col;
             j++) {
          header << QString("Recalculated ROC AUC (%1)").arg(classes[j].name)
                 << QString("Predicted ROC AUC (%1)").arg(classes[j].name)
                 << QString("Recalculated Precision-recall AUC (%1)")
                        .arg(classes[j].name)
                 << QString("Predicted Precision-recall AUC (%1)")
                        .arg(classes[j].name);
        }
      }

      child->getTable()->model()->setObjNames(labels);
      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->getTable()->model()->UpdateModel();
      child->show();
    }
  }
}

void MainWindow::showPLSRecalcY() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
      QString tabname = projectname + " - " + modelname +
                        " - PLS Recalculated Y" + " - " +
                        ui.treeWidget->currentItem()->text(0);
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname,
          projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(
          projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList header;
      header << firstcol_name;

      for (int i = 0; i < projects->value(pid)->getPLSModel(mid)->getNPC();
           i++) {
        for (size_t j = 0;
             j <
             projects->value(pid)->getPLSModel(mid)->Model()->yloadings->row;
             j++) {
          header << QString("y %1 (PC %2)")
                        .arg(QString::number(j + 1))
                        .arg(QString::number(i + 1));
        }
      }

      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPLSExpVar() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
      QString tabname =
          projectname + " - " + modelname + " - PLS Explained Variance";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname);
      size_t row =
          projects->value(pid)->getPLSModel(mid)->Model()->xvarexp->size;
      size_t col = 2 +
                   projects->value(pid)
                       ->getPLSModel(mid)
                       ->Model()
                       ->r2y_recalculated->col +
                   projects->value(pid)
                       ->getPLSModel(mid)
                       ->Model()
                       ->sdec->col; // the explained variance, the sum of
                                    // explained variance, the r^2 y

      child->getTable()->model()->newMatrix(row, col);

      double sumx = 0.f;
      QStringList labels;
      for (size_t i = 0;
           i < projects->value(pid)->getPLSModel(mid)->Model()->xvarexp->size;
           i++) {
        labels.append("PC " + QString::number(i + 1));
        setMatrixValue(
            child->getTable()->model()->Matrix(), i, 0,
            getDVectorValue(
                projects->value(pid)->getPLSModel(mid)->Model()->xvarexp, i));
        sumx += getDVectorValue(
            projects->value(pid)->getPLSModel(mid)->Model()->xvarexp, i);
        if (sumx > 100) {
          setMatrixValue(child->getTable()->model()->Matrix(), i, 1, 100);
        } else {
          setMatrixValue(child->getTable()->model()->Matrix(), i, 1, sumx);
        }

        int l = 2;
        for (size_t j = 0; j < projects->value(pid)
                                   ->getPLSModel(mid)
                                   ->Model()
                                   ->r2y_recalculated->col;
             j++) {
          setMatrixValue(child->getTable()->model()->Matrix(), i, l,
                         getMatrixValue(projects->value(pid)
                                            ->getPLSModel(mid)
                                            ->Model()
                                            ->r2y_recalculated,
                                        i, j));
          l++;
          setMatrixValue(
              child->getTable()->model()->Matrix(), i, l,
              getMatrixValue(
                  projects->value(pid)->getPLSModel(mid)->Model()->sdec, i, j));
          l++;
        }
      }
      child->getTable()->model()->setObjNames(labels);
      QStringList header;
      header << "Principal Component"
             << "X Exp Variance"
             << "Accum X Exp Variance";
      for (size_t j = 0; j < projects->value(pid)
                                 ->getPLSModel(mid)
                                 ->Model()
                                 ->r2y_recalculated->col;
           j++) {
        header << QString("r^2(y: %1)").arg(QString::number(j + 1));
      }

      for (size_t j = 0;
           j < projects->value(pid)->getPLSModel(mid)->Model()->sdec->col;
           j++) {
        header << QString("SDEC(y: %1)").arg(QString::number(j + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->getTable()->model()->UpdateModel();
      child->show();
    }
  }
}

void MainWindow::showPLSRegCoeff() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

      QString tabname = projectname + "-PLS Rergression Coefficient";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname);

      child->getTable()->model()->newMatrix(
          projects->value(pid)->getPLSModel(mid)->Model()->b->size, 1);

      QStringList labels;
      for (size_t i = 0;
           i < projects->value(pid)->getPLSModel(mid)->Model()->xvarexp->size;
           i++) {
        labels.append("PC " + QString::number(i + 1));
        setMatrixValue(
            child->getTable()->model()->Matrix(), i, 0,
            getDVectorValue(projects->value(pid)->getPLSModel(mid)->Model()->b,
                            i));
      }
      child->getTable()->model()->setObjNames(labels);
      QStringList header;
      header << "Principal Component"
             << "Regression Coefficient";
      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->getTable()->model()->UpdateModel();
      child->show();
    }
  }
}

void MainWindow::showPLSWWeights() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

      QString tabname = projectname + " - " + modelname + " - PLS W Weights";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname, projects->value(pid)->getPLSModel(mid)->Model()->xweights);
      QStringList varname =
          projects->value(pid)->getPLSModel(mid)->getXVarName();
      child->getTable()->model()->setObjNames(varname);
      QStringList headername;
      headername << "Variables";
      for (size_t c = 0;
           c < projects->value(pid)->getPLSModel(mid)->Model()->xweights->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPLSQLoadings() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

      QString tabname = projectname + " - " + modelname + " - PLS Q Loadings";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname, projects->value(pid)->getPLSModel(mid)->Model()->yloadings);
      QStringList varname =
          projects->value(pid)->getPLSModel(mid)->getYVarName();
      child->getTable()->model()->setObjNames(varname);
      QStringList headername;
      headername << "Variables";
      for (size_t c = 0;
           c < projects->value(pid)->getPLSModel(mid)->Model()->yloadings->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPLSPLoadings() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

      QString tabname = projectname + " - " + modelname + " - PLS P Loadings";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname, projects->value(pid)->getPLSModel(mid)->Model()->xloadings);

      QStringList varname =
          projects->value(pid)->getPLSModel(mid)->getXVarName();
      child->getTable()->model()->setObjNames(varname);
      QStringList headername;
      headername << "Variables";
      for (size_t c = 0;
           c < projects->value(pid)->getPLSModel(mid)->Model()->xloadings->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPLSUSCores() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

      QString tabname = projectname + " - " + modelname + " - PLS U Scores";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)->getPLSModel(mid)->Model()->yscores,
                      &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(
          projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList headername;
      headername << firstcol_name;
      for (size_t c = 0;
           c < projects->value(pid)->getPLSModel(mid)->Model()->yscores->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPLSTScores() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

      QString tabname = projectname + " - " + modelname + " - PLS T Scores";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)->getPLSModel(mid)->Model()->xscores,
                      &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(
          projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList headername;
      headername << firstcol_name;
      for (size_t c = 0;
           c < projects->value(pid)->getPLSModel(mid)->Model()->xscores->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showCPCABlockScoresPred() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      int predid = getCurrentPredictionID();
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
      /*
       * block_scores sizes
       *  - order = npc
       *  - rows = nobjs
       *  - col = nblocks
       *  Reorg everything!
       */
      size_t nobjs = projects->value(pid)
                         ->getCPCAModel(mid)
                         ->getCPCAPrediction(predid)
                         ->getPredSuperScores()
                         ->row;
      size_t npc = projects->value(pid)
                       ->getCPCAModel(mid)
                       ->getCPCAPrediction(predid)
                       ->getPredSuperScores()
                       ->col;
      size_t nblocks =
          projects->value(pid)->getCPCAModel(mid)->getVarName().size();

      for (size_t k = 0; k < nblocks; k++) {
        QString block_name = projects->value(pid)
                                 ->getCPCAModel(mid)
                                 ->getVarName()[k]
                                 .name; // not right...
        QString tabname =
            projectname + " - " + modelname +
            QString(" - CPCA Block Scores %1 Prediction").arg(block_name);
        matrix *m;
        NewMatrix(&m, nobjs, npc);
        for (size_t i = 0; i < nobjs; i++)
          for (size_t j = 0; j < npc; j++)
            m->data[i][j] = projects->value(pid)
                                ->getCPCAModel(mid)
                                ->getCPCAPrediction(predid)
                                ->getPredBlockScores()
                                ->m[j]
                                ->data[i][k];

        MDIChild *child = createMdiChild();
        child->setWindowID(tabid);
        child->newTable(tabname, m, &projects->value(pid)->getObjectLabels(),
                        &projects->value(pid)->getVariableLabels());
        child->getTable()->model()->setObjNames(projects->value(pid)
                                                    ->getCPCAModel(mid)
                                                    ->getCPCAPrediction(predid)
                                                    ->getObjName());
        QStringList headername;
        headername << firstcol_name;
        for (size_t c = 0; c < m->col; c++) {
          headername << QString("PC %1").arg(QString::number(c + 1));
        }
        child->getTable()->model()->setHorizontalHeaderLabels(headername);
        child->show();
        child->getTable()->setPID(pid);
        connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
        DelMatrix(&m);
      }
    }
  }
}

void MainWindow::showCPCASuperScorePred() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      int predid = getCurrentPredictionID();
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - CPCA Super Scores";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)
                          ->getCPCAModel(mid)
                          ->getCPCAPrediction(predid)
                          ->getPredSuperScores(),
                      &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(projects->value(pid)
                                                  ->getCPCAModel(mid)
                                                  ->getCPCAPrediction(predid)
                                                  ->getObjName());

      QStringList headername;
      headername << firstcol_name;
      for (size_t c = 0; c < projects->value(pid)
                                 ->getCPCAModel(mid)
                                 ->getCPCAPrediction(predid)
                                 ->getPredSuperScores()
                                 ->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showCPCAExpVar() {
  if (CurrentIsModel() == true) {

    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - CPCA Explained Variance";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname);

      size_t row =
          projects->value(pid)->getCPCAModel(mid)->Model()->total_expvar->size;
      size_t col =
          2; // the explained variance and the sum of explained variance

      child->getTable()->model()->newMatrix(row, col);

      double sum = 0.f;
      QStringList labels;
      for (size_t i = 0; i < row; i++) {
        labels.append("PC " + QString::number(i + 1));
        setMatrixValue(
            child->getTable()->model()->Matrix(), i, 0,
            getDVectorValue(
                projects->value(pid)->getCPCAModel(mid)->Model()->total_expvar,
                i));
        sum += getDVectorValue(
            projects->value(pid)->getCPCAModel(mid)->Model()->total_expvar, i);
        if (sum > 100) {
          setMatrixValue(child->getTable()->model()->Matrix(), i, 1, 100);
        } else {
          setMatrixValue(child->getTable()->model()->Matrix(), i, 1, sum);
        }
      }
      child->getTable()->model()->setObjNames(labels);
      QStringList header;
      header << "Principal Component"
             << "X Exp Variance"
             << "Accum X Exp Variance";
      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->getTable()->model()->UpdateModel();
      child->show();
    }
  }
}

void MainWindow::showCPCABlockLoadings() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();

      for (size_t k = 0; k < projects->value(pid)
                                 ->getCPCAModel(mid)
                                 ->Model()
                                 ->block_loadings->order;
           k++) {
        QString block_name =
            projects->value(pid)->getCPCAModel(mid)->getVarName()[k].name;
        QString tabname = projectname + " - " + modelname +
                          QString(" - CPCA Block Loadings %1").arg(block_name);
        MDIChild *child = createMdiChild();
        child->setWindowID(tabid);
        child->newTable(tabname, projects->value(pid)
                                     ->getCPCAModel(mid)
                                     ->Model()
                                     ->block_loadings->m[k]);
        child->getTable()->model()->setObjNames(
            projects->value(pid)->getCPCAModel(mid)->getVarName()[k].objects);
        QStringList headername;
        headername << firstcol_name;
        for (size_t c = 0; c < projects->value(pid)
                                   ->getCPCAModel(mid)
                                   ->Model()
                                   ->block_loadings->m[k]
                                   ->col;
             c++) {
          headername << QString("PC %1").arg(QString::number(c + 1));
        }
        child->getTable()->model()->setHorizontalHeaderLabels(headername);
        child->show();
        child->getTable()->setPID(pid);
        connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      }
    }
  }
}

void MainWindow::showCPCABlockScores() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();
      /*
       * block_scores sizes
       *  - order = npc
       *  - rows = nobjs
       *  - col = nblocks
       *  Reorg everything!
       */
      size_t nobjs =
          projects->value(pid)->getCPCAModel(mid)->Model()->super_scores->row;
      size_t npc =
          projects->value(pid)->getCPCAModel(mid)->Model()->super_scores->col;
      size_t nblocks =
          projects->value(pid)->getCPCAModel(mid)->getVarName().size();

      for (size_t k = 0; k < nblocks; k++) {
        QString block_name = projects->value(pid)
                                 ->getCPCAModel(mid)
                                 ->getVarName()[k]
                                 .name; // not right...
        QString tabname = projectname + " - " + modelname +
                          QString(" - CPCA Block Scores %1").arg(block_name);
        matrix *m;
        NewMatrix(&m, nobjs, npc);
        for (size_t i = 0; i < nobjs; i++)
          for (size_t j = 0; j < npc; j++)
            m->data[i][j] = projects->value(pid)
                                ->getCPCAModel(mid)
                                ->Model()
                                ->block_scores->m[j]
                                ->data[i][k];

        MDIChild *child = createMdiChild();
        child->setWindowID(tabid);
        child->newTable(tabname, m, &projects->value(pid)->getObjectLabels(),
                        &projects->value(pid)->getVariableLabels());
        child->getTable()->model()->setObjNames(
            projects->value(pid)->getCPCAModel(mid)->getObjName());
        QStringList headername;
        headername << firstcol_name;
        for (size_t c = 0; c < m->col; c++) {
          headername << QString("PC %1").arg(QString::number(c + 1));
        }
        child->getTable()->model()->setHorizontalHeaderLabels(headername);
        child->show();
        child->getTable()->setPID(pid);
        connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
        DelMatrix(&m);
      }
    }
  }
}

void MainWindow::showCPCASuperWeights() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - CPCA Super Weights";
      QStringList block_names;
      for (int i = 0;
           i < projects->value(pid)->getCPCAModel(mid)->getVarName().size();
           i++)
        block_names
            << projects->value(pid)->getCPCAModel(mid)->getVarName()[i].name;

      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname,
          projects->value(pid)->getCPCAModel(mid)->Model()->super_weights);
      child->getTable()->model()->setObjNames(block_names);
      QStringList headername;
      headername << firstcol_name;
      for (size_t c = 0;
           c <
           projects->value(pid)->getCPCAModel(mid)->Model()->super_weights->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showCPCASuperScore() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getCPCAModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - CPCA Super Scores";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname,
          projects->value(pid)->getCPCAModel(mid)->Model()->super_scores,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(
          projects->value(pid)->getCPCAModel(mid)->getObjName());
      QStringList headername;
      headername << firstcol_name;
      for (size_t c = 0;
           c <
           projects->value(pid)->getCPCAModel(mid)->Model()->super_scores->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPCAPredScore() {
  if (CurrentIsPrediction() == true) {
    int pid = getCurrentPredictionProjectID();
    int mid = getCurrentPredictionModelID();
    int tabid = getCurrentPredictionTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPCAModel(mid)->getName();
      int predid = getCurrentPredictionID();
      QString tabname = projectname + " - " + modelname +
                        " - PCA Predicted Scores" + " - " +
                        ui.treeWidget->currentItem()->text(0);
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname, projects->value(pid)
                                   ->getPCAModel(mid)
                                   ->getPCAPrediction(predid)
                                   ->getPredScores());
      child->getTable()->model()->setObjNames(projects->value(pid)
                                                  ->getPCAModel(mid)
                                                  ->getPCAPrediction(predid)
                                                  ->getObjName());
      QStringList headername;
      headername << firstcol_name;
      for (size_t c = 0; c < projects->value(pid)
                                 ->getPCAModel(mid)
                                 ->getPCAPrediction(predid)
                                 ->getPredScores()
                                 ->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPCAExpVar() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

      QString tabname =
          projectname + " - " + modelname + " - PCA Explained Variance";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname);

      size_t row =
          projects->value(pid)->getPCAModel(mid)->Model()->varexp->size;
      size_t col =
          2; // the explained variance and the sum of explained variance

      child->getTable()->model()->newMatrix(row, col);

      double sum = 0.f;
      QStringList labels;
      for (size_t i = 0; i < row; i++) {
        labels.append("PC " + QString::number(i + 1));
        setMatrixValue(
            child->getTable()->model()->Matrix(), i, 0,
            getDVectorValue(
                projects->value(pid)->getPCAModel(mid)->Model()->varexp, i));
        sum += getDVectorValue(
            projects->value(pid)->getPCAModel(mid)->Model()->varexp, i);
        if (sum > 100) {
          setMatrixValue(child->getTable()->model()->Matrix(), i, 1, 100);
        } else {
          setMatrixValue(child->getTable()->model()->Matrix(), i, 1, sum);
        }
      }
      child->getTable()->model()->setObjNames(labels);
      QStringList header;
      header << "Principal Component"
             << "X Exp Variance"
             << "Accum X Exp Variance";
      child->getTable()->model()->setHorizontalHeaderLabels(header);
      child->getTable()->model()->UpdateModel();
      child->show();
    }
  }
}

void MainWindow::showPCALoadings() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

      QString tabname = projectname + " - " + modelname + " - PCA Loadings";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(
          tabname, projects->value(pid)->getPCAModel(mid)->Model()->loadings);
      QStringList varname =
          projects->value(pid)->getPCAModel(mid)->getVarName();
      child->getTable()->model()->setObjNames(varname);
      QStringList headername;
      headername << "Variables";
      for (size_t c = 0;
           c < projects->value(pid)->getPCAModel(mid)->Model()->loadings->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::showPCAScore() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      QString modelname = projects->value(pid)->getPCAModel(mid)->getName();

      QString tabname = projectname + " - " + modelname + " - PCA Scores";
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newTable(tabname,
                      projects->value(pid)->getPCAModel(mid)->Model()->scores,
                      &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels());
      child->getTable()->model()->setObjNames(
          projects->value(pid)->getPCAModel(mid)->getObjName());
      QStringList headername;
      headername << firstcol_name;
      for (size_t c = 0;
           c < projects->value(pid)->getPCAModel(mid)->Model()->scores->col;
           c++) {
        headername << QString("PC %1").arg(QString::number(c + 1));
      }
      child->getTable()->model()->setHorizontalHeaderLabels(headername);
      child->show();
      child->getTable()->setPID(pid);
      connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  }
}

void MainWindow::ModelInfo() {
  if (CurrentIsModel() == true) {
    int pid = getCurrentModelProjectID();
    int mid = getCurrentModelID();
    int tabid = getCurrentModelTableID();
    if (pid > -1 && mid > -1 && tabid > -1) {
      MDIChild *child = createMdiChild();
      child->setWindowID(tabid);
      child->newModelInfo();

      QStringList textlst;
      textlst.append(QString("Model name: %1").arg(getCurrentModelName()));
      textlst.append(QString("Model type: %1").arg(getCurrentModelType()));
      QString xhash = getCurrentModelXhash();
      auto nobj = 0;
      auto nvars = 0;
      auto nblocks = 0;
      auto ntarg = 0;
      auto xscaling = getCurrentModelXScalingType();
      auto yscaling = getCurrentModelYScalingType();
      auto xoid = projects->value(pid)->getMatrixID(xhash);
      QString moname;
      auto validationtype = -1;
      auto combinationrule = -1;

      if (getCurrentModelType().compare("PCA Model") == 0) {
        textlst.append(QString("N. PCs: %1").arg(getCurrentModelNComponents()));
        nobj = projects->value(pid)->getPCAModel(mid)->getObjName().size();
        nvars = projects->value(pid)->getPCAModel(mid)->getVarName().size();
      } else if (getCurrentModelType().compare("CPCA Model") == 0) {
        textlst.append(QString("N. PCs: %1").arg(getCurrentModelNComponents()));
        nobj = projects->value(pid)->getCPCAModel(mid)->getObjName().size();
        nblocks = projects->value(pid)->getCPCAModel(mid)->getVarName().size();
      } else if (getCurrentModelType().compare("PLS Model") == 0) {
        textlst.append(QString("N. LVs: %1").arg(getCurrentModelNComponents()));
        nobj = projects->value(pid)->getPLSModel(mid)->getObjName().size();
        nvars = projects->value(pid)->getPLSModel(mid)->getXVarName().size();
        ntarg = projects->value(pid)->getPLSModel(mid)->getYVarName().size();
        validationtype =
            projects->value(pid)->getPLSModel(mid)->getValidation();
      } else if (getCurrentModelType().compare("MLR Model") == 0) {
        nobj = projects->value(pid)->getMLRModel(mid)->getObjName().size();
        nvars = projects->value(pid)->getMLRModel(mid)->getXVarName().size();
        ntarg = projects->value(pid)->getMLRModel(mid)->getYVarName().size();
        validationtype =
            projects->value(pid)->getMLRModel(mid)->getValidation();
      } else if (getCurrentModelType().compare("LDA Model") == 0) {
        nobj = projects->value(pid)->getLDAModel(mid)->getObjName().size();
        nvars = projects->value(pid)->getLDAModel(mid)->getVarName().size();
        ntarg = projects->value(pid)->getLDAModel(mid)->Model()->nclass;
      }

      if (nobj > 0)
        textlst.append(QString("N. objects: %1").arg(QString::number(nobj)));

      if (nvars > 0)
        textlst.append(QString("N. variables: %1").arg(QString::number(nvars)));

      if (nblocks > 0) {
        textlst.append(QString("N. Blocks: %1").arg(QString::number(nblocks)));
        for (int i = 0; i < nblocks; i++) {
          nvars = projects->value(pid)
                      ->getCPCAModel(mid)
                      ->getVarName()[i]
                      .objects.size();
          textlst.append(QString("N. Vars Block %1: %2")
                             .arg(QString::number(i + 1))
                             .arg(QString::number(nvars)));
        }
      }

      if (ntarg > 0)
        textlst.append(QString("N. targets: %1").arg(QString::number(ntarg)));

      if (xoid > -1)
        textlst.append(
            QString("Origin data: %1")
                .arg(projects->value(pid)->getMatrix(xoid)->getName()));
      else {
        if (moname.compare("Origin model not found") != 0) {
          textlst.append(QString("Origin model: %1").arg(moname));
        }
      }

      if (getCurrentModelType().compare("PCA Model") == 0 ||
          getCurrentModelType().compare("CPCA Model") == 0) {
        textlst.append(QString("X centered"));
      } else if (getCurrentModelType().compare("PLS Model") == 0) {
        textlst.append(QString("X and Y centered"));
      } else { // LDA Model
        if (getCurrentModelType().compare("MLR Model") == 0) {
          textlst.append(QString("X and Y as is"));
        } else {
          textlst.append(QString("X as is"));
        }
      }

      if (getCurrentModelType().compare("PCA Model") == 0 ||
          getCurrentModelType().compare("CPCA Model") == 0 ||
          getCurrentModelType().compare("PLS Model") == 0) {
        if (xscaling == 0)
          textlst.append(QString("X not scaled"));
        else if (xscaling == 1)
          textlst.append(
              QString("X scaling type: %1").arg("Standard Deviation"));
        else if (xscaling == 2)
          textlst.append(
              QString("X scaling type: %1").arg("Column root mean square"));
        else if (xscaling == 3)
          textlst.append(QString("X scaling type: %1").arg("Pareto"));
        else if (xscaling == 4)
          textlst.append(
              QString("X scaling type: %1").arg("Min-Max range scaling"));
        else if (xscaling == 5)
          textlst.append(QString("X scaling type: %1").arg("Level scaling"));
      }

      if (getCurrentModelType().compare("PLS Model") == 0) {
        if (yscaling == 0)
          textlst.append(QString("Y not scaled"));
        else if (yscaling == 1)
          textlst.append(
              QString("Y scaling type: %1").arg("Standard Deviation"));
        else if (yscaling == 2)
          textlst.append(
              QString("Y scaling type: %1").arg("Column root mean square"));
        else if (yscaling == 3)
          textlst.append(QString("Y scaling type: %1").arg("Pareto"));
        else if (yscaling == 4)
          textlst.append(
              QString("Y scaling type: %1").arg("Min-Max range scaling"));
        else if (yscaling == 5)
          textlst.append(QString("Y scaling type: %1").arg("Level scaling"));
      }

      if (validationtype == 0) {
        textlst.append(QString("Model Not Validated"));
      } else if (validationtype == LOO_) {
        textlst.append(
            QString("Model validation type: %1").arg("Leave One Out"));
      } else if (validationtype == BOOTSTRAPRGCV_) {
        textlst.append(
            QString("Model validation type: %1")
                .arg("Bootstrap Random Group k-Fold Cross Validation"));
      }

      if (combinationrule == Averaging) {
        textlst.append(QString("Combination Rule: %1").arg("Averaging"));
      } else if (combinationrule == Median) {
        textlst.append(QString("Combination Rule: %1").arg("Median"));
      }

      child->getModelInfoWindow()->setText(textlst);
      child->show();
    }
  }
}

/*
 * pid - pid  (2)
 *    |
 *    | Data   // child 0
 *    |   |
 *    |   Name - Type - Tab Count - Data Position - pid  (5)
 *    |
 *    | Models  // child 1
 *        |
 *        Name - Tab Count - pid - x ID - y ID - x Scaling type - y Scaling type
 * - number of components - Model Type - Data Position  (10)
 *          |
 *         ModelPrediction Name - Tab Count - pid - Model ID - xdata id - ydata
 * id - Data Position - Data Type (PCA Prediction, PLS Prediction, ...) (8)
 *
 * Type could be "Matrix" or "Array"
 * Tab count is an id used to know what tables are opened and to close the
 * deleted table Data Position is used to retrieve the Data Position inside the
 * QList<MATRIX>,  QList<Array> QList<PCAModel> etc... Model Type can be PCA
 * Model, PLS Model, LDA Model, MLR Model
 */
void MainWindow::ShowContextMenu(const QPoint &pos) {
  if (ui.treeWidget->topLevelItemCount() > 0 &&
      ui.treeWidget->selectedItems().size() > 0 &&
      ui.treeWidget->currentItem()->isSelected() == true &&
      ui.treeWidget->currentItem()->columnCount() > 1) {

    QPoint globalPos = ui.treeWidget->mapToGlobal(pos);
    QMenu menu;

#ifdef DEBUG
    qDebug() << "[SHOWCONTEXTMENU] Selected item number of column: "
             << ui.treeWidget->currentItem()->columnCount();
#endif

    if (CurrentIsData() == true) { // this is a Data
      menu.addAction("&Show data", this, SLOT(showData()));
      menu.addAction("&Show descriptive statistics", this,
                     SLOT(showDescrpitiveStatistics()));
      menu.addAction("&Remove data", this, SLOT(removeData()));
      menu.exec(globalPos);
    } else if (CurrentIsModel() == true) {
      QString modeltype = getCurrentModelType();
      int pid = getCurrentModelProjectID();
      int mid = getCurrentModelID();
      if (modeltype.compare("PCA Model") == 0) {
        menu.addAction("&Model Info", this, SLOT(ModelInfo()));
        menu.addAction("&Show T Score", this, SLOT(showPCAScore()));
        menu.addAction("&Show P Loadings", this, SLOT(showPCALoadings()));
        menu.addAction("&Show Explained Variance", this, SLOT(showPCAExpVar()));
        menu.addAction("&Remove Model", this, SLOT(removeModel()));
        menu.exec(globalPos);
      }
      if (modeltype.compare("CPCA Model") == 0) {
        menu.addAction("&Model Info", this, SLOT(ModelInfo()));
        menu.addAction("&Show Super Score", this, SLOT(showCPCASuperScore()));
        menu.addAction("&Show Super Weights", this,
                       SLOT(showCPCASuperWeights()));
        menu.addAction("&Show Block Scores", this, SLOT(showCPCABlockScores()));
        menu.addAction("&Show Block Loadings", this,
                       SLOT(showCPCABlockLoadings()));
        menu.addAction("&Show Explained Variance", this,
                       SLOT(showCPCAExpVar()));
        menu.addAction("&Remove Model", this, SLOT(removeModel()));
        menu.exec(globalPos);
      } else if (modeltype.compare("PLS Model") == 0) {
        menu.addAction("&Model Info", this, SLOT(ModelInfo()));
        menu.addAction("&Show T Score", this, SLOT(showPLSTScores()));
        menu.addAction("&Show U Score", this, SLOT(showPLSUSCores()));
        menu.addAction("&Show P Loadings", this, SLOT(showPLSPLoadings()));
        menu.addAction("&Show Q Loadings", this, SLOT(showPLSQLoadings()));
        menu.addAction("&Show W Weights", this, SLOT(showPLSWWeights()));
        menu.addAction("&Show Regression Coefficient", this,
                       SLOT(showPLSRegCoeff()));
        menu.addAction("&Show Explained Variance", this, SLOT(showPLSExpVar()));
        menu.addAction("&Show Recalculated Y", this, SLOT(showPLSRecalcY()));
        if (projects->value(pid)->getPLSModel(mid)->getValidation() > 0) {
          menu.addAction("&Show Predicted Y", this,
                         SLOT(showPLSValidatedPrediction()));
          menu.addAction("&Show Validation", this, SLOT(showPLSValidation()));
        }

        menu.addAction("&Remove Model", this, SLOT(removeModel()));
        menu.exec(globalPos);
      } else if (modeltype.compare("MLR Model") == 0) {
        menu.addAction("&Model Info", this, SLOT(ModelInfo()));
        menu.addAction("&Show Regression Coefficient", this,
                       SLOT(showMLRCoeff()));
        menu.addAction("&Show Recalculated Y", this, SLOT(showMLRRecalcY()));
        if (projects->value(pid)->getMLRModel(mid)->getValidation() > 0) {
          menu.addAction("&Show Predicted Y", this,
                         SLOT(showMLRValidatedPrediction()));
          menu.addAction("&Show Validation", this, SLOT(showMLRValidation()));
        }

        menu.addAction("&Remove Model", this, SLOT(removeModel()));
        menu.exec(globalPos);
      } else if (modeltype.compare("LDA Model") == 0) {
        menu.addAction("&Model Info", this, SLOT(ModelInfo()));
        menu.addAction("&Show Covariance Group Matrix", this,
                       SLOT(showLDACovarianceGroupMatrix()));
        menu.addAction("&Show Prior Probabilities", this,
                       SLOT(showLDAPriorProbabilities()));
        menu.addAction("&Show Features", this, SLOT(showLDAFeatures()));
        menu.addAction("&Show MVA Normal Distribution", this,
                       SLOT(showLDAMVNormDistrib()));

        if (projects->value(pid)->getLDAModel(mid)->getValidation() > 0) {
          menu.addAction("&Show Validation", this, SLOT(showLDAValidation()));
        }

        menu.addAction("&Remove Model", this, SLOT(removeModel()));
        menu.exec(globalPos);
      } else {
        return;
      }
    } else if (CurrentIsProject() == true) { // this is the project
      menu.addAction("&Add Data", this, SLOT(addData()));
      menu.addAction("&Remove Project", this, SLOT(removeProject()));
      menu.exec(globalPos);
    } else if (CurrentIsPrediction() == true) { // this is the ModelPrediction
      QString predictiontype = getCurrentPredictionType();
      if (predictiontype.compare("PCA Prediction") == 0) {
        menu.addAction("&Show Prediction Score", this,
                       SLOT(showPCAPredScore()));
        menu.addAction("&Remove Prediction", this, SLOT(removePrediction()));
        menu.exec(globalPos);
      } else if (predictiontype.compare("CPCA Prediction") == 0) {
        menu.addAction("&Show Prediction Super Score", this,
                       SLOT(showCPCASuperScorePred()));
        menu.addAction("&Show Prediction Block Score", this,
                       SLOT(showCPCABlockScoresPred()));
        menu.addAction("&Remove Prediction", this, SLOT(removePrediction()));
        menu.exec(globalPos);
      } else if (predictiontype.compare("PLS Prediction") == 0) {
        menu.addAction("&Show Prediction Score", this,
                       SLOT(showPLSPredScore()));
        menu.addAction("&Show Predicted Y", this, SLOT(showPLSPrediction()));
        if (getCurrentPredictionYhash().compare("None") != 0) {
          menu.addAction("&Show Prediction Error", this,
                         SLOT(showPLSPredictionRSquared()));
        }
        menu.addAction("&Remove Prediction", this, SLOT(removePrediction()));
        menu.exec(globalPos);
      }

      else if (predictiontype.compare("MLR Prediction") == 0) {
        menu.addAction("&Show Predicted Y", this, SLOT(showMLRPrediction()));
        if (getCurrentPredictionYhash().compare("None") != 0) {
          menu.addAction("&Show Prediction Error", this,
                         SLOT(showMLRPredictionRSquared()));
        }
        menu.addAction("&Remove Prediction", this, SLOT(removePrediction()));
        menu.exec(globalPos);
      } else if (predictiontype.compare("LDA Prediction") == 0) {
        menu.addAction("&Show Predicted Class", this,
                       SLOT(showLDAPrediction()));
        menu.addAction("&Show Predicted Features", this,
                       SLOT(showLDAPredictionFeatures()));
        menu.addAction("&Remove Prediction", this, SLOT(removePrediction()));
        menu.exec(globalPos);
      } else {
        return;
      }
    } else {
      return;
    }
    TopMenuEnableDisable();
  }
}

void MainWindow::DowngradeDataID() {
  if (CurrentIsData() == true) {
    int pid = getCurrentDataProjectID();
    int did = getCurrentDataID();
    // Downgrading of one all the id of the other models
    for (int i = did + 1; i < getDataCount(pid); i++) {
      if (getDataItem(pid, did)->text(1).compare(getDataType(pid, i)) == 0) {
        int childid = getDataItem(pid, i)->text(3).toInt();
        ui.treeWidget->currentItem()->parent()->child(i)->setText(
            3, QString::number(childid - 1));
      } else {
        continue;
      }
    }
  }
}

void MainWindow::removeData() {
  if (CurrentIsData() == true) {
    bool removedata;
    int pid = getCurrentDataProjectID();
    if (calcpid != -1 && calcpid == pid) {
      removedata = false;
    } else {
      removedata = true;
    }

    if (removedata == true) {

      DowngradeDataID();

      int did = getCurrentDataID();
      int tabid = getCurrentDataTableID();
      if (getCurrentDataType().compare("Matrix") == 0) {
        updateLog(QString("Deleting Matrix\n"));
        projects->value(pid)->delMatrixAt(did);
      } else { // ui.treeWidget->currentItem()->text(1).compare("Array") == 0){
        updateLog(QString("Deleting Array\n"));
        projects->value(pid)->delArrayAt(did);
      }

      closeMDI(tabid);
      getProjectItem(pid)->child(0)->removeChild(ui.treeWidget->currentItem());
      //     ui.treeWidget->currentItem()->parent()->removeChild();
      TopMenuEnableDisable();
    } else {
      return;
    }
  }
}

void MainWindow::addData() {
  if (CurrentIsProject() == true) {

    QString projectname = getCurrentProjectName();
    int pid = getCurrentProjectID();

    updateLog(QString("Adding new data to project: %1").arg(projectname));

    ProjectManager pmanager(projects->value(pid));
    pmanager.setWindowName("Add data to project...");
    pmanager.setLastPath(lastpath);
    pmanager.setAppend(true);

    int lastsizemx = projects->value(pid)->MatrixCount();
    int lastsizear = projects->value(pid)->ArrayCount();

    if (pmanager.exec() == QDialog::Accepted) {
      for (int i = lastsizemx; i < projects->value(pid)->getMATRIXList().size();
           i++) {
        QTreeWidgetItem *subitem3 = new QTreeWidgetItem;
        subitem3->setText(0,
                          projects->value(pid)
                              ->getMatrix(i)
                              ->getName()); /*set the data name from the file*/
        subitem3->setText(1, QString("Matrix")); // Define the type of the data
        subitem3->setText(
            2, QString::number(tabcount_)); // Define the tab id number in order
                                            // to close a specific table
        subitem3->setText(
            3, QString::number(i)); // Define the matrix position id in order to
                                    // find easly when you need to show data.
        subitem3->setText(
            4, QString::number(pid)); // pid for get the array with Value
        tabcount_++;
        getProjectItem(pid)->child(0)->addChild(subitem3);
      }

      for (int i = lastsizear; i < projects->value(pid)->getARRAYList().size();
           i++) {
        QTreeWidgetItem *subitem3 = new QTreeWidgetItem;
        subitem3->setText(0,
                          projects->value(pid)
                              ->getArray(i)
                              ->getName()); /*set the data name from the file*/
        subitem3->setText(1, QString("Array")); // Define the type of the data
        subitem3->setText(
            2, QString::number(tabcount_)); // Define the tab id number in order
                                            // to close a specific table
        subitem3->setText(
            3, QString::number(i)); // Define the matrix position id in order to
                                    // find easly when you need to show data.
        subitem3->setText(
            4, QString::number(pid)); // pid for get the array with Value
        tabcount_++;
        getProjectItem(pid)->child(0)->addChild(subitem3);
      }
    }

    lastpath = pmanager.getLastPath();
#ifdef DEBUG
    qDebug() << QString("last path %1  %2")
                    .arg(lastpath)
                    .arg(pmanager.getLastPath());
#endif
  }
}

void MainWindow::showData() {
  if (CurrentIsData() == true) {
    int pid = getCurrentDataProjectID();
    int tabid = getCurrentDataTableID();
    int did = getCurrentDataID();
    if (pid > -1 && did > -1 && tabid > -1) {
#ifdef DEBUG
      qDebug() << "showData() Project ID " << pid << " Table ID " << tabid
               << " Data ID " << did;
#endif
      QString projectname = projects->value(pid)->getProjectName();
      if (getCurrentDataType().compare("Matrix") == 0) {

        QString tabname = projectname + "-Matrix-" + getCurrentDataName();

        MDIChild *child = createMdiChild();

#ifdef DEBUG
        qDebug() << "pid: " << pid << " did " << did;
#endif
        child->setWindowID(tabid);
        child->newTable(tabname, projects->value(pid)->getMatrix(did)->Matrix(),
                        &projects->value(pid)->getObjectLabels(),
                        &projects->value(pid)->getVariableLabels());
        child->getTable()->setPID(pid);
        // Set Obj Label
        child->getTable()->model()->setObjNames(
            projects->value(pid)->getMatrix(did)->getObjName());
        // Set Header Label
        child->getTable()->model()->setHorizontalHeaderLabels(
            projects->value(pid)->getMatrix(did)->getVarName());

        child->show();
        connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else { // ui.treeWidget->currentItem()->text(1).compare("Array") == 0){
        for (size_t i = 0;
             i < projects->value(pid)->getArray(did)->Array()->order; i++) {
          QString tabname = projectname + "-Array-Matrix-" +
                            getCurrentDataName() + "-" + QString::number(i + 1);
          MDIChild *child = createMdiChild();

          child->setWindowID(tabid);
          child->newTable(tabname,
                          projects->value(pid)->getArray(did)->Array()->m[i],
                          &projects->value(pid)->getObjectLabels(),
                          &projects->value(pid)->getVariableLabels());
          child->getTable()->setPID(pid);
          // Set Obj Label
          child->getTable()->model()->setObjNames(
              projects->value(pid)->getArray(did)->getObjName());
          // Set Header Label
          child->getTable()->model()->setHorizontalHeaderLabels(
              projects->value(pid)->getArray(did)->getVarName());

          child->show();
          connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
                  SLOT(UpdateImageWindow(ImageSignal)));
        }
      }
    }
  }
}

void MainWindow::showDescrpitiveStatistics() {
  if (CurrentIsData() == true) {
    int pid = getCurrentDataProjectID();
    int tabid = getCurrentDataTableID();
    int did = getCurrentDataID();
    if (pid > -1 && did > -1 && tabid > -1) {
      QString projectname = projects->value(pid)->getProjectName();
      if (getCurrentDataType().compare("Matrix") == 0) {
        matrix *stats;
        initMatrix(&stats);
        MatrixColDescStat(projects->value(pid)->getMatrix(did)->Matrix(),
                          stats);
        QString tabname = projectname + "-Matrix-" + getCurrentDataName() +
                          " Descriptive statistics";
        MDIChild *child = createMdiChild();
        child->setWindowID(tabid);
        QStringList colname;
        colname << "Variables"
                << "Average"
                << "Median"
                << "Armonic average"
                << "Var Pop."
                << "Var Sample"
                << "SD Pop."
                << "SD Sample"
                << "CV Pop. %"
                << "CV Sample %"
                << "Min value"
                << "Max value"
                << "N. zeros"
                << "N. Missing values";
        child->newTable(tabname, stats);
        child->getTable()->setPID(pid);
        child->getTable()->setObjLabels(
            &projects->value(pid)->getVariableLabels());
        // Set Obj Label
        QStringList objname;
        for (int i = 1;
             i < projects->value(pid)->getMatrix(did)->getVarName().size(); i++)
          objname.append(projects->value(pid)->getMatrix(did)->getVarName()[i]);
        child->getTable()->model()->setObjNames(objname);
        // Set Header Label
        child->getTable()->model()->setHorizontalHeaderLabels(colname);
        child->show();
      }
    }
  }
}

void MainWindow::removeProject() {
  if (CurrentIsProject() == true) {
    bool removeproject;
    int pid = getCurrentProjectID();
    if (calcpid != -1 && calcpid == pid) {
      removeproject = false;
    } else {
      removeproject = true;
    }

    if (removeproject == true) {
      updateLog(QString("Removing project: %1").arg(getCurrentProjectName()));
#ifdef DEBUG
      qDebug() << "removeProject()  project removed "
               << getCurrentProjectName();
#endif
      // Remove data if these are opened; child(0) is the "Data" menu section
      for (int i = 0; i < getDataCount(pid); i++) {
        closeMDI(getDataTableID(pid, i));
      }
      // Remove models if these are opened; child(1) is the "Model" menu section
      for (int i = 0; i < getModelCount(pid); i++) {
        closeMDI(getModelTableIDAt(pid, i));
        // Remove prediction if are opened
        for (int j = 0; j < getPredictionCountAt(pid, i); j++) {
          closeMDI(getPredictionTableIDAt(pid, i, j));
        }
      }

      /*
      RECENTMODELS m;
      m.path = projects->value(pid)->getProjectPath();
      m.name = projects->value(pid)->getProjectName();
      if(recents.contains(m) == false){
        RecentsProjectSwap(m);
      }
      */

      delete projects->value(pid);
      projects->remove(pid);
#ifdef DEBUG
      qDebug() << "Remaining projects:";
#endif
      QMap<int, DATA *>::const_iterator i = projects->constBegin();
      while (i != projects->constEnd()) {
#ifdef DEBUG
        qDebug() << i.key() << i.value()->getProjectName();
#endif
        ++i;
      }

      ui.treeWidget->takeTopLevelItem(ui.treeWidget->indexOfTopLevelItem(
          getProjectItem(pid))); // remove the project from the qtreewidget
      TopMenuEnableDisable();

      ImageSignal is;
      is.pid = -1;
      UpdateImageWindow(is);
    }
  }
}

void MainWindow::closeEvent(QCloseEvent *bar) {
  // SaveAllProjects();
  bar->accept();
}

void MainWindow::closeMDI(const int &id) {
  foreach (QMdiSubWindow *window, ui.mdiArea->subWindowList()) {
    MDIChild *mdiChild = qobject_cast<MDIChild *>(window);
    if (mdiChild->getWindowID() == id) {
      delete mdiChild;
    } else
      continue;
  }
}

MDIChild *MainWindow::createMdiChild() {
  MDIChild *child = new MDIChild;
  child->setWindowIcon(QIcon(QPixmap(1, 1)));
  ui.mdiArea->addSubWindow(child);
  return child;
}

void MainWindow::updateLog(QString str) {
  ui.log->append(str);
  qApp->processEvents();
}

void MainWindow::RecentsProjectSwap(RECENTMODELS m) {
  recents[3].path = recents[2].path;
  recents[3].name = recents[2].name;

  recents[2].path = recents[1].path;
  recents[2].name = recents[1].name;

  recents[1].path = recents[0].path;
  recents[1].name = recents[0].name;

  recents[0].path = m.path;
  recents[0].name = m.name;
}

QList<RECENTMODELS> MainWindow::GetRecentModels() {
  QFile file(QFileInfo(QString("%1/%2").arg(confdir).arg("recents"))
                 .absoluteFilePath());
  QList<RECENTMODELS> rlst;
  if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    while (!file.atEnd()) {
      QString line = file.readLine();
      QStringList tmpname = line.split(";");
      if (tmpname.size() == 2) {
        QFileInfo fi(tmpname[1].trimmed());
        if (fi.exists() == true) {
          rlst.append(RECENTMODELS());
          rlst.last().name = tmpname[0];
          rlst.last().path = tmpname[1].trimmed();
        } else {
          rlst.append(RECENTMODELS());
          rlst.last().name = "-";
        }
      } else {
        continue;
      }
    }
    file.close();
  }

  if (rlst.size() < 4) {
    for (int i = rlst.size(); i < 4; i++) {
      rlst.append(RECENTMODELS());
      rlst.last().name = "-";
    }
  }

  return rlst;
}

void MainWindow::LoadRecentsModelsFile() {
  QList<RECENTMODELS> rlst = GetRecentModels();
  recents.clear();
  for (int i = 0; i < rlst.size(); i++) {
    QString model_name = rlst[i].name;
    QString model_path = rlst[i].path;
    if (i == 0) {
      qDebug() << model_name;
      ui.action_1->setText(model_name);
      recents.append(rlst[i]);
      connect(ui.action_1, SIGNAL(triggered(bool)), SLOT(OpenRecent1()));
      qDebug() << ui.action_1->text();
    } else if (i == 1) {
      ui.action_2->setText(model_name);
      recents.append(rlst[i]);
      connect(ui.action_2, SIGNAL(triggered(bool)), SLOT(OpenRecent2()));
    } else if (i == 2) {
      ui.action_3->setText(model_name);
      recents.append(rlst[i]);
      connect(ui.action_3, SIGNAL(triggered(bool)), SLOT(OpenRecent3()));
    } else {
      ui.action_4->setText(model_name);
      recents.append(rlst[i]);
      connect(ui.action_4, SIGNAL(triggered(bool)), SLOT(OpenRecent4()));
    }
  }
}

void MainWindow::WriteRecentsModelsFile() {
  bool recents_changed = false;
  QList<RECENTMODELS> rlst = GetRecentModels();
  if (rlst.size() > 0) {
    for (int i = 0; i < rlst.size(); i++) {
      if (recents[i].name.compare(rlst[i].name) == 0 &&
          recents[i].path.compare(rlst[i].path) == 0) {
        recents_changed = true;
        break;
      } else {
        continue;
        ;
      }
    }
  } else {
    recents_changed = true;
  }

  if (recents_changed == true) {
    QFile file(QFileInfo(QString("%1/%2").arg(confdir).arg("recents"))
                   .absoluteFilePath());
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    for (int i = 0; i < recents.size(); i++) {
      if (recents[i].name.compare("-") != 0 && recents[i].name.size() > 0) {
        QFileInfo fi(recents[i].path);
        out << recents[i].name << ";" << recents[i].path.trimmed() << "\n";
      } else {
        continue;
      }
    }
    file.close();
  }
}

void MainWindow::DebugProjectTree(ProjectTree pjtree) {
#ifdef DEBUG
  qDebug() << "DebugProjectTree";
  int i, j, k;
  for (i = 0; i < pjtree.size(); i++) {
    qDebug() << "Project name: " << pjtree[i].name << " id: " << pjtree[i].id;
    for (j = 0; j < pjtree[i].mtree.size(); j++) {
      qDebug() << "Model name: " << pjtree[i].mtree[j].name
               << " id: " << pjtree[i].mtree[j].id;
      for (k = 0; k < pjtree[i].mtree[j].ptree.size(); k++) {
        qDebug() << "Prediction name: " << pjtree[i].mtree[j].ptree[k].name
                 << " id: " << pjtree[i].mtree[j].ptree[k].id;
      }
    }
    qDebug() << "-----------";
  }
#endif
}

void MainWindow::GetPCAProjects(ProjectTree *pjtree) {
  int i, j, k;
  for (i = 0; i < projects->keys().size(); i++) {
    PROJECT a;
    int pid = projects->keys()[i];
    a.name = projects->value(pid)->getProjectName();
    a.id = pid;
    for (j = 0; j < projects->value(pid)->PCACount(); j++) {
      MODELTREE m;
      m.name = projects->value(pid)->getPCAModelAt(j)->getName();
      m.id = projects->value(pid)->getPCAModelAt(j)->getModelID();
      for (k = 0;
           k < projects->value(pid)->getPCAModelAt(j)->PCAPredictionCount();
           k++) {
        m.ptree.append(PREDICTIONTREE());
        m.ptree.last().name = projects->value(pid)
                                  ->getPCAModelAt(j)
                                  ->getPCAPrediction(k)
                                  ->getName();
        m.ptree.last().id = projects->value(pid)
                                ->getPCAModelAt(j)
                                ->getPCAPrediction(k)
                                ->getPredID();
      }
      a.mtree << m;
    }

    if (a.mtree.size() > 0) {
      (*pjtree) << a;
    } else {
      continue;
    }
  }
}

void MainWindow::GetCPCAProjects(ProjectTree *pjtree) {
  int i, j, k;
  for (i = 0; i < projects->keys().size(); i++) {
    PROJECT a;
    int pid = projects->keys()[i];
    a.name = projects->value(pid)->getProjectName();
    a.id = pid;
    for (j = 0; j < projects->value(pid)->CPCACount(); j++) {
      MODELTREE m;
      m.name = projects->value(pid)->getCPCAModelAt(j)->getName();
      m.id = projects->value(pid)->getCPCAModelAt(j)->getModelID();
      for (k = 0;
           k < projects->value(pid)->getCPCAModelAt(j)->CPCAPredictionCount();
           k++) {
        m.ptree.append(PREDICTIONTREE());
        m.ptree.last().name = projects->value(pid)
                                  ->getCPCAModelAt(j)
                                  ->getCPCAPrediction(k)
                                  ->getName();
        m.ptree.last().id = projects->value(pid)
                                ->getCPCAModelAt(j)
                                ->getCPCAPrediction(k)
                                ->getPredID();
      }
      a.mtree << m;
    }

    if (a.mtree.size() > 0) {
      (*pjtree) << a;
    } else {
      continue;
    }
  }
}

void MainWindow::GetPLSProjects(ProjectTree *pjtree) {
  int i, j, k;
  for (i = 0; i < projects->keys().size(); i++) {
    PROJECT a;
    int pid = projects->keys()[i];
    a.name = projects->value(pid)->getProjectName();
    a.id = pid;
    for (j = 0; j < projects->value(pid)->PLSCount(); j++) {
      MODELTREE m;
      m.name = projects->value(pid)->getPLSModelAt(j)->getName();
      m.id = projects->value(pid)->getPLSModelAt(j)->getModelID();
      for (k = 0;
           k < projects->value(pid)->getPLSModelAt(j)->PLSPredictionCount();
           k++) {
        m.ptree.append(PREDICTIONTREE());
        m.ptree.last().name = projects->value(pid)
                                  ->getPLSModelAt(j)
                                  ->getPLSPrediction(k)
                                  ->getName();
        m.ptree.last().id = projects->value(pid)
                                ->getPLSModelAt(j)
                                ->getPLSPrediction(k)
                                ->getPredID();
      }
      a.mtree << m;
    }

    if (a.mtree.size() > 0) {
      (*pjtree) << a;
    } else {
      continue;
    }
  }
}

void MainWindow::GetMLRProjects(ProjectTree *pjtree) {
  int i, j, k;
  for (i = 0; i < projects->keys().size(); i++) {
    PROJECT a;
    int pid = projects->keys()[i];
    a.name = projects->value(pid)->getProjectName();
    a.id = pid;
    for (j = 0; j < projects->value(pid)->MLRCount(); j++) {
      MODELTREE m;
      m.name = projects->value(pid)->getMLRModelAt(j)->getName();
      m.id = projects->value(pid)->getMLRModelAt(j)->getModelID();
      for (k = 0;
           k < projects->value(pid)->getMLRModelAt(j)->MLRPredictionCount();
           k++) {
        m.ptree.append(PREDICTIONTREE());
        m.ptree.last().name = projects->value(pid)
                                  ->getMLRModelAt(j)
                                  ->getMLRPrediction(k)
                                  ->getName();
        m.ptree.last().id = projects->value(pid)
                                ->getMLRModelAt(j)
                                ->getMLRPrediction(k)
                                ->getPredID();
      }
      a.mtree << m;
    }

    if (a.mtree.size() > 0) {
      (*pjtree) << a;
    } else {
      continue;
    }
  }
}

void MainWindow::GetLDAProjects(ProjectTree *pjtree) {
  int i, j, k;
  for (i = 0; i < projects->keys().size(); i++) {
    PROJECT a;
    int pid = projects->keys()[i];
    a.name = projects->value(pid)->getProjectName();
    a.id = pid;
    for (j = 0; j < projects->value(pid)->LDACount(); j++) {
      MODELTREE m;
      m.name = projects->value(pid)->getLDAModelAt(j)->getName();
      m.id = projects->value(pid)->getLDAModelAt(j)->getModelID();
      for (k = 0;
           k < projects->value(pid)->getLDAModelAt(j)->LDAPredictionCount();
           k++) {
        m.ptree.append(PREDICTIONTREE());
        m.ptree.last().name = projects->value(pid)
                                  ->getLDAModelAt(j)
                                  ->getLDAPrediction(k)
                                  ->getName();
        m.ptree.last().id = projects->value(pid)
                                ->getLDAModelAt(j)
                                ->getLDAPrediction(k)
                                ->getPredID();
      }
      a.mtree << m;
    }

    if (a.mtree.size() > 0) {
      (*pjtree) << a;
    } else {
      continue;
    }
  }
}

void MainWindow::ProjectWindow() {
  if (ui.actionProject_Window->isChecked())
    ui.dockWidget->show();
  else
    ui.dockWidget->hide();
}

void MainWindow::LogWindow() {
  if (ui.actionLog_Window->isChecked())
    ui.dockWidget_2->show();
  else
    ui.dockWidget_2->hide();
}

void MainWindow::ImageWindow() {
  if (ui.actionViewer_Window->isChecked()) {
    ui.dockWidget_3->show();
  } else
    ui.dockWidget_3->hide();
}

void MainWindow::NewProject() {
  projects->insert(pid_, new DATA());
  projects->value(pid_)->setProjectID(pid_);

  ProjectManager pmanager(projects->value(pid_));
  pmanager.setWindowName("Create New Project...");
  pmanager.setLastPath(lastpath);
  pmanager.setAppend(false);

  if (pmanager.exec() == QDialog::Accepted) {
    if (!pmanager.getName().isEmpty()) {

      updateLog(QString("Creating New Project: %1\n").arg(pmanager.getName()));

      QTreeWidgetItem *item = new QTreeWidgetItem;
      item->setText(0, pmanager.getName());
      item->setText(1, QString::number(pid_));

      projects->value(pid_)->setProjectName(pmanager.getName());

      QTreeWidgetItem *subitem1 = new QTreeWidgetItem;
      subitem1->setText(0, "Data");
      QTreeWidgetItem *subitem2 = new QTreeWidgetItem;
      subitem2->setText(0, "Models");

      item->addChild(subitem1); // child 0 is named DATA
      item->addChild(subitem2); // child 1 is named MODELS
      ui.treeWidget->addTopLevelItem(item);

      for (int i = 0; i < projects->value(pid_)->getMATRIXList().size(); i++) {
        QTreeWidgetItem *subitem3 = new QTreeWidgetItem;
        subitem3->setText(0,
                          projects->value(pid_)
                              ->getMatrix(i)
                              ->getName()); /*set the data name from the file*/
        subitem3->setText(1, QString("Matrix")); // Define the type of the data
        subitem3->setText(
            2, QString::number(tabcount_)); // Define the tab id number in order
                                            // to close a specific table
        subitem3->setText(
            3, QString::number(i)); // Define the matrix position id in order to
                                    // find easly when you need to show data.
        subitem3->setText(
            4, QString::number(pid_)); // pid for get the array with Value
        tabcount_++;
        getProjectItem(pid_)->child(0)->addChild(subitem3);
      }

      for (int i = 0; i < projects->value(pid_)->getARRAYList().size(); i++) {
        QTreeWidgetItem *subitem3 = new QTreeWidgetItem;
        subitem3->setText(0,
                          projects->value(pid_)
                              ->getArray(i)
                              ->getName()); /*set the data name from the file*/
        subitem3->setText(1, QString("Array")); // Define the type of the data
        subitem3->setText(
            2, QString::number(tabcount_)); // Define the tab id number in order
                                            // to close a specific table
        subitem3->setText(
            3, QString::number(i)); // Define the matrix position id in order to
                                    // find easly when you need to show data.
        subitem3->setText(
            4, QString::number(pid_)); // pid for get the array with Value
        tabcount_++;
        getProjectItem(pid_)->child(0)->addChild(subitem3);
      }

      pid_++;
    }

    lastpath = pmanager.getLastPath();
  } else {
    delete projects->value(pid_);
    projects->remove(pid_);
  }
}

int MainWindow::ProjectOpen(QString fproject) {
  if (!fproject.endsWith(".qsm"))
    return -1;

  QProgressDialog pbdialog("Opening...", 0, 0, INT_MAX, this);

  QProgressBar *bar = new QProgressBar(&pbdialog);
  bar->setRange(0, 3);
  bar->setValue(0);
  pbdialog.setBar(bar);

  pbdialog.setMinimumWidth(350);
  pbdialog.setMinimumDuration(1000);
  pbdialog.setWindowModality(Qt::WindowModal);
  QApplication::processEvents();

  QFileInfo info(fproject);
  projects->insert(pid_, new DATA());
  projects->value(pid_)->setProjectID(pid_);
  projects->value(pid_)->setProjectPath(
      info.absoluteFilePath().toUtf8().data());

  QString projectename = info.fileName().replace(".qsm", "");
  updateLog(QString("Importing Project: %1\n").arg(projectename));
  QTreeWidgetItem *item = new QTreeWidgetItem;
  item->setText(0, projectename);
  item->setText(1, QString::number(pid_));
  projects->insert(pid_, new DATA());
  projects->value(pid_)->setProjectID(pid_);
  projects->value(pid_)->setProjectName(projectename);
  QTreeWidgetItem *subitem1 = new QTreeWidgetItem;
  subitem1->setText(0, "Data");
  QTreeWidgetItem *subitem2 = new QTreeWidgetItem;
  subitem2->setText(0, "Models");
  item->addChild(subitem1); // child 0 is named DATA
  item->addChild(subitem2); // child 1 is named MODELS
  ui.treeWidget->addTopLevelItem(item);

  QStringList log;
  if (projects->value(pid_)->isSQLDatabase(
          info.absoluteFilePath().toUtf8().data()) == true) {
    projects->value(pid_)->OpenSQLData(info.absoluteFilePath().toUtf8().data(),
                                       ui.treeWidget, &tabcount_, &mid_, &log);
  } else {
    // OLD Version
    QMessageBox::warning(this, tr("Warning!"),
                         tr("QSM old file version unsupported!\n"),
                         QMessageBox::Close);
  }

  lastpath = info.absolutePath();
  pbdialog.setValue(3);
  QApplication::processEvents();

  for (int i = 0; i < log.size(); i++) {
    updateLog(log[i]);
  }

  pid_++;
  TopMenuEnableDisable();

  pbdialog.setValue(4);
  QApplication::processEvents();
  pbdialog.close();
  return 0;
}

void MainWindow::OpenProject() {
  QString fproject = QString::fromUtf8(
      QFileDialog::getOpenFileName(this, tr("Open File"), lastpath,
                                   tr("QSM Session(*.qsm)"), 0,
                                   QFileDialog::DontUseNativeDialog)
          .toUtf8());
  ProjectOpen(fproject);
  QFileInfo info(fproject);
  if (info.exists() == true) {
    RECENTMODELS m;
    m.name = info.fileName().replace(".qsm", "");
    m.path = info.absoluteFilePath().toUtf8().data();
    if (recents.contains(m) == false) {
      RecentsProjectSwap(m);
      WriteRecentsModelsFile();
      LoadRecentsModelsFile();
    }
  }
}

void MainWindow::OpenRecent1() { ProjectOpen(recents[0].path); }

void MainWindow::OpenRecent2() { ProjectOpen(recents[1].path); }

void MainWindow::OpenRecent3() { ProjectOpen(recents[2].path); }

void MainWindow::OpenRecent4() { ProjectOpen(recents[3].path); }

void MainWindow::SaveAs() {
  if (projects->size() > 0) {
    SaveDialog savedialog(projects);
    savedialog.setPath(lastpath);
    if (savedialog.exec() == QDialog::Accepted) {
      QString fproject = projects->value(savedialog.getProjectID())
                             ->SaveSQLData(savedialog.getPathToSave());
      if (fproject.isEmpty()) {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Unable to save the project!\n"),
                             QMessageBox::Close);
      } else {
        RECENTMODELS m;
        m.name = projects->value(savedialog.getProjectID())->getProjectName();
        m.path = projects->value(savedialog.getProjectID())->getProjectPath();
        if (recents.contains(m) == false) {
          RecentsProjectSwap(m);
          WriteRecentsModelsFile();
          LoadRecentsModelsFile();
        }
      }
    }
  }
}

void MainWindow::ExtractData() {
  ExtractDataDialog edd(projects);
  if (edd.exec() == QDialog::Accepted) {

    int pid = edd.getProjectID();

    updateLog(QString("Extract Matrix Data to: %1\n")
                  .arg(edd.getMatrix()->getName()));

    projects->value(pid)->addMatrix(edd.getMatrix());

    QTreeWidgetItem *subitem = new QTreeWidgetItem;
    subitem->setText(
        0, edd.getMatrix()->getName());     /*set the data name from the file*/
    subitem->setText(1, QString("Matrix")); // Define the type of the data
    subitem->setText(
        2, QString::number(tabcount_)); // Define the tab id number in order to
                                        // close a specific table
    subitem->setText(
        3, QString::number(projects->value(pid)->MatrixCount() -
                           1)); // Define the matrix position id in order to
                                // find easly when you need to show data.
    subitem->setText(4,
                     QString::number(pid)); // pid for get the array with Value
    tabcount_++;

    getProjectItem(pid)->child(0)->addChild(subitem);
  }
}

void MainWindow::MergeData() {
  MergeDataDialog mdd(projects);
  if (mdd.exec() == QDialog::Accepted) {

    int pid = mdd.getProjectID();
    QString newmxname = mdd.getMergedMatrix()->getName();

    updateLog(QString("New Merged Matrix %1\n").arg(newmxname));
    projects->value(pid)->addMatrix(mdd.getMergedMatrix());

    QTreeWidgetItem *subitem = new QTreeWidgetItem;
    subitem->setText(0, newmxname);         /*set the data name from the file*/
    subitem->setText(1, QString("Matrix")); // Define the type of the data
    subitem->setText(
        2, QString::number(tabcount_)); // Define the tab id number in order to
                                        // close a specific table
    subitem->setText(
        3, QString::number(projects->value(pid)->MatrixCount() -
                           1)); // Define the matrix position id in order to
                                // find easly when you need to show data.
    subitem->setText(4,
                     QString::number(pid)); // pid for get the array with Value
    tabcount_++;
    getProjectItem(pid)->child(0)->addChild(subitem);
  }
}

void MainWindow::AdvancedPretreatment() {
  AdvancedPretreatmentDialog apretd(projects);
  if (apretd.exec() == QDialog::Accepted) {

    int pid = apretd.getProjectID();
    QString newmxname = apretd.getMergedMatrix()->getName();

    updateLog(QString("New Merged Matrix %1\n").arg(newmxname));
    projects->value(pid)->addMatrix(apretd.getMergedMatrix());

    QTreeWidgetItem *subitem = new QTreeWidgetItem;
    subitem->setText(0, newmxname);         /*set the data name from the file*/
    subitem->setText(1, QString("Matrix")); // Define the type of the data
    subitem->setText(
        2, QString::number(tabcount_)); // Define the tab id number in order to
                                        // close a specific table
    subitem->setText(
        3, QString::number(projects->value(pid)->MatrixCount() -
                           1)); // Define the matrix position id in order to
                                // find easly when you need to show data.
    subitem->setText(4,
                     QString::number(pid)); // pid for get the array with Value
    tabcount_++;
    getProjectItem(pid)->child(0)->addChild(subitem);
  }
}

void MainWindow::ShowObjVarList(ListSignal lsig) {
#ifdef DEBUG
  qDebug() << "ShowObjVarList(ListSignal lsig) " << lsig.pid << " " << lsig.id
           << " " << lsig.type;
#endif
  if (lsig.pid != -1 && lsig.pid < projects->size()) {
    if (lsig.id != -1 && lsig.type == OBJLABELST &&
        lsig.id < projects->value(lsig.pid)->getObjectLabels().size()) {
      MDIChild *child = createMdiChild();
      //       child->setWindowID();
      QString tabname =
          QString("%1 - %2")
              .arg(projects->value(lsig.pid)->getProjectName())
              .arg(projects->value(lsig.pid)->getObjectLabels()[lsig.id].name);
      child->newListView(
          tabname,
          projects->value(lsig.pid)->getObjectLabels()[lsig.id].objects);
      child->show();
    } else if (lsig.id != -1 && lsig.type == VARLABELST &&
               lsig.id <
                   projects->value(lsig.pid)->getVariableLabels().size()) {
      MDIChild *child = createMdiChild();
      //       child->setWindowID();
      QString tabname =
          QString("%1 - %2")
              .arg(projects->value(lsig.pid)->getProjectName())
              .arg(
                  projects->value(lsig.pid)->getVariableLabels()[lsig.id].name);
      child->newListView(
          tabname,
          projects->value(lsig.pid)->getVariableLabels()[lsig.id].objects);
      child->show();
    } else {
      if (lsig.id != -1 && lsig.type == TABLABELST &&
          lsig.id < projects->value(lsig.pid)->getVariableTabLabels().size()) {
        MDIChild *child = createMdiChild();
        QString tabname = QString("%1 - %2")
                              .arg(projects->value(lsig.pid)->getProjectName())
                              .arg(projects->value(lsig.pid)
                                       ->getVariableTabLabels()[lsig.id]
                                       ->getName());
        child->newTable(tabname,
                        projects->value(lsig.pid)
                            ->getVariableTabLabels()[lsig.id]
                            ->getMatrix(),
                        &projects->value(lsig.pid)->getObjectLabels(),
                        &projects->value(lsig.pid)->getVariableLabels());
        child->getTable()->setPID(lsig.pid);
        // Set Obj Label
        child->getTable()->model()->setObjNames(
            projects->value(lsig.pid)
                ->getVariableTabLabels()[lsig.id]
                ->getObjectsName());
        // Set Header Label
        child->getTable()->model()->setHorizontalHeaderLabels(
            projects->value(lsig.pid)
                ->getVariableTabLabels()[lsig.id]
                ->getFeaturesName());
        connect(child->getTable(), SIGNAL(TabImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));

        child->show();
      } else {
        return;
      }
    }
  } else {
    return;
  }
}

void MainWindow::AddRemoveVarLabel() {
  LabelDialog ldialog(projects, LabelDialog::VARLABELS);
  ldialog.setPath(lastpath);
  connect(&ldialog, SIGNAL(ShowList(ListSignal)),
          SLOT(ShowObjVarList(ListSignal)));
  ldialog.exec();
}

void MainWindow::AddRemoveObjLabel() {
  LabelDialog ldialog(projects, LabelDialog::OBJLABELS);
  ldialog.setPath(lastpath);
  connect(&ldialog, SIGNAL(ShowList(ListSignal)),
          SLOT(ShowObjVarList(ListSignal)));
  ldialog.exec();
}

void MainWindow::SaveAllProjects() {
  int i = 0;
  while (i < projects->size()) {
    if (projects->value(i)->AutoSave() == true) {
      i++;
    } else {
      QMessageBox msgBox;
      msgBox.setText(QString("Project %1 not saved.")
                         .arg(projects->value(i)->getProjectName()));
      msgBox.setInformativeText("Do you want to save your changes?");
      msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard |
                                QMessageBox::Cancel);
      msgBox.setDefaultButton(QMessageBox::Save);
      int ret = msgBox.exec();
      switch (ret) {
      case QMessageBox::Save:
        // Save and move on!
        SaveAs();
        i++;
        break;
      case QMessageBox::Discard:
        // Don't Save was clicked
        // Just move on!
        i++;
        break;
      case QMessageBox::Cancel:
        // Cancel was clicked
        // Dont save and end the close process
        return;
        break;
      default:
        // should never be reached
        i++;
        break;
      }
    }
  }
}

void MainWindow::Quit() {
  SaveAllProjects();
  close();
}

void MainWindow::PlotVariableVSVariable() {
  VariablePlotDialog vvplot(projects, VariablePlotDialog::VariableVSVariable);
  if (vvplot.exec() == QDialog::Accepted) {

    VariablePlot vplot(projects);

    vplot.setPlotType(vvplot.getVarType());
    vplot.setPID(vvplot.getProjectID());
    vplot.setXHash(vvplot.getDataHash());
    vplot.setObjects(vvplot.getObjects());
    vplot.setVarID1(vvplot.getVariableID1());
    vplot.setVarID2(vvplot.getVariableID2());

    ScatterPlot *plot2D = vplot.VariableVSVariable();
    connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
            SLOT(UpdateImageWindow(ImageSignal)));

    MDIChild *graphchild = createMdiChild();
    graphchild->setWidget(plot2D);
    graphchild->setWindowID(vvplot.getProjectID());
    graphchild->resize(510, 530);
    graphchild->show();
  }
}

void MainWindow::PlotVariableDistribution() {
  VariablePlotDialog vdist(projects, VariablePlotDialog::VariableDistribution);
  if (vdist.exec() == QDialog::Accepted) {

    VariablePlot vplot(projects);
    vplot.setPlotType(vdist.getVarType());
    vplot.setPID(vdist.getProjectID());
    vplot.setXHash(vdist.getDataHash());
    vplot.setObjects(vdist.getObjects());
    vplot.setObjLabels(vdist.getObjLabelSelected());
    vplot.setVarID1(vdist.getVariableID1());

    MDIChild *graphchild = createMdiChild();
    graphchild->setWidget(vplot.VariableDistribution());
    graphchild->setWindowID(vdist.getProjectID());
    graphchild->resize(510, 530);
    graphchild->show();
  }
}

/*
 * pid - pid  (2)
 *    |
 *    | Data   // child 0
 *    |   |
 *    |   Name - Type - Tab Count - Data Position - pid  (5)
 *    |
 *    | Models  // child 1
 *        |
 *        Name - Tab Count - pid - x ID - y ID - x Scaling type - y Scaling type
 * - number of components - Model Type - Data Position  (10)
 *          |
 *         ModelPrediction Name - Tab Count - pid - Model ID - xdata id - ydata
 * id - Data Position - Data Type (PCA Prediction PLS Prediction,, ...) (8)
 *
 * Type could be "Matrix" or "Array"
 * Tab count is an id used to know what tables are opened and to close the
 * deleted table Data Position is used to retrieve the Data Position inside the
 * QList<MATRIX>,  QList<Array> QList<PCAModel> etc... Model Type can be PCA
 * Model, PLS Model, LDA Model, MLR Model
 */

void MainWindow::PCA2DScorePlot() {
  if (ProjectsHavePCA() == true) {
    ProjectTree pjtree;
    GetPCAProjects(&pjtree);
    DebugProjectTree(pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PCAPlot pcaplot(projects);
      pcaplot.setPID(dp.getProjectID());
      pcaplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      pcaplot.ScorePlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PCA2DLoadingsMVANDPlot() {
  if (ProjectsHavePCA() == true) {
    ClassPlotDialog classplotdialog(projects, ClassPlotDialog::PCALOADINGS);
    if (classplotdialog.exec() == QDialog::Accepted &&
        classplotdialog.Plot() == true) {
      PCAPlot pcaplot(projects);
      pcaplot.setPID(classplotdialog.selectedProject());
      pcaplot.setMID(classplotdialog.getModelID());
      pcaplot.setGroups(classplotdialog.getClass());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      pcaplot.LoadingsMVANormDistrib(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(getModelTableID(classplotdialog.selectedProject(),
                                              classplotdialog.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PCA2DLoadingsPlot() {
  if (ProjectsHavePCA() == true) {
    ProjectTree pjtree;
    GetPCAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PCAPlot pcaplot(projects);
      pcaplot.setPID(dp.getProjectID());
      pcaplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      pcaplot.LoadingsPlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
      connect(plot2D, SIGNAL(ScatterPlotVVPlotSignal(vvplotSignal)),
              SLOT(PlotVariableVSVariableBis(vvplotSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PCA2DExpVarPlot() {
  if (ProjectsHavePCA() == true) {
    ProjectTree pjtree;
    GetPCAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PCAPlot pcaplot(projects);
      pcaplot.setPID(dp.getProjectID());
      pcaplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      SimpleLine2DPlot *plot2D;
      pcaplot.ExpVarPlot(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PCATsqContributionPlot() {
  if (ProjectsHavePCA() == true) {
    ProjectTree pjtree;
    GetPCAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    if (dp.exec() == QDialog::Accepted) {
      PCAPlot pcaplot(projects);
      pcaplot.setPID(dp.getProjectID());
      pcaplot.setMID(dp.getModelID());
      pcaplot.setNLatentVariables(dp.getNLV());
      BarPlot *bar_plots = nullptr;
      pcaplot.TsqContributionPlot(&bar_plots);
      if (bar_plots != nullptr) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(bar_plots);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Problem with Barplot and T squared contribution plot!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PCA2DScorePlotPrediction() {
  if (ProjectsHavePCA() == true) {
    ProjectTree pjtree;
    GetPCAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PCAPlot pcaplot(projects);
      pcaplot.setPID(dp.getProjectID());
      pcaplot.setMID(dp.getModelID());
      pcaplot.setPREDID(dp.getPredictionID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      pcaplot.ScorePlotPrediction2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::CPCA2DSuperScorePlot() {
  if (ProjectsHaveCPCA() == true) {
    ProjectTree pjtree;
    GetCPCAProjects(&pjtree);
    DebugProjectTree(pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      CPCAPlot cpcaplot(projects);
      cpcaplot.setPID(dp.getProjectID());
      cpcaplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      cpcaplot.SuperScorePlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No CPCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::CPCA2DSuperWeightsPlot() {
  if (ProjectsHaveCPCA() == true) {
    ProjectTree pjtree;
    GetCPCAProjects(&pjtree);
    DebugProjectTree(pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      CPCAPlot cpcaplot(projects);
      cpcaplot.setPID(dp.getProjectID());
      cpcaplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      cpcaplot.SuperWeightsPlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No CPCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::CPCA2DBlockScoresPlot() {
  if (ProjectsHaveCPCA() == true) {
    ProjectTree pjtree;
    GetCPCAProjects(&pjtree);
    DebugProjectTree(pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      CPCAPlot cpcaplot(projects);
      cpcaplot.setPID(dp.getProjectID());
      cpcaplot.setMID(dp.getModelID());
      QList<ScatterPlot *> plots = cpcaplot.BlockScoresPlot2D();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plots[i], SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No CPCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::CPCA2DBlockLoadingsPlot() {
  if (ProjectsHaveCPCA() == true) {
    ProjectTree pjtree;
    GetCPCAProjects(&pjtree);
    DebugProjectTree(pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      CPCAPlot cpcaplot(projects);
      cpcaplot.setPID(dp.getProjectID());
      cpcaplot.setMID(dp.getModelID());
      QList<ScatterPlot *> plots = cpcaplot.BlockLoadingsPlot2D();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plots[i], SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No CPCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::CPCA2DExpVarPlot() {
  if (ProjectsHaveCPCA() == true) {
    ProjectTree pjtree;
    GetCPCAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      CPCAPlot cpcaplot(projects);
      cpcaplot.setPID(dp.getProjectID());
      cpcaplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      SimpleLine2DPlot *plot2D;
      cpcaplot.ExpVarPlot(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::CPCA2DSuperScoresPlotPrediction() {
  if (ProjectsHaveCPCAPrediction() == true) {
    ProjectTree pjtree;
    GetCPCAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      CPCAPlot cpcaplot(projects);
      cpcaplot.setPID(dp.getProjectID());
      cpcaplot.setMID(dp.getModelID());
      cpcaplot.setPREDID(dp.getPredictionID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      cpcaplot.SuperScorePlotPrediction2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No CPCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::CPCA2DBlockScoresPlotPrediction() {
  if (ProjectsHaveCPCAPrediction() == true) {
    ProjectTree pjtree;
    GetCPCAProjects(&pjtree);
    DebugProjectTree(pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      CPCAPlot cpcaplot(projects);
      cpcaplot.setPID(dp.getProjectID());
      cpcaplot.setMID(dp.getModelID());
      cpcaplot.setPREDID(dp.getPredictionID());
      QList<ScatterPlot *> plots = cpcaplot.BlockScorePlotPrediction2D();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plots[i], SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No CPCA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLS2DPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      plsplot.TU_Plot(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLS2DTTScorePlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      plsplot.T_ScorePlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLS2DPPLoadingsPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      plsplot.P_LoadingsPlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
      connect(plot2D, SIGNAL(ScatterPlotVVPlotSignal(vvplotSignal)),
              SLOT(PlotVariableVSVariableBis(vvplotSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLS2DWWWeightsPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      plsplot.WeightsPlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
      connect(plot2D, SIGNAL(ScatterPlotVVPlotSignal(vvplotSignal)),
              SLOT(PlotVariableVSVariableBis(vvplotSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLS2DUUScorePlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      plsplot.U_ScorePlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLS2DQQLoadingsPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      plsplot.Q_LoadingsPlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
      connect(plot2D, SIGNAL(ScatterPlotVVPlotSignal(vvplotSignal)),
              SLOT(PlotVariableVSVariableBis(vvplotSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLS2DPQLoadingsPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      plsplot.PQ_LoadingsPlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
      connect(plot2D, SIGNAL(ScatterPlotVVPlotSignal(vvplotSignal)),
              SLOT(PlotVariableVSVariableBis(vvplotSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLS2DTTScorePlotPrediction() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setPREDID(dp.getPredictionID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      plsplot.T_ScorePlotPrediction2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotBetaCoefficients() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setNLatentVariables(dp.getNLV());
      BarPlot *betas_barplot = 0;
      plsplot.BetaCoefficients(&betas_barplot);
      MDIChild *graphchild = createMdiChild();
      graphchild->setWidget(betas_barplot);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotBetaCoeffDWPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      SimpleLine2DPlot *dw_betas_plot = 0;
      plsplot.BetaCoefficientsDurbinWatson(&dw_betas_plot);
      MDIChild *graphchild = createMdiChild();
      graphchild->setWidget(dw_betas_plot);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSRecalcVSExpPlotPrediction() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setPREDID(dp.getPredictionID());
      plsplot.setNLatentVariables(dp.getNLV());
      ScatterPlot *plot2D = 0;
      plsplot.RecalcVSExperimentalAndPrediction(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPredictedVSExpAndPredictionPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setPREDID(dp.getPredictionID());
      plsplot.setNLatentVariables(dp.getNLV());
      ScatterPlot *plot2D = 0;
      plsplot.PredictedVSExperimentalAndPrediction(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSRecalcVSExpPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setNLatentVariables(dp.getNLV());

      if (projects->value(dp.getProjectID())
              ->getPLSModel(dp.getModelID())
              ->getAlgorithm() == PLS_) {
        ScatterPlot *plot2D = 0;
        plsplot.RecalcVSExperimental(&plot2D);
        if (plot2D != 0) {
          MDIChild *graphchild = createMdiChild();
          graphchild->setWidget(plot2D);
          graphchild->setWindowID(
              getModelTableID(dp.getProjectID(), dp.getModelID()));
          graphchild->resize(510, 530);
          graphchild->show();
          connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                  SLOT(UpdateImageWindow(ImageSignal)));
        } else {
          QMessageBox::warning(this, tr("Warning!"),
                               tr("Original Data Model not found!\n"),
                               QMessageBox::Close);
        }
      } else {
        QList<QStringList> cellnames;
        QList<QList<QPixmap>> images;
        QList<QList<QColor>> colors;
        plsplot.ClassRecalcVSExperimental(&cellnames, &images, &colors);
        for (int i = 0; i < cellnames.size(); i++) {
          MDIChild *child = createMdiChild();
          QString tabname =
              QString("PLS %1 - Class Recalculated vs Experimental")
                  .arg(projects->value(dp.getProjectID())
                           ->getPLSModel(dp.getModelID())
                           ->getName());
          child->newTable(tabname, cellnames[i], images[i], colors[i]);
          child->show();
          child->getTable()->setPID(dp.getProjectID());
        }
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSRecalcResidualsVSExpPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setNLatentVariables(dp.getNLV());
      ScatterPlot *plot2D = 0;
      plsplot.RecalcResidualsVSExperimental(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPredVSExpPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setNLatentVariables(dp.getNLV());

      if (projects->value(dp.getProjectID())
              ->getPLSModel(dp.getModelID())
              ->getAlgorithm() == PLS_) {
        ScatterPlot *plot2D = 0;
        plsplot.PredictedVSExperimental(&plot2D);
        if (plot2D != 0) {
          MDIChild *graphchild = createMdiChild();
          graphchild->setWidget(plot2D);
          graphchild->setWindowID(
              getModelTableID(dp.getProjectID(), dp.getModelID()));
          graphchild->resize(510, 530);
          graphchild->show();
          connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                  SLOT(UpdateImageWindow(ImageSignal)));
        } else {
          QMessageBox::warning(this, tr("Warning!"),
                               tr("Original Data Model not found!\n"),
                               QMessageBox::Close);
        }
      } else {
        QList<QStringList> cellnames;
        QList<QList<QPixmap>> images;
        QList<QList<QColor>> colors;
        plsplot.ClassPredictedVSExperimental(&cellnames, &images, &colors);
        for (int i = 0; i < cellnames.size(); i++) {
          MDIChild *child = createMdiChild();
          QString tabname = QString("PLS %1 - Class Predicted vs Experimental")
                                .arg(projects->value(dp.getProjectID())
                                         ->getPLSModel(dp.getModelID())
                                         ->getName());
          child->newTable(tabname, cellnames[i], images[i], colors[i]);
          child->show();
          child->getTable()->setPID(dp.getProjectID());
        }
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPredResidualsVSExpPlot() {
  if (ProjectsHavePLS() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setNLatentVariables(dp.getNLV());
      ScatterPlot *plot2D = 0;
      plsplot.PredictedResidualsVSExperimental(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotR2Q2() {
  if (ProjectsHavePLSValidated() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      QList<SimpleLine2DPlot *> plots = plsplot.R2Q2();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No PLS model and/or validation found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotRMSE() {
  if (ProjectsHavePLSValidated() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      QList<SimpleLine2DPlot *> plots = plsplot.RMSE();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No PLS model and/or validation found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotROCAucs() {
  if (ProjectsHavePLSValidated() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      QList<SimpleLine2DPlot *> plots = plsplot.ROCAUCs();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No PLS model and/or validation found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotROCCurves() {
  if (ProjectsHavePLSValidated() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      QList<SimpleLine2DPlot *> plots = plsplot.ROCCurves();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No PLS model and/or validation found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotPRAucs() {
  if (ProjectsHavePLSValidated() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      QList<SimpleLine2DPlot *> plots =
          plsplot.PrecisionRecallAveragePrecision();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No PLS model and/or validation found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotPRCurves() {
  if (ProjectsHavePLSValidated() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());

      QList<SimpleLine2DPlot *> plots = plsplot.PrecisionRecallCurves();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No PLS model and/or validation found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotR2R2Predicted() {
  if (ProjectsHavePLSPrediction() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setPREDID(dp.getPredictionID());
      QList<SimpleLine2DPlot *> plots = plsplot.R2R2Prediction();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No PLS model and/or validation found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotRMSEPredicted() {
  if (ProjectsHavePLSPrediction() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      plsplot.setPREDID(dp.getPredictionID());
      QList<SimpleLine2DPlot *> plots = plsplot.RMSEPrediction();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No PLS model and/or validation found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::PLSPlotYScrambling() {
  if (ProjectsHavePLSYScrambling() == true) {
    ProjectTree pjtree;
    GetPLSProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      PLSPlot plsplot(projects);
      plsplot.setPID(dp.getProjectID());
      plsplot.setMID(dp.getModelID());
      QList<ScatterPlot *> plots = plsplot.YScramblingPlot();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(
        this, tr("Warning!"),
        tr("No PLS Models and Y Scrambling Validation Model Found!\n"),
        QMessageBox::Close);
  }
}

void MainWindow::MLRRecalcVSExpPlot() {
  if (ProjectsHaveMLR() == true) {
    ProjectTree pjtree;
    GetMLRProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      MLRPlot mlrplot(projects);
      mlrplot.setPID(dp.getProjectID());
      mlrplot.setMID(dp.getModelID());
      ScatterPlot *plot2D = 0;
      mlrplot.RecalcVSExperimental(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No PLS Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::MLRRecalcResidualsVSExpPlot() {
  if (ProjectsHaveMLR() == true) {
    ProjectTree pjtree;
    GetMLRProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      MLRPlot mlrplot(projects);
      mlrplot.setPID(dp.getProjectID());
      mlrplot.setMID(dp.getModelID());
      ScatterPlot *plot2D = 0;
      mlrplot.RecalcResidualsVSExperimental(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No MLR Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::MLRBetaCoefficients() {
  if (ProjectsHaveMLR() == true) {
    ProjectTree pjtree;
    GetMLRProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      MLRPlot mlrplot(projects);
      mlrplot.setPID(dp.getProjectID());
      mlrplot.setMID(dp.getModelID());
      QList<BarPlot *> barplots = mlrplot.BetaCoefficients();
      for (int i = 0; i < barplots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(barplots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No MLR Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::MLRPredVSExpPlot() {
  if (ProjectsHaveMLR() == true) {
    ProjectTree pjtree;
    GetMLRProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      MLRPlot mlrplot(projects);
      mlrplot.setPID(dp.getProjectID());
      mlrplot.setMID(dp.getModelID());
      ScatterPlot *plot2D = 0;
      mlrplot.PredictedVSExperimental(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No MLR Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::MLRPredResidualsVSExpPlot() {
  if (ProjectsHaveMLR() == true) {
    ProjectTree pjtree;
    GetMLRProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      MLRPlot mlrplot(projects);
      mlrplot.setPID(dp.getProjectID());
      mlrplot.setMID(dp.getModelID());
      ScatterPlot *plot2D = 0;
      mlrplot.PredictedResidualsVSExperimental(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No MLR Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::MLRRecalcVSExpAndPredictionPlot() {
  if (ProjectsHaveMLR() == true && ProjectsHaveMLRPrediction() == true) {
    ProjectTree pjtree;
    GetMLRProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      MLRPlot mlrplot(projects);
      mlrplot.setPID(dp.getProjectID());
      mlrplot.setMID(dp.getModelID());
      mlrplot.setPREDID(dp.getPredictionID());
      ScatterPlot *plot2D = 0;
      mlrplot.RecalcVSExperimentalAndPrediction(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No MLR Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::MLRPredictedVSExpAndPredictionPlot() {
  if (ProjectsHaveMLR() == true && ProjectsHaveMLRPrediction() == true) {
    ProjectTree pjtree;
    GetMLRProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      MLRPlot mlrplot(projects);
      mlrplot.setPID(dp.getProjectID());
      mlrplot.setMID(dp.getModelID());
      mlrplot.setPREDID(dp.getPredictionID());
      ScatterPlot *plot2D = 0;
      mlrplot.PredictedVSExperimentalAndPrediction(&plot2D);
      if (plot2D != 0) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plot2D);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
        connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
                SLOT(UpdateImageWindow(ImageSignal)));
      } else {
        QMessageBox::warning(this, tr("Warning!"),
                             tr("Original Data Model not found!\n"),
                             QMessageBox::Close);
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No MLR Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::MLRPlotYScrambling() {
  if (ProjectsHaveMLRYScrambling() == true) {
    ProjectTree pjtree;
    GetMLRProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      MLRPlot mlrplot(projects);
      mlrplot.setPID(dp.getProjectID());
      mlrplot.setMID(dp.getModelID());
      QList<ScatterPlot *> plots = mlrplot.YScramblingPlot();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(
        this, tr("Warning!"),
        tr("No MLR Models and Y Scrambling Validation Model Found!\n"),
        QMessageBox::Close);
  }
}

void MainWindow::LDAFeaturePlot2D() {
  if (ProjectsHaveLDA() == true) {
    ProjectTree pjtree;
    GetLDAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      LDAPlot ldaplot(projects);
      ldaplot.setPID(dp.getProjectID());
      ldaplot.setMID(dp.getModelID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      ldaplot.FeaturePlot2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No LDA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::LDAProbabilityDistribution() {
  if (ProjectsHaveLDA() == true) {
    ProjectTree pjtree;
    GetLDAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      LDAPlot ldaplot(projects);
      ldaplot.setPID(dp.getProjectID());
      ldaplot.setMID(dp.getModelID());
      ldaplot.setPREDID(dp.getPredictionID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      ldaplot.ProbabilityDistribution(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No LDA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::LDAROCPlot() {
  if (ProjectsHaveLDAValidated() == true) {
    ProjectTree pjtree;
    GetLDAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      LDAPlot ldaplot(projects);
      ldaplot.setPID(dp.getProjectID());
      ldaplot.setMID(dp.getModelID());
      ldaplot.setPREDID(dp.getPredictionID());
      QList<SimpleLine2DPlot *> plots = ldaplot.ROCPlot();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No LDA Models Validated Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::LDAPRPlot() {
  if (ProjectsHaveLDAValidated() == true) {
    ProjectTree pjtree;
    GetLDAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::TwoColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      LDAPlot ldaplot(projects);
      ldaplot.setPID(dp.getProjectID());
      ldaplot.setMID(dp.getModelID());
      ldaplot.setPREDID(dp.getPredictionID());
      QList<SimpleLine2DPlot *> plots = ldaplot.PRPlot();
      for (int i = 0; i < plots.size(); i++) {
        MDIChild *graphchild = createMdiChild();
        graphchild->setWidget(plots[i]);
        graphchild->setWindowID(
            getModelTableID(dp.getProjectID(), dp.getModelID()));
        graphchild->resize(510, 530);
        graphchild->show();
      }
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"),
                         tr("No LDA Models Validated Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::LDAFeaturePlotAndPrediction2D() {
  if (ProjectsHaveLDA() == true && ProjectsHaveLDAPrediction() == true) {
    ProjectTree pjtree;
    GetLDAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      LDAPlot ldaplot(projects);
      ldaplot.setPID(dp.getProjectID());
      ldaplot.setMID(dp.getModelID());
      ldaplot.setPREDID(dp.getPredictionID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      ldaplot.FeaturePlotAndPrediction2D(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No LDA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::LDAProbabilityDistributionWithPredictions() {
  if (ProjectsHaveLDA() == true && ProjectsHaveLDAPrediction() == true) {
    ProjectTree pjtree;
    GetLDAProjects(&pjtree);
    DialogPlots dp(pjtree, DialogPlots::ThreeColumns);
    dp.hideOptions(true);
    if (dp.exec() == QDialog::Accepted) {
      LDAPlot ldaplot(projects);
      ldaplot.setPID(dp.getProjectID());
      ldaplot.setMID(dp.getModelID());
      ldaplot.setPREDID(dp.getPredictionID());
      MDIChild *graphchild = createMdiChild();
      ScatterPlot *plot2D;
      ldaplot.ProbabilityDistributionWithPredictions(&plot2D);
      graphchild->setWidget(plot2D);
      graphchild->setWindowID(
          getModelTableID(dp.getProjectID(), dp.getModelID()));
      graphchild->resize(510, 530);
      graphchild->show();
      connect(plot2D, SIGNAL(ScatterPlotImageSignalChanged(ImageSignal)),
              SLOT(UpdateImageWindow(ImageSignal)));
    }
  } else {
    QMessageBox::warning(this, tr("Warning!"), tr("No LDA Models Found!\n"),
                         QMessageBox::Close);
  }
}

void MainWindow::DoCPCAPrediction() {
  if (!projects->isEmpty()) {
    int ncpca = 0;
    for (int i = 0; i < projects->values().size(); i++) {
      if (projects->values()[i]->CPCACount() > 0)
        ncpca++;
    }

    if (ncpca > 0) {
      DoPredictionDialog p(projects, CPCA_);
      if (p.exec() == QDialog::Accepted && p.compute() == true) {

        int pid = p.getselectedProject();
        int mid = p.getselectedModel();
        int did = p.getselectedData();

        StartRun();
        CalculationMenuDisable(pid);
        TopMenuEnableDisable();

        QString modelname = p.getPredictionName();
        QStringList objsel = p.getObjectSelected();
        LABELS varsel = projects->value(pid)->getCPCAModel(mid)->getVarName();

        tensor *x;
        initTensor(&x);

        bool tok = PrepareTensor(projects->value(pid)->getMatrix(did), objsel,
                                 varsel, x);

        if (x->order == (size_t)varsel.size() && tok == true) {
          QString str =
              "--------------------\n Computing CPCA Prediction for: ";
          str.append(QString("%1").arg(
              projects->value(p.getselectedProject())->getProjectName()));
          updateLog(str);

          projects->value(pid)->getCPCAModel(mid)->addCPCAPrediction();
          projects->value(pid)
              ->getCPCAModel(mid)
              ->getLastCPCAPrediction()
              ->setName("CPCA Prediction - " + modelname);
          projects->value(pid)
              ->getCPCAModel(mid)
              ->getLastCPCAPrediction()
              ->setPredID(projects->value(pid)
                              ->getCPCAModel(mid)
                              ->CPCAPredictionCount() -
                          1);
          projects->value(pid)
              ->getCPCAModel(mid)
              ->getLastCPCAPrediction()
              ->setDID(did);
          projects->value(pid)
              ->getCPCAModel(mid)
              ->getLastCPCAPrediction()
              ->setDataHash(projects->value(pid)->getMatrix(did)->getHash());
          projects->value(pid)
              ->getCPCAModel(mid)
              ->getLastCPCAPrediction()
              ->setObjName(objsel);

          RUN obj;

          obj.setXTensor(x);
          obj.setCPCAModel(projects->value(pid)->getCPCAModel(mid));

          QFuture<void> future = obj.RunCPCAPrediction();

          while (!future.isFinished()) {
            QApplication::processEvents();
          }

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, projects->value(pid)
                                  ->getCPCAModel(mid)
                                  ->getLastCPCAPrediction()
                                  ->getName());
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, QString::number(mid));
          subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(5, "");
          subitem->setText(6, QString::number(projects->value(pid)
                                                  ->getCPCAModel(mid)
                                                  ->getLastCPCAPrediction()
                                                  ->getPredID()));
          subitem->setText(7, QString("CPCA Prediction"));

          tabcount_++;
          getModelItem(pid, mid)->addChild(subitem);

        } else {
          QMessageBox::critical(
              this, tr("CPCA Prediction Error"),
              tr("Unable to compute CPCA Prediction.\n"
                 "The number of blocsk differ. Please check your data."),
              QMessageBox::Ok);
          updateLog(
              QString("Error!! Unable to compute CPCA Prediction. The number "
                      "of blocks differ. Please check your data.\n"));
        }
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelTensor(&x);
        projects->value(pid)->AutoSave();
      }
    }
  }
}

void MainWindow::DoCPCA() {
  if (!projects->isEmpty()) {
    ModelDialogWizard docpca(projects, CPCA_);

    if (docpca.exec() == QDialog::Accepted && docpca.compute() == true) {
      StartRun();

      int pid = docpca.getselectedProject();
      int did = docpca.getselectedData();
      int xscaling = docpca.getXScalingType();
      int pc = docpca.getNumberOfComponent();
      QString modelname = "CPCA - " + docpca.getModelName();
      QStringList objsel = docpca.getObjectSelected();
      LABELS varsel = docpca.getBlockXVarSelected();

      if (did != -1 && pid != -1) {
        CalculationMenuDisable(pid);

        QString str = "--------------------\n Computing CPCA for: ";
        str.append(QString("%1").arg(projects->value(pid)->getProjectName()));

        updateLog(str);

        projects->value(pid)->addCPCAModel();

        projects->value(pid)->getLastCPCAModel()->setDID(did);
        projects->value(pid)->getLastCPCAModel()->setDataHash(
            projects->value(pid)->getMatrix(did)->getHash());
        projects->value(pid)->getLastCPCAModel()->setXScaling(xscaling);
        projects->value(pid)->getLastCPCAModel()->setNPC(pc);
        projects->value(pid)->getLastCPCAModel()->setModelID(mid_);
        projects->value(pid)->getLastCPCAModel()->setName(modelname);
        projects->value(pid)->getLastCPCAModel()->setObjName(objsel);
        projects->value(pid)->getLastCPCAModel()->setVarName(varsel);

        tensor *x;
        initTensor(&x);

        PrepareTensor(projects->value(pid)->getMatrix(did), objsel, varsel, x);

        RUN obj;
        obj.setXTensor(x);
        obj.setCPCAModel(projects->value(pid)->getLastCPCAModel());
        obj.setXScalingType(xscaling);
        obj.setNumberPC(pc);

        QFuture<void> future = obj.RunCPCA();
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
          WaitRun();
#ifdef DEBUG
          qDebug() << "Print CPCA";
          PrintCPCA(projects->value(pid)->getCPCAModel(mid_)->Model());
#endif

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, modelname);
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(4, QString("-"));
          subitem->setText(5, QString::number(xscaling));
          subitem->setText(6, QString("-"));
          subitem->setText(7, QString::number(pc));
          subitem->setText(8, QString("CPCA Model"));
          subitem->setText(9, QString::number(mid_));

          getProjectItem(pid)->child(1)->addChild(subitem);

          tabcount_++;
          mid_++;
        } else {
          int removeid = projects->value(pid)->CPCACount() - 1;
          projects->value(pid)->delCPCAModelAt(removeid);
        }

        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelTensor(&x);
        projects->value(pid)->AutoSave();
      }
    }
  }
}

void MainWindow::DoPCAPrediction() {
  if (!projects->isEmpty()) {
    int npca = 0;
    for (int i = 0; i < projects->values().size(); i++) {
      if (projects->values()[i]->PCACount() > 0)
        npca++;
    }

    if (npca > 0) {
      DoPredictionDialog p(projects, PCA_);
      if (p.exec() == QDialog::Accepted && p.compute() == true) {

        int pid = p.getselectedProject();
        int mid = p.getselectedModel();
        int did = p.getselectedData();

        StartRun();
        CalculationMenuDisable(pid);
        TopMenuEnableDisable();

        QString modelname = p.getPredictionName();
        QStringList objsel = p.getObjectSelected();
        QStringList varsel =
            projects->value(pid)->getPCAModel(mid)->getVarName();

        matrix *x;

        NewMatrix(&x, objsel.size(), varsel.size());
        bool mxok = PrepareMatrix(projects->value(pid)->getMatrix(did), objsel,
                                  varsel, x);

        if (x->col == (size_t)varsel.size() && mxok == true) {
          QString str = "--------------------\n Computing PCA Prediction for: ";
          str.append(QString("%1").arg(
              projects->value(p.getselectedProject())->getProjectName()));
          updateLog(str);

          projects->value(pid)->getPCAModel(mid)->addPCAPrediction();

          projects->value(pid)
              ->getPCAModel(mid)
              ->getLastPCAPrediction()
              ->setName("PCA Prediction - " + modelname);
          projects->value(pid)
              ->getPCAModel(mid)
              ->getLastPCAPrediction()
              ->setPredID(
                  projects->value(pid)->getPCAModel(mid)->PCAPredictionCount() -
                  1);
          projects->value(pid)
              ->getPCAModel(mid)
              ->getLastPCAPrediction()
              ->setDID(did);
          projects->value(pid)
              ->getPCAModel(mid)
              ->getLastPCAPrediction()
              ->setDataHash(projects->value(pid)->getMatrix(did)->getHash());
          projects->value(pid)
              ->getPCAModel(mid)
              ->getLastPCAPrediction()
              ->setObjName(objsel);

          RUN obj;

          obj.setXMatrix(x);
          obj.setPCAModel(projects->value(pid)->getPCAModel(mid));

          QFuture<void> future = obj.RunPCAPrediction();

          while (!future.isFinished()) {
            QApplication::processEvents();
          }

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, projects->value(pid)
                                  ->getPCAModel(mid)
                                  ->getLastPCAPrediction()
                                  ->getName());
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, QString::number(mid));
          subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(5, "");
          subitem->setText(6, QString::number(projects->value(pid)
                                                  ->getPCAModel(mid)
                                                  ->getLastPCAPrediction()
                                                  ->getPredID()));
          subitem->setText(7, QString("PCA Prediction"));

          tabcount_++;
          getModelItem(pid, mid)->addChild(subitem);

        } else {
          QMessageBox::critical(
              this, tr("PCA Prediction Error"),
              tr("Unable to compute PCA Prediction.\n"
                 "The number of variables differ. Please check your data."),
              QMessageBox::Ok);
          updateLog(
              QString("Error!! Unable to compute PCA Prediction. The number of "
                      "variables differ. Please check your data.\n"));
        }
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelMatrix(&x);
        projects->value(pid)->AutoSave();
      }
    }
  }
}

void MainWindow::DoPCA() {
  if (!projects->isEmpty()) {

    // ModelDialog dopca(projects, PCA_);
    ModelDialogWizard dopca(projects, PCA_);
    if (dopca.exec() == QDialog::Accepted && dopca.compute() == true) {
      StartRun();

      int pid = dopca.getselectedProject();
      int did = dopca.getselectedData();
      int xscaling = dopca.getXScalingType();
      int pc = dopca.getNumberOfComponent();
      QString modelname = "PCA - " + dopca.getModelName();
      QStringList objsel = dopca.getObjectSelected();
      QStringList varsel = dopca.getXVarSelected();

      if (did != -1 && pid != -1) {
        CalculationMenuDisable(pid);

        QString str = "--------------------\n Computing PCA for: ";
        str.append(QString("%1").arg(projects->value(pid)->getProjectName()));

        updateLog(str);

        projects->value(pid)->addPCAModel();

        projects->value(pid)->getLastPCAModel()->setDID(did);
        projects->value(pid)->getLastPCAModel()->setDataHash(
            projects->value(pid)->getMatrix(did)->getHash());
        projects->value(pid)->getLastPCAModel()->setXScaling(xscaling);
        projects->value(pid)->getLastPCAModel()->setNPC(pc);
        projects->value(pid)->getLastPCAModel()->setModelID(mid_);
        projects->value(pid)->getLastPCAModel()->setName(modelname);
        projects->value(pid)->getLastPCAModel()->setObjName(objsel);
        projects->value(pid)->getLastPCAModel()->setVarName(varsel);

        matrix *x;
        initMatrix(&x);

        if (objsel.size() ==
                projects->value(pid)->getMatrix(did)->getObjName().size() &&
            varsel.size() ==
                projects->value(pid)->getMatrix(did)->getVarName().size()) {
          MatrixCopy(projects->value(pid)->getMatrix(did)->Matrix(), &x);
        } else {
          PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, varsel,
                        x);
        }

        RUN obj;
        obj.setXMatrix(x);
        obj.setPCAModel(projects->value(pid)->getLastPCAModel());
        obj.setXScalingType(xscaling);
        obj.setNumberPC(pc);

        QFuture<void> future = obj.RunPCA();
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
          WaitRun();
#ifdef DEBUG
          qDebug() << "Print PCA";
          PrintPCA(projects->value(pid)->getPCAModel(mid_)->Model());
#endif

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, modelname);
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(4, QString("-"));
          subitem->setText(5, QString::number(xscaling));
          subitem->setText(6, QString("-"));
          subitem->setText(7, QString::number(pc));
          subitem->setText(8, QString("PCA Model"));
          subitem->setText(9, QString::number(mid_));

          getProjectItem(pid)->child(1)->addChild(subitem);

          tabcount_++;
          mid_++;
        } else {
          int removeid = projects->value(pid)->PCACount() - 1;
          projects->value(pid)->delPCAModelAt(removeid);
        }

        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelMatrix(&x);
        projects->value(pid)->AutoSave();
      }
    }
  }
}

void MainWindow::DoPLSPrediction() {
  if (!projects->isEmpty()) {
    int npls = 0;
    for (int i = 0; i < projects->values().size(); i++) {
      if (projects->values()[i]->PLSCount() > 0)
        npls++;
    }

    if (npls > 0) {
      DoPredictionDialog p(projects, PLS_);
      if (p.exec() == QDialog::Accepted && p.compute() == true) {

        int pid = p.getselectedProject();
        int mid = p.getselectedModel();
        int did = p.getselectedData();

        StartRun();
        CalculationMenuDisable(pid);
        TopMenuEnableDisable();

        QStringList objsel = p.getObjectSelected();
        QStringList ysel = p.getYVariableSelected();
        QString modelname = p.getPredictionName();

        QStringList xvarsel =
            projects->value(pid)->getPLSModel(mid)->getXVarName();
        matrix *x, *y;
        initMatrix(&x);
        initMatrix(&y);

        bool mxok = PrepareMatrix(projects->value(pid)->getMatrix(did), objsel,
                                  xvarsel, ysel, x, y);

        if (x->col == (size_t)xvarsel.size() && mxok == true) {
          QString str = "--------------------\n Computing PLS Prediction for: ";
          str.append(QString("%1").arg(projects->value(pid)->getProjectName()));
          updateLog(str);

          projects->value(pid)->getPLSModel(mid)->addPLSPrediction();

          projects->value(pid)
              ->getPLSModel(mid)
              ->getLastPLSPrediction()
              ->setName("PLS Prediction - " + modelname);
          projects->value(pid)
              ->getPLSModel(mid)
              ->getLastPLSPrediction()
              ->setPredID(
                  projects->value(pid)->getPLSModel(mid)->PLSPredictionCount() -
                  1);
          projects->value(pid)
              ->getPLSModel(mid)
              ->getLastPLSPrediction()
              ->setDID(did);
          projects->value(pid)
              ->getPLSModel(mid)
              ->getLastPLSPrediction()
              ->setDataHash(projects->value(pid)->getMatrix(did)->getHash());
          projects->value(pid)
              ->getPLSModel(mid)
              ->getLastPLSPrediction()
              ->setObjName(objsel);
          projects->value(pid)
              ->getPLSModel(mid)
              ->getLastPLSPrediction()
              ->setYVarName(ysel);

          RUN obj;
          obj.setXMatrix(x);
          obj.setYMatrix(y);
          obj.setPLSModel(projects->value(pid)->getPLSModel(mid));

          QFuture<void> future = obj.RunPLSPrediction();
          while (!future.isFinished())
            QApplication::processEvents();

          //         ModelPrediction Name - Tab Count - pid - Model ID - xdata
          //         id - ydata id - Data Position - Data Type (PCA Prediction,
          //         PLS Prediction, ...) (8)
          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, projects->value(pid)
                                  ->getPLSModel(mid)
                                  ->getLastPLSPrediction()
                                  ->getName());
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, QString::number(mid));
          subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());

          if (ysel.size() > 0) {
            subitem->setText(5,
                             projects->value(pid)->getMatrix(did)->getHash());
          } else {
            subitem->setText(5, "None");
          }

          subitem->setText(6, QString::number(projects->value(pid)
                                                  ->getPLSModel(mid)
                                                  ->getLastPLSPrediction()
                                                  ->getPredID()));
          subitem->setText(7, QString("PLS Prediction"));

#ifdef DEBUG
          qDebug() << "X Predicted Scores";
          PrintMatrix(projects->value(pid)
                          ->getPLSModel(mid)
                          ->getLastPLSPrediction()
                          ->getXPredScores());
          qDebug() << "Y Dipendent Value Predicted";
          PrintMatrix(projects->value(pid)
                          ->getPLSModel(mid)
                          ->getLastPLSPrediction()
                          ->getYDipVar());
          qDebug() << subitem->text(0) << subitem->text(1) << subitem->text(2)
                   << subitem->text(3) << subitem->text(4) << subitem->text(5);
#endif

          tabcount_++;

          getModelItem(pid, mid)->addChild(subitem);
        } else {
          QMessageBox::critical(
              this, tr("PLS Prediction Error"),
              tr("Unable to compute PLS Prediction.\n"
                 "The number of variables differ. Please check your data."),
              QMessageBox::Ok);
          updateLog(
              QString("Error!! Unable to compute PLS Prediction. The number of "
                      "variables differ. Please check your data.\n"));
        }
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelMatrix(&x);
        DelMatrix(&y);
        projects->value(pid)->AutoSave();
      }
    }
  }
}

void MainWindow::DoPLSValidation() {
  if (!projects->isEmpty()) {
    // collect all pls model;

    ValidatorDialog doplsval(projects, PLSValidation);
    if (doplsval.exec() == QDialog::Accepted && doplsval.compute() == true) {
      int pid = doplsval.getselectedProject();
      int mid = doplsval.getModelID();
      int ngroup = doplsval.getNumberOfGroup();
      int niter = doplsval.getNumberOfIteration();
      int vt = doplsval.getValidType();
      LABELS kfoldclasses = doplsval.getKFoldClasses();
      bool yscrambling = doplsval.ModelYScrambling();
      int n_yscrambling = doplsval.getYSCramblingModels();

      int did;
      did = -1;

      if (pid != -1 && mid != -1) {
        did = projects->value(pid)->getMatrixID(
            projects->value(pid)
                ->getPLSModel(mid)
                ->getDataHash()) /* projects->value(pid)->getPLSModel(mid)->getXID()*/
            ;
      }

      if (did != -1 && did < projects->value(pid)->MatrixCount()) {

        StartRun();
        CalculationMenuDisable(pid);
        updateLog(
            QString("--------------------\n Computing PLS Validation for: %1")
                .arg(projects->value(pid)->getProjectName()));

        QStringList objsel, xvarsel, yvarsel;
        objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
        xvarsel = projects->value(pid)->getPLSModel(mid)->getXVarName();
        yvarsel = projects->value(pid)->getPLSModel(mid)->getYVarName();
        LABELS classes = projects->value(pid)->getPLSModel(mid)->getClasses();

        matrix *x, *y;
        initMatrix(&x);
        initMatrix(&y);

        if (yvarsel.size() > 0 && classes.size() == 0) {
          PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, xvarsel,
                        yvarsel, x, y);
        } else {
          PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, xvarsel,
                        classes, x, y);
        }

        uivector *kfc;
        initUIVector(&kfc);
        PrepareKFoldClasses(objsel, kfoldclasses, kfc);

        RUN obj;
        obj.setXMatrix(x);
        obj.setYMatrix(y);
        obj.setPLSModel(projects->value(pid)->getPLSModel(mid));
        obj.setValidationType(vt);
        obj.setUIVector(kfc);
        obj.setModelYScrambling(yscrambling);
        obj.setModelYScramblingModels(n_yscrambling);

        if (vt == BOOTSTRAPRGCV_) {
          obj.setNumberOfGroups(ngroup);
          obj.setNumberOfIterations(niter);
        }

        projects->value(pid)->getPLSModel(mid)->setValidation(0);
        QFuture<void> future = obj.RunPLSValidation(
            projects->value(pid)->getPLSModel(mid)->getAlgorithm());
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
          projects->value(pid)->getPLSModel(mid)->setValidation(vt);
        }

        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelMatrix(&x);
        DelMatrix(&y);
        DelUIVector(&kfc);
        projects->value(pid)->AutoSave();
      } else {
        QMessageBox::critical(
            this, tr("PLS Validation Error"),
            "Unable to compute PLS Validation.\nData are lost.",
            QMessageBox::Ok);
        updateLog("Unable to compute PLS VALIDATION. Data are lost.");
      }
    }
  }
}

void MainWindow::DoPLSDA() { DoPLS(PLS_DA_); }

void MainWindow::DoPLSRegression() { DoPLS(PLS_); }

void MainWindow::DoPLS(int algtype) {
  if (!projects->isEmpty()) {
    ModelDialogWizard dopls(projects, algtype);
    if (dopls.exec() == QDialog::Accepted && dopls.compute() == true) {
      int pid = dopls.getselectedProject();
      int did = dopls.getselectedData();
      int xscaling = dopls.getXScalingType();
      int yscaling = dopls.getYScalingType();
      int pc = dopls.getNumberOfComponent();

      QStringList objsel = dopls.getObjectSelected();
      QStringList xvarsel = dopls.getXVarSelected();
      QStringList yvarsel = dopls.getYVarSelected();
      LABELS classes = dopls.getClasses();
      QString modelname = "PLS - " + dopls.getModelName();

      if (did != -1 && pid != -1 && objsel.size() > 0 && xvarsel.size() > 0 &&
          yvarsel.size() > 0) {
        StartRun();
        CalculationMenuDisable(pid);
        QString str = "--------------------\n Computing PLS for: ";
        str.append(QString("%1").arg(projects->value(pid)->getProjectName()));
        projects->value(pid)->addPLSModel();
        updateLog(str);
        projects->value(pid)->getLastPLSModel()->setAlgorithm(algtype);
        projects->value(pid)->getLastPLSModel()->setDID(did);
        projects->value(pid)->getLastPLSModel()->setDataHash(
            projects->value(pid)->getMatrix(did)->getHash());
        projects->value(pid)->getLastPLSModel()->setXScaling(xscaling);
        projects->value(pid)->getLastPLSModel()->setYScaling(yscaling);
        if (pc > xvarsel.size()) {
          projects->value(pid)->getLastPLSModel()->setNPC(xvarsel.size());
        } else {
          projects->value(pid)->getLastPLSModel()->setNPC(pc);
        }
        projects->value(pid)->getLastPLSModel()->setModelID(mid_);
        projects->value(pid)->getLastPLSModel()->setName(modelname);
        projects->value(pid)->getLastPLSModel()->setObjName(objsel);
        projects->value(pid)->getLastPLSModel()->setXVarName(xvarsel);
        projects->value(pid)->getLastPLSModel()->setYVarName(yvarsel);

        matrix *x, *y;
        initMatrix(&x);
        initMatrix(&y);

        PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, xvarsel,
                      yvarsel, x, y);

        RUN obj;
        obj.setXMatrix(x);
        obj.setYMatrix(y);
        obj.setPLSModel(projects->value(pid)->getLastPLSModel());
        obj.setXScalingType(xscaling);
        obj.setYScalingType(yscaling);
        obj.setNumberPC(pc);

        QFuture<void> future = obj.RunPLS(algtype);
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
          WaitRun();
#ifdef DEBUG
          PrintPLSModel(projects->value(pid)->getPLSModel(mid_)->Model());
#endif

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, modelname);
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(5, QString::number(xscaling));
          subitem->setText(6, QString::number(yscaling));
          subitem->setText(7, QString::number(pc));
          subitem->setText(8, QString("PLS Model"));
          subitem->setText(9, QString::number(mid_));

          tabcount_++;
          mid_++;
          getProjectItem(pid)->child(1)->addChild(subitem);
        } else {
          int removeid = projects->value(pid)->PLSCount() - 1;
          projects->value(pid)->delPLSModelAt(removeid);
        }

        DelMatrix(&x);
        DelMatrix(&y);
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
      } else if (did != -1 && pid != -1 && objsel.size() > 0 &&
                 xvarsel.size() > 0 && classes.size() > 0) {
        StartRun();
        CalculationMenuDisable(pid);
        QString str = "--------------------\n Computing PLS DA for: ";
        str.append(QString("%1").arg(projects->value(pid)->getProjectName()));
        projects->value(pid)->addPLSModel();

        updateLog(str);
        projects->value(pid)->getLastPLSModel()->setAlgorithm(algtype);
        projects->value(pid)->getLastPLSModel()->setDID(did);
        projects->value(pid)->getLastPLSModel()->setDataHash(
            projects->value(pid)->getMatrix(did)->getHash());
        projects->value(pid)->getLastPLSModel()->setXScaling(xscaling);
        projects->value(pid)->getLastPLSModel()->setYScaling(0);

        if (pc > xvarsel.size()) {
          projects->value(pid)->getLastPLSModel()->setNPC(xvarsel.size());
        } else {
          projects->value(pid)->getLastPLSModel()->setNPC(pc);
        }
        projects->value(pid)->getLastPLSModel()->setModelID(mid_);
        projects->value(pid)->getLastPLSModel()->setName(modelname);
        projects->value(pid)->getLastPLSModel()->setObjName(objsel);
        projects->value(pid)->getLastPLSModel()->setXVarName(xvarsel);
        projects->value(pid)->getLastPLSModel()->setClasses(classes);

        matrix *x, *y;
        initMatrix(&x);
        initMatrix(&y);

        PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, xvarsel,
                      classes, x, y);

        RUN obj;
        obj.setXMatrix(x);
        obj.setYMatrix(y);
        obj.setPLSModel(projects->value(pid)->getLastPLSModel());
        obj.setXScalingType(xscaling);
        obj.setYScalingType(yscaling);
        obj.setNumberPC(pc);

        QFuture<void> future = obj.RunPLS(algtype);
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
          WaitRun();
#ifdef DEBUG
          PrintPLSModel(projects->value(pid)->getPLSModel(mid_)->Model());
#endif

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, modelname);
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(5, QString::number(xscaling));
          subitem->setText(6, QString::number(yscaling));
          subitem->setText(7, QString::number(pc));
          subitem->setText(8, QString("PLS Model"));
          subitem->setText(9, QString::number(mid_));

          tabcount_++;
          mid_++;
          getProjectItem(pid)->child(1)->addChild(subitem);
        } else {
          int removeid = projects->value(pid)->PLSCount() - 1;
          projects->value(pid)->delPLSModelAt(removeid);
        }

        DelMatrix(&x);
        DelMatrix(&y);
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
      }
    }
  }
}

void MainWindow::DoLDAPrediction() {
  if (!projects->isEmpty()) {
    DoPredictionDialog p(projects, LDA_);
    if (p.exec() == QDialog::Accepted && p.compute() == true) {
      int pid = p.getselectedProject();
      int mid = p.getselectedModel();
      int did = p.getselectedData();
      StartRun();
      CalculationMenuDisable(pid);
      TopMenuEnableDisable();
      QString modelname = p.getPredictionName();
      QStringList objsel = p.getObjectSelected();
      QStringList varsel = projects->value(pid)->getLDAModel(mid)->getVarName();

      matrix *x;
      NewMatrix(&x, objsel.size(), varsel.size());

      bool mxok = PrepareMatrix(projects->value(pid)->getMatrix(did), objsel,
                                varsel, x);

      if (x->col == (size_t)varsel.size() && mxok == true) {
        QString str = "--------------------\n Computing LDA Prediction for: ";
        str.append(QString("%1").arg(
            projects->value(p.getselectedProject())->getProjectName()));
        updateLog(str);

        projects->value(pid)->getLDAModel(mid)->addLDAPrediction();

        projects->value(pid)->getLDAModel(mid)->getLastLDAPrediction()->setName(
            "LDA Prediction - " + modelname);
        projects->value(pid)
            ->getLDAModel(mid)
            ->getLastLDAPrediction()
            ->setPredID(
                projects->value(pid)->getLDAModel(mid)->LDAPredictionCount() -
                1);
        projects->value(pid)->getLDAModel(mid)->getLastLDAPrediction()->setDID(
            did);
        projects->value(pid)
            ->getLDAModel(mid)
            ->getLastLDAPrediction()
            ->setDataHash(projects->value(pid)->getMatrix(did)->getHash());
        projects->value(pid)
            ->getLDAModel(mid)
            ->getLastLDAPrediction()
            ->setObjName(objsel);
        projects->value(pid)
            ->getLDAModel(mid)
            ->getLastLDAPrediction()
            ->setNameClasses(
                projects->value(pid)->getLDAModel(mid)->getNameClasses());

        RUN obj;

        obj.setXMatrix(x);
        obj.setLDAModel(projects->value(pid)->getLDAModel(mid));

        QFuture<void> future = obj.RunLDAPrediction();

        while (!future.isFinished()) {
          QApplication::processEvents();
        }

        QList<QStringList> classes;
        int maxclass = projects->value(pid)
                           ->getLDAModel(mid)
                           ->getLastLDAPrediction()
                           ->getPredClasses()
                           ->data[0][0];
        for (size_t i = 1; i < projects->value(pid)
                                   ->getLDAModel(mid)
                                   ->getLastLDAPrediction()
                                   ->getPredClasses()
                                   ->row;
             i++) {
          if (projects->value(pid)
                  ->getLDAModel(mid)
                  ->getLastLDAPrediction()
                  ->getPredClasses()
                  ->data[i][0] > (size_t)maxclass)
            maxclass = projects->value(pid)
                           ->getLDAModel(mid)
                           ->getLastLDAPrediction()
                           ->getPredClasses()
                           ->data[i][0];
          else
            continue;
        }

        maxclass++;
        for (int i = 0; i < maxclass; i++)
          classes.append(QStringList());

        for (size_t i = 1; i < projects->value(pid)
                                   ->getLDAModel(mid)
                                   ->getLastLDAPrediction()
                                   ->getPredClasses()
                                   ->row;
             i++) {
          int cid = projects->value(pid)
                        ->getLDAModel(mid)
                        ->getLastLDAPrediction()
                        ->getPredClasses()
                        ->data[i][0];
          classes[cid].append(objsel[i]);
        }

        projects->value(pid)
            ->getLDAModel(mid)
            ->getLastLDAPrediction()
            ->setClasses(classes);

        QTreeWidgetItem *subitem = new QTreeWidgetItem;
        subitem->setText(0, projects->value(pid)
                                ->getLDAModel(mid)
                                ->getLastLDAPrediction()
                                ->getName());
        subitem->setText(1, QString::number(tabcount_));
        subitem->setText(2, QString::number(pid));
        subitem->setText(3, QString::number(mid));
        subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());
        subitem->setText(5, "");
        subitem->setText(6, QString::number(projects->value(pid)
                                                ->getLDAModel(mid)
                                                ->getLastLDAPrediction()
                                                ->getPredID()));
        subitem->setText(7, QString("LDA Prediction"));

        tabcount_++;
        getModelItem(pid, mid)->addChild(subitem);
      } else {
        QMessageBox::critical(
            this, tr("LDA Prediction Error"),
            tr("Unable to compute LDA Prediction.\n"
               "The number of variables differ. Please check your data."),
            QMessageBox::Ok);
        updateLog(
            QString("Error!! Unable to compute LDA Prediction. The number of "
                    "variables differ. Please check your data.\n"));
      }
      TopMenuEnableDisable();
      CalculationMenuEnable();
      StopRun();
      DelMatrix(&x);
      projects->value(pid)->AutoSave();
    }
  }
}

void MainWindow::DoLDAValidation() {
  if (!projects->isEmpty()) {
    // collect all pls model;

    ValidatorDialog doldavalid(projects, LDAValidation);
    if (doldavalid.exec() == QDialog::Accepted &&
        doldavalid.compute() == true) {
      int pid = doldavalid.getselectedProject();
      int mid = doldavalid.getModelID();
      LABELS kfoldclasses = doldavalid.getKFoldClasses();
      int ngroup = doldavalid.getNumberOfGroup();
      int niter = doldavalid.getNumberOfIteration();
      int vt = doldavalid.getValidType();

      int did;
      did = -1;

      if (pid != -1 && mid != -1) {
        did = projects->value(pid)->getMatrixID(
            projects->value(pid)
                ->getLDAModel(mid)
                ->getDataHash()) /* projects->value(pid)->getLDAModel(mid)->getXID()*/
            ;
      }

      if (did != -1 && did < projects->value(pid)->MatrixCount()) {

        StartRun();
        CalculationMenuDisable(pid);
        updateLog(
            QString("--------------------\n Computing LDA Validation for: %1")
                .arg(projects->value(pid)->getLDAModel(mid)->getName()));

        QStringList objsel, varsel;
        objsel = projects->value(pid)->getLDAModel(mid)->getObjName();
        varsel = projects->value(pid)->getLDAModel(mid)->getVarName();
        QList<QStringList> classes =
            projects->value(pid)->getLDAModel(mid)->getClasses();

        matrix *x;
        matrix *y;
        NewMatrix(&x, objsel.size(), varsel.size());
        NewMatrix(&y, objsel.size(), 1);

        PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, varsel, x);

        // Rudimental y class preparation
        for (int i = 0;
             i < projects->value(pid)->getMatrix(did)->getObjName().size();
             i++) {
          int ii = _index_of_(
              objsel, projects->value(pid)->getMatrix(did)->getObjName()[i]);
          if (ii > -1) {
            for (int j = 0; j < classes.size(); j++) {
              if (classes[j].contains(
                      projects->value(pid)->getMatrix(did)->getObjName()[i]) ==
                  true) {
                y->data[ii][0] = j;
                break;
              } else {
                continue;
              }
            }
          } else {
            continue;
          }
          QApplication::processEvents();
        }

        RUN obj;
        obj.setXMatrix(x);
        obj.setYMatrix(y);
        obj.setLDAModel(projects->value(pid)->getLDAModel(mid));
        obj.setValidationType(vt);

        if (vt == BOOTSTRAPRGCV_) {
          obj.setNumberOfGroups(ngroup);
          obj.setNumberOfIterations(niter);
        }

        projects->value(pid)->getLDAModel(mid)->setValidation(0);
        QFuture<void> future = obj.RunLDAValidation();
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
          projects->value(pid)->getLDAModel(mid)->setValidation(vt);
        }
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelMatrix(&x);
        DelMatrix(&y);
        projects->value(pid)->AutoSave();
      } else {
        QMessageBox::critical(
            this, tr("LDA Validation Error"),
            "Unable to compute LDA Validation.\nData are lost.",
            QMessageBox::Ok);
        updateLog("Unable to compute LDA VALIDATION. Data are lost.");
      }
    }
  }
}

void MainWindow::DoLDA() {
  if (!projects->isEmpty()) {

    //     ModelDialog dolda(projects, LDA_);
    ClassModelDialog dolda(projects);
    if (dolda.exec() == QDialog::Accepted && dolda.compute() == true) {
      int pid = dolda.getselectedProject();
      int did = dolda.getselectedData();

      QList<QStringList> classes = dolda.getClasses();
      QStringList nameclasses = dolda.getNameClasses();
      QStringList varsel = dolda.getVarSelected();

      QStringList objsel;
      for (int i = 0; i < classes.size(); i++)
        objsel.append(classes[i]);

      QString modelname = "LDA - " + dolda.getModelName();

      if (did != -1 && pid != -1 && objsel.size() > 0 && varsel.size() > 0) {
        StartRun();
        CalculationMenuDisable(pid);
        QString str = "--------------------\n Computing LDA for: ";
        str.append(QString("%1").arg(projects->value(pid)->getProjectName()));
        projects->value(pid)->addLDAModel();
        updateLog(str);

        projects->value(pid)->getLastLDAModel()->setDID(did);
        projects->value(pid)->getLastLDAModel()->setDataHash(
            projects->value(pid)->getMatrix(did)->getHash());
        projects->value(pid)->getLastLDAModel()->setModelID(mid_);
        projects->value(pid)->getLastLDAModel()->setName(modelname);
        projects->value(pid)->getLastLDAModel()->setObjName(objsel);
        projects->value(pid)->getLastLDAModel()->setVarName(varsel);
        projects->value(pid)->getLastLDAModel()->setClasses(classes);
        projects->value(pid)->getLastLDAModel()->setNameClasses(nameclasses);

        matrix *x;
        matrix *y;
        NewMatrix(&x, objsel.size(), varsel.size());
        NewMatrix(&y, objsel.size(), 1);

        PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, varsel, x);

        // Rudimental y class preparation
        for (int i = 0;
             i < projects->value(pid)->getMatrix(did)->getObjName().size();
             i++) {
          int ii = _index_of_(
              objsel, projects->value(pid)->getMatrix(did)->getObjName()[i]);
          if (ii > -1) {
            for (int j = 0; j < classes.size(); j++) {
              if (classes[j].contains(
                      projects->value(pid)->getMatrix(did)->getObjName()[i]) ==
                  true) {
                y->data[ii][0] = j;
                break;
              } else {
                continue;
              }
            }
          } else {
            continue;
          }
          QApplication::processEvents();
        }

        RUN obj;
        obj.setXMatrix(x);
        obj.setYMatrix(y);
        obj.setLDAModel(projects->value(pid)->getLastLDAModel());

        QFuture<void> future = obj.RunLDA();
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
          WaitRun();
#ifdef DEBUG
          PrintLDAModel(projects->value(pid)->getLDAModel(mid_)->Model());
#endif

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, modelname);
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(5, "No Scaling");
          subitem->setText(6, "No Scaling");
          subitem->setText(7, "No Principal Components");
          subitem->setText(8, QString("LDA Model"));
          subitem->setText(9, QString::number(mid_));

          tabcount_++;
          mid_++;
          getProjectItem(pid)->child(1)->addChild(subitem);
        } else {
          int removeid = projects->value(pid)->LDACount() - 1;
          projects->value(pid)->delLDAModelAt(removeid);
        }

        DelMatrix(&x);
        DelMatrix(&y);
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        projects->value(pid)->AutoSave();
      }
    }
  }
}

void MainWindow::DoMLRPrediction() {
  if (!projects->isEmpty()) {
    int npls = 0;
    for (int i = 0; i < projects->values().size(); i++) {
      if (projects->values()[i]->MLRCount() > 0)
        npls++;
    }

    if (npls > 0) {
      DoPredictionDialog p(projects, MLR_);
      if (p.exec() == QDialog::Accepted && p.compute() == true) {
        int pid = p.getselectedProject();
        int mid = p.getselectedModel();
        int did = p.getselectedData();
        StartRun();
        CalculationMenuDisable(pid);
        TopMenuEnableDisable();
        QStringList objsel = p.getObjectSelected();
        QStringList ysel = p.getYVariableSelected();
        QString modelname = p.getPredictionName();

        QStringList xvarsel =
            projects->value(pid)->getMLRModel(mid)->getXVarName();
        QStringList yvarsel =
            projects->value(pid)->getMLRModel(mid)->getYVarName();

        matrix *x, *y;
        initMatrix(&x);
        initMatrix(&y);

        bool mxok = PrepareMatrix(projects->value(pid)->getMatrix(did), objsel,
                                  xvarsel, yvarsel, x, y);

        if (x->col == (size_t)xvarsel.size() && mxok == true) {
          QString str = "--------------------\n Computing MLR Prediction for: ";
          str.append(QString("%1").arg(projects->value(pid)->getProjectName()));
          updateLog(str);

          projects->value(pid)->getMLRModel(mid)->addMLRPrediction();

          projects->value(pid)
              ->getMLRModel(mid)
              ->getLastMLRPrediction()
              ->setName("MLR Prediction - " + modelname);
          projects->value(pid)
              ->getMLRModel(mid)
              ->getLastMLRPrediction()
              ->setPredID(
                  projects->value(pid)->getMLRModel(mid)->MLRPredictionCount() -
                  1);
          projects->value(pid)
              ->getMLRModel(mid)
              ->getLastMLRPrediction()
              ->setDID(did);
          projects->value(pid)
              ->getMLRModel(mid)
              ->getLastMLRPrediction()
              ->setDataHash(projects->value(pid)->getMatrix(did)->getHash());
          projects->value(pid)
              ->getMLRModel(mid)
              ->getLastMLRPrediction()
              ->setObjName(objsel);
          projects->value(pid)
              ->getMLRModel(mid)
              ->getLastMLRPrediction()
              ->setYVarName(ysel);

          RUN obj;
          obj.setXMatrix(x);
          obj.setYMatrix(y);
          obj.setMLRModel(projects->value(pid)->getMLRModel(mid));

          QFuture<void> future = obj.RunMLRPrediction();
          while (!future.isFinished())
            QApplication::processEvents();

          //         ModelPrediction Name - Tab Count - pid - Model ID - xdata
          //         id - ydata id - Data Position - Data Type (PCA Prediction,
          //         PLS Prediction, ...) (8)
          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, projects->value(pid)
                                  ->getMLRModel(mid)
                                  ->getLastMLRPrediction()
                                  ->getName());
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, QString::number(mid));
          subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());

          if (ysel.size() > 0) {
            subitem->setText(5,
                             projects->value(pid)->getMatrix(did)->getHash());
          } else {
            subitem->setText(5, "None");
          }

          subitem->setText(6, QString::number(projects->value(pid)
                                                  ->getMLRModel(mid)
                                                  ->getLastMLRPrediction()
                                                  ->getPredID()));
          subitem->setText(7, QString("MLR Prediction"));

          tabcount_++;

          getModelItem(pid, mid)->addChild(subitem);
        } else {
          QMessageBox::critical(
              this, tr("MLR Prediction Error"),
              tr("Unable to compute MLR Prediction.\n"
                 "The number of variables differ. Please check your data."),
              QMessageBox::Ok);
          updateLog(
              QString("Error!! Unable to compute MLR Prediction. The number of "
                      "variables differ. Please check your data.\n"));
        }
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelMatrix(&x);
        DelMatrix(&y);
        projects->value(pid)->AutoSave();
      }
    }
  }
}

void MainWindow::DoMLRValidation() {
  if (!projects->isEmpty()) {
    // collect all pls model;

    ValidatorDialog domlrval(projects, MLRValidation);
    if (domlrval.exec() == QDialog::Accepted && domlrval.compute() == true) {
      int pid = domlrval.getselectedProject();
      int mid = domlrval.getModelID();
      LABELS kfoldclasses = domlrval.getKFoldClasses();
      int ngroup = domlrval.getNumberOfGroup();
      int niter = domlrval.getNumberOfIteration();
      int vt = domlrval.getValidType();
      bool yscrambling = domlrval.ModelYScrambling();
      int n_yscrambling = domlrval.getYSCramblingModels();

      int did;
      did = -1;

      if (pid != -1 && mid != -1) {
        did = projects->value(pid)->getMatrixID(
            projects->value(pid)
                ->getMLRModel(mid)
                ->getDataHash()) /* projects->value(pid)->getMLRModel(mid)->getXID()*/
            ;
      }

      if (did != -1 && did < projects->value(pid)->MatrixCount()) {

        StartRun();
        CalculationMenuDisable(pid);
        updateLog(
            QString("--------------------\n Computing MLR Validation for: %1")
                .arg(projects->value(pid)->getMLRModel(mid)->getName()));

        QStringList objsel, xvarsel, yvarsel;
        objsel = projects->value(pid)->getMLRModel(mid)->getObjName();
        xvarsel = projects->value(pid)->getMLRModel(mid)->getXVarName();
        yvarsel = projects->value(pid)->getMLRModel(mid)->getYVarName();

        matrix *x, *y;
        initMatrix(&x);
        initMatrix(&y);

        PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, xvarsel,
                      yvarsel, x, y);

        uivector *kfc;
        initUIVector(&kfc);
        PrepareKFoldClasses(objsel, kfoldclasses, kfc);

        RUN obj;
        obj.setXMatrix(x);
        obj.setYMatrix(y);
        obj.setUIVector(kfc);
        obj.setMLRModel(projects->value(pid)->getMLRModel(mid));
        obj.setValidationType(vt);
        obj.setModelYScrambling(yscrambling);
        obj.setModelYScramblingModels(n_yscrambling);

        if (vt == BOOTSTRAPRGCV_) {
          obj.setNumberOfGroups(ngroup);
          obj.setNumberOfIterations(niter);
        }

        projects->value(pid)->getMLRModel(mid)->setValidation(0);
        QFuture<void> future = obj.RunMLRValidation();
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
#ifdef DEBUG
          qDebug() << "q2y validation";
          PrintDVector(projects->value(pid)->getMLRModel(mid)->Model()->q2y);
#endif

          projects->value(pid)->getMLRModel(mid)->setValidation(vt);
        }
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        DelMatrix(&x);
        DelMatrix(&y);
        DelUIVector(&kfc);
        projects->value(pid)->AutoSave();
      } else {
        QMessageBox::critical(
            this, tr("MLR Validation Error"),
            "Unable to compute MLR Validation.\nData are lost.",
            QMessageBox::Ok);
        updateLog("Unable to compute MLR VALIDATION. Data are lost.");
      }
    }
  }
}

void MainWindow::DoMLR() {
  if (!projects->isEmpty()) {

    ModelDialog domlr(projects, MLR_);
    if (domlr.exec() == QDialog::Accepted && domlr.compute() == true) {
      int pid = domlr.getselectedProject();
      int did = domlr.getselectedData();

      QStringList objsel = domlr.getObjectSelected();
      QStringList xvarsel = domlr.getXVarSelected();
      QStringList yvarsel = domlr.getYVarSelected();

      QString modelname = "MLR - " + domlr.getModelName();

      if (did != -1 && pid != -1 && objsel.size() > 0 && xvarsel.size() > 0 &&
          yvarsel.size() > 0) {
        StartRun();
        CalculationMenuDisable(pid);
        QString str = "--------------------\n Computing MLR for: ";
        str.append(QString("%1").arg(projects->value(pid)->getProjectName()));
        projects->value(pid)->addMLRModel();
        updateLog(str);

        projects->value(pid)->getLastMLRModel()->setDID(did);
        projects->value(pid)->getLastMLRModel()->setDataHash(
            projects->value(pid)->getMatrix(did)->getHash());
        projects->value(pid)->getLastMLRModel()->setModelID(mid_);
        projects->value(pid)->getLastMLRModel()->setName(modelname);
        projects->value(pid)->getLastMLRModel()->setObjName(objsel);
        projects->value(pid)->getLastMLRModel()->setXVarName(xvarsel);
        projects->value(pid)->getLastMLRModel()->setYVarName(yvarsel);

        matrix *x, *y;
        initMatrix(&x);
        initMatrix(&y);

        PrepareMatrix(projects->value(pid)->getMatrix(did), objsel, xvarsel,
                      yvarsel, x, y);

        RUN obj;
        obj.setXMatrix(x);
        obj.setYMatrix(y);
        obj.setMLRModel(projects->value(pid)->getLastMLRModel());

        QFuture<void> future = obj.RunMLR();
        while (!future.isFinished()) {
          if (stoprun == true) {
            obj.AbortRun();
            QApplication::processEvents();
          } else {
            QApplication::processEvents();
          }
        }

        if (stoprun == false) {
          WaitRun();
#ifdef DEBUG
          PrintMLR(projects->value(pid)->getMLRModel(mid_)->Model());
#endif

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, modelname);
          subitem->setText(1, QString::number(tabcount_));
          subitem->setText(2, QString::number(pid));
          subitem->setText(3, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(4, projects->value(pid)->getMatrix(did)->getHash());
          subitem->setText(5, "No Scaling");
          subitem->setText(6, "No Scaling");
          subitem->setText(7, "No Principal Components");
          subitem->setText(8, QString("MLR Model"));
          subitem->setText(9, QString::number(mid_));

          tabcount_++;
          mid_++;
          getProjectItem(pid)->child(1)->addChild(subitem);
        } else {
          int removeid = projects->value(pid)->MLRCount() - 1;
          projects->value(pid)->delMLRModelAt(removeid);
        }

        DelMatrix(&x);
        DelMatrix(&y);
        TopMenuEnableDisable();
        CalculationMenuEnable();
        StopRun();
        projects->value(pid)->AutoSave();
      }
    }
  }
}

void MainWindow::SubWindowsCascade() { ui.mdiArea->cascadeSubWindows(); }

void MainWindow::SubWindowsTiled() { ui.mdiArea->tileSubWindows(); }

void MainWindow::About() {
  int major = major_;
  int minor = minor_;
  int patch = patch_;
  AboutDialog about(major, minor, patch);
  about.exec();
}

void MainWindow::changeEvent(QEvent *e) {
  QMainWindow::changeEvent(e);
  switch (e->type()) {
  case QEvent::LanguageChange:
    ui.retranslateUi(this);
    break;
  default:
    break;
  }
}

void MainWindow::Test() {

#ifdef DEBUG
  qDebug() << "Current PID " << pid_ << " Current TABID " << tabcount_
           << " Current MID " << mid_;
  QMapIterator<int, DATA *> i((*projects));
  while (i.hasNext()) {
    i.next();
    qDebug() << "######################################################";
    qDebug() << "######################################################";
    qDebug() << "Project: " << i.value()->getProjectName();

    for (int j = 0; j < i.value()->MatrixCount(); j++) {
      qDebug() << "Matrix " << i.value()->getMatrix(j)->getName()
               << " hash: " << i.value()->getMatrix(j)->getHash() << "Varnames "
               << i.value()->getMatrix(j)->getVarName();
    }

    for (int j = 0; j < i.value()->ArrayCount(); j++) {
      qDebug() << "Array " << i.value()->getArray(j)->getName()
               << " hash: " << i.value()->getArray(j)->getHash();
    }

    for (int j = 0; j < i.value()->getObjectLabels().size(); j++) {
      qDebug() << i.value()->getObjectLabels()[j].name << " selected objects "
               << i.value()->getObjectLabels()[j].objects;
    }

    for (int j = 0; j < i.value()->getVariableLabels().size(); j++) {
      qDebug() << i.value()->getVariableLabels()[j].name << " selected objects "
               << i.value()->getVariableLabels()[j].objects;
    }

    qDebug()
        << QString("%1 PCA Models").arg(QString::number(i.value()->PCACount()));
    for (int j = 0; j < i.value()->PCACount(); j++) {
      qDebug() << "Mod Name: " << i.value()->getPCAModelAt(j)->getName()
               << " PC: " << i.value()->getPCAModelAt(j)->getNPC()
               << "Model Position: "
               << i.value()->getPCAModelAt(j)->getModelID() << " Data Matrix: "
               << i.value()->getPCAModelAt(j)->getDataHash();
      qDebug() << QString("%1 PCA Predictions")
                      .arg(QString::number(
                          i.value()->getPCAModelAt(j)->PCAPredictionCount()));
      for (int k = 0; k < i.value()->getPCAModelAt(j)->PCAPredictionCount();
           k++) {
        qDebug()
            << "Prediction Name: "
            << i.value()->getPCAModelAt(j)->getPCAPrediction(k)->getName()
            << "Data Position: "
            << i.value()->getPCAModelAt(j)->getPCAPrediction(k)->getPredID();
      }
    }

    qDebug()
        << QString("%1 PLS Models").arg(QString::number(i.value()->PLSCount()));
    for (int j = 0; j < i.value()->PLSCount(); j++) {
      qDebug() << "Mod Name: " << i.value()->getPLSModelAt(j)->getName()
               << " PC: " << i.value()->getPLSModelAt(j)->getNPC()
               << " Validated: " << i.value()->getPLSModelAt(j)->getValidation()
               << "Model Position: "
               << i.value()->getPLSModelAt(j)->getModelID() << " Data Matrix: "
               << i.value()->getPLSModelAt(j)->getDataHash();
      qDebug() << QString("%1 PCA Predictions")
                      .arg(QString::number(
                          i.value()->getPLSModelAt(j)->PLSPredictionCount()));
      for (int k = 0; k < i.value()->getPLSModelAt(j)->PLSPredictionCount();
           k++) {
        qDebug()
            << "Prediction Name: "
            << i.value()->getPLSModelAt(j)->getPLSPrediction(k)->getName()
            << "Data Position: "
            << i.value()->getPLSModelAt(j)->getPLSPrediction(k)->getPredID();
      }
    }

    // qDebug() << QString("%1 EPLS
    // Models").arg(QString::number(i.value()->EPLSCount())); for(int j = 0; j <
    // i.value()->EPLSCount(); j++){
    //   qDebug() << "Mod Name: " <<  i.value()->getEPLSModelAt(j)->getName() <<
    //   "Model Position: " << i.value()->getEPLSModelAt(j)->getModelID() <<
    //   "PID " << i.value()->getEPLSModelAt(j)->getProjectID();
    //   PrintMatrix(i.value()->getEPLSModelAt(j)->getMap());
    //   PrintUIVector(i.value()->getEPLSModelAt(j)->getSelectedVariables());
    //   PrintUIVector(i.value()->getEPLSModelAt(j)->getVariableDistribution());
    // }
  }
#endif
}

MainWindow::MainWindow(QString confdir_, QString key_) : QMainWindow(0) {
  ui.setupUi(this);

  // #if defined(RELEASE) || defined(WIN32) // look in ImportFileDialog.cpp...
  // same exception...
  ui.actionPCA2DLoadingsMVAND_Plot->setVisible(false);
  // #endif

  confdir = confdir_;
  LoadRecentsModelsFile();

  ui.progressframe->hide();
  ImageWindow();
  stoprun = false;

  projects = new QMap<int, DATA *>;

  QString str = "\nWelcome to QStudioMetrics!\n";
  updateLog(str);

  lastpath = QDir::homePath();
  tabcount_ = 0; // used for mark the tab to show; each project have a specified
                 // tab number in order to destroy this tab if is up!
  calcpid = -1;
  pid_ = 0; // used for mark all the projects with an unique id instead of the
            // name. More projects can have the same name but different id!
  mid_ = 0; // used for mark all the models with an unique id

  havepca = false;
  havepcapred = false;
  havecpca = false;
  havecpcapred = false;
  havepls = false;
  haveplspred = false;
  haveplsvalid = false;
  havemlr = false;
  havemlrvalid = false;
  havemlrpred = false;
  havelda = false;
  haveldapred = false;
  haveldavalid = false;

  ui.treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui.treeWidget, SIGNAL(customContextMenuRequested(const QPoint &)),
          this, SLOT(ShowContextMenu(const QPoint &)));

  connect(ui.actionNewProject, SIGNAL(triggered(bool)), SLOT(NewProject()));
  connect(ui.actionOpenProject, SIGNAL(triggered(bool)), SLOT(OpenProject()));
  connect(ui.actionSaveAs, SIGNAL(triggered(bool)), SLOT(SaveAs()));
  connect(ui.actionQuit, SIGNAL(triggered(bool)), SLOT(Quit()));

  connect(ui.actionAdd_Remove_Object_Labels, SIGNAL(triggered(bool)),
          SLOT(AddRemoveObjLabel()));
  connect(ui.actionAdd_Remove_Variable_Labels, SIGNAL(triggered(bool)),
          SLOT(AddRemoveVarLabel()));
  connect(ui.actionExtract_Data_Objects_and_Variables, SIGNAL(triggered(bool)),
          SLOT(ExtractData()));
  connect(ui.actionMerge_Data, SIGNAL(triggered(bool)), SLOT(MergeData()));
  connect(ui.actionAdvanced_Pretreatment, SIGNAL(triggered(bool)),
          SLOT(AdvancedPretreatment()));

  connect(ui.actionPCA, SIGNAL(triggered(bool)), SLOT(DoPCA()));
  connect(ui.actionPCA_Prediction, SIGNAL(triggered(bool)),
          SLOT(DoPCAPrediction()));

  connect(ui.actionCPCA, SIGNAL(triggered(bool)), SLOT(DoCPCA()));
  connect(ui.actionCPCA_Prediction, SIGNAL(triggered(bool)),
          SLOT(DoCPCAPrediction()));

  connect(ui.actionPLS_Regression, SIGNAL(triggered(bool)),
          SLOT(DoPLSRegression()));
  connect(ui.actionPLS_Discriminant_Analysis, SIGNAL(triggered(bool)),
          SLOT(DoPLSDA()));
  connect(ui.actionPLS_Prediction, SIGNAL(triggered(bool)),
          SLOT(DoPLSPrediction()));
  connect(ui.actionPLS_Validator, SIGNAL(triggered(bool)),
          SLOT(DoPLSValidation()));

  connect(ui.actionMLR, SIGNAL(triggered(bool)), SLOT(DoMLR()));
  connect(ui.actionMLR_Prediction, SIGNAL(triggered(bool)),
          SLOT(DoMLRPrediction()));
  connect(ui.actionMLR_Validator, SIGNAL(triggered(bool)),
          SLOT(DoMLRValidation()));

  connect(ui.actionLDA, SIGNAL(triggered(bool)), SLOT(DoLDA()));
  connect(ui.actionLDA_Prediction, SIGNAL(triggered(bool)),
          SLOT(DoLDAPrediction()));
  connect(ui.actionLDA_Validator, SIGNAL(triggered(bool)),
          SLOT(DoLDAValidation()));

  connect(ui.actionVariableDistribution, SIGNAL(triggered(bool)),
          SLOT(PlotVariableDistribution()));
  connect(ui.actionVariable_vs_Variable, SIGNAL(triggered(bool)),
          SLOT(PlotVariableVSVariable()));

  connect(ui.actionPCA2DScore_Plot, SIGNAL(triggered(bool)),
          SLOT(PCA2DScorePlot()));
  connect(ui.actionPCA2DLoadings_Plot, SIGNAL(triggered(bool)),
          SLOT(PCA2DLoadingsPlot()));
  connect(ui.actionPCA2DTsq_Contribution_Plot, SIGNAL(triggered(bool)),
        SLOT(PCATsqContributionPlot()));
  connect(ui.actionPCA2DExpVarPlot, SIGNAL(triggered(bool)),
          SLOT(PCA2DExpVarPlot()));
  // connect(ui.actionPCA2DLoadingsMVAND_Plot, SIGNAL(triggered(bool)),
  // SLOT(PCA2DLoadingsMVANDPlot()));
  connect(ui.actionPCA2DScore_Plot_Prediction, SIGNAL(triggered(bool)),
          SLOT(PCA2DScorePlotPrediction()));

  connect(ui.actionCPCA2DSuperScores_Plot, SIGNAL(triggered(bool)),
          SLOT(CPCA2DSuperScorePlot()));
  connect(ui.actionCPCA2DSuperWeights_Plot, SIGNAL(triggered(bool)),
          SLOT(CPCA2DSuperWeightsPlot()));
  connect(ui.actionCPCA2DBlockScores_Plot, SIGNAL(triggered(bool)),
          SLOT(CPCA2DBlockScoresPlot()));
  connect(ui.actionCPCA2DBlockLoadings_Plot, SIGNAL(triggered(bool)),
          SLOT(CPCA2DBlockLoadingsPlot()));
  connect(ui.actionCPCA2DExplained_variance_Plot, SIGNAL(triggered(bool)),
          SLOT(CPCA2DExpVarPlot()));
  connect(ui.actionCPCA2DSuperScores_Plot_Prediction, SIGNAL(triggered(bool)),
          SLOT(CPCA2DSuperScoresPlotPrediction()));
  connect(ui.actionCPCA2DBlockScores_Plot_Prediction, SIGNAL(triggered(bool)),
          SLOT(CPCA2DBlockScoresPlotPrediction()));

  connect(ui.actionPLS_Plot, SIGNAL(triggered(bool)), SLOT(PLS2DPlot()));
  connect(ui.actionPLS2D_tt_Score_Plot, SIGNAL(triggered(bool)),
          SLOT(PLS2DTTScorePlot()));
  connect(ui.actionPLS2D_pp_Loadings_Plot, SIGNAL(triggered(bool)),
          SLOT(PLS2DPPLoadingsPlot()));
  connect(ui.actionPLS2D_ww_Weights_Plot, SIGNAL(triggered(bool)),
          SLOT(PLS2DWWWeightsPlot()));
  connect(ui.actionPLS2D_uu_Score_Plot, SIGNAL(triggered(bool)),
          SLOT(PLS2DUUScorePlot()));
  connect(ui.actionPLS2D_qq_Loadings_Plot, SIGNAL(triggered(bool)),
          SLOT(PLS2DQQLoadingsPlot()));
  connect(ui.actionPLS2D_p_q_p_q_Loadings_Plot, SIGNAL(triggered(bool)),
          SLOT(PLS2DPQLoadingsPlot()));
  connect(ui.actionPLS2DScore_Plot_Prediction, SIGNAL(triggered(bool)),
          SLOT(PLS2DTTScorePlotPrediction()));
  connect(ui.actionPLSRecalc_vs_Exp_with_Prediction, SIGNAL(triggered(bool)),
          SLOT(PLSRecalcVSExpPlotPrediction()));
  connect(ui.actionPLSPred_vs_Exp_with_Prediction, SIGNAL(triggered(bool)),
          SLOT(PLSPredictedVSExpAndPredictionPlot()));
  connect(ui.actionPLSRecalc_vs_Exp, SIGNAL(triggered(bool)),
          SLOT(PLSRecalcVSExpPlot()));
  connect(ui.actionPLSRecalc_Residuals_vs_Exp, SIGNAL(triggered(bool)),
          SLOT(PLSRecalcResidualsVSExpPlot()));
  connect(ui.actionPLSPred_vs_Exp, SIGNAL(triggered(bool)),
          SLOT(PLSPredVSExpPlot()));
  connect(ui.actionPLSPred_Residuals_vs_Exp, SIGNAL(triggered(bool)),
          SLOT(PLSPredResidualsVSExpPlot()));
  connect(ui.actionPLSR2_Q2, SIGNAL(triggered(bool)), SLOT(PLSPlotR2Q2()));
  connect(ui.actionPLSRMSE, SIGNAL(triggered(bool)), SLOT(PLSPlotRMSE()));
  connect(ui.actionPLSROC_Curve, SIGNAL(triggered(bool)),
          SLOT(PLSPlotROCCurves()));
  connect(ui.actionPLSROC_AUC_recalculated_predicted, SIGNAL(triggered(bool)),
          SLOT(PLSPlotROCAucs()));
  connect(ui.actionPLSPrecision_recall_Curve, SIGNAL(triggered(bool)),
          SLOT(PLSPlotPRCurves()));
  connect(ui.actionPLSPrecision_Recall_AUC_recalculated_predicted,
          SIGNAL(triggered(bool)), SLOT(PLSPlotPRAucs()));

  connect(ui.actionPLSYScrambling, SIGNAL(triggered(bool)),
          SLOT(PLSPlotYScrambling()));
  connect(ui.actionPLSBeta_Coefficients, SIGNAL(triggered(bool)),
          SLOT(PLSPlotBetaCoefficients()));
  connect(ui.actionBetaCoeffDWPlot, SIGNAL(triggered(bool)),
          SLOT(PLSPlotBetaCoeffDWPlot()));

  connect(ui.actionPLSR2_Prediction, SIGNAL(triggered(bool)),
          SLOT(PLSPlotR2R2Predicted()));
  connect(ui.actionPLSRMSEModel_and_External_Prediction,
          SIGNAL(triggered(bool)), SLOT(PLSPlotRMSEPredicted()));

  connect(ui.actionMLR_Recal_vs_Exp_and_Prediction, SIGNAL(triggered(bool)),
          SLOT(MLRRecalcVSExpAndPredictionPlot()));
  connect(ui.actionMLR_Predict_vs_Exp_and_Prediction, SIGNAL(triggered(bool)),
          SLOT(MLRPredictedVSExpAndPredictionPlot()));
  connect(ui.actionMLR_Beta_Coefficients, SIGNAL(triggered(bool)),
          SLOT(MLRBetaCoefficients()));

  connect(ui.actionMLR_Recalc_vs_Exp, SIGNAL(triggered(bool)),
          SLOT(MLRRecalcVSExpPlot()));
  connect(ui.actionMLR_Recalc_Residuals_vs_Exp, SIGNAL(triggered(bool)),
          SLOT(MLRRecalcResidualsVSExpPlot()));
  connect(ui.actionMLR_Predict_vs_Exp, SIGNAL(triggered(bool)),
          SLOT(MLRPredVSExpPlot()));
  connect(ui.actionMLR_Predict_Residuals_vs_Exp, SIGNAL(triggered(bool)),
          SLOT(MLRPredResidualsVSExpPlot()));
  connect(ui.actionMLR_Y_Scrambling_Plot, SIGNAL(triggered(bool)),
          SLOT(MLRPlotYScrambling()));

  connect(ui.actionLDA2D_Feature_Plot, SIGNAL(triggered(bool)),
          SLOT(LDAFeaturePlot2D()));
  connect(ui.actionLDA2D_Feature_Plot_and_Predictions, SIGNAL(triggered(bool)),
          SLOT(LDAFeaturePlotAndPrediction2D()));
  connect(ui.actionLDAROC_Curve, SIGNAL(triggered(bool)), SLOT(LDAROCPlot()));
  connect(ui.actionLDAPrecision_Recall_Curve, SIGNAL(triggered(bool)),
          SLOT(LDAPRPlot()));
  connect(ui.actionLDAProbability_Distribution_Plot, SIGNAL(triggered(bool)),
          SLOT(LDAProbabilityDistribution()));
  connect(ui.actionLDAProbability_Distribution_Plot_and_Predictions,
          SIGNAL(triggered(bool)),
          SLOT(LDAProbabilityDistributionWithPredictions()));

  connect(ui.actionCascade_SubWindows, SIGNAL(triggered(bool)),
          SLOT(SubWindowsCascade()));
  connect(ui.actionTile_SubWindows, SIGNAL(triggered(bool)),
          SLOT(SubWindowsTiled()));
  connect(ui.actionAbout, SIGNAL(triggered(bool)), SLOT(About()));
  connect(ui.actionProject_Window, SIGNAL(changed()), SLOT(ProjectWindow()));
  connect(ui.actionLog_Window, SIGNAL(changed()), SLOT(LogWindow()));
  connect(ui.actionViewer_Window, SIGNAL(changed()), SLOT(ImageWindow()));

  connect(ui.addProject, SIGNAL(clicked()), SLOT(NewProject()));
  connect(ui.removeProject, SIGNAL(clicked()), SLOT(removeProject()));

  connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(StopRun()));
  //     connect(ui.action_Test, SIGNAL(triggered(bool)), SLOT(Test()));

  TopMenuEnableDisable();
}

MainWindow::~MainWindow() {
  // Free Memory
  QMap<int, DATA *>::const_iterator i = projects->constBegin();
  while (i != projects->constEnd()) {
    i.value()->~DATA();
    ++i;
  }
  // Delete all projects
  delete projects;
#ifdef DEBUG
  qDebug() << "Closing MainWindow";
#endif
}
