#include "ScatterPlot3D.h"
#include <scientific.h>
#include <QStringListModel>
#include <QColorDialog>
#include <QFileDialog>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>

#include "LabelDialog.h"
#include "SelectByDialog.h"
#include "SelectionStyleColor.h"
#include "VariableSelectorDialog.h"
#include "DoClusterAnalysisDialog.h"
#include "run.h"
#include "ValidationClusterPlot.h"

#include <QList>
#include <QMessageBox>
#include <QDebug>

// Define interaction style 
vtkStandardNewMacro(MouseInteractorStylePP);

// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

void ScatterPlot3D::StartRun()
{
  abort = false;
  ui.abortButton->setEnabled(true);
  ui.progressBar->setMinimum(0);
  ui.progressBar->setMaximum(0);
  ui.progressBar->setValue(0);
  ui.progressBar->show();
  ui.abortButton->show();
}

void ScatterPlot3D::StopRun()
{
  abort = true;
  ui.abortButton->setEnabled(false);
  ui.progressBar->setMinimum(0);
  ui.progressBar->setMaximum(100);
  ui.progressBar->setValue(20);
  ui.progressBar->hide();
  ui.abortButton->hide();
}

int ScatterPlot3D::GetIDinarlst(QString arhash)
{
  if(arlst != 0 && (*arlst).size() > 0 && !arhash.isEmpty()){
    for(int i = 0; i < (*arlst).size(); i++){
      if((*arlst)[i]->getHash().compare(arhash) == 0){
        return i;
      }
      else{
        continue;
      }
    }
    return -1;
  }
  else{
    return -1;
  }
}

int ScatterPlot3D::GetIDinmxlst(QString mxhash)
{
  if(mxlst != 0 && (*mxlst).size() > 0 && !mxhash.isEmpty()){
    for(int i = 0; i < (*mxlst).size(); i++){
      if((*mxlst)[i]->getHash().compare(mxhash) == 0){
        return i;
      }
      else{
        continue;
      }
    }
    return -1;
  }
  else{
    return -1;
  }
}

QColor ScatterPlot3D::makeColor(double val, double min, double max, QColor &color1, QColor &color2)
{
/*
colorA = [0, 0, 255] # blue
colorB = [255, 0, 0] # red
function get_gradient_color(val):
# 'val' must be between 0 and 1
for i in [1,2,3]:
color[i] = colorA[i] + val * (colorB[i] - colorA[i])
return color1
*/
  double normval = (val - min) / (max -min);
  QColor ncolor;
  ncolor.setRed(color1.red() + (normval * (color2.red() - color1.red())));
  ncolor.setGreen(color1.green() + (normval * (color2.green() - color1.green())));
  ncolor.setBlue(color1.blue() + (normval * (color2.blue() - color1.blue())));
  return ncolor;
}

void ScatterPlot3D::UpdateSelectionLabel()
{
  QString selectedobjname;
  vtkIdType picked = pointPicker->GetPointId();
  int selectedobject = -1;
  if(picked >= 0){
    /*qDebug() << QString("Point Picked: %1       x: %2  y: %3  z: %4").arg(picked).arg(QString::number(style->getPicked()[0])).arg(QString::number(style->getPicked()[1])).arg(QString::number(style->getPicked()[2]));*/
    double rsquared = square(p.sphereSource()->GetRadius());
    
    for(int i = 0; i < p.NumberOfPoints(); i++){
      double a = square(p.points()->GetPoint(i)[0] - style->getPicked()[0]) + square(p.points()->GetPoint(i)[1] - style->getPicked()[1]) + square(p.points()->GetPoint(i)[2] - style->getPicked()[2]);
      double epsilon = 1e-6;
      if(FLOAT_EQ(a, rsquared, epsilon) || FLOAT_EQ(ceil(a), rsquared, epsilon) || FLOAT_EQ(floor(a), rsquared, epsilon)){
        selectedobjname = p.labels()[i];
        selectedobject = i;
        break;
      }
      else{
        continue;
      }
    }
  }
  
  ui.listView->selectionModel()->clear();
  if(selectedobject > -1 && selectedobject < ui.listView->model()->rowCount()){
    ui.listView->selectionModel()->select(ui.listView->model()->index(selectedobject, 0), QItemSelectionModel::Select);
    ui.listView->scrollTo(ui.listView->model()->index(selectedobject, 0));
  }
  
  /*
  if(selectedobjname.size() > 0){
    for(int i = 0; i < ui.listView->model()->rowCount(); i++){
      if(ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString().compare(selectedobjname) == 0){
        ui.listView->selectionModel()->select(ui.listView->model()->index(i, 0), QItemSelectionModel::Select);
        ui.listView->scrollTo(ui.listView->model()->index(i, 0));
        break;
      }
      else{
        continue;
      }
    }
  }*/
  
  ui.SelectedObjectLine->setText(selectedobjname);
  
  spimgsign.pid = pid;
  spimgsign.imgname.append(selectedobjname);
  emit ScatterPlot3DImageSignalChanged(spimgsign);
  
  EnableDisableButton();
}

void ScatterPlot3D::AbortRun()
{
  abort = true;
  ui.abortButton->setEnabled(false);
}

void ScatterPlot3D::EnableDisableButton()
{
  if(ui.listView->selectionModel()->selectedIndexes().size() > 0 && ui.listView->selectionModel()->selectedIndexes().size() < p.NumberOfPoints()){
    ui.actionSymbolSize->setEnabled(false);
  }
  else{
    if(ui.actionSymbolSize->isEnabled() == false)
      ui.actionSymbolSize->setEnabled(true);
  }
  UpdateSelectedObjects();
}

void ScatterPlot3D::SavePlotImage()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot to Image"), "", tr("JPEG (*.jpg);;PNG (*.png);;All Files (*)"));
  
  if(!fileName.isEmpty()){
    vtkWindowToImageFilter *filter = vtkWindowToImageFilter::New();
    filter->SetInput(ui.qvtkWidget->GetRenderWindow());
    if(fileName.split(".").last().compare("png", Qt::CaseInsensitive) == 0){
      vtkPNGWriter *jw = vtkPNGWriter::New();
      jw->SetInputData(filter->GetOutput());
      jw->SetFileName(fileName.toUtf8());
      jw->Write();
      jw->Delete();
    }
    else{
      vtkJPEGWriter *jw = vtkJPEGWriter::New();
      jw->SetInputData(filter->GetOutput());
      jw->SetFileName(fileName.toUtf8());
      jw->Write();
      jw->Delete();
    }
    filter->Delete();
  }
}

void ScatterPlot3D::setSelectionObjLabel()
{

  QModelIndexList indexes = ui.listView->selectionModel()->selectedRows();
  if(indexes.size() < 1)
    return;
  else{
    QStringList selectedobj;
    QModelIndex current;
    Q_FOREACH(current, indexes){
      selectedobj.append(ui.listView->model()->index(current.row(), 0).data(Qt::DisplayRole).toString());
    }
    
    LabelDialog ldialog(objlabels, selectedobj, LabelDialog::OBJLABELS);
    ldialog.exec();
  }
}

void ScatterPlot3D::setSelectionVarLabel()
{

  QModelIndexList indexes = ui.listView->selectionModel()->selectedRows();
  if(indexes.size() < 1)
    return;
  else{
    QStringList selectedobj;
    QModelIndex current;
    Q_FOREACH(current, indexes){
      selectedobj.append(ui.listView->model()->index(current.row(), 0).data(Qt::DisplayRole).toString());
    }
    
    LabelDialog ldialog(varlabels, selectedobj, LabelDialog::VARLABELS);
    ldialog.exec();
  }
}

