/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for DATAIO.
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

#include "DATAIO.h"
#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#ifdef WIN32
#include <errno.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef MINGW32
#include <dir.h>
#include <direct.h>
#endif
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "scientific.h"

std::string DATAIO::STrim(const std::string &s) {
  if (s.empty()) return "";
  size_t end = s.find_last_not_of(" \t\n\r\f\v");
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::vector<std::string> DATAIO::split(const std::string &s,
                                       const std::string &delim,
                                       const bool keep_empty) {
  std::vector<std::string> result;
  if (delim.empty()) {
      result.push_back(s);
      return result;
  }
  size_t start = 0;
  size_t end = s.find_first_of(delim);
  while (end != std::string::npos) {
      if (keep_empty || end > start) {
          result.push_back(s.substr(start, end - start));
      }
      start = end + 1;
      end = s.find_first_of(delim, start);
  }
  if (keep_empty || start < s.length()) {
      result.push_back(s.substr(start));
  }
  return result;
}

bool DATAIO::FileExists(const char *file_) {
  std::ifstream foo(file_);
  return foo.good();
}

bool DATAIO::DirExists(const char *dir_) {
  struct stat status;
  if (stat(dir_, &status) == 0) {
    if (status.st_mode & S_IFDIR) {
      //         std::cout << "The directory exists." << std::endl;
      return true;
    } else {
      //       std::cout << "The path you entered is a file." << std::endl;
      return false;
    }
  } else {
    //           std::cout << "Path doesn't exist." << std::endl;
    return false;
  }
}

void DATAIO::MakeDir(const char *dir_) {
#ifdef WIN32
  mkdir(dir_);
#else
  mkdir(dir_, S_IRWXU | S_IRWXG);
#endif
}

void DATAIO::RemoveFile(const char *path_file) {
  if (remove(path_file) != 0)
    std::cout << "Error! Unable to remove file:" << path_file << std::endl;
}
void DATAIO::RemoveFiles(const char *dir_) {
  DIR *dp;

  if ((dp = opendir(dir_))) {
    struct stat *buf = new struct stat;
    const struct dirent *entry;
    while ((entry = readdir(dp))) {
      std::string p = std::string(dir_) + "/" + entry->d_name;

      if (!stat(p.c_str(), buf)) {
        if (S_ISREG(buf->st_mode)) {
          RemoveFile(p.c_str());
        }
        if (S_ISDIR(buf->st_mode) &&
            // the following is to ensure we do not dive into directories "."
            // and ".."
            strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
          RemoveFiles(p.c_str());
          rmdir(p.c_str());
        }
      } else {
        std::cout << "ERROR in stat\n";
      }
    }
    delete buf;
    closedir(dp);
  } else {
    std::cout << "ERROR in opendir\n";
  }
}

void DATAIO::RemoveDir(const char *dir_) {
  RemoveFiles(dir_);
  rmdir(dir_);
}

void DATAIO::FileList(const char *dir_, std::vector<std::string> &list) {
  DIR *dp;

  if ((dp = opendir(dir_))) {
    struct stat *buf = new struct stat;
    const struct dirent *entry;
    while ((entry = readdir(dp))) {
      std::string p = std::string(dir_) + "/" + entry->d_name;

      if (!stat(p.c_str(), buf)) {
        if (S_ISREG(buf->st_mode)) {
          list.push_back(p);
        }
        if (S_ISDIR(buf->st_mode) &&
            // the following is to ensure we do not dive into directories "."
            // and ".."
            strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) {
          FileList(p.c_str(), list);
        }
      } else
        std::cout << "ERROR in stat" << std::endl;
    }
    delete buf;
    closedir(dp);
  } else {
    std::cout << "ERROR in opendir\n";
  }
}

void DATAIO::GetMatrixRowCol(const char *file_, const std::string &sep, size_t *row,
                             size_t *col) {
  std::ifstream file;

  (*row) = (*col) = 0;

  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        std::vector<std::string> v = split(line, sep, false);
        size_t col_t = v.size();
        (*row)++;

        if (col_t > (*col))
          (*col) = col_t;
        else
          continue;
      }
    }
  }
  file.close();
}

void DATAIO::ImportMatrix(const char *file_, const std::string &sep, matrix *data) {
  std::ifstream file;
  size_t row, col;

  GetMatrixRowCol(file_, sep, &row, &col);
  ResizeMatrix(data, row, col);

  row = col = 0;
  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        std::vector<std::string> tokenized = split(line, sep, false);
        for (col = 0; col < tokenized.size(); col++) {
          setMatrixValue(data, row, col, atof(tokenized[col].c_str()));
        }
        row++;
      }
    }
  }
  file.close();
}

