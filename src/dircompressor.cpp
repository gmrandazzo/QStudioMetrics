#include "dircompressor.h"
#include <QByteArray>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QStringList>
#include <QTextStream>
#include <QImage>
#include <QPixmap>
#include <QBuffer>

#ifdef DEBUG
#include <QDebug>
#endif

#include <ctime>
#include <cstdlib>

bool DirCompressor::SupportedFile(char *fname)
{
  QString fname_ = QString::fromUtf8(fname);
  for(int i = 0; i < fsupported.size(); i++){
    if(fname_.endsWith(fsupported[i].toLower()) == true || fname_.endsWith(fsupported[i].toUpper()) == true){
      return true;
    }
    else{
      continue;
    }
  }
  return false;
}

bool DirCompressor::rmdir(char *dirName)
{
  bool result = true;
  QDir dir(QString::fromUtf8(dirName));

  if(dir.exists(dirName)){
    Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)){
      if(info.isDir()){
        result = rmdir(info.absoluteFilePath().toUtf8().data());
      }
      else{
        result = QFile::remove(info.absoluteFilePath().toUtf8().data());
      }

      if(!result){
        return result;
      }
    }
    result = dir.rmdir(dirName);
  }
  return result;
}

void DirCompressor::GenRandomString(QString *s, int len){
  static const char alphanum[] = "0123456789"
                                  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                  "abcdefghijklmnopqrstuvwxyz";
  srand(time(0));
  for(int i = 0; i < len; ++i){
    (*s).append(alphanum[rand() % (sizeof(alphanum) - 1)]);
  }
}

void DirCompressor::WriteFile(QStringList flist, char *path_){
  QDir path(QString::fromUtf8(path_));
  if(flist.size() > 0 && path.exists()){
    QString relfilepath = QString::fromUtf8(flist.first().split("@").last().toUtf8());

    /*qDebug() << QString::fromUtf8(relfilepath.toUtf8());*/
    QString fname = QString("%1/%2").arg(QString::fromUtf8(path.absolutePath().toUtf8())).arg(QString::fromUtf8(relfilepath.trimmed().toUtf8()));

    /*qDebug() << "WriteFile " << QString::fromUtf8(fname.toUtf8()) << " " << QString::fromUtf8(path_);*/

    QFileInfo finfo(QString::fromUtf8(fname.toUtf8()));
    if(!finfo.absoluteDir().exists()){
      /*qDebug() << " Make Path... " << QString::fromUtf8(finfo.absolutePath().toUtf8());*/
      path.mkpath(QString::fromUtf8(finfo.absolutePath().toUtf8()));
    }

    if(fname.toLower().endsWith(".jpg") == true
      || fname.toLower().endsWith(".jpeg") == true
      || fname.toLower().endsWith(".png") == true
      || fname.toLower().endsWith(".bmp") == true
      || fname.toLower().endsWith(".ppm") == true
      || fname.toLower().endsWith(".xbp") == true
      || fname.toLower().endsWith(".xmp") == true){


      QString ba = QString(flist[1]);
      QImage simage;

      if(fname.toLower().endsWith(".jpg") == true || fname.toLower().endsWith(".jpeg") == true)
        simage.loadFromData(QByteArray::fromBase64(ba.toUtf8()), "JPG");
      else if(fname.toLower().endsWith(".png") == true)
        simage.loadFromData(QByteArray::fromBase64(ba.toUtf8()), "PNG");
      else if(fname.toLower().endsWith(".bmp") == true)
        simage.loadFromData(QByteArray::fromBase64(ba.toUtf8()), "BMP");
      else if(fname.toLower().endsWith(".ppm") == true)
        simage.loadFromData(QByteArray::fromBase64(ba.toUtf8()), "PPM");
      else if(fname.toLower().endsWith(".xbp") == true)
        simage.loadFromData(QByteArray::fromBase64(ba.toUtf8()), "XBP");
      else if (fname.toLower().endsWith(".xmp") == true)
        simage.loadFromData(QByteArray::fromBase64(ba.toUtf8()), "XPM");
      else
        return;

//       simage.save(QString::fromUtf8(fname.toUtf8()), "JPG");

      QFile imgfile(fname.toUtf8());
      imgfile.open(QIODevice::WriteOnly);
      simage.save(&imgfile, "JPG");
    }
    else{
      QFile file(QString::fromUtf8(fname.toUtf8()));
      if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
      QTextStream out(&file);
      for(int i = 1; i < flist.size(); i++){
        out << flist[i];
      }
      file.close();
    }
  }
}