void ScatterPlot3D::setSelectionStyle()
{
  QList< int > vtkselectedID;
  
  for(int i = 0; i < ui.listView->model()->rowCount(); i++){
    if(ui.listView->selectionModel()->isSelected(ui.listView->model()->index(i, 0)) == true){
      vtkselectedID.append(i);
    }
    else{
      continue;
    }
  }

//   qDebug() << vtkselectedID;
  
  if(typedata == -1){
    SelectionStyleColor obj;
    if(obj.exec() == QDialog::Accepted){
      QColor color = obj.getSolidSymbolColor();
      unsigned char c[3];
      c[0] = (unsigned char)color.red();
      c[1] = (unsigned char)color.green();
      c[2] = (unsigned char)color.blue();
      for(int i = 0; i < vtkselectedID.size(); i++){
        if(vtkselectedID[i] < p.NumberOfPoints()){
          p.colors()->SetTupleValue(vtkselectedID[i], c);
        }
        else{
          // Error!! Object number out of range
          continue; 
        }
      }
    }
  }
  else if(typedata == MATRIXDATA){
    if(type == LOADINGS){
      SelectionStyleColor obj;
      if(obj.exec() == QDialog::Accepted){
        QColor color = obj.getSolidSymbolColor();
        unsigned char c[3];
        c[0] = (unsigned char)color.red();
        c[1] = (unsigned char)color.green();
        c[2] = (unsigned char)color.blue();
        for(int i = 0; i < vtkselectedID.size(); i++){
          if(vtkselectedID[i] < p.NumberOfPoints()){
            p.colors()->SetTupleValue(vtkselectedID[i], c);
          }
          else{
            // Error!! Object number out of range
            continue; 
          }
        }
      }
    }
    else{
      QList< int > xid, yid;
      
      for(int i = 0; i < xhash.size(); i++){
        int xid_ = GetIDinmxlst(xhash[i]);
        if(xid_ != -1){
          xid.append(xid_);
        }
        else{
          continue;
        }
      }
      
      for(int i = 0; i < yhash.size(); i++){
        int yid_ = GetIDinmxlst(yhash[i]);
        if(yid_ != -1){
          yid.append(yid_);
        }
        else{
          continue;
        }
      }
      
      QStringList xvarname, yvarname;
      if(xid.size() > 0){
        xvarname.append((*mxlst)[xid[0]]->getVarName()); // we assume that all x variable are the same. So we get the first variable name
        xvarname.removeAll("Labels");
      }
      
      if(yid.size() > 0){
        yvarname.append((*mxlst)[yid[0]]->getVarName()); // we assume that all y variable are the same. So we get the first variable name
        yvarname.removeAll("Labels");
      }
      
      SelectionStyleColor obj(xvarname, yvarname);
      if(obj.exec() == QDialog::Accepted){
        int changecolor = obj.ChangeColorType();
        
        if(changecolor == 0){ // solid
          QColor color = obj.getSolidSymbolColor();
          unsigned char c[3];
          c[0] = (unsigned char)color.red();
          c[1] = (unsigned char)color.green();
          c[2] = (unsigned char)color.blue();
          
          for(int i = 0; i < vtkselectedID.size(); i++){
            if(vtkselectedID[i] < p.NumberOfPoints()){
              p.colors()->SetTupleValue(vtkselectedID[i], c);
            }
            else{
              // Error!! Object number out of range
              continue;
            }
          }
        }
        else if(changecolor == 1){ // from min to max
          int col = obj.getVariableSymbolColor();
          int vartype = obj.getVariableTypeSymbolColor(); // 0 x; 1 y
          unsigned char c[3];
          
          if((*mxlst).size() > 0 && xid.size() > 0 && vartype == 0){
            double min, max;
            QStringList objnameselected;
            matrix *m;
            NewMatrix(&m, vtkselectedID.size(), (*mxlst)[xid[0]]->Matrix()->col+2); //+2 id sel obj, and value
            
            if(col > (int)m->col-2){
              return;
            }
            
            for(int i = 0; i < vtkselectedID.size(); i++){
              objnameselected.append(p.labels()[p.pointid()[vtkselectedID[i]].id]);
            }
            
            uint l = 0;
            for(int k = 0; k < xid.size(); k++){
              for(uint i = 0; i < (*mxlst)[xid[k]]->Matrix()->row; i++){
                int id_ = objnameselected.indexOf((*mxlst)[xid[k]]->getObjName()[i]);
                if(id_ > -1){
                  for(uint j = 0; j < (*mxlst)[xid[k]]->Matrix()->col; j++){
                    setMatrixValue(m, l, j, getMatrixValue((*mxlst)[xid[k]]->Matrix(), i, j));
                  }
                  setMatrixValue(m, l, m->col-2, vtkselectedID[id_]);
                  setMatrixValue(m, l, m->col-1, getMatrixValue((*mxlst)[xid[k]]->Matrix(), i, col));
                  l++;
                }
                else{
                  continue;
                }
              }
            }
            
            MatrixColumnMinMax(m, col, &min, &max);
            
            for(uint i = 0; i < m->row; i++){
                double val = getMatrixValue(m, i, m->col-1);
                QColor color = makeColor(val, min, max, obj.getMinSymbolColor(), obj.getMaxSymbolColor());
                c[0] = (unsigned char)color.red();
                c[1] = (unsigned char)color.green();
                c[2] = (unsigned char)color.blue();
                p.colors()->SetTupleValue(vtkselectedID[i], c);
            }
            DelMatrix(&m);
          }
          else if((*mxlst).size() > 0 && yid.size() > 0 && vartype == 1){
            double min, max;
            QStringList objnameselected;
            matrix *m;
            NewMatrix(&m, vtkselectedID.size(), (*mxlst)[xid[0]]->Matrix()->col+2); //+2 id sel obj, and value
            
            if(col > (int)m->col-2){
              return;
            }
            
            for(int i = 0; i < vtkselectedID.size(); i++){
              objnameselected.append(p.labels()[p.pointid()[vtkselectedID[i]].id]);
            }
            
            uint l = 0;
            for(int k = 0; k < xid.size(); k++){
              for(uint i = 0; i < (*mxlst)[xid[k]]->Matrix()->row; i++){
                int id_ = objnameselected.indexOf((*mxlst)[xid[k]]->getObjName()[i]);
                if(id_ > -1){
                  for(uint j = 0; j < (*mxlst)[xid[k]]->Matrix()->col; j++){
                    setMatrixValue(m, l, j, getMatrixValue((*mxlst)[xid[k]]->Matrix(), i, j));
                  }
                  setMatrixValue(m, l, m->col-2, vtkselectedID[id_]);
                  setMatrixValue(m, l, m->col-1, getMatrixValue((*mxlst)[xid[k]]->Matrix(), i, col));
                  l++;
                }
                else{
                  continue;
                }
              }
            }
            
            MatrixColumnMinMax(m, col, &min, &max);
            
            for(uint i = 0; i < m->row; i++){
                double val = getMatrixValue(m, i, m->col-1);
                QColor color = makeColor(val, min, max, obj.getMinSymbolColor(), obj.getMaxSymbolColor());
                c[0] = (unsigned char)color.red();
                c[1] = (unsigned char)color.green();
                c[2] = (unsigned char)color.blue();
                p.colors()->SetTupleValue(vtkselectedID[i], c);
            }
            DelMatrix(&m);
          }
        }
      }
    }
  }
//   else if(typedata == ARRAYDATA){
  else{ // WARNING  TESTA QUESTO CON GLI ARRAY
    if(type == LOADINGS){
      SelectionStyleColor obj;
      if(obj.exec() == QDialog::Accepted){
        QColor color = obj.getSolidSymbolColor();
        unsigned char c[3];
        c[0] = (unsigned char)color.red();
        c[1] = (unsigned char)color.green();
        c[2] = (unsigned char)color.blue();
        for(int i = 0; i < vtkselectedID.size(); i++){
          if(vtkselectedID[i] < p.NumberOfPoints()){
            p.colors()->SetTupleValue(vtkselectedID[i], c);
          }
          else{
            // Error!! Object number out of range
            continue; 
          }
        }
      }
    }
    else{
      QList< int > xid, yid;
      int xlevels = 0, ylevels = 0;
      QStringList xvarname, yvarname;
      
      for(int i = 0; i < xhash.size(); i++){
        int xid_ = GetIDinarlst(xhash[i]);
        if(xid_ != -1){
          xid.append(xid_);
        }
        else{
          continue;
        }
      }
      
      for(int i = 0; i < yhash.size(); i++){
        int yid_ = GetIDinarlst(yhash[i]);
        if(yid_ != -1){
          yid.append(yid_);
        }
        else{
          continue;
        }
      }
      
      if(xid.size() > 0){
        xvarname.append((*arlst)[xid[0]]->getVarName()); // we assume that all x variable are the same. So we get the first variable name
        xvarname.removeAll("Labels");
        xlevels = (*arlst)[xid[0]]->Array()->order;
      }
      
      if(yid.size() > 0){
        yvarname.append((*arlst)[yid[0]]->getVarName()); // we assume that all y variable are the same. So we get the first variable name
        yvarname.removeAll("Labels");
        ylevels = (*arlst)[yid[0]]->Array()->order;
      }
      
      SelectionStyleColor obj(xvarname, yvarname, xlevels, ylevels);
      if(obj.exec() == QDialog::Accepted){
        int changecolor = obj.ChangeColorType();
        if(changecolor == 0){ // solid color
          QColor color = obj.getSolidSymbolColor();
          unsigned char c[3];
          c[0] = (unsigned char)color.red();
          c[1] = (unsigned char)color.green();
          c[2] = (unsigned char)color.blue();
          for(int i = 0; i < vtkselectedID.size(); i++){
            if(vtkselectedID[i] < p.NumberOfPoints()){
              p.colors()->SetTupleValue(vtkselectedID[i], c);
            }
            else{
              // Error!! Object number out of range
              continue;
            }
          }
        }
        else if(changecolor == 1){
          int col = obj.getVariableSymbolColor();
          int vartype = obj.getVariableTypeSymbolColor(); // 0 x; 1 y
          int level = obj.getVariableLevelSymbolColor();
          unsigned char c[3];
          
          if((*arlst).size() > 0 && xid.size() > 0 && vartype == 0){
            double min, max;
            if(col < (int)(*arlst)[xid[0]]->Array()->m[level]->col){
              MatrixColumnMinMax((*arlst)[xid[0]]->Array()->m[level], col, &min, &max);
            }
            
            for(int i = 1; i < xid.size(); i++){
              if(col < (int)(*arlst)[xid[i]]->Array()->m[level]->col){
                double t_min, t_max;
                MatrixColumnMinMax((*arlst)[xid[i]]->Array()->m[level], col, &t_min, &t_max);
                if(t_min < min){
                  min = t_min;
                }
                
                if(t_max > max){
                  max = t_max;
                }
              }
            }
            
            
            for(int i = 0; i < vtkselectedID.size(); i++){
              if(vtkselectedID[i] < p.NumberOfPoints()){
                int mxid = -1;
                if(p.pointid()[vtkselectedID[i]].xhash.compare("-") != 0){
                    for(int j = 0; j < (*arlst).size(); j++){
                    if((*arlst)[j]->getHash().compare(p.pointid()[vtkselectedID[i]].xhash) == 0){
                      mxid = j;
                      break;
                    }
                  }
                }
                else{
                  continue;
                }
                
                if(mxid != -1){
                  double val = getArrayValue((*arlst)[mxid]->Array(), level, p.pointid()[vtkselectedID[i]].id, col);
                  QColor color = makeColor(val, min, max, obj.getMinSymbolColor(), obj.getMaxSymbolColor());
                  c[0] = (unsigned char)color.red();
                  c[1] = (unsigned char)color.green();
                  c[2] = (unsigned char)color.blue();
                  p.colors()->SetTupleValue(vtkselectedID[i], c);
                }
              }
              else{
                // Error!! Object number out of range
                continue; 
              }
            }
          }
          else if((*arlst).size() > 0 && yid.size() > 0 && vartype == 1){
            double min, max;
            if(col < (int)(*arlst)[yid[0]]->Array()->m[level]->col){
              MatrixColumnMinMax((*arlst)[yid[0]]->Array()->m[level], col, &min, &max);
            }
            
            for(int i = 1; i < yid.size(); i++){
              if(col < (int)(*arlst)[yid[i]]->Array()->m[level]->col){
                double t_min, t_max;
                MatrixColumnMinMax((*arlst)[yid[i]]->Array()->m[level], col, &t_min, &t_max);
                if(t_min < min){
                  min = t_min;
                }
                
                if(t_max > max){
                  max = t_max;
                }
              }
            }
            
            for(int i = 0; i < vtkselectedID.size(); i++){
              if(vtkselectedID[i] < p.NumberOfPoints()){
                int myid = -1;
                if(p.pointid()[vtkselectedID[i]].yhash.compare("-") != 0){
                    for(int j = 0; j < (*arlst).size(); j++){
                    if((*arlst)[j]->getHash().compare(p.pointid()[vtkselectedID[i]].yhash) == 0){
                      myid = j;
                      break;
                    }
                  }
                }
                else{
                  continue;
                }
                
                if(myid != -1){
                  double val = getArrayValue((*arlst)[myid]->Array(), level, p.pointid()[vtkselectedID[i]].id, col);
                  QColor color = makeColor(val, min, max, obj.getMinSymbolColor(), obj.getMaxSymbolColor());
                  c[0] = (unsigned char)color.red();
                  c[1] = (unsigned char)color.green();
                  c[2] = (unsigned char)color.blue();
                  p.colors()->SetTupleValue(vtkselectedID[i], c);
                }
              }
              else{
                // Error!! Object number out of range
                continue;
              }
            }
          }
        }
      }
    }
  }
  PlotUpdate();
}

