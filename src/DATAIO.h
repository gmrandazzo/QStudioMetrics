/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Header file for DATAIO.
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

#ifndef DATAIO_H
#define DATAIO_H

#include <iostream>
#include <string>
#include <vector>

#include "scientific.h"

namespace DATAIO {
std::vector<std::string> split(const std::string &s, const std::string &delim,
                               const bool keep_empty);

bool FileExists(const char *file_);
bool DirExists(const char *dir_);
void MakeDir(const char *dir_);
void RemoveFile(const char *path_file);
void RemoveFiles(const char *dir_);
void RemoveDir(const char *dir_);
void FileList(const char *dir_, std::vector<std::string> &list);

void GetMatrixRowCol(const char *file_, const std::string &sep, size_t *row,
                     size_t *col);
void ImportMatrix(const char *file_, const std::string &sep, matrix *data);
void GetArrayOrderRowCol(const char *file_, const std::string &sep, size_t *order,
                         size_t *row, size_t *col);
void ImportTensor(const char *file_, const std::string &sep, tensor *data);

void GetVectorSize(const char *file_, size_t *size_);
void ImportStrvector(const char *file_, strvector *strv);
void ImportDvector(const char *file_, dvector *v);
void ImportUIvector(const char *file_, uivector *v);
void GetDVectorListSize(const char *file_, uivector *sizes);
void ImportDvectorList(const char *file_, dvectorlist *lst);

void ImportPCAModel(const char *path_, PCAMODEL *m);
void ImportPLSModel(const char *path_, PLSMODEL *m);
void ImportUPCAModel(const char *path_, UPCAMODEL *m);
void ImportUPLSModel(const char *path_, UPLSMODEL *m);
void ImportMLRModel(const char *path_, MLRMODEL *m);
void ImportLDAModel(const char *path_, LDAMODEL *m);

void WriteStringList(const char *file_, const std::vector<std::string> &strlst);
void WriteComments(const char *file_, const std::vector<std::string> &strvect);
void WriteDvector(const char *file_, dvector *v);
void WriteUIvector(const char *file_, uivector *v);
void WriteDVectorList(const char *file_, dvectorlist *lst);
void WriteMatrix(const char *file_, matrix *m);
void WriteTensor(const char *file_, tensor *a);
void WritePCAModel(const char *path_, PCAMODEL *m);
void WritePLSModel(const char *path_, PLSMODEL *m);
void WriteUPCAModel(const char *path_, UPCAMODEL *m);
void WriteUPLSModel(const char *path_, UPLSMODEL *m);
void WriteMLRModel(const char *path_, MLRMODEL *m);
void WriteLDAModel(const char *pat_, LDAMODEL *m);
} // namespace DATAIO
#endif
