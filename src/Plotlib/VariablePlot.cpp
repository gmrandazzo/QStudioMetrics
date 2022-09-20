#include "VariablePlot.h"
#include "qstudiometricstypes.h"

#ifdef DEBUG
#include <QDebug>
#endif

ScatterPlot* VariablePlot::VariableVSVariable()
{
  /*
  qDebug() << "Plot Variable VS Variable ";
  qDebug() << " XHASH " << xhash;
  qDebug() << " pid " << pid;
  qDebug() << " varid1 " << varid1;
  qDebug() << " varid2 " << varid2;
  qDebug() << " layerid " << layerid;
  qDebug() << " objects " << objects;
  qDebug() << " Labels " << selobjlabels;
  if(type == MATRIXDATA){
    qDebug() << "MATRIXDATA";
  }
  else{
    qDebug() << "ARRAYDATA";
  }
  */
  if(type == MATRIXDATA){
    QList< QStringList > objects_;
    objects_.append(objects);
    QList <QStringList > selobjects;

    QList<matrix*> mxlst;
    for(int i = 0; i < objects_.size(); i++){
      selobjects.append(QStringList());
      int c = 0;
      mxlst.append(new matrix);

      int nobj = 0;
      for(int j = 0; j < xhash.size(); j++){
        for(uint k = 0; k < projects->value(pid)->getMatrix(xhash[j])->Matrix()->row; k++){
          int indx = objects_[i].indexOf(projects->value(pid)->getMatrix(xhash[j])->getObjName()[k]);
          if(indx > -1){
            nobj++;
          }
          else{
            continue;
          }
        }
      }
      
      NewMatrix(&mxlst.last(), nobj, 2);

//       NewMatrix(&mxlst.last(), objects_[i].size(), 2);
      MatrixSet(mxlst.last(), 0.f);
      for(int j = 0; j < xhash.size(); j++){
        for(uint k = 0; k < projects->value(pid)->getMatrix(xhash[j])->Matrix()->row; k++){
          int indx = objects_[i].indexOf(projects->value(pid)->getMatrix(xhash[j])->getObjName()[k]);
          if(indx > -1){
            QString str = projects->value(pid)->getMatrix(xhash[j])->getObjName()[k];
            selobjects.last().append(str);
            setMatrixValue(mxlst.last(), c, 0, getMatrixValue(projects->value(pid)->getMatrix(xhash[j])->Matrix(), k, varid1));
            setMatrixValue(mxlst.last(), c, 1, getMatrixValue(projects->value(pid)->getMatrix(xhash[j])->Matrix(), k, varid2));
            c++;
            objects_[i].removeAt(indx);
          }
          else{
            continue;
          }
        }

      }
    }

    QString vname1 = projects->value(pid)->getMatrix(xhash[0])->getVarName()[varid1+1];
    QString vname2 = projects->value(pid)->getMatrix(xhash[0])->getVarName()[varid2+1];

    ScatterPlot *plt2D = new ScatterPlot(mxlst, selobjects,  &projects->value(pid)->getMATRIXList(), xhash, yhash,  &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), vname1, vname2, QString("Variable %1 VS Variable %2").arg(vname1).arg(vname2), ScatterPlot::SCORES);

    plt2D->SetAutoNameAxes(false);
    plt2D->setPID(pid);

    for(int i = 0; i < mxlst.size(); i++){
      DelMatrix(&mxlst[i]);
    }
    return plt2D;
  }
  else{
    QList<matrix*> mxlst;
    QList< QStringList > objects_ = objects;
    QList< QStringList > selobjects;

    for(int i = 0; i < objects_.size(); i++){
      selobjects.append(QStringList());
      int c = 0;
      mxlst.append(new matrix);
      NewMatrix(&mxlst.last(), objects_[i].size(), 2);
      MatrixSet(mxlst.last(), 0.f);
      for(int j = 0; j < xhash.size(); j++){
        for(uint k = 0; k < projects->value(pid)->getArray(xhash[j])->Array()->m[layerid]->row; k++){
          for(int m = 0; m < objects_[i].size(); m++){
            if(projects->value(pid)->getArray(xhash[j])->getObjName()[k].compare(objects_[i][m]) == 0){
              QString str = objects_[i][m];
              objects_[i].removeAt(m);
              selobjects.last().append(str);
              setMatrixValue(mxlst.last(), c, 0, getMatrixValue(projects->value(pid)->getArray(xhash[j])->Array()->m[layerid], k, varid1));
              setMatrixValue(mxlst.last(), c, 1, getMatrixValue(projects->value(pid)->getArray(xhash[j])->Array()->m[layerid], k, varid2));
              c++;
              break;
            }
            else{
              continue;
            }
          }
        }
      }
    }

    QString vname1 = projects->value(pid)->getArray(xhash[0])->getVarName()[varid1+1];
    QString vname2 = projects->value(pid)->getArray(xhash[0])->getVarName()[varid2+1];

    ScatterPlot *plt2D = new ScatterPlot(mxlst, selobjects, vname1, vname2, QString("Variable %1 VS Variable %2").arg(vname1).arg(vname2));

    plt2D->SetAutoNameAxes(false);
    plt2D->setPID(pid);

    for(int i = 0; i < mxlst.size(); i++){
      DelMatrix(&mxlst[i]);
    }
    return plt2D;
  }
}

