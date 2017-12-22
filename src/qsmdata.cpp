#include "qsmdata.h"
#include <QFile>
#include <QFileDevice>
#include <QFileInfo>
#include <QDir>
#include <QIODevice>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QBuffer>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QDebug>

#include "DATAIO.h"
#include "dircompressor.h"
#include "MainWindow.h"
// #include "ProgressBarDialog.h"
#include "qstudiometricstypes.h"
#include "qstudiometricsdataoperations.h"

void DATA::WriteList(QStringList &lst, QString fname)
{
  QFile file(fname);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return;

  QTextStream out(&file);
  for(int i = 0; i < lst.size(); i++){
    out << lst[i].toUtf8().data() << "\n";
  }
  file.close();
}

void DATA::GenNameLst(uint n, QString name, QStringList &namelst)
{
  for(uint i  = 0; i < n; i++){
    namelst.append(name+QString("%0").arg(i+1));
  }
}

// static void DATA::ImportObjectNames(FILEDATA f, QStringList &objname)
void DATA::ImportRows(QString fname, QStringList &rowlst)
{
  QFile file(fname);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  while(!in.atEnd()){
    QString name = QString::fromUtf8(in.readLine().trimmed().toUtf8());
    if(name.isEmpty())
      continue;
    else
      rowlst.append(name);
  }

  file.close();
}

// static void DATA::ImportVarNames(FILEDATA f, QStringList &varname)
// collst.append("Object Names");
void DATA::ImportColumns(QString fname, QString separator, QStringList &collst)
{
  QFile file(fname);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    return;

  QTextStream in(&file);
  while(!in.atEnd()){
    QStringList list = in.readLine().trimmed().split(separator, QString::SkipEmptyParts);
    for(int i = 0; i < list.size(); i++)
      if(list[i].isEmpty())
        continue;
      else
        collst.append(QString::fromUtf8(list[i].toUtf8()));
  }
  file.close();
}

bool DATA::CopyFile(const QString& sourceFile, const QString& destinationDir)
{
  QFileInfo fileInfo(sourceFile);
  QString destinationFile = destinationDir + QDir::separator() + fileInfo.fileName();
  bool result = QFile::copy(sourceFile, destinationFile);
  return result;
}

void DATA::ImportFileMatrix(const FILEDATA& f)
{
  if(f.datatype == 1){ // import matrix
    addMatrix();
    DATAIO::ImportMatrix(f.filename.toUtf8().data(),
                      f.separator.toStdString(),
                      getMatrix(MatrixCount()-1)->Matrix());
    if(f.fileobjname.isEmpty()){
        GenNameLst(getMatrix(MatrixCount()-1)->Matrix()->row, "Obj", getMatrix(MatrixCount()-1)->getObjName());
    }
    else{
      ImportRows(f.fileobjname, getMatrix(MatrixCount()-1)->getObjName());
    }

    if(f.filevarname.isEmpty()){
      getMatrix(MatrixCount()-1)->getVarName().append(QString("Object Names"));
      GenNameLst(getMatrix(MatrixCount()-1)->Matrix()->col, "Var", getMatrix(MatrixCount()-1)->getVarName());
    }
    else{
      getMatrix(MatrixCount()-1)->getVarName().append(QString("Object Names"));
      ImportColumns(f.filevarname, f.separator, getMatrix(MatrixCount()-1)->getVarName());
      if(getMatrix(MatrixCount()-1)->getVarName().size() == 0){
        getMatrix(MatrixCount()-1)->getVarName().append(QString("Object Names"));
        GenNameLst(getMatrix(MatrixCount()-1)->Matrix()->col, "Var", getMatrix(MatrixCount()-1)->getVarName());
      }
    }

    if(MatrixCount()-2 > 0){
      QString label = f.label;
      int i = 0;
      int id = 1;
      while(i < MatrixCount()-2){
        if(getMatrix(i)->getName().compare(label) == 0){
          label = f.label+"_"+QString::number(id);
          i = 0;
          id += 1;
        }
        i++;
      }
      getMatrix(MatrixCount()-1)->setName(label);
    }
    else{
      getMatrix(MatrixCount()-1)->setName(f.label);
    }
    #ifdef DEBUG
    qDebug() << getMatrix(MatrixCount()-1)->getName();
    #endif
  }
}

void DATA::ImportFileArray(const FILEDATA& f)
{
  if(f.datatype == 2){ // import array
    addArray();

    DATAIO::ImportArray(f.filename.toUtf8().data(),
                    f.separator.toStdString(),
                    getArray(ArrayCount()-1)->Array());

    if(f.fileobjname.isEmpty()){
        GenNameLst(getArray(ArrayCount()-1)->Array()->m[0]->row, "Obj", getArray(ArrayCount()-1)->getObjName());
    }
    else{
      ImportRows(f.fileobjname, getArray(ArrayCount()-1)->getObjName());
    }


    if(f.filevarname.isEmpty()){
      getArray(ArrayCount()-1)->getVarName().append(QString("Object Names"));
      GenNameLst(getArray(ArrayCount()-1)->Array()->m[0]->col, "Var", getArray(ArrayCount()-1)->getVarName());
    }
    else{
      getArray(ArrayCount()-1)->getVarName().append(QString("Object Names"));
      ImportColumns(f.filevarname, f.separator,getArray(ArrayCount()-1)->getVarName());
      if(getArray(ArrayCount()-1)->getVarName().size() == 0){
        getArray(ArrayCount()-1)->getVarName().append(QString("Object Names"));
        GenNameLst(getArray(ArrayCount()-1)->Array()->m[0]->col, "Var", getArray(ArrayCount()-1)->getVarName());
      }
    }

    if(ArrayCount()-2 > 0){
      QString label = f.label;
      int i = 0;
      int id = 1;
      while( i < ArrayCount()-2){
        if(getArray(i)->getName().compare(label) == 0){
          label = f.label+"_"+QString::number(id);
          i = 0;
          id += 1;
        }
        i++;
      }
      getArray(ArrayCount()-1)->setName(label);
    }
    else{
      getArray(ArrayCount()-1)->setName(f.label);
    }
  }
}