void DATAIO::GetArrayOrderRowCol(const char *file_, const std::string &sep,
                                 size_t *order, size_t *row, size_t *col) {
  std::ifstream file;

  (*order) = (*row) = (*col) = 0;

  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    std::string delim = sep;
    size_t row_tmp = 0;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        if (line.starts_with("-") == true && line.size() == 1) {
          (*order)++;

          if (row_tmp > (*row)) {
            (*row) = row_tmp;
          }

          row_tmp = 0;
        } else {
          size_t col_t = split(line, delim, false).size();
          row_tmp++;

          if (col_t > (*col)) {
            (*col) = col_t;
          }
        }
      }
    }

    if (row_tmp > (*row)) {
      (*row) = row_tmp;
    }

    if ((*row) != 0 && (*col) != 0) {
      (*order)++;
    }
  }
  file.close();
}

void DATAIO::ImportTensor(const char *file_, const std::string &sep, tensor *data) {
  std::ifstream file;
  size_t order, row, col;

  GetArrayOrderRowCol(file_, sep, &order, &row, &col);

  for (size_t i = 0; i < order; i++) {
    AddTensorMatrix(data, row, col);
  }

  order = row = col = 0;
  file.open(file_, std::ios::in);

  if (!file.fail()) {
    std::string line;
    std::string delim = sep;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else if (line.starts_with("-") == true && line.size() == 1) {
        order++;
        row = 0;
      } else {
        std::vector<std::string> tokened = split(line, delim, false);
        for (col = 0; col < tokened.size(); col++) {
          setTensorValue(data, order, row, col, atof(tokened[col].c_str()));
        }
        row++;
      }
    }
  }
  file.close();
}

void DATAIO::GetVectorSize(const char *file_, size_t *size_) {
  std::ifstream file;

  (*size_) = 0;

  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        (*size_)++;
      }
    }
  }
  file.close();
}

void DATAIO::ImportStrvector(const char *file_, strvector *strv) {
  std::ifstream file;
  size_t size_;

  GetVectorSize(file_, &size_);
  StrVectorResize(strv, size_);

  size_ = 0;
  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        setStr(strv, size_, STrim(line).data());
        size_++;
      }
    }
  }
  file.close();
}

void DATAIO::ImportDvector(const char *file_, dvector *v) {
  std::ifstream file;
  size_t size_;

  GetVectorSize(file_, &size_);
  DVectorResize(v, size_);

  size_ = 0;
  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        setDVectorValue(v, size_, atof(STrim(line).c_str()));
        size_++;
      }
    }
  }
  file.close();
}

void DATAIO::ImportUIvector(const char *file_, uivector *v) {
  std::ifstream file;
  size_t size_;

  GetVectorSize(file_, &size_);

  size_ = 0;
  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        UIVectorAppend(v, atoi(STrim(line).c_str()));
        size_++;
      }
    }
  }
  file.close();
}

void DATAIO::GetDVectorListSize(const char *file_, uivector *sizes) {
  std::ifstream file;

  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    size_t sz = 0;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        if (line.starts_with("//") == true) { // skip line
          UIVectorAppend(sizes, sz);
          sz = 0;
        } else {
          sz++;
        }
      }
    }
  }
  file.close();
}

void DATAIO::ImportDvectorList(const char *file_, dvectorlist *lst) {
  std::ifstream file;
  uivector *sizes;
  initUIVector(&sizes);
  GetDVectorListSize(file_, sizes);

  size_t dvx = 0;
  dvector *dv;
  NewDVector(&dv, sizes->data[dvx]);
  dvx += 1;

  file.open(file_, std::ios::in);
  if (!file.fail()) {
    std::string line;
    size_t sz = 0;
    while (getline(file, line)) {
      if (line.starts_with("#") == true || line.empty()) { // skip line
        continue;
      } else {
        if (line.starts_with("//") == true) { // skip line
          DVectorListAppend(lst, dv);
          DelDVector(&dv);
          sz = 0;
          dvx += 1;
          NewDVector(&dv, sizes->data[dvx]);
        } else {
          dv->data[sz] = atof(STrim(line).c_str());
          sz++;
        }
      }
    }
  }

  DVectorListAppend(lst, dv);

  file.close();
  DelUIVector(&sizes);
  DelDVector(&dv);
}

