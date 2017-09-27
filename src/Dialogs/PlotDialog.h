#ifndef PLOTDIALOG_H
#define PLOTDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QStandardItemModel>
#include "qsmdata.h"
#include "ui_PlotDialog.h"

class PlotDialog : public QDialog
{
  Q_OBJECT

public:
  PlotDialog(PROJECTS *projects, int type); // 0 = PCA;   1 = PLS; 4 = PLS-Validation Plot; 2 = UPCA;   3 = UPLS  5 = UPLS-Validation Plot; 6 = PCA Prediction ; 7 PLS Prediction; 8 = UPCA Prediction; 9 = UPLS Prediction
  ~PlotDialog();
  
  int selectedProject(){ return selectedproject_; }
  int getModelID(){ return modelid; }
  int getPredID(){ return predid; }
  int getNPC(){ return npc; }
  bool Plot(){ return plot_; }

private slots:
  void actionPlot();
  void setProject(QModelIndex);
  void setModelID(QModelIndex);
  void setPredictionID(QModelIndex);
  void setNPC();
  
private:
  Ui::PlotDialog ui;
  PROJECTS *projects_;
  QStandardItemModel *tab1, *tab2, *tab3;
  QList<int> pids;
  QList<int> mids;
  QList<int> predids;
  int selectedproject_;
  int type;
  int modelid, predid, npc;
  bool plot_;
  void CheckPlot();

};

#endif