void ScatterPlot3D::selectByVarLabels()
{
  
  if(typedata == MATRIXDATA){
    QList< int > xid;
    for(int i = 0; i < xhash.size(); i++){
      xid.append(GetIDinmxlst(xhash[i]));
    }
    
    VariableSelectorDialog vseldialog(objnames, (*mxlst)[xid[0]]->getVarName(), (*mxlst), xid, varlabels);
        
    if(vseldialog.exec() == QDialog::Accepted){
      QStringList varselected = vseldialog.getSelectedVariables();
      for(int i = 0; i < ui.listView->model()->rowCount(); i++){
        if(varselected.indexOf(ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString()) > -1){
          ui.listView->selectionModel()->select(ui.listView->model()->index(i, 0), QItemSelectionModel::Select);
        }
        else{
          continue;
        }
      }
    }
    else{
      return;
    }
  }
  else if(typedata == ARRAYDATA){
    VariableSelectorDialog vseldialog((*arlst)[GetIDinarlst(xhash[0])]->getVarName(), varlabels);
    
    if(vseldialog.exec() == QDialog::Accepted){
      QStringList varselected = vseldialog.getSelectedVariables();
      for(int i = 0; i < ui.listView->model()->rowCount(); i++){
        if(varselected.indexOf(ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString()) > -1){
          ui.listView->selectionModel()->select(ui.listView->model()->index(i, 0), QItemSelectionModel::Select);
        }
        else{
          continue;
        }
      }
    }
    else{
      return;
    }
  }
  
  EnableDisableButton();
}


void ScatterPlot3D::selectByObjLabels()
{

}