void DATAIO::ImportPCAModel(const char *path_, PCAMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string tscore = base + "/T-Scores.txt";
  std::string ploadings = base + "/P-Loadings.txt";
  std::string expvar = base + "/ExpVar.txt";
  std::string columnscaling = base + "/ColumnScaling.txt";
  std::string columnaverage = base + "/ColumnAverage.txt";

  std::string sep = " \t";
  ImportMatrix(tscore.c_str(), sep, m->scores);
  ImportMatrix(ploadings.c_str(), sep, m->loadings);
  ImportDvector(expvar.c_str(), m->varexp);
  ImportDvector(columnscaling.c_str(), m->colscaling);
  ImportDvector(columnaverage.c_str(), m->colaverage);
}

void DATAIO::ImportPLSModel(const char *path_, PLSMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string tscore = base + "/X-T-Scores.txt";
  std::string ploadings = base + "/X-P-Loadings.txt";
  std::string weights = base + "/X-W-Weights.txt";
  std::string xexpvar = base + "/X-ExpVar.txt";
  std::string xcolumnaverage = base + "/X-ColumnAverage.txt";
  std::string xcolumnscaling = base + "/X-ColumnScaling.txt";

  std::string uscore = base + "/Y-U-Scores.txt";
  std::string qloadings = base + "/Y-Q-Loadings.txt";
  std::string ycolumnaverage = base + "/Y-ColumnAverage.txt";
  std::string ycolumnscaling = base + "/Y-ColumnScaling.txt";

  std::string bcoeff = base + "/b-Coefficients.txt";
  std::string r2y = base + "/r2y.txt";
  std::string sdec = base + "/sdec.txt";
  std::string recalc_y = base + "/Recalculated_y.txt";
  std::string recalc_residuals = base + "/Recalculated_Residuals.txt";

  std::string validatedq2y = base + "/Validated_q2y.txt";
  std::string validatedsdep = base + "/Validated_sdep.txt";
  std::string validatedbias = base + "/Validated_bias.txt";
  std::string validatedypred = base + "/Validated_Predicted_Y.txt";
  std::string validatedypred_residuals = base + "/Validated_Predicted_Residuals.txt";

  std::string yscrambling = base + "/YScrambling_r2q2y.txt";

  std::string sep = " \t";
  ImportMatrix(tscore.c_str(), sep, m->xscores);
  ImportMatrix(ploadings.c_str(), sep, m->xloadings);
  ImportMatrix(weights.c_str(), sep, m->xweights);
  ImportDvector(xexpvar.c_str(), m->xvarexp);
  ImportDvector(xcolumnaverage.c_str(), m->xcolaverage);
  ImportDvector(xcolumnscaling.c_str(), m->xcolscaling);

  //   ImportDvector(path_, "/Y-ExpVar.txt", m->yvarexp);
  ImportMatrix(uscore.c_str(), sep, m->yscores);
  ImportMatrix(qloadings.c_str(), sep, m->yloadings);
  ImportDvector(ycolumnaverage.c_str(), m->ycolaverage);
  ImportDvector(ycolumnscaling.c_str(), m->ycolscaling);

  ImportDvector(bcoeff.c_str(), m->b);

  ImportMatrix(r2y.c_str(), sep, m->r2y_recalculated);
  ImportMatrix(sdec.c_str(), sep, m->sdec);
  ImportMatrix(recalc_y.c_str(), sep, m->recalculated_y);
  ImportMatrix(recalc_residuals.c_str(), sep, m->recalc_residuals);

  ImportMatrix(validatedq2y.c_str(), sep, m->q2y);
  ImportMatrix(validatedsdep.c_str(), sep, m->sdep);
  ImportMatrix(validatedbias.c_str(), sep, m->bias);
  ImportMatrix(validatedypred.c_str(), sep, m->predicted_y);
  ImportMatrix(validatedypred_residuals.c_str(), sep, m->pred_residuals);

  ImportMatrix(yscrambling.c_str(), sep, m->yscrambling);
}

void DATAIO::ImportUPCAModel(const char *path_, UPCAMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string tscore = base + "/T-Scores.txt";
  std::string ploadings = base + "/P-Loadings.txt";
  std::string expvar = base + "/ExpVar.txt";
  std::string columnscaling = base + "/ColumnScaling.txt";
  std::string columnaverage = base + "/ColumnAverage.txt";

  std::string sep = " \t";
  std::cout << "Import PCA " << path_ << std::endl;
  ImportMatrix(tscore.c_str(), sep, m->scores);
  ImportTensor(ploadings.c_str(), sep, m->loadings);
  ImportDvector(expvar.c_str(), m->varexp);

  ImportDvectorList(columnscaling.c_str(), m->colscaling);
  ImportDvectorList(columnaverage.c_str(), m->colaverage);
}

