// QStudiom-GPSHumanizer
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "DATAIO.h"

using namespace std;

#include "scientific.h"

void help(char **argv) {
  std::cout << "Usage:" << endl;
  std::cout << argv[0] << "\t -i <coordinate file>"
            << "\t -l <objnamelist>"
            << "\t -n <Number Of Compounds>"
            << "\t -c <id method>"
            << " \t -m <id metric space>" << std::endl;
  std::cout << "\t  Methods:\n 0: Random;\t1: MDC;\t2: MaxDissimilarity"
            << std::endl;
  std::cout << " \t Metrics:\n 0: Euclidean;\t1: Manhattan;\t2: Cosine"
            << std::endl;
  std::cout
      << "\n " << argv[0]
      << " was writen by Giuseppe Marco Randazzo <gmrandazzo@gmail.com> \n"
      << std::endl;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    help(argv);
  } else {
    string matrixfile, labelfile;
    int method = 0, metric = 0, n = 1;
    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "-i") == 0) {
        if (i + 1 < argc) {
          matrixfile = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-l") == 0) {
        if (i + 1 < argc) {
          labelfile = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-n") == 0) {
        if (i + 1 < argc) {
          n = atoi(argv[i + 1]);
        }
      }

      if (strcmp(argv[i], "-c") == 0) {
        if (i + 1 < argc) {
          method = atoi(argv[i + 1]);
        }
      }

      if (strcmp(argv[i], "-m") == 0) {
        if (i + 1 < argc) {
          metric = atoi(argv[i + 1]);
        }
      }
    }

    if (!matrixfile.empty() && !labelfile.empty()) {
      matrix *m;
      uivector *selections;
      strvector *labels;

      initStrVector(&labels);
      DATAIO::ImportStrvector(labelfile, &labels);

      initMatrix(&m);
      DATAIO::ImportMatrix(matrixfile, " \t", m);

      initUIVector(&selections);

      if (method == 1) {
        MDC(m, n, metric, &selections, 0);
      } else {
        MaxMinDis(m, n, metric, &selections, 0);
      }

      for (uint i = 0; i < selections->size; i++) {
        cout << getStr(labels, getUIVectorValue(selections, i)) << endl;
      }

      for (uint i = 0; i < selections->size; i++) {
        for (uint j = 0; j < m->col - 1; j++) {
          cout << getMatrixValue(m, getUIVectorValue(selections, i), j) << "\t";
        }
        cout << getMatrixValue(m, getUIVectorValue(selections, i), m->col - 1)
             << endl;
      }

      DelStrVector(&labels);
      DelUIVector(&selections);
      DelMatrix(&m);
    } else {
      cout << "No option selected." << endl;
    }
  }
  return 0;
}
