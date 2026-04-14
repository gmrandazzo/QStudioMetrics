#include <QDateTime>
#include <QHostInfo>
/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Core data structure or utility for data.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "qsmdata.h"
#include <QBuffer>
#include <QByteArray>
#include <QDir>
#include <QFile>
#include <QFileDevice>
#include <QFileInfo>
#include <QIODevice>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVariant>
#include <memory>

#ifdef DEBUG
#include <QDebug>
#endif

#include "DATAIO.h"
#include "MainWindow.h"
#include "Dialogs/GenericProgressDialog.h"
// #include "ProgressBarDialog.h"
#include "qstudiometricsdataoperations.h"
#include "qstudiometricstypes.h"

void DATA::WriteList(QStringList &lst, QString fname) {
  QFile file(fname);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return;

  QTextStream out(&file);
  for (int i = 0; i < lst.size(); i++) {
    out << lst[i].toUtf8().data() << "\n";
  }
  file.close();
}

void DATA::GenNameLst(uint n, QString name, QStringList &namelst) {
  for (uint i = 0; i < n; i++) {
    namelst.append(name + QString("%0").arg(i + 1));
  }
}

// static void DATA::ImportObjectNames(FILEDATA f, QStringList &objname)
void DATA::ImportRows(QString fname, QStringList &rowlst) {
  QFile file(fname);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  while (!in.atEnd()) {
    QString name = QString::fromUtf8(in.readLine().trimmed().toUtf8());
    if (name.isEmpty())
      continue;
    else
      rowlst.append(name);
  }

  file.close();
}

// static void DATA::ImportVarNames(FILEDATA f, QStringList &varname)
// collst.append(firstcol_name);
void DATA::ImportColumns(QString fname, QString separator,
                         QStringList &collst) {
  QFile file(fname);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  while (!in.atEnd()) {
    QStringList list =
        in.readLine().trimmed().split(separator, Qt::SkipEmptyParts);
    for (int i = 0; i < list.size(); i++)
      if (list[i].isEmpty())
        continue;
      else
        collst.append(QString::fromUtf8(list[i].toUtf8()));
  }
  file.close();
}

bool DATA::CopyFile(const QString &sourceFile, const QString &destinationDir) {
  QFileInfo fileInfo(sourceFile);
  QString destinationFile =
      destinationDir + QDir::separator() + fileInfo.fileName();
  bool result = QFile::copy(sourceFile, destinationFile);
  return result;
}

void DATA::ImportFileMatrix(const FILEDATA &f) {
  if (f.datatype == 1) { // import matrix
    addMatrix();
    DATAIO::ImportMatrix(f.filename.toUtf8().data(), f.separator.toStdString(),
                         getMatrix(MatrixCount() - 1)->Matrix());
    if (f.fileobjname.isEmpty()) {
      GenNameLst(getMatrix(MatrixCount() - 1)->Matrix()->row, "Obj",
                 getMatrix(MatrixCount() - 1)->getObjName());
    } else {
      ImportRows(f.fileobjname, getMatrix(MatrixCount() - 1)->getObjName());
    }

    if (f.filevarname.isEmpty()) {
      getMatrix(MatrixCount() - 1)->getVarName().append(QString(firstcol_name));
      GenNameLst(getMatrix(MatrixCount() - 1)->Matrix()->col, "Var",
                 getMatrix(MatrixCount() - 1)->getVarName());
    } else {
      getMatrix(MatrixCount() - 1)->getVarName().append(QString(firstcol_name));
      ImportColumns(f.filevarname, f.separator,
                    getMatrix(MatrixCount() - 1)->getVarName());
      if (getMatrix(MatrixCount() - 1)->getVarName().size() == 0) {
        getMatrix(MatrixCount() - 1)
            ->getVarName()
            .append(QString(firstcol_name));
        GenNameLst(getMatrix(MatrixCount() - 1)->Matrix()->col, "Var",
                   getMatrix(MatrixCount() - 1)->getVarName());
      }
    }

    if (MatrixCount() - 2 > 0) {
      QString label = f.label;
      int i = 0;
      int id = 1;
      while (i < MatrixCount() - 2) {
        if (getMatrix(i)->getName().compare(label) == 0) {
          label = f.label + "_" + QString::number(id);
          i = 0;
          id += 1;
        }
        i++;
      }
      getMatrix(MatrixCount() - 1)->setName(label);
    } else {
      getMatrix(MatrixCount() - 1)->setName(f.label);
    }
#ifdef DEBUG
    qDebug() << getMatrix(MatrixCount() - 1)->getName();
#endif
  }
}

void DATA::ImportFileArray(const FILEDATA &f) {
  if (f.datatype == 2) { // import array
    addArray();

    DATAIO::ImportTensor(f.filename.toUtf8().data(), f.separator.toStdString(),
                         getArray(ArrayCount() - 1)->Array());

    if (f.fileobjname.isEmpty()) {
      GenNameLst(getArray(ArrayCount() - 1)->Array()->m[0]->row, "Obj",
                 getArray(ArrayCount() - 1)->getObjName());
    } else {
      ImportRows(f.fileobjname, getArray(ArrayCount() - 1)->getObjName());
    }

    if (f.filevarname.isEmpty()) {
      getArray(ArrayCount() - 1)->getVarName().append(QString(firstcol_name));
      GenNameLst(getArray(ArrayCount() - 1)->Array()->m[0]->col, "Var",
                 getArray(ArrayCount() - 1)->getVarName());
    } else {
      getArray(ArrayCount() - 1)->getVarName().append(QString(firstcol_name));
      ImportColumns(f.filevarname, f.separator,
                    getArray(ArrayCount() - 1)->getVarName());
      if (getArray(ArrayCount() - 1)->getVarName().size() == 0) {
        getArray(ArrayCount() - 1)->getVarName().append(QString(firstcol_name));
        GenNameLst(getArray(ArrayCount() - 1)->Array()->m[0]->col, "Var",
                   getArray(ArrayCount() - 1)->getVarName());
      }
    }

    if (ArrayCount() - 2 > 0) {
      QString label = f.label;
      int i = 0;
      int id = 1;
      while (i < ArrayCount() - 2) {
        if (getArray(i)->getName().compare(label) == 0) {
          label = f.label + "_" + QString::number(id);
          i = 0;
          id += 1;
        }
        i++;
      }
      getArray(ArrayCount() - 1)->setName(label);
    } else {
      getArray(ArrayCount() - 1)->setName(f.label);
    }
  }
}

