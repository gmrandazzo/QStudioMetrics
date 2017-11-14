#include "SimpleScatterPlot3D.h"
#include <scientific.h>
#include <QApplication>
#include <QStringListModel>
#include <QColorDialog>
#include <QFileDialog>

#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>
#include <vtkPNGWriter.h>

#include <QList>
#include <QMessageBox>


// For compatibility with new VTK generic data arrays
#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif


QColor SimpleScatterPlot3D::makeColor(double val, double min, double max, QColor &color1, QColor &color2)
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

void SimpleScatterPlot3D::SetSymbolSize()
{
  p.sphereSource()->SetRadius(ui.actionSymbolSize->value() * scaling);
  PlotUpdate();
}

void SimpleScatterPlot3D::SavePlotImage()
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

void SimpleScatterPlot3D::PlotUpdate()
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

void SimpleScatterPlot3D::setScalingFactor()
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

  MatrixColumnMinMax(m, 0, &xmin, &xmax);
  MatrixColumnMinMax(m, 1, &ymin, &ymax);
  MatrixColumnMinMax(m, 2, &zmin, &zmax);

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

void SimpleScatterPlot3D::slotExit()
{
  qApp->exit();
}

SimpleScatterPlot3D::SimpleScatterPlot3D(matrix* m_, QString windowtitle, QString xname_, QString yname_, QString zname_)
{
  ui.setupUi(this);
  pid = -1;

  xname = xname_;
  yname = yname_;
  zname = zname_;

  outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outlineActor = vtkSmartPointer<vtkActor>::New();
  cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
  renderer = vtkSmartPointer<vtkRenderer>::New();

  setWindowTitle(windowtitle);

  // deep copy
  initMatrix(&m);
  MatrixCopy(m_, &m);

  // Set Points and Labels
  p.points()->SetNumberOfPoints(m->row);
  setScalingFactor();
  for(uint i = 0; i < m->row; i++){
    p.points()->SetPoint(i, m->data[i][0], m->data[i][1], m->data[i][2]);
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

  //SET SINGLE COLOR WHITE
  p.colors()->SetName("colors");
  p.colors()->SetNumberOfComponents(3); // 3 because is a tuple of red green and blue
//   p.colors()->SetNumberOfValues(npoints);
  QColor symbolcolor = QColor((Qt::GlobalColor)QtColours[0]);
  unsigned char c[3] = {255, 255, 255};
  c[0] = symbolcolor.red();
  c[1] = symbolcolor.green();
  c[2] = symbolcolor.blue();
  for(uint i = 0; i < m->row; i++){
    p.colors()->InsertNextTupleValue(c);
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
  outlinecolor = QColor((Qt::GlobalColor)QtColours[0]);
  outlineActor->GetProperty()->SetColor(outlinecolor.redF(),outlinecolor.greenF(),outlinecolor.blueF());

  cubeAxesActor->SetBounds(outlineMapper->GetBounds());
  cubeAxesActor->SetCamera(renderer->GetActiveCamera());

  cubeAxesActor->XAxisMinorTickVisibilityOff();
  cubeAxesActor->YAxisMinorTickVisibilityOff();
  cubeAxesActor->ZAxisMinorTickVisibilityOff();

  cubeAxesActor->DrawXGridlinesOn();
  cubeAxesActor->DrawYGridlinesOn();
  cubeAxesActor->DrawZGridlinesOn();
#if VTK_MAJOR_VERSION > 5
  cubeAxesActor->SetGridLineLocation(vtkCubeAxesActor::vtkCubeAxesActor::VTK_GRID_LINES_FURTHEST);
#endif

  cubeAxesActor->SetXTitle(QString("%1").arg(xname).toUtf8().constData());
  cubeAxesActor->SetYTitle(QString("%1").arg(yname).toUtf8().constData());
  cubeAxesActor->SetZTitle(QString("%1").arg(zname).toUtf8().constData());

  renderer->AddActor(p.actor());
  renderer->AddActor(cubeAxesActor);
  renderer->AddActor(outlineActor);

  backgroundcolor = QColor(Qt::black);
  renderer->SetBackground(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF());

  renderer->ResetCamera();

  ui.qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

  connect(ui.actionSymbolSize, SIGNAL(valueChanged(double)), SLOT(SetSymbolSize()));
  connect(ui.saveImageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
//   connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}


SimpleScatterPlot3D::SimpleScatterPlot3D(matrix* m_, QList<double> colorvalue, QString windowtitle, QString xname_, QString yname_, QString zname_)
{
  ui.setupUi(this);
  pid = -1;

  xname = xname_;
  yname = yname_;
  zname = zname_;

  outline = vtkSmartPointer<vtkOutlineFilter>::New();
  outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  outlineActor = vtkSmartPointer<vtkActor>::New();
  cubeAxesActor = vtkSmartPointer<vtkCubeAxesActor>::New();
  renderer = vtkSmartPointer<vtkRenderer>::New();

  setWindowTitle(windowtitle);

  // deep copy
  initMatrix(&m);
  MatrixCopy(m_, &m);

  // Set Points and Labels
  p.points()->SetNumberOfPoints(m->row);
  setScalingFactor();
  for(uint i = 0; i < m->row; i++){
    p.points()->SetPoint(i, m->data[i][0], m->data[i][1], m->data[i][2]);
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

  //SET SINGLE COLOR WHITE
  p.colors()->SetName("colors");
  p.colors()->SetNumberOfComponents(3); // 3 because is a tuple of red green and blue
//   p.colors()->SetNumberOfValues(npoints);
  QColor symbolcolor = QColor((Qt::GlobalColor)QtColours[0]);
  unsigned char c[3] = {255, 255, 255};
  c[0] = symbolcolor.red();
  c[1] = symbolcolor.green();
  c[2] = symbolcolor.blue();
  for(uint i = 0; i < m->row; i++){
    p.colors()->InsertNextTupleValue(c);
  }

  double min, max;
  min = max = colorvalue[0];
  for(int i = 1; i < colorvalue.size(); i++){
    if(colorvalue[i] < min)
      min = colorvalue[i];

    if(colorvalue[i] > max)
      max = colorvalue[i];

  }

  QColor mincolor = QColor((Qt::GlobalColor)QtColours[3]);
  QColor maxcolor = QColor((Qt::GlobalColor)QtColours[2]);
  for(uint i = 0; i < m->row; i++){
      QColor color = makeColor(colorvalue[i], min, max, mincolor, maxcolor);
      c[0] = (unsigned char)color.red();
      c[1] = (unsigned char)color.green();
      c[2] = (unsigned char)color.blue();
      p.colors()->SetTypedTuple(i, c);
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
  outlinecolor = QColor((Qt::GlobalColor)QtColours[0]);
  outlineActor->GetProperty()->SetColor(outlinecolor.redF(),outlinecolor.greenF(),outlinecolor.blueF());

  cubeAxesActor->SetBounds(outlineMapper->GetBounds());
  cubeAxesActor->SetCamera(renderer->GetActiveCamera());

  cubeAxesActor->SetXTitle(QString("%1").arg(xname).toUtf8().constData());
  cubeAxesActor->SetYTitle(QString("%1").arg(yname).toUtf8().constData());
  cubeAxesActor->SetZTitle(QString("%1").arg(zname).toUtf8().constData());

  cubeAxesActor->DrawXGridlinesOn();
  cubeAxesActor->DrawYGridlinesOn();
  cubeAxesActor->DrawZGridlinesOn();
#if VTK_MAJOR_VERSION > 5
  cubeAxesActor->SetGridLineLocation(vtkCubeAxesActor::VTK_GRID_LINES_FURTHEST);
#endif

  cubeAxesActor->XAxisMinorTickVisibilityOff();
  cubeAxesActor->YAxisMinorTickVisibilityOff();
  cubeAxesActor->ZAxisMinorTickVisibilityOff();

  renderer->AddActor(p.actor());
  renderer->AddActor(cubeAxesActor);
  renderer->AddActor(outlineActor);

  backgroundcolor = QColor(Qt::black);
  renderer->SetBackground(backgroundcolor.redF(), backgroundcolor.greenF(), backgroundcolor.blueF());

  renderer->ResetCamera();

  ui.qvtkWidget->GetRenderWindow()->AddRenderer(renderer);

  connect(ui.actionSymbolSize, SIGNAL(valueChanged(double)), SLOT(SetSymbolSize()));
  connect(ui.saveImageButton, SIGNAL(clicked(bool)), SLOT(SavePlotImage()));
//   connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
}

SimpleScatterPlot3D::~SimpleScatterPlot3D()
{
  DelMatrix(&m);
}