void DATAIO::ImportUPLSModel(const char *path_, UPLSMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string tscore = base + "/X-T-Scores.txt";
  std::string ploadings = base + "/X-P-Loadings.txt";
  std::string weights = base + "/X-W-Weights.txt";
  std::string xexpvar = base + "/X-ExpVar.txt";
  std::string xcolumnaverage = base + "/X-ColumnAverage.txt";
  std::string xcolumnscaling = base + "/X-ColumnScaling.txt";

  std::string uscore = base + "/Y-U-Scores.txt";
  std::string qloadings = base + "/Y-Q-Loadings.txt";
  std::string ycolumnaverage = base + "/Y-ColumnAverage.txt";
  std::string ycolumnscaling = base + "/Y-ColumnScaling.txt";

  std::string bcoeff = base + "/b-Coefficients.txt";
  std::string r2x = base + "/r2x.txt";
  std::string r2y = base + "/r2y.txt";
  std::string sdec = base + "/sdec.txt";
  std::string recalc_y = base + "/Recalculated_y.txt";
  std::string recalc_residuals = base + "/Recalculated_Residuals.txt";

  std::string validatedr2x = base + "/Validated_r2x.txt";
  std::string validatedq2y = base + "/Validated_q2y.txt";
  std::string validatedsdep = base + "/Validated_sdep.txt";
  std::string validatedypred = base + "/Validated_Predicted_Y.txt";
  std::string validatedypred_residuals = base + "/Validated_Predicted_Residuals.txt";

  std::string yscramblingq2y = base + "/YScrambling_q2y.txt";
  std::string yscramblingsdep = base + "/YScrambling_sdep.txt";

  std::string sep = " \t";
  ImportMatrix(tscore.c_str(), sep, m->xscores);
  ImportTensor(ploadings.c_str(), sep, m->xloadings);
  ImportTensor(weights.c_str(), sep, m->xweights);
  ImportDvector(xexpvar.c_str(), m->xvarexp);
  ImportDvectorList(xcolumnaverage.c_str(), m->xcolaverage);
  ImportDvectorList(xcolumnscaling.c_str(), m->xcolscaling);

  //   ImportDvector(path_, "/Y-ExpVar.txt", m->yvarexp);
  ImportMatrix(uscore.c_str(), sep, m->yscores);
  ImportTensor(qloadings.c_str(), sep, m->yloadings);
  ImportDvectorList(ycolumnaverage.c_str(), m->ycolaverage);
  ImportDvectorList(ycolumnscaling.c_str(), m->ycolscaling);

  ImportDvector(bcoeff.c_str(), m->b);

  ImportDvector(r2x.c_str(), m->r2x_model);
  ImportTensor(r2y.c_str(), sep, m->r2y_model);
  ImportTensor(sdec.c_str(), sep, m->sdec);
  ImportTensor(recalc_y.c_str(), sep, m->recalculated_y);
  ImportTensor(recalc_residuals.c_str(), sep, m->recalc_residuals);

  ImportDvector(validatedr2x.c_str(), m->r2x_validation);
  ImportTensor(validatedq2y.c_str(), sep, m->q2y);
  ImportTensor(validatedsdep.c_str(), sep, m->sdep);
  ImportTensor(validatedypred.c_str(), sep, m->predicted_y);
  ImportTensor(validatedypred_residuals.c_str(), sep, m->pred_residuals);

  ImportTensor(yscramblingq2y.c_str(), sep, m->q2y_yscrambling);
  ImportTensor(yscramblingsdep.c_str(), sep, m->sdep_yscrambling);
}

void DATAIO::ImportMLRModel(const char *path_, MLRMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string bcoeff = base + "/b-Coefficients.txt";
  std::string r2y = base + "/r2y.txt";
  std::string sdec = base + "/sdec.txt";
  std::string recalc_y = base + "/Recalculated_y.txt";
  std::string recalc_residuals = base + "/Recalculated_Residuals.txt";
  std::string ymean = base + "/Y_Mean.txt";

  std::string validatedq2y = base + "/Validated_q2y.txt";
  std::string validatedsdep = base + "/Validated_sdep.txt";
  std::string validatedbias = base + "/Validated_bias.txt";
  std::string validatedypred = base + "/Validated_Predicted_Y.txt";
  std::string validatedypred_residuals = base + "/Validated_Predicted_Residuals.txt";

  std::string r2q2scrambling = base + "/YScrambling_r2q2y.txt";

  std::string sep = " \t";

  ImportMatrix(bcoeff.c_str(), sep, m->b);

  ImportDvector(r2y.c_str(), m->r2y_model);
  ImportDvector(sdec.c_str(), m->sdec);
  ImportMatrix(recalc_y.c_str(), sep, m->recalculated_y);
  ImportMatrix(recalc_residuals.c_str(), sep, m->recalc_residuals);
  ImportDvector(ymean.c_str(), m->ymean);

  ImportDvector(validatedq2y.c_str(), m->q2y);
  ImportDvector(validatedsdep.c_str(), m->sdep);
  ImportMatrix(validatedypred.c_str(), sep, m->predicted_y);
  ImportMatrix(validatedypred_residuals.c_str(), sep, m->pred_residuals);
  ImportDvector(validatedbias.c_str(), m->bias);

  ImportMatrix(r2q2scrambling.c_str(), sep, m->r2q2scrambling);
}

