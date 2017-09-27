#ifndef SCATTERPLOT2D_H
#define SCATTERPLOT2D_H

#include <QWidget>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <scientific.h>

#include "Chart/chart.h"

#include "ui_ScatterPlot2D.h"
#include "qsmdata.h"
#include "../Dialogs/FindCorrelationWidget.h"

struct QSMPOINT{
  int radius;
  QList <qreal> coord;
  QColor color;
  QString name;
};

class ScatterPlot2D: public QWidget
{
  Q_OBJECT

public:
  enum{
    SIMPLE = 0,
    SCORES = 1,
    LOADINGS = 2
  };

  // Simple scatter plot without line
  ScatterPlot2D(QList<matrix*> &m_, QList<QStringList>& objname,
  QString xaxsisname_, QString yaxsisname_, QString windowtitle);

  // Scatter plot for matrix data (SCORES, LOADINGS, VAR vs VAR)
  ScatterPlot2D(QList<matrix*> &m_, QList<QStringList>& objname,
  QList<MATRIX*> *mxlst_, QStringList xhash_, QStringList yhash_,
  LABELS *objlabels_, LABELS *varlabels_,
  QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_);

  // Scatter Plot for matrix data (SCORES, LOADINGS, VAR vs VAR and TABLABELS
  ScatterPlot2D(QList<matrix*> &m_, QList<QStringList>& objname,
  QList<MATRIX*> *mxlst_, QStringList xhash_, QStringList yhash_,
  TABLABELS *vartablabels, LABELS *objlabels_, LABELS *varlabels_,
  QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_);

  // Simple scatter plot with line
  ScatterPlot2D(QList<matrix*>& mx_, QList<matrix*>& my_, dvector* b_,
  QList<QStringList>& objname,  QString xaxsisname_, QString yaxsisname_, QString windowtitle); // typeplot = 1

  // Scatter Plot with line and selector options. for matrix
  ScatterPlot2D(QList<matrix*>& mx_, QList<matrix*>& my_, dvector* b_,
  QList<QStringList>& objname, QList<MATRIX*>* mxlst_,
  QStringList xhash_, QStringList yhash_,
  LABELS *objlabels_,  LABELS *varlabels_,
  QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_); // MATRIXDATA

  ScatterPlot2D(QList<matrix*>& mx_, QList<matrix*>& my_, dvector* b_, QList<QStringList>& objname, QList<ARRAY*>* arlst_, QStringList xhash_, QStringList yhash_, LABELS *objlabels_,  LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_); // ARRAYDATA

  /* Scatter Plot without line but with x and y*/
  ScatterPlot2D(QList<matrix*>& mx_, QList<matrix*>& my_, QList<QStringList>& objname, QList<MATRIX*>* mxlst_, QStringList xhash_, QStringList yhash_, LABELS* objlabels_,  LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_); // MATRIXDATA

  ScatterPlot2D(QList<matrix*>& mx_, QList<matrix*>& my_, QList<QStringList>& objname, QList<MATRIX*>* mxlst_, QStringList xhash_, QStringList yhash_, TABLABELS *vartablabels_, LABELS* objlabels_,  LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_); // MATRIXDATA SPECIAL USED FROM LOADINGS MVNDISTRIB

  ScatterPlot2D(QList<matrix*>& mx_, QList<matrix*>& my_, QList<QStringList>& objname, QList<ARRAY*>* arlst_, QStringList xhash_, QStringList yhash_, LABELS* objlabels_,  LABELS *varlabels_, QString xaxsisname_, QString yaxsisname_, QString windowtitle, int type_); // ARRAYDATA

  ~ScatterPlot2D();

  void setArrayLayer(int layer_){ layer = layer_; }
  void SetAutoNameAxes(bool autonameaxes_);
  void setAxisNameExtensions(QStringList nameaxisext_);
  void setHotellingConfidenceEllipse(bool ehotel_);

  void BuildDiagonal();
  void addCurve(QList< matrix *> m, QStringList curvenames_, QList< QColor > curvecolors_);
  void removeAllCurves();

  void setPID(int pid_){ pid = pid_; }
  void setMID(int mid_){ mid = mid_; }
  void setModelType(int mtype_){ mtype = mtype_; }

  enum { SINGLEAXIS, DOUBLEAXIS };
public slots:
  void slotExit();
  void SelectAll();
  void SelectBy();
  void SelectByVarlabels();
  void invertSelection();
  void hideSelection();
  void clearSelection();
  void setSelectionStyle();
  void SetSelectionObjLabels();
  void SetSelectionVarLabels();
  void DoClusterAnalysis();
  void setOnOffHotellingEllipse();
  void SetAxisScale();
  void SavePlotImage();
  void SaveSelection();
  void setAxis();
  void PlotUpdate();
  void ResetPlot();
  void AbortRun();
  void ShowContextMenu(const QPoint& pos);
  void SetSelectionName();
  void FindCorrelations();
  void EmitVVPlotSignal(vvplotSignal vvp);
  void HighlightObjects(highlightSignal hhs);
  void CloseFindCorrelationWidget();

signals:
  void ScatterPlot2DImageSignalChanged(ImageSignal new_spi);
  void ScatterPlot2DVVPlotSignal(vvplotSignal vvp);

private:
  Ui::ScatterPlot2D ui;
  QVBoxLayout *plotLayout;
  Chart *chart;

  FindCorrelationWidget *cwidget;
  QList <QSMPOINT> p;

  QList< matrix * > curves;
  QStringList curvenames;
  QList < QColor > curvecolors;

  QString xaxisname, yaxisname;
  QStringList nameaxisext;
  QColor color;
  int typedata, type;
  QList<MATRIX*> *mxlst;
  QList<ARRAY*> *arlst;
  QVector<qreal> acoeff; /*angular coefficient*/
  QStringList xhash, yhash;
  LABELS *objlabels, *varlabels;
  TABLABELS *vartablabels;

  QList<int> selectedIDS;
  ImageSignal spi;
  int pid;
  int mid;
  int mtype;

  int layer;
  void getPointSelected();
  void initPoint(QList<matrix*> mx, QList<QStringList> name);
  void initPoint(QList<matrix*> mx, QList<matrix*> my, QList<QStringList> name);
  void BuildLine(qreal angular_coeff);
  void BuildHotellingEllipse();
  QColor makeColor(double val, double min, double max, QColor &color1, QColor &color2);
  int makeSize(double val, double min, double max, int sizemin, int sizemax);

  int GetIDinmxlst(QString mxhash);
  int GetIDinarlst(QString arhash);
  int axischange;
  bool autonameaxes;
  bool ehotel;
  bool abort;

  void StartRun();
  void StopRun();
};

#endif
