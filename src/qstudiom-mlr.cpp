//QStudiom-pls
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
  std::cout << "Make a model: " << argv[0] <<" -model -x <xinput file>  -y <yinput file>" << " -o <output file>\n"  << std::endl;
  std::cout << "Make a prediction: " << argv[0] <<" -predict -dm <input model> -x <xinput file> -o <output file>\n" << std::endl;
  std::cout << "Make the cross validation: " << argv[0] <<" -cv -x  <xinput file>  -y <yinput file> -dm <input path model> -g <N° of groups> -i <N° iterations> -nth <N° threads>\n" << std::endl;

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
    size_t ngroups = 0, iterations = 20, nthreads = 1;
    string xinputdata, yinputdata, pathmodel, outputfile, xsep, ysep;
    bool genmodel, makeprediction, makecrossvalidation, verbose;
    verbose = genmodel = makeprediction = makecrossvalidation = false;
    xsep = ysep = ";";

    for(int i = 0; i < argc; i++){
      if(strcmp(argv[i], "-model") == 0 || strcmp(argv[i], "-m") == 0){
        genmodel = true;
      }
      if(strcmp(argv[i], "-predict") == 0 || strcmp(argv[i], "-p") == 0){
        makeprediction = true;
      }

      if(strcmp(argv[i], "-cross-validation") == 0 || strcmp(argv[i], "-cv") == 0){
        makecrossvalidation = true;
      }

      if(strcmp(argv[i], "-xdata") == 0 || strcmp(argv[i], "-x") == 0){
        if(i+1 < argc){
          xinputdata = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-ydata") == 0 || strcmp(argv[i], "-y") == 0){
        if(i+1 < argc){
          yinputdata = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-out") == 0 || strcmp(argv[i], "-o") == 0){
        if(i+1 < argc){
          outputfile = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-g") == 0){
        if(i+1 < argc)
          ngroups = atoi(argv[i+1]);
      }

      if(strcmp(argv[i], "-i") == 0){
        if(i+1 < argc)
          iterations = atoi(argv[i+1]);
      }

      if(strcmp(argv[i], "-data-model") == 0 || strcmp(argv[i], "-dm") == 0){
        if(i+1 < argc)
          pathmodel = argv[i+1];
      }

      if(strcmp(argv[i], "-sx") == 0){
        if(i+1 < argc)
          xsep = argv[i+1];
      }

      if(strcmp(argv[i], "-sy") == 0){
        if(i+1 < argc)
          ysep = argv[i+1];
      }

      if(strcmp(argv[i], "-s") == 0){
        if(i+1 < argc)
          xsep = ysep = argv[i+1];
      }

      if(strcmp(argv[i], "-nth") == 0){
        if(i+1 < argc)
          nthreads = atoi (argv[i+1]);
      }

      if(strcmp(argv[i], "-verbose") == 0 || strcmp(argv[i], "-v") == 0){
        verbose = true;
      }
    }

    if(genmodel == true && !xinputdata.empty() && !yinputdata.empty() && !outputfile.empty()){
      matrix *xdata, *ydata;

      initMatrix(&xdata);
      initMatrix(&ydata);

      DATAIO::ImportMatrix((char*)xinputdata.c_str(), xsep, xdata);
      DATAIO::ImportMatrix((char*)yinputdata.c_str(), ysep, ydata);

      MLRMODEL *m;
      NewMLRModel(&m);

      if(verbose == true){
        printf("x row %lu col %lu\n", xdata->row, xdata->col);
        printf("y row %lu col %lu\n", ydata->row, ydata->col);
      }

      MLR(xdata, ydata, m, NULL);
      MLRRegressionStatistics(ydata, m->recalculated_y, &(m->r2y_model), &(m->sdec), NULL);

      DATAIO::WriteMLRModel((char*)outputfile.c_str(), m);

      DelMLRModel(&m);
      DelMatrix(&xdata);
      DelMatrix(&ydata);
    }
    else if(makeprediction == true && !pathmodel.empty() && !xinputdata.empty() && !outputfile.empty()){
      matrix *xdata;
      initMatrix(&xdata);

      DATAIO::ImportMatrix((char*)xinputdata.c_str(), xsep, xdata);

      MLRMODEL *m;
      NewMLRModel(&m);
      DATAIO::ImportMLRModel((char*)pathmodel.c_str(), m);

      matrix *predicted_y;
      initMatrix(&predicted_y);
      MLRPredictY(xdata, NULL, m, &predicted_y, NULL, NULL, NULL);

      DATAIO::MakeDir((char*)outputfile.c_str());
      string ypred = outputfile+"/Y-Pred.txt";
      DATAIO::WriteMatrix((char*)ypred.c_str(), predicted_y);

      DelMatrix(&predicted_y);
      DelMLRModel(&m);
      DelMatrix(&xdata);
    }
    else if(makecrossvalidation == true && !xinputdata.empty() && !yinputdata.empty() && ngroups > 0){
      matrix *xdata, *ydata;
      matrix *pred, *predresiduals;
      dvector *q2y, *sdep, *bias;

      initMatrix(&xdata);
      initMatrix(&ydata);
      DATAIO::ImportMatrix((char*)xinputdata.c_str(), xsep, xdata);
      DATAIO::ImportMatrix((char*)yinputdata.c_str(), ysep, ydata);


      initDVector(&q2y);
      initDVector(&sdep);
      initDVector(&bias);
      initMatrix(&pred);
      initMatrix(&predresiduals);

      MODELINPUT minpt;
      minpt.mx = &xdata;
      minpt.my = &ydata;

      BootstrapRandomGroupsCV(&minpt, ngroups, iterations, _MLR_, &pred, &predresiduals, nthreads, NULL, 0);
      MLRRegressionStatistics(ydata, pred, &q2y, &sdep, &bias);

      if(!pathmodel.empty()){
        string q2yfile = pathmodel+"/Validated_q2y.txt";
        string sdepfile = pathmodel+"/Validated_sdep.txt";
        string predfile = pathmodel+"/Validated_Predicted_Y.txt";
        string resfile = pathmodel+"/Validated_Predicted_Residuals.txt";
        DATAIO::WriteDvector((char*)q2yfile.c_str(), q2y);
        DATAIO::WriteDvector((char*)sdepfile.c_str(), sdep);
        DATAIO::WriteMatrix((char*)predfile.c_str(), pred);
        DATAIO::WriteMatrix((char*)resfile.c_str(), predresiduals);
      }

      /*
      cout.setf(ios_base::right,ios_base::adjustfield);
      cout.setf(ios::fixed,ios::floatfield);

      cout << "Cross Validated Q^2" << endl;
      for(size_t i = 0; i < q2y->row; i++){
        for(size_t j = 0; j < q2y->col; j++){
          if(j == 0)
            cout << "PC " << i+1 << ": " << setw(3) << getMatrixValue(q2y, i, j);
          else if(j == q2y->col-1)
            cout << setw(10) << getMatrixValue(q2y, i, j);
          else
            cout << setw(10) << getMatrixValue(q2y, i, j);
        }
        cout << endl;
      }

      cout << "SDEP" << endl;
      for(size_t i = 0; i < sdep->row; i++){
        for(size_t j = 0; j < sdep->col; j++){
          if(j == 0)
            cout << "PC " << i+1 << ": " << setw(3) << getMatrixValue(sdep, i, j);
          else if(j == sdep->col-1)
            cout << setw(10) << getMatrixValue(sdep, i, j);
          else
            cout << setw(10) << getMatrixValue(sdep, i, j);
        }
        cout << endl;
      }

      cout << "BIAS" << endl;
      for(size_t i = 0; i < bias->row; i++){
        for(size_t j = 0; j < bias->col; j++){
          if(j == 0)
            cout << "PC " << i+1 << ": " << setw(3) << getMatrixValue(bias, i, j);
          else if(j == bias->col-1)
            cout << setw(10) << getMatrixValue(bias, i, j);
          else
            cout << setw(10) << getMatrixValue(bias, i, j);
        }
        cout << endl;
      }

      cout << "YScrambling Q^2" << endl;
      for(size_t i = 0; i < q2y_consistency->row; i++){
        for(size_t j = 0; j < q2y_consistency->col; j++){
          if(j == 0)
            cout << "PC " << i+1 << ": " << setw(3) << getMatrixValue(q2y_consistency, i, j);
          else if(j == q2y_consistency->col-1)
            cout << setw(10) << getMatrixValue(q2y_consistency, i, j) << endl;
          else
            cout << setw(10) << getMatrixValue(q2y_consistency, i, j);
        }
        cout << endl;
      }

      cout << "YScrambling SDEP" << endl;
      for(size_t i = 0; i < sdep_consistency->row; i++){
        for(size_t j = 0; j < sdep_consistency->col; j++){
          if(j == 0)
            cout << "PC " << i+1 << ": " << setw(3) << getMatrixValue(sdep_consistency, i, j);
          else if(j == sdep_consistency->col-1)
            cout << setw(10) << getMatrixValue(sdep_consistency, i, j) << endl;
          else
            cout << setw(10) << getMatrixValue(sdep_consistency, i, j);
        }
        cout << endl;
      }
      */

      DelDVector(&q2y);
      DelDVector(&bias);
      DelDVector(&sdep);
      DelMatrix(&pred);
      DelMatrix(&predresiduals);
      DelMatrix(&xdata);
      DelMatrix(&ydata);
    }
    else{
      cout << "Error!" << endl;
      help(argv);
    }
  }
  return 0;

}
