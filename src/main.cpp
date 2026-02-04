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
#ifdef BUILDEXEC
  QFileInfo finfo(argv[0]);
#ifdef OSX
  QStringList paths;
  paths << finfo.absolutePath() + "/../Frameworks";
  paths << finfo.absolutePath() + "/../PlugIns";
  QCoreApplication::setLibraryPaths(paths);
#endif

#ifdef WIN32
  QStringList paths;
  paths << finfo.path();
  paths << finfo.path() + "/plugins";
  QCoreApplication::setLibraryPaths(paths);
#endif
#endif

  // qDebug() << QCoreApplication::libraryPaths();

  QApplication app(argc, argv);
#ifdef BUILDEXEC
  QApplication::setStyle(QStyleFactory::create("Fusion"));
#endif

  QString path;
  QString k;
  path = QString("%1/.QStudioMetrics/").arg(QDir::homePath());

  if (DATAIO::DirExists((char *)path.toStdString().c_str()) != true) {
    DATAIO::MakeDir((char *)path.toStdString().c_str());
  }

  MainWindow mainWin(path, k);
  mainWin.show();
  return app.exec();
}
