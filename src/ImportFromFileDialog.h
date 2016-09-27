#ifndef IMPORTFROMFILEDIALOG_H
#define IMPORTFROMFILEDIALOG_H

#include <QDialog>
#include <QStringList>
#include <QStandardItemModel>
#include <QString>
#include <QAbstractListModel>
#include <QObject>
#include <QStringList>

#include "ui_ImportFromFileDialog.h"
#include "data.h"

class StringListModel : public QAbstractListModel
{
  Q_OBJECT

public:
  StringListModel(QObject *parent = 0) : QAbstractListModel(parent) {}
  StringListModel(const QStringList &strings, QObject *parent = 0) : QAbstractListModel(parent), stringList(strings) {}

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

  void Clear();
  void AppendRows(QStringList rows);
  void AppendRow(QString row);


  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value,

                int role = Qt::EditRole);



  bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex());
  bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex());

  QStringList getStringList(){ return stringList; }
private:
    QStringList stringList;
};


class ImportFromFileDialog: public QDialog
{
  Q_OBJECT

public:
  ImportFromFileDialog();
  ~ImportFromFileDialog();

  QString getLastPath(){ return path; }
  QString getFileName();
  int getDataType();
  MATRIX *getMatrix(){ return m; }
  ARRAY *getArray(){ return a; }
  void setPath(QString &path_){ path = path_; }

private slots:
  void Open();
  void LoadFileInMem();
  void OK();
  void UpdateView();
  void Preview();
  void Transpose();
  void EnableDisable();

  void ObjSelectAll();
  void ObjInvertSelection();
  void ObjUnselectAll();
  void ObjSelectBy();

  void VarSelectAll();
  void VarInvertSelection();
  void VarUnselectAll();
  void VarSelectBy();

private:
  Ui::ImportFromFileDialog ui;
  QStandardItemModel* model;
  QString path;

  bool transposestatus;
  int idsplitlinebyindex, idstartlinebox, idstoplinebox;
  QList<QStringList> file;
  QList<int> columntoskip;

  StringListModel *tab1, *tab2;
  QStringList varnames, objnames;
  MATRIX *m;
  ARRAY *a;


  QString getLabel();
  QString getSeparator();
  QString getSkipChar();
  int getStartLine();
  int getStopLine();
  bool FirstColumnObjNames();
  bool FirstRowVarNames();
  void LoadFile(QString file_);
  void LoadObjectNames();
  void LoadVariableNames();

  void GetColumtoDiscard();

  void CountNObjects();
  void CountNVariables();

  void BuildMatrix();
  void BuildArray();
};

#endif