void ScatterPlot3D::DoClusterAnalysis()
{ 
  DoClusterAnalysisDialog docluster(mxlst, arlst, DoClusterAnalysisDialog::PlotSelection);
  if(docluster.exec() == QDialog::Accepted){
 
    uivector *clusterids;
    initUIVector(&clusterids);
    
    if(docluster.getDataType() == 0){
      matrix *coordinates;
      uint row = 0;
      
      for(int i = 0; i < m.size(); i++){
        row += m[i]->row;
      }
  
      NewMatrix(&coordinates, row, 3);
      
      row = 0;
      for(int i = 0; i < m.size(); i++){
        for(uint j = 0; j < m[i]->row; j++){
          setMatrixValue(coordinates, row, 0, getMatrixValue(m[i], j, (ui.valXaxis->value() - 1)));
          setMatrixValue(coordinates, row, 1, getMatrixValue(m[i], j, (ui.valYaxis->value() - 1)));
          setMatrixValue(coordinates, row, 2, getMatrixValue(m[i], j, (ui.valZaxis->value() - 1)));
          row++;
        }
      }
      
      
      int ncluster = 2;
      if(docluster.ValidateCluster() == true){
        RUN obj2;
        
        dvector *toplot;
        initDVector(&toplot);
        
        obj2.setMatrix(coordinates);
        obj2.setDVector(toplot);
        obj2.setValidationType(docluster.getVaidationType());
        obj2.setNumberOfGroups(docluster.getNGroups());
        obj2.setNumberOfIterations(docluster.getNIterations());
        obj2.setNMaxClusters(docluster.getMaxClustersNumber());

        StartRun();
        QFuture<void> future = obj2.RunClusterValidation();
        
        while(!future.isFinished()){
          if(abort == true){
            obj2.AbortRun();
            QApplication::processEvents();
          }
          else{
            QApplication::processEvents();
          }
        }
        
        StopRun();
        
        if(toplot->size > 0){
          
          QString yaxestitle;
          if(docluster.getVaidationType() == JUMPMETHOD){
            yaxestitle = "Jumps";
          }
          else{
            yaxestitle = "K-Means Distance Minimization";
          }
          
          ValidationClusterPlot vclusterplot(toplot, yaxestitle);
          if(vclusterplot.exec() == QDialog::Accepted){
            qDebug() << "NCLUSTERS: " <<  vclusterplot.getNClusters();
            ncluster = vclusterplot.getNClusters();
            DelDVector(&toplot);
          }
          else{
            DelDVector(&toplot);
            return;
          }
        }
        else{
          DelDVector(&toplot);
          return;
        }
      }
      else{
        ncluster = docluster.getNumberOfClusters();
      }
      
      RUN obj;
      
      obj.setMatrix(coordinates);
      obj.setUIVector(clusterids);
      obj.setClusteringAlgorithm(docluster.getAlgorithmType());
      obj.setNumberOfCluster(ncluster);
      obj.setObjectSelectionType(docluster.getExtractObjects());
      obj.setNMaxObjects(docluster.getNMaxObjects());
      
      StartRun();
      QFuture<void> future = obj.RunClustering();
      
      while(!future.isFinished()){
        if(abort == true){
          obj.AbortRun();
          QApplication::processEvents();
        }
        else{
          QApplication::processEvents();
        }
      }
      
      DelMatrix(&coordinates);
      
      QList<int> QtColours;
      QtColours.append(7);  // 1 Qt::red 7       Red (#ff0000)
      QtColours.append(9);  // 2 Qt::blue        9       Blue (#0000ff)
      QtColours.append(8);  // 3 Qt::green       8       Green (#00ff00)
      QtColours.append(10); // 4 Qt::cyan        10      Cyan (#00ffff)
      QtColours.append(11); // 5 Qt::magenta     11      Magenta (#ff00ff)
      QtColours.append(12); // 6 Qt::yellow      12      Yellow (#ffff00)
      QtColours.append(5);  // 7 Qt::gray        5       Gray (#a0a0a4)
      QtColours.append(16); // 8 Qt::darkCyan    16      Dark cyan (#008080)
      QtColours.append(17); // 9 Qt::darkMagenta 17      Dark magenta (#800080)
      QtColours.append(18); // 10 Qt:darkYellow  18      Dark yellow (#808000)
      QtColours.append(4);  // 11 Qt::darkGray    4       Dark gray (#808080)
      QtColours.append(6);  // 12 Qt::lightGray   6       Light gray (#c0c0c0)
      QtColours.append(13); // 13 Qt:darkRed     13      Dark red (#800000)
      QtColours.append(14); // 14 Qt::darkGreen   14      Dark green (#008000)
      QtColours.append(15); // 15 Qt::darkBlue    15      Dark blue (#000080)
      
      QList<QColor> colors;
      if(QtColours.size() < ncluster){
        for(int i = QtColours.size(); i < ncluster; i++){
          QColor _color_;
          _color_.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
          colors.append(_color_);
        }
      }
      
      
      for(int i = 0; i < p.NumberOfPoints(); i++){
        int id = getUIVectorValue(clusterids, i)-1;
        if(id > -1){
          unsigned char c[3];
          QColor color;
          if(id < QtColours.size()){
            color = QColor((Qt::GlobalColor)QtColours[id]);
          }
          else{
            color = colors[id-QtColours.size()];
          }
          
          c[0] = (unsigned char)color.red();
          c[1] = (unsigned char)color.green();
          c[2] = (unsigned char)color.blue();
          p.colors()->SetTupleValue(i, c);
          
          
        }
        else{
          continue;
        }
      }
      
      if(docluster.SaveClusterLabels() == true){
        // Set Labels
        LABELS clusterobjlabels;
        for(int i = 0; i < ncluster; i++){
          clusterobjlabels.append(LABEL());
          clusterobjlabels.last().name = QString("%1_%2").arg(docluster.getClusterLabelSufix()).arg(QString::number(i+1));
        }
        for(int i = 0; i < p.NumberOfPoints(); i++){
          int id = getUIVectorValue(clusterids, i)-1;
          if(id > -1){
            clusterobjlabels[id].objects.append(objnames[i]);  
          }
          else{
            continue;
          }
        }  
        objlabels->append(clusterobjlabels);
      }
      
      StopRun();
    }
    else{
      matrix *m;
      int dataid = GetIDinmxlst(docluster.getDataHash());
      bool ismatrix = false;
      
      if(dataid == -1){
        dataid = GetIDinarlst(docluster.getDataHash());
        if(dataid != -1){
          m = (*arlst)[dataid]->Array()->m[docluster.getLayerID()];
        }
        else{
          QMessageBox::critical(this, tr("Cluster Analysis Error"), tr("Unable to make run Cluster Analysis\nSelected Data not found or lost!"), QMessageBox::Ok);
          return;
        }
      }
      else{
        m = (*mxlst)[dataid]->Matrix();
        ismatrix = true;
      }
      
      
      int ncluster = 2;
      if(docluster.ValidateCluster() == true){
        RUN obj2;
        
        dvector *toplot;
        initDVector(&toplot);
        
        obj2.setMatrix(m);
        obj2.setDVector(toplot);
        obj2.setValidationType(docluster.getVaidationType());
        obj2.setNumberOfGroups(docluster.getNGroups());
        obj2.setNumberOfIterations(docluster.getNIterations());
        obj2.setNMaxClusters(docluster.getMaxClustersNumber());

        StartRun();
        QFuture<void> future = obj2.RunClusterValidation();
        
        while(!future.isFinished()){
          if(abort == true){
            obj2.AbortRun();
            QApplication::processEvents();
          }
          else{
            QApplication::processEvents();
          }
        }
        
        StopRun();
        
        if(toplot->size > 0){
          
          QString yaxestitle;
          if(docluster.getVaidationType() == JUMPMETHOD){
            yaxestitle = "Jumps";
          }
          else{
            yaxestitle = "K-Means Distance Minimization";
          }
          
          ValidationClusterPlot vclusterplot(toplot, yaxestitle);
          if(vclusterplot.exec() == QDialog::Accepted){
            ncluster = vclusterplot.getNClusters();
            DelDVector(&toplot);
          }
          else{
            DelDVector(&toplot);
            return;
          }
        }
        else{
          DelDVector(&toplot);
          return;
        }
      }
      else{
        ncluster = docluster.getNumberOfClusters();
      }   
      
      
      RUN obj;
      obj.setMatrix(m);
      obj.setUIVector(clusterids);
      obj.setClusteringAlgorithm(docluster.getAlgorithmType());
      obj.setNumberOfCluster(ncluster);
      obj.setObjectSelectionType(docluster.getExtractObjects());
      obj.setNMaxObjects(docluster.getNMaxObjects());
      
      StartRun();
      QFuture<void> future = obj.RunClustering();
      
      while(!future.isFinished()){
        if(abort == true){
          obj.AbortRun();
          QApplication::processEvents();
        }
        else{
          QApplication::processEvents();
        }
      }
         
      QList<int> QtColours;
      QtColours.append(7);  // 1 Qt::red 7       Red (#ff0000)
      QtColours.append(9);  // 2 Qt::blue        9       Blue (#0000ff)
      QtColours.append(8);  // 3 Qt::green       8       Green (#00ff00)
      QtColours.append(10); // 4 Qt::cyan        10      Cyan (#00ffff)
      QtColours.append(11); // 5 Qt::magenta     11      Magenta (#ff00ff)
      QtColours.append(12); // 6 Qt::yellow      12      Yellow (#ffff00)
      QtColours.append(5);  // 7 Qt::gray        5       Gray (#a0a0a4)
      QtColours.append(16); // 8 Qt::darkCyan    16      Dark cyan (#008080)
      QtColours.append(17); // 9 Qt::darkMagenta 17      Dark magenta (#800080)
      QtColours.append(18); // 10 Qt:darkYellow  18      Dark yellow (#808000)
      QtColours.append(4);  // 11 Qt::darkGray    4       Dark gray (#808080)
      QtColours.append(6);  // 12 Qt::lightGray   6       Light gray (#c0c0c0)
      QtColours.append(13); // 13 Qt:darkRed     13      Dark red (#800000)
      QtColours.append(14); // 14 Qt::darkGreen   14      Dark green (#008000)
      QtColours.append(15); // 15 Qt::darkBlue    15      Dark blue (#000080)
      
      QList<QColor> colors;
      if(QtColours.size() < ncluster){
        for(int i = QtColours.size(); i < ncluster; i++){
          QColor _color_;
          _color_.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
          colors.append(_color_);
        }
      }
      
      // Set Labels
      LABELS clusterobjlabels;
      if(docluster.SaveClusterLabels() == true){
        for(int i = 0; i < ncluster; i++){
          clusterobjlabels.append(LABEL());
          clusterobjlabels.last().name = QString("%1 - %2").arg(docluster.getClusterLabelSufix()).arg(QString::number(i+1));
        }
      }
      
      if(ismatrix == true){
        for(uint i = 0; i < clusterids->size; i++){
          int objid = -1;
          for(int j = 0; j < p.NumberOfPoints(); j++){
            if((*mxlst)[dataid]->getObjName()[i].compare(objnames[j]) == 0){
              objid = j;
              break;
            }
            else{
              continue;
            }
          }
          
          if(objid > -1){
            int id = getUIVectorValue(clusterids, i)-1;
            if(id > -1){
              unsigned char c[3];
              QColor color;
              if(id < QtColours.size()){
                color = QColor((Qt::GlobalColor)QtColours[id]);
              }
              else{
                color = colors[id-QtColours.size()];
              }
              
              c[0] = (unsigned char)color.red();
              c[1] = (unsigned char)color.green();
              c[2] = (unsigned char)color.blue();
              
              p.colors()->SetTupleValue(objid, c);
              
              if(docluster.SaveClusterLabels() == true){
                clusterobjlabels[id].objects.append(objnames[objid]); 
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
      
      if(docluster.SaveClusterLabels() == true){
        objlabels->append(clusterobjlabels);
      }
      
      StopRun();
    }
    DelUIVector(&clusterids);
  }
  PlotUpdate();
}

void ScatterPlot3D::UpdateSelectedObjects()
{
  ui.objectSelected->setText(QString("Selected: %1/%2").arg(QString::number(ui.listView->selectionModel()->selectedIndexes().size())).arg(QString::number(ui.listView->model()->rowCount())));
}

// Select points by column
void ScatterPlot3D::selectBy()
{
  matrix *coordinates;
  uint row = 0;
  
  for(int i = 0; i < m.size(); i++){
    row += m[i]->row;
  }
  
  NewMatrix(&coordinates, row, 3);
  
  row = 0;
  for(int i = 0; i < m.size(); i++){
    for(uint j = 0; j < m[i]->row; j++){
      setMatrixValue(coordinates, row, 0, getMatrixValue(m[i], j, (ui.valXaxis->value() - 1)));
      setMatrixValue(coordinates, row, 1, getMatrixValue(m[i], j, (ui.valYaxis->value() - 1)));
      setMatrixValue(coordinates, row, 2, getMatrixValue(m[i], j, (ui.valZaxis->value() - 1)));
      row++;
    }
  }
  
  QList< int > selectedobjid;
  
  QStringList labellst;
  for(int i = 0; i < ui.listView->model()->rowCount(); i++){
    labellst.append(ui.listView->model()->index(i, 0).data(Qt::DisplayRole).toString());
  }
  
  if(typedata == MATRIXDATA){  
    
    QList< int > mxid;
    
    for(int i = 0; i < xhash.size(); i++){
      int id_ = GetIDinmxlst(xhash[i]);
      if(id_ != -1){
        mxid.append(id_);
      }
      else{
        continue;
      }
    }

    
    SelectByDialog seldialog(labellst, coordinates, mxlst, mxid, objlabels, varlabels);
    if(seldialog.exec() == QDialog::Accepted){
      selectedobjid = seldialog.GetSelectedObjID();
    }
  }
  else if(typedata == ARRAYDATA){
    
  }
  
  
  QItemSelection selection;
  for (int i = 0; i < selectedobjid.size(); i++) {
    QModelIndex topLeft = ui.listView->model()->index(selectedobjid[i], 0);
    QModelIndex bottomRight = ui.listView->model()->index(selectedobjid[i], 0);
    selection << QItemSelectionRange(topLeft, bottomRight);
  }
  ui.listView->selectionModel()->select(selection, QItemSelectionModel::Select);
  
  /*
  for(int i = 0; i < selectedobject.size(); i++){
    int index = labellst.indexOf(selectedobject[i]);
    if(index != -1 && index < p.NumberOfPoints()){
      ui.listView->selectionModel()->select(ui.listView->model()->index(index, 0), QItemSelectionModel::Select);
    }
  }
  */
  PlotUpdate();
  DelMatrix(&coordinates);
  // Here open qwidget 
  EnableDisableButton();
}

// Invert Selected Points
void ScatterPlot3D::invertSelection()
{
  QItemSelectionModel *selectionModel =  ui.listView->selectionModel();
  QAbstractItemModel *model = ui.listView->model();

  QModelIndex topLeft;
  QModelIndex bottomRight;
  QItemSelection toggleSelection;
  topLeft = model->index(0, 0);
  bottomRight = model->index(model->rowCount()-1, 0);
  toggleSelection.select(topLeft, bottomRight);
  selectionModel->select(toggleSelection, QItemSelectionModel::Toggle);
  EnableDisableButton();
}

// Unselect pounts
void ScatterPlot3D::unselectAll()
{
  ui.listView->selectionModel()->clear();
  EnableDisableButton();
}

// Select all points
void ScatterPlot3D::selectAll()
{ 
  
  ui.listView->selectAll();
  /*
  for(int i = 0; i < ui.listView->model()->rowCount(); i++){
    ui.listView->selectionModel()->select(ui.listView->model()->index(i, 0), QItemSelectionModel::Select);
  }*/
  EnableDisableButton();
}

void ScatterPlot3D::ResetPlot()
{
  // Set the default point values
  ui.valXaxis->setValue(ui.valXaxis->minimum());
  
  if(ui.valYaxis->minimum()+1 < ui.valXaxis->maximum()){
    ui.valYaxis->setValue(ui.valYaxis->minimum()+1);
  }
  else{
    ui.valYaxis->setValue(ui.valYaxis->minimum());
  }

  if(ui.valZaxis->minimum()+2 < ui.valZaxis->maximum()){
    ui.valZaxis->setValue(ui.valXaxis->minimum()+2);
  }
  else{
    if(ui.valZaxis->minimum()+1 < ui.valZaxis->maximum()){
      ui.valZaxis->setValue(ui.valXaxis->minimum()+1);
    }
    else{
      ui.valZaxis->setValue(ui.valXaxis->minimum());
    }
  }
  setPoints();
  
  // Set the default symbol color
  QColor symbolcolor;
  int k = 0;
  QList<int> QtColours;
  QtColours.append(3);  // 0 Qt::white  3       White (#ffffff)
  QtColours.append(7);  // 1 Qt::red 7       Red (#ff0000)
  QtColours.append(9);  // 2 Qt::blue        9       Blue (#0000ff)
  QtColours.append(8);  // 3 Qt::green       8       Green (#00ff00)
  QtColours.append(10); // 4 Qt::cyan        10      Cyan (#00ffff)
  QtColours.append(11); // 5 Qt::magenta     11      Magenta (#ff00ff)
  QtColours.append(12); // 6 Qt::yellow      12      Yellow (#ffff00)
  QtColours.append(5);  // 7 Qt::gray        5       Gray (#a0a0a4)
  QtColours.append(16); // 8 Qt::darkCyan    16      Dark cyan (#008080)
  QtColours.append(17); // 9 Qt::darkMagenta 17      Dark magenta (#800080)
  QtColours.append(18); // 10 Qt:darkYellow  18      Dark yellow (#808000)
  QtColours.append(4);  // 11 Qt::darkGray    4       Dark gray (#808080)
  QtColours.append(6);  // 12 Qt::lightGray   6       Light gray (#c0c0c0)
  QtColours.append(13); // 13 Qt:darkRed     13      Dark red (#800000)
  QtColours.append(14); // 14 Qt::darkGreen   14      Dark green (#008000)
  QtColours.append(15); // 15 Qt::darkBlue    15      Dark blue (#000080)
    
  unsigned char c[3] = {255, 255, 255};
  for(int mxid = 0; mxid < m.size(); mxid++){
    if(mxid < QtColours.size()){
      symbolcolor = QColor((Qt::GlobalColor)QtColours[mxid]);
    }
    else{
      symbolcolor.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
    }
    
    c[0] = symbolcolor.red();
    c[1] = symbolcolor.green();
    c[2] = symbolcolor.blue();
    
    for(uint i = 0; i < m[mxid]->row; i++){
      p.colors()->SetTupleValue(k, c);
      k++;
    }
  }
  
  //Set the default symbol type and size
  ui.actionSymbolSize->setValue(6);
  SetSymbolSize();
  
  // Set default otline color
  outlinecolor = QColor(Qt::white);
  ui.outlineColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(outlinecolor.red()).arg(outlinecolor.green()).arg(outlinecolor.blue()));
  outlineActor->GetProperty()->SetColor(outlinecolor.redF(),outlinecolor.greenF(),outlinecolor.blueF());
  
  //set default background color
  backgroundcolor = QColor(Qt::black);
  ui.backgroundColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(backgroundcolor.red()).arg(backgroundcolor.green()).arg(backgroundcolor.blue()));
  renderer->SetBackground(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF());
  
  PlotUpdate();
}

void ScatterPlot3D::PlotUpdate()
{  
  setScalingFactor();
  p.Update();
  outline->Modified();
  outline->Update();
  outlineMapper->Modified();
  outlineMapper->Update();
  cubeAxesActor->SetBounds(outlineMapper->GetBounds());
  cubeAxesActor->Modified(); 
  renderer->Modified();
  
  ui.qvtkWidget->update();
  ui.qvtkWidget->repaint();
}

void ScatterPlot3D::setScalingFactor()
{
  double xmin, 
         xmax, 
         ymin, 
         ymax, 
         zmin, 
         zmax, 
         xext,
         yext,
         zext;
         
  xmin = xmax = getMatrixValue(m[0], 0, ui.valXaxis->value()-1);
  ymin = ymax = getMatrixValue(m[0], 0, ui.valYaxis->value()-1);
  zmin = zmax = getMatrixValue(m[0], 0, ui.valZaxis->value()-1);
  
  for(int k = 0; k < m.size(); k++){
    double tmpmin, tmpmax;
    MatrixColumnMinMax(m[k], ui.valXaxis->value()-1, &tmpmin, &tmpmax);
    
    if(k == 0){
      xmin = tmpmin;
      xmax = tmpmax;
    }
    else{
      if(tmpmin < xmin)
        xmin = tmpmin;
      
      if(tmpmax > xmax)
        xmax = tmpmax;
    }
    
    MatrixColumnMinMax(m[k], ui.valYaxis->value()-1, &tmpmin, &tmpmax);
    
    if(k == 0){
      ymin = tmpmin;
      ymax = tmpmax;
    }
    else{
      if(tmpmin < ymin)
        ymin = tmpmin;
      
      if(tmpmax > ymax)
        ymax = tmpmax;
    }
    
    MatrixColumnMinMax(m[k], ui.valZaxis->value()-1, &tmpmin, &tmpmax);
    
    if(k == 0){
      zmin = tmpmin;
      zmax = tmpmax;
    }
    else{
      if(tmpmin < zmin)
        zmin = tmpmin;
      
      if(tmpmax > zmax)
        zmax = tmpmax;
    }
  }
  
  xext = sqrt(square(xmax - xmin));
  yext = sqrt(square(ymax - ymin));
  zext = sqrt(square(zmax - zmin));
  
  if(xext < yext){
    if(xext < zext){
      scaling = xext / 500;
    }
    else{
      scaling = zext / 500;
    }
  }
  else{
    if(yext < zext){
      scaling = yext / 500;
    }
    else{
      scaling = zext / 500;
    }
  }
}

void ScatterPlot3D::SetOutlineColor()
{
  if(outlinecolor.isValid()){
    outlinecolor = QColorDialog::getColor(outlinecolor, this);
    if(outlinecolor.isValid()){
      ui.outlineColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(outlinecolor.red()).arg(outlinecolor.green()).arg(outlinecolor.blue()));
    }
  }
  else{ // Set the default color that is black
    outlinecolor = QColor(Qt::white);
    ui.outlineColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(outlinecolor.red()).arg(outlinecolor.green()).arg(outlinecolor.blue()));
  }
  
  outlineActor->GetProperty()->SetColor(outlinecolor.redF(),outlinecolor.greenF(),outlinecolor.blueF());
  PlotUpdate();
}

void ScatterPlot3D::SetBackgroundColor()
{
  if(backgroundcolor.isValid()){
    backgroundcolor = QColorDialog::getColor(backgroundcolor, this);
    if(backgroundcolor.isValid()){
      ui.backgroundColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(backgroundcolor.red()).arg(backgroundcolor.green()).arg(backgroundcolor.blue()));
    }
  }
  else{ // Set the default color that is black
    backgroundcolor = QColor(Qt::black);
    ui.backgroundColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(backgroundcolor.red()).arg(backgroundcolor.green()).arg(backgroundcolor.blue()));
  }
  
  renderer->SetBackground(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF());
  PlotUpdate();
}

