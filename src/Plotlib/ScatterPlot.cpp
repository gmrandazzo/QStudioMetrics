#include "ScatterPlot.h"
#include <scientific.h>

#include <QStringListModel>
#include <QColorDialog>
#include <QFileDialog>
#include <QMenu>
#include <QContextMenuEvent>
#include <QMessageBox>

#include <unistd.h>

#include "../Dialogs/LabelDialog.h"
#include "../Dialogs/SelectByDialog.h"

#include "../Dialogs/SelectionStyleDialog.h"
#include "../Dialogs/LinePlotStyleDialog.h"
#include "../Dialogs/VariableSelectorDialog.h"
#include "../Dialogs/DoClusterAnalysisDialog.h"
#include "../Dialogs/FindCorrelationWidget.h"
#include "../Dialogs/ValidationClusterPlot.h"
#include "../Dialogs/PlotSettingsDialog.h"
#include "run.h"


Graphs* ScatterPlot::NewGraph(PEngine peng)
{
  if(peng == PEngine::Qtchart){
    return new Chart();
    //return new ChartQt(); // Low performance
  }
  else{
    return new QPlotlyWindow(this);
  }
}

void ScatterPlot::StartRun()
{
  abort = false;
  ui.abortButton->setEnabled(true);
  ui.progressBar->setMinimum(0);
  ui.progressBar->setMaximum(0);
  ui.progressBar->setValue(0);
  ui.progressBar->show();
  ui.abortButton->show();
}

void ScatterPlot::StopRun()
{
  abort = true;
  ui.abortButton->setEnabled(false);
  ui.progressBar->setMinimum(0);
  ui.progressBar->setMaximum(100);
  ui.progressBar->setValue(20);
  ui.progressBar->hide();
  ui.abortButton->hide();
}

int ScatterPlot::GetIDinmxlst(QString mxhash)
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

int ScatterPlot::makeSize(double val, double min, double max, int sizemin, int sizemax)
{
  double normval = (val - min) / (max -min);
  return (int)floor(sizemin + (normval * (sizemax - sizemin)));
}

QColor ScatterPlot::makeColor(double val, double min, double max, QColor &color1, QColor &color2)
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

