#ifndef DIALOGPLOTS_H
#define DIALOGPLOTS_H

#include <QDialog>
#include <QModelIndex>
#include <QStandardItemModel>
#include "qsmdata.h"
#include "ui_DialogPlots.h"

class DialogPlots : public QDialog
{
  Q_OBJECT

public:
  enum{
    TwoColumns = 0,
    ThreeColumns
  };

  DialogPlots(ProjectTree pjtree_, int dtype_);
  ~DialogPlots();
  void hideOptions(bool);
  int getProjectID();
  int getModelID();
  int getPredictionID();
  int getNLV();

private slots:
  void actionPlot();
  void setProject(QModelIndex, QModelIndex);
  void setModel(QModelIndex, QModelIndex);
  void setPrediction(QModelIndex, QModelIndex);

private:
  Ui::DialogPlots ui;
  ProjectTree pjtree;
  int dtype;
  int pid, mid, predid;
  QStandardItemModel *tab1, *tab2, *tab3;
  void EnableDisableButtons();
};

#endif