void DATA::OpenData(QString dir, QTreeWidget *treeWidget, int *tabcount_, int *mid_, QStringList *log)
{
  /* Warning!! there are some Methods from MainWindow and they works only if the ui is started... */
  QFileInfo info(dir);
  QDir datadir = QDir(dir+"/Data");
  QDir modelsdir = QDir(dir+"/Models");
  QDir objlabelsdir = QDir(dir+"/ObjectLabels");
  QDir varlabelsdir = QDir(dir+"/VariableLabels");
  QDir vartablabelsdir = QDir(dir+"/VariableTabLabels");
  QDir imgdir = QDir(dir+"/Images");

  if(datadir.exists() == true){
    QDir mxdir = QDir(datadir.absolutePath()+"/Matrix");
    if(mxdir.exists() == true){
      QStringList mxlist = mxdir.entryList();
      mxlist.removeAll(".");
      mxlist.removeAll("..");
      for(int i = 0; i < mxlist.size(); i++){
        QDir mx = QDir(mxdir.absolutePath()+"/"+mxlist[i]);
        if(mx.exists() == true){
          if(QFile::exists(mx.absolutePath()+"/data.txt") == true
            && QFile::exists(mx.absolutePath()+"/label.txt") == true
            && QFile::exists(mx.absolutePath()+"/varname.txt") == true){
            FILEDATA f;
            f.label = mxlist[i];
            f.filename = mx.absolutePath()+"/data.txt";
            f.fileobjname = mx.absolutePath()+"/label.txt";
            f.filevarname= mx.absolutePath()+"/varname.txt";
            f.datatype = 1;
            f.separator = "\t";
            f.skiplineby = "#";

            ImportFileMatrix(f);
            QTreeWidgetItem *subitem = new QTreeWidgetItem;
            subitem->setText(0, f.label); /*set the data name from the file*/
            subitem->setText(1, QString("Matrix")); // Define the type of the data
            subitem->setText(2, QString::number((*tabcount_))); // Define the tab id number in order to close a specific table
            subitem->setText(3, QString::number(MatrixCount()-1)); // Define the matrix position id in order to find easly when you need to show data.
            subitem->setText(4, QString::number(getProjectID())); // pid for get the array with Value
            (*tabcount_)++;

            MainWindow::getProjectItem(getProjectID(), treeWidget)->child(0)->addChild(subitem);

          }
          else{
            (*log).append(QString("Corrupted Matrix: %1\n").arg(mx.absolutePath()));
          }
        }
        else{
          (*log).append(QString("No such Matrix: %1\n").arg(mx.absolutePath()));
        }
      }
    }

    QDir ardir = QDir(datadir.absolutePath()+"/Array");
    if(ardir.exists() == true){
      QStringList arlist = ardir.entryList();
      arlist.removeAll(".");
      arlist.removeAll("..");
      for(int i = 0; i < arlist.size(); i++){
        QDir ar = QDir(ardir.absolutePath()+"/"+arlist[i]);
        if(ar.exists() == true){
          if(QFile::exists(ar.absolutePath()+"/data.txt") == true
            && QFile::exists(ar.absolutePath()+"/label.txt") == true
            && QFile::exists(ar.absolutePath()+"/varname.txt") == true){
            FILEDATA f;
            f.label = arlist[i];
            f.filename = ar.absolutePath()+"/data.txt";
            f.fileobjname = ar.absolutePath()+"/label.txt";
            f.filevarname= ar.absolutePath()+"/varname.txt";
            f.datatype = 2;
            f.separator = "\t";
            f.skiplineby = "#";

            ImportFileArray(f);
            QTreeWidgetItem *subitem = new QTreeWidgetItem;
            subitem->setText(0, QString(f.label));
            subitem->setText(1, QString("Array"));  // Define the type of the data
            subitem->setText(2, QString::number((*tabcount_))); // Define the tab id number in order to close a specific table
            subitem->setText(3, QString::number(ArrayCount()-1)); // Define the array position id in order to find easly when you need to show data.
            subitem->setText(4, QString::number(getProjectID())); // pid for get the array with Value
            (*tabcount_)++;
            MainWindow::getProjectItem(getProjectID(), treeWidget)->child(0)->addChild(subitem);
          }
          else{
            (*log).append(QString("Corrupted Array: %1\n").arg(ar.absolutePath()));
          }
        }
        else{
          (*log).append(QString("No such Array: %1\n").arg(ar.absolutePath()));
        }
      }
    }
  }
  else{
    (*log).append(QString("%1 Data Project Corrupted!").arg(info.fileName()));
  }

  if(objlabelsdir.exists() == true){
    QStringList labellist = objlabelsdir.entryList();
    labellist.removeAll(".");
    labellist.removeAll("..");
    for(int i = 0; i < labellist.size(); i++){
      if(QFile::exists(objlabelsdir.absolutePath()+"/"+labellist[i]) == true){
        QFile file(objlabelsdir.absolutePath()+"/"+labellist[i]);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
          (*log).append(QString("No such directory: %1\n").arg(labellist[i]));
        }
        else{
          getObjectLabels().append(LABEL());
          QString name = labellist[i];
          name.remove(".txt", Qt::CaseInsensitive);
          getObjectLabels().last().name = name;
          QTextStream in(&file);
          while(!in.atEnd()){
            getObjectLabels().last().objects.append(in.readLine().trimmed());
          }

          file.close();
        }
      }
      else{
        (*log).append(QString("No such directory: %1\n").arg(labellist[i]));
      }
    }
  }

  if(varlabelsdir.exists() == true){
    QStringList labellist = varlabelsdir.entryList();
    labellist.removeAll(".");
    labellist.removeAll("..");
    for(int i = 0; i < labellist.size(); i++){
      if(QFile::exists(varlabelsdir.absolutePath()+"/"+labellist[i]) == true){
        QFile file(varlabelsdir.absolutePath()+"/"+labellist[i]);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
          (*log).append(QString("No such directory: %1\n").arg(labellist[i]));
        }
        else{
          getVariableLabels().append(LABEL());
          QString name = labellist[i];
          name.remove(".txt", Qt::CaseInsensitive);
          getVariableLabels().last().name = name;
          QTextStream in(&file);
          while(!in.atEnd()){
            getVariableLabels().last().objects.append(in.readLine().trimmed());
          }

          file.close();
        }
      }
      else{
        (*log).append(QString("No such directory: %1\n").arg(labellist[i]));
      }
    }
  }

  QDir tabmxdir = QDir(vartablabelsdir.absolutePath());
  if(tabmxdir.exists() == true){
    QStringList mxlist = tabmxdir.entryList();
    mxlist.removeAll(".");
    mxlist.removeAll("..");
    for(int i = 0; i < mxlist.size(); i++){
      QDir mx = QDir(tabmxdir.absolutePath()+"/"+mxlist[i]);
      if(mx.exists() == true){
        if(QFile::exists(mx.absolutePath()+"/data.txt") == true
          && QFile::exists(mx.absolutePath()+"/label.txt") == true
          && QFile::exists(mx.absolutePath()+"/varname.txt") == true){

          getVariableTabLabels().append(new TABLABEL());
          getVariableTabLabels().last()->setName(mxlist[i]);
          DATAIO::ImportMatrix(QString("%1/%2").arg(mx.absolutePath()).arg("/data.txt").toUtf8().data(),
                              "\t",
                                getVariableTabLabels().last()->getMatrix());

          ImportRows(mx.absolutePath()+"/label.txt", getVariableTabLabels().last()->getObjectsName());
          ImportRows(mx.absolutePath()+"/varname.txt", getVariableTabLabels().last()->getFeaturesName());
        }
        else{
          (*log).append(QString("Corrupted Matrix: %1\n").arg(mx.absolutePath()));
        }
      }
      else{
        (*log).append(QString("No such Matrix: %1\n").arg(mx.absolutePath()));
      }
    }
  }

  if(imgdir.exists() && !imgdir.entryInfoList().isEmpty()){
    QFileInfo dirinfo(imgdir.absolutePath());
    QFileInfoList list = imgdir.entryInfoList();
    for(int i = 0; i < list.size(); i++){
      QFileInfo fileInfo = list.at(i);
      QStringList split = fileInfo.fileName().split(".", QString::SkipEmptyParts);
      if(!split.isEmpty()){
        if(split.last().toLower().compare("jpg") == 0 ||
          split.last().toLower().compare("jpeg") == 0 ||
          split.last().toLower().compare("png") == 0 ||
          split.last().toLower().compare("bmp") == 0 ||
          split.last().toLower().compare("ppm") == 0 ||
          split.last().toLower().compare("xbm") == 0 ||
          split.last().toLower().compare("xpm") == 0){

          /* check if already exist a file with this name....*/
          bool duplicate = false;
          for(int k = 0; k < getImages().size(); k++){
            if(getImages()[k].name.compare(fileInfo.fileName()) == 0){
              duplicate = true;
              break;
            }
            else{
              continue;
            }
          }

          if(duplicate == false){
            getImages().append(IMAGE());
            getImages().last().name = fileInfo.fileName().split(".", QString::SkipEmptyParts).first();
            getImages().last().filepath = fileInfo.absoluteFilePath();
            getImages().last().image = QPixmap(fileInfo.absoluteFilePath());
          }
          else{
            continue;
          }
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

  if(modelsdir.exists() == true){
    QDir pcadir = QDir(modelsdir.absolutePath()+"/PCAMODELS");
    QDir plsdir = QDir(modelsdir.absolutePath()+"/PLSMODELS");
    QDir varseldir = QDir(modelsdir.absolutePath()+"/VARIABLESELECTIONS");
    QDir mlrdir = QDir(modelsdir.absolutePath()+"/MLRMODELS");
    QDir ldadir = QDir(modelsdir.absolutePath()+"/LDAMODELS");
    if(pcadir.exists() == true){
      QStringList pcalist = pcadir.entryList();
      pcalist.removeAll(".");
      pcalist.removeAll("..");
      for(int i = 0; i < pcalist.size(); i++){
        QDir pca = QDir(pcadir.absolutePath()+"/"+pcalist[i]);
        if(pca.exists() == true
          && QFile::exists(pca.absolutePath()+"/info.txt") == true){
          (*log).append(QString("Importing PCA Model %1").arg(pcalist[i]));
          addPCAModel();
          getLastPCAModel()->ImportPCAModel(pca.absolutePath().toUtf8().data(), QString("PCA - %1").arg(pcalist[i].trimmed().toUtf8().data()).toUtf8().data());
          getLastPCAModel()->setModelID((*mid_));

          int xid = -1;
          for(int j = 0; j < MatrixCount(); j++){
            if(getMatrix(j)->getHash().compare(getLastPCAModel()->getDataHash()) == 0){
              xid = j;
              break;
            }
          }

          if(xid == -1){
            (*log).append(QString("Warning on PCA Model %1. Unable to find data matrix.").arg(pcalist[i]));
          }

          getLastPCAModel()->setDID(xid);

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
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
          MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
          (*tabcount_)++;

          #ifdef DEBUG
          qDebug() << "PCA NAME: " << subitem->text(0) << " TABCOUNT " << subitem->text(1) << " Project ID " << subitem->text(2) << " XHASH " << subitem->text(3) << " YHASH" << subitem->text(4) << " XSCALING " << subitem->text(5) <<" YSCALING " << subitem->text(6) << "Number of Components " << subitem->text(7) << " MODEL TYPE " << subitem->text(8) << " MODEL ID " << subitem->text(9);
          #endif

	  QDir pcapred = pca.absolutePath()+"/PREDICTIONS";
          if(pcapred.exists() == true){
            QStringList pcapredlist = pcapred.entryList();
            pcapredlist.removeAll(".");
            pcapredlist.removeAll("..");
            for(int j = 0; j < pcapredlist.size(); j++){
              QDir pcapredpath = QDir(QString::fromUtf8(pcapred.absolutePath().toUtf8()+"/"+pcapredlist[j].toUtf8()));
              (*log).append(QString("Importing PCA Prediction %1").arg(pcapredlist[j]));
              if(pcapredpath.exists() == true
                && QFile::exists(pcapredpath.absolutePath()+"/T-Scores-Pred.txt") == true
                && QFile::exists(pcapredpath.absolutePath()+"/info.txt") == true){
                #ifdef DEBUG
		qDebug() << pcapredpath.absolutePath() << " PCA Prediction - " + pcapredlist[j];
                #endif
		getLastPCAModel()->addPCAPrediction();
                getLastPCAModel()->getLastPCAPrediction()->ImportPCAPrediction(pcapredpath.absolutePath().toUtf8().data(), QString("PCA Prediction - %1").arg(QString::fromUtf8(pcapredlist[j].toUtf8())).toUtf8().data());
                getLastPCAModel()->getLastPCAPrediction()->setPredID(getLastPCAModel()->PCAPredictionCount()-1);

                int predid = -1;
                for(int k = 0; k <  MatrixCount(); k++){
                  if(getMatrix(k)->getHash().compare(getLastPCAModel()->getLastPCAPrediction()->getDataHash()) == 0){
                    predid = k;
                    break;
                  }
                  else{
                    continue;
                  }
                }

                if(predid == -1){
                  (*log).append(QString("Warning on PCA Prediction %1. Unable to find data matrix.").arg(pcapredlist[i]));
                }

                getLastPCAModel()->getLastPCAPrediction()->setDID(predid);
        //         ModelPrediction Name - Tab Count - pid_ - Model ID - xdata id - ydata id - Data Position - Data Type (PCA Prediction, UPCA Prediction, ...) (8)
                QTreeWidgetItem *subitem = new QTreeWidgetItem;
                subitem->setText(0, getPCAModel((*mid_))->getLastPCAPrediction()->getName());
                subitem->setText(1, QString::number((*tabcount_)));
                subitem->setText(2, QString::number(getProjectID()));
                subitem->setText(3, QString::number((*mid_)));
                subitem->setText(4, getLastPCAModel()->getLastPCAPrediction()->getDataHash());
                subitem->setText(5, "-");
                subitem->setText(6, QString::number(getPCAModel((*mid_))->getLastPCAPrediction()->getPredID()));
                subitem->setText(7, QString("PCA Prediction"));
                (*tabcount_)++;
                MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
              }
            }
          }
          (*mid_)++;
        }
        else{
          (*log).append(QString("Error PCA Model %1 Corrupted").arg(pcalist[i]));
        }
      }
    }

    if(plsdir.exists() == true){
      QStringList plslist = plsdir.entryList();
      plslist.removeAll(".");
      plslist.removeAll("..");
      for(int i = 0; i < plslist.size(); i++){
        QDir pls = QDir(QString::fromUtf8(plsdir.absolutePath().toUtf8())+"/"+plslist[i]);
        if(pls.exists() == true
          && QFile::exists(QString::fromUtf8(pls.absolutePath().toUtf8())+"/info.txt") == true){
          (*log).append(QString("Importing PLS Model %1").arg(plslist[i]));
          addPLSModel();
          getLastPLSModel()->ImportPLSModel(pls.absolutePath().toUtf8().data(), QString("PLS - %2").arg(QString::fromUtf8(plslist[i].toUtf8())).toUtf8().data());
          getLastPLSModel()->setModelID((*mid_));

          int did = -1;
          for(int j = 0; j < MatrixCount(); j++){
            if(getMatrix(j)->getHash().compare(getLastPLSModel()->getDataHash()) == 0){
              did = j;
            }
            else{
              continue;
            }
          }

          if(did == -1){
            (*log).append(QString("Warning on PLS Model %1. Unable to find data matrix.").arg(plslist[i]));
          }

          getLastPLSModel()->setDID(did);
          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, "PLS - "+plslist[i]);
          subitem->setText(1, QString::number((*tabcount_)));
          subitem->setText(2, QString::number(getProjectID()));
          subitem->setText(3, getLastPLSModel()->getDataHash());
          subitem->setText(4, getLastPLSModel()->getDataHash());
          subitem->setText(5, QString::number(getLastPLSModel()->getXScaling()));
          subitem->setText(6, QString::number(getLastPLSModel()->getYScaling()));
          subitem->setText(7, QString::number(getLastPLSModel()->getNPC()));
          subitem->setText(8, QString("PLS Model"));
          subitem->setText(9, QString::number((*mid_)));
          MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
          (*tabcount_)++;

          QDir plspred = pls.absolutePath()+"/PREDICTIONS";
          if(plspred.exists() == true){
            QStringList plspredlist = plspred.entryList();
            plspredlist.removeAll(".");
            plspredlist.removeAll("..");
            for(int j = 0; j < plspredlist.size(); j++){
              QDir plspredpath = QDir(QString::fromUtf8(plspred.absolutePath().toUtf8())+"/"+plspredlist[j]);
              (*log).append(QString("Importing PLS Prediction %1").arg(plspredlist[j]));
              if(plspredpath.exists()
                && QFile::exists(QString::fromUtf8(plspredpath.absolutePath().toUtf8())+"/Y-Pred.txt") == true
                && QFile::exists(QString::fromUtf8(plspredpath.absolutePath().toUtf8())+"/T-Scores-Pred.txt") == true
                && QFile::exists(QString::fromUtf8(plspredpath.absolutePath().toUtf8())+"/info.txt") == true){
                getLastPLSModel()->addPLSPrediction();
                getLastPLSModel()->getLastPLSPrediction()->ImportPLSPrediction(plspredpath.absolutePath().toUtf8().data(), QString::fromUtf8("PLS Prediction - %1").arg(QString::fromUtf8(plspredlist[j].toUtf8())).toUtf8().data());
                getLastPLSModel()->getLastPLSPrediction()->setPredID(getLastPLSModel()->PLSPredictionCount()-1);

                int xpredid = -1;
                for(int k = 0; k <  MatrixCount(); k++){
                  if(getMatrix(k)->getHash().compare(getLastPLSModel()->getLastPLSPrediction()->getDataHash()) == 0){
                    xpredid = k;
                    break;
                  }
                  else{
                    continue;
                  }
                }

                if(xpredid == -1){
                  (*log).append(QString("Warning in PLS Prediction %1. Unable to find data matrix. ").arg(plspredlist[j]));
                }

                getLastPLSModel()->getLastPLSPrediction()->setDID(xpredid);

        //         ModelPrediction Name - Tab Count - pid_ - Model ID - xdata id - ydata id - Data Position - Data Type (PLS Prediction, UPLS Prediction, ...) (8)
                QTreeWidgetItem *subitem = new QTreeWidgetItem;
                subitem->setText(0, getPLSModel((*mid_))->getLastPLSPrediction()->getName());
                subitem->setText(1, QString::number((*tabcount_)));
                subitem->setText(2, QString::number(getProjectID()));
                subitem->setText(3, QString::number((*mid_)));
                subitem->setText(4, getLastPLSModel()->getLastPLSPrediction()->getDataHash());
                subitem->setText(5, getLastPLSModel()->getLastPLSPrediction()->getDataHash());
                subitem->setText(6, QString::number(getPLSModel((*mid_))->getLastPLSPrediction()->getPredID()));
                subitem->setText(7, QString("PLS Prediction"));
                (*tabcount_)++;
                MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
              }
            }
          }
          (*mid_)++;
        }
        else{
          (*log).append(QString("Error PLS Model %1 Corrupted").arg(plslist[i]));
        }
      }
    }

    if(varseldir.exists() == true){
      QStringList varsellist = varseldir.entryList();
      varsellist.removeAll(".");
      varsellist.removeAll("..");
      for(int i = 0; i < varsellist.size(); i++){
        QDir varsel = QDir(varseldir.absolutePath()+"/"+varsellist[i]);
        if(varsel.exists() == true
          && QFile::exists(varsel.absolutePath()+"/info.txt") == true){
          (*log).append(QString("Importing Variable Selection Model %1").arg(varsellist[i]));
          addVarSelModel();
          getLastVarSelModel()->ImportVarSelModel(varsel.absolutePath(), "VSEL - "+varsellist[i].trimmed());
          getLastVarSelModel()->setModelID((*mid_));

          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, getLastVarSelModel()->getName());
          subitem->setText(1, QString::number((*tabcount_)));
          subitem->setText(2, QString::number(getProjectID()));
          subitem->setText(3, getLastVarSelModel()->getModelHash());
          subitem->setText(4, "-");
          subitem->setText(5, "-");
          subitem->setText(6, "-");
          subitem->setText(7, "-");
          subitem->setText(8, QString("PLS Variable Selection Model"));
          subitem->setText(9, QString::number((*mid_)));
          MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
          (*tabcount_)++;
          (*mid_)++;
        }
        else{
          (*log).append(QString("Error PCA Model %1 Corrupted").arg(varsellist[i]));
        }
      }
    }

    if(mlrdir.exists() == true){
      QStringList mlrlist = mlrdir.entryList();
      mlrlist.removeAll(".");
      mlrlist.removeAll("..");
      for(int i = 0; i < mlrlist.size(); i++){
        QDir mlr = QDir(QString::fromUtf8(mlrdir.absolutePath().toUtf8())+"/"+mlrlist[i]);
        if(mlr.exists() == true
          && QFile::exists(QString::fromUtf8(mlr.absolutePath().toUtf8())+"/info.txt") == true){
          (*log).append(QString("Importing MLR Model %1").arg(mlrlist[i]));
          addMLRModel();
          getLastMLRModel()->ImportMLRModel(mlr.absolutePath().toUtf8().data(), QString("MLR - %2").arg(QString::fromUtf8(mlrlist[i].toUtf8())).toUtf8().data());
          getLastMLRModel()->setModelID((*mid_));

          int did = -1;
          for(int j = 0; j < MatrixCount(); j++){
            if(getMatrix(j)->getHash().compare(getLastMLRModel()->getDataHash()) == 0){
              did = j;
            }
            else{
              continue;
            }
          }

          if(did == -1){
            (*log).append(QString("Warning on MLR Model %1. Unable to find data matrix.").arg(mlrlist[i]));
          }

          getLastMLRModel()->setDID(did);
          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, "MLR - "+mlrlist[i]);
          subitem->setText(1, QString::number((*tabcount_)));
          subitem->setText(2, QString::number(getProjectID()));
          subitem->setText(3, getLastMLRModel()->getDataHash());
          subitem->setText(4, getLastMLRModel()->getDataHash());
          subitem->setText(5, "-");
          subitem->setText(6, "-");
          subitem->setText(7, "-");
          subitem->setText(8, QString("MLR Model"));
          subitem->setText(9, QString::number((*mid_)));
          MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
          (*tabcount_)++;

          QDir mlrpred = mlr.absolutePath()+"/PREDICTIONS";
          if(mlrpred.exists() == true){
            QStringList mlrpredlist = mlrpred.entryList();
            mlrpredlist.removeAll(".");
            mlrpredlist.removeAll("..");
            for(int j = 0; j < mlrpredlist.size(); j++){
              QDir mlrpredpath = QDir(QString::fromUtf8(mlrpred.absolutePath().toUtf8())+"/"+mlrpredlist[j]);
              (*log).append(QString("Importing MLR Prediction %1").arg(mlrpredlist[j]));
              if(mlrpredpath.exists()
                && QFile::exists(QString::fromUtf8(mlrpredpath.absolutePath().toUtf8())+"/Y-Pred.txt") == true
                && QFile::exists(QString::fromUtf8(mlrpredpath.absolutePath().toUtf8())+"/info.txt") == true){
                getLastMLRModel()->addMLRPrediction();
                getLastMLRModel()->getLastMLRPrediction()->ImportMLRPrediction(mlrpredpath.absolutePath().toUtf8().data(), QString::fromUtf8("MLR Prediction - %1").arg(QString::fromUtf8(mlrpredlist[j].toUtf8())).toUtf8().data());
                getLastMLRModel()->getLastMLRPrediction()->setPredID(getLastMLRModel()->MLRPredictionCount()-1);

                int xpredid = -1;
                for(int k = 0; k <  MatrixCount(); k++){
                  if(getMatrix(k)->getHash().compare(getLastMLRModel()->getLastMLRPrediction()->getDataHash()) == 0){
                    xpredid = k;
                    break;
                  }
                  else{
                    continue;
                  }
                }

                if(xpredid == -1){
                  (*log).append(QString("Warning in MLR Prediction %1. Unable to find data matrix. ").arg(mlrpredlist[j]));
                }

                getLastMLRModel()->getLastMLRPrediction()->setDID(xpredid);

        //         ModelPrediction Name - Tab Count - pid_ - Model ID - xdata id - ydata id - Data Position - Data Type (MLR Prediction, UMLR Prediction, ...) (8)
                QTreeWidgetItem *subitem = new QTreeWidgetItem;
                subitem->setText(0, getMLRModel((*mid_))->getLastMLRPrediction()->getName());
                subitem->setText(1, QString::number((*tabcount_)));
                subitem->setText(2, QString::number(getProjectID()));
                subitem->setText(3, QString::number((*mid_)));
                subitem->setText(4, getLastMLRModel()->getLastMLRPrediction()->getDataHash());
                subitem->setText(5, getLastMLRModel()->getLastMLRPrediction()->getDataHash());
                subitem->setText(6, QString::number(getMLRModel((*mid_))->getLastMLRPrediction()->getPredID()));
                subitem->setText(7, QString("MLR Prediction"));
                (*tabcount_)++;
                MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
              }
            }
          }
          (*mid_)++;
        }
        else{
          (*log).append(QString("Error MLR Model %1 Corrupted").arg(mlrlist[i]));
        }
      }
    }

    if(ldadir.exists() == true){
      QStringList ldalist = ldadir.entryList();
      ldalist.removeAll(".");
      ldalist.removeAll("..");
      for(int i = 0; i < ldalist.size(); i++){
        QDir lda = QDir(QString::fromUtf8(ldadir.absolutePath().toUtf8())+"/"+ldalist[i]);
        if(lda.exists() == true
          && QFile::exists(QString::fromUtf8(lda.absolutePath().toUtf8())+"/info.txt") == true){
          (*log).append(QString("Importing LDA Model %1").arg(ldalist[i]));
          addLDAModel();
          getLastLDAModel()->ImportLDAModel(lda.absolutePath().toUtf8().data(), QString("LDA - %2").arg(QString::fromUtf8(ldalist[i].toUtf8())).toUtf8().data());
          getLastLDAModel()->setModelID((*mid_));

          int did = -1;
          for(int j = 0; j < MatrixCount(); j++){
            if(getMatrix(j)->getHash().compare(getLastLDAModel()->getDataHash()) == 0){
              did = j;
            }
            else{
              continue;
            }
          }

          if(did == -1){
            (*log).append(QString("Warning on LDA Model %1. Unable to find data matrix.").arg(ldalist[i]));
          }

          getLastLDAModel()->setDID(did);
          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, "LDA - "+ldalist[i]);
          subitem->setText(1, QString::number((*tabcount_)));
          subitem->setText(2, QString::number(getProjectID()));
          subitem->setText(3, getLastLDAModel()->getDataHash());
          subitem->setText(4, getLastLDAModel()->getDataHash());
          subitem->setText(5, "-");
          subitem->setText(6, "-");
          subitem->setText(7, "-");
          subitem->setText(8, QString("LDA Model"));
          subitem->setText(9, QString::number((*mid_)));
          MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
          (*tabcount_)++;

          QDir ldapred = lda.absolutePath()+"/PREDICTIONS";
          if(ldapred.exists() == true){
            QStringList ldapredlist = ldapred.entryList();
            ldapredlist.removeAll(".");
            ldapredlist.removeAll("..");
            for(int j = 0; j < ldapredlist.size(); j++){
              QDir ldapredpath = QDir(QString::fromUtf8(ldapred.absolutePath().toUtf8())+"/"+ldapredlist[j]);
              (*log).append(QString("Importing LDA Prediction %1").arg(ldapredlist[j]));
              if(ldapredpath.exists() && QFile::exists(QString::fromUtf8(ldapredpath.absolutePath().toUtf8())+"/info.txt") == true){
                getLastLDAModel()->addLDAPrediction();
                getLastLDAModel()->getLastLDAPrediction()->ImportLDAPrediction(ldapredpath.absolutePath().toUtf8().data(), QString::fromUtf8("LDA Prediction - %1").arg(QString::fromUtf8(ldapredlist[j].toUtf8())).toUtf8().data());
                getLastLDAModel()->getLastLDAPrediction()->setPredID(getLastLDAModel()->LDAPredictionCount()-1);

                int xpredid = -1;
                for(int k = 0; k <  MatrixCount(); k++){
                  if(getMatrix(k)->getHash().compare(getLastLDAModel()->getLastLDAPrediction()->getDataHash()) == 0){
                    xpredid = k;
                    break;
                  }
                  else{
                    continue;
                  }
                }

                if(xpredid == -1){
                  (*log).append(QString("Warning in LDA Prediction %1. Unable to find data matrix. ").arg(ldapredlist[j]));
                }

                getLastLDAModel()->getLastLDAPrediction()->setDID(xpredid);

        //         ModelPrediction Name - Tab Count - pid_ - Model ID - xdata id - ydata id - Data Position - Data Type (LDA Prediction, ULDA Prediction, ...) (8)
                QTreeWidgetItem *subitem = new QTreeWidgetItem;
                subitem->setText(0, getLDAModel((*mid_))->getLastLDAPrediction()->getName());
                subitem->setText(1, QString::number((*tabcount_)));
                subitem->setText(2, QString::number(getProjectID()));
                subitem->setText(3, QString::number((*mid_)));
                subitem->setText(4, getLastLDAModel()->getLastLDAPrediction()->getDataHash());
                subitem->setText(5, getLastLDAModel()->getLastLDAPrediction()->getDataHash());
                subitem->setText(6, QString::number(getLDAModel((*mid_))->getLastLDAPrediction()->getPredID()));
                subitem->setText(7, QString("LDA Prediction"));
                (*tabcount_)++;
                MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
              }
            }
          }
          (*mid_)++;
        }
        else{
          (*log).append(QString("Error LDA Model %1 Corrupted").arg(ldalist[i]));
        }
      }
    }
  }
}

