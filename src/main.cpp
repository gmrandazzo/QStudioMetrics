#include <QApplication>
#include <QtCore/QFileInfo>
#include <QtNetwork/QHostInfo>
#include <QDir>
#include <QFile>

#include "DATAIO.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
  #ifdef BUILDEXEC
  QFileInfo finfo(argv[0]);
  #ifdef OSX
  QCoreApplication::setLibraryPaths(QStringList(finfo.absolutePath()+"/../Plugins"));
  #endif
  #ifdef WIN32
  QCoreApplication::setLibraryPaths(QStringList(finfo.path()+"/plugins"));
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