bool DATA::isSQLDatabase(QString sqlfile) {
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(sqlfile);
  if (db.open() == true) {
    QSqlQuery query = QSqlQuery(db);
    query.exec("PRAGMA table_info(pcaTable)");
    int i = 0;
    while (query.next()) {
      // fields << query.value(1).toString();
      i++;
    }

    if (i > 0) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}
void DATA::OpenSQLData(QString sqlfile, QTreeWidget *treeWidget, int *tabcount_,
                       int *mid_, QStringList *log) {
  /* Warning!! there are some Methods from MainWindow and they works only if the
   * ui is started... */
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(sqlfile);
  if (db.open()) {
    projectpath = sqlfile;
    QSqlQuery query = QSqlQuery(db);
    query.exec("SELECT * from matrixTable");
    while (query.next()) {
      // get the query values
      QString name = query.value(0).toString();
      QString s_objname = query.value(1).toString();
      QString s_varname = query.value(2).toString();
      QString s_matrix = query.value(3).toString();
      // add the data matrix
      addMatrix();
      getMatrix(MatrixCount() - 1)->setName(name);
      getMatrix(MatrixCount() - 1)->getObjName() =
          DeserializeQStringList(s_objname);
      getMatrix(MatrixCount() - 1)->getVarName() =
          DeserializeQStringList(s_varname);
      DeserializeMatrix(s_matrix, getMatrix(MatrixCount() - 1)->Matrix());

      // Add matrix to the treeview
      if (treeWidget) {
        auto subitem = std::make_unique<QTreeWidgetItem>();
        subitem->setText(0, name); /*set the data name from the file*/
        subitem->setText(1, QString("Matrix")); // Define the type of the data
        subitem->setText(
            2, QString::number((*tabcount_))); // Define the tab id number in
                                               // order to close a specific
                                               // table
        subitem->setText(
            3, QString::number(MatrixCount() -
                               1)); // Define the matrix position id in order to
                                    // find easly when you need to show data.
        subitem->setText(
            4,
            QString::number(
                getProjectID())); // pid for get the tensor with Value
        MainWindow::getProjectItem(getProjectID(), treeWidget)
            ->child(0)
            ->addChild(subitem.release());
      }
      (*tabcount_)++;
      (*log).append(QString("Matrix %1 imported.\n").arg(name));
    }

    query.exec("SELECT * from arrayTable");
    while (query.next()) {
      // get the query values
      QString name = query.value(0).toString();
      QString s_objname = query.value(1).toString();
      QString s_varname = query.value(2).toString();
      QString s_tensor = query.value(3).toString();
      // add the data matrix
      addArray();
      getArray(ArrayCount() - 1)->setName(name);
      getArray(ArrayCount() - 1)->getObjName() =
          DeserializeQStringList(s_objname);
      getArray(ArrayCount() - 1)->getVarName() =
          DeserializeQStringList(s_varname);
      DeserializeTensor(s_tensor, getArray(ArrayCount() - 1)->Array());

      // Add matrix to the treeview
      if (treeWidget) {
        auto subitem = std::make_unique<QTreeWidgetItem>();
        subitem->setText(0, name); /*set the data name from the file*/
        subitem->setText(1, QString("Array")); // Define the type of the data
        subitem->setText(
            2, QString::number((*tabcount_))); // Define the tab id number in
                                               // order to close a specific
                                               // table
        subitem->setText(
            3, QString::number(MatrixCount() -
                               1)); // Define the matrix position id in order to
                                    // find easly when you need to show data.
        subitem->setText(
            4,
            QString::number(
                getProjectID())); // pid for get the tensor with Value
        MainWindow::getProjectItem(getProjectID(), treeWidget)
            ->child(0)
            ->addChild(subitem.release());
      }
      (*tabcount_)++;
      (*log).append(QString("Array %1 imported.\n").arg(name));
    }

    query.exec("SELECT * from objlabelTable");
    while (query.next()) {
      // get the query values
      QString name = query.value(0).toString();
      QString s_values = query.value(1).toString();
      getObjectLabels().append(LABEL());
      getObjectLabels().last().name = name;
      getObjectLabels().last().objects = DeserializeQStringList(s_values);
      (*log).append(QString("Object label %1 imported.\n").arg(name));
    }

    query.exec("SELECT * from varlabelTable");
    while (query.next()) {
      // get the query values
      QString name = query.value(0).toString();
      QString s_values = query.value(1).toString();
      getVariableLabels().append(LABEL());
      getVariableLabels().last().name = name;
      getVariableLabels().last().objects = DeserializeQStringList(s_values);
      (*log).append(QString("Variable label %1 imported.\n").arg(name));
    }

    query.exec("SELECT * from vartablabelTable");
    while (query.next()) {
      // get the query values
      QString name = query.value(0).toString();
      QString s_objname = query.value(1).toString();
      QString s_varname = query.value(2).toString();
      QString s_matrix = query.value(3).toString();

      getVariableTabLabels().append(new TABLABEL());
      getVariableTabLabels().last()->setName(name);
      getVariableTabLabels().last()->getObjectsName() =
          DeserializeQStringList(s_objname);
      getVariableTabLabels().last()->getFeaturesName() =
          DeserializeQStringList(s_varname);
      matrix *tmp = getVariableTabLabels().last()->getMatrix();
      DeserializeMatrix(s_matrix, tmp);

      (*log).append(QString("Variable table label %1 imported.\n").arg(name));
    }

    query.exec("SELECT * from imgTable");
    while (query.next()) {
      QString name = query.value(0).toString();
      QByteArray outByteArray = query.value(1).toByteArray();
      getImages().append(IMAGE());
      getImages().last().name = name;
      // getImages().last().filepath = fileInfo.absoluteFilePath();
      getImages().last().image.loadFromData(outByteArray);
    }

    //(name TEXT, pcahash TEXT, hashinputmx TEXT, objname TEXT, scores TEXT)
    query.exec("SELECT * from pcapredTable");
    QList<QSqlRecord> pcapredlist;

    while (query.next()) {
      pcapredlist.append(query.record());
    }

    query.exec("SELECT * from pcaTable");
    while (query.next()) {
      // get the query values by name
      QString name = query.value("name").toString();
      int npc = query.value("numcomp").toInt();
      int xscaling = query.value("scalingtype").toInt();
      QString hashinputmx = query.value("hashinputmx").toString();
      QString s_objname = query.value("objname").toString();
      QString s_varname = query.value("varname").toString();
      QString s_scores = query.value("scores").toString();
      QString s_loadings = query.value("loadings").toString();
      QString s_dmodx = query.value("dmodx").toString();
      QString s_varexp = query.value("varexp").toString();
      QString s_colscaling = query.value("colscaling").toString();
      QString s_colaverage = query.value("colaverage").toString();

      // add the data matrix
      addPCAModel();
      getLastPCAModel()->setModelID((*mid_));
      getLastPCAModel()->setName(name);
      getLastPCAModel()->setNPC(npc);
      getLastPCAModel()->setXScaling(xscaling);
      getLastPCAModel()->setDataHash(hashinputmx);
      getLastPCAModel()->getObjName() = DeserializeQStringList(s_objname);
      getLastPCAModel()->getVarName() = DeserializeQStringList(s_varname);
      if (!getLastPCAModel()->Model()->scores)
        initMatrix(&getLastPCAModel()->Model()->scores);
      if (!getLastPCAModel()->Model()->loadings)
        initMatrix(&getLastPCAModel()->Model()->loadings);
      if (!getLastPCAModel()->Model()->dmodx)
        initMatrix(&getLastPCAModel()->Model()->dmodx);
      DeserializeMatrix(s_scores, getLastPCAModel()->Model()->scores);
      DeserializeMatrix(s_loadings, getLastPCAModel()->Model()->loadings);
      DeserializeMatrix(s_dmodx, getLastPCAModel()->Model()->dmodx);
      DeserializeDVector(s_colscaling, getLastPCAModel()->Model()->colscaling);
      DeserializeDVector(s_colaverage, getLastPCAModel()->Model()->colaverage);
      DeserializeDVector(s_varexp, getLastPCAModel()->Model()->varexp);

      int xid = -1;
      for (int j = 0; j < MatrixCount(); j++) {
        if (getMatrix(j)->getHash().compare(hashinputmx) == 0) {
          xid = j;
          break;
        }
      }

      if (xid == -1) {
        (*log).append(
            QString(
                "Warning on PCA Model %1. Unable to find origin data matrix.")
                .arg(name));
      }

      getLastPCAModel()->setDID(xid);

      if (treeWidget) {
        auto subitem = std::make_unique<QTreeWidgetItem>();
        subitem->setText(0, getLastPCAModel()->getName());
        subitem->setText(1, QString::number((*tabcount_)));
        subitem->setText(2, QString::number(getProjectID()));
        subitem->setText(3, getLastPCAModel()->getDataHash());
        subitem->setText(4, QString("-"));
        subitem->setText(5, QString::number(getLastPCAModel()->getXScaling()));
        subitem->setText(6, QString("-"));
        subitem->setText(7, QString::number(getLastPCAModel()->getNPC()));
        subitem->setText(8, QString("PCA Model"));
        subitem->setText(9, QString::number((*mid_)));
        MainWindow::getProjectItem(getProjectID(), treeWidget)
            ->child(1)
            ->addChild(subitem.release());
      }
      if (tabcount_)
        (*tabcount_)++;
      (*log).append(QString("PCA model %1 imported.\n").arg(name));

      QString pcahash = getLastPCAModel()->getHash();
      for (int i = 0; i < pcapredlist.size(); i++) {
        if (pcapredlist[i].value("pcahash").toString().compare(pcahash) == 0) {
          getLastPCAModel()->addPCAPrediction();
          getLastPCAModel()->getLastPCAPrediction()->setPredID(
              getLastPCAModel()->PCAPredictionCount() - 1);
          getLastPCAModel()->getLastPCAPrediction()->setName(
              pcapredlist[i].value("name").toString());
          getLastPCAModel()->getLastPCAPrediction()->setDataHash(
              pcapredlist[i].value("hashinputmx").toString());
          getLastPCAModel()->getLastPCAPrediction()->getObjName() =
              DeserializeQStringList(
                  pcapredlist[i].value("objname").toString());
          DeserializeMatrix(
              pcapredlist[i].value("scores").toString(),
              getLastPCAModel()->getLastPCAPrediction()->getPredScores());

          int predid = -1;
          for (int k = 0; k < MatrixCount(); k++) {
            if (getMatrix(k)->getHash().compare(
                    getLastPCAModel()->getLastPCAPrediction()->getDataHash()) ==
                0) {
              predid = k;
              break;
            }
          }

          if (predid == -1) {
            (*log).append(QString("Warning on PCA Prediction %1. Unable to "
                                  "find the origin data matrix.")
                              .arg(getLastPCAModel()
                                       ->getLastPCAPrediction()
                                       ->getName()));
          }

          getLastPCAModel()->getLastPCAPrediction()->setDID(predid);
          if (treeWidget) {
            auto preditem = std::make_unique<QTreeWidgetItem>();
            preditem->setText(
                0, getLastPCAModel()->getLastPCAPrediction()->getName());
            preditem->setText(1, QString::number((*tabcount_)));
            preditem->setText(2, QString::number(getProjectID()));
            preditem->setText(3, QString::number((*mid_)));
            preditem->setText(
                4, getLastPCAModel()->getLastPCAPrediction()->getDataHash());
            preditem->setText(5, "-");
            preditem->setText(
                6,
                QString::number(
                    getLastPCAModel()->getLastPCAPrediction()->getPredID()));
            preditem->setText(7, QString("PCA Prediction"));
            MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)
                ->addChild(preditem.release());
          }
          if (tabcount_)
            (*tabcount_)++;
        }
      }
      (*mid_)++;
    
    query.exec("SELECT * from auditTable");
    while (query.next()) {
      AUDIT_ENTRY entry;
      entry.timestamp = query.value(0).toString();
      entry.action = query.value(1).toString();
      entry.details = query.value(2).toString();
      entry.user = query.value(3).toString();
      audit_trail.append(entry);
    }

    }

    query.exec("SELECT * from plspredTable");
    QList<QSqlRecord> plspredlist;
    while (query.next()) {
      plspredlist.append(query.record());
    }

    query.exec("SELECT * from plsTable");
    while (query.next()) {
      // get the query values by name for robustness
      QString name = query.value("name").toString();
      int npc = query.value("nlvs").toInt();
      int xscaling = query.value("xscalingtype").toInt();
      int yscaling = query.value("yscalingtype").toInt();
      QString hashinputmx = query.value("hashinputmx").toString();
      QString s_objname = query.value("objname").toString();
      QString s_xvarname = query.value("xvarname").toString();
      QString s_yvarname = query.value("yvarname").toString();
      QString s_classes = query.value("classes").toString();
      QString s_tscores = query.value("tscores").toString();
      QString s_ploadings = query.value("ploadings").toString();
      QString s_weights = query.value("weights").toString();
      QString s_xvarexp = query.value("xvarexp").toString();
      QString s_xcolscaling = query.value("xcolscaling").toString();
      QString s_xcolaverage = query.value("xcolaverage").toString();
      QString s_uscores = query.value("uscores").toString();
      QString s_qloadings = query.value("qloadings").toString();
      QString s_ycolscaling = query.value("ycolscaling").toString();
      QString s_ycolaverage = query.value("ycolaverage").toString();
      QString s_b = query.value("b").toString();
      QString s_r2y_model = query.value("r2y_model").toString();
      QString s_sdec = query.value("sdec").toString();
      QString s_recalc_y = query.value("recalc_y").toString();
      QString s_recalc_residuals = query.value("recalc_residuals").toString();

      // Handle legacy databases without algtype
      int algtype = PLS_;
      if (query.record().contains("algtype")) {
        algtype = query.value("algtype").toInt();
      } else {
        // Heuristic: if we have ROC data, it's likely a PLS-DA model
        if (!query.value("roc_auc_recalculated").toString().isEmpty() &&
            query.value("roc_auc_recalculated").toString() != "NULL") {
          algtype = PLS_DA_;
        }
      }

      int validationtype = query.value("validationtype").toInt();
      QString s_q2y = query.value("q2y").toString();
      QString s_sdep = query.value("sdep").toString();
      QString s_bias = query.value("bias").toString();
      QString s_predicted_y = query.value("predicted_y").toString();
      QString s_predicted_residuals =
          query.value("predicted_residuals").toString();
      QString s_roc_recalculated = query.value("roc_recalculated").toString();
      QString s_roc_validation = query.value("roc_validation").toString();
      QString s_roc_auc_recalculated =
          query.value("roc_auc_recalculated").toString();
      QString s_roc_auc_validation =
          query.value("roc_auc_validation").toString();
      QString s_precision_recall_recalculated =
          query.value("precision_recall_recalculated").toString();
      QString s_precision_recall_validation =
          query.value("precision_recall_validation").toString();
      QString s_precision_recall_ap_recalculated =
          query.value("precision_recall_ap_recalculated").toString();
      QString s_precision_recall_ap_validation =
          query.value("precision_recall_ap_validation").toString();
      QString s_yscrambling = query.value("yscrambling").toString();

      // add the data matrix
      addPLSModel();
      getLastPLSModel()->setModelID((*mid_));
      getLastPLSModel()->setName(name);
      getLastPLSModel()->setNPC(npc);
      getLastPLSModel()->setXScaling(xscaling);
      getLastPLSModel()->setYScaling(yscaling);
      getLastPLSModel()->setDataHash(hashinputmx);
      getLastPLSModel()->getObjName() = DeserializeQStringList(s_objname);
      getLastPLSModel()->setXVarName(DeserializeQStringList(s_xvarname));
      getLastPLSModel()->setYVarName(DeserializeQStringList(s_yvarname));
      getLastPLSModel()->setClasses(DeserializeLABELS(s_classes));
      getLastPLSModel()->setAlgorithm(algtype);

      if (!getLastPLSModel()->Model()->xscores)
        initMatrix(&getLastPLSModel()->Model()->xscores);
      if (!getLastPLSModel()->Model()->xloadings)
        initMatrix(&getLastPLSModel()->Model()->xloadings);
      if (!getLastPLSModel()->Model()->xweights)
        initMatrix(&getLastPLSModel()->Model()->xweights);
      if (!getLastPLSModel()->Model()->xvarexp)
        initDVector(&getLastPLSModel()->Model()->xvarexp);
      if (!getLastPLSModel()->Model()->xcolaverage)
        initDVector(&getLastPLSModel()->Model()->xcolaverage);
      if (!getLastPLSModel()->Model()->xcolscaling)
        initDVector(&getLastPLSModel()->Model()->xcolscaling);
      if (!getLastPLSModel()->Model()->yscores)
        initMatrix(&getLastPLSModel()->Model()->yscores);
      if (!getLastPLSModel()->Model()->yloadings)
        initMatrix(&getLastPLSModel()->Model()->yloadings);
      if (!getLastPLSModel()->Model()->ycolaverage)
        initDVector(&getLastPLSModel()->Model()->ycolaverage);
      if (!getLastPLSModel()->Model()->ycolscaling)
        initDVector(&getLastPLSModel()->Model()->ycolscaling);
      if (!getLastPLSModel()->Model()->b)
        initDVector(&getLastPLSModel()->Model()->b);
      if (!getLastPLSModel()->Model()->r2y_recalculated)
        initMatrix(&getLastPLSModel()->Model()->r2y_recalculated);
      if (!getLastPLSModel()->Model()->sdec)
        initMatrix(&getLastPLSModel()->Model()->sdec);
      if (!getLastPLSModel()->Model()->recalculated_y)
        initMatrix(&getLastPLSModel()->Model()->recalculated_y);
      if (!getLastPLSModel()->Model()->recalc_residuals)
        initMatrix(&getLastPLSModel()->Model()->recalc_residuals);
      if (!getLastPLSModel()->Model()->q2y)
        initMatrix(&getLastPLSModel()->Model()->q2y);
      if (!getLastPLSModel()->Model()->sdep)
        initMatrix(&getLastPLSModel()->Model()->sdep);
      if (!getLastPLSModel()->Model()->bias)
        initMatrix(&getLastPLSModel()->Model()->bias);
      if (!getLastPLSModel()->Model()->predicted_y)
        initMatrix(&getLastPLSModel()->Model()->predicted_y);
      if (!getLastPLSModel()->Model()->pred_residuals)
        initMatrix(&getLastPLSModel()->Model()->pred_residuals);
      if (!getLastPLSModel()->Model()->roc_recalculated)
        initTensor(&getLastPLSModel()->Model()->roc_recalculated);
      if (!getLastPLSModel()->Model()->roc_validation)
        initTensor(&getLastPLSModel()->Model()->roc_validation);
      if (!getLastPLSModel()->Model()->roc_auc_recalculated)
        initMatrix(&getLastPLSModel()->Model()->roc_auc_recalculated);
      if (!getLastPLSModel()->Model()->roc_auc_validation)
        initMatrix(&getLastPLSModel()->Model()->roc_auc_validation);
      if (!getLastPLSModel()->Model()->precision_recall_recalculated)
        initTensor(&getLastPLSModel()->Model()->precision_recall_recalculated);
      if (!getLastPLSModel()->Model()->precision_recall_validation)
        initTensor(&getLastPLSModel()->Model()->precision_recall_validation);
      if (!getLastPLSModel()->Model()->precision_recall_ap_recalculated)
        initMatrix(&getLastPLSModel()->Model()->precision_recall_ap_recalculated);
      if (!getLastPLSModel()->Model()->precision_recall_ap_validation)
        initMatrix(&getLastPLSModel()->Model()->precision_recall_ap_validation);
      if (!getLastPLSModel()->Model()->yscrambling)
        initMatrix(&getLastPLSModel()->Model()->yscrambling);

      DeserializeMatrix(s_tscores, getLastPLSModel()->Model()->xscores);
      DeserializeMatrix(s_ploadings, getLastPLSModel()->Model()->xloadings);
      DeserializeMatrix(s_weights, getLastPLSModel()->Model()->xweights);
      DeserializeDVector(s_xvarexp, getLastPLSModel()->Model()->xvarexp);
      DeserializeDVector(s_xcolaverage, getLastPLSModel()->Model()->xcolaverage);
      DeserializeDVector(s_xcolscaling, getLastPLSModel()->Model()->xcolscaling);
      DeserializeMatrix(s_uscores, getLastPLSModel()->Model()->yscores);
      DeserializeMatrix(s_qloadings, getLastPLSModel()->Model()->yloadings);
      DeserializeDVector(s_ycolaverage, getLastPLSModel()->Model()->ycolaverage);
      DeserializeDVector(s_ycolscaling, getLastPLSModel()->Model()->ycolscaling);
      DeserializeDVector(s_b, getLastPLSModel()->Model()->b);
      DeserializeMatrix(s_r2y_model,
                        getLastPLSModel()->Model()->r2y_recalculated);
      DeserializeMatrix(s_sdec, getLastPLSModel()->Model()->sdec);
      DeserializeMatrix(s_recalc_y, getLastPLSModel()->Model()->recalculated_y);
      DeserializeMatrix(s_recalc_residuals,
                        getLastPLSModel()->Model()->recalc_residuals);
      getLastPLSModel()->setValidation(validationtype);
      DeserializeMatrix(s_q2y, getLastPLSModel()->Model()->q2y);
      DeserializeMatrix(s_sdep, getLastPLSModel()->Model()->sdep);
      DeserializeMatrix(s_bias, getLastPLSModel()->Model()->bias);
      DeserializeMatrix(s_predicted_y, getLastPLSModel()->Model()->predicted_y);
      DeserializeMatrix(s_predicted_residuals,
                        getLastPLSModel()->Model()->pred_residuals);
      DeserializeTensor(s_roc_recalculated,
                        getLastPLSModel()->Model()->roc_recalculated);
      DeserializeTensor(s_roc_validation,
                        getLastPLSModel()->Model()->roc_validation);
      DeserializeMatrix(s_roc_auc_recalculated,
                        getLastPLSModel()->Model()->roc_auc_recalculated);
      DeserializeMatrix(s_roc_auc_validation,
                        getLastPLSModel()->Model()->roc_auc_validation);
      DeserializeTensor(
          s_precision_recall_recalculated,
          getLastPLSModel()->Model()->precision_recall_recalculated);
      DeserializeTensor(
          s_precision_recall_validation,
          getLastPLSModel()->Model()->precision_recall_validation);
      DeserializeMatrix(
          s_precision_recall_ap_recalculated,
          getLastPLSModel()->Model()->precision_recall_ap_recalculated);
      DeserializeMatrix(
          s_precision_recall_ap_validation,
          getLastPLSModel()->Model()->precision_recall_ap_validation);
      DeserializeMatrix(s_yscrambling, getLastPLSModel()->Model()->yscrambling);

      int xid = -1;
      for (int j = 0; j < MatrixCount(); j++) {
        if (getMatrix(j)->getHash().compare(hashinputmx) == 0) {
          xid = j;
          break;
        }
      }

      if (xid == -1) {
        (*log).append(QString("Warning on PLS Model %1. Unable to find the "
                              "origin data matrix.")
                          .arg(name));
      }

      getLastPLSModel()->setDID(xid);
      if (treeWidget) {
        auto subitem = std::make_unique<QTreeWidgetItem>();
        subitem->setText(0, getLastPLSModel()->getName());
        subitem->setText(1, QString::number((*tabcount_)));
        subitem->setText(2, QString::number(getProjectID()));
        subitem->setText(3, getLastPLSModel()->getDataHash());
        subitem->setText(4, getLastPLSModel()->getDataHash());
        subitem->setText(5, QString::number(getLastPLSModel()->getXScaling()));
        subitem->setText(6, QString::number(getLastPLSModel()->getYScaling()));
        subitem->setText(7, QString::number(getLastPLSModel()->getNPC()));
        subitem->setText(8, QString("PLS Model"));
        subitem->setText(9, QString::number((*mid_)));
        MainWindow::getProjectItem(getProjectID(), treeWidget)
            ->child(1)
            ->addChild(subitem.release());
      }
      if (tabcount_)
        (*tabcount_)++;
      (*log).append(QString("PLS model %1 imported.\n").arg(name));

      QString plshash = getLastPLSModel()->getHash();

      for (int i = 0; i < plspredlist.size(); i++) {
        if (plspredlist[i].value("plshash").toString().compare(plshash) == 0) {
          getLastPLSModel()->addPLSPrediction();
          getLastPLSModel()->getLastPLSPrediction()->setPredID(
              getLastPLSModel()->PLSPredictionCount() - 1);
          getLastPLSModel()->getLastPLSPrediction()->setName(
              plspredlist[i].value("name").toString());
          getLastPLSModel()->getLastPLSPrediction()->setDataHash(
              plspredlist[i].value("hashinputmx").toString());
          getLastPLSModel()->getLastPLSPrediction()->getObjName() =
              DeserializeQStringList(plspredlist[i].value("objname").toString());
          getLastPLSModel()->getLastPLSPrediction()->getYVarName() =
              DeserializeQStringList(
                  plspredlist[i].value("yvarname").toString());
          DeserializeMatrix(
              plspredlist[i].value("tscores").toString(),
              getLastPLSModel()->getLastPLSPrediction()->getXPredScores());
          DeserializeMatrix(
              plspredlist[i].value("predicted_y").toString(),
              getLastPLSModel()->getLastPLSPrediction()->getYDipVar());
          DeserializeMatrix(
              plspredlist[i].value("r2y").toString(),
              getLastPLSModel()->getLastPLSPrediction()->getR2Y());
          DeserializeMatrix(
              plspredlist[i].value("sdec").toString(),
              getLastPLSModel()->getLastPLSPrediction()->getSDEC());

          int predid = -1;
          for (int k = 0; k < MatrixCount(); k++) {
            if (getMatrix(k)->getHash().compare(
                    getLastPLSModel()->getLastPLSPrediction()->getDataHash()) ==
                0) {
              predid = k;
              break;
            } else {
              continue;
            }
          }

          if (predid == -1) {
            (*log).append(QString("Warning on PLS Prediction %1. Unable to "
                                  "find the origin data matrix.")
                              .arg(getLastPLSModel()
                                       ->getLastPLSPrediction()
                                       ->getName()));
          }

          getLastPLSModel()->getLastPLSPrediction()->setDID(predid);
          if (treeWidget) {
            auto preditem = std::make_unique<QTreeWidgetItem>();
            preditem->setText(
                0, getLastPLSModel()->getLastPLSPrediction()->getName());
            preditem->setText(1, QString::number((*tabcount_)));
            preditem->setText(2, QString::number(getProjectID()));
            preditem->setText(3, QString::number((*mid_)));
            preditem->setText(
                4, getLastPLSModel()->getLastPLSPrediction()->getDataHash());
            preditem->setText(
                5, getLastPLSModel()->getLastPLSPrediction()->getDataHash());
            preditem->setText(
                6,
                QString::number(
                    getLastPLSModel()->getLastPLSPrediction()->getPredID()));
            preditem->setText(7, QString("PLS Prediction"));
            MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)
                ->addChild(preditem.release());
          }
          if (tabcount_)
            (*tabcount_)++;
        } else {
          continue;
        }
      }
      (*mid_)++;
    
    query.exec("SELECT * from auditTable");
    while (query.next()) {
      AUDIT_ENTRY entry;
      entry.timestamp = query.value(0).toString();
      entry.action = query.value(1).toString();
      entry.details = query.value(2).toString();
      entry.user = query.value(3).toString();
      audit_trail.append(entry);
    }

    }

    query.exec("SELECT * from mlrpredTable");
    QList<QSqlRecord> mlrpredlist;
    while (query.next()) {
      mlrpredlist.append(query.record());
    }

    query.exec("SELECT * from mlrTable");
    while (query.next()) {
      // get the query values by name
      QString name = query.value("name").toString();
      QString hashinputmx = query.value("hashinputmx").toString();
      QString serialized_objname = query.value("objname").toString();
      QString serialized_xvarname = query.value("xvarname").toString();
      QString serialized_yvarname = query.value("yvarname").toString();
      QString serialized_b = query.value("b").toString();
      QString serialized_r2y = query.value("r2y_model").toString();
      QString serialized_sdec = query.value("sdec").toString();
      QString serialized_recalc_y = query.value("recalc_y").toString();
      QString serialized_recalc_residuals =
          query.value("recalc_residuals").toString();
      int validationtype = query.value("validationtype").toInt();
      QString serialized_ymean = query.value("ymean").toString();
      QString serialized_q2y = query.value("q2y").toString();
      QString serialized_sdep = query.value("sdep").toString();
      QString serialized_bias = query.value("bias").toString();
      QString serialized_predicted_y = query.value("predicted_y").toString();
      QString serialized_predicted_residuals =
          query.value("predicted_residuals").toString();
      QString serialized_yscrambling = query.value("yscrambling").toString();

      addMLRModel();
      getLastMLRModel()->setModelID((*mid_));
      getLastMLRModel()->setName(name);
      getLastMLRModel()->getObjName() =
          DeserializeQStringList(serialized_objname);
      getLastMLRModel()->getXVarName() =
          DeserializeQStringList(serialized_xvarname);
      getLastMLRModel()->getYVarName() =
          DeserializeQStringList(serialized_yvarname);
      DeserializeMatrix(serialized_b, getLastMLRModel()->Model()->b);
      DeserializeDVector(serialized_r2y, getLastMLRModel()->Model()->r2y_model);
      DeserializeDVector(serialized_sdec, getLastMLRModel()->Model()->sdec);
      DeserializeMatrix(serialized_recalc_y,
                        getLastMLRModel()->Model()->recalculated_y);
      DeserializeMatrix(serialized_recalc_residuals,
                        getLastMLRModel()->Model()->recalc_residuals);
      DeserializeDVector(serialized_ymean, getLastMLRModel()->Model()->ymean);
      DeserializeDVector(serialized_q2y, getLastMLRModel()->Model()->q2y);
      DeserializeDVector(serialized_sdep, getLastMLRModel()->Model()->sdep);
      DeserializeDVector(serialized_bias, getLastMLRModel()->Model()->bias);
      DeserializeMatrix(serialized_predicted_y,
                        getLastMLRModel()->Model()->predicted_y);
      DeserializeMatrix(serialized_predicted_residuals,
                        getLastMLRModel()->Model()->pred_residuals);
      DeserializeMatrix(serialized_yscrambling,
                        getLastMLRModel()->Model()->r2q2scrambling);
      getLastMLRModel()->setValidation(validationtype);

      int did = -1;
      for (int j = 0; j < MatrixCount(); j++) {
        if (getMatrix(j)->getHash().compare(hashinputmx) == 0) {
          did = j;
        }
      }

      if (did == -1) {
        (*log).append(QString("Warning on MLR Model %1. Unable to find the "
                              "origin data matrix.")
                          .arg(name));
      }

      getLastMLRModel()->setDID(did);
      if (treeWidget) {
        auto subitem = std::make_unique<QTreeWidgetItem>();
        subitem->setText(0, name);
        subitem->setText(1, QString::number((*tabcount_)));
        subitem->setText(2, QString::number(getProjectID()));
        subitem->setText(3, getLastMLRModel()->getDataHash());
        subitem->setText(4, getLastMLRModel()->getDataHash());
        subitem->setText(5, "-");
        subitem->setText(6, "-");
        subitem->setText(7, "-");
        subitem->setText(8, QString("MLR Model"));
        subitem->setText(9, QString::number((*mid_)));
        MainWindow::getProjectItem(getProjectID(), treeWidget)
            ->child(1)
            ->addChild(subitem.release());
      }
      if (tabcount_)
        (*tabcount_)++;

      QString mlrhash = getLastMLRModel()->getHash();
      for (int i = 0; i < mlrpredlist.size(); i++) {
        if (mlrpredlist[i].value("mlrhash").toString().compare(mlrhash) == 0) {
          getLastMLRModel()->addMLRPrediction();
          getLastMLRModel()->getLastMLRPrediction()->setPredID(
              getLastMLRModel()->MLRPredictionCount() - 1);
          getLastMLRModel()->getLastMLRPrediction()->setName(
              mlrpredlist[i].value("name").toString());
          getLastMLRModel()->getLastMLRPrediction()->setDataHash(
              mlrpredlist[i].value("hashinputmx").toString());
          getLastMLRModel()->getLastMLRPrediction()->getObjName() =
              DeserializeQStringList(
                  mlrpredlist[i].value("objname").toString());
          getLastMLRModel()->getLastMLRPrediction()->getYVarName() =
              DeserializeQStringList(
                  mlrpredlist[i].value("yvarname").toString());
          DeserializeMatrix(
              mlrpredlist[i].value("ypred").toString(),
              getLastMLRModel()->getLastMLRPrediction()->getYDipVar());

          int preddid = -1;
          for (int k = 0; k < MatrixCount(); k++) {
            if (getMatrix(k)->getHash().compare(
                    getLastMLRModel()->getLastMLRPrediction()->getDataHash()) ==
                0) {
              preddid = k;
              break;
            }
          }

          if (preddid == -1) {
            (*log).append(QString("Warning on MLR Prediction %1. Unable to "
                                  "find origin data matrix.")
                              .arg(getLastMLRModel()
                                       ->getLastMLRPrediction()
                                       ->getName()));
          }

          getLastMLRModel()->getLastMLRPrediction()->setDID(preddid);
          if (treeWidget) {
            auto preditem = std::make_unique<QTreeWidgetItem>();
            preditem->setText(
                0, getLastMLRModel()->getLastMLRPrediction()->getName());
            preditem->setText(1, QString::number((*tabcount_)));
            preditem->setText(2, QString::number(getProjectID()));
            preditem->setText(3, QString::number((*mid_)));
            preditem->setText(
                4, getLastMLRModel()->getLastMLRPrediction()->getDataHash());
            preditem->setText(
                5, getLastMLRModel()->getLastMLRPrediction()->getDataHash());
            preditem->setText(
                6, QString::number(
                       getLastMLRModel()->getLastMLRPrediction()->getPredID()));
            preditem->setText(7, QString("MLR Prediction"));
            MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)
                ->addChild(preditem.release());
          }
          if (tabcount_)
            (*tabcount_)++;
        }
      }
      (*mid_)++;
    
    query.exec("SELECT * from auditTable");
    while (query.next()) {
      AUDIT_ENTRY entry;
      entry.timestamp = query.value(0).toString();
      entry.action = query.value(1).toString();
      entry.details = query.value(2).toString();
      entry.user = query.value(3).toString();
      audit_trail.append(entry);
    }

    }

    query.exec("SELECT * from ldapredTable");
    QList<QSqlRecord> ldapredlist;
    while (query.next()) {
      ldapredlist.append(query.record());
    }

    query.exec("SELECT * from ldaTable");
    while (query.next()) {
      // name, hashinputmx, objname, varname, validation, roc, roc_aucs, pr,
      // pr_aucs, recalculated_y, recalculated_residuals, predicted_y,
      // predicted_residuals, pprob, evect, eval, mu, mnpdf, features, fmean,
      // fsdev, inv_cov, nclass, class_start, classid, classes, nameclasses

      QString name = query.value("name").toString();
      QString hashinputmx = query.value("hashinputmx").toString();
      QString serialized_objname = query.value("objname").toString();
      QString serialized_varname = query.value("varname").toString();
      int validationtype = query.value("validationtype").toInt();
      QString serialized_roc = query.value("roc").toString();
      QString serialized_roc_aucs = query.value("roc_aucs").toString();
      QString serialized_pr = query.value("pr").toString();
      QString serialized_pr_aucs = query.value("pr_aucs").toString();
      QString serialized_recalculated_y = query.value("recalculated_y").toString();
      QString serialized_recalculated_residuals =
          query.value("recalculated_residuals").toString();
      QString serialized_predicted_y = query.value("predicted_y").toString();
      QString serialized_predicted_residuals =
          query.value("predicted_residuals").toString();
      QString serialized_prob = query.value("pprob").toString();
      QString serialized_evect = query.value("evect").toString();
      QString serialized_eval = query.value("eval").toString();
      QString serialized_mu = query.value("mu").toString();
      QString serialized_mnpdf = query.value("mnpdf").toString();
      QString serialized_features = query.value("features").toString();
      QString serialized_fmean = query.value("fmean").toString();
      QString serialized_fsdev = query.value("fsdev").toString();
      QString serialized_inv_cov = query.value("inv_cov").toString();
      int nclass = query.value("nclass").toInt();
      int class_start = query.value("class_start").toInt();
      QString serialized_classid = query.value("classid").toString();
      QString serialized_classes = query.value("classes").toString();
      QString serialized_nameclasses = query.value("nameclasses").toString();

      addLDAModel();
      getLastLDAModel()->setName(name);
      getLastLDAModel()->setModelID((*mid_));
      getLastLDAModel()->getObjName() =
          DeserializeQStringList(serialized_objname);
      getLastLDAModel()->getVarName() =
          DeserializeQStringList(serialized_varname);

      if (!getLastLDAModel()->Model()->roc)
        initTensor(&getLastLDAModel()->Model()->roc);
      if (!getLastLDAModel()->Model()->roc_aucs)
        initDVector(&getLastLDAModel()->Model()->roc_aucs);
      if (!getLastLDAModel()->Model()->pr)
        initTensor(&getLastLDAModel()->Model()->pr);
      if (!getLastLDAModel()->Model()->pr_aucs)
        initDVector(&getLastLDAModel()->Model()->pr_aucs);
      if (!getLastLDAModel()->Model()->recalculated_y)
        initMatrix(&getLastLDAModel()->Model()->recalculated_y);
      if (!getLastLDAModel()->Model()->recalculated_residuals)
        initMatrix(&getLastLDAModel()->Model()->recalculated_residuals);
      if (!getLastLDAModel()->Model()->predicted_y)
        initMatrix(&getLastLDAModel()->Model()->predicted_y);
      if (!getLastLDAModel()->Model()->predicted_residuals)
        initMatrix(&getLastLDAModel()->Model()->predicted_residuals);
      if (!getLastLDAModel()->Model()->pprob)
        initDVector(&getLastLDAModel()->Model()->pprob);
      if (!getLastLDAModel()->Model()->eval)
        initDVector(&getLastLDAModel()->Model()->eval);
      if (!getLastLDAModel()->Model()->evect)
        initMatrix(&getLastLDAModel()->Model()->evect);
      if (!getLastLDAModel()->Model()->mu)
        initMatrix(&getLastLDAModel()->Model()->mu);
      if (!getLastLDAModel()->Model()->mnpdf)
        initTensor(&getLastLDAModel()->Model()->mnpdf);
      if (!getLastLDAModel()->Model()->features)
        initTensor(&getLastLDAModel()->Model()->features);
      if (!getLastLDAModel()->Model()->fmean)
        initMatrix(&getLastLDAModel()->Model()->fmean);
      if (!getLastLDAModel()->Model()->fsdev)
        initMatrix(&getLastLDAModel()->Model()->fsdev);
      if (!getLastLDAModel()->Model()->inv_cov)
        initMatrix(&getLastLDAModel()->Model()->inv_cov);
      if (!getLastLDAModel()->Model()->classid)
        initUIVector(&getLastLDAModel()->Model()->classid);

      DeserializeTensor(serialized_roc, getLastLDAModel()->Model()->roc);
      DeserializeDVector(serialized_roc_aucs,
                         getLastLDAModel()->Model()->roc_aucs);
      DeserializeTensor(serialized_pr, getLastLDAModel()->Model()->pr);
      DeserializeDVector(serialized_pr_aucs,
                         getLastLDAModel()->Model()->pr_aucs);
      DeserializeMatrix(serialized_recalculated_y,
                        getLastLDAModel()->Model()->recalculated_y);
      DeserializeMatrix(serialized_recalculated_residuals,
                        getLastLDAModel()->Model()->recalculated_residuals);
      DeserializeMatrix(serialized_predicted_y,
                        getLastLDAModel()->Model()->predicted_y);
      DeserializeMatrix(serialized_predicted_residuals,
                        getLastLDAModel()->Model()->predicted_residuals);
      DeserializeDVector(serialized_prob, getLastLDAModel()->Model()->pprob);
      DeserializeDVector(serialized_eval, getLastLDAModel()->Model()->eval);
      DeserializeMatrix(serialized_evect, getLastLDAModel()->Model()->evect);
      DeserializeMatrix(serialized_mu, getLastLDAModel()->Model()->mu);
      DeserializeTensor(serialized_mnpdf, getLastLDAModel()->Model()->mnpdf);
      DeserializeTensor(serialized_features,
                        getLastLDAModel()->Model()->features);
      DeserializeMatrix(serialized_fmean, getLastLDAModel()->Model()->fmean);
      DeserializeMatrix(serialized_fsdev, getLastLDAModel()->Model()->fsdev);
      DeserializeMatrix(serialized_inv_cov,
                        getLastLDAModel()->Model()->inv_cov);
      DeserializeUIVector(serialized_classid,
                          getLastLDAModel()->Model()->classid);
      getLastLDAModel()->Model()->nclass = nclass;
      getLastLDAModel()->Model()->class_start = class_start;
      getLastLDAModel()->setValidation(validationtype);
      getLastLDAModel()->getNameClasses() =
          DeserializeQStringList(serialized_nameclasses);
      QStringList classes = serialized_classes.split("\\");
      for (int j = 0; j < classes.size(); j++)
        getLastLDAModel()->getClasses().append(
            DeserializeQStringList(classes[j]));

      int did = -1;
      for (int j = 0; j < MatrixCount(); j++) {
        if (getMatrix(j)->getHash().compare(hashinputmx) == 0) {
          did = j;
        } else {
          continue;
        }
      }

      if (did == -1) {
        (*log).append(QString("Warning on LDA Model %1. Unable to find the "
                              "origin data matrix.")
                          .arg(name));
      }

      getLastLDAModel()->setDID(did);
      if (treeWidget) {
        auto subitem = std::make_unique<QTreeWidgetItem>();
        subitem->setText(0, getLastLDAModel()->getName());
        subitem->setText(1, QString::number((*tabcount_)));
        subitem->setText(2, QString::number(getProjectID()));
        subitem->setText(3, getLastLDAModel()->getDataHash());
        subitem->setText(4, "-");
        subitem->setText(5, "-");
        subitem->setText(6, "-");
        subitem->setText(7, "-");
        subitem->setText(8, QString("LDA Model"));
        subitem->setText(9, QString::number((*mid_)));
        MainWindow::getProjectItem(getProjectID(), treeWidget)
            ->child(1)
            ->addChild(subitem.release());
      }
      if (tabcount_)
        (*tabcount_)++;

      //  ldapredTable (name TEXT, ldahash TEXT, hashinputmx TEXT, objname TEXT,
      //  varname TEXT, pred_class TEXT, pred_features TEXT,
      //                prob TEXT, classes TEXT, nameclasses TEXT, mnpdf
      //                TEXT)"));
      QString ldahash = getLastLDAModel()->getHash();
      for (int j = 0; j < ldapredlist.size(); j++) {
        if (ldapredlist[j].value("ldahash").toString().compare(ldahash) == 0) {
          (*log).append(
              QString("Importing LDA Prediction %1")
                  .arg(ldapredlist[j].value("name").toString()));
          getLastLDAModel()->addLDAPrediction();
          getLastLDAModel()->getLastLDAPrediction()->setPredID(
              getLastLDAModel()->LDAPredictionCount() - 1);
          getLastLDAModel()->getLastLDAPrediction()->setName(
              ldapredlist[j].value("name").toString());
          getLastLDAModel()->getLastLDAPrediction()->setDataHash(
              ldapredlist[j].value("hashinputmx").toString());
          getLastLDAModel()->getLastLDAPrediction()->getObjName() =
              DeserializeQStringList(ldapredlist[j].value("objname").toString());
          getLastLDAModel()->getLastLDAPrediction()->getVarName() =
              DeserializeQStringList(ldapredlist[j].value("varname").toString());
          DeserializeMatrix(
              ldapredlist[j].value("pred_class").toString(),
              getLastLDAModel()->getLastLDAPrediction()->getPredClasses());
          DeserializeMatrix(
              ldapredlist[j].value("pred_features").toString(),
              getLastLDAModel()->getLastLDAPrediction()->getPredFeatures());
          DeserializeMatrix(
              ldapredlist[j].value("prob").toString(),
              getLastLDAModel()->getLastLDAPrediction()->getProbPred());
          QStringList pred_classes =
              ldapredlist[j].value("classes").toString().split("\\");
          for (int k = 0; k < pred_classes.size(); k++)
            getLastLDAModel()->getLastLDAPrediction()->getClasses().append(
                DeserializeQStringList(pred_classes[k]));
          getLastLDAModel()->getLastLDAPrediction()->getNameClasses() =
              DeserializeQStringList(
                  ldapredlist[j].value("nameclasses").toString());
          DeserializeMatrix(
              ldapredlist[j].value("mnpdf").toString(),
              getLastLDAModel()->getLastLDAPrediction()->getMVNProbDistrib());

          int preddid = -1;
          for (int k = 0; k < MatrixCount(); k++) {
            if (getMatrix(k)->getHash().compare(
                    getLastLDAModel()->getLastLDAPrediction()->getDataHash()) ==
                0) {
              preddid = k;
              break;
            } else {
              continue;
            }
          }

          if (preddid == -1) {
            (*log).append(QString("Warning in LDA Prediction %1. Unable to "
                                  "find the origin data matrix. ")
                              .arg(getLastLDAModel()
                                       ->getLastLDAPrediction()
                                       ->getName()));
          }

          getLastLDAModel()->getLastLDAPrediction()->setDID(preddid);
          //         ModelPrediction Name - Tab Count - pid_ - Model ID - xdata
          //         id - ydata id - Data Position - Data Type (LDA Prediction,
          //         ULDA Prediction, ...) (8)
          if (treeWidget) {
            auto preditem = std::make_unique<QTreeWidgetItem>();
            preditem->setText(
                0, getLastLDAModel()->getLastLDAPrediction()->getName());
            preditem->setText(1, QString::number((*tabcount_)));
            preditem->setText(2, QString::number(getProjectID()));
            preditem->setText(3, QString::number((*mid_)));
            preditem->setText(
                4, getLastLDAModel()->getLastLDAPrediction()->getDataHash());
            preditem->setText(5, "-");
            preditem->setText(
                6,
                QString::number(
                    getLastLDAModel()->getLastLDAPrediction()->getPredID()));
            preditem->setText(7, QString("LDA Prediction"));
            MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)
                ->addChild(preditem.release());
          }
          if (tabcount_)
            (*tabcount_)++;
        }
      }
      (*mid_)++;
    
    query.exec("SELECT * from auditTable");
    while (query.next()) {
      AUDIT_ENTRY entry;
      entry.timestamp = query.value(0).toString();
      entry.action = query.value(1).toString();
      entry.details = query.value(2).toString();
      entry.user = query.value(3).toString();
      audit_trail.append(entry);
    }

    }
  } else {
    // In this case check first if the plugins and the dll are
    // correctly loaded and dependencies satisfied.
    // check the dll of every plugin
    (*log).append(QString("Warning! Unable to open the file %1 ").arg(sqlfile));
  }
}

