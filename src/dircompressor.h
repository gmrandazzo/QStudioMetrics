#ifndef DIRCOMPRESSOR_H
#define DIRCOMPRESSOR_H
#include <QDir>
#include <QFileInfo>
#include <QString>
#include <QStringList>

class DirCompressor {
public:
  DirCompressor() {
    dtype = clevel = -1;
    fsupported << ".txt";
    fsupported << ".jpg";
    fsupported << ".jpeg";
    fsupported << ".png";
    fsupported << ".bmp";
    fsupported << ".ppm";
    fsupported << ".xbp";
    fsupported << ".xmp";
  }
  int compress();
  int decompress();
  void setFile(const char *file);
  void setDir(const char *dir);
  void setSupportedFile(QString ext) {
    if (!ext.isEmpty()) {
      if (ext.at(0) == '.') {
        fsupported.append(ext);
      } else {
        fsupported.append("." + ext);
      }
    } else {
      return;
    }
  }
  void setOutput(const char *out_);
  void setCompressionLevel(int clevel_);
  void setExtractPath(const char *extrpath_);

private:
  enum { DIRECTORY = 0, FILE = 1 };
  void ScanDir(QDir dir, QFileInfoList *list);
  void ReadFileToString(char *fname, QStringList *filemem);
  void WriteFile(QStringList flist, char *path_);
  void GenRandomString(QString *s, int len);
  bool rmdir(char *dirName);
  int dtype;
  bool SupportedFile(char *fname);
  QStringList fsupported;
  QString dirorfile;
  QString extrpath;
  QString out;
  int clevel;
};
#endif
