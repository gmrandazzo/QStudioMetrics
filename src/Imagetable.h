#ifndef IMAGETABLE_H
#define IMAGETABLE_H

#include <QFile>
#include <QStandardItemModel> // Remove this
#include <QStringList>
#include <QTextStream>
#include <QWidget>
#include <scientific.h>

#include <QAbstractTableModel>

#include <QContextMenuEvent>

#include "Dialogs/GenericProgressDialog.h"
#include "qsmdata.h"
#include "ui_Table.h"

#include <iostream>

class ImageTableModel : public QAbstractTableModel {
  Q_OBJECT
public:
  ImageTableModel(QObject *parent);
  int rowCount(const QModelIndex &parent = QModelIndex()) const override;
  int columnCount(const QModelIndex &parent = QModelIndex()) const override;
  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override;
};

#endif
