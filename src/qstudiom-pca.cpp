//QStudiom-pca
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>

#include "DATAIO.h"

#include <vector>
#include <getopt.h>
#include <cstring>
using namespace std;

#include "scientific.h"


void help(char **argv)
{
  std::cout << "Usage" << endl;
  std::cout << "Make a model: " << argv[0] <<"\t -model -i <input file>" << "\t-o <output file>" << "\t-c <N° of PC> \t -a (autoscaling)" <<std::endl;
  std::cout << "Make a prediction: " << argv[0] <<"\t -predict -dm <input model> -i <input file>" << "\t-o <output file>" << "\t-c <N° of PC>" << std::endl;
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
    size_t npc = 0, autoscaling = 0;
    string inputdata, labelfname, datamodel, outputfile, sep;
    bool genmodel, makeprediction;
    genmodel = makeprediction = false;
    sep = " \t";
    
    for(int i = 0; i < argc; i++){
      if(strcmp(argv[i], "-model") == 0 || strcmp(argv[i], "-m") == 0){
        genmodel = true;
      }
      
      if(strcmp(argv[i], "-predict") == 0 || strcmp(argv[i], "-p") == 0){
        makeprediction = true;
      }
      
      if(strcmp(argv[i], "-data") == 0 || strcmp(argv[i], "-i") == 0){
        if(i+1 < argc){
          inputdata = argv[i+1];
        }
      }
            
//       if(strcmp(argv[i], "-label") == 0 || strcmp(argv[i], "-l") == 0){
//         if(i+1 < argc){
//           labelfname = argv[i+1];
//         }
//       }
      
      if(strcmp(argv[i], "-out") == 0 || strcmp(argv[i], "-o") == 0){
        if(i+1 < argc){
          outputfile = argv[i+1];
        }
      }
      
      if(strcmp(argv[i], "-c") == 0){
        if(i+1 < argc)
          npc = atoi(argv[i+1]);
      }
      
      if(strcmp(argv[i], "-data-model") == 0 || strcmp(argv[i], "-dm") == 0){
        if(i+1 < argc)
          datamodel = argv[i+1];
      }
      
      if(strcmp(argv[i], "-a") == 0){
        if(i+1 < argc)
          autoscaling = atoi(argv[i+1]);
      }
      
      if(strcmp(argv[i], "-s") == 0){
        if(i+1 < argc)
          sep = argv[i+1];
      }

    }
    
    if(genmodel == true && !inputdata.empty() && npc > 0 && !outputfile.empty()){
      matrix *data;
      PCAMODEL *m;
      
      initMatrix(&data);
      
      DATAIO::ImportMatrix((char*)inputdata.c_str(), sep, data);
      NewPCAModel(&m);
      PCA(data, autoscaling, npc, m, NULL);
      
      DATAIO::WritePCAModel((char*)outputfile.c_str(), m);
      
      DelPCAModel(&m);
      DelMatrix(&data);
    }
    else if(makeprediction == true && !datamodel.empty() && !inputdata.empty() && npc > 0){
      matrix *xdata;
      initMatrix(&xdata);
      
      DATAIO::ImportMatrix((char*)inputdata.c_str(), sep, xdata);
      
      PrintMatrix(xdata);
      
      PCAMODEL *m;
      NewPCAModel(&m);
      
      DATAIO::ImportPCAModel((char*)datamodel.c_str(), m);

      matrix *xscores, *indvar;
      dvector *seps;
      
      initMatrix(&xscores);
      initMatrix(&indvar);
      initDVector(&seps);
      
      PCAScorePredictor(xdata, m, npc, &xscores);
      PCAIndVarPredictor(xscores, m->loadings, m->colaverage, m->colscaling, npc, &indvar);
      
      string pscores = datamodel+"T-Score-Pred.txt";
      string pindvar = datamodel+"Ind-Var-Pred.txt";
      DATAIO::WriteMatrix((char*)pscores.c_str(), xscores);
      DATAIO::WriteMatrix((char*)pindvar.c_str(), indvar);
      
      DelMatrix(&indvar);
      DelMatrix(&xscores);
      DelPCAModel(&m);
      DelMatrix(&xdata);
    }
    else{
      cout << "No option selected." << endl;
    }
  }
  return 0;
  
}