bool DATA::AutoSave(GenericProgressDialog *pbdialog) {
  QFileInfo check_file(projectpath);
  if (check_file.exists() && check_file.isFile()) {
    if (pbdialog) {
      pbdialog->setRange(0, 5);
      pbdialog->hideCancel();
      pbdialog->show();
    }
    SaveSQLData(check_file.absolutePath(), pbdialog);
    if (pbdialog)
      pbdialog->setValue(5);
    return true;
  } else
    return false;
}

void DATA::saveMatrixToSQL(QSqlQuery *query, MATRIX *m) {
  QString mxname = m->getName();
  QString objname_serialized = SerializeQStringList(m->getObjName());
  QStringList varname = m->getVarName();
  // varname.removeFirst();
  QString varname_serialized = SerializeQStringList(varname);
  QString serialized_mx = SerializeMatrix(m->Matrix());

  query->prepare("INSERT INTO matrixTable (name, objname, varname, mx) VALUES "
                 "(:name, :objname, :varname, :mx)");
  query->bindValue(":name", mxname);
  query->bindValue(":objname", objname_serialized);
  query->bindValue(":varname", varname_serialized);
  query->bindValue(":mx", serialized_mx);
  query->exec();
}

void DATA::saveArrayToSQL(QSqlQuery *query, ARRAY *a) {
  QString arname = a->getName();
  QString objname_serialized = SerializeQStringList(a->getObjName());
  QStringList varname = a->getVarName();
  // varname.removeFirst();
  QString varname_serialized = SerializeQStringList(varname);
  QString serialized_ar = SerializeTensor(a->Array());

  query->prepare("INSERT INTO arrayTable (name, objname, varname, ar) VALUES "
                 "(:name, :objname, :varname, :ar)");
  query->bindValue(":name", arname);
  query->bindValue(":objname", objname_serialized);
  query->bindValue(":varname", varname_serialized);
  query->bindValue(":ar", serialized_ar);
  query->exec();
}