bool DATA::isSQLDatabase(QString sqlfile)
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(sqlfile);
  if(db.open() == true){
    QSqlQuery query = QSqlQuery(db);
    query.exec("PRAGMA table_info(pcaTable)");
    int i = 0;
    while(query.next()) {
      //fields << query.value(1).toString();
      i++;
    }

    if(i > 0){
      return true;
    }
    else{
      return false;
    }
  }
  else{
    qDebug() << "FALSE";
    return false;
  }
}
void DATA::OpenSQLData(QString sqlfile, QTreeWidget *treeWidget, int *tabcount_, int *mid_, QStringList *log)
{
  /* Warning!! there are some Methods from MainWindow and they works only if the ui is started... */
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(sqlfile);
  db.open();
  QSqlQuery query = QSqlQuery(db);

  query.exec("SELECT * from matrixTable");
  while (query.next()){
    // get the query values
    QString name = query.value(0).toString();
    QString s_objname = query.value(1).toString();
    QString s_varname = query.value(2).toString();
    QString s_matrix = query.value(3).toString();
    //add the data matrix
    addMatrix();
    getMatrix(MatrixCount()-1)->setName(name);
    getMatrix(MatrixCount()-1)->getObjName() = DeserializeQStringList(s_objname);
    getMatrix(MatrixCount()-1)->getVarName() = DeserializeQStringList(s_varname);
    DeserializeMatrix(s_matrix, &getMatrix(MatrixCount()-1)->Matrix());

    // Add matrix to the treeview
    QTreeWidgetItem *subitem = new QTreeWidgetItem;
    subitem->setText(0, name); /*set the data name from the file*/
    subitem->setText(1, QString("Matrix")); // Define the type of the data
    subitem->setText(2, QString::number((*tabcount_))); // Define the tab id number in order to close a specific table
    subitem->setText(3, QString::number(MatrixCount()-1)); // Define the matrix position id in order to find easly when you need to show data.
    subitem->setText(4, QString::number(getProjectID())); // pid for get the array with Value
    (*tabcount_)++;
    MainWindow::getProjectItem(getProjectID(), treeWidget)->child(0)->addChild(subitem);
    (*log).append(QString("Matrix %1 imported.\n").arg(name));
  }

  query.exec("SELECT * from arrayTable");
  while (query.next()){
    // get the query values
    QString name = query.value(0).toString();
    QString s_objname = query.value(1).toString();
    QString s_varname = query.value(2).toString();
    QString s_array = query.value(3).toString();
    //add the data matrix
    addArray();
    getArray(ArrayCount()-1)->setName(name);
    getArray(ArrayCount()-1)->getObjName() = DeserializeQStringList(s_objname);
    getArray(ArrayCount()-1)->getVarName() = DeserializeQStringList(s_varname);
    DeserializeArray(s_array, &getArray(ArrayCount()-1)->Array());

    // Add matrix to the treeview
    QTreeWidgetItem *subitem = new QTreeWidgetItem;
    subitem->setText(0, name); /*set the data name from the file*/
    subitem->setText(1, QString("Array")); // Define the type of the data
    subitem->setText(2, QString::number((*tabcount_))); // Define the tab id number in order to close a specific table
    subitem->setText(3, QString::number(MatrixCount()-1)); // Define the matrix position id in order to find easly when you need to show data.
    subitem->setText(4, QString::number(getProjectID())); // pid for get the array with Value
    (*tabcount_)++;
    MainWindow::getProjectItem(getProjectID(), treeWidget)->child(0)->addChild(subitem);
    (*log).append(QString("Array %1 imported.\n").arg(name));
  }

  query.exec("SELECT * from objlabelTable");
  while (query.next()){
    // get the query values
    //query.exec(QString("CREATE TABLE IF NOT EXISTS objlabelTable (name TEXT, values TEXT)"));
    QString name = query.value(0).toString();
    QString s_values = query.value(1).toString();
    getObjectLabels().append(LABEL());
    getObjectLabels().last().name = name;
    getObjectLabels().last().objects = DeserializeQStringList(s_values);
    (*log).append(QString("Object label %1 imported.\n").arg(name));
  }

  query.exec("SELECT * from varlabelTable");
  while (query.next()){
    // get the query values
    QString name = query.value(0).toString();
    QString s_values = query.value(1).toString();
    getVariableLabels().append(LABEL());
    getVariableLabels().last().name = name;
    getVariableLabels().last().objects = DeserializeQStringList(s_values);
    (*log).append(QString("Variable label %1 imported.\n").arg(name));
  }

  query.exec("SELECT * from vartablabelTable");
  while (query.next()){
    // get the query values
    QString name = query.value(0).toString();
    QString s_objname = query.value(1).toString();
    QString s_varname = query.value(2).toString();
    QString s_matrix = query.value(3).toString();

    getVariableTabLabels().append(new TABLABEL());
    getVariableTabLabels().last()->setName(name);
    getVariableTabLabels().last()->getObjectsName() = DeserializeQStringList(s_objname);
    getVariableTabLabels().last()->getFeaturesName() = DeserializeQStringList(s_varname);
    matrix *tmp = getVariableTabLabels().last()->getMatrix();
    DeserializeMatrix(s_matrix, &tmp);

    (*log).append(QString("Variable table label %1 imported.\n").arg(name));
  }

  query.exec("SELECT * from imgTable");
  while (query.next()){
    QString name = query.value(0).toString();
    QByteArray outByteArray = query.value(1).toByteArray();
    getImages().append(IMAGE());
    getImages().last().name = name;
    //getImages().last().filepath = fileInfo.absoluteFilePath();
    getImages().last().image.loadFromData(outByteArray);
  }

  //(name TEXT, pcahash TEXT, hashinputmx TEXT, objname TEXT, scores TEXT)
  query.exec("SELECT * from pcapredTable");
  QList<QStringList> pcapredlist;

  while (query.next()){
    pcapredlist.append(QStringList());
    pcapredlist.last().append(query.value(0).toString());
    pcapredlist.last().append(query.value(1).toString());
    pcapredlist.last().append(query.value(2).toString());
    pcapredlist.last().append(query.value(3).toString());
    pcapredlist.last().append(query.value(4).toString());
  }

  query.exec("SELECT * from pcaTable");
  while (query.next()){
    // get the query values
    QString name = query.value(0).toString();
    int npc = query.value(1).toInt();
    int xscaling = query.value(2).toInt();
    QString hashinputmx = query.value(3).toString();
    QString s_objname = query.value(4).toString();
    QString s_varname = query.value(5).toString();
    QString s_scores = query.value(6).toString();
    QString s_loadings = query.value(7).toString();
    QString s_varexp = query.value(8).toString();
    QString s_colscaling = query.value(9).toString();
    QString s_colaverage = query.value(10).toString();

    //add the data matrix
    addPCAModel();
    getLastPCAModel()->setModelID((*mid_));
    getLastPCAModel()->setName(name);
    getLastPCAModel()->setNPC(npc);
    getLastPCAModel()->setXScaling(xscaling);
    getLastPCAModel()->setDataHash(hashinputmx);
    getLastPCAModel()->getObjName() = DeserializeQStringList(s_objname);
    getLastPCAModel()->getVarName() = DeserializeQStringList(s_varname);
    DeserializeMatrix(s_scores, &getLastPCAModel()->Model()->scores);
    DeserializeMatrix(s_loadings, &getLastPCAModel()->Model()->loadings);
    DeserializeDVector(s_colscaling, &getLastPCAModel()->Model()->colscaling);
    DeserializeDVector(s_colaverage, &getLastPCAModel()->Model()->colaverage);
    DeserializeDVector(s_varexp, &getLastPCAModel()->Model()->varexp);

    int xid = -1;
    for(int j = 0; j < MatrixCount(); j++){
      if(getMatrix(j)->getHash().compare(hashinputmx) == 0){
        xid = j;
        break;
      }
    }

    if(xid == -1){
      (*log).append(QString("Warning on PCA Model %1. Unable to find origin data matrix.").arg(name));
    }

    getLastPCAModel()->setDID(xid);

    QTreeWidgetItem *subitem = new QTreeWidgetItem;
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
    MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
    (*tabcount_)++;
    (*log).append(QString("PCA model %1 imported.\n").arg(name));

    QString pcahash = getLastPCAModel()->getHash();
    for(int i = 0; i < pcapredlist.size(); i++){
      if(pcapredlist[i][1].compare(pcahash) == 0){
        getLastPCAModel()->addPCAPrediction();
        getLastPCAModel()->getLastPCAPrediction()->setPredID(getLastPCAModel()->PCAPredictionCount()-1);
        getLastPCAModel()->getLastPCAPrediction()->setName(pcapredlist[i][0]);
        getLastPCAModel()->getLastPCAPrediction()->setDataHash(pcapredlist[i][2]);
        getLastPCAModel()->getLastPCAPrediction()->getObjName() = DeserializeQStringList(pcapredlist[i][3]);
        DeserializeMatrix(pcapredlist[i][4], getLastPCAModel()->getLastPCAPrediction()->PredScoresPointer());

        int predid = -1;
        for(int k = 0; k <  MatrixCount(); k++){
          if(getMatrix(k)->getHash().compare(getLastPCAModel()->getLastPCAPrediction()->getDataHash()) == 0){
            predid = k;
            break;
          }
          else{
            continue;
          }
        }

        if(predid == -1){
          (*log).append(QString("Warning on PCA Prediction %1. Unable to find data matrix.").arg(pcapredlist[i][0]));
        }

        getLastPCAModel()->getLastPCAPrediction()->setDID(predid);
        //         ModelPrediction Name - Tab Count - pid_ - Model ID - xdata id - ydata id - Data Position - Data Type (PCA Prediction, UPCA Prediction, ...) (8)
        QTreeWidgetItem *subitem = new QTreeWidgetItem;
        subitem->setText(0, getPCAModel((*mid_))->getLastPCAPrediction()->getName());
        subitem->setText(1, QString::number((*tabcount_)));
        subitem->setText(2, QString::number(getProjectID()));
        subitem->setText(3, QString::number((*mid_)));
        subitem->setText(4, getLastPCAModel()->getLastPCAPrediction()->getDataHash());
        subitem->setText(5, "-");
        subitem->setText(6, QString::number(getPCAModel((*mid_))->getLastPCAPrediction()->getPredID()));
        subitem->setText(7, QString("PCA Prediction"));
        (*tabcount_)++;
        MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
      }
      else{
        continue;
      }
    }
    (*mid_)++;
  }


  query.exec("SELECT * from plspredTable");
  QList<QStringList> plspredlist;

  while (query.next()){
    plspredlist.append(QStringList());
    plspredlist.last().append(query.value(0).toString());
    plspredlist.last().append(query.value(1).toString());
    plspredlist.last().append(query.value(2).toString());
    plspredlist.last().append(query.value(3).toString());
    plspredlist.last().append(query.value(4).toString());
    plspredlist.last().append(query.value(5).toString());
    plspredlist.last().append(query.value(6).toString());
    plspredlist.last().append(query.value(7).toString());
    plspredlist.last().append(query.value(8).toString());
  }

  query.exec("SELECT * from plsTable");
  while (query.next()){
    // get the query values
    QString name = query.value(0).toString();
    int npc = query.value(1).toInt();
    int xscaling = query.value(2).toInt();
    int yscaling = query.value(3).toInt();
    QString hashinputmx = query.value(4).toString();
    QString s_objname = query.value(5).toString();
    QString s_xvarname = query.value(6).toString();
    QString s_yvarname = query.value(7).toString();
    QString s_tscores = query.value(8).toString();
    QString s_ploadings = query.value(9).toString();
    QString s_weights = query.value(10).toString();
    QString s_xvarexp = query.value(11).toString();
    QString s_xcolscaling = query.value(12).toString();
    QString s_xcolaverage = query.value(13).toString();
    QString s_uscores = query.value(14).toString();
    QString s_qloadings = query.value(15).toString();
    QString s_ycolscaling = query.value(16).toString();
    QString s_ycolaverage = query.value(17).toString();
    QString s_b = query.value(18).toString();
    QString s_r2y_model = query.value(19).toString();
    QString s_sdec = query.value(20).toString();
    QString s_recalc_y = query.value(21).toString();
    QString s_recalc_residuals = query.value(22).toString();
    int validationtype = query.value(23).toInt();
    QString s_q2y = query.value(24).toString();
    QString s_sdep = query.value(25).toString();
    QString s_bias = query.value(26).toString();
    QString s_predicted_y = query.value(27).toString();
    QString s_predicted_residuals = query.value(28).toString();
    QString s_r2q2scrambling  = query.value(29).toString();
    QString s_q2_sample_validation = query.value(30).toString();
    QString s_sdep_sample_validation = query.value(31).toString();
    QString s_q2_sample_validation_surface = query.value(32).toString();
    QString s_sdep_sample_validation_surface = query.value(33).toString();

    //add the data matrix
    addPLSModel();
    getLastPLSModel()->setModelID((*mid_));
    getLastPLSModel()->setName(name);
    getLastPLSModel()->setNPC(npc);
    getLastPLSModel()->setXScaling(xscaling);
    getLastPLSModel()->setYScaling(yscaling);
    getLastPLSModel()->setDataHash(hashinputmx);
    getLastPLSModel()->getObjName() = DeserializeQStringList(s_objname);
    getLastPLSModel()->getXVarName() = DeserializeQStringList(s_xvarname);
    getLastPLSModel()->getYVarName() = DeserializeQStringList(s_yvarname);
    DeserializeMatrix(s_tscores, &getLastPLSModel()->Model()->xscores);
    DeserializeMatrix(s_ploadings, &getLastPLSModel()->Model()->xloadings);
    DeserializeMatrix(s_weights, &getLastPLSModel()->Model()->xweights);
    DeserializeDVector(s_xvarexp, &getLastPLSModel()->Model()->xvarexp);
    DeserializeDVector(s_xcolaverage, &getLastPLSModel()->Model()->xcolaverage);
    DeserializeDVector(s_xcolscaling, &getLastPLSModel()->Model()->xcolscaling);
    DeserializeMatrix(s_uscores, &getLastPLSModel()->Model()->yscores);
    DeserializeMatrix(s_qloadings, &getLastPLSModel()->Model()->yloadings);
    DeserializeDVector(s_ycolaverage, &getLastPLSModel()->Model()->ycolaverage);
    DeserializeDVector(s_ycolscaling, &getLastPLSModel()->Model()->ycolscaling);
    DeserializeDVector(s_b, &getLastPLSModel()->Model()->b);
    DeserializeMatrix(s_r2y_model, &getLastPLSModel()->Model()->r2y_model);
    DeserializeMatrix(s_sdec, &getLastPLSModel()->Model()->sdec);
    DeserializeMatrix(s_recalc_y, &getLastPLSModel()->Model()->recalculated_y);
    DeserializeMatrix(s_recalc_residuals, &getLastPLSModel()->Model()->recalc_residuals);
    getLastPLSModel()->setValidation(validationtype);
    DeserializeMatrix(s_q2y, &getLastPLSModel()->Model()->q2y);
    DeserializeMatrix(s_sdep, &getLastPLSModel()->Model()->sdep);
    DeserializeMatrix(s_bias, &getLastPLSModel()->Model()->bias);
    DeserializeMatrix(s_predicted_y, &getLastPLSModel()->Model()->predicted_y);
    DeserializeMatrix(s_predicted_residuals, &getLastPLSModel()->Model()->pred_residuals);
    DeserializeMatrix(s_r2q2scrambling, &getLastPLSModel()->Model()->r2q2scrambling);
    DeserializeMatrix(s_q2_sample_validation, &getLastPLSModel()->Model()->q2_sample_validation);
    DeserializeMatrix(s_sdep_sample_validation, &getLastPLSModel()->Model()->sdep_sample_validation);
    DeserializeMatrix(s_q2_sample_validation_surface, &getLastPLSModel()->Model()->q2_sample_validation_surface);
    DeserializeMatrix(s_sdep_sample_validation_surface, &getLastPLSModel()->Model()->sdep_sample_validation_surface);

    int xid = -1;
    for(int j = 0; j < MatrixCount(); j++){
      if(getMatrix(j)->getHash().compare(hashinputmx) == 0){
        xid = j;
        break;
      }
    }

    if(xid == -1){
      (*log).append(QString("Warning on PLS Model %1. Unable to find the origin data matrix.").arg(name));
    }

    getLastPLSModel()->setDID(xid);
    QTreeWidgetItem *subitem = new QTreeWidgetItem;
    subitem->setText(0, "PLS - "+getLastPLSModel()->getName());
    subitem->setText(1, QString::number((*tabcount_)));
    subitem->setText(2, QString::number(getProjectID()));
    subitem->setText(3, getLastPLSModel()->getDataHash());
    subitem->setText(4, getLastPLSModel()->getDataHash());
    subitem->setText(5, QString::number(getLastPLSModel()->getXScaling()));
    subitem->setText(6, QString::number(getLastPLSModel()->getYScaling()));
    subitem->setText(7, QString::number(getLastPLSModel()->getNPC()));
    subitem->setText(8, QString("PLS Model"));
    subitem->setText(9, QString::number((*mid_)));
    MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
    (*tabcount_)++;
    (*log).append(QString("PLS model %1 imported.\n").arg(name));

    QString plshash = getLastPLSModel()->getHash();

    for(int i = 0; i < plspredlist.size(); i++){
      if(plspredlist[i][1].compare(plshash) == 0){
        getLastPLSModel()->addPLSPrediction();
        getLastPLSModel()->getLastPLSPrediction()->setPredID(getLastPLSModel()->PLSPredictionCount()-1);
        getLastPLSModel()->getLastPLSPrediction()->setName(plspredlist[i][0]);
        getLastPLSModel()->getLastPLSPrediction()->setDataHash(plspredlist[i][2]);
        getLastPLSModel()->getLastPLSPrediction()->getObjName() = DeserializeQStringList(plspredlist[i][3]);
        getLastPLSModel()->getLastPLSPrediction()->getYVarName() = DeserializeQStringList(plspredlist[i][4]);
        DeserializeMatrix(plspredlist[i][5], getLastPLSModel()->getLastPLSPrediction()->XPredScoresPointer());
        DeserializeMatrix(plspredlist[i][6], getLastPLSModel()->getLastPLSPrediction()->XPredScoresPointer());
        DeserializeMatrix(plspredlist[i][7], getLastPLSModel()->getLastPLSPrediction()->R2YPointer());
        DeserializeMatrix(plspredlist[i][8], getLastPLSModel()->getLastPLSPrediction()->SDECPointer());

        int predid = -1;
        for(int k = 0; k <  MatrixCount(); k++){
          if(getMatrix(k)->getHash().compare(getLastPLSModel()->getLastPLSPrediction()->getDataHash()) == 0){
            predid = k;
            break;
          }
          else{
            continue;
          }
        }

        if(predid == -1){
          (*log).append(QString("Warning on PLS Prediction %1. Unable to find the origin data matrix.").arg(plspredlist[i][0]));
        }

        getLastPLSModel()->getLastPLSPrediction()->setDID(predid);
        QTreeWidgetItem *subitem = new QTreeWidgetItem;
        subitem->setText(0, getPLSModel((*mid_))->getLastPLSPrediction()->getName());
        subitem->setText(1, QString::number((*tabcount_)));
        subitem->setText(2, QString::number(getProjectID()));
        subitem->setText(3, QString::number((*mid_)));
        subitem->setText(4, getLastPLSModel()->getLastPLSPrediction()->getDataHash());
        subitem->setText(5, getLastPLSModel()->getLastPLSPrediction()->getDataHash());
        subitem->setText(6, QString::number(getPLSModel((*mid_))->getLastPLSPrediction()->getPredID()));
        subitem->setText(7, QString("PLS Prediction"));
        (*tabcount_)++;
        MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
      }
      else{
        continue;
      }
    }
    (*mid_)++;
  }


  query.exec("SELECT * from varselTable");
  while (query.next()){
    // get the query values
    QString name = query.value(0).toString();
    QString serialized_info = query.value(1).toString();
    QString serialized_varnames = query.value(2).toString();
    QString hashinputmx = query.value(3).toString();
    QString serialized_varsel = query.value(4).toString();
    QString serialized_vardist = query.value(5).toString();
    QString serialized_map = query.value(6).toString();

    addVarSelModel();
    getLastVarSelModel()->setModelID((*mid_));
    getLastVarSelModel()->setName(name);
    getLastVarSelModel()->getVariableNames() = DeserializeQStringList(serialized_varnames);
    DeserializeUIVector(serialized_varsel, getLastVarSelModel()->SelectedVablesPointer());
    DeserializeUIVector(serialized_vardist, getLastVarSelModel()->VariableDistributionPointer());
    DeserializeMatrix(serialized_map, getLastVarSelModel()->MapPointer());
    QStringList info = DeserializeQStringList(serialized_info);
    getLastVarSelModel()->setVariableSelectionAlgorithm(info[1]);
    getLastVarSelModel()->setVariableSelectionAlgorithmOptions(info[2]);

    QTreeWidgetItem *subitem = new QTreeWidgetItem;
    subitem->setText(0, getLastVarSelModel()->getName());
    subitem->setText(1, QString::number((*tabcount_)));
    subitem->setText(2, QString::number(getProjectID()));
    subitem->setText(3, getLastVarSelModel()->getModelHash());
    subitem->setText(4, "-");
    subitem->setText(5, "-");
    subitem->setText(6, "-");
    subitem->setText(7, "-");
    subitem->setText(8, QString("PLS Variable Selection Model"));
    subitem->setText(9, QString::number((*mid_)));
    MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
    (*tabcount_)++;
    (*mid_)++;
  }

  query.exec("SELECT * from mlrpredTable");
  QList<QStringList> mlrpredlist;

  while (query.next()){
    mlrpredlist.append(QStringList());
    mlrpredlist.last().append(query.value(0).toString());
    mlrpredlist.last().append(query.value(1).toString());
    mlrpredlist.last().append(query.value(2).toString());
    mlrpredlist.last().append(query.value(3).toString());
    mlrpredlist.last().append(query.value(4).toString());
    mlrpredlist.last().append(query.value(5).toString());
    mlrpredlist.last().append(query.value(6).toString());
    mlrpredlist.last().append(query.value(7).toString());
  }

  query.exec("SELECT * from mlrTable");
  while(query.next()){
    // get the query values
    QString name = query.value(0).toString();
    QString hashinputmx = query.value(1).toString();
    QString serialized_objname = query.value(2).toString();
    QString serialized_xvarname = query.value(3).toString();
    QString serialized_yvarname = query.value(4).toString();
    QString serialized_b = query.value(5).toString();
    QString serialized_r2y = query.value(6).toString();
    QString serialized_sdec = query.value(7).toString();
    QString serialized_recalc_y = query.value(8).toString();
    QString serialized_recalc_residuals = query.value(9).toString();
    int validationtype = query.value(10).toInt();
    QString serialized_ymean = query.value(11).toString();
    QString serialized_q2y = query.value(12).toString();
    QString serialized_sdep = query.value(13).toString();
    QString serialized_bias = query.value(14).toString();
    QString serialized_predicted_y = query.value(15).toString();
    QString serialized_predicted_residuals = query.value(16).toString();
    QString serialized_r2q2scrambling = query.value(167).toString();

    addMLRModel();
    getLastMLRModel()->setModelID((*mid_));
    getLastMLRModel()->setName(name);
    getLastMLRModel()->getObjName() = DeserializeQStringList(serialized_objname);
    getLastMLRModel()->getXVarName() = DeserializeQStringList(serialized_xvarname);
    getLastMLRModel()->getYVarName() = DeserializeQStringList(serialized_yvarname);
    DeserializeMatrix(serialized_b, &getLastMLRModel()->Model()->b);
    DeserializeDVector(serialized_r2y, &getLastMLRModel()->Model()->r2y_model);
    DeserializeDVector(serialized_sdec, &getLastMLRModel()->Model()->sdec);
    DeserializeMatrix(serialized_recalc_y, &getLastMLRModel()->Model()->recalculated_y);
    DeserializeMatrix(serialized_recalc_residuals, &getLastMLRModel()->Model()->recalc_residuals);
    DeserializeDVector(serialized_ymean, &getLastMLRModel()->Model()->ymean);
    DeserializeDVector(serialized_q2y, &getLastMLRModel()->Model()->q2y);
    DeserializeDVector(serialized_sdep, &getLastMLRModel()->Model()->sdep);
    DeserializeDVector(serialized_bias, &getLastMLRModel()->Model()->bias);
    DeserializeDVector(serialized_bias, &getLastMLRModel()->Model()->bias);
    DeserializeMatrix(serialized_predicted_y, &getLastMLRModel()->Model()->predicted_y);
    DeserializeMatrix(serialized_predicted_residuals, &getLastMLRModel()->Model()->pred_residuals);
    DeserializeMatrix(serialized_r2q2scrambling, &getLastMLRModel()->Model()->r2q2scrambling);
    getLastMLRModel()->setValidation(validationtype);


    int did = -1;
    for(int j = 0; j < MatrixCount(); j++){
      if(getMatrix(j)->getHash().compare(hashinputmx) == 0){
        did = j;
      }
      else{
        continue;
      }
    }

    if(did == -1){
      (*log).append(QString("Warning on MLR Model %1. Unable to find the origin data matrix.").arg(name));
    }

    getLastMLRModel()->setDID(did);
    QTreeWidgetItem *subitem = new QTreeWidgetItem;
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
    MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
    (*tabcount_)++;

    QString mlrhash = getLastMLRModel()->getHash();
    for(int i = 0; i < mlrpredlist.size(); i++){
      if(mlrpredlist[i][1].compare(mlrhash) == 0){
        getLastMLRModel()->addMLRPrediction();
        //name TEXT, mlrhash TEXT, hashinputmx TEXT, objname TEXT, yvarname TEXT, predicted_y TEXT, r2y TEXT, sdec TXT
        getLastMLRModel()->getLastMLRPrediction()->setPredID(getLastMLRModel()->MLRPredictionCount()-1);
        getLastMLRModel()->getLastMLRPrediction()->setName(mlrpredlist[i][0]);
        getLastMLRModel()->getLastMLRPrediction()->setDataHash(mlrpredlist[i][2]);
        getLastMLRModel()->getLastMLRPrediction()->getObjName() = DeserializeQStringList(mlrpredlist[i][3]);
        getLastMLRModel()->getLastMLRPrediction()->getYVarName() = DeserializeQStringList(mlrpredlist[i][4]);
        DeserializeMatrix(mlrpredlist[i][5], getLastMLRModel()->getLastMLRPrediction()->YDipVarPointer());
        DeserializeDVector(mlrpredlist[i][6], getLastMLRModel()->getLastMLRPrediction()->R2YPointer());
        DeserializeDVector(mlrpredlist[i][7], getLastMLRModel()->getLastMLRPrediction()->SDECPointer());

        int did = -1;
        for(int k = 0; k <  MatrixCount(); k++){
          if(getMatrix(k)->getHash().compare(getLastMLRModel()->getLastMLRPrediction()->getDataHash()) == 0){
            did = k;
            break;
          }
          else{
            continue;
          }
        }

        if(did == -1){
          (*log).append(QString("Warning on MLR Prediction %1. Unable to find origin data matrix.").arg(mlrpredlist[i][0]));
        }

        getLastMLRModel()->getLastMLRPrediction()->setDID(did);
        QTreeWidgetItem *subitem = new QTreeWidgetItem;
        subitem->setText(0, getLastMLRModel()->getLastMLRPrediction()->getName());
        subitem->setText(1, QString::number((*tabcount_)));
        subitem->setText(2, QString::number(getProjectID()));
        subitem->setText(3, QString::number((*mid_)));
        subitem->setText(4, getLastMLRModel()->getLastMLRPrediction()->getDataHash());
        subitem->setText(5, getLastMLRModel()->getLastMLRPrediction()->getDataHash());
        subitem->setText(6, QString::number(getLastMLRModel()->getLastMLRPrediction()->getPredID()));
        subitem->setText(7, QString("MLR Prediction"));
        (*tabcount_)++;
        MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
      }
      else{
        i++;
      }
    }
    (*mid_)++;
  }

  query.exec("SELECT * from ldapredTable");
  QList<QStringList> ldapredlist;

  while (query.next()){
    ldapredlist.append(QStringList());
    ldapredlist.last().append(query.value(0).toString());
    ldapredlist.last().append(query.value(1).toString());
    ldapredlist.last().append(query.value(2).toString());
    ldapredlist.last().append(query.value(3).toString());
    ldapredlist.last().append(query.value(4).toString());
    ldapredlist.last().append(query.value(5).toString());
    ldapredlist.last().append(query.value(6).toString());
    ldapredlist.last().append(query.value(7).toString());
    ldapredlist.last().append(query.value(8).toString());
    ldapredlist.last().append(query.value(9).toString());
    ldapredlist.last().append(query.value(10).toString());
  }

  query.exec("SELECT * from ldaTable");
  while (query.next()){
    //name, hashinputmx, objname, varname, validation, acc, npv, ppv, spec, sens, pprob, evect, eval,
    // mu, mnpdf, features, fmean, fsdev, inv_cov, nclass, class_start, classid, classes, nameclasses
    QString name =  query.value(0).toString();
    QString hashinputmx =  query.value(1).toString();
    QString serialized_objname =  query.value(2).toString();
    QString serialized_varname =  query.value(3).toString();
    int validationtype =  query.value(4).toInt();
    QString serialized_acc = query.value(5).toString();
    QString serialized_npv = query.value(6).toString();
    QString serialized_ppv = query.value(7).toString();
    QString serialized_spec = query.value(8).toString();
    QString serialized_sens = query.value(9).toString();
    QString serialized_prob = query.value(10).toString();
    QString serialized_evect = query.value(11).toString();
    QString serialized_eval = query.value(12).toString();
    QString serialized_mu = query.value(13).toString();
    QString serialized_mnpdf = query.value(14).toString();
    QString serialized_features = query.value(15).toString();
    QString serialized_fmean = query.value(16).toString();
    QString serialized_fsdev = query.value(17).toString();
    QString serialized_inv_cov = query.value(18).toString();
    int nclass = query.value(19).toInt();
    int class_start = query.value(20).toInt();
    QString serialized_classid = query.value(21).toString();
    QString serialized_classes = query.value(22).toString();
    QString serialized_nameclasses = query.value(23).toString();

    addLDAModel();
    getLastLDAModel()->setName(name);
    getLastLDAModel()->setModelID((*mid_));
    getLastLDAModel()->getObjName() = DeserializeQStringList(serialized_objname);
    getLastLDAModel()->getVarName() = DeserializeQStringList(serialized_varname);
    DeserializeDVector(serialized_acc, &getLastLDAModel()->Model()->acc);
    DeserializeDVector(serialized_npv, &getLastLDAModel()->Model()->npv);
    DeserializeDVector(serialized_ppv, &getLastLDAModel()->Model()->ppv);
    DeserializeDVector(serialized_spec, &getLastLDAModel()->Model()->spec);
    DeserializeDVector(serialized_sens, &getLastLDAModel()->Model()->sens);
    DeserializeDVector(serialized_prob, &getLastLDAModel()->Model()->pprob);
    DeserializeDVector(serialized_eval, &getLastLDAModel()->Model()->eval);
    DeserializeMatrix(serialized_evect, &getLastLDAModel()->Model()->evect);
    DeserializeMatrix(serialized_mu, &getLastLDAModel()->Model()->mu);
    DeserializeArray(serialized_mnpdf, &getLastLDAModel()->Model()->mnpdf);
    DeserializeArray(serialized_features, &getLastLDAModel()->Model()->features);
    DeserializeMatrix(serialized_fmean, &getLastLDAModel()->Model()->fmean);
    DeserializeMatrix(serialized_fsdev, &getLastLDAModel()->Model()->fsdev);
    DeserializeMatrix(serialized_inv_cov, &getLastLDAModel()->Model()->inv_cov);
    DeserializeUIVector(serialized_classid, &getLastLDAModel()->Model()->classid);
    getLastLDAModel()->Model()->nclass = nclass;
    getLastLDAModel()->Model()->class_start = class_start;
    getLastLDAModel()->setValidation(validationtype);
    getLastLDAModel()->getNameClasses() = DeserializeQStringList(serialized_nameclasses);
    QStringList classes = serialized_classes.split("\\");
    for(int j = 0; j < classes.size(); j++)
      getLastLDAModel()->getClasses().append(DeserializeQStringList(classes[j]));

    int did = -1;
    for(int j = 0; j < MatrixCount(); j++){
      if(getMatrix(j)->getHash().compare(hashinputmx) == 0){
        did = j;
      }
      else{
        continue;
      }
    }

    if(did == -1){
      (*log).append(QString("Warning on LDA Model %1. Unable to find the origin data matrix.").arg(name));
    }

    getLastLDAModel()->setDID(did);
    QTreeWidgetItem *subitem = new QTreeWidgetItem;
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
    MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
    (*tabcount_)++;

    //  ldapredTable (name TEXT, ldahash TEXT, hashinputmx TEXT, objname TEXT, varname TEXT, pred_class TEXT, pred_features TEXT,
    //                prob TEXT, classes TEXT, nameclasses TEXT, mnpdf TEXT)"));
    QString ldahash = getLastLDAModel()->getHash();
    for(int j = 0; j < ldapredlist.size(); j++){
      if(ldapredlist[j][1].compare(ldahash) == 0){
        (*log).append(QString("Importing LDA Prediction %1").arg(ldapredlist[j][0]));
        getLastLDAModel()->addLDAPrediction();
        getLastLDAModel()->getLastLDAPrediction()->setPredID(getLastLDAModel()->LDAPredictionCount()-1);
        getLastLDAModel()->getLastLDAPrediction()->setName(ldapredlist[j][0]);
        getLastLDAModel()->getLastLDAPrediction()->setDataHash(ldapredlist[j][2]);
        getLastLDAModel()->getLastLDAPrediction()->getObjName() = DeserializeQStringList(ldapredlist[j][3]);
        getLastLDAModel()->getLastLDAPrediction()->getVarName() = DeserializeQStringList(ldapredlist[j][4]);
        DeserializeUIVector(ldapredlist[j][5], getLastLDAModel()->getLastLDAPrediction()->getPredClassesPointer());
        DeserializeMatrix(ldapredlist[j][6], getLastLDAModel()->getLastLDAPrediction()->getPredFeaturesPointer());
        DeserializeMatrix(ldapredlist[j][7], getLastLDAModel()->getLastLDAPrediction()->getProbPredPointer());
        QStringList pred_classes = ldapredlist[j][8].split("\\");
        for(int k = 0; k < pred_classes.size(); k++)
          getLastLDAModel()->getLastLDAPrediction()->getClasses().append(DeserializeQStringList(pred_classes[k]));
        getLastLDAModel()->getLastLDAPrediction()->getNameClasses() = DeserializeQStringList(ldapredlist[j][9]);
        DeserializeMatrix(ldapredlist[j][10], getLastLDAModel()->getLastLDAPrediction()->getProbPredPointer());

        int did = -1;
        for(int k = 0; k <  MatrixCount(); k++){
          if(getMatrix(k)->getHash().compare(getLastLDAModel()->getLastLDAPrediction()->getDataHash()) == 0){
            did = k;
            break;
          }
          else{
            continue;
          }
        }

        if(did == -1){
          (*log).append(QString("Warning in LDA Prediction %1. Unable to find the origin data matrix. ").arg(ldapredlist[j][0]));
        }

        getLastLDAModel()->getLastLDAPrediction()->setDID(did);
//         ModelPrediction Name - Tab Count - pid_ - Model ID - xdata id - ydata id - Data Position - Data Type (LDA Prediction, ULDA Prediction, ...) (8)
        QTreeWidgetItem *subitem = new QTreeWidgetItem;
        subitem->setText(0, getLDAModel((*mid_))->getLastLDAPrediction()->getName());
        subitem->setText(1, QString::number((*tabcount_)));
        subitem->setText(2, QString::number(getProjectID()));
        subitem->setText(3, QString::number((*mid_)));
        subitem->setText(4, getLastLDAModel()->getLastLDAPrediction()->getDataHash());
        subitem->setText(5, "-");
        subitem->setText(6, QString::number(getLDAModel((*mid_))->getLastLDAPrediction()->getPredID()));
        subitem->setText(7, QString("LDA Prediction"));
        (*tabcount_)++;
        MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
      }
    }
    (*mid_)++;
  }
}

