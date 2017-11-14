#ifndef SIMPLESCATTERPLOT3D_H
#define SIMPLESCATTERPLOT3D_H
#include <scientific.h>
#include <QApplication>
#include <QWidget>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include "ui_SimpleScatterPlot3D.h"
#include "qsmdata.h"

#include <vtkVersion.h>
#include <vtkProperty.h>
#include <vtkCellArray.h>
#include <vtkUnsignedCharArray.h>
#include <vtkStringArray.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkPoints.h>
#include <vtkGlyph3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkLabeledDataMapper.h>
#include <vtkCubeAxesActor.h>
#include <vtkOutlineFilter.h>
#include <vtkPointSource.h>
#include <vtkIntArray.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkPointData.h>
#include <vtkPointSetToLabelHierarchy.h>


#include <vtkRendererCollection.h>
#include <vtkPointPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkObjectFactory.h>
#include <vtkEventQtSlotConnect.h>

class SimplePOINT3D
{
public:
  SimplePOINT3D(){
    points_ = vtkSmartPointer<vtkPoints>::New();
    colors_ = vtkSmartPointer<vtkUnsignedCharArray>::New();
    polydata_ = vtkSmartPointer<vtkPolyData>::New();
    sphereSource_ = vtkSmartPointer<vtkSphereSource>::New();
    glyph3D_ = vtkSmartPointer<vtkGlyph3D>::New();
    mapper_ = vtkSmartPointer<vtkPolyDataMapper>::New();
    actor_ = vtkSmartPointer<vtkActor>::New();
  }
  void Update(){
    points_->Modified();
    colors_->Modified();
    sphereSource_->Modified();
    sphereSource_->Update();
    glyph3D_->Modified();
    glyph3D_->Update();
    mapper_->Modified();
    actor_->Modified();
  }
  QStringList &labels(){ return labels_;}
  vtkSmartPointer<vtkPoints> &points(){ return points_; }
  vtkSmartPointer<vtkUnsignedCharArray> &colors(){ return colors_; }
  vtkSmartPointer<vtkPolyData> &polydata(){ return polydata_; }
  vtkSmartPointer<vtkSphereSource> &sphereSource(){ return sphereSource_; }
  vtkSmartPointer<vtkGlyph3D> &glyph3D(){ return glyph3D_; }
  vtkSmartPointer<vtkPolyDataMapper> &mapper(){ return mapper_; }
  vtkSmartPointer<vtkActor> &actor(){ return actor_; }

private:
  QStringList labels_;
  vtkSmartPointer<vtkPoints> points_;
  vtkSmartPointer<vtkUnsignedCharArray> colors_;
  vtkSmartPointer<vtkPolyData> polydata_;
  vtkSmartPointer<vtkSphereSource> sphereSource_;
  vtkSmartPointer<vtkGlyph3D> glyph3D_;
  vtkSmartPointer<vtkPolyDataMapper> mapper_;
  vtkSmartPointer<vtkActor> actor_;
};

class SimpleScatterPlot3D: public QWidget
{
  Q_OBJECT

public:
  /*
   * m matrix must be of type x;y;z
   * colorvalue must have the same number of row
   */
  SimpleScatterPlot3D(matrix *m_, QString windowtitle, QString xname_, QString yname_, QString zname);
  SimpleScatterPlot3D(matrix *m_, QList<double> colorvalue, QString windowtitle, QString xname_, QString yname_, QString zname);
  ~SimpleScatterPlot3D();
  void setPID(int pid_){ pid = pid_; }

public slots:
  void slotExit();
  void PlotUpdate();
  void SavePlotImage();
  void SetSymbolSize();

private:
  Ui::SimpleScatterPlot3D ui;
  void setScalingFactor();
  QColor backgroundcolor, outlinecolor;
  double xAxismin, xAxismax, yAxismin, yAxismax, zAxismin, zAxismax, scaling;
  matrix *m;
  int typedata, pid;
  SimplePOINT3D p;
  vtkSmartPointer<vtkOutlineFilter> outline;
  vtkSmartPointer<vtkPolyDataMapper> outlineMapper;
  vtkSmartPointer<vtkActor> outlineActor;
  vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor;
  vtkSmartPointer<vtkRenderer> renderer;

  QString xname, yname, zname;
  QColor makeColor(double val, double min, double max, QColor &color1, QColor &color2);
};

#endif