QString DATA::SaveSQLData(QString savepath, GenericProgressDialog *pbdialog) {
  if (pbdialog) {
    pbdialog->setRange(0, 5);
    pbdialog->hideCancel();
    pbdialog->show();
  }
  QString dbName = savepath + "/" + getProjectName() + ".qsm";
  projectpath = dbName;

  if (QFile::exists(dbName)) {
    if (!QFile::remove(dbName)) {
#ifdef DEBUG
      qDebug() << "Failed to remove existing database file:" << dbName;
#endif
    }
  }

  {
    // Use a unique connection name to avoid conflicts
    QString connectionName = QString("SaveConnection_%1").arg(getProjectName());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName(dbName);
    
    if (!db.open()) {
#ifdef DEBUG
      qDebug() << "Failed to open database:" << db.lastError().text();
#endif
      return QString();
    }

    QSqlQuery query(db);
    db.transaction(); // Use transaction for much faster insertions
    /*
     * The table is subdivided into five column
     * modeltype: which describe the type of data that is stored
     * txt1: one field of the data part of modeltype
     * txt2: second field of the data part of modeltype
     * txt3: third field of the data part of modeltype
     * txt4: fourth field of the data part of modeltype
     *
     * When you import the table first you read the first column
     * and then you can process the txt1, txt2...
     */
    query.exec(QString("CREATE TABLE IF NOT EXISTS matrixTable (name TEXT, "
                       "objname TEXT, varname  TEXT, mx TEXT)"));
    if (MatrixCount() > 0) {
      for (int i = 0; i < MatrixCount(); i++) {
        saveMatrixToSQL(&query, getMatrix(i));
      }
    }

    /* Step 1 matrix saved */
    if (pbdialog)
      pbdialog->setValue(1);

    query.exec(QString("CREATE TABLE IF NOT EXISTS arrayTable (name TEXT, "
                       "objname TEXT, varname  TEXT, ar TEXT)"));
    if (ArrayCount() > 0) {
      /* txt1 = arname
       * txt2 = objname_serialized
       * txt3 = varname_serialized
       * txt4 = serialized_ar
       */
      for (int i = 0; i < ArrayCount(); i++) {
        saveArrayToSQL(&query, getArray(i));
      }
    }

    /* Step 2 array/tensors saved */
    if (pbdialog)
      pbdialog->setValue(2);

    query.exec(QString("CREATE TABLE IF NOT EXISTS objlabelTable (name TEXT, "
                       "lstvalues TEXT)"));
    for (int i = 0; i < getObjectLabels().size(); i++) {
      query.prepare("INSERT INTO objlabelTable (name, lstvalues) VALUES (:name, :lstvalues)");
      query.bindValue(":name", getObjectLabels()[i].name);
      query.bindValue(":lstvalues", SerializeQStringList(getObjectLabels()[i].objects));
      query.exec();
    }

    /* Step 3 object labels saved */
    if (pbdialog)
      pbdialog->setValue(3);

    query.exec(QString("CREATE TABLE IF NOT EXISTS varlabelTable (name TEXT, "
                       "lstvalues TEXT)"));
    for (int i = 0; i < getVariableLabels().size(); i++) {
      query.prepare("INSERT INTO varlabelTable (name, lstvalues) VALUES (:name, :lstvalues)");
      query.bindValue(":name", getVariableLabels()[i].name);
      query.bindValue(":lstvalues", SerializeQStringList(getVariableLabels()[i].objects));
      query.exec();
    }

    query.exec(QString("CREATE TABLE IF NOT EXISTS vartablabelTable (name "
                       "TEXT, objname TEXT, varname  TEXT, mx TEXT)"));
    if (getVariableTabLabels().size() > 0) {
      for (int i = 0; i < getVariableTabLabels().size(); i++) {
        QString mxname = getVariableTabLabels()[i]->getName();
        QString objname_serialized =
            SerializeQStringList(getVariableTabLabels()[i]->getObjectsName());
        QStringList varname = getVariableTabLabels()[i]->getFeaturesName();
        // varname.removeFirst(); // remove the first var name that is
        // automatically inserted during import
        QString varname_serialized = SerializeQStringList(varname);
        QString serialized_mx =
            SerializeMatrix(getVariableTabLabels()[i]->getMatrix());

        query.prepare("INSERT INTO vartablabelTable (name, objname, varname, "
                      "mx) VALUES (:name, :objname, :varname, :mx)");
        query.bindValue(":name", mxname);
        query.bindValue(":objname", objname_serialized);
        query.bindValue(":varname", varname_serialized);
        query.bindValue(":mx", serialized_mx);
        query.exec();
      }
    }

    /* Step 4 variable labels saved */
    if (pbdialog)
      pbdialog->setValue(4);

    query.exec(QString(
        "CREATE TABLE IF NOT EXISTS imgTable (name TEXT,  imagedata BLOB)"));
    if (getImages().size() > 0) {
      for (int i = 0; i < getImages().size(); i++) {
        QByteArray inByteArray;
        QBuffer inBuffer(&inByteArray);
        if (inBuffer.open(QIODevice::WriteOnly)) {
          getImages()[i].image.save(&inBuffer, "PNG");
        }
        query.prepare("INSERT INTO imgTable (name, imagedata) VALUES (:name, "
                      ":imagedata)");
        query.bindValue(":name", getImages()[i].name);
        query.bindValue(":imagedata", inByteArray);
        query.exec();
      }
    }

    // Write MODELS into Models dir  RIMUOVI PCA - da ogni nome
    query.exec(QString("CREATE TABLE IF NOT EXISTS pcaTable (name TEXT, "
                       "numcomp INT, scalingtype INT, hashinputmx TEXT, "
                       "objname TEXT, varname TEXT, scores TEXT, loadings "
                       "TEXT, DMODX TEXT, varexp TEXT, colscaling TEXT, "
                       "colaverage TEXT)"));
    query.exec(
        QString("CREATE TABLE IF NOT EXISTS pcapredTable (name TEXT, pcahash "
                "TEXT, hashinputmx TEXT, objname TEXT, scores TEXT)"));
    for (int i = 0; i < PCACount(); i++) {
      PCAModel* mod = getPCAModelAt(i);
      query.prepare(
          "INSERT INTO pcaTable (name, numcomp, scalingtype, hashinputmx, "
          "objname, varname, scores, loadings, dmodx, varexp, colscaling, "
          "colaverage) VALUES (:name, :numcomp, :scalingtype, :hashinputmx, "
          ":objname, :varname, :scores, :loadings, :dmodx, :varexp, "
          ":colscaling, :colaverage)");
      query.bindValue(":name", mod->getName());
      query.bindValue(":numcomp", mod->getNPC());
      query.bindValue(":scalingtype", mod->getXScaling());
      query.bindValue(":hashinputmx", mod->getDataHash());
      query.bindValue(":objname", SerializeQStringList(mod->getObjName()));
      query.bindValue(":varname", SerializeQStringList(mod->getVarName()));
      query.bindValue(":scores", SerializeMatrix(mod->Model()->scores));
      query.bindValue(":loadings", SerializeMatrix(mod->Model()->loadings));
      query.bindValue(":dmodx", SerializeMatrix(mod->Model()->dmodx));
      query.bindValue(":colscaling", SerializeDVector(mod->Model()->colscaling));
      query.bindValue(":colaverage", SerializeDVector(mod->Model()->colaverage));
      query.bindValue(":varexp", SerializeDVector(mod->Model()->varexp));
      query.exec();

      for (int j = 0; j < mod->PCAPredictionCount(); j++) {
        PCAPREDICTION* pred = mod->getPCAPrediction(j);
        query.prepare("INSERT INTO pcapredTable (name, pcahash, "
                      "hashinputmx, objname, scores) VALUES (:name, "
                      ":pcahash, :hashinputmx, :objname, :scores)");
        query.bindValue(":name", pred->getName());
        query.bindValue(":pcahash", mod->getHash());
        query.bindValue(":hashinputmx", pred->getDataHash());
        query.bindValue(":objname", SerializeQStringList(pred->getObjName()));
        query.bindValue(":scores", SerializeMatrix(pred->getPredScores()));
        query.exec();
      }
    }

    query.exec("CREATE TABLE IF NOT EXISTS plsTable (name TEXT, nlvs INT, "
               "xscalingtype INT, yscalingtype INT, hashinputmx TEXT, "
               "objname TEXT, xvarname TEXT, yvarname TEXT, classes TEXT, "
               "tscores TEXT, ploadings TEXT, weights TEXT, xvarexp TEXT, "
               "xcolscaling TEXT, xcolaverage TEXT, uscores TEXT, qloadings TEXT, "
               "ycolscaling TEXT, ycolaverage TEXT, b TEXT, r2y_model TEXT, "
               "sdec TEXT, recalc_y TEXT, recalc_residuals TEXT, algtype INT, validationtype INT, "
               "q2y TEXT, sdep TEXT, bias TEXT, predicted_y TEXT, "
               "predicted_residuals TEXT, roc_recalculated TEXT, roc_validation TEXT, "
               "roc_auc_recalculated TEXT, roc_auc_validation TEXT, "
               "precision_recall_recalculated TEXT, precision_recall_validation TEXT, "
               "precision_recall_ap_recalculated TEXT, precision_recall_ap_validation TEXT, "
               "yscrambling TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS plspredTable (name TEXT, plshash TEXT, "
               "hashinputmx TEXT, objname TEXT, yvarname TEXT, tscores TEXT, "
               "predicted_y TEXT, r2y TEXT, sdec TEXT)");

    for (int i = 0; i < PLSCount(); i++) {
      PLSModel* mod = getPLSModelAt(i);
      query.prepare("INSERT INTO plsTable (name, nlvs, xscalingtype, yscalingtype, hashinputmx, "
                    "objname, xvarname, yvarname, classes, tscores, ploadings, weights, "
                    "xvarexp, xcolscaling, xcolaverage, uscores, qloadings, ycolscaling, "
                    "ycolaverage, b, r2y_model, sdec, recalc_y, recalc_residuals, "
                    "algtype, validationtype, q2y, sdep, bias, predicted_y, predicted_residuals, "
                    "roc_recalculated, roc_validation, roc_auc_recalculated, roc_auc_validation, "
                    "precision_recall_recalculated, precision_recall_validation, "
                    "precision_recall_ap_recalculated, precision_recall_ap_validation, "
                    "yscrambling) VALUES (:name, :nlvs, :xscalingtype, :yscalingtype, "
                    ":hashinputmx, :objname, :xvarname, :yvarname, :classes, :tscores, "
                    ":ploadings, :weights, :xvarexp, :xcolscaling, :xcolaverage, :uscores, "
                    ":qloadings, :ycolscaling, :ycolaverage, :b, :r2y_model, :sdec, "
                    ":recalc_y, :recalc_residuals, :algtype, :validationtype, :q2y, :sdep, :bias, "
                    ":predicted_y, :predicted_residuals, :roc_recalculated, :roc_validation, "
                    ":roc_auc_recalculated, :roc_auc_validation, :precision_recall_recalculated, "
                    ":precision_recall_validation, :precision_recall_ap_recalculated, "
                    ":precision_recall_ap_validation, :yscrambling)");
      
      query.bindValue(":name", mod->getName());
      query.bindValue(":nlvs", mod->getNPC());
      query.bindValue(":xscalingtype", mod->getXScaling());
      query.bindValue(":yscalingtype", mod->getYScaling());
      query.bindValue(":hashinputmx", mod->getDataHash());
      query.bindValue(":objname", SerializeQStringList(mod->getObjName()));
      query.bindValue(":xvarname", SerializeQStringList(mod->getXVarName()));
      query.bindValue(":yvarname", SerializeQStringList(mod->getYVarName()));
      query.bindValue(":classes", SerializeLABELS(mod->getClasses()));
      query.bindValue(":tscores", SerializeMatrix(mod->Model()->xscores));
      query.bindValue(":ploadings", SerializeMatrix(mod->Model()->xloadings));
      query.bindValue(":weights", SerializeMatrix(mod->Model()->xweights));
      query.bindValue(":xvarexp", SerializeDVector(mod->Model()->xvarexp));
      query.bindValue(":xcolscaling", SerializeDVector(mod->Model()->xcolscaling));
      query.bindValue(":xcolaverage", SerializeDVector(mod->Model()->xcolaverage));
      query.bindValue(":uscores", SerializeMatrix(mod->Model()->yscores));
      query.bindValue(":qloadings", SerializeMatrix(mod->Model()->yloadings));
      query.bindValue(":ycolscaling", SerializeDVector(mod->Model()->ycolscaling));
      query.bindValue(":ycolaverage", SerializeDVector(mod->Model()->ycolaverage));
      query.bindValue(":b", SerializeDVector(mod->Model()->b));
      query.bindValue(":r2y_model", SerializeMatrix(mod->Model()->r2y_recalculated));
      query.bindValue(":sdec", SerializeMatrix(mod->Model()->sdec));
      query.bindValue(":recalc_y", SerializeMatrix(mod->Model()->recalculated_y));
      query.bindValue(":recalc_residuals", SerializeMatrix(mod->Model()->recalc_residuals));
      query.bindValue(":algtype", mod->getAlgorithm());
      query.bindValue(":validationtype", mod->getValidation());
      query.bindValue(":q2y", SerializeMatrix(mod->Model()->q2y));
      query.bindValue(":sdep", SerializeMatrix(mod->Model()->sdep));
      query.bindValue(":bias", SerializeMatrix(mod->Model()->bias));
      query.bindValue(":predicted_y", SerializeMatrix(mod->Model()->predicted_y));
      query.bindValue(":predicted_residuals", SerializeMatrix(mod->Model()->pred_residuals));
      query.bindValue(":roc_recalculated", SerializeTensor(mod->Model()->roc_recalculated));
      query.bindValue(":roc_validation", SerializeTensor(mod->Model()->roc_validation));
      query.bindValue(":roc_auc_recalculated", SerializeMatrix(mod->Model()->roc_auc_recalculated));
      query.bindValue(":roc_auc_validation", SerializeMatrix(mod->Model()->roc_auc_validation));
      query.bindValue(":precision_recall_recalculated", SerializeTensor(mod->Model()->precision_recall_recalculated));
      query.bindValue(":precision_recall_validation", SerializeTensor(mod->Model()->precision_recall_validation));
      query.bindValue(":precision_recall_ap_recalculated", SerializeMatrix(mod->Model()->precision_recall_ap_recalculated));
      query.bindValue(":precision_recall_ap_validation", SerializeMatrix(mod->Model()->precision_recall_ap_validation));
      query.bindValue(":yscrambling", SerializeMatrix(mod->Model()->yscrambling));
      query.exec();

      for (int j = 0; j < mod->PLSPredictionCount(); j++) {
        PLSPREDICTION* pred = mod->getPLSPrediction(j);
        query.prepare("INSERT INTO plspredTable (name, plshash, hashinputmx, objname, yvarname, "
                      "tscores, predicted_y, r2y, sdec) VALUES (:name, :plshash, :hashinputmx, "
                      ":objname, :yvarname, :tscores, :predicted_y, :r2y, :sdec)");
        query.bindValue(":name", pred->getName());
        query.bindValue(":plshash", mod->getHash());
        query.bindValue(":hashinputmx", pred->getDataHash());
        query.bindValue(":objname", SerializeQStringList(pred->getObjName()));
        query.bindValue(":yvarname", SerializeQStringList(pred->getYVarName()));
        query.bindValue(":tscores", SerializeMatrix(pred->getXPredScores()));
        query.bindValue(":predicted_y", SerializeMatrix(pred->getYDipVar()));
        query.bindValue(":r2y", SerializeMatrix(pred->getR2Y()));
        query.bindValue(":sdec", SerializeMatrix(pred->getSDEC()));
        query.exec();
      }
    }

    query.exec("CREATE TABLE IF NOT EXISTS mlrTable (name TEXT, hashinputmx TEXT, "
               "objname TEXT, xvarname TEXT, yvarname TEXT, b TEXT, r2y TEXT, "
               "sdec TEXT, recalc_y TEXT, recalc_residuals TEXT, validationtype INT, "
               "ymean TEXT, q2y TEXT, sdep TEXT, bias TEXT, predicted_y TEXT, "
               "predicted_residuals TEXT, yscrambling TEXT)");

    query.exec("CREATE TABLE IF NOT EXISTS mlrpredTable (name TEXT, mlrhash TEXT, "
               "hashinputmx TEXT, objname TEXT, yvarname TEXT, predicted_y TEXT, "
               "r2y TEXT, sdec TEXT)");

    for (int i = 0; i < MLRCount(); i++) {
      MLRModel* mod = getMLRModelAt(i);
      query.prepare("INSERT INTO mlrTable (name, hashinputmx, objname, xvarname, yvarname, "
                    "b, r2y, sdec, recalc_y, recalc_residuals, validationtype, ymean, "
                    "q2y, sdep, bias, predicted_y, predicted_residuals, yscrambling) "
                    "VALUES (:name, :hashinputmx, :objname, :xvarname, :yvarname, :b, :r2y, "
                    ":sdec, :recalc_y, :recalc_residuals, :validationtype, :ymean, :q2y, "
                    ":sdep, :bias, :predicted_y, :predicted_residuals, :yscrambling)");
      query.bindValue(":name", mod->getName());
      query.bindValue(":hashinputmx", mod->getDataHash());
      query.bindValue(":objname", SerializeQStringList(mod->getObjName()));
      query.bindValue(":xvarname", SerializeQStringList(mod->getXVarName()));
      query.bindValue(":yvarname", SerializeQStringList(mod->getYVarName()));
      query.bindValue(":b", SerializeMatrix(mod->Model()->b));
      query.bindValue(":r2y", SerializeDVector(mod->Model()->r2y_model));
      query.bindValue(":sdec", SerializeDVector(mod->Model()->sdec));
      query.bindValue(":recalc_y", SerializeMatrix(mod->Model()->recalculated_y));
      query.bindValue(":recalc_residuals", SerializeMatrix(mod->Model()->recalc_residuals));
      query.bindValue(":validationtype", mod->getValidation());
      query.bindValue(":ymean", SerializeDVector(mod->Model()->ymean));
      query.bindValue(":q2y", SerializeDVector(mod->Model()->q2y));
      query.bindValue(":sdep", SerializeDVector(mod->Model()->sdep));
      query.bindValue(":bias", SerializeDVector(mod->Model()->bias));
      query.bindValue(":predicted_y", SerializeMatrix(mod->Model()->predicted_y));
      query.bindValue(":predicted_residuals", SerializeMatrix(mod->Model()->pred_residuals));
      query.bindValue(":yscrambling", SerializeMatrix(mod->Model()->r2q2scrambling));
      query.exec();

      for (int j = 0; j < mod->MLRPredictionCount(); j++) {
        MLRPREDICTION* pred = mod->getMLRPrediction(j);
        query.prepare("INSERT INTO mlrpredTable (name, mlrhash, hashinputmx, objname, yvarname, "
                      "predicted_y, r2y, sdec) VALUES (:name, :mlrhash, :hashinputmx, "
                      ":objname, :yvarname, :predicted_y, :r2y, :sdec)");
        query.bindValue(":name", pred->getName());
        query.bindValue(":mlrhash", mod->getHash());
        query.bindValue(":hashinputmx", pred->getDataHash());
        query.bindValue(":objname", SerializeQStringList(pred->getObjName()));
        query.bindValue(":yvarname", SerializeQStringList(pred->getYVarName()));
        query.bindValue(":predicted_y", SerializeMatrix(pred->getYDipVar()));
        query.bindValue(":r2y", SerializeDVector(pred->getR2Y()));
        query.bindValue(":sdec", SerializeDVector(pred->getSDEC()));
        query.exec();
      }
    }

    query.exec("CREATE TABLE IF NOT EXISTS ldaTable (name TEXT, hashinputmx TEXT, "
               "objname TEXT, varname TEXT, validation INT, roc TEXT, roc_aucs TEXT, "
               "pr TEXT, pr_aucs TEXT, recalculated_y TEXT, recalculated_residuals TEXT, "
               "predicted_y TEXT, predicted_residuals TEXT, pprob TEXT, evect TEXT, "
               "eval TEXT, mu TEXT, mnpdf TEXT, features TEXT, fmean TEXT, fsdev TEXT, "
               "inv_cov TEXT, nclass INT, class_start INT, classid TEXT, classes TEXT, "
               "nameclasses TEXT)");
    query.exec("CREATE TABLE IF NOT EXISTS ldapredTable (name TEXT, ldahash TEXT, "
               "hashinputmx TEXT, objname TEXT, varname TEXT, pred_class TEXT, "
               "pred_features TEXT, prob TEXT, classes TEXT, nameclasses TEXT, mnpdf TEXT)");

    for (int i = 0; i < LDACount(); i++) {
      LDAModel* mod = getLDAModelAt(i);
      query.prepare(
          "INSERT INTO ldaTable (name, hashinputmx, objname, varname, "
          "validation, roc, roc_aucs, pr, pr_aucs, recalculated_y, "
          "recalculated_residuals, predicted_y, predicted_residuals, pprob, "
          "evect, eval, mu, mnpdf, features, fmean, fsdev, inv_cov, nclass, "
          "class_start, classid, classes, nameclasses) VALUES (:name, "
          ":hashinputmx, :objname, :varname, :validation, :roc, :roc_aucs, "
          ":pr, :pr_aucs, :recalculated_y, :recalculated_residuals, "
          ":predicted_y, :predicted_residuals, :pprob, :evect, :eval, :mu, "
          ":mnpdf, :features, :fmean, :fsdev, :inv_cov, :nclass, "
          ":class_start, :classid, :classes, :nameclasses)");
      query.bindValue(":name", mod->getName());
      query.bindValue(":hashinputmx", mod->getDataHash());
      query.bindValue(":objname", SerializeQStringList(mod->getObjName()));
      query.bindValue(":varname", SerializeQStringList(mod->getVarName()));
      query.bindValue(":validation", mod->getValidation());
      query.bindValue(":roc", SerializeTensor(mod->Model()->roc));
      query.bindValue(":roc_aucs", SerializeDVector(mod->Model()->roc_aucs));
      query.bindValue(":pr", SerializeTensor(mod->Model()->pr));
      query.bindValue(":pr_aucs", SerializeDVector(mod->Model()->pr_aucs));
      query.bindValue(":recalculated_y", SerializeMatrix(mod->Model()->recalculated_y));
      query.bindValue(":recalculated_residuals", SerializeMatrix(mod->Model()->recalculated_residuals));
      query.bindValue(":predicted_y", SerializeMatrix(mod->Model()->predicted_y));
      query.bindValue(":predicted_residuals", SerializeMatrix(mod->Model()->predicted_residuals));
      query.bindValue(":pprob", SerializeDVector(mod->Model()->pprob));
      query.bindValue(":eval", SerializeDVector(mod->Model()->eval));
      query.bindValue(":evect", SerializeMatrix(mod->Model()->evect));
      query.bindValue(":mu", SerializeMatrix(mod->Model()->mu));
      query.bindValue(":mnpdf", SerializeTensor(mod->Model()->mnpdf));
      query.bindValue(":features", SerializeTensor(mod->Model()->features));
      query.bindValue(":fmean", SerializeMatrix(mod->Model()->fmean));
      query.bindValue(":fsdev", SerializeMatrix(mod->Model()->fsdev));
      query.bindValue(":inv_cov", SerializeMatrix(mod->Model()->inv_cov));
      query.bindValue(":nclass", (qlonglong)mod->Model()->nclass);
      query.bindValue(":class_start", (qlonglong)mod->Model()->class_start);
      query.bindValue(":classid", SerializeUIVector(mod->Model()->classid));
      query.bindValue(":nameclasses", SerializeQStringList(mod->getNameClasses()));
      
      QString serialized_classes;
      for (int j = 0; j < mod->getClasses().size() - 1; j++)
        serialized_classes += SerializeQStringList(mod->getClasses()[j]) + "\\";
      if (!mod->getClasses().isEmpty())
        serialized_classes += SerializeQStringList(mod->getClasses().last());
      query.bindValue(":classes", serialized_classes);
      query.exec();

      for (int j = 0; j < mod->LDAPredictionCount(); j++) {
        LDAPREDICTION* pred = mod->getLDAPrediction(j);
        query.prepare("INSERT INTO ldapredTable (name, ldahash, hashinputmx, objname, varname, "
                      "pred_class, pred_features, prob, classes, nameclasses, mnpdf) "
                      "VALUES (:name, :ldahash, :hashinputmx, :objname, :varname, :pred_class, "
                      ":pred_features, :prob, :classes, :nameclasses, :mnpdf)");
        query.bindValue(":name", pred->getName());
        query.bindValue(":ldahash", mod->getHash());
        query.bindValue(":hashinputmx", pred->getDataHash());
        query.bindValue(":objname", SerializeQStringList(pred->getObjName()));
        query.bindValue(":varname", SerializeQStringList(pred->getVarName()));
        query.bindValue(":pred_class", SerializeMatrix(pred->getPredClasses()));
        query.bindValue(":pred_features", SerializeMatrix(pred->getPredFeatures()));
        query.bindValue(":prob", SerializeMatrix(pred->getProbPred()));
        query.bindValue(":nameclasses", SerializeQStringList(pred->getNameClasses()));
        query.bindValue(":mnpdf", SerializeMatrix(pred->getMVNProbDistrib()));
        
        QString pred_serialized_classes;
        for (int k = 0; k < pred->getClasses().size() - 1; k++)
          pred_serialized_classes += SerializeQStringList(pred->getClasses()[k]) + "\\";
        if (!pred->getClasses().isEmpty())
          pred_serialized_classes += SerializeQStringList(pred->getClasses().last());
        query.bindValue(":classes", pred_serialized_classes);
        query.exec();
      }
    }

    query.exec(QString("CREATE TABLE IF NOT EXISTS auditTable (timestamp TEXT, action TEXT, details TEXT, user TEXT)"));
    for (const auto &entry : audit_trail) {
      query.prepare("INSERT INTO auditTable (timestamp, action, details, user) VALUES (:timestamp, :action, :details, :user)");
      query.bindValue(":timestamp", entry.timestamp);
      query.bindValue(":action", entry.action);
      query.bindValue(":details", entry.details);
      query.bindValue(":user", entry.user);
      query.exec();
    }

    db.commit();
    db.close();
    QSqlDatabase::removeDatabase(connectionName);
    if (pbdialog)
      pbdialog->setValue(5);
    return dbName;
  }
}

