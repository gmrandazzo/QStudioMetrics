#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
#include <QDialog>
#include <QString>
#include <QList>
#include <QStandardItemModel>
#include "qsmdata.h"
#include "ui_ProjectManager.h"

class ProjectManager: public QDialog
{
  Q_OBJECT

public:
  explicit ProjectManager(DATA *data_);
  ~ProjectManager();
  void setWindowName(QString name_);
  void setAppend(bool appendstate_);
  QString getName(){ return ui.lineEdit->text(); }
  void setName(QString name){ ui.lineEdit->setText(name); }
  void setLastPath(const QString &lastpath_){ lastpath = lastpath_; }
  QString &getLastPath(){ return lastpath; }
  
private slots:
  void Remove();
  void Add();
  void AddImagesAsRGB();
  void AddImageList();
  void Finish();
  
private:
  Ui::ProjectManager ui;
  DATA *data;
  QList< int > matrixid, arrayid;
  QList<QStringList> imagenamelst;
  bool appendstate;
  QStandardItemModel *table;
  QString lastpath;
};

#endif
