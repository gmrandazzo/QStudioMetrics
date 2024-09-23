#include <cstring>
#include <iostream>
using namespace std;

#include "DATAIO.h"

extern "C" {
#include "scientific.h"
}

void help(char **argv) {
  std::cout << "Usage:" << endl;
  std::cout << " Yates Effect: " << argv[0] << "-y <matrix coordinates>"
            << "\t -o <output file>" << std::endl;
  std::cout << " RSM : " << argv[0]
            << "-rsm <matrix coordinates> <y coordinates>"
            << "\t -o <output file>" << std::endl;
  std::cout
      << "\n " << argv[0]
      << " was writen by Giuseppe Marco Randazzo <gmrandazzo@gmail.com> \n"
      << std::endl;
}

int main(int argc, char **argv) {
  if (argc <= 1) {
    help(argv);
  } else {
    string x_, y_, outputfile;

    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "-y") == 0 || strcmp(argv[i], "-yates") == 0) {
        if (i + 1 < argc) {
          x_ = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-rsm") == 0) {
        if (i + 2 < argc) {
          x_ = argv[i + 1];
          y_ = argv[i + 2];
        }
      }

      if (strcmp(argv[i], "-o") == 0) {
        if (i + 1 < argc) {
          outputfile = argv[i + 1];
        }
      }
    }

    if (!x_.empty() && y_.empty() && !outputfile.empty()) { // Make Yates Effect
      matrix *inmx, *outmx;
      initMatrix(&inmx);
      initMatrix(&outmx);

      DATAIO::ImportMatrix(x_, "\t", inmx);
      MatrixCode(inmx, outmx);
      PrintMatrix(outmx);

      DelMatrix(&inmx);
      DelMatrix(&outmx);
    } else if (!x_.empty() && !y_.empty() &&
               !outputfile.empty()) { // Make Response Surface Metholodogy
      matrix *x, *codedx, *expx;
      dvector *y, *coeff;

      initMatrix(&x);
      initMatrix(&codedx);
      initMatrix(&expx);
      initDVector(&y);
      initDVector(&coeff);

      DATAIO::ImportMatrix(x_, "\t", x);
      DATAIO::ImportDvector(y_, y);

      MatrixCode(x, codedx);

      BifactorialMatrixExpansion(codedx, expx);
      PrintMatrix(expx);
      OrdinaryLeastSquares(expx, y, coeff);

      PrintDVector(coeff);

      DelMatrix(&expx);
      DelMatrix(&x);
      DelMatrix(&codedx);
      DelDVector(&y);
      DelDVector(&coeff);
    } else {
      cout << "No option selected." << endl;
    }
  }
  return 0;
}