void DATA::addMatrix() { 
  matrix_.append(new MATRIX()); 
  addAuditEntry("Add Matrix", "Added new empty matrix");
}

void DATA::addMatrix(MATRIX *mx) {
  matrix_.append(new MATRIX());
  addAuditEntry("Add Matrix", "Added new empty matrix");
  matrix *m = matrix_.last()->Matrix();
  MatrixCopy(mx->Matrix(), &m);
  matrix_.last()->getObjName().append(mx->getObjName());
  matrix_.last()->getVarName().append(mx->getVarName());
  matrix_.last()->setName(mx->getName());
  addAuditEntry("Add Matrix", QString("Added matrix: %1").arg(mx->getName()));
}

void DATA::addArray() { array_.append(new ARRAY()); }

void DATA::addArray(ARRAY *ar) {
  array_.append(new ARRAY());
  tensor *ar_ = array_.last()->Array();

  for (uint order = 0; order < ar->Array()->order; order++) {
    TensorAppendMatrix(ar_, ar->Array()->m[order]);
  }

  array_.last()->getObjName().append(ar->getObjName());
  array_.last()->getVarName().append(ar->getVarName());
  array_.last()->setName(ar->getName());
}

void DATA::delMatrixAt(int i) {
  if (i < matrix_.size()) {
    delete matrix_[i];
    addAuditEntry("Delete Matrix", QString("Deleted matrix at index: %1").arg(i));
    matrix_.removeAt(i);
  }
}

