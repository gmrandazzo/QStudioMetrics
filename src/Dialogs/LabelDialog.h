#ifndef LABELDIALOG_H
#define LABELDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>
#include "qstudiometricstypes.h"

#include "qsmdata.h"

#include "ui_LabelDialog.h"

struct labtype{
  int id;
  int type;
};

class LabelDialog: public QDialog
{
  Q_OBJECT

public:
  enum{
    OBJLABELS = 0,
    VARLABELS = 1,
    SHOWOBJLABELS = 2,
    SHOWVARLABELS = 3
  };
  LabelDialog(PROJECTS *projects_, int windowtype_); // used in mainwindow
  LabelDialog(LABELS *objlabels_, QStringList &selectedobject_, int windowtype_); // used for table selection and plot2D/3D
  ~LabelDialog();
  void setPath(QString path_){ lastpath = path_; }

signals:
  void ShowList(ListSignal lsig);
  
private slots:
  void AddLabel();
  void ImportLabelList();
  void ImportTable();
  void Show();
  void RemoveLabel();
  void OK();
  void Cancel();
  void Close();
  void GenLabelView(QModelIndex);
  void setLabelId(QModelIndex);
  
private:
  QList<labtype> lidlst;
  Ui::LabelDialog ui;
  int windowtype;
  QString lastpath;
  int pid;
  int lid;
  PROJECTS *projects;
  LABELS *labels;
  TABLABELS *tablabels;
  QStringList selectedobject;
  QStandardItemModel *tab1, *tab2;
  bool ObjlabelContains(QString name);
  ListSignal lsig_;
  int ntablabels();
  int nlabels();
  
};

#endif

