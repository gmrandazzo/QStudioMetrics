//QStudiom-hypergridmap
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

#include "DATAIO.h"

#include <vector>
#include <getopt.h>
#include <cstring>
using namespace std;

#include "scientific.h"


void help(char **argv)
{
  std::cout << "Usage" << endl;
  std::cout << "Make a model: " << argv[0] <<"\t -i <input file>" << "\t-b <output bin appartenence>" << "\t-g <grid step size>\t-o <output grid map>" <<std::endl;
  std::cout << "Predict     : " << argv[0] <<"\t -i <input file>" << "\t-h <hyper grid map dir>" << "\t-b <output bin appartenence>" <<std::endl;
  std::cout << "\n\t ---------------------------------------------------------------------------- " << std::endl;
  std::cout << "\t | "<< argv[0] << " was writen by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>  |" << std::endl;
  std::cout << "\t ---------------------------------------------------------------------------- \n" << std::endl;
}

int main(int argc, char **argv)
{
  if(argc <= 1){
    help(argv);
  }
  else{
    size_t grid_step_size = 0;
    string inputdata, inhgm, outbins, outhgm, sep;
    sep = ", \t";

    for(int i = 0; i < argc; i++){
      if(strcmp(argv[i], "-input") == 0 || strcmp(argv[i], "-i") == 0){
        if(i+1 < argc){
          inputdata = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-out") == 0 || strcmp(argv[i], "-o") == 0){
        if(i+1 < argc){
          outhgm = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-outbins") == 0 || strcmp(argv[i], "-b") == 0){
        if(i+1 < argc)
          outbins = argv[i+1];
      }

      if(strcmp(argv[i], "-grid") == 0 || strcmp(argv[i], "-g") == 0){
        if(i+1 < argc){
          grid_step_size = atoi(argv[i+1]);
        }
      }

      if(strcmp(argv[i], "-hgminput") == 0 || strcmp(argv[i], "-h") == 0){
        if(i+1 < argc){
          inhgm = argv[i+1];
        }
      }
    }


    if(!inputdata.empty() &&
      !outbins.empty() &&
      !outhgm.empty() &&
      inhgm.empty() &&
      grid_step_size > 0){ // make a model
      matrix *data;
      initMatrix(&data);
      DATAIO::ImportMatrix((char*)inputdata.c_str(), sep, data);

      HyperGridModel *hgm;
      hgmbins *bins_id;
      NewHyperGridMap(&hgm);
      HyperGridMap(data, grid_step_size, &bins_id, &hgm);
      printf("Total number of bins : %lf %zu\n", hgm->bsize, hgm->gsize);
      DATAIO::MakeDir((char*)outhgm.c_str());
      string gmap = outhgm+"/gmap.txt";
      DATAIO::WriteMatrix((char*)gmap.c_str(), hgm->gmap);

      ofstream fbins;
      fbins.open(outbins);
      for(size_t i = 0; i < bins_id->nobj; i++){
        std::string hashstr;
        for(size_t j = 0; j < bins_id->hash_size-1; j++)
          hashstr += std::to_string(bins_id->hash[i][j]) + "-";
        hashstr += std::to_string(bins_id->hash[i][bins_id->hash_size-1]) + "\n";
        fbins << hashstr;
      }
      fbins.close();

      DelHyperGridMap(&hgm);
      DelHGMBins(&bins_id);
      DelMatrix(&data);
    }
    else if(!inputdata.empty() &&
            !inhgm.empty() &&
            !outbins.empty() &&
            outhgm.empty() &&
            grid_step_size == 0){ // make prediction
      matrix *data;
      initMatrix(&data);
      DATAIO::ImportMatrix((char*)inputdata.c_str(), sep, data);
      hgmbins *bins_id;
      HyperGridModel *hgm;
      NewHyperGridMap(&hgm);
      string gmap = inhgm+"/gmap.txt";
      DATAIO::ImportMatrix((char*)gmap.c_str(), sep, hgm->gmap);
      grid_step_size = ceil((hgm->gmap->data[0][1]-hgm->gmap->data[0][0])/hgm->gmap->data[0][2]);
      hgm->gsize = grid_step_size;
      hgm->bsize = 1;
      for(size_t i = 0; i < hgm->gmap->col; i++){
        hgm->bsize *= grid_step_size;
      }
      HyperGridMapObjects(data, hgm, &bins_id);

      ofstream fbins;
      fbins.open(outbins);
      for(size_t i = 0; i < bins_id->nobj; i++){
        std::string hashstr;
        for(size_t j = 0; j < bins_id->hash_size; j++)
          hashstr += std::to_string(bins_id->hash[i][j]);
        fbins << hashstr << "\n";
      }
      fbins.close();

      DelHyperGridMap(&hgm);
      DelHGMBins(&bins_id);
      DelMatrix(&data);
    }
    else{
      cout << "No option selected." << endl;
    }
  }
  return 0;
}