void DATA::delArrayAt(int i) {
  if (i < array_.size()) {
    delete array_[i];
    array_.removeAt(i);
  }
}

void DATA::delMatrix() {
  for (int i = 0; i < matrix_.size(); i++) {
    delete matrix_[i];
  }
  matrix_.clear();
}

void DATA::delArray() {
  for (int i = 0; i < array_.size(); i++) {
    delete array_[i];
  }
  array_.clear();
}

void DATA::addPCAModel() { 
  pcamodel.append(new PCAModel()); 
  addAuditEntry("Add PCA Model", "Added new PCA model");
}

void DATA::delPCAModel(int mid) {
  for (int i = 0; i < pcamodel.size(); i++) {
    if (mid == pcamodel[i]->getModelID()) {
      pcamodel[i]->delPCAPredictions();
      delete pcamodel[i];
      pcamodel.removeAt(i);
      break;
    } else {
      continue;
    }
  }
}

void DATA::delPCAModelAt(int id) {
  if (id < pcamodel.size()) {
    pcamodel[id]->delPCAPredictions();
    delete pcamodel[id];
    addAuditEntry("Delete PCA Model", QString("Deleted PCA model at index: %1").arg(id));
    pcamodel.removeAt(id);
  }
}

void DATA::delPCAModels() {
  for (int i = 0; i < pcamodel.size(); i++) {
    pcamodel[i]->delPCAPredictions();
    delete pcamodel[i];
  }
  pcamodel.clear();
}

