// ClassPlotDialog.ui
#ifndef CLASSPLOTDIALOG_H
#define CLASSPLOTDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <QList>
#include <QStringList>
#include <QStandardItemModel>
#include "qsmdata.h"

#include "ui_ClassPlotDialog.h"

class ClassPlotDialog : public QDialog
{
  Q_OBJECT

public:
  enum WindowType{
      PCALOADINGS = 0,
      PLSLOADINGS = 1,
      UPCALOADINGS = 2,
      UPLSLOADINGS = 3
  };
  
  ClassPlotDialog (PROJECTS *projects, int type); // 0 = PCA;   1 = PLS; 2 = UPCA;   3 = UPLS; LDA
  ~ClassPlotDialog ();
  
  int selectedProject(){ return selectedproject_; }
  int getModelID(){ return modelid; }
  LABELS getClass(){ return g; }
  bool Plot(){ return plot_; }

private slots:
  void actionPlot();
  void setProject(QModelIndex);
  void setModelID(QModelIndex);
  void Find();
  void FindAll();
  void AddGroup();
  void RemoveGroup();
  void SelectAll();
  void InvertSelection();
  void SelectBy();
  void UnselectAll();
  
private:
  Ui::ClassPlotDialog  ui;
  PROJECTS *projects_;
  QStandardItemModel *tab1, *tab2, *tab3, *tab4;
  QList<int> pids;
  QList<int> mids;
  int selectedproject_;
  int type;
  int modelid;
  bool plot_;
  void CheckPlot();
  LABELS labels, g;

};

#endif

