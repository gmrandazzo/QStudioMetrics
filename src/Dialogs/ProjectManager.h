/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for ProjectManager.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H
#include "qsmdata.h"
#include "ui_ProjectManager.h"
#include <QDialog>
#include <QList>
#include <QStandardItemModel>
#include <QString>

class ProjectManager : public QDialog {
  Q_OBJECT

public:
  explicit ProjectManager(DATA *data_);
  ~ProjectManager();
  void setWindowName(QString name_);
  void setAppend(bool appendstate_);
  QString getName() { return ui.lineEdit->text(); }
  void setName(QString name) { ui.lineEdit->setText(name); }
  void setLastPath(const QString &lastpath_) { lastpath = lastpath_; }
  QString &getLastPath() { return lastpath; }

private slots:
  void Remove();
  void Add();
  void AddImagesAsRGB();
  void AddImageList();
  void Finish();

private:
  Ui::ProjectManager ui;
  DATA *data;
  QList<int> matrixid, arrayid;
  QList<QStringList> imagenamelst;
  bool appendstate;
  QStandardItemModel *table;
  QString lastpath;
};

#endif
