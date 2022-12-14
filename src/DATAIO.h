#ifndef DATAIO_H
#define DATAIO_H

#include <iostream>
#include <string>
#include <vector>

#include "scientific.h"

namespace DATAIO {
  std::vector<std::string> split(const std::string& s, const std::string& delim, const bool keep_empty);

  bool FileExists(char *file_);
  bool DirExists(char *dir_);
  void MakeDir(char *dir_);
  void RemoveFile(char *path_file);
  void RemoveFiles(char *dir_);
  void RemoveDir(char *dir_);
  void FileList(char *dir_, std::vector<std::string> &list);

  void GetMatrixRowCol(char *file_, const std::string &sep, size_t *row, size_t *col);
  void ImportMatrix(char *file_, const std::string &sep, matrix *data);
  void GetArrayOrderRowCol(char *file_, const std::string &sep, size_t *order, size_t *row, size_t *col);
  void ImportTensor(char *file_, const std::string &sep, tensor *data);

  void GetVectorSize(char *file_, size_t *size_);
  void ImportStrvector(char *file_, strvector *strv);
  void ImportDvector(char *file_, dvector *v);
  void ImportUIvector(char *file_, uivector *v);
  void GetDVectorListSize(char *file_, uivector *sizes);
  void ImportDvectorList(char *file_, dvectorlist *lst);

  void ImportPCAModel(char *path_, PCAMODEL *m);
  void ImportPLSModel(char *path_, PLSMODEL *m);
  void ImportUPCAModel(char *path_, UPCAMODEL *m);
  void ImportUPLSModel(char *path_, UPLSMODEL *m);
  void ImportMLRModel(char *path_, MLRMODEL *m);
  void ImportLDAModel(char *path_, LDAMODEL *m);

  void WriteStringList(char *file_, std::vector<std::string> &strlst);
  void WriteComments(char *file_, std::vector<std::string> &strvect);
  void WriteDvector(char *file_, dvector *v);
  void WriteUIvector(char *file_, uivector *v);
  void WriteDVectorList(char *file_, dvectorlist *lst);
  void WriteMatrix(char *file_, matrix *m);
  void WriteTensor(char *file_, tensor *a);
  void WritePCAModel(char *path_, PCAMODEL *m);
  void WritePLSModel(char *path_, PLSMODEL* m);
  void WriteUPCAModel(char *path_, UPCAMODEL *m);
  void WriteUPLSModel(char *path_, UPLSMODEL* m);
  void WriteMLRModel(char *path_, MLRMODEL* m);
  void WriteLDAModel(char *pat_, LDAMODEL* m);
}
#endif
