#include "qsmdatamanager.h"

QStringList DeserializeQStringList(QString serialized_lst)
{
  return serialized_lst.split(";");
}

QString SerializeQStringList(QStringList lst)
{
  QString serialized_lst;
  for(int i = 0; i < lst.size(); i++)
    serialized_lst += ";"+lst[i]
  return serialized_lst;
}

void DeserializeMatrix(QString serialized_mx, matrix **mx)
{
  QList< QList<QString> > _mx_;
  QStringList row = serialized_mx.split("\n")
  for(int i = 0; i < row.size(); i++){
    _mx_.append(row.split(";"));
  }
  ResizeMatrix(mx, _mx_.size(), _mx_[0].size());
  for(int i = 0; i < _mx_.size(); i++)
    for(int j = 0; j < _mx_[i].size(); j++)
      (*mx)->data[i][j] = _mx_[i][j].toDouble();
}

QString SerializeMatrix(matrix *mx)
{
  QString serialized_mx;
  for(int i = 0; i < mx->row; i++){
    //start a row and concatenate values
    for(int j = 0; j < mx->col; j++){
      serialized_mx += ";" + QString::number(mx->data[i][j]);
    }
    serialized_mx += "\n"; //end row
  }
  return serialized_mx;
}

void DeserializeArray(QString serialized_ar, array **ar)
{
  QList< QList< QList<QString> > > _ar_;
  QStringList orders = serialized_ar.split("o");
  for(int k = 0; k < orders.size(); k++){
    QStringList row = orders[k].split("\n");
    for(int i = 0; i < row.size(); i++){
      _ar_.append(row.split(";"))
    }
  }

  //NewArray(ar, _ar_.size());
  for(int k = 0; k < _ar_.size(); k++){
    AddArrayMatrix(ar, _ar_[k].size(), _ar_[k][0].size());
    for(int i = 0; i < _ar_[k].size(); i++){
      for(int j = 0; j < _ar_[k][i].size(); j++){
        (ar)->m[k]->data[i][j] = _ar_[k][i][j].toDouble();
      }
    }
  }
}

QString SerializeArray(array *ar)
{
  QString serialized_ar;
  for(int k = 0; k < ar->order; k++){
    for(int i = 0; i < ar->row; i++){
      //start a row and concatenate values
      for(int j = 0; j < mx->col; j++){
        serialized_ar += ";" + QString::number(mx->data[i][j]);
      }
      serialized_ar += "\n"; //end row
     }
     serialized_ar += "o";
  }
  return serialized_ar;
}

QString DATA::SaveData(QString dbName, QString modname)
{
  //QString dbName( "myDatabase.db3" );
  QFile::remove(dbName);
  QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
  db.setDatabaseName( dbName );
  db.open();
  QSqlQuery query = QSqlQuery(db);
  query.exec(QString("CREATE TABLE IF NOT EXISTS %1(modeltype TEXT, name TEXT, objnames TEXT, varnames  TEXT, tables TEXT)").arg(modname));

  if(MatrixCount() > 0){
    for(int i = 0; i < MatrixCount(); i++){
      QString mxname = getMatrix(i)->getName()
      QString serialized_mx = (SerializeMatrix(getMatrix(i)->Matrix());
      QString objname_serialized = SerializeQStringList(getMatrix(i)->getObjName());
      QStringList varname = getMatrix(i)->getVarName();
      varname.removeFirst();
      QString varname_serialized = SerializeQStringList(varname);
      query.exec(QString("INSERT INTO %1 (modeltype, name, objnames,  varnames, tables) VALUES ('matrix', '%2', %3', '%4', '%5', '%6')").arg(modname).arg(mxname).arg(objname_serialized).arg(varname_serialized).arg(serialized_mx));
    }
  }

  pbdialog.setValue(1);

  if(ArrayCount() > 0){
    for(int i = 0; i < ArrayCount(); i++){
      QString arname = getArray(i)->getName().toUtf8();
      QString serialized_ar = (SerializeArray(getArray(i)->Array());
      QString objname_serialized = SerializeQStringList(getArray(i)->getObjName());
      QStringList varname = getArray(i)->getVarName();
      varname.removeFirst();
      QString varname_serialized = SerializeQStringList(varname);
      query.exec(QString("INSERT INTO %1 (modeltype, name, objnames,  varnames, tables) VALUES ('array', '%2', %3', '%4', '%5', '%6')").arg(modname).arg(arname).arg(objname_serialized).arg(varname_serialized).arg(serialized_ar));
    }
  }

  pbdialog.setValue(2);

  //CONTINUE FROM HERE!!!
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
      qDebug() << varname.first();
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
  return savefname;
}


QSMDataManager::QSMDataManager(const QString& path)
{
  /*
  m_db = QSqlDatabase::addDatabase("QSQLITE");
  m_db.setDatabaseName(path);

  if(!m_db.open()){
    qDebug() << "Error: connection with database fail";
  }
  else{
    qDebug() << "Database: connection ok";
  }*/
}
