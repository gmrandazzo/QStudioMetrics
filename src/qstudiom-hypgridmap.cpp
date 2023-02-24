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
  std::cout << "Make a model: " << argv[0] <<"\t -i <input file>" << "\t-b <output bin appartenence>" << "\t-g <grid step size>\t-o <output grid map>\t-nth <number of threads>" <<std::endl;
  std::cout << "Predict     : " << argv[0] <<"\t -i <input file>" << "\t-h <hyper grid map dir>" << "\t-b <output bin appartenence>\t-nth <number of threads>" <<std::endl;
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
      
      // Calculate the manhattan distance from the centroid of points and the mahlanobis Distance
      /*
      matrix *centroid;
      NewMatrix(&centroid, 1, data->col);
      for(size_t j = 0; j < data->col; j++){
        for(size_t i = 0; i < data->row; i++){
          centroid->data[0][j] += data->data[i][j];
        }
        centroid->data[0][j] /= (double) data->row;
      }

      matrix *dsts;
      NewMatrix(&dsts, data->row, 2);

      puts("Calculate ManhattanDistance");
      matrix *mandst;
      initMatrix(&mandst);
      ManhattanDistance(data, centroid, &mandst, nthreads);
      puts("Done");

      for(size_t i = 0; i < data->row; i++){
        dsts->data[i][0] = mandst->data[0][i];
      }
      DelMatrix(&mandst);

      dvector *mdst;
      matrix *invcov;
      initDVector(&mdst);
      initMatrix(&invcov);

      puts("Calculate MahalanobisDistance");
      // in this case mu is the same of colaverage!
      MahalanobisDistance(data, &invcov, NULL, &mdst);
      PrintDVector(mdst);
      puts("Done");

      for(size_t i = 0; i < mdst->size; i++){
        dsts->data[i][1] = mdst->data[i];
      }
      DelDVector(&mdst);
      */

      HyperGridModel *hgm;
      hgmbins *bins_id;
      NewHyperGridMap(&hgm);
      HyperGridMap(data, grid_step_size, &bins_id, &hgm);
      printf("Total number of bins : %e %zu\n", hgm->bsize, hgm->gsize);
      DATAIO::MakeDir((char*)outhgm.c_str());
      string gmap_dir = outhgm+"/gmap.txt";
      string colavg_dir = outhgm+"/colavg.txt";
      string colscaling_dir = outhgm+"/colscaling.txt";
      /*string centroid_dir = outhgm+"/centroid.txt";
      string invcov_dir = outhgm+"/invcov.txt";
      string dsts_dir = outhgm+"/dists.txt";*/
      DATAIO::WriteMatrix((char*)gmap_dir.c_str(), hgm->gmap);
      //DATAIO::WriteMatrix((char*)dsts_dir.c_str(), dsts);
      DATAIO::WriteDvector((char*)colavg_dir.c_str(), hgm->colaverage);
      DATAIO::WriteDvector((char*)colscaling_dir.c_str(), hgm->colscaling);
      /* Store the centroid
      DATAIO::WriteMatrix((char*)centroid_dir.c_str(), centroid);*/
      /* Store the inverse covariance matrix
      DATAIO::WriteMatrix((char*)invcov_dir.c_str(), invcov);*/

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
      /*DelMatrix(&dsts);
      DelMatrix(&centroid);
      DelMatrix(&invcov);*/
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
      string gmap_dir = inhgm+"/gmap.txt";
      DATAIO::ImportMatrix((char*)gmap_dir.c_str(), sep, hgm->gmap);
      grid_step_size = ceil((hgm->gmap->data[0][1]-hgm->gmap->data[0][0])/hgm->gmap->data[0][2]);
      hgm->gsize = grid_step_size;
      hgm->bsize = 1;
      for(size_t i = 0; i < hgm->gmap->col; i++){
        hgm->bsize *= grid_step_size;
      }
      /*
      matrix *centroid;
      string centroid_dir = inhgm+"/centroid.txt";
      initMatrix(&centroid);
      DATAIO::ImportMatrix((char*)centroid_dir.c_str(), sep, centroid);

      string invcov_dir = inhgm+"/invcov.txt";
      matrix *invcov;
      initMatrix(&invcov);
      DATAIO::ImportMatrix((char*)invcov_dir.c_str(), sep, invcov);

      string colavg_dir = inhgm+"/colavg.txt";
      dvector *colavg;
      initDVector(&colavg);
      DATAIO::ImportDvector((char*)colavg_dir.c_str(), colavg);

      matrix *dsts;
      NewMatrix(&dsts, data->row, 2);

      matrix *mandst;
      initMatrix(&mandst);
      ManhattanDistance(data, centroid, &mandst, nthreads);

      for(size_t i = 0; i < data->row; i++){
        dsts->data[i][0] = mandst->data[0][i];
      }
      DelMatrix(&mandst);

      dvector *mdst;
      initDVector(&mdst);
      // in this case mu is the same of colaverage!
      MahalanobisDistance(data, &invcov, &colavg, &mdst);

      for(size_t i = 0; i < mdst->size; i++){
        dsts->data[i][1] = mdst->data[i];
      }
      DelDVector(&mdst);
      */
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
      /*DelDVector(&colavg);
      DelMatrix(&invcov);
      DelMatrix(&dsts);*/
    }
    else{
      cout << "No option selected." << endl;
    }
  }
  return 0;
}
