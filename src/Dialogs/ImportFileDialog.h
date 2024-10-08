#ifndef IMPORTFILEDIALOG_H
#define IMPORTFILEDIALOG_H

#include <QAbstractListModel>
#include <QDialog>
#include <QObject>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

#include "qsmdata.h"
#include "ui_ImportFileDialog.h"

struct FSIZE {
  size_t row;
  size_t col;
  size_t linelenght;
};

class ImportFileDialog : public QDialog {
  Q_OBJECT

public:
  ImportFileDialog();
  ~ImportFileDialog();

  QString getLastPath() { return path; }
  QString getFileName();
  MATRIX *getMatrix() { return m; }
  ARRAY *getArray() { return a; }
  void setPath(QString &path_) { path = path_; }

private slots:
  void Open();
  void OK();
  void Cancel();
  void Preview();

private:
  Ui::ImportFileDialog ui;
  QString path;
  MATRIX *m;
  ARRAY *a;
  QStandardItemModel *model;

  void AssignName(QStringList &list, QString name);
  QString getSkipChar();
  QString getSeparator();
  QString getLabel();

  FSIZE GetSize();
  size_t getHeader(QStringList *header);
  QList<size_t> getLineToSkip();

  void ImportType0();
  void ImportType1();
  void ImportType2();
  void ImportType3();

  void BuildMatrix();
  void Clean_rnames();
  QMap<QString, int> rnames;
  //   void BuildArray();
};

#endif