void ScatterPlot3D::SetSymbolSize()
{
  p.sphereSource()->SetRadius(ui.actionSymbolSize->value() * scaling);
  PlotUpdate();
}

void ScatterPlot3D::setPoints()
{
  int k = 0;
  for(int i = 0; i < m.size(); i++){
    for(uint j = 0; j < m[i]->row; j++){
      p.points()->SetPoint(k, getMatrixValue(m[i], j, ui.valXaxis->value()-1), getMatrixValue(m[i], j, ui.valYaxis->value()-1), getMatrixValue(m[i], j, ui.valZaxis->value()-1));
      k++;
    }
  }
  cubeAxesActor->SetXTitle(QString("%1 %2").arg(xname).arg(ui.valXaxis->value()).toUtf8().constData());
  cubeAxesActor->SetYTitle(QString("%1 %2").arg(yname).arg(ui.valYaxis->value()).toUtf8().constData());
  cubeAxesActor->SetZTitle(QString("%1 %2").arg(zname).arg(ui.valZaxis->value()).toUtf8().constData());
  PlotUpdate();
}

void ScatterPlot3D::slotExit() 
{
  qApp->exit();
}

ScatterPlot3D::ScatterPlot3D(matrix* m_, QStringList& objname_, QString windowtitle, QString xname_, QString yname_, QString zname_)
{
  ui.setupUi(this);
  typedata = -1;
  pid = -1;
  ui.buttonSetSelectionLabel->hide();
  ui.buttonSelectBy->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();
  
  xname = xname_;
  yname = yname_;
  zname = zname_;
  
  ui.valXaxis_name->setText(xname);
  ui.valYaxis_name->setText(yname);
  ui.valZaxis_name->setText(zname);
  
  mxlst = 0;
  arlst = 0;
  
  pointPicker = vtkSmartPointer<vtkPointPicker>::New();
  style = vtkSmartPointer<MouseInteractorStylePP>::New();
  outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outlineActor = vtkSmartPointer<vtkActor>::New();
  cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
  renderer = vtkSmartPointer<vtkRenderer>::New(); 
  
  setWindowTitle(windowtitle);
  
  objnames.append(objname_);
  
  ui.listView->setModel(new QStringListModel(objnames));
  
  // deep copy
  matrix *_m_;
  m.append(_m_);
  initMatrix(&m.last());
  MatrixCopy(m_, &m.last());
  
  //chek for the max of col that could be plot:
  uint maxcol = m[0]->col;
  uint npoints = m[0]->row;
  
  ui.valXaxis->setMaximum(maxcol);
  ui.valYaxis->setMaximum(maxcol);
  ui.valZaxis->setMaximum(maxcol);
 
  // Set Points and Labels
  p.points()->SetNumberOfPoints(npoints);
  
  setScalingFactor();
  int k = 0;
  for(int i = 0; i < m.size(); i++){
    for(uint j = 0; j < m[i]->row; j++){
      p.points()->SetPoint(k, getMatrixValue(m[i], j, ui.valXaxis->value()-1), getMatrixValue(m[i], j, ui.valYaxis->value()-1), getMatrixValue(m[i], j, ui.valZaxis->value()-1));
      
      p.pointid().append(POINT3DID());
      p.pointid().last().id = j;
      p.pointid().last().xhash = "-";
      p.pointid().last().yhash = "-";
      
      if(k < objnames.size()){
        p.labels().append(objnames[k]);
      }
      else{ // Append Generic name
        p.labels().append(QString("Obj %1").arg(QString::number(k+1)));
      }
      
      k++;
    }
  }
  
  // Set Object Colors
  QList<int> QtColours;
  QtColours.append(3);  // 0 Qt::white  3       White (#ffffff)
  QtColours.append(7);  // 1 Qt::red 7       Red (#ff0000)
  QtColours.append(9);  // 2 Qt::blue        9       Blue (#0000ff)
  QtColours.append(8);  // 3 Qt::green       8       Green (#00ff00)
  QtColours.append(10); // 4 Qt::cyan        10      Cyan (#00ffff)
  QtColours.append(11); // 5 Qt::magenta     11      Magenta (#ff00ff)
  QtColours.append(12); // 6 Qt::yellow      12      Yellow (#ffff00)
  QtColours.append(5);  // 7 Qt::gray        5       Gray (#a0a0a4)
  QtColours.append(16); // 8 Qt::darkCyan    16      Dark cyan (#008080)
  QtColours.append(17); // 9 Qt::darkMagenta 17      Dark magenta (#800080)
  QtColours.append(18); // 10 Qt:darkYellow  18      Dark yellow (#808000)
  QtColours.append(4);  // 11 Qt::darkGray    4       Dark gray (#808080)
  QtColours.append(6);  // 12 Qt::lightGray   6       Light gray (#c0c0c0)
  QtColours.append(13); // 13 Qt:darkRed     13      Dark red (#800000)
  QtColours.append(14); // 14 Qt::darkGreen   14      Dark green (#008000)
  QtColours.append(15); // 15 Qt::darkBlue    15      Dark blue (#000080)

  
  p.colors()->SetName("colors");
  p.colors()->SetNumberOfComponents(3); // 3 because is a tuple of red green and blue
//   p.colors()->SetNumberOfValues(npoints);
  
  QColor symbolcolor;
  unsigned char c[3] = {255, 255, 255};
  for(int mxid = 0; mxid < m.size(); mxid++){
    if(mxid < QtColours.size()){
      symbolcolor = QColor((Qt::GlobalColor)QtColours[mxid]);
    }
    else{
      symbolcolor.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
    }
    
    c[0] = symbolcolor.red();
    c[1] = symbolcolor.green();
    c[2] = symbolcolor.blue();
    
    for(uint i = 0; i < m[mxid]->row; i++){
      p.colors()->InsertNextTupleValue(c);
    }
  }
  
  p.polydata()->SetPoints(p.points()); // add to polydata the points
  p.polydata()->GetPointData()->SetScalars(p.colors()); // bound points to polydata and set colors
  
  // Set Symbol size
  p.sphereSource()->SetRadius(ui.actionSymbolSize->value() * scaling);
  p.glyph3D()->SetSourceConnection(p.sphereSource()->GetOutputPort());
  // Set on glyp3D the shape
  p.glyph3D()->SetInputData(p.polydata());
  p.glyph3D()->SetColorModeToColorByScalar();
  p.glyph3D()->ScalingOff();
  p.glyph3D()->Update();
  
  // Create a mapper and actor
  p.mapper()->SetInputConnection(p.glyph3D()->GetOutputPort());
  p.actor()->SetMapper(p.mapper());
  
  
  // Creating outline box with mapper and actor
  outline->SetInputData(p.polydata());
  outlineMapper->SetInputConnection(outline->GetOutputPort());
  outlineActor->SetMapper(outlineMapper);
  
  SetOutlineColor();
  
  cubeAxesActor->SetBounds(outlineMapper->GetBounds());
  cubeAxesActor->SetCamera(renderer->GetActiveCamera());

  cubeAxesActor->SetXTitle(QString("%1 %2").arg(xname).arg(ui.valXaxis->value()).toUtf8().constData());
  cubeAxesActor->SetYTitle(QString("%1 %2").arg(yname).arg(ui.valYaxis->value()).toUtf8().constData());
  cubeAxesActor->SetZTitle(QString("%1 %2").arg(zname).arg(ui.valZaxis->value()).toUtf8().constData());
  
  renderer->AddActor(p.actor());
  renderer->AddActor(cubeAxesActor);
  renderer->AddActor(outlineActor);
  
  backgroundcolor = QColor(Qt::black);
  ui.backgroundColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(backgroundcolor.red()).arg(backgroundcolor.green()).arg(backgroundcolor.blue()));
  renderer->SetBackground(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF());
  
  renderer->ResetCamera();
  
  ui.qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
  
  ui.qvtkWidget->GetInteractor()->SetPicker(pointPicker);
  ui.qvtkWidget->GetInteractor()->SetInteractorStyle(style);
  
  UpdateSelectedObjects();
  
  Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  this->Connections->Connect(this->ui.qvtkWidget->GetRenderWindow()->GetInteractor(), vtkCommand::LeftButtonPressEvent, this, SLOT(UpdateSelectionLabel()));
  connect(ui.listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButton()));
  connect(ui.actionSymbolSize, SIGNAL(valueChanged(double)), SLOT(SetSymbolSize()));
  connect(ui.actionBackgroundSelectColor, SIGNAL(clicked(bool)), SLOT(SetBackgroundColor()));
  connect(ui.actionOutlineSelectColor, SIGNAL(clicked(bool)), SLOT(SetOutlineColor()));
  connect(ui.valXaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.valYaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.valZaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.resetbutton, SIGNAL(clicked(bool)), SLOT(ResetPlot()));
  connect(ui.buttonSelectAll, SIGNAL(clicked(bool)), SLOT(selectAll()));
  connect(ui.buttonUnselectAll, SIGNAL(clicked(bool)), SLOT(unselectAll()));
  connect(ui.buttonInvertSelection, SIGNAL(clicked(bool)), SLOT(invertSelection()));
