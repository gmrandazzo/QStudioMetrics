#include "DATAIO.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <sys/stat.h>
#include <cstring>
#include <dirent.h>

#ifdef WIN32
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#ifdef MINGW32
#include <direct.h>
#include <dir.h>
#endif
#else
#include <sys/types.h>
#include <unistd.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "scientific.h"

#define MAXCHARS 2048

inline char *Trim(char *s)
{
    char *ptr;
    if (!s)
        return NULL;   // handle NULL string
    if (!*s)
        return s;      // handle empty string
    for (ptr = s + strlen(s) - 1; (ptr >= s) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return s;
}

inline std::string STrim(const std::string &s)
{
    char *ptr;
    if (!s.empty())
      return s; // handle empty string

    char *str = (char*) s.c_str();
    for(ptr = str + strlen(str) - 1; (ptr >= str) && isspace(*ptr); --ptr);
    ptr[1] = '\0';
    return (std::string)str;
}

std::vector<std::string> DATAIO::split(const std::string& s, const std::string& delim, const bool keep_empty = true)
{
  char *token;
  std::vector<std::string> result;

  token = strtok((char*)s.c_str(), (char*)delim.c_str());

  while(token != NULL){
    result.push_back(token);
    token = strtok(NULL, (char*)delim.c_str());
  }

  return result;
}

bool DATAIO::FileExists(char* file_)
{
  std::fstream foo;
  foo.open(file_);

  if(foo.is_open() == true){
    foo.close();
    return true;
  }
  else{
    return false;
  }
}

bool DATAIO::DirExists(char *dir_)
{
  if(access(dir_, 0) == 0){
    struct stat status;
    stat(dir_, &status);
    if(status.st_mode & S_IFDIR ){
//         std::cout << "The directory exists." << std::endl;
      return true;
    }
    else{
//       std::cout << "The path you entered is a file." << std::endl;
      return false;
    }
  }
  else{
//           std::cout << "Path doesn't exist." << std::endl;
    return false;
  }
}

void DATAIO::MakeDir(char *dir_)
{
  #ifdef WIN32
  mkdir(dir_);
  #else
  mkdir(dir_, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
  #endif
}

void DATAIO::RemoveFile(char *path_file)
{
  if(remove(path_file) != 0)
    std::cout << "Error! Unable to remove file:" << path_file << std::endl;
}
void DATAIO::RemoveFiles(char *dir_)
{
  struct dirent *entry;
  DIR *dp;

  if((dp = opendir(dir_))){
    struct stat *buf = new struct stat;
    while((entry = readdir(dp))){
      char p[MAXCHARS];
      strcpy(p, dir_);
      strcat(p, "/");
      strcat(p, entry->d_name);

      if(!stat(p, buf)){
        if(S_ISREG(buf->st_mode)){
          RemoveFile(p);
        }
        if(S_ISDIR(buf->st_mode) &&
      // the following is to ensure we do not dive into directories "." and ".."
           strcmp(entry->d_name, ".")  && strcmp(entry->d_name, "..")){
          RemoveFiles(p);
          rmdir(p);
        }
      }
      else{
        std::cout << "ERROR in stat\n";
      }
    }
    delete buf;
    closedir(dp);
  }
  else{
    std::cout << "ERROR in opendir\n";
  }
}

void DATAIO::RemoveDir(char *dir_)
{
  RemoveFiles(dir_);
  rmdir(dir_);
}


void DATAIO::FileList(char *dir_, std::vector<std::string> &list)
{
  struct dirent *entry;
  DIR *dp;

  if((dp = opendir(dir_))){
    struct stat *buf = new struct stat;
    while((entry = readdir(dp))){
      char p[MAXCHARS];
      strcpy(p, dir_);
      strcat(p, "/");
      strcat(p, entry->d_name);

      if(!stat(p, buf)){
        if(S_ISREG(buf->st_mode)){
          list.push_back(p);
        }
        if(S_ISDIR(buf->st_mode) &&
      // the following is to ensure we do not dive into directories "." and ".."
                  strcmp(entry->d_name, ".")  && strcmp(entry->d_name, "..") ){
            FileList(p, list);
        }
      }
      else
        std::cout << "ERROR in stat"<< std::endl;
    }
    delete buf;
    closedir(dp);
  }
  else{
    std::cout << "ERROR in opendir\n";
  }
}

void DATAIO::GetMatrixRowCol(char *file_, const std::string  &sep, size_t *row, size_t *col)
{
  std::ifstream  file;
  std::string line;

  (*row) = (*col) = 0;

  file.open(file_, std::ios::in);
  if(!file.fail()){
    while(getline(file, line)) {
      if(line.find("#") == 0 || line.empty()){ // skip line
        continue;
      }
      else{
        std::vector<std::string> v = split(line, sep, false);
        size_t col_t = v.size();
        (*row)++;

        if(col_t > (*col))
          (*col) = col_t;
        else
          continue;
      }
    }
  }
  file.close();

}

void DATAIO::ImportMatrix(char *file_, const std::string &sep, matrix *data)
{
  std::ifstream  file;
  std::string line;
  size_t row, col;

  GetMatrixRowCol(file_, sep, &row, &col);
  ResizeMatrix(&data, row, col);

  row = col = 0;
  file.open(file_, std::ios::in);
  if(!file.fail()) {
    while( getline(file, line) ) {
      if(line.find("#") == 0 || line.empty()){ // skip line
        continue;
      }
      else{
        std::vector<std::string> tokenized = split(line, sep, false);
        for(col = 0; col < tokenized.size(); col++){
          setMatrixValue(data, row, col, atof(tokenized[col].c_str()));
        }
        row++;
      }
    }
  }
  file.close();
}

void DATAIO::GetArrayOrderRowCol(char *file_, const std::string  &sep, size_t *order, size_t *row, size_t *col)
{
  std::ifstream  file;
  std::string line;
  std::string delim = sep;
  size_t row_tmp = 0;

  (*order) = (*row) = (*col) = 0;

  file.open(file_, std::ios::in);
  if(!file.fail()){
    while(getline(file, line)){
      if(line.find("#") == 0 || line.empty()) { // skip line
        continue;
      }
      else{
        if(line.find("-") == 0 && line.size() == 1){
          (*order)++;

          if(row_tmp > (*row)){
            (*row) = row_tmp;
          }

          row_tmp = 0;
        }
        else{
          size_t col_t = split(line, delim, false).size();
          row_tmp++;

          if(col_t > (*col)){
            (*col) = col_t;
          }
        }
      }
    }

    if(row_tmp > (*row)){
      (*row) = row_tmp;
    }

    if((*row) != 0 && (*col) != 0){
      (*order)++;
    }
  }
  file.close();
}

void DATAIO::ImportTensor(char *file_, const std::string  &sep, tensor *data)
{
  std::ifstream  file;
  std::string line;
  std::string delim = sep;
  size_t order, row, col;

  GetArrayOrderRowCol(file_, sep, &order, &row, &col);

  for(size_t i = 0; i < order; i++){
    AddTensorMatrix(&data, row, col);
  }

  order = row = col = 0;
  file.open(file_, std::ios::in);

  if(!file.fail()) {
    while(getline(file, line) ){
      if(line.find("#") == 0 || line.empty()){ // skip line
        continue;
      }
      else if(line.find("-") == 0 && line.size() == 1){
        order++;
        row = 0;
      }
      else{
        std::vector<std::string> tokened = split(line, delim, false);
        for(col = 0; col < tokened.size(); col++){
          setTensorValue(data, order, row, col, atof(tokened[col].c_str()));
        }
        row++;
      }
    }
  }
  file.close();
}


void DATAIO::GetVectorSize(char *file_, size_t *size_)
{
  std::ifstream  file;
  std::string line;

  (*size_) = 0;

  file.open(file_, std::ios::in);
  if(!file.fail()) {
    while(getline(file, line)) {
      if(line.find("#") == 0 || line.empty()){ // skip line
        continue;
      }
      else{
        (*size_)++;
      }
    }
  }
  file.close();
}

void DATAIO::ImportStrvector(char *file_, strvector **strv)
{
  std::ifstream  file;
  std::string line;
  size_t size_;

  GetVectorSize(file_, &size_);
  StrVectorResize(strv, size_);

  size_ = 0;
  file.open(file_, std::ios::in);
  if(!file.fail()) {
    while(getline(file, line)){
      if(line.find("#") == 0 || line.empty()){ // skip line
        continue;
      }
      else{
        setStr((*strv), size_, Trim((char*)line.c_str()));
        size_++;
      }
    }
  }
  file.close();
}

void DATAIO::ImportDvector(char *file_, dvector *v)
{
  std::ifstream  file;
  std::string line;
  size_t size_;

  GetVectorSize(file_, &size_);
  DVectorResize(&v, size_);

  size_ = 0;
  file.open(file_, std::ios::in);
  if(!file.fail()) {
    while(getline(file, line)){
      if(line.find("#") == 0 || line.empty()){ // skip line
        continue;
      }
      else{
        setDVectorValue(v, size_, atof(Trim((char*)line.c_str())));
        size_++;
      }
    }
  }
  file.close();
}

void DATAIO::ImportUIvector(char *file_, uivector *v)
{
  std::ifstream  file;
  std::string line;
  size_t size_;

  GetVectorSize(file_, &size_);

  size_ = 0;
  file.open(file_, std::ios::in);
  if(!file.fail()) {
    while(getline(file, line)){
      if(line.find("#") == 0 || line.empty()){ // skip line
        continue;
      }
      else{
        UIVectorAppend(&v, atoi(Trim((char*)line.c_str())));
        size_++;
      }
    }
  }
  file.close();
}

void DATAIO::ImportPCAModel(char *path_, PCAMODEL* m)
{
  char tscore[MAXCHARS], ploadings[MAXCHARS], expvar[MAXCHARS], columnscaling[MAXCHARS], columnaverage[MAXCHARS];

  strcpy(tscore, path_); strcat(tscore, "/T-Scores.txt");
  strcpy(ploadings, path_); strcat(ploadings, "/P-Loadings.txt");
  strcpy(expvar, path_); strcat(expvar, "/ExpVar.txt");
  strcpy(columnscaling, path_); strcat(columnscaling, "/ColumnScaling.txt");
  strcpy(columnaverage, path_); strcat(columnaverage, "/ColumnAverage.txt");

  std::string sep = " \t";
  ImportMatrix(tscore, sep, m->scores);
  ImportMatrix(ploadings, sep, m->loadings);
  ImportDvector(expvar, m->varexp);
  ImportDvector(columnscaling, m->colscaling);
  ImportDvector(columnaverage, m->colaverage);
}

void DATAIO::ImportPLSModel(char *path_, PLSMODEL* m)
{
  char tscore[MAXCHARS], ploadings[MAXCHARS], weights[MAXCHARS], xexpvar[MAXCHARS], xcolumnscaling[MAXCHARS], xcolumnaverage[MAXCHARS],
       uscore[MAXCHARS], qloadings[MAXCHARS], ycolumnscaling[MAXCHARS], ycolumnaverage[MAXCHARS],
       bcoeff[MAXCHARS], r2y[MAXCHARS], sdec[MAXCHARS], validatedypred[MAXCHARS], validatedq2y[MAXCHARS], validatedsdep[MAXCHARS], validatedbias[MAXCHARS], yscrambling[MAXCHARS], recalc_y[MAXCHARS], recalc_residuals[MAXCHARS], validatedypred_residuals[MAXCHARS];

  strcpy(tscore, path_); strcat(tscore, "/X-T-Scores.txt");
  strcpy(ploadings, path_); strcat(ploadings, "/X-P-Loadings.txt");
  strcpy(weights, path_); strcat(weights, "/X-W-Weights.txt");
  strcpy(xexpvar, path_); strcat(xexpvar, "/X-ExpVar.txt");
  strcpy(xcolumnaverage, path_); strcat(xcolumnaverage, "/X-ColumnAverage.txt");
  strcpy(xcolumnscaling, path_); strcat(xcolumnscaling, "/X-ColumnScaling.txt");

  strcpy(uscore, path_); strcat(uscore, "/Y-U-Scores.txt");
  strcpy(qloadings, path_); strcat(qloadings, "/Y-Q-Loadings.txt");
  strcpy(ycolumnaverage, path_); strcat(ycolumnaverage, "/Y-ColumnAverage.txt");
  strcpy(ycolumnscaling, path_); strcat(ycolumnscaling, "/Y-ColumnScaling.txt");

  strcpy(bcoeff, path_); strcat(bcoeff, "/b-Coefficients.txt");
  strcpy(r2y, path_); strcat(r2y, "/r2y.txt");
  strcpy(sdec, path_); strcat(sdec, "/sdec.txt");
  strcpy(recalc_y, path_); strcat(recalc_y, "/Recalculated_y.txt");
  strcpy(recalc_residuals, path_); strcat(recalc_residuals, "/Recalculated_Residuals.txt");

  strcpy(validatedq2y, path_); strcat(validatedq2y, "/Validated_q2y.txt");
  strcpy(validatedsdep, path_); strcat(validatedsdep, "/Validated_sdep.txt");
  strcpy(validatedbias, path_); strcat(validatedbias, "/Validated_bias.txt");
  strcpy(validatedypred, path_); strcat(validatedypred, "/Validated_Predicted_Y.txt");
  strcpy(validatedypred_residuals, path_); strcat(validatedypred_residuals, "/Validated_Predicted_Residuals.txt");

  strcpy(yscrambling, path_); strcat(yscrambling, "/YScrambling_r2q2y.txt");

  std::string sep = " \t";
  ImportMatrix(tscore, sep, m->xscores);
  ImportMatrix(ploadings, sep, m->xloadings);
  ImportMatrix(weights, sep, m->xweights);
  ImportDvector(xexpvar, m->xvarexp);
  ImportDvector(xcolumnaverage, m->xcolaverage);
  ImportDvector(xcolumnscaling, m->xcolscaling);

//   ImportDvector(path_, "/Y-ExpVar.txt", m->yvarexp);
  ImportMatrix(uscore, sep, m->yscores);
  ImportMatrix(qloadings, sep, m->yloadings);
  ImportDvector(ycolumnaverage, m->ycolaverage);
  ImportDvector(ycolumnscaling, m->ycolscaling);

  ImportDvector(bcoeff, m->b);

  ImportMatrix(r2y, sep, m->r2y_recalculated);
  ImportMatrix(sdec, sep, m->sdec);
  ImportMatrix(recalc_y, sep, m->recalculated_y);
  ImportMatrix(recalc_residuals, sep, m->recalc_residuals);

  ImportMatrix(validatedq2y, sep, m->q2y);
  ImportMatrix(validatedsdep, sep, m->sdep);
  ImportMatrix(validatedbias, sep, m->bias);
  ImportMatrix(validatedypred, sep, m->predicted_y);
  ImportMatrix(validatedypred_residuals, sep, m->pred_residuals);

  ImportMatrix(yscrambling, sep, m->yscrambling);
}

void DATAIO::ImportUPCAModel(char *path_, UPCAMODEL* m)
{
  char tscore[MAXCHARS], ploadings[MAXCHARS], expvar[MAXCHARS], columnscaling[MAXCHARS], columnaverage[MAXCHARS];

  strcpy(tscore, path_); strcat(tscore, "/T-Scores.txt");
  strcpy(ploadings, path_); strcat(ploadings, "/P-Loadings.txt");
  strcpy(expvar, path_); strcat(expvar, "/ExpVar.txt");
  strcpy(columnscaling, path_); strcat(columnscaling, "/ColumnScaling.txt");
  strcpy(columnaverage, path_); strcat(columnaverage, "/ColumnAverage.txt");

  std::string sep = " \t";
  std::cout << "Import PCA " << path_ << std::endl;
  ImportMatrix(tscore, sep, m->scores);
  ImportTensor(ploadings, sep, m->loadings);
  ImportDvector(expvar, m->varexp);
  ImportMatrix(columnscaling, sep, m->colscaling);
  ImportMatrix(columnaverage, sep, m->colaverage);
}

void DATAIO::ImportUPLSModel(char *path_, UPLSMODEL* m)
{
  char tscore[MAXCHARS], ploadings[MAXCHARS], weights[MAXCHARS], xexpvar[MAXCHARS], xcolumnscaling[MAXCHARS], xcolumnaverage[MAXCHARS],
       uscore[MAXCHARS], qloadings[MAXCHARS], ycolumnscaling[MAXCHARS], ycolumnaverage[MAXCHARS],
       bcoeff[MAXCHARS], r2x[MAXCHARS], r2y[MAXCHARS], sdec[MAXCHARS], validatedypred[MAXCHARS], validatedr2x[MAXCHARS], validatedq2y[MAXCHARS], validatedsdep[MAXCHARS],
       yscramblingq2y[MAXCHARS], yscramblingsdep[MAXCHARS], recalc_y[MAXCHARS], recalc_residuals[MAXCHARS], validatedypred_residuals[MAXCHARS];

  strcpy(tscore, path_); strcat(tscore, "/X-T-Scores.txt");
  strcpy(ploadings, path_); strcat(ploadings, "/X-P-Loadings.txt");
  strcpy(weights, path_); strcat(weights, "/X-W-Weights.txt");
  strcpy(xexpvar, path_); strcat(xexpvar, "/X-ExpVar.txt");
  strcpy(xcolumnaverage, path_); strcat(xcolumnaverage, "/X-ColumnAverage.txt");
  strcpy(xcolumnscaling, path_); strcat(xcolumnscaling, "/X-ColumnScaling.txt");

  strcpy(uscore, path_); strcat(uscore, "/Y-U-Scores.txt");
  strcpy(qloadings, path_); strcat(qloadings, "/Y-Q-Loadings.txt");
  strcpy(ycolumnaverage, path_); strcat(ycolumnaverage, "/Y-ColumnAverage.txt");
  strcpy(ycolumnscaling, path_); strcat(ycolumnscaling, "/Y-ColumnScaling.txt");

  strcpy(bcoeff, path_); strcat(bcoeff, "/b-Coefficients.txt");
  strcpy(r2x, path_); strcat(r2x, "/r2x.txt");
  strcpy(r2y, path_); strcat(r2y, "/r2y.txt");
  strcpy(sdec, path_); strcat(sdec, "/sdec.txt");
  strcpy(recalc_y, path_); strcat(recalc_y, "/Recalculated_y.txt");
  strcpy(recalc_residuals, path_); strcat(recalc_residuals, "/Recalculated_Residuals.txt");


  strcpy(validatedr2x, path_); strcat(validatedr2x, "/Validated_r2x.txt");
  strcpy(validatedq2y, path_); strcat(validatedq2y, "/Validated_q2y.txt");
  strcpy(validatedsdep, path_); strcat(validatedsdep, "/Validated_sdep.txt");
  strcpy(validatedypred, path_); strcat(validatedypred, "/Validated_Predicted_Y.txt");
  strcpy(validatedypred_residuals, path_); strcat(validatedypred_residuals, "/Validated_Predicted_Residuals.txt");

  strcpy(yscramblingq2y, path_); strcat(yscramblingq2y, "/YScrambling_q2y.txt");
  strcpy(yscramblingsdep, path_); strcat(yscramblingsdep, "/YScrambling_sdep.txt");

  std::string sep = " \t";
  ImportMatrix(tscore, sep, m->xscores);
  ImportTensor(ploadings, sep, m->xloadings);
  ImportTensor(weights, sep, m->xweights);
  ImportDvector(xexpvar, m->xvarexp);
  ImportMatrix(xcolumnaverage, sep, m->xcolaverage);
  ImportMatrix(xcolumnscaling, sep, m->xcolscaling);

//   ImportDvector(path_, "/Y-ExpVar.txt", m->yvarexp);
  ImportMatrix(uscore, sep, m->yscores);
  ImportTensor(qloadings, sep, m->yloadings);
  ImportMatrix(ycolumnaverage, sep, m->ycolaverage);
  ImportMatrix(ycolumnscaling, sep, m->ycolscaling);

  ImportDvector(bcoeff, m->b);

  ImportDvector(r2x, m->r2x_model);
  ImportTensor(r2y, sep, m->r2y_model);
  ImportTensor(sdec, sep, m->sdec);
  ImportTensor(recalc_y, sep, m->recalculated_y);
  ImportTensor(recalc_residuals, sep, m->recalc_residuals);

  ImportDvector(validatedr2x, m->r2x_validation);
  ImportTensor(validatedq2y, sep, m->q2y);
  ImportTensor(validatedsdep, sep, m->sdep);
  ImportTensor(validatedypred, sep, m->predicted_y);
  ImportTensor(validatedypred_residuals, sep, m->pred_residuals);

  ImportTensor(yscramblingq2y, sep, m->q2y_yscrambling);
  ImportTensor(yscramblingsdep, sep, m->sdep_yscrambling);
}

void DATAIO::ImportMLRModel(char *path_, MLRMODEL* m)
{
  char bcoeff[MAXCHARS], r2y[MAXCHARS], sdec[MAXCHARS], ymean[MAXCHARS], validatedypred[MAXCHARS], validatedq2y[MAXCHARS], validatedsdep[MAXCHARS], validatedbias[MAXCHARS],
       r2q2scrambling[MAXCHARS], recalc_y[MAXCHARS], recalc_residuals[MAXCHARS], validatedypred_residuals[MAXCHARS];

  strcpy(bcoeff, path_); strcat(bcoeff, "/b-Coefficients.txt");
  strcpy(r2y, path_); strcat(r2y, "/r2y.txt");
  strcpy(sdec, path_); strcat(sdec, "/sdec.txt");
  strcpy(recalc_y, path_); strcat(recalc_y, "/Recalculated_y.txt");
  strcpy(recalc_residuals, path_); strcat(recalc_residuals, "/Recalculated_Residuals.txt");
  strcpy(ymean, path_); strcat(ymean, "/Y_Mean.txt");

  strcpy(validatedq2y, path_); strcat(validatedq2y, "/Validated_q2y.txt");
  strcpy(validatedsdep, path_); strcat(validatedsdep, "/Validated_sdep.txt");
  strcpy(validatedbias, path_); strcat(validatedbias, "/Validated_bias.txt");
  strcpy(validatedypred, path_); strcat(validatedypred, "/Validated_Predicted_Y.txt");
  strcpy(validatedypred_residuals, path_); strcat(validatedypred_residuals, "/Validated_Predicted_Residuals.txt");

  strcpy(r2q2scrambling, path_); strcat(r2q2scrambling, "/YScrambling_r2q2y.txt");

  std::string sep = " \t";

  ImportMatrix(bcoeff, sep, m->b);

  ImportDvector(r2y, m->r2y_model);
  ImportDvector(sdec, m->sdec);
  ImportMatrix(recalc_y, sep, m->recalculated_y);
  ImportMatrix(recalc_residuals, sep, m->recalc_residuals);
  ImportDvector(ymean, m->ymean);

  ImportDvector(validatedq2y, m->q2y);
  ImportDvector(validatedsdep, m->sdep);
  ImportMatrix(validatedypred, sep, m->predicted_y);
  ImportMatrix(validatedypred_residuals, sep, m->pred_residuals);
  ImportDvector(validatedbias, m->bias);

  ImportMatrix(r2q2scrambling, sep, m->r2q2scrambling);
}

void DATAIO::ImportLDAModel(char *path_, LDAMODEL* m)
{
  uivector *otherinfo;
  char roc[MAXCHARS], roc_aucs[MAXCHARS], pr[MAXCHARS], pr_aucs[MAXCHARS],
       pprob[MAXCHARS], eval[MAXCHARS], mu[MAXCHARS], evect[MAXCHARS],
       mnpdf[MAXCHARS], features[MAXCHARS], inv_cov[MAXCHARS],
       others[MAXCHARS], classid[MAXCHARS], fmean[MAXCHARS], fsdev[MAXCHARS],
       recalculated_y[MAXCHARS], recalculated_residuals[MAXCHARS],
       predicted_y[MAXCHARS], predicted_residuals[MAXCHARS];

  strcpy(roc, path_); strcat(roc, "/ROC.txt");
  strcpy(roc_aucs, path_); strcat(roc_aucs, "/ROCAUCS.txt");
  strcpy(pr, path_); strcat(pr, "/PRECISIONRECALL.txt");
  strcpy(pr_aucs, path_); strcat(pr_aucs, "/PRECISIONRECALLAUCS.txt");
  strcpy(pprob, path_); strcat(pprob, "/PPROB.txt");

  strcpy(recalculated_y, path_); strcat(recalculated_y, "/RECALCULATEDY.txt");
  strcpy(recalculated_residuals, path_); strcat(recalculated_residuals, "/RECALCULATED_RESIDUALS.txt");
  strcpy(predicted_y, path_); strcat(predicted_y, "/PRECISIONRECALLAUCS.txt");
  strcpy(predicted_residuals, path_); strcat(predicted_residuals, "/PREDICTED_RESIDUALS.txt");

  strcpy(eval, path_); strcat(eval, "/EVAL.txt");
  strcpy(mu, path_); strcat(mu, "/MU.txt");
  strcpy(evect, path_); strcat(evect, "/EVECT.txt");
  strcpy(mnpdf, path_); strcat(mnpdf, "/MNPDF.txt");
  strcpy(features, path_); strcat(features, "/FEATURES.txt");
  strcpy(fmean, path_); strcat(fmean, "/FEATUREMEAN.txt");
  strcpy(fsdev, path_); strcat(fsdev, "/FEATURESDEV.txt");

  strcpy(inv_cov, path_); strcat(inv_cov, "/INVCOV.txt");
  strcpy(others, path_); strcat(others, "/MODINFO.txt");
  strcpy(classid, path_); strcat(classid, "/CLASSID.txt");

  std::string sep = " \t";

  ImportTensor(roc, sep, m->roc);
  ImportDvector(roc_aucs, m->roc_aucs);
  ImportTensor(pr, sep, m->pr);
  ImportDvector(pr_aucs, m->pr_aucs);

  ImportMatrix(recalculated_y, sep, m->recalculated_y);
  ImportMatrix(recalculated_residuals, sep, m->recalculated_residuals);
  ImportMatrix(recalculated_y, sep, m->predicted_y);
  ImportMatrix(recalculated_residuals, sep, m->predicted_residuals);

  ImportDvector(eval, m->eval);
  ImportMatrix(mu, sep, m->mu);
  ImportMatrix(evect, sep, m->evect);
  ImportTensor(mnpdf, sep, m->mnpdf);

  ImportTensor(features, sep, m->features);
  ImportMatrix(fmean, sep, m->fmean);
  ImportMatrix(fmean, sep, m->fsdev);
  ImportMatrix(inv_cov, sep, m->inv_cov);

  initUIVector(&otherinfo);
  ImportUIvector(others, otherinfo);
  m->nclass = otherinfo->data[0];
  m->class_start = otherinfo->data[1];
  DelUIVector(&otherinfo);
  ImportUIvector(classid, m->classid);
}

void DATAIO::WriteStringList(char *file_, std::vector<std::string> &strlst)
{
  std::fstream out;
  out.open (file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right,std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for(size_t i = 0; i < strlst.size(); i++){
    out << strlst[i] << std::endl;
  }
  out.close();
}

void DATAIO::WriteComments(char *file_, std::vector<std::string> &strvect)
{
  std::fstream out;
  out.open (file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for(size_t i = 0; i < strvect.size(); i++){
    out << "#" << strvect[i] << std::endl;
  }
  out.close();
}

void DATAIO::WriteDvector(char *file_, dvector *v)
{
  std::fstream out;
  out.open (file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for(size_t i = 0; i < v->size; i++){
    out << getDVectorValue(v, i) << std::endl;
  }
  out.close();
}

void DATAIO::WriteUIvector(char *file_, uivector *v)
{
  std::fstream out;
  out.open (file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);
  for(size_t i = 0; i < v->size; i++){
    out << getUIVectorValue(v, i) << std::endl;
  }
  out.close();
}

void DATAIO::WriteMatrix(char *file_, matrix *m)
{
  std::fstream out;
  out.open (file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);

  for(size_t i = 0; i < m->row; i++){
    for(size_t j = 0; j < m->col; j++){
        out << m->data[i][j];
        if(j < m->col-1)
          out << "\t";
    }
    out << std::endl;
  }
  out.close();
}

void DATAIO::WriteTensor(char *file_, tensor *a)
{
  std::fstream out;
  out.open (file_, std::ios::out | std::ios::app);
  out.setf(std::ios_base::right, std::ios_base::adjustfield);
  out.setf(std::ios::fixed, std::ios::floatfield);

  for(size_t k = 0; k < a->order; k++){
    for(size_t i = 0; i < a->m[k]->row; i++){
      for(size_t j = 0; j < a->m[k]->col; j++){
        out << getTensorValue(a, k, i, j);
        if(j < a->m[k]->col-1)
          out << "\t";
      }
      out << std::endl;
    }
    if(k < a->order-1)
      out << "-" << std::endl;
  }
  out.close();
}

void DATAIO::WritePCAModel(char *path_, PCAMODEL* m)
{
  char tscore[MAXCHARS], ploadings[MAXCHARS], expvar[MAXCHARS], columnscaling[MAXCHARS], columnaverage[MAXCHARS];

  strcpy(tscore, path_); strcat(tscore, "/T-Scores.txt");
  strcpy(ploadings, path_); strcat(ploadings, "/P-Loadings.txt");
  strcpy(expvar, path_); strcat(expvar, "/ExpVar.txt");
  strcpy(columnscaling, path_); strcat(columnscaling, "/ColumnScaling.txt");
  strcpy(columnaverage, path_); strcat(columnaverage, "/ColumnAverage.txt");

  if(DirExists(path_) == true){
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(tscore, m->scores);
  WriteMatrix(ploadings, m->loadings);
  WriteDvector(expvar, m->varexp);
  WriteDvector(columnscaling, m->colscaling);
  WriteDvector(columnaverage, m->colaverage);
}

void DATAIO::WritePLSModel(char *path_, PLSMODEL* m)
{
  char tscore[MAXCHARS], ploadings[MAXCHARS], weights[MAXCHARS], xexpvar[MAXCHARS], xcolumnscaling[MAXCHARS], xcolumnaverage[MAXCHARS],
       uscore[MAXCHARS], qloadings[MAXCHARS], ycolumnscaling[MAXCHARS], ycolumnaverage[MAXCHARS],
       bcoeff[MAXCHARS], r2y[MAXCHARS], sdec[MAXCHARS], validatedypred[MAXCHARS], validatedq2y[MAXCHARS], validatedsdep[MAXCHARS], validatedbias[MAXCHARS], yscrambling[MAXCHARS], recalc_y[MAXCHARS], recalc_residuals[MAXCHARS], validatedypred_residuals[MAXCHARS];

  strcpy(tscore, path_); strcat(tscore, "/X-T-Scores.txt");
  strcpy(ploadings, path_); strcat(ploadings, "/X-P-Loadings.txt");
  strcpy(weights, path_); strcat(weights, "/X-W-Weights.txt");
  strcpy(xexpvar, path_); strcat(xexpvar, "/X-ExpVar.txt");
  strcpy(xcolumnaverage, path_); strcat(xcolumnaverage, "/X-ColumnAverage.txt");
  strcpy(xcolumnscaling, path_); strcat(xcolumnscaling, "/X-ColumnScaling.txt");

  strcpy(uscore, path_); strcat(uscore, "/Y-U-Scores.txt");
  strcpy(qloadings, path_); strcat(qloadings, "/Y-Q-Loadings.txt");
  strcpy(ycolumnaverage, path_); strcat(ycolumnaverage, "/Y-ColumnAverage.txt");
  strcpy(ycolumnscaling, path_); strcat(ycolumnscaling, "/Y-ColumnScaling.txt");

  strcpy(bcoeff, path_); strcat(bcoeff, "/b-Coefficients.txt");
  strcpy(r2y, path_); strcat(r2y, "/r2y.txt");
  strcpy(sdec, path_); strcat(sdec, "/sdec.txt");
  strcpy(recalc_y, path_); strcat(recalc_y, "/Recalculated_y.txt");
  strcpy(recalc_residuals, path_); strcat(recalc_residuals, "/Recalculated_Residuals.txt");

  strcpy(validatedq2y, path_); strcat(validatedq2y, "/Validated_q2y.txt");
  strcpy(validatedsdep, path_); strcat(validatedsdep, "/Validated_sdep.txt");
  strcpy(validatedbias, path_); strcat(validatedbias, "/Validated_bias.txt");
  strcpy(validatedypred, path_); strcat(validatedypred, "/Validated_Predicted_Y.txt");
  strcpy(validatedypred_residuals, path_); strcat(validatedypred_residuals, "/Validated_Predicted_Residuals.txt");

  strcpy(yscrambling, path_); strcat(yscrambling, "/YScrambling_r2q2y.txt");

  if(DirExists(path_) == true){
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(tscore, m->xscores);
  WriteMatrix(ploadings, m->xloadings);
  WriteMatrix(weights, m->xweights);
  WriteDvector(xexpvar, m->xvarexp);
  WriteDvector(xcolumnaverage, m->xcolaverage);
  WriteDvector(xcolumnscaling, m->xcolscaling);

//   WriteDvector(path_, "/Y-ExpVar.txt", m->yvarexp);
  WriteMatrix(uscore, m->yscores);
  WriteMatrix(qloadings, m->yloadings);
  WriteDvector(ycolumnaverage, m->ycolaverage);
  WriteDvector(ycolumnscaling, m->ycolscaling);

  WriteDvector(bcoeff, m->b);

  WriteMatrix(r2y, m->r2y_recalculated);
  WriteMatrix(sdec, m->sdec);
  WriteMatrix(recalc_y, m->recalculated_y);
  WriteMatrix(recalc_residuals, m->recalc_residuals);

  WriteMatrix(validatedq2y, m->q2y);
  WriteMatrix(validatedsdep, m->sdep);
  WriteMatrix(validatedbias, m->bias);
  WriteMatrix(validatedypred, m->predicted_y);
  WriteMatrix(validatedypred_residuals, m->pred_residuals);

  WriteMatrix(yscrambling, m->yscrambling);
}

void DATAIO::WriteUPCAModel(char *path_, UPCAMODEL* m)
{
  char tscore[MAXCHARS], ploadings[MAXCHARS], expvar[MAXCHARS], columnscaling[MAXCHARS], columnaverage[MAXCHARS];

  strcpy(tscore, path_); strcat(tscore, "/T-Scores.txt");
  strcpy(ploadings, path_); strcat(ploadings, "/P-Loadings.txt");
  strcpy(expvar, path_); strcat(expvar, "/ExpVar.txt");
  strcpy(columnscaling, path_); strcat(columnscaling, "/ColumnScaling.txt");
  strcpy(columnaverage, path_); strcat(columnaverage, "/ColumnAverage.txt");

  if(DirExists(path_) == true){
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(tscore, m->scores);
  WriteTensor(ploadings, m->loadings);
  WriteDvector(expvar, m->varexp);
  WriteMatrix(columnscaling, m->colscaling);
  WriteMatrix(columnaverage, m->colaverage);
}


void DATAIO::WriteUPLSModel(char *path_, UPLSMODEL* m)
{
  char tscore[MAXCHARS], ploadings[MAXCHARS], weights[MAXCHARS], xexpvar[MAXCHARS], xcolumnscaling[MAXCHARS], xcolumnaverage[MAXCHARS],
       uscore[MAXCHARS], qloadings[MAXCHARS], ycolumnscaling[MAXCHARS], ycolumnaverage[MAXCHARS],
       bcoeff[MAXCHARS], r2x[MAXCHARS], r2y[MAXCHARS], sdec[MAXCHARS], validatedypred[MAXCHARS], validatedr2x[MAXCHARS], validatedq2y[MAXCHARS], validatedsdep[MAXCHARS],
       yscramblingq2y[MAXCHARS], yscramblingsdep[MAXCHARS], recalc_y[MAXCHARS], recalc_residuals[MAXCHARS], validatedypred_residuals[MAXCHARS];

  strcpy(tscore, path_); strcat(tscore, "/X-T-Scores.txt");
  strcpy(ploadings, path_); strcat(ploadings, "/X-P-Loadings.txt");
  strcpy(weights, path_); strcat(weights, "/X-W-Weights.txt");
  strcpy(xexpvar, path_); strcat(xexpvar, "/X-ExpVar.txt");
  strcpy(xcolumnaverage, path_); strcat(xcolumnaverage, "/X-ColumnAverage.txt");
  strcpy(xcolumnscaling, path_); strcat(xcolumnscaling, "/X-ColumnScaling.txt");

  strcpy(uscore, path_); strcat(uscore, "/Y-U-Scores.txt");
  strcpy(qloadings, path_); strcat(qloadings, "/Y-Q-Loadings.txt");
  strcpy(ycolumnaverage, path_); strcat(ycolumnaverage, "/Y-ColumnAverage.txt");
  strcpy(ycolumnscaling, path_); strcat(ycolumnscaling, "/Y-ColumnScaling.txt");

  strcpy(bcoeff, path_); strcat(bcoeff, "/b-Coefficients.txt");
  strcpy(r2x, path_); strcat(r2x, "/r2x.txt");
  strcpy(r2y, path_); strcat(r2y, "/r2y.txt");
  strcpy(sdec, path_); strcat(sdec, "/sdec.txt");
  strcpy(recalc_y, path_); strcat(recalc_y, "/Recalculated_y.txt");
  strcpy(recalc_residuals, path_); strcat(recalc_residuals, "/Recalculated_Residuals.txt");

  strcpy(validatedypred, path_); strcat(validatedypred, "/Validated_Predicted_Y.txt");
  strcpy(validatedypred_residuals, path_); strcat(validatedypred_residuals, "/Validated_Predicted_Residuals.txt");
  strcpy(validatedr2x, path_); strcat(validatedr2x, "/Validated_r2x.txt");
  strcpy(validatedq2y, path_); strcat(validatedq2y, "/Validated_q2y.txt");
  strcpy(validatedsdep, path_); strcat(validatedsdep, "/Validated_sdep.txt");
  strcpy(yscramblingq2y, path_); strcat(yscramblingq2y, "/YScrambling_q2y.txt");
  strcpy(yscramblingsdep, path_); strcat(yscramblingsdep, "/YScrambling_sdep.txt");


  if(DirExists(path_) == true){
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(tscore, m->xscores);
  WriteTensor(ploadings, m->xloadings);
  WriteTensor(weights, m->xweights);
  WriteDvector(xexpvar, m->xvarexp);
  WriteMatrix(xcolumnaverage, m->xcolaverage);
  WriteMatrix(xcolumnscaling, m->xcolscaling);

//   WriteDvector(path_, "/Y-ExpVar.txt", m->yvarexp);
  WriteMatrix(uscore, m->yscores);
  WriteTensor(qloadings, m->yloadings);
  WriteMatrix(ycolumnaverage, m->ycolaverage);
  WriteMatrix(ycolumnscaling, m->ycolscaling);

  WriteDvector(bcoeff, m->b);

  WriteDvector(r2x, m->r2x_model);
  WriteTensor(r2y, m->r2y_model);
  WriteTensor(sdec, m->sdec);
  WriteTensor(recalc_y, m->recalculated_y);
  WriteTensor(recalc_residuals, m->recalc_residuals);

  WriteDvector(validatedr2x, m->r2x_validation);
  WriteTensor(validatedq2y, m->q2y);
  WriteTensor(validatedsdep, m->sdep);
  WriteTensor(validatedypred, m->predicted_y);
  WriteTensor(validatedypred_residuals, m->pred_residuals);

  WriteTensor(yscramblingq2y, m->q2y_yscrambling);
  WriteTensor(yscramblingsdep, m->sdep_yscrambling);
}

void DATAIO::WriteMLRModel(char *path_, MLRMODEL* m)
{
  char bcoeff[MAXCHARS], r2y[MAXCHARS], sdec[MAXCHARS], ymean[MAXCHARS], validatedypred[MAXCHARS], validatedq2y[MAXCHARS], validatedsdep[MAXCHARS], validatedbias[MAXCHARS],
       r2q2scrambling[MAXCHARS], recalc_y[MAXCHARS], recalc_residuals[MAXCHARS], validatedypred_residuals[MAXCHARS];

  strcpy(bcoeff, path_); strcat(bcoeff, "/b-Coefficients.txt");
  strcpy(r2y, path_); strcat(r2y, "/r2y.txt");
  strcpy(sdec, path_); strcat(sdec, "/sdec.txt");
  strcpy(recalc_y, path_); strcat(recalc_y, "/Recalculated_y.txt");
  strcpy(recalc_residuals, path_); strcat(recalc_residuals, "/Recalculated_Residuals.txt");
  strcpy(ymean, path_); strcat(ymean, "/Y_Mean.txt");

  strcpy(validatedypred, path_); strcat(validatedypred, "/Validated_Predicted_Y.txt");
  strcpy(validatedypred_residuals, path_); strcat(validatedypred_residuals, "/Validated_Predicted_Residuals.txt");
  strcpy(validatedq2y, path_); strcat(validatedq2y, "/Validated_q2y.txt");
  strcpy(validatedsdep, path_); strcat(validatedsdep, "/Validated_sdep.txt");
  strcpy(validatedbias, path_); strcat(validatedbias, "/Validated_bias.txt");
  strcpy(r2q2scrambling, path_); strcat(r2q2scrambling, "/YScrambling_r2q2y.txt");

  if(DirExists(path_) == true){
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteMatrix(bcoeff, m->b);
  WriteDvector(r2y, m->r2y_model);
  WriteDvector(sdec, m->sdec);
  WriteMatrix(recalc_y, m->recalculated_y);
  WriteMatrix(recalc_residuals, m->recalc_residuals);
  WriteDvector(ymean, m->ymean);

  WriteDvector(validatedq2y, m->q2y);
  WriteDvector(validatedsdep, m->sdep);
  WriteMatrix(validatedypred, m->predicted_y);
  WriteMatrix(validatedypred_residuals, m->pred_residuals);
  WriteDvector(validatedbias, m->bias);

  WriteMatrix(r2q2scrambling, m->r2q2scrambling);
}


void DATAIO::WriteLDAModel(char *path_, LDAMODEL* m)
{
  uivector *otherinfo;
  char roc[MAXCHARS], roc_aucs[MAXCHARS], pr[MAXCHARS], pr_aucs[MAXCHARS],
       pprob[MAXCHARS], eval[MAXCHARS], mu[MAXCHARS], evect[MAXCHARS],
       mnpdf[MAXCHARS], features[MAXCHARS], inv_cov[MAXCHARS],
       others[MAXCHARS], classid[MAXCHARS], fmean[MAXCHARS], fsdev[MAXCHARS],
       recalculated_y[MAXCHARS], recalculated_residuals[MAXCHARS],
       predicted_y[MAXCHARS], predicted_residuals[MAXCHARS];

  strcpy(roc, path_); strcat(roc, "/ROC.txt");
  strcpy(roc_aucs, path_); strcat(roc_aucs, "/ROCAUCS.txt");
  strcpy(pr, path_); strcat(pr, "/PRECISIONRECALL.txt");
  strcpy(pr_aucs, path_); strcat(pr_aucs, "/PRECISIONRECALLAUCS.txt");
  strcpy(recalculated_y, path_); strcat(recalculated_y, "/RECALCULATEDY.txt");
  strcpy(recalculated_residuals, path_); strcat(recalculated_residuals, "/RECALCULATED_RESIDUALS.txt");
  strcpy(predicted_y, path_); strcat(predicted_y, "/PRECISIONRECALLAUCS.txt");
  strcpy(predicted_residuals, path_); strcat(predicted_residuals, "/PREDICTED_RESIDUALS.txt");
  strcpy(pprob, path_); strcat(pprob, "/PPROB.txt");
  strcpy(eval, path_); strcat(eval, "/EVAL.txt");
  strcpy(mu, path_); strcat(mu, "/MU.txt");
  strcpy(evect, path_); strcat(evect, "/EVECT.txt");
  strcpy(mnpdf, path_); strcat(mnpdf, "/MNPDF.txt");
  strcpy(features, path_); strcat(features, "/FEATURES.txt");
  strcpy(fmean, path_); strcat(fmean, "/FEATUREMEAN.txt");
  strcpy(fsdev, path_); strcat(fsdev, "/FEATURESDEV.txt");
  strcpy(inv_cov, path_); strcat(inv_cov, "/INVCOV.txt");
  strcpy(others, path_); strcat(others, "/MODINFO.txt");
  strcpy(classid, path_); strcat(classid, "/CLASSID.txt");

  if(DirExists(path_) == true){
    RemoveDir(path_);
  }

  MakeDir(path_);

  WriteTensor(roc, m->roc);
  WriteDvector(roc_aucs, m->roc_aucs);
  WriteTensor(pr, m->pr);
  WriteDvector(pr_aucs, m->pr_aucs);

  WriteMatrix(recalculated_y, m->recalculated_y);
  WriteMatrix(recalculated_residuals, m->recalculated_residuals);
  WriteMatrix(predicted_y, m->predicted_y);
  WriteMatrix(predicted_residuals, m->predicted_residuals);

  WriteDvector(eval, m->eval);
  WriteMatrix(mu, m->mu);
  WriteMatrix(evect, m->evect);
  WriteTensor(mnpdf, m->mnpdf);

  WriteTensor(features, m->features);
  WriteMatrix(fmean, m->fmean);
  WriteMatrix(fmean, m->fsdev);
  WriteMatrix(inv_cov, m->inv_cov);

  NewUIVector(&otherinfo, 2);
  otherinfo->data[0] = m->nclass;
  otherinfo->data[1] = m->class_start;
  WriteUIvector(others, otherinfo);
  DelUIVector(&otherinfo);
  WriteUIvector(classid, m->classid);
}
