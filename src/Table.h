#ifndef TABLE_H
#define TABLE_H

#include <scientific.h>
#include <QWidget>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QStandardItemModel> // Remove this

#include <QAbstractTableModel>

#include <QContextMenuEvent>

#include "Dialogs/GenericProgressDialog.h"
#include "qsmdata.h"
#include "ui_Table.h"

#include <iostream>

class Model : public QAbstractTableModel
{
  Q_OBJECT
public:
  explicit Model(QObject *parent = 0);
  explicit Model(matrix *m_, QObject* parent);
  explicit Model(QList<QStringList> tab_, QObject* parent);
  ~Model();

  void setItem(uint row, uint col, double value);
  void setObjNames(QStringList labels_);
  QStringList getObjNames(){ return labels; }

  void setHorizontalHeaderLabels(QStringList& headerlabels);
  QStringList getHorizontalHeaderLabels(){ return header; }

//   bool setData(matrix *m_, int role=Qt::EditRole);

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;


  void setVariable(int variable_){ variable = variable_; }
  void setMinColumnColor(QColor mincolor_){ mincolor = mincolor_; }
  void setMaxColumnColor(QColor maxcolor_){ maxcolor = maxcolor_; }
  void setMinColumnVal(double minval_){ minval = minval_; }
  void setMaxColumnVal(double maxval_){ maxval = maxval_; }

  void init_Matrix();
  void newMatrix(uint row, uint col);
  void newMatrix(uint row, uint col, QStringList objnames, QStringList headernames);
  void delMatrix();
  matrix *Matrix(){ return m; }
  void UpdateModel();

private:
  matrix *m;
  QList<QStringList> tab;
  bool isallocatedmatrix;
  QStringList labels; // TODO: ADD THE POSSIBILITY FOR CHARGE AN IMAGE
  QStringList header;
  QList<uint> id;
  QColor mincolor, maxcolor;
  double minval, maxval;
  int variable;
  QColor makeColor(double val, const double& min, const double& max, const QColor& color1, const QColor& color2) const;
};

class Table : public QWidget
{
  Q_OBJECT

public:
  explicit Table(QWidget *parent = 0);
  explicit Table(matrix *m_, QWidget *parent = 0);
  explicit Table(matrix *m_, LABELS *objlabels_, LABELS *varlabels_, QWidget *parent = 0);
  explicit Table(QList<QStringList> tab, LABELS *objlabels_, LABELS *varlabels_, QWidget *parent = 0);
  explicit Table(QStringList names, QList<QPixmap> images, QList<QColor> colors, QWidget *parent = 0);
  ~Table();

  void setPID(int pid_){ pid = pid_; } /*used for img signal*/
  void setObjLabels(LABELS *objlabels_){ objlabels = objlabels_; }
  void setVarLabels(LABELS *varlabels_){ varlabels = varlabels_; }
  Model* model() const { return model_; }

private slots:
  void copy();
  void addObjectLabel();
  void addVariableLabel();
  void searchBy();
  void selectBy();
  void highlitingCell();
  void resetHighliting();
  void ExportTable();
  void stopRun();
  void SetSelectionName();
  void SortByColumn(int);
  void SaveAsImage();

signals:
  void TabImageSignalChanged(ImageSignal new_ts);

private:
  Ui::Table ui;
  Model *model_;
  QStandardItemModel *imgtabmodel_;
  LABELS *objlabels, *varlabels;
  ImageSignal ts;
  int pid;

protected:
   void contextMenuEvent(QContextMenuEvent *event);
   GenericProgressDialog pdialog;
   bool stoprun;
   void StartSelectionRun();
   void StopSelectionRun();
};

#endif