//   connect(ui.buttonSelectBy, SIGNAL(clicked(bool)), SLOT(selectBy()));
  connect(ui.buttonSetSelectionStyle, SIGNAL(clicked(bool)), SLOT(setSelectionStyle()));
  connect(ui.buttonSavePlotToImage, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

ScatterPlot3D::ScatterPlot3D(QList< matrix* > m_, QList< QStringList >& objnames_, QString windowtitle, QString xname_, QString yname_, QString zname_): QWidget()
{
  ui.setupUi(this);
  typedata = -1;
  pid = -1;
  ui.buttonSetSelectionLabel->hide();
  ui.buttonSelectBy->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();
  
  xname = xname_;
  yname = yname_;
  zname = zname_;
  
  ui.valXaxis_name->setText(xname);
  ui.valYaxis_name->setText(yname);
  ui.valZaxis_name->setText(zname);
  
  mxlst = 0;
  arlst = 0;
  
  outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outlineActor = vtkSmartPointer<vtkActor>::New();
  cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
  renderer = vtkSmartPointer<vtkRenderer>::New(); 
  
  pointPicker = vtkSmartPointer<vtkPointPicker>::New();
  style = vtkSmartPointer<MouseInteractorStylePP>::New();
  
  setWindowTitle(windowtitle);
  
  for(int i = 0; i < objnames_.size(); i++){
    objnames.append(objnames[i]);
  }
  
  ui.listView->setModel(new QStringListModel(objnames));
  
  // deep copy
  for(int i = 0; i < m_.size(); i++){
    matrix *_m_;
    m.append(_m_);
    initMatrix(&m.last());
    MatrixCopy(m_[i], &m.last());
  }
  
  //chek for the max of col that could be plot:
  uint maxcol = m[0]->col;
  uint npoints = m[0]->row;
  for(int i = 1; i < m.size(); i++){
    npoints += m[i]->row;
    if(m[i]->col < maxcol){
      maxcol = m[i]->col;
    }
    else{
      continue;
    }
  }
  
  ui.valXaxis->setMaximum(maxcol);
  ui.valYaxis->setMaximum(maxcol);
  ui.valZaxis->setMaximum(maxcol);
 
  // Set Points and Labels
  p.points()->SetNumberOfPoints(npoints);
  
  setScalingFactor();
  int k = 0;
  for(int i = 0; i < m.size(); i++){
    for(uint j = 0; j < m[i]->row; j++){
      p.points()->SetPoint(k, getMatrixValue(m[i], j, ui.valXaxis->value()-1), getMatrixValue(m[i], j, ui.valYaxis->value()-1), getMatrixValue(m[i], j, ui.valZaxis->value()-1));
      
      p.pointid().append(POINT3DID());
      p.pointid().last().id = j;
      p.pointid().last().xhash = "-";
      p.pointid().last().yhash = "-";
      
      if(k < objnames.size()){
        p.labels().append(objnames[k]);
      }
      else{ // Append Generic name
        p.labels().append(QString("Obj %1").arg(QString::number(k+1)));
      }
      
      k++;
    }
  }
  
  // Set Object Colors
  QList<int> QtColours;
  QtColours.append(3);  // 0 Qt::white  3       White (#ffffff)
  QtColours.append(7);  // 1 Qt::red 7       Red (#ff0000)
  QtColours.append(9);  // 2 Qt::blue        9       Blue (#0000ff)
  QtColours.append(8);  // 3 Qt::green       8       Green (#00ff00)
  QtColours.append(10); // 4 Qt::cyan        10      Cyan (#00ffff)
  QtColours.append(11); // 5 Qt::magenta     11      Magenta (#ff00ff)
  QtColours.append(12); // 6 Qt::yellow      12      Yellow (#ffff00)
  QtColours.append(5);  // 7 Qt::gray        5       Gray (#a0a0a4)
  QtColours.append(16); // 8 Qt::darkCyan    16      Dark cyan (#008080)
  QtColours.append(17); // 9 Qt::darkMagenta 17      Dark magenta (#800080)
  QtColours.append(18); // 10 Qt:darkYellow  18      Dark yellow (#808000)
  QtColours.append(4);  // 11 Qt::darkGray    4       Dark gray (#808080)
  QtColours.append(6);  // 12 Qt::lightGray   6       Light gray (#c0c0c0)
  QtColours.append(13); // 13 Qt:darkRed     13      Dark red (#800000)
  QtColours.append(14); // 14 Qt::darkGreen   14      Dark green (#008000)
  QtColours.append(15); // 15 Qt::darkBlue    15      Dark blue (#000080)

  
  p.colors()->SetName("colors");
  p.colors()->SetNumberOfComponents(3); // 3 because is a tuple of red green and blue
//   p.colors()->SetNumberOfValues(npoints);
  
  QColor symbolcolor;
  unsigned char c[3] = {255, 255, 255};
  for(int mxid = 0; mxid < m.size(); mxid++){
    if(mxid < QtColours.size()){
      symbolcolor = QColor((Qt::GlobalColor)QtColours[mxid]);
    }
    else{
      symbolcolor.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
    }
    
    c[0] = symbolcolor.red();
    c[1] = symbolcolor.green();
    c[2] = symbolcolor.blue();
    
    for(uint i = 0; i < m[mxid]->row; i++){
      p.colors()->InsertNextTupleValue(c);
    }
  }
  
  p.polydata()->SetPoints(p.points()); // add to polydata the points
  p.polydata()->GetPointData()->SetScalars(p.colors()); // bound points to polydata and set colors
  
  // Set Symbol size
  p.sphereSource()->SetRadius(ui.actionSymbolSize->value() * scaling);
  p.glyph3D()->SetSourceConnection(p.sphereSource()->GetOutputPort());
  // Set on glyp3D the shape
  p.glyph3D()->SetInputData(p.polydata());
  p.glyph3D()->SetColorModeToColorByScalar();
  p.glyph3D()->ScalingOff();
  p.glyph3D()->Update();
  
  // Create a mapper and actor
  p.mapper()->SetInputConnection(p.glyph3D()->GetOutputPort());
  p.actor()->SetMapper(p.mapper());
  
  
  // Creating outline box with mapper and actor
  outline->SetInputData(p.polydata());
  outlineMapper->SetInputConnection(outline->GetOutputPort());
  outlineActor->SetMapper(outlineMapper);
  
  SetOutlineColor();
  
  cubeAxesActor->SetBounds(outlineMapper->GetBounds());
  cubeAxesActor->SetCamera(renderer->GetActiveCamera());

  cubeAxesActor->SetXTitle(QString("%1 %2").arg(xname).arg(ui.valXaxis->value()).toUtf8().constData());
  cubeAxesActor->SetYTitle(QString("%1 %2").arg(yname).arg(ui.valYaxis->value()).toUtf8().constData());
  cubeAxesActor->SetZTitle(QString("%1 %2").arg(zname).arg(ui.valZaxis->value()).toUtf8().constData());
  
  renderer->AddActor(p.actor());
  renderer->AddActor(cubeAxesActor);
  renderer->AddActor(outlineActor);
  
  backgroundcolor = QColor(Qt::black);
  ui.backgroundColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(backgroundcolor.red()).arg(backgroundcolor.green()).arg(backgroundcolor.blue()));
  renderer->SetBackground(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF());
  
  renderer->ResetCamera();
  
  ui.qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
  
  ui.qvtkWidget->GetInteractor()->SetPicker(pointPicker);
  ui.qvtkWidget->GetInteractor()->SetInteractorStyle(style);
  
  UpdateSelectedObjects();
  
  Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  this->Connections->Connect(this->ui.qvtkWidget->GetRenderWindow()->GetInteractor(), vtkCommand::LeftButtonPressEvent, this, SLOT(UpdateSelectionLabel()));
  connect(ui.listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButton()));
  connect(ui.actionSymbolSize, SIGNAL(valueChanged(double)), SLOT(SetSymbolSize()));
  connect(ui.actionBackgroundSelectColor, SIGNAL(clicked(bool)), SLOT(SetBackgroundColor()));
  connect(ui.actionOutlineSelectColor, SIGNAL(clicked(bool)), SLOT(SetOutlineColor()));
  connect(ui.valXaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.valYaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.valZaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.resetbutton, SIGNAL(clicked(bool)), SLOT(ResetPlot()));
  connect(ui.buttonSelectAll, SIGNAL(clicked(bool)), SLOT(selectAll()));
  connect(ui.buttonUnselectAll, SIGNAL(clicked(bool)), SLOT(unselectAll()));
  connect(ui.buttonInvertSelection, SIGNAL(clicked(bool)), SLOT(invertSelection()));
