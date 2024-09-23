// QStudiom-upls
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "DATAIO.h"

#include <cstring>
#include <getopt.h>
#include <vector>
using namespace std;

extern "C" {
#include "scientific.h"
}

void help(char **argv) {
  std::cout << "Usage" << endl;
  std::cout << "Make a model: " << argv[0]
            << "\t -model -x <xinput file> -y <yinput file>"
            << "\t-o <output file>"
            << "\t-c <N° of PC> \t -xa(autoscaling for x tensor)\t "
               "-ya(autoscaling for y tensor)"
            << std::endl;
  std::cout << "Make a prediction: " << argv[0]
            << "\t -predict -dm <input model> -i <input file>"
            << "\t-o <output file>"
            << "\t-c <N° of PC>" << std::endl;
  std::cout << "Make the cross validation: " << argv[0]
            << "\t -cv -x <xinput file> -y <yinput file> \t -dm <input path "
               "model> \t -c <N° of PC> \t -g <N° of groups> -xa(autoscaling "
               "for x tensor)\t -ya(autoscaling for y tensor)"
            << std::endl;
  std::cout << "\n\t "
               "---------------------------------------------------------------"
               "------------- "
            << std::endl;
  std::cout
      << "\t | " << argv[0]
      << " was writen by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>  |"
      << std::endl;
  std::cout << "\t "
               "---------------------------------------------------------------"
               "------------- \n"
            << std::endl;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    help(argv);
  } else {
    size_t npc = 0, ngroups = 0, iterations = 20, xautoscaling = 0,
           yautoscaling = 0;
    string xinputdata, yinputdata, pathmodel, outpath, xsep, ysep;
    bool genmodel, makeprediction, makecrossvalidation;
    genmodel = makeprediction = makecrossvalidation = false;
    xsep = ysep = " \t";

    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "-model") == 0 || strcmp(argv[i], "-m") == 0) {
        genmodel = true;
      }

      if (strcmp(argv[i], "-predict") == 0 || strcmp(argv[i], "-p") == 0) {
        makeprediction = true;
      }

      if (strcmp(argv[i], "-cross-validation") == 0 ||
          strcmp(argv[i], "-cv") == 0) {
        makecrossvalidation = true;
      }

      if (strcmp(argv[i], "-xdata") == 0 || strcmp(argv[i], "-x") == 0) {
        if (i + 1 < argc) {
          xinputdata = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-ydata") == 0 || strcmp(argv[i], "-y") == 0) {
        if (i + 1 < argc) {
          yinputdata = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-out") == 0 || strcmp(argv[i], "-o") == 0) {
        if (i + 1 < argc) {
          outpath = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-c") == 0) {
        if (i + 1 < argc)
          npc = atoi(argv[i + 1]);
      }

      if (strcmp(argv[i], "-g") == 0) {
        if (i + 1 < argc)
          ngroups = atoi(argv[i + 1]);
      }

      if (strcmp(argv[i], "-i") == 0) {
        if (i + 1 < argc)
          iterations = atoi(argv[i + 1]);
      }

      if (strcmp(argv[i], "-data-model") == 0 || strcmp(argv[i], "-dm") == 0) {
        if (i + 1 < argc)
          pathmodel = argv[i + 1];
      }

      if (strcmp(argv[i], "-xa") == 0) {
        if (i + 1 < argc)
          xautoscaling = atoi(argv[i + 1]);
      }

      if (strcmp(argv[i], "-ya") == 0) {
        if (i + 1 < argc)
          yautoscaling = atoi(argv[i + 1]);
      }

      if (strcmp(argv[i], "-sx") == 0) {
        if (i + 1 < argc)
          xsep = argv[i + 1];
      }

      if (strcmp(argv[i], "-sy") == 0) {
        if (i + 1 < argc)
          ysep = argv[i + 1];
      }

      if (strcmp(argv[i], "-s") == 0) {
        if (i + 1 < argc)
          xsep = ysep = argv[i + 1];
      }
    }

    if (genmodel == true && !xinputdata.empty() && !yinputdata.empty() &&
        !outpath.empty() && npc > 0) {
      tensor *xdata, *ydata;

      initTensor(&xdata);
      initTensor(&ydata);

      DATAIO::ImportArray(xinputdata, xsep.c_str(), xdata);
      DATAIO::ImportArray(yinputdata, ysep.c_str(), ydata);

      UPLSMODEL *m;
      NewUPLSModel(&m);

      UPLS(xdata, ydata, npc, xautoscaling, yautoscaling, m);
      UPLSRSquared(xdata, ydata, m, npc, &(m->r2x_model), &(m->r2y_model),
                   &(m->sdec));

      DATAIO::WriteUPLSModel(outpath, m);

      DelUPLSModel(&m);
      DelTensor(&xdata);
      DelTensor(&ydata);
    } else if (makeprediction == true && !pathmodel.empty() &&
               !outpath.empty() && !xinputdata.empty() && npc > 0) {
      tensor *xdata;

      initTensor(&xdata);

      DATAIO::ImportArray(xinputdata, xsep.c_str(), xdata);

      UPLSMODEL *m;
      NewUPLSModel(&m);
      DATAIO::ImportUPLSModel(pathmodel, m);

      matrix *xscores;
      tensor *y;
      initMatrix(&xscores);
      initTensor(&y);

      UPLSScorePredictor(xdata, m, npc, &xscores);
      UPLSYPredictor(xscores, m, npc, &y);

      DATAIO::MakeDir(outpath);
      DATAIO::WriteMatrix(outpath + "/X-Pred-Scores.txt", xscores);
      DATAIO::WriteArray(outpath + "/Y-DipVar-Pred.txt", y);

      DelTensor(&y);
      DelMatrix(&xscores);
      DelUPLSModel(&m);
      DelTensor(&xdata);
    } else if (makecrossvalidation == true && !xinputdata.empty() &&
               !pathmodel.empty() && !yinputdata.empty() && npc > 0 &&
               ngroups > 0) {
      tensor *xdata, *ydata;
      dvector *r2x;
      dvector *validated_b;
      tensor *validated_yloadings;
      tensor *q2y;
      tensor *sdep;

      initTensor(&xdata);
      initTensor(&ydata);

      DATAIO::ImportArray(xinputdata, xsep.c_str(), xdata);
      DATAIO::ImportArray(yinputdata, ysep.c_str(), ydata);

      initDVector(&r2x);
      initDVector(&validated_b);
      initTensor(&validated_yloadings);
      initTensor(&q2y);
      initTensor(&sdep);

      UPLSCrossValidation(xdata, ydata, xautoscaling, yautoscaling, npc,
                          ngroups, iterations, &r2x, &q2y, &sdep, &validated_b,
                          &validated_yloadings);

      DATAIO::WriteDvector(pathmodel + "/Validated_b-Coefficients.txt",
                           validated_b);
      DATAIO::WriteArray(pathmodel + "/Validated_Y-Q-Loadings.txt",
                         validated_yloadings);
      DATAIO::WriteDvector(pathmodel + "/Validated_r2x.txt", r2x);
      DATAIO::WriteArray(pathmodel + "/Validated_q2y.txt", q2y);
      DATAIO::WriteArray(pathmodel + "/Validated_sdep.txt", sdep);

      cout.setf(ios_base::right, ios_base::adjustfield);
      cout.setf(ios::fixed, ios::floatfield);

      cout << "Cross Validated R^2" << endl;
      for (uint i = 0; i < r2x->size; i++) {
        cout << "PC " << i + 1 << ": " << setw(3) << getDVectorValue(r2x, i)
             << endl;
      }
      cout << "Cross Validated Q^2" << endl;
      for (uint k = 0; k < q2y->order; k++) {
        cout << ">> PC " << k + 1 << endl;

        for (uint j = 0; j < q2y->m[k]->col; j++)
          cout << setw(10) << "layer" << j + 1;
        cout << endl;

        for (uint i = 0; i < q2y->m[k]->row; i++) {
          for (uint j = 0; j < q2y->m[k]->col; j++) {
            if (j == 0)
              cout << "y" << i + 1 << ": " << setw(3)
                   << getArrayValue(q2y, k, i, j);
            else if (j == q2y->m[k]->col - 1)
              cout << setw(10) << getArrayValue(q2y, k, i, j) << endl;
            else
              cout << setw(10) << getArrayValue(q2y, k, i, j);
          }
          cout << endl;
        }
        cout << endl;
      }
      cout << "SDEP" << endl;
      for (uint k = 0; k < sdep->order; k++) {
        cout << ">> PC " << k + 1 << endl;

        for (uint j = 0; j < sdep->m[k]->col; j++)
          cout << setw(10) << "layer" << j + 1;
        cout << endl;

        for (uint i = 0; i < sdep->m[k]->row; i++) {
          for (uint j = 0; j < sdep->m[k]->col; j++) {
            if (j == 0)
              cout << "y" << i + 1 << ": " << setw(3)
                   << getArrayValue(sdep, k, i, j);
            else if (j == sdep->m[k]->col - 1)
              cout << setw(10) << getArrayValue(sdep, k, i, j) << endl;
            else
              cout << setw(10) << getArrayValue(sdep, k, i, j);
          }
          cout << endl;
        }
        cout << endl;
      }

      DelTensor(&sdep);
      DelDVector(&r2x);
      DelTensor(&q2y);
      DelTensor(&xdata);
      DelTensor(&ydata);
    } else {
      cout << "No option selected." << endl;
    }
  }
  return 0;
}
