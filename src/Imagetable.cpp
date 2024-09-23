#include "Imagetable.h"

ImageTableModel::ImageTableModel(QObject *parent)
    : QAbstractTableModel(parent) {}

int ImageTableModel::rowCount(const QModelIndex & /*parent*/) const {
  return 2;
}
int ImageTableModel::columnCount(const QModelIndex & /*parent*/) const {
  return 3;
}
QVariant ImageTableModel::data(const QModelIndex &index, int role) const {
  if (role == Qt::DisplayRole) {
    return QString("Row%1, Column%2")
        .arg(index.row() + 1)
        .arg(index.column() + 1);
  }
  return QVariant();
}