//   connect(ui.buttonSelectBy, SIGNAL(clicked(bool)), SLOT(selectBy()));
  connect(ui.buttonSetSelectionStyle, SIGNAL(clicked(bool)), SLOT(setSelectionStyle()));
  connect(ui.buttonSavePlotToImage, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

ScatterPlot3D::ScatterPlot3D(QList< matrix* > m_, QList< QStringList >& objnames_, QList<MATRIX*> *mxlst_,QStringList xhash_, QStringList yhash_, LABELS* objlabels_, LABELS *varlabels_, QString windowtitle, QString xname_, QString yname_, QString zname_, int type_): QWidget()
{
  type = type_;
  typedata = MATRIXDATA;
  pid = -1;
  ui.setupUi(this);
  ui.progressBar->hide();
  ui.abortButton->hide();
  
  xname = xname_, 
  yname = yname_;
  zname = zname_;
  
  ui.valXaxis_name->setText(xname);
  ui.valYaxis_name->setText(yname);
  ui.valZaxis_name->setText(zname);
  
  if(type == LOADINGS){
    ui.clusterAnalysisButton->hide();
  }

  pointPicker = vtkSmartPointer<vtkPointPicker>::New();
  style = vtkSmartPointer<MouseInteractorStylePP>::New();
  
  outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outlineActor = vtkSmartPointer<vtkActor>::New();
  cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
  renderer = vtkSmartPointer<vtkRenderer>::New(); 
  
  setWindowTitle(windowtitle);
  
  for(int i = 0; i < objnames_.size(); i++){
    objnames.append(objnames_[i]);
  }
  
  ui.listView->setModel(new QStringListModel(objnames));
  
  objlabels = objlabels_; 
  varlabels = varlabels_;
  mxlst = mxlst_;
  arlst = 0;
  xhash = xhash_;
  yhash = yhash_;
//   m = m_;
  
  for(int i = 0; i < m_.size(); i++){
    matrix *_m_;
    m.append(_m_);
    initMatrix(&m.last());
    MatrixCopy(m_[i], &m.last());
  }
  
  //chek for the max of col that could be plot:
  uint maxcol = m[0]->col;
  uint npoints = m[0]->row;
  for(int i = 1; i < m.size(); i++){
    npoints += m[i]->row;
    if(m[i]->col < maxcol){
      maxcol = m[i]->col;
    }
    else{
      continue;
    }
  }
  
  ui.valXaxis->setMaximum(maxcol);
  ui.valYaxis->setMaximum(maxcol);
  ui.valZaxis->setMaximum(maxcol);
 
  // Set Points and Labels
  p.points()->SetNumberOfPoints(npoints);
  
  setScalingFactor();
  int k = 0;
  for(int i = 0; i < m.size(); i++){
    for(uint j = 0; j < m[i]->row; j++){
      p.points()->SetPoint(k, getMatrixValue(m[i], j, ui.valXaxis->value()-1), getMatrixValue(m[i], j, ui.valYaxis->value()-1), getMatrixValue(m[i], j, ui.valZaxis->value()-1));
      p.pointid().append(POINT3DID());
      
      if(xhash.size() > 0 && yhash.size() > 0 && xhash.size() == yhash.size()){
        p.pointid().last().id = j;
        if(i < xhash.size()){
          p.pointid().last().xhash = xhash[i];
        }
        else{
          p.pointid().last().xhash = "-";
        }
        
        if(i < yhash.size()){
          p.pointid().last().yhash = yhash[i];
        }
        else{
          p.pointid().last().yhash = "-";
        }
      }
      else if(xhash.size() > 0 && yhash.size() > 0 && xhash.size() != yhash.size()){
        p.pointid().last().id = j;
        if(i < xhash.size()){
          p.pointid().last().xhash = xhash[i];
        }
        else{
          p.pointid().last().xhash = "-";
        }
        
        if(i < yhash.size()){
          p.pointid().last().yhash = yhash[i];
        }
        else{
          p.pointid().last().yhash = "-";
        }
      }
      else if(xhash.size() > 0 && yhash.size() == 0){
        p.pointid().last().id = j;
        if(i < xhash.size()){
          p.pointid().last().xhash = xhash[i];
        }
        else{
          p.pointid().last().xhash = "-";
        }
        
        p.pointid().last().yhash = "-";
      }
      else if(xhash.size() == 0 && yhash.size() > 0){
        p.pointid().last().id = j;
        p.pointid().last().xhash = "-";
        
        if(i < yhash.size()){
          p.pointid().last().yhash = yhash[i];
        }
        else{
          p.pointid().last().yhash = "-";
        }
      }
      else{ // xhash.size() == 0 && yhash.size() == 0
        p.pointid().last().id = j;
        p.pointid().last().xhash = "-";
        p.pointid().last().yhash = "-";
      }
      
      if(k < objnames.size()){
        p.labels().append(objnames[k]);
      }
      else{ // Append Generic name
        p.labels().append(QString("Obj %1").arg(QString::number(k+1)));
      }
      
      k++;
    }
  }
  
  // Set Object Colors
  QList< int > QtColours;
  QtColours.append(3);
  QtColours.append(7);
  QtColours.append(9);
  QtColours.append(8);
  QtColours.append(10);
  QtColours.append(11);
  QtColours.append(12);
  QtColours.append(5);
  QtColours.append(16);
  QtColours.append(17);
  QtColours.append(18);
  QtColours.append(4);
  QtColours.append(6);
  QtColours.append(13);
  QtColours.append(14);
  QtColours.append(15);
  
  p.colors()->SetName("colors");
  p.colors()->SetNumberOfComponents(3); // 3 because is a tuple of red green and blue
//   p.colors()->SetNumberOfValues(npoints);
  
  QColor symbolcolor;
  unsigned char c[3] = {255, 255, 255};
  for(int mxid = 0; mxid < m.size(); mxid++){
    if(mxid < QtColours.size()){
      symbolcolor = QColor((Qt::GlobalColor)QtColours[mxid]);
    }
    else{
      symbolcolor.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
    }
    
    c[0] = symbolcolor.red();
    c[1] = symbolcolor.green();
    c[2] = symbolcolor.blue();
    
    for(uint i = 0; i < m[mxid]->row; i++){
      p.colors()->InsertNextTupleValue(c);
    }
  }
  
  
  p.polydata()->SetPoints(p.points()); // add to polydata the points
  p.polydata()->GetPointData()->SetScalars(p.colors()); // bound points to polydata and set colors
  
  
  
  // Set Symbol size
  p.sphereSource()->SetRadius(ui.actionSymbolSize->value() * scaling);
  p.glyph3D()->SetSourceConnection(p.sphereSource()->GetOutputPort());
  // Set on glyp3D the shape
  p.glyph3D()->SetInputData(p.polydata());
  p.glyph3D()->SetColorModeToColorByScalar();
  p.glyph3D()->ScalingOff();
  p.glyph3D()->Update();
  // Create a mapper and actor
  p.mapper()->SetInputConnection(p.glyph3D()->GetOutputPort());
  
  p.actor()->SetMapper(p.mapper());
  
  
  // Creating outline box with mapper and actor
  outline->SetInputData(p.polydata());
  outlineMapper->SetInputConnection(outline->GetOutputPort());
  outlineActor->SetMapper(outlineMapper);
  
  SetOutlineColor();
  
  cubeAxesActor->SetBounds(outlineMapper->GetBounds());
  cubeAxesActor->SetCamera(renderer->GetActiveCamera());

  cubeAxesActor->SetXTitle(QString("%1 %2").arg(xname).arg(ui.valXaxis->value()).toUtf8().constData());
  cubeAxesActor->SetYTitle(QString("%1 %2").arg(yname).arg(ui.valYaxis->value()).toUtf8().constData());
  cubeAxesActor->SetZTitle(QString("%1 %2").arg(zname).arg(ui.valZaxis->value()).toUtf8().constData());
  
  renderer->AddActor(p.actor());
  renderer->AddActor(cubeAxesActor);
  renderer->AddActor(outlineActor);
  
  backgroundcolor = QColor(Qt::black);
  ui.backgroundColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(backgroundcolor.red()).arg(backgroundcolor.green()).arg(backgroundcolor.blue()));
  renderer->SetBackground(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF());
  
  renderer->ResetCamera();
  
  ui.qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

  ui.qvtkWidget->GetInteractor()->SetPicker(pointPicker);
  ui.qvtkWidget->GetInteractor()->SetInteractorStyle(style);
  
  UpdateSelectedObjects();
  
  Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  this->Connections->Connect(this->ui.qvtkWidget->GetRenderWindow()->GetInteractor(), vtkCommand::LeftButtonPressEvent, this, SLOT(UpdateSelectionLabel()));
  connect(ui.listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButton()));
  connect(ui.actionSymbolSize, SIGNAL(valueChanged(double)), SLOT(SetSymbolSize()));
  connect(ui.actionBackgroundSelectColor, SIGNAL(clicked(bool)), SLOT(SetBackgroundColor()));
  connect(ui.actionOutlineSelectColor, SIGNAL(clicked(bool)), SLOT(SetOutlineColor()));
  connect(ui.valXaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.valYaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.valZaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.resetbutton, SIGNAL(clicked(bool)), SLOT(ResetPlot()));
  connect(ui.buttonSelectAll, SIGNAL(clicked(bool)), SLOT(selectAll()));
  connect(ui.buttonUnselectAll, SIGNAL(clicked(bool)), SLOT(unselectAll()));
  connect(ui.buttonInvertSelection, SIGNAL(clicked(bool)), SLOT(invertSelection()));
  connect(ui.clusterAnalysisButton, SIGNAL(clicked(bool)), SLOT(DoClusterAnalysis()));
  connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
  
  if(type == SCORES){
    connect(ui.buttonSelectBy, SIGNAL(clicked(bool)), SLOT(selectBy()));
    connect(ui.buttonSetSelectionLabel, SIGNAL(clicked(bool)), SLOT(setSelectionObjLabel()));
  }
  else{
    connect(ui.buttonSelectBy, SIGNAL(clicked(bool)), SLOT(selectByVarLabels()));
    connect(ui.buttonSetSelectionLabel, SIGNAL(clicked(bool)), SLOT(setSelectionVarLabel()));
  }
  
  connect(ui.buttonSetSelectionStyle, SIGNAL(clicked(bool)), SLOT(setSelectionStyle()));
  
  connect(ui.buttonSavePlotToImage, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}