void DATA::addICAModel() {
  icamodel.append(new ICAModel());
  addAuditEntry("Add ICA Model", "Added new ICA model");
}

void DATA::delICAModel(int mid) {
  for (int i = 0; i < icamodel.size(); i++) {
    if (mid == icamodel[i]->getModelID()) {
      icamodel[i]->delICAPredictions();
      delete icamodel[i];
      icamodel.removeAt(i);
      break;
    } else {
      continue;
    }
  }
}

void DATA::delICAModelAt(int id) {
  if (id < icamodel.size()) {
    icamodel[id]->delICAPredictions();
    delete icamodel[id];
    addAuditEntry("Delete ICA Model",
                  QString("Deleted ICA model at index: %1").arg(id));
    icamodel.removeAt(id);
  }
}

void DATA::delICAModels() {
  for (int i = 0; i < icamodel.size(); i++) {
    icamodel[i]->delICAPredictions();
    delete icamodel[i];
  }
  icamodel.clear();
}

void DATA::addCPCAModel() { 
  cpcamodel.append(new CPCAModel()); 
  addAuditEntry("Add CPCA Model", "Added new CPCA model");
}

void DATA::delCPCAModel(int mid) {
  for (int i = 0; i < cpcamodel.size(); i++) {
    if (mid == cpcamodel[i]->getModelID()) {
      cpcamodel[i]->delCPCAPredictions();
      delete cpcamodel[i];
      cpcamodel.removeAt(i);
      break;
    } else {
      continue;
    }
  }
}

void DATA::delCPCAModelAt(int id) {
  if (id < cpcamodel.size()) {
    cpcamodel[id]->delCPCAPredictions();
    delete cpcamodel[id];
    addAuditEntry("Delete PCA Model", QString("Deleted PCA model at index: %1").arg(id));
    addAuditEntry("Delete CPCA Model", QString("Deleted CPCA model at index: %1").arg(id));
    cpcamodel.removeAt(id);
  }
}

void DATA::delCPCAModels() {
  for (int i = 0; i < cpcamodel.size(); i++) {
    cpcamodel[i]->delCPCAPredictions();
    delete cpcamodel[i];
  }
  cpcamodel.clear();
}

void DATA::addPLSModel() { 
  plsmodel.append(new PLSModel()); 
  addAuditEntry("Add PLS Model", "Added new PLS model");
}