void DirCompressor::ReadFileToString(char *fname, QStringList *filemem)
{
  QString fname_ = QString::fromUtf8(fname);

  if(fname_.toLower().endsWith(".jpg") == true
    || fname_.toLower().endsWith(".jpeg") == true
    || fname_.toLower().endsWith(".png") == true
    || fname_.toLower().endsWith(".bmp") == true
    || fname_.toLower().endsWith(".ppm") == true
    || fname_.toLower().endsWith(".xbp") == true
    || fname_.toLower().endsWith(".xmp") == true){

    QFileInfo finfo(fname_);
    QPixmap image(finfo.absoluteFilePath());
    #ifdef DEBUG
    qDebug() << "DirCompressor::ReadFileToString path: " << finfo.absoluteFilePath();
    #endif
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "JPG"); // writes image into ba in JGP format
    QString encoded = QString(ba.toBase64());
  /*
   QImage image;
    if(fname_.toLower().endsWith(".jpg") == true || fname_.toLower().endsWith(".jpeg") == true)
      image.load(fname_, "JPG");
    else if(fname_.toLower().endsWith(".png") == true)
      image.load(fname_, "PNG");
    else if(fname_.toLower().endsWith(".bmp") == true)
      image.load(fname_, "BMP");
    else if(fname_.toLower().endsWith(".ppm") == true)
      image.load(fname_, "PPM");
    else if(fname_.toLower().endsWith(".xbp") == true)
      image.load(fname_, "XBP");
    else if (fname_.toLower().endsWith(".xmp") == true)
      image.load(fname_, "XPM");
    else
      image.load(fname_);
    */

    (*filemem).append(encoded);
    (*filemem).append("\n");
  }
  else{
    QFile file(QString::fromUtf8(fname));
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
      return;

    while (!file.atEnd()) {
      (*filemem).append(QString::fromUtf8(file.readLine()));
    }
    file.close();
  }
}

void DirCompressor::ScanDir(QDir dir, QFileInfoList *list)
{
  QDirIterator iterator(QString::fromUtf8(dir.absolutePath().toUtf8()), QDirIterator::Subdirectories);
  while(iterator.hasNext()) {
    iterator.next();
    if(!iterator.fileInfo().isDir()){
      QString filename = QString::fromUtf8(iterator.fileName().toUtf8());
      if(SupportedFile(filename.toUtf8().data()) == true){
        (*list).append(QFileInfo(QString::fromUtf8(iterator.filePath().toUtf8())));
      }
      else{
        continue;
      }
    }
  }
}

/*
int DirCompressor::compress()
{
  printf("compress %s as %s\n", dirorfile.toUtf8().data(), out.toUtf8().data());
  if(dirorfile.isEmpty() == false && out.isEmpty() == false){

    if(dtype == DIRECTORY){
      QDir dir(QString::fromUtf8(dirorfile.toUtf8()));
      QFileInfoList filelist;
      ScanDir(dir, &filelist);

      QString dircontent;
      for(int i = 0; i < filelist.size(); i++){
//         printf("Compress %s\n", filelist[i].absoluteFilePath().toUtf8().data());
        QString filepath = QString::fromUtf8(filelist[i].absoluteFilePath().toUtf8().data());
        filepath.remove(QString::fromUtf8(dir.absolutePath().toUtf8().data()));
        dircontent.append(QString("FILE@%1\n").arg(QString::fromUtf8(filepath.toUtf8().data())));
        ReadFileToString(filelist[i].absoluteFilePath().toUtf8().data(), &dircontent);
      }

      QFile outfile(QString::fromUtf8(out.toUtf8()));

      if(!outfile.open(QIODevice::WriteOnly))
        return -1;

      QByteArray compressedData = qCompress(dircontent.toUtf8());
      outfile.write(compressedData);
      outfile.close();
    }
    else{
      QFile infile(QString::fromUtf8(dirorfile.toUtf8()));
      QFile outfile(QString::fromUtf8(out.toUtf8()));

      if(!infile.open(QIODevice::ReadOnly))
        return -1;

      if(!outfile.open(QIODevice::WriteOnly))
        return -1;

      QByteArray uncompressedData = infile.readAll();
      QByteArray compressedData = qCompress(uncompressedData);

      outfile.write(compressedData);

      infile.close();
      outfile.close();
    }
    return 0;
  }
  else{
    return -1;
  }
}
*/

