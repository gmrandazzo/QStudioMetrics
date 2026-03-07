/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for dircompressor.
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
  void ReadFileToString(const char *fname, QStringList *filemem);
  static void WriteFile(QStringList flist, const char *path_);
  static void GenRandomString(QString *s, int len);
  static bool rmdir(const char *dirName);
  int dtype;
  bool SupportedFile(const char *fname);
  QStringList fsupported;
  QString dirorfile;
  QString extrpath;
  QString out;
  int clevel;
};
#endif