void DATA::delPLSModel(int mid) {
  for (int i = 0; i < plsmodel.size(); i++) {
    if (mid == plsmodel[i]->getModelID()) {
      plsmodel[i]->delPLSPredictions();
      delete plsmodel[i];
      plsmodel.removeAt(i);
      break;
    } else {
      continue;
    }
  }
}

void DATA::delPLSModelAt(int id) {
  if (id < plsmodel.size()) {
    plsmodel[id]->delPLSPredictions();
    delete plsmodel[id];
    addAuditEntry("Delete PLS Model", QString("Deleted PLS model at index: %1").arg(id));
    plsmodel.removeAt(id);
  }
}

void DATA::delPLSModels() {
  for (int i = 0; i < plsmodel.size(); i++) {
    plsmodel[i]->delPLSPredictions();
    delete plsmodel[i];
  }
  plsmodel.clear();
}

void DATA::addMLRModel() { 
  mlrmodel.append(new MLRModel()); 
  addAuditEntry("Add MLR Model", "Added new MLR model");
}

void DATA::delMLRModel(int mid) {
  for (int i = 0; i < mlrmodel.size(); i++) {
    if (mid == mlrmodel[i]->getModelID()) {
      delete mlrmodel[i];
      mlrmodel.removeAt(i);
      break;
    } else {
      continue;
    }
  }
}

void DATA::delMLRModelAt(int id) {
  if (id < mlrmodel.size()) {
    delete mlrmodel[id];
    addAuditEntry("Delete MLR Model", QString("Deleted MLR model at index: %1").arg(id));
    mlrmodel.removeAt(id);
  }
}

void DATA::delMLRModels() {
  for (int i = 0; i < mlrmodel.size(); i++) {
    delete mlrmodel[i];
  }
  mlrmodel.clear();
}

void DATA::addLDAModel() { 
  ldamodel.append(new LDAModel()); 
  addAuditEntry("Add LDA Model", "Added new LDA model");
}

void DATA::delLDAModel(int mid) {
  for (int i = 0; i < ldamodel.size(); i++) {
    if (mid == ldamodel[i]->getModelID()) {
      delete ldamodel[i];
      ldamodel.removeAt(i);
      break;
    } else {
      continue;
    }
  }
}

void DATA::delLDAModelAt(int id) {
  if (id < ldamodel.size()) {
    delete ldamodel[id];
    addAuditEntry("Delete LDA Model", QString("Deleted LDA model at index: %1").arg(id));
    ldamodel.removeAt(id);
  }
}

void DATA::delLDAModels() {
  for (int i = 0; i < ldamodel.size(); i++) {
    delete ldamodel[i];
  }
  ldamodel.clear();
}

void DATA::setProjectID(int id_) { uniqueid = id_; }

int DATA::getProjectID() { return uniqueid; }

int DATA::getMatrixID(QString hash) {
  for (int i = 0; i < matrix_.size(); i++) {
    if (matrix_[i]->getHash().compare(hash) == 0) {
      return i;
    } else {
      continue;
    }
  }
  return -1;
}

int DATA::getArrayID(QString hash) {
  for (int i = 0; i < array_.size(); i++) {
    if (array_[i]->getHash().compare(hash) == 0) {
      return i;
    } else {
      continue;
    }
  }
  return -1;
}

MATRIX *DATA::getMatrix(int id) {
  Q_ASSERT(id < matrix_.size());
  return matrix_[id];
}

MATRIX *DATA::getMatrix(QString hash) {
  for (int i = 0; i < matrix_.size(); i++) {
    if (matrix_[i]->getHash().compare(hash) == 0) {
      return matrix_[i];
    } else {
      continue;
    }
  }
  return 0;
}

ARRAY *DATA::getArray(int id) {
  Q_ASSERT(id < array_.size());
  return array_.at(id);
}

ARRAY *DATA::getArray(QString hash) {
  for (int i = 0; i < array_.size(); i++) {
    if (array_[i]->getHash().compare(hash) == 0) {
      return array_[i];
    } else {
      continue;
    }
  }
  return 0;
}

PCAModel *DATA::getLastPCAModel() { return pcamodel.last(); }

ICAModel *DATA::getLastICAModel() { return icamodel.last(); }

CPCAModel *DATA::getLastCPCAModel() { return cpcamodel.last(); }

PLSModel *DATA::getLastPLSModel() { return plsmodel.last(); }

LDAModel *DATA::getLastLDAModel() { return ldamodel.last(); }

MLRModel *DATA::getLastMLRModel() { return mlrmodel.last(); }

PCAModel *DATA::getPCAModelAt(int id) {
  Q_ASSERT(id < pcamodel.size());
  return pcamodel[id];
}

ICAModel *DATA::getICAModelAt(int id) {
  Q_ASSERT(id < icamodel.size());
  return icamodel[id];
}

CPCAModel *DATA::getCPCAModelAt(int id) {
  Q_ASSERT(id < cpcamodel.size());
  return cpcamodel[id];
}

PLSModel *DATA::getPLSModelAt(int id) {
  Q_ASSERT(id < plsmodel.size());
  return plsmodel[id];
}

LDAModel *DATA::getLDAModelAt(int id) {
  Q_ASSERT(id < ldamodel.size());
  return ldamodel[id];
}

MLRModel *DATA::getMLRModelAt(int id) {
  Q_ASSERT(id < mlrmodel.size());
  return mlrmodel[id];
}

PCAModel *DATA::getPCAModel(int mid) {
  for (int i = 0; i < pcamodel.size(); i++) {
    if (mid == pcamodel[i]->getModelID()) {
      return pcamodel[i];
    } else {
      continue;
    }
  }
  return 0;
}

ICAModel *DATA::getICAModel(int mid) {
  for (int i = 0; i < icamodel.size(); i++) {
    if (mid == icamodel[i]->getModelID()) {
      return icamodel[i];
    } else {
      continue;
    }
  }
  return 0;
}

CPCAModel *DATA::getCPCAModel(int mid) {
  for (int i = 0; i < cpcamodel.size(); i++) {
    if (mid == cpcamodel[i]->getModelID()) {
      return cpcamodel[i];
    } else {
      continue;
    }
  }
  return 0;
}

PLSModel *DATA::getPLSModel(int mid) {
  for (int i = 0; i < plsmodel.size(); i++) {
    if (mid == plsmodel[i]->getModelID()) {
      return plsmodel[i];
    } else {
      continue;
    }
  }
  return 0;
}

PLSModel *DATA::getPLSModel(QString hash) {
  for (int i = 0; i < plsmodel.size(); i++) {
    if (plsmodel[i]->getHash().compare(hash) == 0) {
      return plsmodel[i];
    } else {
      continue;
    }
  }
  return 0;
}

LDAModel *DATA::getLDAModel(int mid) {
  for (int i = 0; i < ldamodel.size(); i++) {
    if (mid == ldamodel[i]->getModelID()) {
      return ldamodel[i];
    } else {
      continue;
    }
  }
  return 0;
}

MLRModel *DATA::getMLRModel(int mid) {
  for (int i = 0; i < mlrmodel.size(); i++) {
    if (mid == mlrmodel[i]->getModelID()) {
      return mlrmodel[i];
    } else {
      continue;
    }
  }
  return 0;
}

int DATA::MatrixCount() { return matrix_.size(); }

int DATA::ArrayCount() { return array_.size(); }

int DATA::PCACount() { return pcamodel.size(); }

int DATA::ICACount() { return icamodel.size(); }

int DATA::CPCACount() { return cpcamodel.size(); }

int DATA::PLSCount() { return plsmodel.size(); }

int DATA::LDACount() { return ldamodel.size(); }

int DATA::MLRCount() { return mlrmodel.size(); }

DATA::DATA() { uniqueid = -1; }

DATA::~DATA() {
#ifdef DEBUG
  qDebug() << "Delete Data";
#endif
  delMatrix();
  delArray();
  delPCAModels();
  delICAModels();
  delPLSModels();
  delMLRModels();
}

void DATA::savePCAModelToSQL(QSqlQuery *query, PCAModel *mod) {
  query->prepare(
      "INSERT INTO pcaTable (name, numcomp, scalingtype, hashinputmx, "
      "objname, varname, scores, loadings, dmodx, varexp, colscaling, "
      "colaverage) VALUES (:name, :numcomp, :scalingtype, :hashinputmx, "
      ":objname, :varname, :scores, :loadings, :dmodx, :varexp, "
      ":colscaling, :colaverage)");
  query->bindValue(":name", mod->getName());
  query->bindValue(":numcomp", mod->getNPC());
  query->bindValue(":scalingtype", mod->getXScaling());
  query->bindValue(":hashinputmx", mod->getDataHash());
  query->bindValue(":objname", SerializeQStringList(mod->getObjName()));
  query->bindValue(":varname", SerializeQStringList(mod->getVarName()));
  query->bindValue(":scores", SerializeMatrix(mod->Model()->scores));
  query->bindValue(":loadings", SerializeMatrix(mod->Model()->loadings));
  query->bindValue(":dmodx", SerializeMatrix(mod->Model()->dmodx));
  query->bindValue(":colscaling", SerializeDVector(mod->Model()->colscaling));
  query->bindValue(":colaverage", SerializeDVector(mod->Model()->colaverage));
  query->bindValue(":varexp", SerializeDVector(mod->Model()->varexp));
  query->exec();
}

void DATA::loadPCAModelsFromSQL(QSqlQuery *query, int *mid_, int *tabcount_,
                                QTreeWidget *treeWidget, QStringList *log) {
  while (query->next()) {
    QString name = query->value(0).toString();
    int npc = query->value(1).toInt();
    int xscaling = query->value(2).toInt();
    QString hashinputmx = query->value(3).toString();
    QString s_objname = query->value(4).toString();
    QString s_varname = query->value(5).toString();
    QString s_scores = query->value(6).toString();
    QString s_loadings = query->value(7).toString();
    QString s_dmodx = query->value(8).toString();
    QString s_varexp = query->value(9).toString();
    QString s_colscaling = query->value(10).toString();
    QString s_colaverage = query->value(11).toString();

    addPCAModel();
    getLastPCAModel()->setModelID((*mid_));
    getLastPCAModel()->setName(name);
    getLastPCAModel()->setNPC(npc);
    getLastPCAModel()->setXScaling(xscaling);
    getLastPCAModel()->setDataHash(hashinputmx);
    getLastPCAModel()->getObjName() = DeserializeQStringList(s_objname);
    getLastPCAModel()->getVarName() = DeserializeQStringList(s_varname);
    DeserializeMatrix(s_scores, getLastPCAModel()->Model()->scores);
    DeserializeMatrix(s_loadings, getLastPCAModel()->Model()->loadings);
    DeserializeMatrix(s_dmodx, getLastPCAModel()->Model()->dmodx);
    DeserializeDVector(s_colscaling, getLastPCAModel()->Model()->colscaling);
    DeserializeDVector(s_colaverage, getLastPCAModel()->Model()->colaverage);
    DeserializeDVector(s_varexp, getLastPCAModel()->Model()->varexp);

    // ... find DID logic ...
    int xid = -1;
    for (int j = 0; j < MatrixCount(); j++) {
      if (getMatrix(j)->getHash().compare(hashinputmx) == 0) {
        xid = j;
        break;
      }
    }
    getLastPCAModel()->setDID(xid);

    if (treeWidget) {
      auto subitem = std::make_unique<QTreeWidgetItem>();
      subitem->setText(0, getLastPCAModel()->getName());
      subitem->setText(8, QString("PCA Model"));
      subitem->setText(9, QString::number((*mid_)));
      MainWindow::getProjectItem(getProjectID(), treeWidget)
          ->child(1)
          ->addChild(subitem.release());
    }
    if (tabcount_)
      (*tabcount_)++;
    if (log)
      (*log).append(QString("PCA model %1 imported.\n").arg(name));
    (*mid_)++;
    
    query->exec("SELECT * from auditTable");
    while (query->next()) {
      AUDIT_ENTRY entry;
      entry.timestamp = query->value(0).toString();
      entry.action = query->value(1).toString();
      entry.details = query->value(2).toString();
      entry.user = query->value(3).toString();
      audit_trail.append(entry);
    }

  }
}

void DATA::addAuditEntry(QString action, QString details) {
  AUDIT_ENTRY entry;
  entry.timestamp = QDateTime::currentDateTime().toString(Qt::ISODate);
  entry.action = action;
  entry.details = details;
  entry.user = QHostInfo::localHostName() + "/" + qgetenv("USER");
  if (entry.user.isEmpty()) entry.user = qgetenv("USERNAME");
  audit_trail.append(entry);
}
