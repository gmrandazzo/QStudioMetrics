#include <QApplication>
#include <QStyleFactory>
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
  //#ifdef OSX
  //QCoreApplication::setLibraryPaths(QStringList(finfo.absolutePath()+"/../Plugins"));
  //#endif
  #ifdef WIN32
  QCoreApplication::setLibraryPaths(QStringList(finfo.path()+"/plugins"));
  #endif
  #endif

  //qDebug() << QCoreApplication::libraryPaths();

  QApplication app(argc, argv);
  #ifdef BUILDEXEC
  QApplication::setStyle(QStyleFactory::create("Fusion"));
  #endif
  
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
