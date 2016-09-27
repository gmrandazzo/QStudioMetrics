#include <QApplication>
#include <QtCore/QFileInfo>
#include <QtNetwork/QHostInfo>
#include <QDir>
#include <QFile>

#include "DATAIO.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{

  QString path;
  QString k;
  path = QString("%1/.QStudioMetrics/").arg(QDir::homePath());

  if(DATAIO::DirExists((char*)path.toStdString().c_str()) != true){
    DATAIO::MakeDir((char*)path.toStdString().c_str());
  }
  
  QApplication app(argc, argv);
  MainWindow mainWin(path, k);
  mainWin.show();
  return app.exec();
}
