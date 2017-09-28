#include <QApplication>
#include <QtCore/QFileInfo>
#include <QtNetwork/QHostInfo>
#include <QDir>
#include <QFile>

#include "DATAIO.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
  #ifdef RELEASE
  #ifdef WIN32
  QDir dir(argv[0]);
  Q_ASSERT(dir.cdUp());
  #ifdef OSX
  Q_ASSERT(dir.cdUp());
  #endif
  Q_ASSERT(dir.cd("plugins"));
  QCoreApplication::setLibraryPaths(QStringList(dir.absolutePath()));
  #endif
  #endif
  QApplication app(argc, argv);
  QString path;
  QString k;
  path = QString("%1/.QStudioMetrics/").arg(QDir::homePath());

  if(DATAIO::DirExists((char*)path.toStdString().c_str()) != true){
    DATAIO::MakeDir((char*)path.toStdString().c_str());
  }

  MainWindow mainWin(path, k);
  mainWin.show();
  return app.exec();
}
