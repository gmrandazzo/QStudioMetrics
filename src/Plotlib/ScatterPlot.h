#ifndef SCATTERPLOT2D_H
#define SCATTERPLOT2D_H

#include <QWidget>
#include <QList>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QStringList>
#include <scientific.h>

#include "Chart/chart.h"
#include "Chart/qplotly.h"

#include "ui_ScatterPlot.h"
#include "qsmdata.h"
#include "../Dialogs/FindCorrelationWidget.h"


struct QSMPOINT{
  int radius;
  QList <qreal> coord;
  QColor color;
  QString name;
};

class ScatterPlot: public QWidget
{
  Q_OBJECT

public:
  enum{
    SIMPLE = 0,
    SCORES,
    LOADINGS,
    SINGLEAXIS,
    DOUBLEAXIS,
    TRIPLEAXIS
  };

  // Simple scatter plot without line
  ScatterPlot(QList<matrix*> &m_,
              QList<QStringList>& objname,
              QString xaxsisname_,
              QString yaxsisname_,
              QString windowtitle);

  // Scatter plot for matrix data (SCORES, LOADINGS, VAR vs VAR)
  ScatterPlot(QList<matrix*> &m_,
              QList<QStringList>& objname,
              QList<MATRIX*> *mxlst_,
              QStringList xhash_,
              QStringList yhash_,
              LABELS *objlabels_,
              LABELS *varlabels_,
              QString xaxsisname_,
              QString yaxsisname_,
              QString windowtitle,
              int type_);

  ScatterPlot(QList<matrix*> &m_,
              QList<QStringList>& objname,
              QList<MATRIX*> *mxlst_,
              QStringList xhash_,
              QStringList yhash_,
              LABELS *objlabels_,
              LABELS *varlabels_,
              QString xaxsisname_,
              QString yaxsisname_,
              QString zaxsisname_,
              QString windowtitle,
              int type_);

  // Scatter Plot for matrix data (SCORES, LOADINGS, VAR vs VAR and TABLABELS
  ScatterPlot(QList<matrix*> &m_,
              QList<QStringList>& objname,
              QList<MATRIX*> *mxlst_,
              QStringList xhash_,
              QStringList yhash_,
              TABLABELS *vartablabels,
              LABELS *objlabels_,
              LABELS *varlabels_,
              QString xaxsisname_,
              QString yaxsisname_,
              QString windowtitle,
              int type_);

  // Simple scatter plot with line
  ScatterPlot(QList<matrix*>& mx_,
              QList<matrix*>& my_,
              dvector* b_,
              QList<QStringList>& objname,
              QString xaxsisname_,
              QString yaxsisname_,
              QString windowtitle); // typeplot = 1

  // Scatter Plot with line and selector options. for matrix
  ScatterPlot(QList<matrix*>& mx_,
              QList<matrix*>& my_,
              dvector* b_,
              QList<QStringList>& objname,
              QList<MATRIX*>* mxlst_,
              QStringList xhash_,
              QStringList yhash_,
              LABELS *objlabels_,
              LABELS *varlabels_,
              QString xaxsisname_,
              QString yaxsisname_,
              QString windowtitle,
              int type_); // MATRIXDATA

  /* Scatter Plot without line but with x and y*/
  ScatterPlot(QList<matrix*>& mx_,
              QList<matrix*>& my_,
              QList<QStringList>& objname,
              QList<MATRIX*>* mxlst_,
              QStringList xhash_,
              QStringList yhash_,
              LABELS* objlabels_,
              LABELS *varlabels_,
              QString xaxsisname_,
              QString yaxsisname_,
              QString windowtitle,
              int type_); // MATRIXDATA

  ScatterPlot(QList<matrix*>& mx_,
              QList<matrix*>& my_,
              QList<QStringList>& objname,
              QList<MATRIX*>* mxlst_,
              QStringList xhash_,
              QStringList yhash_,
              TABLABELS *vartablabels_,
              LABELS* objlabels_,
              LABELS *varlabels_,
              QString xaxsisname_,
              QString yaxsisname_,
              QString windowtitle, int type_); // MATRIXDATA SPECIAL USED FROM LOADINGS MVNDISTRIB

  ~ScatterPlot();

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

public slots:
  void slotExit();
  void SelectAll();
  void SelectBy();
  void SelectByVarlabels();
  void invertSelection();
  void hideSelection();
  void clearSelection();
  void setSelectionStyle();
  void showPointLabels();
  void hidePointLabels();
  void SetSelectionObjLabels();
  void SetSelectionVarLabels();
  void DoClusterAnalysis();
  void setOnOffHotellingEllipse();
  void SavePlotImage();
  void SaveSelection();
  void OpenPlotSettingsDialog();
  void UpdatePointPosition();
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
  void ScatterPlotImageSignalChanged(ImageSignal new_spi);
  void ScatterPlotVVPlotSignal(vvplotSignal vvp);

private:
  Ui::ScatterPlot ui;
  enum class PEngine { Qtchart, QPlotly };

  //QPlotlyWindow *chart;
  // or...
  //Chart *chart;
  // or...
  // WARNING: Plotly give problems on windows with WEBKIT and is extremelly slow
  // With this workaround i choose between the old plotter and the new qplotly
  Graphs *NewGraph(PEngine peng);
  Graphs *chart;

  FindCorrelationWidget *cwidget;
  QList <QSMPOINT> p;
  QList< matrix * > curves;
  QStringList curvenames;
  QList < QColor > curvecolors;
  QString xaxisname, yaxisname, zaxisname;
  QStringList nameaxisext;
  QStringList markersymbls;
  QColor color;
  int type;
  QList<MATRIX*> *mxlst;
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
  void UpdateAxisName();
  void initPoint(QList<matrix*> mx, QList<QStringList> name);
  void initPoint(QList<matrix*> mx, QList<matrix*> my, QList<QStringList> name);
  void BuildLine(qreal angular_coeff);
  void BuildHotellingEllipse();
  int random(int min, int max);
  QList<QColor> GenColorList(int size);
  QColor makeColor(double val, double min, double max, QColor &color1, QColor &color2);
  int makeSize(double val, double min, double max, int sizemin, int sizemax);
  int GetIDinmxlst(QString mxhash);
  int GetIDinarlst(QString arhash);
  int axistype;
  bool autonameaxes;
  bool ehotel;
  bool abort;
  void StartRun();
  void StopRun();
};

#endif
