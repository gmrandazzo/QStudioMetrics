/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Main entry point for the application.
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

#include <QApplication>
#include <QDir>
#include <QFile>
#include <QStringList>
#include <QStyleFactory>
#include <QtCore/QFileInfo>
#include <QtNetwork/QHostInfo>

#include "DATAIO.h"
#include "MainWindow.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  app.setStyle(QStyleFactory::create("Universal"));
  app.setAttribute(Qt::AA_DontUseNativeMenuBar);

  QString path;
  QString k;
  path = QString("%1/.QStudioMetrics/").arg(QDir::homePath());

  if (DATAIO::DirExists(path.toStdString().c_str()) != true) {
    DATAIO::MakeDir(path.toStdString().c_str());
  }

  MainWindow mainWin(path, k);
  mainWin.show();
  return app.exec();
}