int ScatterPlot::random(int min, int max) //range : [min, max)
{
   static bool first = true;
   if (first)
   {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

QList<QColor> ScatterPlot::GenColorList(int size)
{
  // Create color list
  QList<QColor> colors;
  colors << Qt::black << Qt::blue << Qt::red << Qt::green << Qt::magenta << Qt::cyan;

  if(size > colors.size()){
    srand(time(0));
    for(int i = 0; i < size - colors.size(); i++){
      colors.append(QColor(random(0, 256), random(0, 256), random(0, 256)));
    }
  }

  return colors;
}

/*
 * p.coord is a sequence of x and y:
 *  p.coord[x1][y1][x2][y2][x3][y3] etc...
 */
void ScatterPlot::BuildLine(qreal angular_coeff)
{
  qreal xmin, xmax, ymin, ymax;
  xmin = xmax = chart->getPoint(0)->x();
  ymin = ymax = chart->getPoint(0)->y();
  for(int i = 1; i < chart->PointSize(); i++){
    if(chart->getPoint(i)->x() < xmin){
      xmin = chart->getPoint(i)->x();
    }
    /*
    if(chart->getPoint(i)->x() > xmax){
      xmax = chart->getPoint(i)->x();
    }

    if(chart->getPoint(i)->y() < ymin){
      ymin = chart->getPoint(i)->y();
    }
    */
    if(chart->getPoint(i)->y() > ymax){
      ymax = chart->getPoint(i)->y();
    }
  }

  ymin = xmin * angular_coeff;
  xmax = ymax / angular_coeff;

  chart->RemoveAllCurves();
  QVector< QPointF > line;
  line.append(QPointF(xmin, ymin));
  line.append(QPointF(xmax, ymax));
  chart->addCurve(line, "fit line", Qt::red);


  /*
  for(int j = 0; j < p.first().coord.size(); j+=2){
    double xmin, xmax;
    xmin = xmax = p.first().coord[j];
    for(int i = 1; i < p.size(); i++){
      if(p[i].coord[j] < xmin)
        xmin = p.first().coord[j];

      if(p[i].coord[j] > xmax)
        xmax = p[i].coord[j];
    }

    // Adding 5% to min and max
    if(xmin > 0){
      xmin -= (5*xmin)/100;
    }
    else{
      xmin -= ((-1*xmin)*5)/100;
    }

    xmax += (5*xmax)/100;

    double ymin = xmin*getDVectorValue(b, j/2.);
    double ymax = xmax*getDVectorValue(b, j/2.);

    QVector< QPointF > line;
    line.append(QPointF(xmin, ymin));
    line.append(QPointF(xmax, ymax));

    chart->addCurve(line, "fit line", Qt::red);
  }*/
}

void ScatterPlot::BuildHotellingEllipse()
{
   /* The ellipse equation is given by the following formula:
    * (x/a)^2 + (y/b)^2 = s
    *
    * In our case a and b are the standard deviation in x axis
    * and y axis. S for our confidence interval of 95% is 5.991
    * coming from the t distribution table.
    *
   */

 /*
  * build the covariance matrix of the datas.
  */

  qDebug() << "Build Confidence Ellipse";
  matrix *xy, *covxy;
  int vpoint = 0;
  for(int i = 0; i < chart->PointSize(); i++){
    if(chart->getPoint(i)->isVisible() == true){
      vpoint++;
    }
    else
      continue;
  }
  NewMatrix(&xy, vpoint, 2);
  vpoint = 0;
  for(int i = 0; i < chart->PointSize(); i++){
   if(chart->getPoint(i)->isVisible() == true){
     xy->data[vpoint][0] = chart->getPoint(i)->x();
     xy->data[vpoint][1] = chart->getPoint(i)->y();
     vpoint++;
   }
  }

  /* Calculate the eigenvectors and eigenvalue of the covariance matrix*/
  initMatrix(&covxy);
  MatrixCovariance(xy, &covxy);

  dvector *eval;
  matrix *evect;

  initDVector(&eval);
  initMatrix(&evect);

  EVectEval(covxy, &eval, &evect);

  DelMatrix(&covxy);

  /*Get the index of the largest and the smallest eigenvector */
  int min_evect, max_evect;
  min_evect = max_evect = 0;
  for(size_t i = 1; i < eval->size; i++){
    if(eval->data[i] < eval->data[min_evect]){
      min_evect = i;
    }

    if(eval->data[i] > eval->data[max_evect]){
      max_evect = i;
    }
  }

  /*Calculate the angle between the x axis and the largest evect */
  double angle = atan(evect->data[max_evect][1]/evect->data[max_evect][0]);
  /* This angle is between -pi and pi.
   * Let's shift it such that the angle is between 0 and 2pi*/
  if(angle < 0){
    angle = angle + 2*3.14159265359;
  }

  /*calculate the center of the ellipse */
  dvector *cc;
  initDVector(&cc);
  MatrixColAverage(xy, &cc);
  DelMatrix(&xy); // now we can remove xy


  /*Get the 95% of confidence intervall error ellipse*/
  size_t np_ellipse = 100;
  double chisquared_val = 2.4477;
  double phi = angle;
  dvector *theta_grid;
  NewDVector(&theta_grid, np_ellipse);

  /*Generate a linearly spaced vector with range 0, 2*3.14 of size np_ellipse*/
  double delta_space = (2*3.14159265359)/100.;
  theta_grid->data[0] = 0.f;
  for(size_t i = 1; i < 100; i++){
    theta_grid->data[i] = i*delta_space;
  }

  double a = chisquared_val*sqrt(eval->data[max_evect]);
  double b = chisquared_val*sqrt(eval->data[min_evect]);

  DelDVector(&eval);
  DelMatrix(&evect);

  /*The ellipse in x and y coordinates*/
  matrix *ellipse;
  NewMatrix(&ellipse, np_ellipse, 2);

  for(size_t i = 0; i < np_ellipse; i++){
    ellipse->data[i][0]  = a*cos(theta_grid->data[i]);
    ellipse->data[i][1]  = b*sin(theta_grid->data[i]);
  }

  DelDVector(&theta_grid);

  /*Define a rotation matrix */
  matrix *rot_mx;
  NewMatrix(&rot_mx, 2, 2);
  rot_mx->data[0][0] = cos(phi); rot_mx->data[0][1] = sin(phi);
  rot_mx->data[1][0] = -sin(phi); rot_mx->data[1][1] = cos(phi);

  /* Rotate the ellipse to some angle phi */
  matrix *r_ellipse;
  NewMatrix(&r_ellipse, np_ellipse, 2);

  MatrixDotProduct(ellipse, rot_mx, r_ellipse);
  DelMatrix(&ellipse);
  DelMatrix(&rot_mx);

  /*Draw the ellipse*/
  int cid = -1;
  for(int i = 0; i < chart->getCurves().size(); i++){
    if(chart->getCurves()[i].name().compare("Hotelling ellipse 95% confidence") == 0){
      cid = i;
      break;
    }
    else{
      continue;
    }
  }

  if(cid > -1){
    for(size_t i = 0; i < r_ellipse->row; i++){
      chart->getCurves()[cid].getPoints()[i].setX(r_ellipse->data[i][0]+cc->data[0]);
      chart->getCurves()[cid].getPoints()[i].setY(r_ellipse->data[i][1]+cc->data[1]);
    }
    chart->getCurves()[cid].getPoints()[r_ellipse->row].setX(r_ellipse->data[0][0]+cc->data[0]);
    chart->getCurves()[cid].getPoints()[r_ellipse->row].setY(r_ellipse->data[0][1]+cc->data[1]);
  }
  else{
    QVector< QPointF > curve;
    for(size_t i = 0; i < r_ellipse->row; i++){
      curve.append(QPointF(r_ellipse->data[i][0]+cc->data[0],r_ellipse->data[i][1]+cc->data[1]));
    }
    curve.append(QPointF(r_ellipse->data[0][0]+cc->data[0],r_ellipse->data[0][1]+cc->data[1])); // Close the curve
    chart->addCurve(curve, "Hotelling ellipse 95% confidence", Qt::black);
  }

  DelDVector(&cc);
  DelMatrix(&r_ellipse);
  PlotUpdate();
}

/*
 *          1                 2                 3                ...
 *
 *      0       1        2        3        4       5
 *  _____________________________________________________
 * |  t1    |  u1    |  t2    |  u2    |  t3    |  u3    | ......
 *
 *
 *  1:
 *    t1 = 0 + 2 * (1-1) = 0;
 *    u1 = 1 + 2 * (1-1) = 1;
 *
 *  2:
 *    t1 = 0 + 2 * (2-1) = 2;
 *    u1 = 1 + 2 * (2-1) = 3;
 *
 *  3:
 *    t1 = 0 + 2 * (3-1) = 4;
 *    u1 = 1 + 2 * (3-1) = 5;
 *
 *
 *   etc...
 */

void ScatterPlot::UpdateAxisName()
{
  if(autonameaxes == true){
    if(axistype == SINGLEAXIS){
      if(nameaxisext.size() > 0){
        chart->setXaxisName(QString("%1 %2").arg(xaxisname).arg(nameaxisext[ui.axis3->value()-1]));
        chart->setYaxisName(QString("%1 %2").arg(yaxisname).arg(nameaxisext[ui.axis3->value()-1]));
      }
      else{
        chart->setXaxisName(QString("%1 %2").arg(xaxisname).arg(QString::number(ui.axis3->value())));
        chart->setYaxisName(QString("%1 %2").arg(yaxisname).arg(QString::number(ui.axis3->value())));
      }
    }
    else if(axistype == DOUBLEAXIS){
      if(nameaxisext.size() > 0){
        chart->setXaxisName(QString("%1 %2").arg(xaxisname).arg(nameaxisext[ui.axis1->value()-1]));
        chart->setYaxisName(QString("%1 %2").arg(yaxisname).arg(nameaxisext[ui.axis2->value()-1]));
      }
      else{
        chart->setXaxisName(QString("%1 %2").arg(xaxisname).arg(QString::number(ui.axis1->value())));
        chart->setYaxisName(QString("%1 %2").arg(yaxisname).arg(QString::number(ui.axis2->value())));
      }
    }
    else{
      if(nameaxisext.size() > 0){
        chart->setXaxisName(QString("%1 %2").arg(xaxisname).arg(nameaxisext[ui.axis1->value()-1]));
        chart->setYaxisName(QString("%1 %2").arg(yaxisname).arg(nameaxisext[ui.axis2->value()-1]));
        chart->setZaxisName(QString("%1 %2").arg(yaxisname).arg(nameaxisext[ui.axis3->value()-1]));
      }
      else{
        chart->setXaxisName(QString("%1 %2").arg(xaxisname).arg(QString::number(ui.axis1->value())));
        chart->setYaxisName(QString("%1 %2").arg(yaxisname).arg(QString::number(ui.axis2->value())));
        chart->setZaxisName(QString("%1 %2").arg(yaxisname).arg(QString::number(ui.axis3->value())));
      }
    }
  }
  else{
    if(xaxisname.size() > 0)
      chart->setXaxisName(xaxisname);
    if(yaxisname.size() > 0)
      chart->setYaxisName(yaxisname);
    if(zaxisname.size() > 0)
      chart->setZaxisName(zaxisname);
  }
}

void ScatterPlot::initPoint(QList<matrix*> mx, QList<matrix*> my, QList<QStringList> name)
{
  //Only for 2D plot!
  QList<QColor> colors = GenColorList(mx.size());

  // Create a copy of datas into a structure...
  for(int mxid = 0; mxid < mx.size(); mxid++){
    for(uint i = 0; i < mx[mxid]->row; i++){
      p.append(QSMPOINT());
      for(uint j = 0; j < mx[mxid]->col; j++){
        p.last().coord.append(mx[mxid]->data[i][j]);
        p.last().coord.append(my[mxid]->data[i][j]);
      }
      p.last().name = name[mxid][i];
      p.last().color = colors[mxid];
      p.last().radius = 8;
    }
  }

  // plot the points... we suppose that...
  for(int i = 0; i < p.size(); i++){
    chart->addPoint(p[i].coord[0], p[i].coord[1], p[i].name, p[i].color, p[i].radius);
  }
  UpdateAxisName();
}

void ScatterPlot::initPoint(QList<matrix*> mx, QList<QStringList> name)
{
  //For 2D and 3D plots!
  qDebug() << "2D and 3D plot init";
  QList<QColor> colors = GenColorList(mx.size());

  for(int mxid = 0; mxid < mx.size(); mxid++){
    for(uint i = 0; i < mx[mxid]->row; i++){
      p.append(QSMPOINT());
      for(uint j = 0; j < mx[mxid]->col; j++){
        p.last().coord.append(mx[mxid]->data[i][j]);
      }
      p.last().name = name[mxid][i];
      p.last().color = colors[mxid];
      p.last().radius = 8;
    }
  }

  uint x, y, z;
  if(axistype == SINGLEAXIS){
    x = (uint) 0 + (2*ui.axis3->value()-2);
    y = (uint) 1 + (2*ui.axis3->value()-2);
    z = 0;
  }
  else if(axistype == DOUBLEAXIS){
    x = (uint) ui.axis1->value()-1;
    y = (uint) ui.axis2->value()-1;
    z = 0;
  }
  else{
    x = (uint) ui.axis1->value()-1;
    y = (uint) ui.axis2->value()-1;
    z = (uint) ui.axis3->value()-1;
  }



  if(axistype == SINGLEAXIS || axistype == DOUBLEAXIS){
    for(int i = 0; i < p.size(); i++){
      chart->addPoint(p[i].coord[x], p[i].coord[y], p[i].name, p[i].color, p[i].radius);
    }
  }
  else{
    for(int i = 0; i < p.size(); i++){
      chart->addPoint(p[i].coord[x], p[i].coord[y], p[i].coord[z], p[i].name, p[i].color, p[i].radius);
    }
  }

  UpdateAxisName();
}


void ScatterPlot::SelectAll()
{
  chart->SelectAll();
}

void ScatterPlot::SelectBy()
{
  matrix *coordinates;

  QStringList objname;

  NewMatrix(&coordinates, chart->PointSize(), 2);

  for(int i = 0; i < chart->PointSize(); i++){
    coordinates->data[i][0] = chart->getPoint(i)->x();
    coordinates->data[i][1] = chart->getPoint(i)->y();
    objname.append(chart->getPoint(i)->name());
  }

  QStringList selectedobject;
//   QList<int> selectedIDS_;
  QList< int > selectedobjid;
  QList<int> mxid;

  for(int i = 0; i < xhash.size(); i++){
    int id_ = GetIDinmxlst(xhash[i]);
    if(id_  != -1){
      mxid.append(id_ );
    }
    else{
      continue;
    }
  }

  SelectByDialog seldialog(objname, coordinates, mxlst, mxid, objlabels, varlabels);
  if(seldialog.exec() == QDialog::Accepted){
    selectedobjid = seldialog.GetSelectedObjID();
  }

  for(int i = 0; i < selectedobjid.size(); i++){
    chart->getPoint(selectedobjid[i])->setSelection(true);
    selectedIDS.append(selectedobjid[i]);
  }

  PlotUpdate();
  DelMatrix(&coordinates);
}

void ScatterPlot::SelectByVarlabels()
{
  selectedIDS.clear();
  QList<int> selectedIDS_;

  QStringList varnames;
  for(int i = 0; i < chart->PointSize(); i++){
    varnames.append(chart->getPoint(i)->name());
  }

  QStringList objnames;

  QList<int> xid;
  for(int i = 0; i < xhash.size(); i++){
    xid.append(GetIDinmxlst(xhash[i]));
  }

  if(vartablabels == 0){
    for(int i = 0; i < xid.size(); i++){
      objnames.append((*mxlst)[xid[i]]->getObjName());
    }
  }
  else{
    for(int i = 0; i < chart->PointSize(); i++){
      objnames.append(chart->getPoint(i)->name());
    }
  }
  VariableSelectorDialog vseldialog(objnames, varnames, (*mxlst), xid, varlabels);

  if(vseldialog.exec() == QDialog::Accepted){
    QStringList varselected = vseldialog.getSelectedVariables();

    for(int i = 0; i < chart->PointSize(); i++){
      if(varselected.indexOf(chart->getPoint(i)->name()) > -1){
        chart->getPoint(i)->setSelection(true);
        selectedIDS_.append(i);
      }
      else{
        continue;
      }
    }
  }
  else{
    return;
  }

  selectedIDS = selectedIDS_;
  PlotUpdate();
}

void ScatterPlot::invertSelection()
{
  QList<int> selectedIDS_;
  for(int i = 0; i < chart->PointSize(); i++){
    if(chart->getPoint(i)->isSelected() == true){
      chart->getPoint(i)->setSelection(false);
    }
    else{
      chart->getPoint(i)->setSelection(true);
      selectedIDS_.append(i);
    }
  }

  selectedIDS = selectedIDS_;
  PlotUpdate();
}

void ScatterPlot::hideSelection()
{
  for(int i = 0; i < selectedIDS.size(); i++){
    chart->getPoint(selectedIDS[i])->setVisible(false);
  }
  PlotUpdate();
  clearSelection();
}

void ScatterPlot::clearSelection()
{
  qDebug() << "Clear Selection";
  for(int i = 0; i < selectedIDS.size(); i++){
    chart->getPoint(selectedIDS[i])->setSelection(false);
  }
  chart->ClearSelection();
  selectedIDS.clear();
  PlotUpdate();
}

void ScatterPlot::SetSelectionObjLabels()
{
  if(selectedIDS.size() <= 0)
    return;
  else{
    QStringList selectedobj;
    for(int i = 0; i < selectedIDS.size(); i++){
      selectedobj.append(chart->getPoint(selectedIDS[i])->name());
    }
    LabelDialog ldialog(objlabels, selectedobj, LabelDialog::OBJLABELS);
    ldialog.exec();
  }
}


void ScatterPlot::SetSelectionVarLabels()
{
  if(selectedIDS.size() <= 0)
    return;
  else{
    QStringList selectedobj;

    for(int i = 0; i < selectedIDS.size(); i++){
      selectedobj.append(chart->getPoint(selectedIDS[i])->name());
    }

    LabelDialog ldialog(varlabels, selectedobj, LabelDialog::VARLABELS);
    ldialog.exec();
  }
}

void ScatterPlot::setSelectionStyle()
{
  if(type == LOADINGS){
    if(vartablabels != 0){
      QStringList varnames;
      for(int i = 0; i < (*vartablabels).size(); i++){
        varnames.append((*vartablabels)[i]->getFeaturesName());
      }
      varnames.removeAll("Object Names");
      varnames.removeDuplicates();

      SelectionStyleDialog obj(varnames);
      obj.setSymbolNames(markersymbls);
      if(obj.exec() == QDialog::Accepted){
        int changeshape = obj.ChangeShape();
        SelectionStyleDialog::ChangeType changecolor = obj.ChangeColor();
        SelectionStyleDialog::ChangeType changesize = obj.ChangeSize();

        if(changeshape == 1){
          MarkerType mtype = MarkerType(obj.getSymbolType());
          for(int i = 0; i < selectedIDS.size(); i++){
            chart->getPoint(selectedIDS[i])->setMarkerType(mtype);
          }
        }

        if(changecolor == SelectionStyleDialog::FIXED){
          // FIXED COLOR CHANGE
          for(int i = 0; i < selectedIDS.size(); i++){
            chart->getPoint(selectedIDS[i])->setColor(obj.getSolidSymbolColor());
          }
        }
        else if(changecolor == SelectionStyleDialog::GRADIENT){
          // GRADIENT COLOR CHANGE
          QString symcolvname = obj.getVariableSymbolColor();
          QColor mincolor = obj.getMinSymbolColor();
          QColor maxcolor = obj.getMaxSymbolColor();
          double min, max;
          QStringList objnameselected;
          QList <double> dval;

          for(int i = 0; i < selectedIDS.size(); i++){
            objnameselected.append(chart->getPoint(selectedIDS[i])->name());
            dval.append(9999.0);
          }

          min =  9999.0;
          max = -9999.0;
          for(int i = 0; i <  objnameselected.size(); i++){
            for(int j = 0; j < (*vartablabels).size(); j++){
              int objindx = (*vartablabels)[j]->getObjectsName().indexOf(objnameselected[i]);
              if(objindx > -1){
                int varindx = (*vartablabels)[j]->getFeaturesName().indexOf(symcolvname)-1;
                if(varindx > -1){
                  dval[i] = (*vartablabels)[j]->getMatrix()->data[objindx][varindx];
                  if(dval[i] < min){
                    min = dval[i];
                  }

                  if(dval[i] > max && dval[i] < 9999){
                    max = dval[i];
                  }

                  break;
                }
                else{
                  break; // this object do not contain this variable in matrix
                }
              }
              else{
                continue;
              }
            }
          }

          for(int i = 0; i < dval.size(); i++){
            if(FLOAT_EQ(dval[i], 9999.0, 1e-1)){
              continue;
            }
            else{
              chart->getPoint(selectedIDS[i])->setColor(makeColor(dval[i], min, max, mincolor, maxcolor));
            }
          }
        }

        if(changesize == SelectionStyleDialog::FIXED){
          // FIXED SIZE CHANGE
          int symsize = obj.getFixedSymbolSize();
          for(int i = 0; i < selectedIDS.size(); i++){
            chart->getPoint(selectedIDS[i])->setRadius(symsize);
          }
        }
        else if(changesize == SelectionStyleDialog::GRADIENT){
          // GRADIENT SIZE CHANGE
          QString symcolvname = obj.getVariableSymbolSize();
          int minsymbsize = obj.getMinSymbolSize();
          int maxsymbsize = obj.getMaxSymbolSize();
          double min, max;
          QStringList objnameselected;
          QList <double> dval;

          for(int i = 0; i < selectedIDS.size(); i++){
            objnameselected.append(chart->getPoint(selectedIDS[i])->name());
            dval.append(9999.0);
          }

          min =  9999.0;
          max = -9999.0;
          for(int i = 0; i <  objnameselected.size(); i++){
            for(int j = 0; j < (*vartablabels).size(); j++){
              int objindx = (*vartablabels)[j]->getObjectsName().indexOf(objnameselected[i]);
              if(objindx > -1){
                int varindx = (*vartablabels)[j]->getFeaturesName().indexOf(symcolvname)-1;
                if(varindx > -1){
                  dval[i] = (*vartablabels)[j]->getMatrix()->data[objindx][varindx];
                  if(dval[i] < min){
                    min = dval[i];
                  }

                  if(dval[i] > max && dval[i] < 9999){
                    max = dval[i];
                  }

                  break;
                }
                else{
                  break; // this object do not contain this variable in matrix
                }
              }
              else{
                continue;
              }
            }
          }

          for(int i = 0; i < dval.size(); i++){
            if(FLOAT_EQ(dval[i], 9999.0, 1e-1)){
              continue;
            }
            else{
              chart->getPoint(selectedIDS[i])->setRadius(makeSize(dval[i], min, max, minsymbsize, maxsymbsize));
            }
          }
        }
      }
    }
    else{
      SelectionStyleDialog obj;
      obj.setSymbolNames(markersymbls);
      if(obj.exec() == QDialog::Accepted){
        for(int i = 0; i < selectedIDS.size(); i++){
          chart->getPoint(selectedIDS[i])->setRadius(obj.getFixedSymbolSize());
          chart->getPoint(selectedIDS[i])->setColor(obj.getSolidSymbolColor());
        }
      }
    }
  }
  else{ // Scores
    QStringList varname;
    QList<int> xid, yid;
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

    if(xid.size() > 0){
      for(int i = 0; i < xid.size(); i++){
        varname.append((*mxlst)[xid[i]]->getVarName());
      }
      varname.removeDuplicates();
      //varname.removeAll("Labels");
    }

    if(yid.size() > 0){
      for(int i = 0; i < yid.size(); i++){
        varname.append((*mxlst)[yid[i]]->getVarName());
      }
      varname.removeDuplicates();
      //varname.removeAll("Labels");
    }

    varname.removeAll("Object Names");

    SelectionStyleDialog obj(varname);
    obj.setSymbolNames(markersymbls);
    if(obj.exec() == QDialog::Accepted){
      int changeshape = obj.ChangeShape();
      SelectionStyleDialog::ChangeType changecolor = obj.ChangeColor();
      SelectionStyleDialog::ChangeType changesize = obj.ChangeSize();

      if(changeshape == 1){
        MarkerType mtype = MarkerType(obj.getSymbolType());
        for(int i = 0; i < selectedIDS.size(); i++){
          chart->getPoint(selectedIDS[i])->setMarkerType(mtype);
        }
      }

      if(changecolor == SelectionStyleDialog::FIXED){
        for(int i = 0; i < selectedIDS.size(); i++){
          chart->getPoint(selectedIDS[i])->setColor(obj.getSolidSymbolColor());
        }
      }
      else if(changecolor == SelectionStyleDialog::GRADIENT){
        QString symcolvname = obj.getVariableSymbolColor();
        QColor mincolor = obj.getMinSymbolColor();
        QColor maxcolor = obj.getMaxSymbolColor();
        double min, max;
        QStringList objnameselected;
        QList <double> dval;

        for(int i = 0; i < selectedIDS.size(); i++){
          objnameselected.append(chart->getPoint(selectedIDS[i])->name());
          dval.append(9999.0);
        }

        min =  9999.0;
        max = -9999.0;
        for(int i = 0; i <  objnameselected.size(); i++){
          for(int j = 0; j < (*mxlst).size(); j++){
            int objindx = (*mxlst)[j]->getObjName().indexOf(objnameselected[i]);
            if(objindx > -1){
              int varindx = (*mxlst)[j]->getVarName().indexOf(symcolvname)-1;
              if(varindx > -1){
                dval[i] = (*mxlst)[j]->Matrix()->data[objindx][varindx];
                if(dval[i] < min){
                  min = dval[i];
                }

                if(dval[i] > max && dval[i] < 9999){
                  max = dval[i];
                }

                break;
              }
              else{
                break; // this object do not contain this variable in matrix
              }
            }
            else{
              continue;
            }
          }
        }

        for(int i = 0; i < dval.size(); i++){
          if(FLOAT_EQ(dval[i], 9999.0, 1e-1)){
            continue;
          }
          else{
            chart->getPoint(selectedIDS[i])->setColor(makeColor(dval[i], min, max, mincolor, maxcolor));
          }
        }
      }

      if(changesize == SelectionStyleDialog::FIXED){
        for(int i = 0; i < selectedIDS.size(); i++){
          chart->getPoint(selectedIDS[i])->setRadius(obj.getFixedSymbolSize());
        }
      }
      else if(changesize == SelectionStyleDialog::GRADIENT){
        QString symcolvname = obj.getVariableSymbolSize();
        int minsymbsize = obj.getMinSymbolSize();
        int maxsymbsize = obj.getMaxSymbolSize();
        double min, max;
        QStringList objnameselected;
        QList <double> dval;

        for(int i = 0; i < selectedIDS.size(); i++){
          objnameselected.append(chart->getPoint(selectedIDS[i])->name());
          dval.append(-9999.0);
        }

        min =  9999.0;
        max = -9999.0;
        for(int i = 0; i <  objnameselected.size(); i++){
          for(int j = 0; j < (*mxlst).size(); j++){
            int objindx = (*mxlst)[j]->getObjName().indexOf(objnameselected[i]);
            if(objindx > -1){
              int varindx = (*mxlst)[j]->getVarName().indexOf(symcolvname)-1;
              if(varindx > -1){
                dval[i] = (*mxlst)[j]->Matrix()->data[objindx][varindx];
                if(dval[i] < min){
                  min = dval[i];
                }

                if(dval[i] > max && dval[i] < 9999){
                  max = dval[i];
                }

                break;
              }
              else{
                break; // this object do not contain this variable in matrix
              }
            }
            else{
              continue;
            }
          }
        }

        for(int i = 0; i < dval.size(); i++){
          if(FLOAT_EQ(dval[i], -9999.0, 1e-1)){
            continue;
          }
          else{
            chart->getPoint(selectedIDS[i])->setRadius(makeSize(dval[i], min, max, minsymbsize, maxsymbsize));
          }
        }
      }
    }
  }
  PlotUpdate();
}

void ScatterPlot::showPointLabels()
{
  for(int i = 0; i < selectedIDS.size(); i++){
    chart->getPoint(selectedIDS[i])->setLabelVisible(true);
  }
  PlotUpdate();
}

void ScatterPlot::hidePointLabels()
{
  for(int i = 0; i < selectedIDS.size(); i++){
    chart->getPoint(selectedIDS[i])->setLabelVisible(false);
  }
  PlotUpdate();
}

void ScatterPlot::OpenPlotSettingsDialog()
{
  double xmin, xmax, ymin, ymax;
  int xtick, ytick;
  chart->getXminXmaxXTick(&xmin, &xmax, &xtick);
  chart->getYminYmaxYTick(&ymin, &ymax, &ytick);

  PlotSettingsDialog psettings(xmin, xmax, xtick, ymin, ymax, ytick);
  if(psettings.exec() == QDialog::Accepted){
    int titlesize = psettings.getPlotTitleSize();
    int axisvaluesize = psettings.getAxisValueSize();

    int xlabelsize = psettings.getXlabelSize();
    double xmin = psettings.getXmin();
    double xmax = psettings.getXmax();
    int xtick = psettings.getXTick();

    int ylabelsize = psettings.getYlabelSize();
    double ymin = psettings.getYmin();
    double ymax = psettings.getYmax();
    int ytick = psettings.getYTick();
    QString qdb = QString(" ScatterPlot::OpenPlotSettingsDialog title size %1 axis val size %2 xlbl size %3  ylbl size %4 xtick %5 ytick %6").arg(titlesize).arg(axisvaluesize).arg(xlabelsize).arg(ylabelsize).arg(xtick).arg(ytick);
    qDebug() << qdb;
    chart->setPlotTitleSize(titlesize);
    chart->setAxisValueSize(axisvaluesize);

    chart->setXLabelSize(xlabelsize);
    chart->setXminXmaxXTick(xmin, xmax, xtick);

    chart->setYLabelSize(ylabelsize);
    chart->setYminYmaxYTick(ymin, ymax, ytick);
    PlotUpdate();
  }
  else{
    return;
  }
}

void ScatterPlot::SaveSelection()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Selection to..."), "", tr("TXT (*.txt);;All Files (*)"));

  std::vector<std::string> namelst;
  for(int i = 0; i < selectedIDS.size(); i++){
    namelst.push_back(chart->getPoint(selectedIDS[i])->name().toUtf8().data());
  }

  DATAIO::WriteStringList(fileName.toUtf8().data(), namelst);
}


void ScatterPlot::SavePlotImage()
{
  QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot to Image"), "", tr("JPEG (*.jpg);;PNG (*.png);; PDF (*.pdf);;All Files (*)"));

  if(!fileName.isEmpty()){
    chart->SaveAsImage(fileName);
  }
}

void ScatterPlot::setOnOffHotellingEllipse()
{
  if(ehotel == true){ // Remove Hotelling
    int cid = -1;
    for(int i = 0; i < chart->getCurves().size(); i++){
      if(chart->getCurves()[i].name().compare("Hotelling ellipse 95% confidence") == 0){
        cid = i;
        break;
      }
      else{
        continue;
      }
    }
    if(cid > -1){
      chart->RemoveCurveAt(cid);
    }
    ehotel = false;
  }
  else{ // Add Hotelling
    BuildHotellingEllipse();
    ehotel = true;
  }
  PlotUpdate();
}

void ScatterPlot::DoClusterAnalysis()
{
  DoClusterAnalysisDialog docluster(mxlst, NULL, DoClusterAnalysisDialog::PlotSelection);
  if(docluster.exec() == QDialog::Accepted){

    uivector *clusterids;
    initUIVector(&clusterids);

    if(docluster.getDataType() == 0){
      matrix *coordinates;

      NewMatrix(&coordinates, p.size(), 2);


      for(int i = 0; i < chart->PointSize(); i++){
        coordinates->data[i][0] = chart->getPoint(i)->x();
        coordinates->data[i][1] = chart->getPoint(i)->y();
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
      obj.setObjectSelectionType(docluster.getExtractObjects());
      obj.setNMaxObjects(docluster.getNMaxObjects());
      obj.setNumberOfCluster(ncluster);

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

      QList<QColor> colors = GenColorList(ncluster);

      for(int i = 0; i < chart->PointSize(); i++){
        int cid = getUIVectorValue(clusterids, i)-1;
        if(cid > -1){
          chart->getPoint(i)->setColor(colors[cid]);
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

        for(int i = 0; i < chart->PointSize(); i++){
          int id = getUIVectorValue(clusterids, i)-1;
          if(id > -1){
            clusterobjlabels[id].objects.append(chart->getPoint(i)->name());
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
      m = (*mxlst)[dataid]->Matrix();

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


      QList<QColor> colors = GenColorList(ncluster);

      // Set Labels
      LABELS clusterobjlabels;
      if(docluster.SaveClusterLabels() == true){
        for(int i = 0; i < ncluster; i++){
          clusterobjlabels.append(LABEL());
          clusterobjlabels.last().name = QString("%1 - %2").arg(docluster.getClusterLabelSufix()).arg(QString::number(i+1));
        }
      }

      for(uint i = 0; i < clusterids->size; i++){
        int objid = -1;
        for(int j = 0; j < chart->PointSize(); j++){
          if((*mxlst)[dataid]->getObjName()[i].compare(chart->getPoint(j)->name()) == 0){
            objid = j;
            break;
          }
          else{
            continue;
          }
        }

        if(objid > -1){
          int cid = getUIVectorValue(clusterids, i)-1;
          if(cid > -1){
            chart->getPoint(objid)->setColor(colors[cid]);
            if(docluster.SaveClusterLabels() == true){
              clusterobjlabels[cid].objects.append(chart->getPoint(objid)->name());
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

      if(docluster.SaveClusterLabels() == true){
        objlabels->append(clusterobjlabels);
      }

      StopRun();
    }
    DelUIVector(&clusterids);
  }
  PlotUpdate();
}

void ScatterPlot::SetSelectionName()
{
  spi.pid = pid;
  spi.imgname.clear();
  for(int i = 0; i < chart->PointSize(); i++){
    if(chart->getPoint(i)->isVisible() == true
      && chart->getPoint(i)->isSelected() == true){
      spi.imgname.append(chart->getPoint(i)->name());
    }
    else{
      continue;
    }
  }
  emit ScatterPlotImageSignalChanged(spi);
//   clearSelection(); is not necessary...
  PlotUpdate();
}

void ScatterPlot::CloseFindCorrelationWidget()
{
  cwidget = 0;
}

void ScatterPlot::HighlightObjects(highlightSignal hhs)
{
  for(int i = 0; i < hhs.vid1.size(); i++){
    if(hhs.vid1[i] < chart->PointSize() && hhs.vid1[i] > -1)
      chart->getPoint(hhs.vid1[i])->setSelection(true);
    if(hhs.vid2[i] < chart->PointSize() && hhs.vid2[i] > -1)
      chart->getPoint(hhs.vid2[i])->setSelection(true);
  }
  PlotUpdate();
}


void ScatterPlot::EmitVVPlotSignal(vvplotSignal vvp)
{
  emit ScatterPlotVVPlotSignal(vvp);
}

void ScatterPlot::FindCorrelations()
{
  // Angle betwee the origin and two points...

  matrix *correl, *disterr, *coordinates;
  QStringList varnames;

  NewMatrix(&coordinates, chart->PointSize(), 2);
  for(int i = 0; i < chart->PointSize(); i++){
    coordinates->data[i][0] = chart->getPoint(i)->x();
    coordinates->data[i][1] = chart->getPoint(i)->y();
    varnames.append(chart->getPoint(i)->name());
  }

  // Now calculating the angle with the following formula:
  // angle = acos(AC + BC - AB) / (2*AC*BC) * 180 / 3.14
  // AC = distance first point from center
  // BC = distance second point from center
  // AB = distance from the first point and the second point

  NewMatrix(&correl, chart->PointSize(), chart->PointSize());
  NewMatrix(&disterr, chart->PointSize(), chart->PointSize());
  for(uint i = 0; i < coordinates->row; i++){
    double p1c = sqrt(square(coordinates->data[i][0]) + square(coordinates->data[i][1]));
    double p2c = 0.f, p1p2 = 0.f;
    correl->data[i][i] = disterr->data[i][i] = 0; // the distance between it self is 0
    for(uint j = i+1; j < coordinates->row; j++){
      p2c = sqrt(square(coordinates->data[j][0]) + square(coordinates->data[j][1]));
      p1p2 = sqrt(square(coordinates->data[i][0] - coordinates->data[j][0]) + square(coordinates->data[i][1] - coordinates->data[j][1]));
      correl->data[i][j] = correl->data[j][i] = (acos((square(p1c) + square(p2c) - square(p1p2))/(2*p1c*p2c))) * 180 / 3.1415926535;
      disterr->data[i][j] = disterr->data[j][i] = sqrt(square(p1c - p2c));
    }
  }

  if(cwidget != NULL)
    delete cwidget;

  cwidget = new FindCorrelationWidget(correl, disterr, &varnames, varlabels, pid, mid, mtype);

  connect(cwidget, SIGNAL(CloseFindCorrelationWidgetSignal()), SLOT(CloseFindCorrelationWidget()));

  if(pid > -1 && mid > -1)
    connect(cwidget, SIGNAL(VVPlotSignal(vvplotSignal)), SLOT(EmitVVPlotSignal(vvplotSignal)));

  connect(cwidget, SIGNAL(HighlightSignal(highlightSignal)), SLOT(HighlightObjects(highlightSignal)));

  cwidget->show();

  DelMatrix(&correl);
  DelMatrix(&disterr);
  DelMatrix(&coordinates);
}

void ScatterPlot::ShowContextMenu(const QPoint& pos)
{
  getPointSelected();
  /*QPoint globalPos = ui.plotwidget->mapToGlobal(pos);*/
  QPoint globalPos = chart->weview()->mapToGlobal(pos);
  QMenu menu;

  if(type == SCORES){
    menu.addAction("&Select All", this, SLOT(SelectAll()));
    menu.addAction("&Select by...", this, SLOT(SelectBy()));
    menu.addAction("&Invert Selection", this, SLOT(invertSelection()));
    menu.addAction("&Hide Selection", this, SLOT(hideSelection()));
    menu.addAction("&Clear Selection", this, SLOT(clearSelection()));

    /*if(selectedIDS.size() == 1){*/
    menu.addAction("&Show Image Selection", this, SLOT(SetSelectionName()));
    menu.addAction("&Show selected point labels", this, SLOT(showPointLabels()));
    menu.addAction("&Hide selected point labels", this, SLOT(hidePointLabels()));
    /*}*/

    menu.addSeparator();
    menu.addAction("&Set Selection Style", this, SLOT(setSelectionStyle()));

    if(objlabels != 0){
      menu.addAction("&Set Selection Label", this, SLOT(SetSelectionObjLabels()));
    }

    menu.addSeparator();
    menu.addAction("&Cluster Analysis", this, SLOT(DoClusterAnalysis()));
    menu.addSeparator();

    if(ehotel == true)
      menu.addAction("&Set OFF Hotelling Confidence Ellipse", this, SLOT(setOnOffHotellingEllipse()));
    else
      menu.addAction("&Set ON Hotelling Confidence Ellipse", this, SLOT(setOnOffHotellingEllipse()));
  }
  else if(type == LOADINGS){
    menu.addAction("&Select All", this, SLOT(SelectAll()));
    menu.addAction("&Select by...", this, SLOT(SelectByVarlabels()));
    menu.addAction("&Invert Selection", this, SLOT(invertSelection()));
    menu.addAction("&Hide Selection", this, SLOT(hideSelection()));
    menu.addAction("&Clear Selection", this, SLOT(clearSelection()));
    menu.addAction("&Set Selection Style", this, SLOT(setSelectionStyle()));
    menu.addAction("&Show selected point labels", this, SLOT(showPointLabels()));
    menu.addAction("&Hide selected point labels", this, SLOT(hidePointLabels()));

    if(varlabels != 0){
      menu.addAction("&Add Variable Label to Selected", this, SLOT(SetSelectionVarLabels()));
    }

    if(selectedIDS.size() == 1){
      menu.addAction("&Show Image Selection", this, SLOT(SetSelectionName()));
    }

    if(pid > -1 && mid > -1 && (mtype == PCA_ || mtype == PLS_)){
      menu.addSeparator();
      menu.addAction("&Find Correlations", this, SLOT(FindCorrelations()));
    }
  }

  menu.addSeparator();
  menu.addAction("&Save Plot Image", this, SLOT(SavePlotImage()));
  menu.addAction("&Save Selection", this, SLOT(SaveSelection()));

  if(selectedIDS.size() > 0){
    menu.actions().last()->setEnabled(true);
  }
  else{
    menu.actions().last()->setEnabled(false);
  }

  menu.addSeparator();
  menu.addAction("&Plot Settings", this, SLOT(OpenPlotSettingsDialog()));

  menu.addAction("&Reset Plot", this, SLOT(ResetPlot()));
  menu.exec(globalPos);
}


void ScatterPlot::getPointSelected()
{
  selectedIDS.clear();
  for(int i = 0; i < chart->PointSize(); i++){
    if(chart->getPoint(i)->isSelected() == true){
      selectedIDS.append(i);
    }
    else{
      continue;
    }
  }
}

void ScatterPlot::AbortRun()
{
  abort = true;
  ui.abortButton->setEnabled(false);
}

void ScatterPlot::ResetPlot()
{
  selectedIDS.clear();

  for(int i = 0; i < p.size(); i++){
    chart->getPoint(i)->setColor(p[i].color);
    if(axistype == SINGLEAXIS || axistype == DOUBLEAXIS){
      chart->getPoint(i)->setX(p[i].coord[0]);
      chart->getPoint(i)->setY(p[i].coord[1]);
    }
    else{
      chart->getPoint(i)->setX(p[i].coord[0]);
      chart->getPoint(i)->setY(p[i].coord[1]);
      chart->getPoint(i)->setZ(p[i].coord[2]);
    }
    chart->getPoint(i)->setRadius(p[i].radius);
    chart->getPoint(i)->setVisible(true);
    chart->getPoint(i)->setSelection(false);
  }

  if(axistype == SINGLEAXIS || axistype == DOUBLEAXIS){
    ui.axis3->setValue(ui.axis3->minimum());
    ui.axis1->setValue(ui.axis1->minimum());
    ui.axis2->setValue(ui.axis1->value()+1);
  }
  else{
    ui.axis1->setValue(ui.axis1->minimum());
    ui.axis2->setValue(ui.axis1->value()+1);
    ui.axis3->setValue(ui.axis2->value()+1);
  }

  UpdatePointPosition();
  PlotUpdate();
}

void ScatterPlot::PlotUpdate()
{
  chart->Refresh();
  //chart->update();
}

void ScatterPlot::UpdatePointPosition()
{
  int x, y, z = -1;
  if(axistype == SINGLEAXIS){
    x = 0 + (2*ui.axis3->value()-2);
    y = 1 + (2*ui.axis3->value()-2);
  }
  else if (axistype == DOUBLEAXIS){
    x = ui.axis1->value()-1;
    y = ui.axis2->value()-1;
  }
  else{
    x = ui.axis1->value()-1;
    y = ui.axis2->value()-1;
    z = ui.axis3->value()-1;
  }

  for(int i = 0; i < chart->PointSize(); i++){
    chart->getPoint(i)->setX(p[i].coord[x]);
    chart->getPoint(i)->setY(p[i].coord[y]);
    if(z > -1){
      chart->getPoint(i)->setZ(p[i].coord[z]);
    }
  }

  if(axistype == SINGLEAXIS){
    if(acoeff.size() > 0 && ui.axis3->value()-1 < acoeff.size()){
      BuildLine(acoeff[ui.axis3->value()-1]);
    }
  }

  UpdateAxisName();

  if(curves.size() > 0){
    if(acoeff.size() == 0){
      chart->RemoveAllCurves();
    }
    // else is already done
    for(int k = 0; k < curves.size(); k++){
      QVector< QPointF > line;
      for(uint i = 0; i < curves[k]->row; i++){
        line.append(QPointF(curves[k]->data[i][x], curves[k]->data[i][y]));
      }
      chart->addCurve(line, curvenames[k], curvecolors[k]);
    }
  }

  if(ehotel == true){
    BuildHotellingEllipse();
  }

  PlotUpdate();
}

void ScatterPlot::slotExit()
{
  qApp->exit();
}

void ScatterPlot::setHotellingConfidenceEllipse(bool ehotel_){
  ehotel = ehotel_;
  if(ehotel == true)
    BuildHotellingEllipse();
  else
    return;
}

void ScatterPlot::setAxisNameExtensions(QStringList nameaxisext_)
{
  nameaxisext.append(nameaxisext_);
  UpdateAxisName();
}

void ScatterPlot::SetAutoNameAxes(bool autonameaxes_)
{
  autonameaxes = autonameaxes_;
  ui.axis1_name->hide();
  ui.axis1->hide();
  ui.axis2_name->hide();
  ui.axis2->hide();
  ui.axis3_name->hide();
  ui.axis3->hide();
}

void ScatterPlot::addCurve(QList< matrix* > curves_, QStringList curvenames_, QList< QColor > curvecolors_)
{
  for(int i = 0; i < curves_.size(); i++){
    curves.append(new matrix);
    initMatrix(&curves.last());
    MatrixCopy(curves_.at(i), &curves.last());
  }
  curvenames.append(curvenames_);
  curvecolors.append(curvecolors_);
  UpdatePointPosition();
}

void ScatterPlot::removeAllCurves()
{
  for(int i = 0; i < curves.size(); i++){
    DelMatrix(&curves[i]);
  }
  curves.clear();
  curvenames.clear();
  curvecolors.clear();
}

void ScatterPlot::BuildDiagonal()
{
  // bisection in points
  for(int i = 0; i < p.first().coord.size()/2; i++)
    acoeff.append(1);

  BuildLine(acoeff.first());
  PlotUpdate();
}

ScatterPlot::ScatterPlot(QList<matrix*> &m_, QList<QStringList>& objname, QString xaxsisname_, QString yaxsisname_, QString windowtitle)
{
  qDebug() << "ScatterPlot F1";
  ui.setupUi(this);
  type = SIMPLE;
  pid = mid = mtype -1;
  cwidget = 0;
  ehotel = false;
  markersymbls << "Circle" << "Square" << "Triangle";
//       chart.setWindowTitle(QObject::tr("chart"));
//     PlotFromfile(&chart, argv[1]);

  //chart = new Chart();
  ////chart = new QPlotlyWindow(this);

  axistype = DOUBLEAXIS;

  chart = NewGraph(PEngine::Qtchart);
  chart->setPlotTitle(windowtitle);
  ui.plotlayout->addWidget(chart->weview());

  ui.axis3_name->hide();
  ui.axis3->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();



  ui.axis1_name->setText(xaxsisname_);
  ui.axis2_name->setText(yaxsisname_);

  mxlst = 0;
  objlabels = varlabels = 0;
  vartablabels = 0;

  autonameaxes = true;
  xaxisname = xaxsisname_;
  yaxisname = yaxsisname_;

  initPoint(m_, objname);

  setWindowTitle(windowtitle);

  ui.axis1->setValue(1);
  ui.axis2->setValue(2);

  //chek for the max of col that could be plot:
  int maxcol = p[0].coord.size();
  for(int i = 1; i < p.size(); i++){
    if(p[i].coord.size() < maxcol){
      maxcol = p[i].coord.size();
    }
    else{
      continue;
    }
  }

  ui.axis1->setMaximum(maxcol);
  ui.axis2->setMaximum(maxcol);

  //Finally render the scene
  chart->Center();
  chart->Plot();

  // Set up action signals and slots
  connect(ui.axis1, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
  connect(ui.axis2, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));

  connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));

  chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
}

ScatterPlot::ScatterPlot(QList<matrix*> &m_, QList<QStringList>& objname, QList<MATRIX*> *mxlst_,  QStringList xhash_, QStringList yhash_, LABELS *objlabels_, LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_)
{
  qDebug() << "ScatterPlot F2";
  ui.setupUi(this);
  type = type_;
  pid = mid = mtype = -1;
  cwidget = 0;
  ehotel = false;
  markersymbls << "Circle" << "Square" << "Triangle";

  //chart = new Chart();
  //chart = new QPlotlyWindow(this);
  axistype = DOUBLEAXIS;
  chart = NewGraph(PEngine::Qtchart);
  chart->setPlotTitle(windowtitle);
  ui.plotlayout->addWidget(chart->weview());

  ui.axis3_name->hide();
  ui.axis3->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();



  ui.axis1_name->setText(xaxsisname_);
  ui.axis2_name->setText(yaxsisname_);
  ui.axis1->setValue(1);
  ui.axis2->setValue(2);

  autonameaxes = true;
  xaxisname = xaxsisname_;
  yaxisname = yaxsisname_;

  initPoint(m_, objname);

  setWindowTitle(windowtitle);

  // used in selection by..
  mxlst = mxlst_;
  xhash = xhash_;
  yhash = yhash_;
  objlabels = objlabels_;
  varlabels = varlabels_;
  vartablabels = 0;


  //chek for the max of col that could be plot:
  int maxcol = p[0].coord.size();
  for(int i = 1; i < p.size(); i++){
    if(p[i].coord.size() < maxcol){
      maxcol = p[i].coord.size();
    }
    else{
      continue;
    }
  }

  ui.axis1->setMaximum(maxcol);
  ui.axis2->setMaximum(maxcol);

  //Finally render the scene
  chart->Center();
  chart->Plot();
  // Set up action signals and slots
  connect(ui.axis1, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
  connect(ui.axis2, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
  connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
}

ScatterPlot::ScatterPlot(QList<matrix*> &m_, QList<QStringList>& objname,
                         QList<MATRIX*> *mxlst_,  QStringList xhash_, QStringList yhash_,
                         LABELS *objlabels_, LABELS *varlabels_,
                         QString xaxsisname_, QString yaxsisname_, QString zaxsisname_,
                         QString windowtitle, int type_)
{
  qDebug() << "ScatterPlot F3";
  ui.setupUi(this);
  type = type_;
  pid = mid = mtype = -1;
  cwidget = 0;
  ehotel = false;
  markersymbls << "Circle" << "Square" << "Cross";
  //chart = new Chart();
  //chart = new QPlotlyWindow(this);
  axistype = TRIPLEAXIS;
  chart = NewGraph(PEngine::QPlotly);
  chart->setPlotTitle(windowtitle);
  ui.plotlayout->addWidget(chart->weview());

  ui.progressBar->hide();
  ui.abortButton->hide();


  ui.axis1_name->setText(xaxsisname_);
  ui.axis2_name->setText(yaxsisname_);
  ui.axis3_name->setText(zaxsisname_);

  autonameaxes = true;
  xaxisname = xaxsisname_;
  yaxisname = yaxsisname_;
  zaxisname = zaxsisname_;

  setWindowTitle(windowtitle);

  // used in selection by..
  mxlst = mxlst_;
  xhash = xhash_;
  yhash = yhash_;
  objlabels = objlabels_;
  varlabels = varlabels_;
  vartablabels = 0;

  ui.axis1->setValue(1);
  ui.axis2->setValue(2);
  ui.axis3->setValue(3);
  initPoint(m_, objname);

  //chek for the max of col that could be plot:
  int maxcol = p[0].coord.size();
  for(int i = 1; i < p.size(); i++){
    if(p[i].coord.size() < maxcol){
      maxcol = p[i].coord.size();
    }
    else{
      continue;
    }
  }

  ui.axis1->setMaximum(maxcol);
  ui.axis2->setMaximum(maxcol);
  ui.axis3->setMaximum(maxcol);

  //Finally render the scene
  chart->Center();
  //chart->Refresh();
  chart->Plot();
  // Set up action signals and slots
  connect(ui.axis1, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
  connect(ui.axis2, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
  connect(ui.axis3, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
  connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  this->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(this, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
}


ScatterPlot::ScatterPlot(QList<matrix*> &m_, QList<QStringList>& objname, QList<MATRIX*> *mxlst_, QStringList xhash_, QStringList yhash_, TABLABELS *vartablabels_, LABELS *objlabels_, LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_)
{
  ui.setupUi(this);
  type = type_;
  pid = mid = mtype = -1;
  cwidget = 0;
  ehotel = false;
  markersymbls << "Circle" << "Square" << "Triangle";
  //chart = new Chart();
  //chart = new QPlotlyWindow(this);
  axistype = DOUBLEAXIS;
  chart = NewGraph(PEngine::Qtchart);
  chart->setPlotTitle(windowtitle);
  ui.plotlayout->addWidget(chart->weview());

  ui.axis3_name->hide();
  ui.axis3->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();



  ui.axis1_name->setText(xaxsisname_);
  ui.axis2_name->setText(yaxsisname_);

  autonameaxes = true;
  xaxisname = xaxsisname_;
  yaxisname = yaxsisname_;
  initPoint(m_, objname);

  setWindowTitle(windowtitle);

  // used in selection by..
  mxlst = mxlst_;
  xhash = xhash_;
  yhash = yhash_;
  objlabels = objlabels_;
  varlabels = varlabels_;
  vartablabels = vartablabels_;

  //chek for the max of col that could be plot:
  int maxcol = p[0].coord.size();
  for(int i = 1; i < p.size(); i++){
    if(p[i].coord.size() < maxcol){
      maxcol = p[i].coord.size();
    }
    else{
      continue;
    }
  }

  ui.axis1->setMaximum(maxcol);
  ui.axis2->setMaximum(maxcol);

  //Finally render the scene
  chart->Center();
  chart->Plot();

  // Set up action signals and slots
  connect(ui.axis1, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
  connect(ui.axis2, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
  connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
  chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
}

ScatterPlot::ScatterPlot(QList<matrix*> &mx_, QList<matrix*> &my_, dvector* b_, QList<QStringList>& objname,  QString xaxsisname_, QString yaxsisname_, QString windowtitle)
{
  ui.setupUi(this);
  type = SIMPLE;
  pid = mid = mtype = -1;
  cwidget = 0;
  ehotel = false;
  markersymbls << "Circle" << "Square" << "Triangle";
  //chart = new Chart();
  //chart = new QPlotlyWindow(this);
  axistype = SINGLEAXIS;
  chart = NewGraph(PEngine::Qtchart);
  ui.plotlayout->addWidget(chart->weview());

  ui.axis1->hide();
  ui.axis2->hide();
  ui.axis1_name->hide();
  ui.axis2_name->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();



  ui.axis1_name->setText(QString("%1 / %2").arg(xaxsisname_).arg(yaxsisname_));

  mxlst = 0;

  // Copy data into local plot
  if(mx_.size() == my_.size()){
    autonameaxes = true;
    xaxisname = xaxsisname_;
    yaxisname = yaxsisname_;
    initPoint(mx_, my_, objname);

    setWindowTitle(windowtitle);

    //chek for the max of col that could be plot:
    int maxcol = p[0].coord.size();
    for(int i = 1; i < p.size(); i++){
      if(p[i].coord.size() < maxcol){
        maxcol = p[i].coord.size();
      }
      else{
        continue;
      }
    }

    maxcol /= 2;
    ui.axis3->setMaximum(maxcol);

    for(uint i = 0; i < b_->size; i++)
      acoeff.append(b_->data[i]);

    BuildLine(acoeff.first());

    //Finally render the scene
    chart->Center();
    chart->Plot();

    // Set up action signals and slots
    connect(ui.axis3, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
    connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
    chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
  }
  else{
    QMessageBox::warning(this, tr("ScatterPlot Error"), tr("Unable to make plot\n Different row/col in mx and my. Please check your data."), QMessageBox::Ok);
  }
}

ScatterPlot::ScatterPlot(QList<matrix*>& mx_, QList<matrix*>& my_, dvector* b_, QList< QStringList >& objname, QList<MATRIX*>* mxlst_,  QStringList xhash_, QStringList yhash_, LABELS* objlabels_, LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_)
{
  ui.setupUi(this);
  type = type_;
  pid = mid = mtype = -1;
  cwidget = 0;
  ehotel = false;
  markersymbls << "Circle" << "Square" << "Triangle";
  //chart = new Chart();
  //chart = new QPlotlyWindow(this);
  axistype = SINGLEAXIS;
  chart = NewGraph(PEngine::Qtchart);
  chart->setPlotTitle(windowtitle);
  ui.plotlayout->addWidget(chart->weview());

  ui.axis1->hide();
  ui.axis2->hide();
  ui.axis1_name->hide();
  ui.axis2_name->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();



  ui.axis3_name->setText(QString("%1 / %2").arg(xaxsisname_).arg(yaxsisname_));

  objlabels = objlabels_;
  varlabels = varlabels_;
  vartablabels = 0;

  // Copy data into local plot
  if(mx_.size() == my_.size()){
    mxlst = mxlst_;
    xhash = xhash_;
    yhash = yhash_;

    autonameaxes = true;
    xaxisname = xaxsisname_;
    yaxisname = yaxsisname_;
    initPoint(mx_, my_, objname);

    setWindowTitle(windowtitle);

    //chek for the max of col that could be plot:
    int maxcol = p[0].coord.size();
    for(int i = 1; i < p.size(); i++){
      if(p[i].coord.size() < maxcol){
        maxcol = p[i].coord.size();
      }
      else{
        continue;
      }
    }

    maxcol /= 2; // because is x an y ensemble
    ui.axis3->setMaximum(maxcol);

    for(uint i = 0; i < b_->size; i++)
      acoeff.append(b_->data[i]);

    BuildLine(acoeff.first());

    //Finally render the scene
    chart->Center();
    chart->Plot();

    // Set up action signals and slots
    connect(ui.axis3, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
    connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
    /*chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
    */
    chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
  }
  else{
    QMessageBox::warning(this, tr("ScatterPlot Error"), tr("Unable to make plot\n Different row/col in mx and my. Please check your data."), QMessageBox::Ok);
  }
}

ScatterPlot::ScatterPlot(QList< matrix* >& mx_, QList< matrix* >& my_, QList< QStringList >& objname, QList< MATRIX* >* mxlst_,  QStringList xhash_, QStringList yhash_, LABELS* objlabels_, LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_)
{
  ui.setupUi(this);
  type = type_;
  pid = mid = mtype = -1;
  cwidget = 0;
  ehotel = false;
  markersymbls << "Circle" << "Square" << "Triangle";
  //chart = new Chart();
  //chart = new QPlotlyWindow(this);
  axistype = SINGLEAXIS;
  chart = NewGraph(PEngine::Qtchart);
  chart->setPlotTitle(windowtitle);
  ui.plotlayout->addWidget(chart->weview());

  ui.axis1->hide();
  ui.axis2->hide();
  ui.axis1_name->hide();
  ui.axis2_name->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();



  ui.axis3_name->setText(QString("%1 / %2").arg(xaxsisname_).arg(yaxsisname_));

  objlabels = objlabels_;
  varlabels = varlabels_;
  vartablabels = 0;


  // Copy data into local plot
  if(mx_.size() == my_.size()){
    mxlst = mxlst_;
    xhash = xhash_;
    yhash = yhash_;

    autonameaxes = true;
    xaxisname = xaxsisname_;
    yaxisname = yaxsisname_;
    initPoint(mx_, my_, objname);

    setWindowTitle(windowtitle);

    //chek for the max of col that could be plot:
    int maxcol = p[0].coord.size();
    for(int i = 1; i < p.size(); i++){
      if(p[i].coord.size() < maxcol){
        maxcol = p[i].coord.size();
      }
      else{
        continue;
      }
    }

    maxcol /= 2;
    ui.axis3->setMaximum(maxcol);

    //Finally render the scene
    chart->Center();
    chart->Plot();

    // Set up action signals and slots
    connect(ui.axis3, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
    connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
    chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
  }
  else{
    QMessageBox::warning(this, tr("ScatterPlot Error"), tr("Unable to make plot\n Different row/col in mx and my. Please check your data."), QMessageBox::Ok);
  }
}

ScatterPlot::ScatterPlot(QList< matrix* >& mx_, QList< matrix* >& my_, QList< QStringList >& objname, QList< MATRIX* >* mxlst_,  QStringList xhash_, QStringList yhash_, TABLABELS *vartablabels_, LABELS* objlabels_, LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_)
{
  ui.setupUi(this);
  type = type_;
  pid = mid = mtype = -1;
  cwidget = 0;
  ehotel = false;
  markersymbls << "Circle" << "Square" << "Triangle";
  //chart = new Chart();
  //chart = new QPlotlyWindow(this);
  axistype = SINGLEAXIS;
  chart = NewGraph(PEngine::Qtchart);
  chart->setPlotTitle(windowtitle);
  ui.plotlayout->addWidget(chart->weview());

  ui.axis1->hide();
  ui.axis2->hide();
  ui.axis1_name->hide();
  ui.axis2_name->hide();
  ui.progressBar->hide();
  ui.abortButton->hide();

  ui.axis3_name->setText(QString("%1 / %2").arg(xaxsisname_).arg(yaxsisname_));

  objlabels = objlabels_;
  varlabels = varlabels_;
  vartablabels = vartablabels_;

  // Copy data into local plot
  if(mx_.size() == my_.size()){
    mxlst = mxlst_;
    xhash = xhash_;
    yhash = yhash_;

    autonameaxes = true;
    xaxisname = xaxsisname_;
    yaxisname = yaxsisname_;
    initPoint(mx_, my_, objname);

    setWindowTitle(windowtitle);

    //chek for the max of col that could be plot:
    int maxcol = p[0].coord.size();
    for(int i = 1; i < p.size(); i++){
      if(p[i].coord.size() < maxcol){
        maxcol = p[i].coord.size();
      }
      else{
        continue;
      }
    }

    maxcol /= 2;

    ui.axis3->setMaximum(maxcol);

    //Finally render the scene
    chart->Center();
    chart->Plot();

    // Set up action signals and slots
    connect(ui.axis3, SIGNAL(valueChanged(int)), SLOT(UpdatePointPosition()));
    connect(ui.abortButton, SIGNAL(clicked(bool)), SLOT(AbortRun()));
    chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
  }
  else{
    QMessageBox::warning(this, tr("ScatterPlot Error"), tr("Unable to make plot\n Different row/col in mx and my. Please check your data."), QMessageBox::Ok);
  }
}

ScatterPlot::~ScatterPlot()
{
  delete chart;
  if(cwidget != 0)
    delete cwidget;
}