void DATAIO::ImportLDAModel(const char *path_, LDAMODEL *m) {
  if (!path_) return;
  uivector *otherinfo;
  std::string base(path_);
  std::string roc = base + "/ROC.txt";
  std::string roc_aucs = base + "/ROCAUCS.txt";
  std::string pr = base + "/PRECISIONRECALL.txt";
  std::string pr_aucs = base + "/PRECISIONRECALLAUCS.txt";

  std::string recalculated_y = base + "/RECALCULATEDY.txt";
  std::string recalculated_residuals = base + "/RECALCULATED_RESIDUALS.txt";
  std::string predicted_y = base + "/PREDICTEDY.txt";
  std::string predicted_residuals = base + "/PREDICTED_RESIDUALS.txt";

  std::string eval = base + "/EVAL.txt";
  std::string mu = base + "/MU.txt";
  std::string evect = base + "/EVECT.txt";
  std::string mnpdf = base + "/MNPDF.txt";
  std::string features = base + "/FEATURES.txt";
  std::string fmean = base + "/FEATUREMEAN.txt";
  std::string fsdev = base + "/FEATURESDEV.txt";

  std::string inv_cov = base + "/INVCOV.txt";
  std::string others = base + "/MODINFO.txt";
  std::string classid = base + "/CLASSID.txt";

  std::string sep = " \t";

  ImportTensor(roc.c_str(), sep, m->roc);
  ImportDvector(roc_aucs.c_str(), m->roc_aucs);
  ImportTensor(pr.c_str(), sep, m->pr);
  ImportDvector(pr_aucs.c_str(), m->pr_aucs);

  ImportMatrix(recalculated_y.c_str(), sep, m->recalculated_y);
  ImportMatrix(recalculated_residuals.c_str(), sep, m->recalculated_residuals);
  ImportMatrix(predicted_y.c_str(), sep, m->predicted_y);
  ImportMatrix(predicted_residuals.c_str(), sep, m->predicted_residuals);

  ImportDvector(eval.c_str(), m->eval);
  ImportMatrix(mu.c_str(), sep, m->mu);
  ImportMatrix(evect.c_str(), sep, m->evect);
  ImportTensor(mnpdf.c_str(), sep, m->mnpdf);

  ImportTensor(features.c_str(), sep, m->features);
  ImportMatrix(fmean.c_str(), sep, m->fmean);
  ImportMatrix(fsdev.c_str(), sep, m->fsdev);
  ImportMatrix(inv_cov.c_str(), sep, m->inv_cov);

  initUIVector(&otherinfo);
  ImportUIvector(others.c_str(), otherinfo);
  m->nclass = otherinfo->data[0];
  m->class_start = otherinfo->data[1];
  DelUIVector(&otherinfo);
  ImportUIvector(classid.c_str(), m->classid);
}