ScatterPlot3D::ScatterPlot3D(QList< matrix* > m_, QList< QStringList >& objnames_, QList<ARRAY*> *arlst_,QStringList xhash_, QStringList yhash_, LABELS* objlabels_, LABELS *varlabels_, QString windowtitle, QString xname_, QString yname_, QString zname_, int type_): QWidget()
{
  type = type_;
  typedata = ARRAYDATA;
  pid = -1;
  ui.setupUi(this);
  ui.progressBar->hide();
  ui.abortButton->hide();
  
  xname = xname_;
  yname = yname_;
  zname = zname_;
  
  ui.valXaxis_name->setText(xname);
  ui.valYaxis_name->setText(yname);
  ui.valZaxis_name->setText(zname);
  
  if(type == LOADINGS){
    ui.clusterAnalysisButton->hide();
  }
  
  pointPicker = vtkSmartPointer<vtkPointPicker>::New();
  style = vtkSmartPointer<MouseInteractorStylePP>::New();
  
  outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outlineActor = vtkSmartPointer<vtkActor>::New();
  cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
  renderer = vtkSmartPointer<vtkRenderer>::New(); 
  
  setWindowTitle(windowtitle);
  

  for(int i = 0; i < objnames_.size(); i++){
    objnames.append( objnames_[i]);
  }

  ui.listView->setModel(new QStringListModel(objnames));
  
  objlabels = objlabels_;
  varlabels = varlabels_;
  mxlst = 0;
  arlst = arlst_;
  xhash = xhash_;
  yhash = yhash_;
  
  for(int i = 0; i < m_.size(); i++){
    matrix *_m_;
    m.append(_m_);
    initMatrix(&m.last());
    MatrixCopy(m_[i], &m.last());
  }
  
  //chek for the max of col that could be plot:
  uint maxcol = m[0]->col;
  uint npoints = m[0]->row;
  for(int i = 1; i < m.size(); i++){
    npoints += m[i]->row;
    if(m[i]->col < maxcol){
      maxcol = m[i]->col;
    }
    else{
      continue;
    }
  }
  
  ui.valXaxis->setMaximum(maxcol);
  ui.valYaxis->setMaximum(maxcol);
  ui.valZaxis->setMaximum(maxcol);
 
  // Set Points and Labels
  p.points()->SetNumberOfPoints(npoints);
  
  setScalingFactor();
  int k = 0;
  for(int i = 0; i < m.size(); i++){
    for(uint j = 0; j < m[i]->row; j++){
      p.points()->SetPoint(k, getMatrixValue(m[i], j, ui.valXaxis->value()-1), getMatrixValue(m[i], j, ui.valYaxis->value()-1), getMatrixValue(m[i], j, ui.valZaxis->value()-1));
      
      p.pointid().append(POINT3DID());
      
      if(xhash.size() > 0 && yhash.size() > 0 && xhash.size() == yhash.size()){
        p.pointid().last().id = j;
        if(i < xhash.size()){
          p.pointid().last().xhash = xhash[i];
        }
        else{
          p.pointid().last().xhash = "-";
        }
        
        if(i < yhash.size()){
          p.pointid().last().yhash = yhash[i];
        }
        else{
          p.pointid().last().yhash = "-";
        }
      }
      else if(xhash.size() > 0 && yhash.size() > 0 && xhash.size() != yhash.size()){
        p.pointid().last().id = j;
        if(i < xhash.size()){
          p.pointid().last().xhash = xhash[i];
        }
        else{
          p.pointid().last().xhash = "-";
        }
        
        if(i < yhash.size()){
          p.pointid().last().yhash = yhash[i];
        }
        else{
          p.pointid().last().yhash = "-";
        }
      }
      else if(xhash.size() > 0 && yhash.size() == 0){
        p.pointid().last().id = j;
        if(i < xhash.size()){
          p.pointid().last().xhash = xhash[i];
        }
        else{
          p.pointid().last().xhash = "-";
        }
        
        p.pointid().last().yhash = "-";
      }
      else if(xhash.size() == 0 && yhash.size() > 0){
        p.pointid().last().id = j;
        p.pointid().last().xhash = "-";
        
        if(i < yhash.size()){
          p.pointid().last().yhash = yhash[i];
        }
        else{
          p.pointid().last().yhash = "-";
        }
      }
      else{ // xhash.size() == 0 && yhash.size() == 0
        p.pointid().last().id = j;
        p.pointid().last().xhash = "-";
        p.pointid().last().yhash = "-";
      }
      
      
      if(k < objnames.size()){
        p.labels().append(objnames[k]);
      }
      else{ // Append Generic name
        p.labels().append(QString("Obj %1").arg(QString::number(k+1)));
      }
      
      k++;
    }
  }
  
  // Set Object Colors
  QList< int > QtColours;
  QtColours.append(3);
  QtColours.append(7);
  QtColours.append(9);
  QtColours.append(8);
  QtColours.append(10);
  QtColours.append(11);
  QtColours.append(12);
  QtColours.append(5);
  QtColours.append(16);
  QtColours.append(17);
  QtColours.append(18);
  QtColours.append(4);
  QtColours.append(6);
  QtColours.append(13);
  QtColours.append(14);
  QtColours.append(15);
  
  p.colors()->SetName("colors");
  p.colors()->SetNumberOfComponents(3); // 3 because is a tuple of red green and blue
  
  QColor symbolcolor;
  unsigned char c[3] = {255, 255, 255};
  for(int mxid = 0; mxid < m.size(); mxid++){
    if(mxid < QtColours.size()){
      symbolcolor = QColor((Qt::GlobalColor)QtColours[mxid]);
    }
    else{
      symbolcolor.setRgb(randInt(0, 255), randInt(0, 255), randInt(0, 255));
    }
    
    c[0] = symbolcolor.red();
    c[1] = symbolcolor.green();
    c[2] = symbolcolor.blue();
    
    for(uint i = 0; i < m[mxid]->row; i++){
      p.colors()->InsertNextTupleValue(c);
    }
  }
  
  p.polydata()->SetPoints(p.points()); // add to polydata the points
  p.polydata()->GetPointData()->SetScalars(p.colors()); // bound points to polydata and set colors

  // Set Symbol size
  p.sphereSource()->SetRadius(ui.actionSymbolSize->value() * scaling);
  p.glyph3D()->SetSourceConnection(p.sphereSource()->GetOutputPort());
  // Set on glyp3D the shape
  p.glyph3D()->SetInputData(p.polydata());
  p.glyph3D()->SetColorModeToColorByScalar();
  p.glyph3D()->ScalingOff();
  p.glyph3D()->Update();
  
  // Create a mapper and actor
  p.mapper()->SetInputConnection(p.glyph3D()->GetOutputPort());
  p.actor()->SetMapper(p.mapper());
  
  
  // Creating outline box with mapper and actor
  outline->SetInputData(p.polydata());
  outlineMapper->SetInputConnection(outline->GetOutputPort());
  outlineActor->SetMapper(outlineMapper);
  
  SetOutlineColor();
  
  cubeAxesActor->SetBounds(outlineMapper->GetBounds());
  cubeAxesActor->SetCamera(renderer->GetActiveCamera());
  
  cubeAxesActor->SetXTitle(QString("%1 %2").arg(xname).arg(ui.valXaxis->value()).toUtf8().constData());
  cubeAxesActor->SetYTitle(QString("%1 %2").arg(yname).arg(ui.valYaxis->value()).toUtf8().constData());
  cubeAxesActor->SetZTitle(QString("%1 %2").arg(zname).arg(ui.valZaxis->value()).toUtf8().constData());
  
  renderer->AddActor(p.actor());
  renderer->AddActor(cubeAxesActor);
  renderer->AddActor(outlineActor);
  
  backgroundcolor = QColor(Qt::black);
  ui.backgroundColorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(backgroundcolor.red()).arg(backgroundcolor.green()).arg(backgroundcolor.blue()));
  renderer->SetBackground(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF());
  
  renderer->ResetCamera();
  
  ui.qvtkWidget->GetRenderWindow()->AddRenderer(renderer);
  
  ui.qvtkWidget->GetInteractor()->SetPicker(pointPicker);
  ui.qvtkWidget->GetInteractor()->SetInteractorStyle(style);
  
  UpdateSelectedObjects();
  
  Connections = vtkSmartPointer<vtkEventQtSlotConnect>::New();
  this->Connections->Connect(this->ui.qvtkWidget->GetRenderWindow()->GetInteractor(), vtkCommand::LeftButtonPressEvent, this, SLOT(UpdateSelectionLabel()));
  connect(ui.listView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)), SLOT(EnableDisableButton()));
  connect(ui.actionSymbolSize, SIGNAL(valueChanged(double)), SLOT(SetSymbolSize()));
  connect(ui.actionBackgroundSelectColor, SIGNAL(clicked(bool)), SLOT(SetBackgroundColor()));
  connect(ui.actionOutlineSelectColor, SIGNAL(clicked(bool)), SLOT(SetOutlineColor()));
  connect(ui.valXaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.valYaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.valZaxis, SIGNAL(valueChanged(int)), SLOT(setPoints()));
  connect(ui.resetbutton, SIGNAL(clicked(bool)), SLOT(ResetPlot()));
  connect(ui.buttonSelectAll, SIGNAL(clicked(bool)), SLOT(selectAll()));
  connect(ui.buttonUnselectAll, SIGNAL(clicked(bool)), SLOT(unselectAll()));
  connect(ui.buttonInvertSelection, SIGNAL(clicked(bool)), SLOT(invertSelection()));
  connect(ui.buttonSetSelectionStyle, SIGNAL(clicked(bool)), SLOT(setSelectionStyle()));
  connect(ui.clusterAnalysisButton, SIGNAL(clicked(bool)), SLOT(DoClusterAnalysis()));
  connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
  
  if(type == SCORES){
    connect(ui.buttonSelectBy, SIGNAL(clicked(bool)), SLOT(selectBy()));
    connect(ui.buttonSetSelectionLabel, SIGNAL(clicked(bool)), SLOT(setSelectionObjLabel()));
  }
  else{
    connect(ui.buttonSelectBy, SIGNAL(clicked(bool)), SLOT(selectByVarLabels()));
    connect(ui.buttonSetSelectionLabel, SIGNAL(clicked(bool)), SLOT(setSelectionVarLabel()));
  }
  
  connect(ui.buttonSavePlotToImage, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

ScatterPlot3D::~ScatterPlot3D()
{
  for(int i = 0; i < m.size(); i++){
    DelMatrix(&m[i]);
  }
  m.clear();
} 