int DirCompressor::compress()
{
  printf("compress %s as %s\n", dirorfile.toUtf8().data(), out.toUtf8().data());
  if(dirorfile.isEmpty() == false && out.isEmpty() == false){
    /*qDebug() << "Compress";*/

    if(dtype == DIRECTORY){
      QDir dir(QString::fromUtf8(dirorfile.toUtf8()));
      QFileInfoList filelist;
      ScanDir(dir, &filelist);

//       QString dircontent;
      QStringList dircontent;
      for(int i = 0; i < filelist.size(); i++){
        /*printf("Compress %s\n", filelist[i].absoluteFilePath().toUtf8().data());*/
        QString filepath = QString::fromUtf8(filelist[i].absoluteFilePath().toUtf8().data());
        filepath.remove(QString::fromUtf8(dir.absolutePath().toUtf8().data()));
//         dircontent.append(QString("FILE@%1\n").arg(QString::fromUtf8(filepath.toUtf8().data())));
        dircontent.append(QString("FILE@%1\n").arg(QString::fromUtf8(filepath.toUtf8().data())));
        ReadFileToString(filelist[i].absoluteFilePath().toUtf8().data(), &dircontent);
//         dircontent.append("END\n"); USED IN THE SECOND TYPE
      }

      QFile outfile(QString::fromUtf8(out.toUtf8()));
      /*qDebug() << outfile.fileName();*/

      if(!outfile.open(QIODevice::WriteOnly))
        return -1;

      QByteArray alldata;
      alldata.append(dircontent.join("").toUtf8());
      QByteArray compressedData = qCompress(alldata);
//       QTextCodec *codec = QTextCodec::codecForName("UTF-8");
//       QByteArray compressedData = qCompress(codec->fromUnicode(dircontent.join("\n")));
      outfile.write(compressedData);
      outfile.close();
    }
    else{
      QFile infile(QString::fromUtf8(dirorfile.toUtf8()));
      QFile outfile(QString::fromUtf8(out.toUtf8()));

      if(!infile.open(QIODevice::ReadOnly))
        return -1;

      if(!outfile.open(QIODevice::WriteOnly))
        return -1;

      QByteArray uncompressedData = infile.readAll();
      QByteArray compressedData = qCompress(uncompressedData);

      outfile.write(compressedData);

      infile.close();
      outfile.close();
    }
    return 0;
  }
  else{
    return -1;
  }
}

