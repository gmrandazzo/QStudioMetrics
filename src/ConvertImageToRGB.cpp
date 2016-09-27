#include <QtCore>
#include <QImage>
#include <QtGui>
#include <QColor>
#include <QFile>
#include <QCoreApplication>
#include <QString>
#include <iostream>

int main(int argc, char **argv){
  QImage img(argv[1]);
  QFile file(argv[2]);
  file.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(&file);
  
  QString rowname = argv[1];
  rowname.remove(".jpg");
  
  out << QString("%1").arg(rowname) << "\t";
  for ( int row = 1; row < img.height() + 1; ++row ){
    for ( int col = 1; col < img.width() + 1; ++col ){
      QColor clrCurrent(img.pixel(row,col));
      
      out << clrCurrent.red() +  clrCurrent.green() + clrCurrent.blue() << "\t";
        
      /*std::cout << "Pixel at [" << row << "," << col << "] contains color ("
                << clrCurrent.red() << ", "
                << clrCurrent.green() << ", "
                << clrCurrent.blue() << ", "
                << clrCurrent.alpha() << ")."
                << std::endl;*/
    }
  }
  out << "\n";
  file.close(); 
  return 0;
}