void DATA::AutoSave()
{
  QFileInfo check_file(projectfautosave);
  if(check_file.exists() && check_file.isFile()){
    GenericProgressDialog pbdialog;
    pbdialog.setRange(0,5);
    pbdialog.hideCancel();
    pbdialog.show();
    SaveData(check_file.absolutePath());
    pbdialog.setValue(5);
    return;
  }
  else
    return;
}

QString DATA::SaveData(QString savepath)
{
  GenericProgressDialog pbdialog;
  pbdialog.setRange(0,5);
  pbdialog.hideCancel();
  pbdialog.show();

  QString savedir = QString::fromUtf8(savepath.toUtf8()) +"/"+getProjectName();
  QString savefname = QString::fromUtf8(savedir.toUtf8())+".qsm";
  // CREATE THE DIRECTORY TO SAVE MODELS AND FILES
  if(DATAIO::DirExists(savedir.toUtf8().data()) == true){
    DATAIO::RemoveDir(savedir.toUtf8().data());
  }

  DATAIO::MakeDir(savedir.toUtf8().data());
  DATAIO::MakeDir(QString("%1%2").arg(QString::fromUtf8(savedir.toUtf8())).arg("/Data").toUtf8().data());
  DATAIO::MakeDir(QString("%1%2").arg(QString::fromUtf8(savedir.toUtf8())).arg("/Models").toUtf8().data());
  DATAIO::MakeDir(QString("%1%2").arg(QString::fromUtf8(savedir.toUtf8())).arg("/ObjectLabels").toUtf8().data());
  DATAIO::MakeDir(QString("%1%2").arg(QString::fromUtf8(savedir.toUtf8())).arg("/VariableLabels").toUtf8().data());
  DATAIO::MakeDir(QString("%1%2").arg(QString::fromUtf8(savedir.toUtf8())).arg("/VariableTabLabels").toUtf8().data());
  DATAIO::MakeDir(QString("%1%2").arg(QString::fromUtf8(savedir.toUtf8())).arg("/Images").toUtf8().data());

  // Write DATA int Data dir
  if(MatrixCount() > 0){
    QString mxpath = QString::fromUtf8(savedir.toUtf8())+"/Data/Matrix/";
    DATAIO::MakeDir(mxpath.toUtf8().data());
    for(int i = 0; i < MatrixCount(); i++){
      QString mxname = QString::fromUtf8(mxpath.toUtf8())+QString::fromUtf8(getMatrix(i)->getName().toUtf8());
      DATAIO::MakeDir(mxname.toUtf8().data());
      DATAIO::WriteMatrix(QString(QString::fromUtf8(mxname.toUtf8())+"/data.txt").toUtf8().data(), getMatrix(i)->Matrix());
      WriteList(getMatrix(i)->getObjName(), mxname+"/label.txt");
      QStringList varname = getMatrix(i)->getVarName();
      varname.removeFirst(); // remove the first var name that is automatically inserted during import
      WriteList(varname, QString::fromUtf8(mxname.toUtf8())+"/varname.txt");
    }
  }

  pbdialog.setValue(1);

  if(ArrayCount() > 0){
    QString arpath = QString::fromUtf8(savedir.toUtf8())+"/Data/Array/";
    DATAIO::MakeDir(arpath.toUtf8().data());
    for(int i = 0; i < ArrayCount(); i++){
      QString arname = QString::fromUtf8(arpath.toUtf8())+QString::fromUtf8(getArray(i)->getName().toUtf8());
      DATAIO::MakeDir(arname.toUtf8().data());
      DATAIO::WriteArray(QString(QString::fromUtf8(arname.toUtf8())+"/data.txt").toUtf8().data(), getArray(i)->Array());
      WriteList(getArray(i)->getObjName(), arname+"/label.txt");
      QStringList varname = getArray(i)->getVarName();
      varname.removeFirst();
      WriteList(varname, arname+"/varname.txt");
    }
  }

  pbdialog.setValue(2);

  if(getObjectLabels().size() > 0){
    QString labelspath = QString::fromUtf8(savedir.toUtf8())+"/ObjectLabels/";
    for(int i = 0; i < getObjectLabels().size(); i++){
      QString labelname = QString::fromUtf8(labelspath.toUtf8())+QString::fromUtf8(getObjectLabels()[i].name.toUtf8())+".txt";
      WriteList(getObjectLabels()[i].objects, labelname);
    }
  }

  pbdialog.setValue(3);

  if(getVariableLabels().size() > 0){
    QString labelspath = QString::fromUtf8(savedir.toUtf8())+"/VariableLabels/";
    //DATAIO::MakeDir(labelspath.toStdString());
    for(int i = 0; i < getVariableLabels().size(); i++){
      QString labelname = QString::fromUtf8(labelspath.toUtf8())+QString::fromUtf8(getVariableLabels()[i].name.toUtf8())+".txt";
      WriteList(getVariableLabels()[i].objects, labelname);
    }
  }

  if(getVariableTabLabels().size() > 0){
    QString labelspath = QString::fromUtf8(savedir.toUtf8())+"/VariableTabLabels/";
    for(int i = 0; i < getVariableTabLabels().size(); i++){
      QString mxname = QString::fromUtf8(labelspath.toUtf8())+QString::fromUtf8(getVariableTabLabels()[i]->getName().toUtf8());
      DATAIO::MakeDir(mxname.toUtf8().data());
      DATAIO::WriteMatrix(QString(QString::fromUtf8(mxname.toUtf8())+"/data.txt").toUtf8().data(), getVariableTabLabels()[i]->getMatrix());
      WriteList(getVariableTabLabels()[i]->getObjectsName(), mxname+"/label.txt");
      QStringList varname = getVariableTabLabels()[i]->getFeaturesName();
      #ifdef DEBUG
      qDebug() << varname.first();
      #endif
      varname.removeFirst(); // remove the first var name that is automatically inserted during import
      WriteList(varname, QString::fromUtf8(mxname.toUtf8())+"/varname.txt");
    }
  }

  pbdialog.setValue(4);

  if(getImages().size() > 0){
    QString destdir = QString::fromUtf8(savedir.toUtf8())+"/Images/";
    for(int i = 0; i < getImages().size(); i++){

      QFile imgfile(QString("%1/%2.jpg").arg(destdir).arg(getImages()[i].name));
      imgfile.open(QIODevice::WriteOnly);
      getImages()[i].image.save(&imgfile, "JPG");

//       CopyFile(QString::fromUtf8(getImages()[i].filepath.toUtf8()), destdir);
    }
  }

  // Write MODELS into Models dir  RIMUOVI PCA - da ogni nome
  if(PCACount() > 0){
    DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/PCAMODELS").toUtf8().data());
    for(int i = 0; i < PCACount(); i++){
      QString dirname = getPCAModelAt(i)->getName().remove("PCA - ", Qt::CaseSensitive).trimmed();
      getPCAModelAt(i)->WritePCAModel(QString("%1/Models/PCAMODELS/").arg(QString::fromUtf8(savedir.toUtf8())).toUtf8().data(), dirname.toUtf8().data());

      if(getPCAModelAt(i)->PCAPredictionCount() > 0){
        DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/PCAMODELS/%1/PREDICTIONS").arg(dirname).toUtf8().data());
        for(int j = 0; j < getPCAModelAt(i)->PCAPredictionCount(); j++){
          QString dirnamepred = getPCAModelAt(i)->getPCAPrediction(j)->getName().remove("PCA Prediction - ", Qt::CaseSensitive).trimmed();
          getPCAModelAt(i)->getPCAPrediction(j)->WritePCAPrediction(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/PCAMODELS/%1/PREDICTIONS/").arg(dirname).toUtf8().data(), dirnamepred.toUtf8().data());
        }
      }
    }
  }

  if(PLSCount() > 0){
    DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/PLSMODELS").toUtf8().data());
    for(int i = 0; i < PLSCount(); i++){
      QString dirname = getPLSModelAt(i)->getName().remove("PLS - ", Qt::CaseSensitive).trimmed();
      getPLSModelAt(i)->WritePLSModel(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/PLSMODELS/").toUtf8().data(), dirname.toUtf8().data());

      if(getPLSModelAt(i)->PLSPredictionCount() > 0){
        DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/PLSMODELS/"+getPLSModelAt(i)->getName().remove("PLS Model - ").trimmed()).toUtf8().data());
        for(int j = 0; j < getPLSModelAt(i)->PLSPredictionCount(); j++){
          QString dirnamepred = getPLSModelAt(i)->getPLSPrediction(j)->getName().remove("PLS Prediction - ", Qt::CaseSensitive).trimmed();
          getPLSModelAt(i)->getPLSPrediction(j)->WritePLSPrediction(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/PLSMODELS/%1/PREDICTIONS/").arg(dirname).toUtf8().data(), dirnamepred.toUtf8().data());
        }
      }
    }
  }


  if(VarSelCount() > 0){
    DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/VARIABLESELECTIONS").toUtf8().data());
    for(int i = 0; i < VarSelCount(); i++){
      QString dirname = QString(QString::fromUtf8(savedir.toUtf8())+"/Models/VARIABLESELECTIONS/%1").arg(getVarSelModelAt(i)->getName().remove("VSEL - ", Qt::CaseSensitive).trimmed());
      getVarSelModelAt(i)->WriteVarSelModel(dirname);
    }
  }


  if(MLRCount() > 0){
    DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/MLRMODELS").toUtf8().data());
    for(int i = 0; i < MLRCount(); i++){
      QString dirname = getMLRModelAt(i)->getName().remove("MLR - ", Qt::CaseSensitive).trimmed();
      getMLRModelAt(i)->WriteMLRModel(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/MLRMODELS/").toUtf8().data(), dirname.toUtf8().data());

      if(getMLRModelAt(i)->MLRPredictionCount() > 0){
        DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/MLRMODELS/"+getMLRModelAt(i)->getName().remove("MLR Model - ").trimmed()).toUtf8().data());
        for(int j = 0; j < getMLRModelAt(i)->MLRPredictionCount(); j++){
          QString dirnamepred = getMLRModelAt(i)->getMLRPrediction(j)->getName().remove("MLR Prediction - ", Qt::CaseSensitive).trimmed();
          getMLRModelAt(i)->getMLRPrediction(j)->WriteMLRPrediction(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/MLRMODELS/%1/PREDICTIONS/").arg(dirname).toUtf8().data(), dirnamepred.toUtf8().data());
        }
      }
    }
  }

  if(LDACount() > 0){
    DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/LDAMODELS").toUtf8().data());
    for(int i = 0; i < LDACount(); i++){
      QString dirname = getLDAModelAt(i)->getName().remove("LDA - ", Qt::CaseSensitive).trimmed();
      getLDAModelAt(i)->WriteLDAModel(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/LDAMODELS/").toUtf8().data(), dirname.toUtf8().data());

      if(getLDAModelAt(i)->LDAPredictionCount() > 0){
        DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/LDAMODELS/"+getLDAModelAt(i)->getName().remove("LDA Model - ").trimmed()).toUtf8().data());
        for(int j = 0; j < getLDAModelAt(i)->LDAPredictionCount(); j++){
          QString dirnamepred = getLDAModelAt(i)->getLDAPrediction(j)->getName().remove("LDA Prediction - ", Qt::CaseSensitive).trimmed();
          getLDAModelAt(i)->getLDAPrediction(j)->WriteLDAPrediction(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/LDAMODELS/%1/PREDICTIONS/").arg(dirname).toUtf8().data(), dirnamepred.toUtf8().data());
        }
      }
    }
  }

  // Compress
  DirCompressor dc;
  dc.setDir(savedir.toUtf8().data());
  dc.setOutput(savefname.toUtf8().data());
  dc.compress();
  DATAIO::RemoveDir(savedir.toUtf8().data());

  pbdialog.setValue(5);
  projectfautosave = savefname;
  return savefname;
}

QString DATA::SaveSQLData(QString savepath)
{
  GenericProgressDialog pbdialog;
  pbdialog.setRange(0,5);
  pbdialog.hideCancel();
  pbdialog.show();
  QString dbName = savepath+"/"+getProjectName()+".qsm";
  //QString dbName( "myDatabase.db3" );
  QFile::remove(dbName);
  QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
  db.setDatabaseName(dbName);
  db.open();
  QSqlQuery query = QSqlQuery(db);
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
  query.exec(QString("CREATE TABLE IF NOT EXISTS matrixTable (name TEXT, objname TEXT, varname  TEXT, mx TEXT)"));
  if(MatrixCount() > 0){
    for(int i = 0; i < MatrixCount(); i++){
      QString mxname = getMatrix(i)->getName();
      QString objname_serialized = SerializeQStringList(getMatrix(i)->getObjName());
      QStringList varname = getMatrix(i)->getVarName();
      //varname.removeFirst();
      QString varname_serialized = SerializeQStringList(varname);
      QString serialized_mx = SerializeMatrix(getMatrix(i)->Matrix());

      query.prepare("INSERT INTO matrixTable (name, objname, varname, mx) VALUES (:name, :objname, :varname, :mx)");
      query.bindValue(":name", mxname);
      query.bindValue(":objname", objname_serialized);
      query.bindValue(":varname", varname_serialized);
      query.bindValue(":mx", serialized_mx);
      query.exec();
    }
  }

  pbdialog.setValue(1);

  query.exec(QString("CREATE TABLE IF NOT EXISTS arrayTable (name TEXT, objname TEXT, varname  TEXT, ar TEXT)"));
  if(ArrayCount() > 0){
    /* txt1 = arname
     * txt2 = objname_serialized
     * txt3 = varname_serialized
     * txt4 = serialized_ar
     */
    for(int i = 0; i < ArrayCount(); i++){
      QString arname = getArray(i)->getName();
      QString objname_serialized = SerializeQStringList(getArray(i)->getObjName());
      QStringList varname = getArray(i)->getVarName();
      //varname.removeFirst();
      QString varname_serialized = SerializeQStringList(varname);
      QString serialized_ar = SerializeArray(getArray(i)->Array());

      query.prepare("INSERT INTO arrayTable (name, objname, varname, ar) VALUES (:name, :objname, :varname, :ar)");
      query.bindValue(":name", arname);
      query.bindValue(":objname", objname_serialized);
      query.bindValue(":varname", varname_serialized);
      query.bindValue(":ar", serialized_ar);
      query.exec();
    }
  }

  pbdialog.setValue(2);

  query.exec(QString("CREATE TABLE IF NOT EXISTS objlabelTable (name TEXT, values TEXT)"));
  if(getObjectLabels().size() > 0){
    //txt1 = serialized_label
    for(int i = 0; i < getObjectLabels().size(); i++){
      QString labelname = getObjectLabels()[i].name;
      QString serialized_label = SerializeQStringList(getObjectLabels()[i].objects);

      query.prepare("INSERT INTO objlabelTable (name, values) VALUES (:name, :values)");
      query.bindValue(":name", labelname);
      query.bindValue(":values", serialized_label);
      query.exec();
    }
  }

  pbdialog.setValue(3);

  query.exec(QString("CREATE TABLE IF NOT EXISTS varlabelTable (name TEXT, values TEXT)"));
  if(getVariableLabels().size() > 0){
    //DATAIO::MakeDir(labelspath.toStdString());
    for(int i = 0; i < getVariableLabels().size(); i++){
      QString labelname = getVariableLabels()[i].name;
      QString serialized_label = SerializeQStringList(getObjectLabels()[i].objects);

      query.prepare("INSERT INTO varlabelTable (name, values) VALUES (:name, :values)");
      query.bindValue(":name", labelname);
      query.bindValue(":values", serialized_label);
      query.exec();
    }
  }

  query.exec(QString("CREATE TABLE IF NOT EXISTS vartablabelTable (name TEXT, objname TEXT, varname  TEXT, mx TEXT)"));
  if(getVariableTabLabels().size() > 0){
    for(int i = 0; i < getVariableTabLabels().size(); i++){
      QString mxname = getVariableTabLabels()[i]->getName();
      QString objname_serialized = SerializeQStringList(getVariableTabLabels()[i]->getObjectsName());
      QStringList varname = getVariableTabLabels()[i]->getFeaturesName();
      //varname.removeFirst(); // remove the first var name that is automatically inserted during import
      QString varname_serialized = SerializeQStringList(varname);
      QString serialized_mx = SerializeMatrix(getVariableTabLabels()[i]->getMatrix());

      query.prepare("INSERT INTO vartablabelTable (name, objname, varname, mx) VALUES (:name, :objname, :varname, :mx)");
      query.bindValue(":name", mxname);
      query.bindValue(":objname", objname_serialized);
      query.bindValue(":varname", varname_serialized);
      query.bindValue(":mx", serialized_mx);
      query.exec();
    }
  }

  pbdialog.setValue(4);

  query.exec(QString("CREATE TABLE IF NOT EXISTS imgTable (name TEXT,  imagedata BLOB)"));
  if(getImages().size() > 0){
    for(int i = 0; i < getImages().size(); i++){
      QByteArray inByteArray;
      QBuffer inBuffer(&inByteArray);
      inBuffer.open(QIODevice::WriteOnly);
      getImages()[i].image.save(&inBuffer, "PNG");
      query.prepare("INSERT INTO imgTable (name, imagedata) VALUES (:name, :imagedata)" );
      query.bindValue(":name", getImages()[i].name);
      query.bindValue(":imagedata", inByteArray);
      query.exec();
    }
  }

  // Write MODELS into Models dir  RIMUOVI PCA - da ogni nome
  query.exec(QString("CREATE TABLE IF NOT EXISTS pcaTable (name TEXT, numcomp INT, scalingtype INT, hashinputmx TEXT, objname TEXT, varname TEXT, scores TEXT, loadings  TEXT, varexp TEXT, colscaling TEXT, colaverage TEXT)"));
  query.exec(QString("CREATE TABLE IF NOT EXISTS pcapredTable (name TEXT, pcahash TEXT, hashinputmx TEXT, objname TEXT, scores TEXT)"));
  if(PCACount() > 0){
    for(int i = 0; i < PCACount(); i++){
      QString modname = getPCAModelAt(i)->getName()/*.remove("PCA - ", Qt::CaseSensitive).trimmed()*/;
      QString objname_serialized = SerializeQStringList(getPCAModelAt(i)->getObjName());
      QString varname_serialized = SerializeQStringList(getPCAModelAt(i)->getVarName());
      QString serialized_scores = SerializeMatrix(getPCAModelAt(i)->Model()->scores);
      QString serialized_loadings = SerializeMatrix(getPCAModelAt(i)->Model()->loadings);
      QString serialized_colaverage = SerializeDVector(getPCAModelAt(i)->Model()->colaverage);
      QString serialized_colscaling = SerializeDVector(getPCAModelAt(i)->Model()->colscaling);
      QString serialized_varexp = SerializeDVector(getPCAModelAt(i)->Model()->varexp);
      query.prepare("INSERT INTO pcaTable (name, numcomp, scalingtype, hashinputmx, objname, varname, scores, loadings, varexp, colscaling, colaverage) VALUES (:name, :numcomp, :scalingtype, :hashinputmx, :objname, :varname, :scores, :loadings, :varexp, :colscaling, :colaverage)");
      query.bindValue(":name", modname);
      query.bindValue(":numcomp", getPCAModelAt(i)->getNPC());
      query.bindValue(":scalingtype", getPCAModelAt(i)->getXScaling());
      query.bindValue(":hashinputmx", getPCAModelAt(i)->getDataHash());
      query.bindValue(":objname", objname_serialized);
      query.bindValue(":varname", varname_serialized);
      query.bindValue(":scores", serialized_scores);
      query.bindValue(":loadings", serialized_loadings);
      query.bindValue(":colscaling", serialized_colaverage);
      query.bindValue(":colaverage", serialized_colscaling);
      query.bindValue(":varexp", serialized_varexp);
      query.exec();

      if(getPCAModelAt(i)->PCAPredictionCount() > 0){
        for(int j = 0; j < getPCAModelAt(i)->PCAPredictionCount(); j++){
          QString pred_objname_serialized = SerializeQStringList(getPCAModelAt(i)->getPCAPrediction(j)->getObjName());
          QString pred_serialized_mx = SerializeMatrix(getPCAModelAt(i)->getPCAPrediction(j)->getPredScores());
          query.prepare("INSERT INTO pcapredTable (name, pcahash, hashinputmx, objname, scores) VALUES (:name, :pcahash, :hashinputmx, :objname, :scores)");
          query.bindValue(":name", getPCAModelAt(i)->getPCAPrediction(j)->getName()/*.remove("PCA Prediction - ", Qt::CaseSensitive).trimmed()*/);
          query.bindValue(":pcahash", getPCAModelAt(i)->getHash());
          query.bindValue(":hashinputmx", getPCAModelAt(i)->getPCAPrediction(j)->getDataHash());
          query.bindValue(":objname", pred_objname_serialized);
          query.bindValue(":scores", pred_serialized_mx);
          query.exec();
        }
      }
    }
  }

  query.exec(QString("CREATE TABLE IF NOT EXISTS plsTable (name TEXT, nlvs INT, xscalingtype INT, yscalingtype INT, hashinputmx TEXT, objname TEXT, xvarname TEXT, yvarname TEXT, tscores TEXT, ploadings  TEXT, weights TEXT, xvarexp TEXT, xcolscaling TEXT, xcolaverage TEXT, uscores TEXT, qloadings TEXT, ycolscaling TEXT, ycolaverage TEXT, b TEXT, r2y_model TEXT, sdec TEXT, recalc_y TEXT, recalc_residuals TEXT, validationtype INT, q2y TEXT, sdep TEXT, bias TEXT, predicted_y TEXT, predicted_residuals TEXT, r2q2scrambling TEXT, q2_sample_validation TEXT, sdep_sample_validation TEXT, q2_sample_validation_surface TEXT, sdep_sample_validation_surface TEXT)"));
  query.exec(QString("CREATE TABLE IF NOT EXISTS plspredTable (name TEXT, plshash TEXT, hashinputmx TEXT, objname TEXT, yvarname TEXT, tscores TEXT, predicted_y TEXT, r2y TEXT, sdec TXT)"));
  if(PLSCount() > 0){
    for(int i = 0; i < PLSCount(); i++){
      QString modname = getPLSModelAt(i)->getName()/*.remove("PCA - ", Qt::CaseSensitive).trimmed()*/;
      QString objname_serialized = SerializeQStringList(getPLSModelAt(i)->getObjName());
      QString xvarname_serialized = SerializeQStringList(getPLSModelAt(i)->getXVarName());
      QString yvarname_serialized = SerializeQStringList(getPLSModelAt(i)->getYVarName());

      QString serialized_tscores = SerializeMatrix(getPLSModelAt(i)->Model()->xscores);
      QString serialized_ploadings = SerializeMatrix(getPLSModelAt(i)->Model()->xloadings);
      QString serialized_weights = SerializeMatrix(getPLSModelAt(i)->Model()->xweights);
      QString serialized_xvarexp = SerializeDVector(getPLSModelAt(i)->Model()->xvarexp);
      QString serialized_xcolaverage = SerializeDVector(getPLSModelAt(i)->Model()->xcolaverage);
      QString serialized_xcolscaling = SerializeDVector(getPLSModelAt(i)->Model()->xcolscaling);


      QString serialized_uscores = SerializeMatrix(getPLSModelAt(i)->Model()->yscores);
      QString serialized_qloadings = SerializeMatrix(getPLSModelAt(i)->Model()->yloadings);
      QString serialized_ycolaverage = SerializeDVector(getPLSModelAt(i)->Model()->ycolaverage);
      QString serialized_ycolscaling = SerializeDVector(getPLSModelAt(i)->Model()->ycolscaling);
      QString serialized_b= SerializeDVector(getPLSModelAt(i)->Model()->b);
      QString serialized_r2y_model = SerializeMatrix(getPLSModelAt(i)->Model()->r2y_model);
      QString serialized_sdec = SerializeMatrix(getPLSModelAt(i)->Model()->sdec);
      QString serialized_recalc_y = SerializeMatrix(getPLSModelAt(i)->Model()->recalculated_y);
      QString serialized_recalc_residuals = SerializeMatrix(getPLSModelAt(i)->Model()->recalc_residuals);

      QString serialized_q2y = SerializeMatrix(getPLSModelAt(i)->Model()->q2y);
      QString serialized_sdep = SerializeMatrix(getPLSModelAt(i)->Model()->sdep);
      QString serialized_bias = SerializeMatrix(getPLSModelAt(i)->Model()->bias);
      QString serialized_predicted_y = SerializeMatrix(getPLSModelAt(i)->Model()->predicted_y);
      QString serialized_pred_residuals = SerializeMatrix(getPLSModelAt(i)->Model()->pred_residuals);

      QString serialized_r2q2scrambling = SerializeMatrix(getPLSModelAt(i)->Model()->r2q2scrambling);
      QString serialized_q2_sample_validation = SerializeMatrix(getPLSModelAt(i)->Model()->q2_sample_validation);
      QString serialized_sdep_sample_validation = SerializeMatrix(getPLSModelAt(i)->Model()->sdep_sample_validation);
      QString serialized_q2_sample_validation_surface = SerializeMatrix(getPLSModelAt(i)->Model()->q2_sample_validation_surface);
      QString serialized_sdep_sample_validation_surface = SerializeMatrix(getPLSModelAt(i)->Model()->sdep_sample_validation_surface);


      query.prepare("INSERT INTO plsTable (name, nlvs, xscalingtype, yscalingtype, hashinputmx, objname, xvarname, yvarname, tscores, ploadings, weights , xvarexp, xcolscaling, xcolaverage, uscores, qloadings, ycolscaling, ycolaverage, b, r2y_model, sdec, recalc_y, recalc_residuals, validationtype, q2y, sdep, bias, predicted_y, predicted_residuals , r2q2scrambling, q2_sample_validation, sdep_sample_validation, q2_sample_validation_surface, sdep_sample_validation_surface) VALUES (:name, :nlvs, :xscalingtype, :yscalingtype, :hashinputmx, :objname, :xvarname, :yvarname, :tscores, :ploadings, :weights , :xvarexp, :xcolscaling, :xcolaverage, :uscores, :qloadings, :ycolscaling, :ycolaverage, :b, :r2y_model, :sdec, :recalc_y, :recalc_residuals, :validationtype, :q2y, :sdep, :bias, :predicted_y, :predicted_residuals , :r2q2scrambling, :q2_sample_validation, :sdep_sample_validation, :q2_sample_validation_surface, :sdep_sample_validation_surface)");
      query.bindValue(":name", modname);
      query.bindValue(":nlvs", getPLSModelAt(i)->getNPC());
      query.bindValue(":xscalingtype", getPLSModelAt(i)->getXScaling());
      query.bindValue(":yscalingtype", getPLSModelAt(i)->getYScaling());
      query.bindValue(":hashinputmx", getPLSModelAt(i)->getDataHash());
      query.bindValue(":objname", objname_serialized);
      query.bindValue(":xvarname", xvarname_serialized);
      query.bindValue(":yvarname", yvarname_serialized);
      query.bindValue(":tscores", serialized_tscores);
      query.bindValue(":ploadings", serialized_ploadings);
      query.bindValue(":weights", serialized_weights);
      query.bindValue(":xvarexp", serialized_xvarexp);
      query.bindValue(":xcolscaling", serialized_xcolscaling);
      query.bindValue(":xcolaverage", serialized_xcolaverage);
      query.bindValue(":uscores", serialized_uscores);
      query.bindValue(":qloadings", serialized_qloadings);
      query.bindValue(":ycolscaling", serialized_ycolscaling);
      query.bindValue(":ycolaverage", serialized_ycolaverage);
      query.bindValue(":b", serialized_b);
      query.bindValue(":r2y_model", serialized_r2y_model);
      query.bindValue(":sdec", serialized_sdec);
      query.bindValue(":recalc_y", serialized_recalc_y);
      query.bindValue(":recalc_residuals", serialized_recalc_residuals);
      query.bindValue(":validationtype", getPLSModelAt(i)->getValidation());
      query.bindValue(":q2y", serialized_q2y);
      query.bindValue(":sdep", serialized_sdep);
      query.bindValue(":bias", serialized_bias);
      query.bindValue(":predicted_y", serialized_predicted_y);
      query.bindValue(":predicted_residuals", serialized_pred_residuals);
      query.bindValue(":r2q2scrambling", serialized_r2q2scrambling);
      query.bindValue(":q2_sample_validation", serialized_q2_sample_validation);
      query.bindValue(":sdep_sample_validation", serialized_sdep_sample_validation);
      query.bindValue(":q2_sample_validation_surface", serialized_q2_sample_validation_surface);
      query.bindValue(":sdep_sample_validation_surface", serialized_sdep_sample_validation_surface);
      query.exec();

      if(getPLSModelAt(i)->PLSPredictionCount() > 0){
        for(int j = 0; j < getPLSModelAt(i)->PLSPredictionCount(); j++){
          QString pred_objname_serialized = SerializeQStringList(getPLSModelAt(i)->getPLSPrediction(j)->getObjName());
          QString pred_yvarname_serialized = SerializeQStringList(getPLSModelAt(i)->getPLSPrediction(j)->getYVarName());
          QString pred_serialized_mx = SerializeMatrix(getPLSModelAt(i)->getPLSPrediction(j)->getXPredScores());
          QString pred_serialized_predicted_y = SerializeMatrix(getPLSModelAt(i)->getPLSPrediction(j)->getYDipVar());
          QString pred_serialized_r2y = SerializeMatrix(getPLSModelAt(i)->getPLSPrediction(j)->getR2Y());
          QString pred_serialized_sdec = SerializeMatrix(getPLSModelAt(i)->getPLSPrediction(j)->getSDEC());

          query.prepare("INSERT INTO plspredTable (name, plshash, hashinputmx, objname, yvarname, tscores, predicted_y, r2y, sdec) VALUES (:name, :plshash, :hashinputmx, :objname, :yvarname, :tscores, :predicted_y, :r2y, :sdec)");
          query.bindValue(":name", getPLSModelAt(i)->getPLSPrediction(j)->getName()/*.remove("PLS Prediction - ", Qt::CaseSensitive).trimmed()*/);
          query.bindValue(":plshash", getPLSModelAt(i)->getHash());
          query.bindValue(":hashinputmx", getPLSModelAt(i)->getPLSPrediction(j)->getDataHash());
          query.bindValue(":objname", pred_objname_serialized);
          query.bindValue(":yvarname", pred_yvarname_serialized);
          query.bindValue(":tscores", pred_serialized_mx);
          query.bindValue(":predicted_y", pred_serialized_predicted_y);
          query.bindValue(":r2y", pred_serialized_r2y);
          query.bindValue(":sdec", pred_serialized_sdec);
          query.exec();
        }
      }
    }
  }

  query.exec(QString("CREATE TABLE IF NOT EXISTS varselTable (name TEXT, info TEXT, varname TEXT, hashinputmx TEXT, idselectedvars TEXT, vardistribution TEXT, map TEXT)"));
  if(VarSelCount() > 0){
    for(int i = 0; i < VarSelCount(); i++){
      QString name = getVarSelModelAt(i)->getName();
      QString serialized_varnames = SerializeQStringList(getVarSelModelAt(i)->getVariableNames());
      QString serialized_vsel = SerializeUIVector(getVarSelModelAt(i)->getSelectedVariables());
      QString serialized_vdist = SerializeUIVector(getVarSelModelAt(i)->getVariableDistribution());
      QString serialized_map = SerializeMatrix(getVarSelModelAt(i)->getMap());

      QStringList info;
      info.append(getVarSelModelAt(i)->getVariableSelectionAlgorithm());
      info.append(getVarSelModelAt(i)->getVariableSelectionAlgorithmOptions());
      QString serialized_info = SerializeQStringList(info);

      query.prepare("INSERT INTO varselTable (name, info, varname, hashinputmx, idselectedvars, vardistribution, map) VALUES (:name, :info, :varname, :modelhash, :idselectedvars, :vardistribution, :map)");
      query.bindValue(":name", name);
      query.bindValue(":info", serialized_info);
      query.bindValue(":varname", serialized_varnames);
      query.bindValue(":hashinputmx", getVarSelModelAt(i)->getModelHash());
      query.bindValue(":idselectedvars", serialized_vsel);
      query.bindValue(":vardistribution", serialized_vdist);
      query.bindValue(":map", serialized_map);
      query.exec();
    }
  }

  query.exec(QString("CREATE TABLE IF NOT EXISTS mlrTable (name TEXT, hashinputmx TEXT, objname TEXT, xvarname TEXT, yvarname TEXT, b TEXT, r2y TEXT, sdec TEXT,  recalc_y TEXT, recalc_residuals TEXT, validationtype INT, ymean TEXT, q2y TEXT, sdep TEXT, bias TEXT, predicted_y TEXT, predicted_residuals TEXT, r2q2scrambling TEXT)"));
  query.exec(QString("CREATE TABLE IF NOT EXISTS mlrpredTable (name TEXT, mlrhash TEXT, hashinputmx TEXT, objname TEXT, yvarname TEXT, predicted_y TEXT, r2y TEXT, sdec TXT)"));
  if(MLRCount() > 0){
    for(int i = 0; i < MLRCount(); i++){
      QString modname = getMLRModelAt(i)->getName()/*.remove("MLR - ", Qt::CaseSensitive).trimmed();*/;
      QString objname_serialized = SerializeQStringList(getMLRModelAt(i)->getObjName());
      QString xvarname_serialized = SerializeQStringList(getMLRModelAt(i)->getXVarName());
      QString yvarname_serialized = SerializeQStringList(getMLRModelAt(i)->getYVarName());
      QString serialized_b = SerializeMatrix(getMLRModelAt(i)->Model()->b);
      QString serialized_recalc_y = SerializeMatrix(getMLRModelAt(i)->Model()->recalculated_y);
      QString serialized_recalc_residuals = SerializeMatrix(getMLRModelAt(i)->Model()->recalc_residuals);
      QString serialized_r2y_model = SerializeDVector(getMLRModelAt(i)->Model()->r2y_model);
      QString serialized_sdec = SerializeDVector(getMLRModelAt(i)->Model()->sdec);
      QString serialized_ymean = SerializeDVector(getMLRModelAt(i)->Model()->ymean);
      QString serialized_q2y = SerializeDVector(getMLRModelAt(i)->Model()->q2y);
      QString serialized_sdep = SerializeDVector(getMLRModelAt(i)->Model()->sdep);
      QString serialized_bias = SerializeDVector(getMLRModelAt(i)->Model()->bias);
      QString serialized_predicted_y = SerializeMatrix(getMLRModelAt(i)->Model()->predicted_y);
      QString serialized_pred_residuals = SerializeMatrix(getMLRModelAt(i)->Model()->pred_residuals);
      QString serialized_r2q2scrambling = SerializeMatrix(getMLRModelAt(i)->Model()->r2q2scrambling);

      query.prepare("INSERT INTO mlrTable (name, hashinputmx , objname, xvarname, yvarname, b, r2y, sdec,  recalc_y, recalc_residuals, validationtype, ymean, q2y, sdep, bias, predicted_y, predicted_residuals, r2q2scrambling) VALUES (:name, :hashinputmx , :objname, :xvarname, :yvarname, :b, :r2y, :sdec, :recalc_y, :recalc_residuals, :validationtype, :ymean, :q2y, :sdep, :bias, :predicted_y, :predicted_residuals, :r2q2scrambling)");
      query.bindValue(":name", modname);
      query.bindValue(":hashinputmx", getMLRModelAt(i)->getDataHash());
      query.bindValue(":objname", objname_serialized);
      query.bindValue(":xvarname", xvarname_serialized);
      query.bindValue(":yvarname", yvarname_serialized);
      query.bindValue(":b", serialized_b);
      query.bindValue(":r2y", serialized_r2y_model);
      query.bindValue(":sdec", serialized_sdec);
      query.bindValue(":recalc_y", serialized_recalc_y);
      query.bindValue(":recalc_residuals", serialized_recalc_residuals);
      query.bindValue(":validationtype", getMLRModelAt(i)->getValidation());
      query.bindValue(":ymean", serialized_ymean);
      query.bindValue(":q2y", serialized_q2y);
      query.bindValue(":sdep", serialized_sdep);
      query.bindValue(":bias", serialized_bias);
      query.bindValue(":predicted_y", serialized_predicted_y);
      query.bindValue(":predicted_residuals", serialized_pred_residuals);
      query.bindValue(":r2q2scrambling", serialized_r2q2scrambling);
      query.exec();

      if(getMLRModelAt(i)->MLRPredictionCount() > 0){
        for(int j = 0; j < getMLRModelAt(i)->MLRPredictionCount(); j++){
          QString pred_objname_serialized = SerializeQStringList(getMLRModelAt(i)->getMLRPrediction(j)->getObjName());
          QString pred_yvarname_serialized = SerializeQStringList(getMLRModelAt(i)->getMLRPrediction(j)->getYVarName());
          QString pred_serialized_predicted_y = SerializeMatrix(getMLRModelAt(i)->getMLRPrediction(j)->getYDipVar());
          QString pred_serialized_r2y = SerializeDVector(getMLRModelAt(i)->getMLRPrediction(j)->getR2Y());
          QString pred_serialized_sdec = SerializeDVector(getMLRModelAt(i)->getMLRPrediction(j)->getSDEC());

          query.prepare("INSERT INTO mlrpredTable (name, mlrhash, hashinputmx, objname, yvarname, predicted_y, r2y, sdec) VALUES (:name, :mlrhash, :hashinputmx, :objname, :yvarname, :predicted_y, :r2y, :sdec)");
          query.bindValue(":name", getMLRModelAt(i)->getMLRPrediction(j)->getName()/*.remove("PLS Prediction - ", Qt::CaseSensitive).trimmed()*/);
          query.bindValue(":mlrhash", getMLRModelAt(i)->getHash());
          query.bindValue(":hashinputmx", getMLRModelAt(i)->getMLRPrediction(j)->getDataHash());
          query.bindValue(":objname", pred_objname_serialized);
          query.bindValue(":yvarname", pred_objname_serialized);
          query.bindValue(":predicted_y", pred_serialized_predicted_y);
          query.bindValue(":r2y", pred_serialized_r2y);
          query.bindValue(":sdec", pred_serialized_sdec);
          query.exec();
        }
      }
    }
  }

  query.exec(QString("CREATE TABLE IF NOT EXISTS ldaTable (name TEXT, hashinputmx TEXT, objname TEXT, varname TEXT, validation INT, acc TEXT, npv TEXT, ppv TEXT, spec TEXT, sens TEXT, pprob TEXT, evect TEXT, eval TEXT, mu TEXT, mnpdf TEXT, features TEXT, fmean TEXT, fsdev TEXT, inv_cov TEXT, nclass INT, class_start INT, classid TEXT, classes TEXT, nameclasses TEXT)"));
  query.exec(QString("CREATE TABLE IF NOT EXISTS ldapredTable (name TEXT, ldahash TEXT, hashinputmx TEXT, objname TEXT, varname TEXT, pred_class TEXT, pred_features TEXT, prob TEXT, classes TEXT, nameclasses TEXT, mnpdf TEXT)"));
  if(LDACount() > 0){
    for(int i = 0; i < LDACount(); i++){
      QString modname = getLDAModelAt(i)->getName()/*.remove("LDA - ", Qt::CaseSensitive).trimmed()*/;
      QString objname_serialized = SerializeQStringList(getLDAModelAt(i)->getObjName());
      QString varname_serialized = SerializeQStringList(getLDAModelAt(i)->getVarName());

      QString serialized_accuracy = SerializeDVector(getLDAModelAt(i)->Model()->acc);
      QString serialized_negativepredictedvalues = SerializeDVector(getLDAModelAt(i)->Model()->npv);
      QString serialized_positivepredictedvalues = SerializeDVector(getLDAModelAt(i)->Model()->ppv);
      QString serialized_specificity = SerializeDVector(getLDAModelAt(i)->Model()->spec);
      QString serialized_sensitivity = SerializeDVector(getLDAModelAt(i)->Model()->sens);
      QString serialized_priorprobability = SerializeDVector(getLDAModelAt(i)->Model()->pprob);

      QString serialized_eigenvalues = SerializeDVector(getLDAModelAt(i)->Model()->eval);
      QString serialized_eigenvectors = SerializeMatrix(getLDAModelAt(i)->Model()->evect);
      QString serialized_mu = SerializeMatrix(getLDAModelAt(i)->Model()->mu);
      QString serialized_mnpdf = SerializeArray(getLDAModelAt(i)->Model()->mnpdf);
      QString serialized_features = SerializeArray(getLDAModelAt(i)->Model()->features);
      QString serialized_fmean = SerializeMatrix(getLDAModelAt(i)->Model()->fmean);
      QString serialized_fsdev = SerializeMatrix(getLDAModelAt(i)->Model()->fsdev);
      QString serialized_invcov = SerializeMatrix(getLDAModelAt(i)->Model()->inv_cov);
      int nclass = getLDAModelAt(i)->Model()->nclass;
      int class_start = getLDAModelAt(i)->Model()->class_start;
      QString serialized_classid = SerializeUIVector(getLDAModelAt(i)->Model()->classid);

      QList<QStringList> classes = getLDAModelAt(i)->getClasses();
      QStringList nameclasses = getLDAModelAt(i)->getNameClasses();

      QString serialized_classes;
      for(int j = 0; j < classes.size()-1; j++)
        serialized_classes += SerializeQStringList(classes[j])+"\\";
      serialized_classes += SerializeQStringList(classes.last());

      QString serialized_nameclasses = SerializeQStringList(nameclasses);

      query.prepare("INSERT INTO ldaTable (name, hashinputmx, objname, varname, validation, acc, npv, ppv, spec, sens, pprob, evect, eval, mu, mnpdf, features, fmean, fsdev, inv_cov, nclass, class_start, classid, classes, nameclasses) VALUES (:name, :hashinputmx, :objname, :varname, :validation, :acc, :npv, :ppv, :spec, :sens, :pprob, :evect, :eval, :mu, :mnpdf, :features, :fmean, :fsdev, :inv_cov, :nclass, :class_start, :classid, :classes, :nameclasses)");
      query.bindValue(":name", modname);
      query.bindValue(":hashinputmx", getLDAModelAt(i)->getDataHash());
      query.bindValue(":objname", objname_serialized);
      query.bindValue(":varname", varname_serialized);
      query.bindValue(":validation", getLDAModelAt(i)->getValidation());
      query.bindValue(":acc", serialized_accuracy);
      query.bindValue(":npv", serialized_negativepredictedvalues);
      query.bindValue(":ppv", serialized_positivepredictedvalues);
      query.bindValue(":spec", serialized_specificity);
      query.bindValue(":sens", serialized_sensitivity);
      query.bindValue(":pprob", serialized_priorprobability);
      query.bindValue(":evect", serialized_eigenvectors);
      query.bindValue(":eval", serialized_eigenvalues);
      query.bindValue(":mu", serialized_mu);
      query.bindValue(":mnpdf", serialized_mnpdf);
      query.bindValue(":features", serialized_features);
      query.bindValue(":fmean", serialized_fmean);
      query.bindValue(":fsdev", serialized_fsdev);
      query.bindValue(":inv_cov", serialized_invcov);
      query.bindValue(":nclass", nclass);
      query.bindValue(":class_start", class_start);
      query.bindValue(":classid", serialized_classid);
      query.bindValue(":classes", serialized_classes);
      query.bindValue(":nameclasses", serialized_nameclasses);
      query.exec();


      if(getLDAModelAt(i)->LDAPredictionCount() > 0){
        for(int j = 0; j < getLDAModelAt(i)->LDAPredictionCount(); j++){
          QString pred_objname_serialized = SerializeQStringList(getLDAModelAt(i)->getLDAPrediction(j)->getObjName());
          QString pred_varname_serialized = SerializeQStringList(getLDAModelAt(i)->getLDAPrediction(j)->getVarName());
          QString pred_serialized_predicted_class = SerializeUIVector(getLDAModelAt(i)->getLDAPrediction(j)->getPredClasses());
          QString pred_serialized_predicted_features = SerializeMatrix(getLDAModelAt(i)->getLDAPrediction(j)->getPredFeatures());
          QString pred_serialized_probability = SerializeMatrix(getLDAModelAt(i)->getLDAPrediction(j)->getProbPred());
          QString pred_serialized_mnpdf = SerializeMatrix(getLDAModelAt(i)->getLDAPrediction(j)->getMVNProbDistrib());

          QList<QStringList> classes = getLDAModelAt(i)->getLDAPrediction(j)->getClasses();
          QStringList nameclasses = getLDAModelAt(i)->getLDAPrediction(j)->getNameClasses();
          QString pred_serialized_classes;
          for(int k = 0; k < classes.size()-1; k++)
            pred_serialized_classes += SerializeQStringList(classes[k])+"\\";
          pred_serialized_classes += SerializeQStringList(classes.last());

          QString pred_serialized_nameclasses = SerializeQStringList(nameclasses);

          query.prepare("INSERT INTO ldapredTable (name, ldahash, hashinputmx, objname, varname, pred_class, pred_features, prob, classes, nameclasses, mnpdf) VALUES (:name, :ldahash, :hashinputmx, :objname, :yvarname, :pred_class, :pred_features, :prob, :classes, :nameclasses, :mnpdf)");
          query.bindValue(":name", getLDAModelAt(i)->getLDAPrediction(j)->getName());
          query.bindValue(":ldahash", getLDAModelAt(i)->getHash());
          query.bindValue(":hashinputmx", getLDAModelAt(i)->getLDAPrediction(j)->getDataHash());
          query.bindValue(":objname", pred_objname_serialized);
          query.bindValue(":varname", pred_varname_serialized);
          query.bindValue(":pred_class", pred_serialized_predicted_class);
          query.bindValue(":pred_features", pred_serialized_predicted_features);
          query.bindValue(":prob", pred_serialized_probability);
          query.bindValue(":classes", pred_serialized_classes);
          query.bindValue(":nameclasses", pred_serialized_nameclasses);
          query.bindValue(":mnpdf", pred_serialized_mnpdf);
          query.exec();
        }
      }
    }
  }

  /*
  // Compress
  DirCompressor dc;
  dc.setDir(savedir.toUtf8().data());
  dc.setOutput(savefname.toUtf8().data());
  dc.compress();
  DATAIO::RemoveDir(savedir.toUtf8().data());
  */
  db.close();
  pbdialog.setValue(5);
  return dbName;
}

void DATA::addMatrix()
{

  matrix_.append(new MATRIX());
}

void DATA::addMatrix(MATRIX* mx)
{
  matrix_.append(new MATRIX());
  matrix *m = matrix_.last()->Matrix();
  MatrixCopy(mx->Matrix(), &m);
  matrix_.last()->getObjName().append(mx->getObjName());
  matrix_.last()->getVarName().append(mx->getVarName());
  matrix_.last()->setName(mx->getName());
}

void DATA::addArray()
{
  array_.append(new ARRAY());
}

void DATA::addArray(ARRAY* ar)
{
  array_.append(new ARRAY());
  array *ar_ = array_.last()->Array();

  for(uint order = 0; order < ar->Array()->order; order++){
    ArrayAppendMatrix(&ar_, ar->Array()->m[order]);
  }

  array_.last()->getObjName().append(ar->getObjName());
  array_.last()->getVarName().append(ar->getVarName());
  array_.last()->setName(ar->getName());
}

void DATA::delMatrixAt(int i)
{
  if(i < matrix_.size()){
    delete matrix_[i];
    matrix_.removeAt(i);
  }

}

void DATA::delArrayAt(int i)
{
  if(i < array_.size()){
    delete array_[i];
    array_.removeAt(i);
  }
}

void DATA::delMatrix()
{
  for(int i = 0; i < matrix_.size(); i++){
    delete matrix_[i];
  }
  matrix_.clear();
}

void DATA::delArray()
{
  for(int i = 0; i < array_.size(); i++){
    delete array_[i];
  }
  array_.clear();
}

void DATA::addPCAModel()
{
  pcamodel.append(new PCAModel());
}

void DATA::delPCAModel(int mid)
{
  for(int i = 0; i < pcamodel.size(); i++){
    if(mid == pcamodel[i]->getModelID()){
      pcamodel[i]->delPCAPredictions();
      delete pcamodel[i];
      pcamodel.removeAt(i);
      break;
    }
    else{
      continue;
    }
  }
}

void DATA::delPCAModelAt(int id)
{
  if(id < pcamodel.size()){
    pcamodel[id]->delPCAPredictions();
    delete pcamodel[id];
    pcamodel.removeAt(id);
  }
}

void DATA::delPCAModels()
{
 for(int i = 0; i < pcamodel.size(); i++){
    pcamodel[i]->delPCAPredictions();
    delete pcamodel[i];
  }
  pcamodel.clear();
}

void DATA::addPLSModel()
{
  plsmodel.append(new PLSModel());
}

void DATA::delPLSModel(int mid)
{
  for(int i = 0; i < plsmodel.size(); i++){
    if(mid == plsmodel[i]->getModelID()){
      plsmodel[i]->delPLSPredictions();
      delete plsmodel[i];
      plsmodel.removeAt(i);
      break;
    }
    else{
      continue;
    }
  }
}

void DATA::delPLSModelAt(int id)
{
  if(id < plsmodel.size()){
    plsmodel[id]->delPLSPredictions();
    delete plsmodel[id];
    plsmodel.removeAt(id);
  }
}


void DATA::delPLSModels()
{
  for(int i = 0; i < plsmodel.size(); i++){
    plsmodel[i]->delPLSPredictions();
    delete plsmodel[i];
  }
  plsmodel.clear();
}

void DATA::addVarSelModel()
{
  varselectionmodel.append(new VariableSelectionModel());
}

void DATA::delVarSelModel(int mid)
{
  for(int i = 0; i < varselectionmodel.size(); i++){
    if(mid == varselectionmodel[i]->getModelID()){
      delete varselectionmodel[i];
      varselectionmodel.removeAt(i);
      break;
    }
    else{
      continue;
    }
  }
}

void DATA::delVarSelModelAt(int id)
{
  if(id < varselectionmodel.size()){
    delete varselectionmodel[id];
    varselectionmodel.removeAt(id);
  }
}

void DATA::delVarSelModels()
{
  for(int i = 0; i < varselectionmodel.size(); i++){
    delete varselectionmodel[i];
  }
  varselectionmodel.clear();
}


void DATA::addMLRModel()
{
  mlrmodel.append(new MLRModel());
}

void DATA::delMLRModel(int mid)
{
  for(int i = 0; i < mlrmodel.size(); i++){
    if(mid == mlrmodel[i]->getModelID()){
      delete mlrmodel[i];
      mlrmodel.removeAt(i);
      break;
    }
    else{
      continue;
    }
  }
}

void DATA::delMLRModelAt(int id)
{
  if(id < mlrmodel.size()){
    delete mlrmodel[id];
    mlrmodel.removeAt(id);
  }
}

void DATA::delMLRModels()
{
  for(int i = 0; i < mlrmodel.size(); i++){
    delete mlrmodel[i];
  }
  mlrmodel.clear();
}


void DATA::addLDAModel()
{
  ldamodel.append(new LDAModel());
}

void DATA::delLDAModel(int mid)
{
  for(int i = 0; i < ldamodel.size(); i++){
    if(mid == ldamodel[i]->getModelID()){
      delete ldamodel[i];
      ldamodel.removeAt(i);
      break;
    }
    else{
      continue;
    }
  }
}

void DATA::delLDAModelAt(int id)
{
  if(id < ldamodel.size()){
    delete ldamodel[id];
    ldamodel.removeAt(id);
  }
}

void DATA::delLDAModels()
{
  for(int i = 0; i < ldamodel.size(); i++){
    delete ldamodel[i];
  }
  ldamodel.clear();
}

void DATA::setProjectID(int id_)
{
  uniqueid = id_;
}

int DATA::getProjectID()
{
  return uniqueid;
}

int DATA::getMatrixID(QString hash)
{
  for(int i = 0; i < matrix_.size(); i++){
    if(matrix_[i]->getHash().compare(hash) == 0){
      return i;
    }
    else{
      continue;
    }
  }
  return -1;
}

int DATA::getArrayID(QString hash)
{
  for(int i = 0; i < array_.size(); i++){
    if(array_[i]->getHash().compare(hash) == 0){
      return i;
    }
    else{
      continue;
    }
  }
  return -1;
}

MATRIX* DATA::getMatrix(int id)
{
  Q_ASSERT(id < matrix_.size());
  return matrix_[id];
}

MATRIX* DATA::getMatrix(QString hash)
{
  for(int i = 0; i < matrix_.size(); i++){
    if(matrix_[i]->getHash().compare(hash) == 0){
      return matrix_[i];
    }
    else{
      continue;
    }
  }
  return 0;
}


ARRAY* DATA::getArray(int id)
{
  Q_ASSERT(id < array_.size());
  return array_.at(id);
}

ARRAY* DATA::getArray(QString hash)
{
  for(int i = 0; i < array_.size(); i++){
    if(array_[i]->getHash().compare(hash) == 0){
      return array_[i];
    }
    else{
      continue;
    }
  }
  return 0;
}


PCAModel* DATA::getLastPCAModel()
{
  return pcamodel.last();
}

PLSModel* DATA::getLastPLSModel()
{
  return plsmodel.last();
}

VariableSelectionModel* DATA::getLastVarSelModel()
{
  return varselectionmodel.last();
}

LDAModel* DATA::getLastLDAModel()
{
  return ldamodel.last();
}

MLRModel* DATA::getLastMLRModel()
{
  return mlrmodel.last();
}

PCAModel* DATA::getPCAModelAt(int id)
{
  Q_ASSERT(id < pcamodel.size());
  return pcamodel[id];
}

PLSModel* DATA::getPLSModelAt(int id)
{
  Q_ASSERT(id < plsmodel.size());
  return plsmodel[id];
}

VariableSelectionModel* DATA::getVarSelModelAt(int id)
{
  Q_ASSERT(id < varselectionmodel.size());
  return varselectionmodel[id];
}

LDAModel* DATA::getLDAModelAt(int id)
{
  Q_ASSERT(id < ldamodel.size());
  return ldamodel[id];
}

MLRModel* DATA::getMLRModelAt(int id)
{
  Q_ASSERT(id < mlrmodel.size());
  return mlrmodel[id];
}

PCAModel* DATA::getPCAModel(int mid)
{
  for(int i = 0; i < pcamodel.size(); i++){
    if(mid == pcamodel[i]->getModelID()){
      return pcamodel[i];
      break;
    }
    else{
      continue;
    }
  }
  return 0;
}

PLSModel* DATA::getPLSModel(int mid)
{
  for(int i = 0; i < plsmodel.size(); i++){
    if(mid == plsmodel[i]->getModelID()){
      return plsmodel[i];
      break;
    }
    else{
      continue;
    }
  }
  return 0;
}

PLSModel* DATA::getPLSModel(QString hash)
{
  for(int i = 0; i < plsmodel.size(); i++){
    if(plsmodel[i]->getHash().compare(hash) == 0){
      return plsmodel[i];
      break;
    }
    else{
      continue;
    }
  }
  return 0;
}

LDAModel* DATA::getLDAModel(int mid)
{
  for(int i = 0; i < ldamodel.size(); i++){
    if(mid == ldamodel[i]->getModelID()){
      return ldamodel[i];
      break;
    }
    else{
      continue;
    }
  }
  return 0;
}

MLRModel* DATA::getMLRModel(int mid)
{
  for(int i = 0; i < mlrmodel.size(); i++){
    if(mid == mlrmodel[i]->getModelID()){
      return mlrmodel[i];
      break;
    }
    else{
      continue;
    }
  }
  return 0;
}

VariableSelectionModel* DATA::getVarSelModel(int mid)
{
  for(int i = 0; i < varselectionmodel.size(); i++){
    if(mid == varselectionmodel[i]->getModelID()){
      return varselectionmodel[i];
      break;
    }
    else{
      continue;
    }
  }
  return 0;
}

int DATA::MatrixCount()
{
  return matrix_.size();
}

int DATA::ArrayCount()
{
  return array_.size();
}

int DATA::PCACount()
{
  return pcamodel.size();
}

int DATA::PLSCount()
{
  return plsmodel.size();
}

int DATA::VarSelCount()
{
  return varselectionmodel.size();
}

int DATA::LDACount()
{
  return ldamodel.size();
}

int DATA::MLRCount()
{
  return mlrmodel.size();
}

DATA::DATA()
{
  uniqueid = -1;
}

DATA::~DATA()
{
  #ifdef DEBUG
  qDebug() << "Delete Data";
  #endif
  delMatrix();
  delArray();
  delPCAModels();
  delPLSModels();
  delVarSelModels();
  delMLRModels();
}