void DATAIO::WriteStringList(const char *file_, const std::vector<std::string> &strlst) {
  std::fstream out;
  out.open(file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for (size_t i = 0; i < strlst.size(); i++) {
    out << strlst[i] << std::endl;
  }
  out.close();
}

void DATAIO::WriteComments(const char *file_, const std::vector<std::string> &strvect) {
  std::fstream out;
  out.open(file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for (size_t i = 0; i < strvect.size(); i++) {
    out << "#" << strvect[i] << std::endl;
  }
  out.close();
}

void DATAIO::WriteDvector(const char *file_, dvector *v) {
  std::fstream out;
  out.open(file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for (size_t i = 0; i < v->size; i++) {
    out << getDVectorValue(v, i) << std::endl;
  }
  out.close();
}

void DATAIO::WriteUIvector(const char *file_, uivector *v) {
  std::fstream out;
  out.open(file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for (size_t i = 0; i < v->size; i++) {
    out << getUIVectorValue(v, i) << std::endl;
  }
  out.close();
}

void DATAIO::WriteDVectorList(const char *file_, dvectorlist *lst) {
  std::fstream out;
  out.open(file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for (size_t i = 0; i < lst->size; i++) {
    for (size_t j = 0; j < lst->d[i]->size; j++) {
      out << lst->d[i]->data[j] << std::endl;
    }

    if (i < lst->size - 1) {
      out << "//" << std::endl;
    }
  }
  out.close();
}

void DATAIO::WriteMatrix(const char *file_, matrix *m) {
  std::fstream out;
  out.open(file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);

  for (size_t i = 0; i < m->row; i++) {
    for (size_t j = 0; j < m->col; j++) {
      out << m->data[i][j];
      if (j < m->col - 1)
        out << "\t";
    }
    out << std::endl;
  }
  out.close();
}

void DATAIO::WriteTensor(const char *file_, tensor *a) {
  std::fstream out;
  out.open(file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);

  for (size_t k = 0; k < a->order; k++) {
    for (size_t i = 0; i < a->m[k]->row; i++) {
      for (size_t j = 0; j < a->m[k]->col; j++) {
        out << getTensorValue(a, k, i, j);
        if (j < a->m[k]->col - 1)
          out << "\t";
      }
      out << std::endl;
    }
    if (k < a->order - 1)
      out << "-" << std::endl;
  }
  out.close();
}

void DATAIO::WritePCAModel(const char *path_, PCAMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string tscore = base + "/T-Scores.txt";
  std::string ploadings = base + "/P-Loadings.txt";
  std::string expvar = base + "/ExpVar.txt";
  std::string columnscaling = base + "/ColumnScaling.txt";
  std::string columnaverage = base + "/ColumnAverage.txt";

  if (DirExists(path_) == true) {
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(tscore.c_str(), m->scores);
  WriteMatrix(ploadings.c_str(), m->loadings);
  WriteDvector(expvar.c_str(), m->varexp);
  WriteDvector(columnscaling.c_str(), m->colscaling);
  WriteDvector(columnaverage.c_str(), m->colaverage);
}

void DATAIO::WritePLSModel(const char *path_, PLSMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string tscore = base + "/X-T-Scores.txt";
  std::string ploadings = base + "/X-P-Loadings.txt";
  std::string weights = base + "/X-W-Weights.txt";
  std::string xexpvar = base + "/X-ExpVar.txt";
  std::string xcolumnaverage = base + "/X-ColumnAverage.txt";
  std::string xcolumnscaling = base + "/X-ColumnScaling.txt";

  std::string uscore = base + "/Y-U-Scores.txt";
  std::string qloadings = base + "/Y-Q-Loadings.txt";
  std::string ycolumnaverage = base + "/Y-ColumnAverage.txt";
  std::string ycolumnscaling = base + "/Y-ColumnScaling.txt";

  std::string bcoeff = base + "/b-Coefficients.txt";
  std::string r2y = base + "/r2y.txt";
  std::string sdec = base + "/sdec.txt";
  std::string recalc_y = base + "/Recalculated_y.txt";
  std::string recalc_residuals = base + "/Recalculated_Residuals.txt";

  std::string validatedq2y = base + "/Validated_q2y.txt";
  std::string validatedsdep = base + "/Validated_sdep.txt";
  std::string validatedbias = base + "/Validated_bias.txt";
  std::string validatedypred = base + "/Validated_Predicted_Y.txt";
  std::string validatedypred_residuals = base + "/Validated_Predicted_Residuals.txt";

  std::string yscrambling = base + "/YScrambling_r2q2y.txt";

  if (DirExists(path_) == true) {
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(tscore.c_str(), m->xscores);
  WriteMatrix(ploadings.c_str(), m->xloadings);
  WriteMatrix(weights.c_str(), m->xweights);
  WriteDvector(xexpvar.c_str(), m->xvarexp);
  WriteDvector(xcolumnaverage.c_str(), m->xcolaverage);
  WriteDvector(xcolumnscaling.c_str(), m->xcolscaling);

  //   WriteDvector(path_, "/Y-ExpVar.txt", m->yvarexp);
  WriteMatrix(uscore.c_str(), m->yscores);
  WriteMatrix(qloadings.c_str(), m->yloadings);
  WriteDvector(ycolumnaverage.c_str(), m->ycolaverage);
  WriteDvector(ycolumnscaling.c_str(), m->ycolscaling);

  WriteDvector(bcoeff.c_str(), m->b);

  WriteMatrix(r2y.c_str(), m->r2y_recalculated);
  WriteMatrix(sdec.c_str(), m->sdec);
  WriteMatrix(recalc_y.c_str(), m->recalculated_y);
  WriteMatrix(recalc_residuals.c_str(), m->recalc_residuals);

  WriteMatrix(validatedq2y.c_str(), m->q2y);
  WriteMatrix(validatedsdep.c_str(), m->sdep);
  WriteMatrix(validatedbias.c_str(), m->bias);
  WriteMatrix(validatedypred.c_str(), m->predicted_y);
  WriteMatrix(validatedypred_residuals.c_str(), m->pred_residuals);

  WriteMatrix(yscrambling.c_str(), m->yscrambling);
}

void DATAIO::WriteUPCAModel(const char *path_, UPCAMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string tscore = base + "/T-Scores.txt";
  std::string ploadings = base + "/P-Loadings.txt";
  std::string expvar = base + "/ExpVar.txt";
  std::string columnscaling = base + "/ColumnScaling.txt";
  std::string columnaverage = base + "/ColumnAverage.txt";

  if (DirExists(path_) == true) {
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(tscore.c_str(), m->scores);
  WriteTensor(ploadings.c_str(), m->loadings);
  WriteDvector(expvar.c_str(), m->varexp);
  WriteDVectorList(columnscaling.c_str(), m->colscaling);
  WriteDVectorList(columnaverage.c_str(), m->colaverage);
}

void DATAIO::WriteUPLSModel(const char *path_, UPLSMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string tscore = base + "/X-T-Scores.txt";
  std::string ploadings = base + "/X-P-Loadings.txt";
  std::string weights = base + "/X-W-Weights.txt";
  std::string xexpvar = base + "/X-ExpVar.txt";
  std::string xcolumnaverage = base + "/X-ColumnAverage.txt";
  std::string xcolumnscaling = base + "/X-ColumnScaling.txt";

  std::string uscore = base + "/Y-U-Scores.txt";
  std::string qloadings = base + "/Y-Q-Loadings.txt";
  std::string ycolumnaverage = base + "/Y-ColumnAverage.txt";
  std::string ycolumnscaling = base + "/Y-ColumnScaling.txt";

  std::string bcoeff = base + "/b-Coefficients.txt";
  std::string r2x = base + "/r2x.txt";
  std::string r2y = base + "/r2y.txt";
  std::string sdec = base + "/sdec.txt";
  std::string recalc_y = base + "/Recalculated_y.txt";
  std::string recalc_residuals = base + "/Recalculated_Residuals.txt";

  std::string validatedypred = base + "/Validated_Predicted_Y.txt";
  std::string validatedypred_residuals = base + "/Validated_Predicted_Residuals.txt";
  std::string validatedr2x = base + "/Validated_r2x.txt";
  std::string validatedq2y = base + "/Validated_q2y.txt";
  std::string validatedsdep = base + "/Validated_sdep.txt";
  std::string yscramblingq2y = base + "/YScrambling_q2y.txt";
  std::string yscramblingsdep = base + "/YScrambling_sdep.txt";

  if (DirExists(path_) == true) {
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(tscore.c_str(), m->xscores);
  WriteTensor(ploadings.c_str(), m->xloadings);
  WriteTensor(weights.c_str(), m->xweights);
  WriteDvector(xexpvar.c_str(), m->xvarexp);
  WriteDVectorList(xcolumnaverage.c_str(), m->xcolaverage);
  WriteDVectorList(xcolumnscaling.c_str(), m->xcolscaling);

  //   WriteDvector(path_, "/Y-ExpVar.txt", m->yvarexp);
  WriteMatrix(uscore.c_str(), m->yscores);
  WriteTensor(qloadings.c_str(), m->yloadings);
  WriteDVectorList(ycolumnaverage.c_str(), m->ycolaverage);
  WriteDVectorList(ycolumnscaling.c_str(), m->ycolscaling);

  WriteDvector(bcoeff.c_str(), m->b);

  WriteDvector(r2x.c_str(), m->r2x_model);
  WriteTensor(r2y.c_str(), m->r2y_model);
  WriteTensor(sdec.c_str(), m->sdec);
  WriteTensor(recalc_y.c_str(), m->recalculated_y);
  WriteTensor(recalc_residuals.c_str(), m->recalc_residuals);

  WriteDvector(validatedr2x.c_str(), m->r2x_validation);
  WriteTensor(validatedq2y.c_str(), m->q2y);
  WriteTensor(validatedsdep.c_str(), m->sdep);
  WriteTensor(validatedypred.c_str(), m->predicted_y);
  WriteTensor(validatedypred_residuals.c_str(), m->pred_residuals);

  WriteTensor(yscramblingq2y.c_str(), m->q2y_yscrambling);
  WriteTensor(yscramblingsdep.c_str(), m->sdep_yscrambling);
}

void DATAIO::WriteMLRModel(const char *path_, MLRMODEL *m) {
  if (!path_) return;
  std::string base(path_);
  std::string bcoeff = base + "/b-Coefficients.txt";
  std::string r2y = base + "/r2y.txt";
  std::string sdec = base + "/sdec.txt";
  std::string recalc_y = base + "/Recalculated_y.txt";
  std::string recalc_residuals = base + "/Recalculated_Residuals.txt";
  std::string ymean = base + "/Y_Mean.txt";

  std::string validatedypred = base + "/Validated_Predicted_Y.txt";
  std::string validatedypred_residuals = base + "/Validated_Predicted_Residuals.txt";
  std::string validatedq2y = base + "/Validated_q2y.txt";
  std::string validatedsdep = base + "/Validated_sdep.txt";
  std::string validatedbias = base + "/Validated_bias.txt";
  std::string r2q2scrambling = base + "/YScrambling_r2q2y.txt";

  if (DirExists(path_) == true) {
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(bcoeff.c_str(), m->b);
  WriteDvector(r2y.c_str(), m->r2y_model);
  WriteDvector(sdec.c_str(), m->sdec);
  WriteMatrix(recalc_y.c_str(), m->recalculated_y);
  WriteMatrix(recalc_residuals.c_str(), m->recalc_residuals);
  WriteDvector(ymean.c_str(), m->ymean);

  WriteDvector(validatedq2y.c_str(), m->q2y);
  WriteDvector(validatedsdep.c_str(), m->sdep);
  WriteMatrix(validatedypred.c_str(), m->predicted_y);
  WriteMatrix(validatedypred_residuals.c_str(), m->pred_residuals);
  WriteDvector(validatedbias.c_str(), m->bias);

  WriteMatrix(r2q2scrambling.c_str(), m->r2q2scrambling);
}

void DATAIO::WriteLDAModel(const char *path_, LDAMODEL *m) {
  if (!path_) return;
  uivector *otherinfo;
  std::string base(path_);
  std::string roc = base + "/ROC.txt";
  std::string roc_aucs = base + "/ROCAUCS.txt";
  std::string pr = base + "/PRECISIONRECALL.txt";
  std::string pr_aucs = base + "/PRECISIONRECALLAUCS.txt";
  std::string recalculated_y = base + "/RECALCULATEDY.txt";
  std::string recalculated_residuals = base + "/RECALCULATED_RESIDUALS.txt";
  std::string predicted_y = base + "/PREDICTEDY.txt";
  std::string predicted_residuals = base + "/PREDICTED_RESIDUALS.txt";
  std::string eval = base + "/EVAL.txt";
  std::string mu = base + "/MU.txt";
  std::string evect = base + "/EVECT.txt";
  std::string mnpdf = base + "/MNPDF.txt";
  std::string features = base + "/FEATURES.txt";
  std::string fmean = base + "/FEATUREMEAN.txt";
  std::string fsdev = base + "/FEATURESDEV.txt";
  std::string inv_cov = base + "/INVCOV.txt";
  std::string others = base + "/MODINFO.txt";
  std::string classid = base + "/CLASSID.txt";

  if (DirExists(path_) == true) {
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteTensor(roc.c_str(), m->roc);
  WriteDvector(roc_aucs.c_str(), m->roc_aucs);
  WriteTensor(pr.c_str(), m->pr);
  WriteDvector(pr_aucs.c_str(), m->pr_aucs);

  WriteMatrix(recalculated_y.c_str(), m->recalculated_y);
  WriteMatrix(recalculated_residuals.c_str(), m->recalculated_residuals);
  WriteMatrix(predicted_y.c_str(), m->predicted_y);
  WriteMatrix(predicted_residuals.c_str(), m->predicted_residuals);

  WriteDvector(eval.c_str(), m->eval);
  WriteMatrix(mu.c_str(), m->mu);
  WriteMatrix(evect.c_str(), m->evect);
  WriteTensor(mnpdf.c_str(), m->mnpdf);

  WriteTensor(features.c_str(), m->features);
  WriteMatrix(fmean.c_str(), m->fmean);
  WriteMatrix(fsdev.c_str(), m->fsdev);
  WriteMatrix(inv_cov.c_str(), m->inv_cov);

  NewUIVector(&otherinfo, 2);
  otherinfo->data[0] = m->nclass;
  otherinfo->data[1] = m->class_start;
  WriteUIvector(others.c_str(), otherinfo);
  DelUIVector(&otherinfo);
  WriteUIvector(classid.c_str(), m->classid);
}
