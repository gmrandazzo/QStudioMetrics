#include "qsmdata.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QIODevice>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include "DATAIO.h"
#include "dircompressor.h"
#include "MainWindow.h"
// #include "ProgressBarDialog.h"
#include "qstudiometricstypes.h"

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
    getMatrix(MatrixCount()-1)->GenHash();
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
    getArray(ArrayCount()-1)->GenHash();
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
    QDir upcadir = QDir(modelsdir.absolutePath()+"/UPCAMODELS");
    QDir uplsdir = QDir(modelsdir.absolutePath()+"/UPLSMODELS");
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

    if(upcadir.exists() == true){
      QStringList upcalist = upcadir.entryList();
      upcalist.removeAll(".");
      upcalist.removeAll("..");
      for(int i = 0; i < upcalist.size(); i++){
        QDir upca = QDir(QString::fromUtf8(upcadir.absolutePath().toUtf8())+"/"+upcalist[i]);
        if(upca.exists() == true
          && QFile::exists(upca.absolutePath()+"/info.txt") == true){
          (*log).append(QString("Importing UPCA Model %1").arg(upcalist[i]));
          addUPCAModel();
          getLastUPCAModel()->ImportUPCAModel(upca.absolutePath().toUtf8().data(), QString("UPCA - %1").arg(QString::fromUtf8(upcalist[i].toUtf8())).toUtf8().data());
          getLastUPCAModel()->setModelID((*mid_));

          int xid = -1;
          for(int j = 0; j < ArrayCount(); j++){
            if(getArray(j)->getHash().compare(getLastUPCAModel()->getDataHash()) == 0){
              xid = j;
              break;
            }
          }

          if(xid == -1){
            (*log).append(QString("Warning UPCA Model %1. Unable to find data").arg(upcalist[i]));
          }

          getLastUPCAModel()->setDID(xid);
          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0, getLastUPCAModel()->getName());
          subitem->setText(1, QString::number((*tabcount_)));
          subitem->setText(2, QString::number(getProjectID()));
          subitem->setText(3, getLastUPCAModel()->getDataHash());
          subitem->setText(4, QString("-"));
          subitem->setText(5, QString::number(getLastUPCAModel()->getXScaling()));
          subitem->setText(6, QString("-"));
          subitem->setText(7, QString::number(getLastUPCAModel()->getNPC()));
          subitem->setText(8, QString("UPCA Model"));
          subitem->setText(9, QString::number((*mid_)));
          MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
          (*tabcount_)++;
          QDir upcapred = upca.absolutePath()+"/PREDICTIONS";
          if(upcapred.exists() == true){
            QStringList upcapredlist = upcapred.entryList();
            upcapredlist.removeAll(".");
            upcapredlist.removeAll("..");
            for(int j = 0; j < upcapredlist.size(); j++){
              QDir upcapredpath = QDir(upcapred.absolutePath()+"/"+upcapredlist[j]);
              (*log).append(QString("Importing UPCA Model %1").arg(upcapredlist[j]));
              if(upcapredpath.exists()
                && QFile::exists(QString::fromUtf8(upcapredpath.absolutePath().toUtf8())+"/T-Scores-Pred.txt") == true
                && QFile::exists(QString::fromUtf8(upcapredpath.absolutePath().toUtf8())+"/info.txt") == true){
                getLastUPCAModel()->addUPCAPrediction();
                getLastUPCAModel()->getLastUPCAPrediction()->ImportUPCAPrediction(upcapredpath.absolutePath().toUtf8().data(), QString::fromUtf8("UPCA Prediction - %1").arg(QString::fromUtf8(upcapredlist[j].toUtf8())).toUtf8().data());
                getLastUPCAModel()->getLastUPCAPrediction()->setPredID(getLastUPCAModel()->UPCAPredictionCount()-1);

                int predid = -1;
                for(int k = 0; k <  ArrayCount(); k++){
                  if(getArray(k)->getHash().compare(getLastUPCAModel()->getLastUPCAPrediction()->getDataHash()) == 0){
                    predid = k;
                    break;
                  }
                  else{
                    continue;
                  }
                }

                if(predid == -1){
                  (*log).append(QString("Warning UPCA Prediction %1. Unable to find data").arg(upcapredlist[j]));
                }

                getLastUPCAModel()->getLastUPCAPrediction()->setDID(predid);
        //         ModelPrediction Name - Tab Count - pid_ - Model ID - xdata id - ydata id - Data Position - Data Type (UPCA Prediction, UUPCA Prediction, ...) (8)
                QTreeWidgetItem *subitem = new QTreeWidgetItem;
                subitem->setText(0, getUPCAModel((*mid_))->getLastUPCAPrediction()->getName());
                subitem->setText(1, QString::number((*tabcount_)));
                subitem->setText(2, QString::number(getProjectID()));
                subitem->setText(3, QString::number((*mid_)));
                subitem->setText(4, getUPCAModel((*mid_))->getLastUPCAPrediction()->getDataHash());
                subitem->setText(5, "-");
                subitem->setText(6, QString::number(getUPCAModel((*mid_))->getLastUPCAPrediction()->getPredID()));
                subitem->setText(7, QString("UPCA Prediction"));
                (*tabcount_)++;
                MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
              }
            }
          }
          (*mid_)++;
        }
        else{
          (*log).append(QString("Error UPCA Model %1 Corrupted").arg(upcalist[i]));
        }
      }
    }

    if(uplsdir.exists() == true){
      QStringList uplslist = uplsdir.entryList();
      uplslist.removeAll(".");
      uplslist.removeAll("..");

      for(int i = 0; i < uplslist.size(); i++){
        QDir upls = QDir(QString::fromUtf8(uplsdir.absolutePath().toUtf8())+"/"+QString::fromUtf8(uplslist[i].toUtf8()));
        if(upls.exists() == true
          && QFile::exists(QString::fromUtf8(upls.absolutePath().toUtf8())+"/info.txt") == true){
          (*log).append(QString("Importing UPLS Model %1").arg(uplslist[i]));

          addUPLSModel();
          getLastUPLSModel()->ImportUPLSModel(upls.absolutePath().toUtf8().data(), QString("UPLS - %1").arg(QString::fromUtf8(uplslist[i].toUtf8())).toUtf8().data());
          getLastUPLSModel()->setModelID((*mid_));

          int xid = -1, yid = -1;
          for(int j = 0; j < ArrayCount(); j++){
            if(getArray(j)->getHash().compare(getLastUPLSModel()->getXHash()) == 0){
              xid = j;
            }
            else if(getArray(j)->getHash().compare(getLastUPLSModel()->getYHash()) == 0){
              yid = j;
            }
            else{
              continue;
            }
          }

          if(xid == -1){
            (*log).append(QString("Warning in UPLS Model %1. Unable to find data.").arg(uplslist[i]));
          }

          getLastUPLSModel()->setXID(xid);
          getLastUPLSModel()->setYID(yid);
          QTreeWidgetItem *subitem = new QTreeWidgetItem;
          subitem->setText(0,  getLastUPLSModel()->getName());
          subitem->setText(1, QString::number((*tabcount_)));
          subitem->setText(2, QString::number(getProjectID()));
          subitem->setText(3, getLastUPLSModel()->getXHash());
          subitem->setText(4, getLastUPLSModel()->getYHash());
          subitem->setText(5, QString::number(getLastUPLSModel()->getXScaling()));
          subitem->setText(6, QString::number(getLastUPLSModel()->getYScaling()));
          subitem->setText(7, QString::number(getLastUPLSModel()->getNPC()));
          subitem->setText(8, QString("UPLS Model"));
          subitem->setText(9, QString::number((*mid_)));
          MainWindow::getProjectItem(getProjectID(), treeWidget)->child(1)->addChild(subitem);
          (*tabcount_)++;
          QDir uplspred = upls.absolutePath()+"/PREDICTIONS";
          if(uplspred.exists() == true){
            QStringList uplspredlist = uplspred.entryList();
            uplspredlist.removeAll(".");
            uplspredlist.removeAll("..");
            for(int j = 0; j < uplspredlist.size(); j++){
              QDir uplspredpath = QDir(uplspred.absolutePath()+"/"+uplspredlist[j]);
              (*log).append(QString("Importing UPLS Model %1").arg(uplspredlist[j]));
              if(uplspredpath.exists()
                && QFile::exists(QString::fromUtf8(uplspredpath.absolutePath().toUtf8())+"/Y-Pred.txt") == true
                && QFile::exists(QString::fromUtf8(uplspredpath.absolutePath().toUtf8())+"/T-Scores-Pred.txt") == true
                && QFile::exists(QString::fromUtf8(uplspredpath.absolutePath().toUtf8())+"/info.txt") == true){
                getLastUPLSModel()->addUPLSPrediction();
                getLastUPLSModel()->getLastUPLSPrediction()->ImportUPLSPrediction(uplspredpath.absolutePath().toUtf8().data(), QString("UPLS Prediction - %1").arg(QString::fromUtf8(uplspredlist[j].toUtf8())).toUtf8().data());
                getLastUPLSModel()->getLastUPLSPrediction()->setPredID(getLastUPLSModel()->UPLSPredictionCount()-1);

                int xpredid = -1, ypredid = -1;
                for(int k = 0; k <  ArrayCount(); k++){
                  if(getArray(k)->getHash().compare(getLastUPLSModel()->getLastUPLSPrediction()->getXHash()) == 0){
                    xpredid = k;
                  }
                  else if(getArray(k)->getHash().compare(getLastUPLSModel()->getLastUPLSPrediction()->getYHash()) == 0){
                    ypredid = k;
                  }
                  else{
                    continue;
                  }
                }

                if(xpredid == -1){
                  (*log).append(QString("Warning in UPLS Prediction %1. Unable to find data.").arg(uplspredlist[j]));
                }
                getLastUPLSModel()->getLastUPLSPrediction()->setXID(xpredid);
                getLastUPLSModel()->getLastUPLSPrediction()->setYID(ypredid);
        //         ModelPrediction Name - Tab Count - pid_ - Model ID - x data id - y data id - Data Position - Data Type (UPLS Prediction, UUPLS Prediction, ...) (8)
                QTreeWidgetItem *subitem = new QTreeWidgetItem;
                subitem->setText(0, getUPLSModel((*mid_))->getLastUPLSPrediction()->getName());
                subitem->setText(1, QString::number((*tabcount_)));
                subitem->setText(2, QString::number(getProjectID()));
                subitem->setText(3, QString::number((*mid_)));
                subitem->setText(4, getLastUPLSModel()->getLastUPLSPrediction()->getXHash());
                subitem->setText(5, getLastUPLSModel()->getLastUPLSPrediction()->getYHash());
                subitem->setText(6, QString::number(getUPLSModel((*mid_))->getLastUPLSPrediction()->getPredID()));
                subitem->setText(7, QString("UPLS Prediction"));
                (*tabcount_)++;
                MainWindow::getModelItem(getProjectID(), (*mid_), treeWidget)->addChild(subitem);
              }
            }
          }
          (*mid_)++;
        }
        else{
          (*log).append(QString("Error UPLS Model %1 Corrupted").arg(uplslist[i]));
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
          subitem->setText(3, getLastVarSelModel()->getXHash());
          subitem->setText(4, getLastVarSelModel()->getYHash());
          subitem->setText(5, QString::number(getLastVarSelModel()->getXScaling()));
          subitem->setText(6, QString::number(getLastVarSelModel()->getYScaling()));
          subitem->setText(7, QString::number(getLastVarSelModel()->getNumberOfComponents()));
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
    /*DATAIO::MakeDir(labelspath.toStdString());*/
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


  if(UPCACount() > 0){
    DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/UPCAMODELS").toUtf8().data());
    for(int i = 0; i < UPCACount(); i++){
      QString dirname = getUPCAModelAt(i)->getName().remove("UPCA - ", Qt::CaseSensitive).trimmed();
      getUPCAModelAt(i)->WriteUPCAModel(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/UPCAMODELS/").toUtf8().data(), dirname.toUtf8().data());

      if(getUPCAModelAt(i)->UPCAPredictionCount() > 0){
        DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/UPCAMODELS/%1/PREDICTIONS").arg(dirname).toUtf8().data());
        for(int j = 0; j < getUPCAModelAt(i)->UPCAPredictionCount(); j++){
          QString dirnamepred = getUPCAModelAt(i)->getUPCAPrediction(j)->getName().remove("UPCA Prediction - ", Qt::CaseSensitive).trimmed();
          getUPCAModelAt(i)->getUPCAPrediction(j)->WriteUPCAPrediction(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/UPCAMODELS/%1/PREDICTIONS/").arg(dirname).toUtf8().data(), dirnamepred.toUtf8().data());
        }
      }
    }
  }

  if(UPLSCount() > 0){
    DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/UPLSMODELS").toUtf8().data());
    for(int i = 0; i < UPLSCount(); i++){
      QString dirname = getUPLSModelAt(i)->getName().remove("UPLS - ", Qt::CaseSensitive).trimmed();
      getUPLSModelAt(i)->WriteUPLSModel(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/UPLSMODELS/").toUtf8().data(), dirname.toUtf8().data());

      if(getUPLSModelAt(i)->UPLSPredictionCount() > 0){
        DATAIO::MakeDir(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/UPLSMODELS/"+getUPLSModelAt(i)->getName().remove("UPLS Model - ").trimmed()).toUtf8().data());
        for(int j = 0; j < getUPLSModelAt(i)->UPLSPredictionCount(); j++){
          QString dirnamepred = getUPLSModelAt(i)->getUPLSPrediction(j)->getName().remove("UPLS Prediction - ", Qt::CaseSensitive).trimmed();
          getUPLSModelAt(i)->getUPLSPrediction(j)->WriteUPLSPrediction(QString(QString::fromUtf8(savedir.toUtf8())+"/Models/UPLSMODELS/%1/PREDICTIONS/").arg(dirname).toUtf8().data(), dirnamepred.toUtf8().data());
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
  matrix_.last()->GenHash();
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
  array_.last()->GenHash();
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

void DATA::addUPCAModel()
{
  upcamodel.append(new UPCAModel());
}

void DATA::delUPCAModel(int mid)
{
  for(int i = 0; i < upcamodel.size(); i++){
    if(mid == upcamodel[i]->getModelID()){
      upcamodel[i]->delUPCAPredictions();
      delete upcamodel[i];
      upcamodel.removeAt(i);
      break;
    }
    else{
      continue;
    }
  }
}

void DATA::delUPCAModelAt(int id)
{
  if(id < upcamodel.size()){
    upcamodel[id]->delUPCAPredictions();
    delete upcamodel[id];
    upcamodel.removeAt(id);
  }
}


void DATA::delUPCAModels()
{
  for(int i = 0; i < upcamodel.size(); i++){
    upcamodel[i]->delUPCAPredictions();
    delete upcamodel[i];
  }
  upcamodel.clear();
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

void DATA::addUPLSModel()
{
  uplsmodel.append(new UPLSModel());
}

void DATA::delUPLSModel(int mid)
{
  for(int i = 0; i < uplsmodel.size(); i++){
    if(mid == uplsmodel[i]->getModelID()){
      uplsmodel[i]->delUPLSPredictions();
      delete uplsmodel[i];
      uplsmodel.removeAt(i);
      break;
    }
    else{
      continue;
    }
  }
}

void DATA::delUPLSModelAt(int id)
{
  if(id < uplsmodel.size()){
    uplsmodel[id]->delUPLSPredictions();
    delete uplsmodel[id];
    uplsmodel.removeAt(id);
  }
}

void DATA::delUPLSModels()
{
  for(int i = 0; i < uplsmodel.size(); i++){
    uplsmodel[i]->delUPLSPredictions();
    delete uplsmodel[i];
  }
  uplsmodel.clear();
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

UPCAModel* DATA::getLastUPCAModel()
{
  return upcamodel.last();
}

UPLSModel* DATA::getLastUPLSModel()
{
  return uplsmodel.last();
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

UPCAModel* DATA::getUPCAModelAt(int id)
{
  Q_ASSERT(id < upcamodel.size());
  return upcamodel[id];
}

UPLSModel* DATA::getUPLSModelAt(int id)
{
  Q_ASSERT(id < uplsmodel.size());
  return uplsmodel[id];
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

UPCAModel* DATA::getUPCAModel(int mid)
{
  for(int i = 0; i < upcamodel.size(); i++){
    if(mid == upcamodel[i]->getModelID()){
      return upcamodel[i];
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

UPLSModel* DATA::getUPLSModel(int mid)
{
  for(int i = 0; i < uplsmodel.size(); i++){
    if(mid == uplsmodel[i]->getModelID()){
      return uplsmodel[i];
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

int DATA::UPCACount()
{
  return upcamodel.size();
}

int DATA::UPLSCount()
{
  return uplsmodel.size();
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
  delUPCAModels();
  delUPLSModels();
  delVarSelModels();
  delMLRModels();
}
