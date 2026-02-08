/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for qstudiom-kmeans.
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

// QStudiom-clustering
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
  std::cout << "Make a model: " << argv[0] << "\t -i <input file>"
            << "\t-o <output file>"
            << "\t-c <centroid outputs>\t-t <initializer type>\t-n <number of "
               "clusters>\t-nth <number of threads>"
            << std::endl;
  std::cout << "Initializer types:" << std::endl;
  std::cout << " 0 = random" << std::endl;
  std::cout << " 1 = D. Arthur k-means++ " << std::endl;
  std::cout << " 2 = MDC initialization " << std::endl;
  std::cout << " 3 = Diss compound initialization" << std::endl;
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
    int n_clusters = 0, init = 1, nthreads = 1;
    string inputdata, outcentroids, outputfile;

    for (int i = 0; i < argc; i++) {
      if (strcmp(argv[i], "-input") == 0 || strcmp(argv[i], "-i") == 0) {
        if (i + 1 < argc) {
          inputdata = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-out") == 0 || strcmp(argv[i], "-o") == 0) {
        if (i + 1 < argc) {
          outputfile = argv[i + 1];
        }
      }

      if (strcmp(argv[i], "-centroids") == 0 || strcmp(argv[i], "-c") == 0) {
        if (i + 1 < argc)
          outcentroids = argv[i + 1];
      }

      if (strcmp(argv[i], "-nclusters") == 0 || strcmp(argv[i], "-n") == 0) {
        if (i + 1 < argc) {
          n_clusters = atoi(argv[i + 1]);
        }
      }

      if (strcmp(argv[i], "-init") == 0 || strcmp(argv[i], "-t") == 0) {
        if (i + 1 < argc) {
          init = atoi(argv[i + 1]);
        }
      }

      if (strcmp(argv[i], "-nth") == 0) {
        if (i + 1 < argc) {
          nthreads = atoi(argv[i + 1]);
        }
      }
    }

    if (!inputdata.empty() && !outputfile.empty() && !outcentroids.empty() &&
        n_clusters > 0) {
      string sep = ", \t";
      matrix *data;
      initMatrix(&data);
      matrix *centroids;
      initMatrix(&centroids);
      uivector *clusters;
      initUIVector(&clusters);
      DATAIO::ImportMatrix(inputdata.c_str(), sep, data);
      KMeans(data, n_clusters, init, clusters, centroids, nthreads);
      DATAIO::WriteUIvector(outputfile.c_str(), clusters);
      DATAIO::WriteMatrix(outcentroids.c_str(), centroids);
      DelUIVector(&clusters);
      DelMatrix(&centroids);
      DelMatrix(&data);
    } else {
      cout << "No option selected." << endl;
    }
  }
  return 0;
}