/*
int DirCompressor::decompress()
{

//   qDebug() << "Decompress";
//   printf("extract file %s to %s\n", dirorfile.toUtf8().data(), extrpath.toUtf8().data());

  QFileInfo f(QString::fromUtf8(dirorfile.toUtf8().data()));

  if(dirorfile.isEmpty() == false && f.isFile() && extrpath.isEmpty() == false){
    QFile infile(QString::fromUtf8(dirorfile.toUtf8().data()));
    if(!infile.open(QIODevice::ReadOnly))
      return -1;

    QByteArray compressedData = infile.readAll();
    QString uncompressedData;
    if(compressedData.size() > 0)
      uncompressedData = QString(qUncompress(compressedData));
    else
      return -1;

    infile.close();

    //wirte temporaryfile
    QDir dir(QString::fromUtf8(extrpath.toUtf8().data()));
    QString tmp;
    GenRandomString(&tmp, 10);
    QString fname = QString("%1/%2").arg(QString::fromUtf8(dir.absolutePath().toUtf8())).arg(tmp);
//     qDebug() << QString::fromUtf8(fname.toUtf8());
    QFile file(QString::fromUtf8(fname.toUtf8()));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return -1;

    QTextStream out(&file);
    out << uncompressedData;
    file.close();

    QStringList flist;
    QFile file2(QString::fromUtf8(fname.toUtf8()));
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text))
      return -1;

    while(!file2.atEnd()){
      QString line = file2.readLine();
      if(line.trimmed().compare("END", Qt::CaseSensitive) == 0){
        WriteFile(flist, dir.absolutePath().toUtf8().data());
        flist.clear();
      }
      else{
        flist.append(line);
      }
    }
    file2.close();

    //write the last file...
    WriteFile(flist, dir.absolutePath().toUtf8().data());
    file2.remove();
    file.remove();
    return 0;
  }
  else{
    return -1;
  }
}
*/
int DirCompressor::decompress()
{

//   qDebug() << "Decompress";

//   printf("extract file %s to %s\n", dirorfile.toUtf8().data(), extrpath.toUtf8().data());

  QFileInfo f(QString::fromUtf8(dirorfile.toUtf8().data()));

//   qDebug() << f.isFile() << " " << QString::fromUtf8(f.absoluteFilePath().toUtf8().data());

  if(dirorfile.isEmpty() == false && f.isFile() && extrpath.isEmpty() == false){
    QFile infile(QString::fromUtf8(dirorfile.toUtf8().data()));
    if(!infile.open(QIODevice::ReadOnly))
      return -1;

    QByteArray compressedData = infile.readAll();
    QString uncompressedData;
    if(compressedData.size() > 0)
      uncompressedData = QString(qUncompress(compressedData));
    else
      return -1;

    infile.close();

    //wirte temporaryfile
    QDir dir(QString::fromUtf8(extrpath.toUtf8().data()));
    QString tmp;
    GenRandomString(&tmp, 10);
    QString fname = QString("%1/%2").arg(QString::fromUtf8(dir.absolutePath().toUtf8())).arg(tmp);
//     qDebug() << QString::fromUtf8(fname.toUtf8());
    QFile file(QString::fromUtf8(fname.toUtf8()));
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
      return -1;

    QTextStream out(&file);
    out << uncompressedData;
    file.close();

    QStringList flist;
    QFile file2(QString::fromUtf8(fname.toUtf8()));
    bool firstline = true;
    if(!file2.open(QIODevice::ReadOnly | QIODevice::Text))
      return -1;

    while(!file2.atEnd()){
      QString line = file2.readLine();
      if(line.contains("FILE@") == true && firstline == false){
        flist.append("\n");
        WriteFile(flist, dir.absolutePath().toUtf8().data());
        flist.clear();
        flist.append(line);
      }
      else{
        if(firstline == true){
          firstline = false;
          flist.append(line);
        }
        else{
          flist.append(line);
        }
      }
    }
    file2.close();

    //write the last file...
    WriteFile(flist, dir.absolutePath().toUtf8().data());
    file2.remove();
    file.remove();
    return 0;
  }
  else{
    return -1;
  }
}

void DirCompressor::setCompressionLevel(int clevel_)
{
  clevel = clevel_;
}

void DirCompressor::setExtractPath(const char *extrpath_)
{
  extrpath = QString::fromUtf8(extrpath_);
}

void DirCompressor::setDir(const char *dir)
{
  dtype = DIRECTORY;
  dirorfile = QString::fromUtf8(dir);
}

void DirCompressor::setFile(const char *file)
{
  dtype = FILE;
  dirorfile = QString::fromUtf8(file);
}

void DirCompressor::setOutput(const char *out_)
{
  out = QString::fromUtf8(out_);
}
