#ifndef FindCorrelationWidget_H
#define FindCorrelationWidget_H

#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include <QWidget>

#include "qsmdata.h"
#include "qstudiometricstypes.h"

#include <ui_FindCorrelationWidget.h>

class FindCorrelationWidget : public QWidget {
  Q_OBJECT

public:
  FindCorrelationWidget(matrix *cmx_, matrix *derr_, QStringList *varname_,
                        LABELS *varlabels_, int pid_, int mid_, int mtype_);
  ~FindCorrelationWidget();

private slots:
  void Find();
  void SelectAll();
  void UnselectAll();
  void InvertSelection();
  void PlotVarVSVar();
  void Higlight();

signals:
  void VVPlotSignal(vvplotSignal vvs);
  void HighlightSignal(highlightSignal hhs);
  void CloseFindCorrelationWidgetSignal();

private:
  Ui::FindCorrelationWidget ui;
  QStandardItemModel *model;
  QStringList varname;
  LABELS *varlabels;
  matrix *cmx, *derr;
  QList<CorrVarVarPlotSignal> corr_0, corr_180;
  int pid, mid, mtype;
};

#endif
