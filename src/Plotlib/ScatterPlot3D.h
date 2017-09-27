#ifndef SCATTERPLOT3D_H
#define SCATTERPLOT3D_H
#include <scientific.h>
#include <QApplication>

#include <QWidget>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include "ui_ScatterPlot3D.h"
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

class MouseInteractorStylePP : public vtkInteractorStyleTrackballCamera
{
public:
  static MouseInteractorStylePP* New();
  vtkTypeMacro(MouseInteractorStylePP, vtkInteractorStyleTrackballCamera);

  virtual void OnLeftButtonDown() override final {
    picked[0] = picked[1] = picked[2] = -9999;
//     qDebug() << "Picking pixel: " << this->Interactor->GetEventPosition()[0] << " " << this->Interactor->GetEventPosition()[1];
    this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0],
                        this->Interactor->GetEventPosition()[1],
                        0,  // always zero.
                        this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());


    this->Interactor->GetPicker()->GetPickPosition(picked);
//     qDebug() << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2];
    // Forward events
    vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
  }

  double *getPicked(){ return picked; }
private:
  double picked[3];
};

struct QSMPOINT3DID{
  int id; //point number
  QString xhash;
  QString yhash;
};

class POINT3D
{
public:
  POINT3D(){
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
  int NumberOfPoints(){ return pointid_.size(); }

  QList<QSMPOINT3DID> &pointid(){ return pointid_; }
  QStringList &labels(){ return labels_;}
  vtkSmartPointer<vtkPoints> &points(){ return points_; }
  vtkSmartPointer<vtkUnsignedCharArray> &colors(){ return colors_; }
  vtkSmartPointer<vtkPolyData> &polydata(){ return polydata_; }
  vtkSmartPointer<vtkSphereSource> &sphereSource(){ return sphereSource_; }
  vtkSmartPointer<vtkGlyph3D> &glyph3D(){ return glyph3D_; }
  vtkSmartPointer<vtkPolyDataMapper> &mapper(){ return mapper_; }
  vtkSmartPointer<vtkActor> &actor(){ return actor_; }

private:
  QList<QSMPOINT3DID> pointid_;
  QStringList labels_;
  vtkSmartPointer<vtkPoints> points_;
  vtkSmartPointer<vtkUnsignedCharArray> colors_;
  vtkSmartPointer<vtkPolyData> polydata_;
  vtkSmartPointer<vtkSphereSource> sphereSource_;
  vtkSmartPointer<vtkGlyph3D> glyph3D_;
  vtkSmartPointer<vtkPolyDataMapper> mapper_;
  vtkSmartPointer<vtkActor> actor_;
};

class ScatterPlot3D: public QWidget
{
  Q_OBJECT

public:
  enum{
    SCORES = 0,
    LOADINGS = 1,
    OBJECTLABELS = 0,
    VARIABLELABELS = 1
  };

  ScatterPlot3D(matrix *m_, QStringList &objname, QString windowtitle, QString xname_, QString yname_, QString zname);
  ScatterPlot3D(QList<matrix*> m_, QList<QStringList> &objname, QString windowtitle, QString xname_, QString yname_, QString zname_);
  ScatterPlot3D(QList<matrix*> m_, QList<QStringList> &objname, QList<MATRIX*> *mxlst, QStringList xhash_, QStringList yhash, LABELS *objlabels_, LABELS *varlabels_, QString windowtitle, QString xname, QString yname, QString zname, int type_);
  ScatterPlot3D(QList<matrix*> m_, QList<QStringList> &objname, QList<ARRAY*> *arlst, QStringList xhash_, QStringList yhash, LABELS  *objlabels_, LABELS *varlabels_, QString windowtitle,  QString xname, QString yname, QString zname, int type_);
  ~ScatterPlot3D();

  void setPID(int pid_){ pid = pid_; }

public slots:
  void slotExit();
  void setPoints();
  void SetSymbolSize();
  void SetBackgroundColor();
  void SetOutlineColor();
  void PlotUpdate();
  void ResetPlot();
  void selectAll();
  void unselectAll();
  void invertSelection();
  void selectBy();
  void selectByVarLabels();
  void selectByObjLabels();
  void DoClusterAnalysis();
  void UpdateSelectedObjects();
  void setSelectionStyle();
  void setSelectionObjLabel();
  void setSelectionVarLabel();
  void SavePlotImage();
  void EnableDisableButton();
  void AbortRun();
  void UpdateSelectionLabel();

signals:
  void ScatterPlot3DImageSignalChanged(ImageSignal new_spi);

private:
  Ui::ScatterPlot3D ui;
  void setScalingFactor();
  QColor backgroundcolor, outlinecolor;
  double xAxismin, xAxismax, yAxismin, yAxismax, zAxismin, zAxismax, scaling;
  QStringList objnames;
  QList<matrix*> m;
  QStringList xhash, yhash;
  QList<MATRIX*> *mxlst;
  QList<ARRAY*> *arlst;
  LABELS *objlabels, *varlabels;
  int type, typedata, pid;
  POINT3D p;
  vtkSmartPointer<vtkOutlineFilter> outline;
  vtkSmartPointer<vtkPolyDataMapper> outlineMapper;
  vtkSmartPointer<vtkActor> outlineActor;
  vtkSmartPointer<vtkCubeAxesActor> cubeAxesActor;
  vtkSmartPointer<vtkRenderer> renderer;
  vtkSmartPointer<vtkPointPicker> pointPicker;
  vtkSmartPointer<MouseInteractorStylePP> style;
  vtkSmartPointer<vtkEventQtSlotConnect> Connections;

  ImageSignal spimgsign;

  QString xname, yname, zname;
  QColor makeColor(double val, double min, double max, QColor &color1, QColor &color2);

  int GetIDinmxlst(QString mxhash);
  int GetIDinarlst(QString arhash);

  bool abort;
  void StartRun();
  void StopRun();
};

#endif
