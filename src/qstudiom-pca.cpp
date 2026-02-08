/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for qstudiom-pca.
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

// QStudiom-pca
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

#include "scientific.h"

void help(char **argv) {
  std::cout << "Usage" << endl;
  std::cout << "Make a model: " << argv[0] << "\t -model -i <input file>"
            << "\t-o <output file>"
            << "\t-c <N° of PC> \t -a [Autoscaling type]" << std::endl;
  std::cout << "Make a prediction: " << argv[0]
            << "\t -predict -dm <input model> -i <input file>"
            << "\t-o <output directory>"
            << "\t-c <N° of PC>" << std::endl;
  std::cout << "Autoscaling type:" << std::endl;
  std::cout << " 0 = no scaling" << std::endl;
  std::cout << " 1 = SDEV autoscaling" << std::endl;
  std::cout << " 2 = RMS scaling" << std::endl;
  std::cout << " 3 = Pareto scaling" << std::endl;
  std::cout << " 4 = Range scaling" << std::endl;
  std::cout << " 5 = Level scalign" << std::endl;
  std::cout << "\nN.B.: Data should be without row/column names" << std::endl;
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
    size_t npc = 0;
    int autoscaling = 0;
    string inputdata, datamodel, outputfile, sep;
    bool genmodel, makeprediction;
    genmodel = makeprediction = false;
    sep = ", \t";

    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "-model") == 0 || strcmp(argv[i], "-m") == 0) {
        genmodel = true;
      }

      if (strcmp(argv[i], "-predict") == 0 || strcmp(argv[i], "-p") == 0) {
        makeprediction = true;
      }

      if (strcmp(argv[i], "-data") == 0 || strcmp(argv[i], "-i") == 0) {
        if (i + 1 < argc) {
          inputdata = argv[i + 1];
        }
      }

      //       if(strcmp(argv[i], "-label") == 0 || strcmp(argv[i], "-l") == 0){
      //         if(i+1 < argc){
      //           labelfname = argv[i+1];
      //         }
      //       }

      if (strcmp(argv[i], "-out") == 0 || strcmp(argv[i], "-o") == 0) {
        if (i + 1 < argc) {
          outputfile = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-c") == 0) {
        if (i + 1 < argc)
          npc = atoi(argv[i + 1]);
      }

      if (strcmp(argv[i], "-data-model") == 0 || strcmp(argv[i], "-dm") == 0) {
        if (i + 1 < argc)
          datamodel = argv[i + 1];
      }

      if (strcmp(argv[i], "-a") == 0) {
        if (i + 1 < argc)
          autoscaling = atoi(argv[i + 1]);
      }

      if (strcmp(argv[i], "-s") == 0) {
        if (i + 1 < argc)
          sep = argv[i + 1];
      }
    }

    if (genmodel == true && !inputdata.empty() && npc > 0 &&
        !outputfile.empty()) {
      matrix *data;
      PCAMODEL *m;

      initMatrix(&data);

      DATAIO::ImportMatrix(inputdata.c_str(), sep, data);
      NewPCAModel(&m);
      PCA(data, autoscaling, npc, m, NULL);

      DATAIO::WritePCAModel(outputfile.c_str(), m);

      DelPCAModel(&m);
      DelMatrix(&data);
    } else if (makeprediction == true && !datamodel.empty() &&
               !inputdata.empty() && !outputfile.empty() && npc > 0) {
      matrix *xdata;
      initMatrix(&xdata);

      DATAIO::ImportMatrix(inputdata.c_str(), sep, xdata);

      // PrintMatrix(xdata);

      PCAMODEL *m;
      NewPCAModel(&m);

      DATAIO::ImportPCAModel(datamodel.c_str(), m);

      matrix *xscores, *indvar;
      dvector *seps;

      initMatrix(&xscores);
      initMatrix(&indvar);
      initDVector(&seps);

      PCAScorePredictor(xdata, m, npc, xscores);
      PCAIndVarPredictor(xscores, m->loadings, m->colaverage, m->colscaling,
                         npc, indvar);

      if (DATAIO::DirExists(outputfile.c_str()) == true) {
        DATAIO::RemoveDir(outputfile.c_str());
      }

      DATAIO::MakeDir(outputfile.c_str());

      string pscores = outputfile + "/" + "T-Score-Pred.txt";
      string pindvar = outputfile + "/" + "Ind-Var-Pred.txt";
      DATAIO::WriteMatrix(pscores.c_str(), xscores);
      DATAIO::WriteMatrix(pindvar.c_str(), indvar);

      DelMatrix(&indvar);
      DelMatrix(&xscores);
      DelPCAModel(&m);
      DelMatrix(&xdata);
    } else {
      cout << "No option selected." << endl;
    }
  }
  return 0;
}