BarPlot* VariablePlot::VariableDistribution()
{
  /*
  qDebug() << "Plot Variable Distribution";
  qDebug() << " XHASH " << xhash;
  qDebug() << " pid " << pid;
  qDebug() << " varid " << varid1;
  qDebug() << " layerid " << layerid;
  qDebug() << " objects " << objects;
  qDebug() << " Labels " << selobjlabels;
  if(type == MATRIXDATA){
    qDebug() << "MATRIXDATA";
  }
  else{
    qDebug() << "ARRAYDATA";
  }
  */

  if(type == MATRIXDATA){
    QList<dvector*> v;
    for(int i = 0; i < objects.size(); i++){
      v.append(new dvector);
      initDVector(&v.last());
      for(int j = 0; j < xhash.size(); j++){
        for(uint k = 0; k < projects->value(pid)->getMatrix(xhash[j])->Matrix()->row; k++){
          for(int m = 0; m < objects[i].size(); m++){
            if(projects->value(pid)->getMatrix(xhash[j])->getObjName()[k].compare(objects[i][m]) == 0){
              QString str = objects[i][m];
              objects[i].removeAll(str);
              DVectorAppend(&v.last(), getMatrixValue(projects->value(pid)->getMatrix(xhash[j])->Matrix(), k, varid1));
              break;
            }
            else{
              continue;
            }
          }
        }
      }
    }

    QString vname1 = projects->value(pid)->getMatrix(xhash[0])->getVarName()[varid1+1];

    BarPlot *bplot = new BarPlot(v, QString("Distributuion Plot for %1").arg(vname1), vname1, "Number of Objects", selobjlabels);

    for(int i = 0; i < v.size(); i++){
      DelDVector(&v[i]);
    }
    return bplot;
  }
  else{
    QList<dvector*> v;
    for(int i = 0; i < objects.size(); i++){
      v.append(new dvector);
      initDVector(&v.last());
      for(int j = 0; j < xhash.size(); j++){
        for(uint k = 0; k < projects->value(pid)->getArray(xhash[j])->Array()->m[layerid]->row; k++){
          for(int m = 0; m < objects[i].size(); m++){
            if(projects->value(pid)->getArray(xhash[j])->getObjName()[k].compare(objects[i][m]) == 0){
              QString str = objects[i][m];
              objects[i].removeAll(str);
              DVectorAppend(&v.last(), getMatrixValue(projects->value(pid)->getArray(xhash[j])->Array()->m[layerid], k, varid1));
              break;
            }
            else{
              continue;
            }
          }
        }
      }
    }

    QString vname1 = projects->value(pid)->getArray(xhash[0])->getVarName()[varid1+1];

    BarPlot *bplot = new BarPlot(v, QString("Distributuion Plot for %1").arg(vname1), vname1, "Number of Objects", selobjlabels);

    for(int i = 0; i < v.size(); i++){
      DelDVector(&v[i]);
    }
    return bplot;
  }
}

VariablePlot::VariablePlot(PROJECTS* projects_)
{
  projects = projects_;
  type = pid = layerid = varid1 = varid2 = -1;
}
