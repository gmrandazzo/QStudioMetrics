//QStudiom-GPSHumanizer
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>

#include "DATAIO.h"

#define FLOAT_EQ(x,v, EPSILON) (((v - EPSILON) < x) && (x <( v + EPSILON)))

using namespace std;

extern "C" {
  #include "scientific.h"
}


void help(char **argv)
{
  std::cout << "Usage:" << endl;
  std::cout << argv[0] << "\t -t <template coordinate file>" <<"\t -a <active coordinate file>" << "\t -i <inactive coordinate file>" << "\t -d <decoys coordinate file>" << "\t-l <ligands coordinate files>" << "\t-o <output file>" << "\t-r  <resolution int>" << std::endl;
  std::cout << "\t  Methods:\n * -cd <int> if you want compute the distance. The number is for select between different Metrick: 1 = Euclidean " << std::endl;
  std::cout << " * -a <int> if you want autoscale the data: 1 for column SDEV; 2 for column RMS"<< std::endl;
  std::cout << "\n "<< argv[0] << " was writen by Giuseppe Marco Randazzo <gmrandazzo@gmail.com> \n" << std::endl;
}

void getMinMax(dvector *v, double *min, double *max)
{
  (*min) = (*max) = getDVectorValue(v, 0);
  for(size_t i = 1; i < v->size; i++){
    if(getDVectorValue(v, i) < (*min))
      (*min) = getDVectorValue(v, i);
    else if(getDVectorValue(v, i) > (*max))
      (*max) = getDVectorValue(v, i);
    else
      continue;
  }
}

/*
 * normalize(x) = (x-min(x))/(max(x)-min(x))
 */
void Rescale(dvector *v, double min, double max)
{
  for(size_t i = 0; i < v->size; i++){
    setDVectorValue(v, i, (getDVectorValue(v, i)-min) / (max - min) );
  }
}

int main(int argc, char **argv)
{
  if(argc <= 1){
    help(argv);
  }
  else{
    int dtype = -1;
    size_t autoscaling = 0;
    string template_, active, inactive, decoys, ligands, outputpath;
    double res = 20, rmin = -99, rmax = -99;


    for(int i = 0; i < argc; i++){
      if(strcmp(argv[i], "-compute-distances") == 0 || strcmp(argv[i], "-cd") == 0){
        if(i+1 < argc){
          dtype = atoi(argv[i+1]); // type of distances
        }
      }

      if(strcmp(argv[i], "-template") == 0 || strcmp(argv[i], "-t") == 0){
        if(i+1 < argc){
          template_ = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-active") == 0 || strcmp(argv[i], "-a") == 0){
        if(i+1 < argc){
          active = argv[i+1];
        }
      }


      if(strcmp(argv[i], "-inactive") == 0 || strcmp(argv[i], "-i") == 0){
        if(i+1 < argc){
          inactive = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-decoys") == 0 || strcmp(argv[i], "-d") == 0){
        if(i+1 < argc){
          decoys = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-ligands") == 0 || strcmp(argv[i], "-l") == 0){
        if(i+1 < argc){
          ligands = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-out") == 0 || strcmp(argv[i], "-o") == 0){
        if(i+1 < argc){
          outputpath = argv[i+1];
        }
      }

      if(strcmp(argv[i], "-r") == 0){
        if(i+1 < argc)
          res = atof(argv[i+1]);
      }

      if(strcmp(argv[i], "-autoscaling") == 0 || strcmp(argv[i], "-as") == 0){
        if(i+1 < argc)
          autoscaling = atoi(argv[i+1]);
      }

      if(strcmp(argv[i], "-rmin") == 0){
        if(i+1 < argc)
          rmin = atof(argv[i+1]);
      }

      if(strcmp(argv[i], "-rmax") == 0){
        if(i+1 < argc)
          rmax = atof(argv[i+1]);
      }

    }

    if(dtype != -1  && !template_.empty() && !active.empty() && inactive.empty() && decoys.empty() && !outputpath.empty()){ //Make only sensitivity by computing distances
      matrix *a, *t;
      matrix *dtp, *s;
      dvector *mm;

      initMatrix(&dtp);
      NewDVector(&mm, 2); //Allocate min max dvector

      initMatrix(&a);
      initMatrix(&t);

      DATAIO::ImportMatrix(template_, " \t", t);
      DATAIO::ImportMatrix(active, " \t", a);

      if(autoscaling > 0){
        if(a->col == t->col){
          matrix *mx;
          dvector *colscaling;
          NewMatrix(&mx, a->row+t->row, a->col);
          initDVector(&colscaling);

          // Copy all data in one matrix.
          for(uint k = 0; k < a->row; k++)
            for(uint j = 0; j < a->col; j++)
              setMatrixValue(mx, k, j, getMatrixValue(a, k, j));

          for(uint k = 0; k < t->row; k++)
            for(uint j = 0; j < t->col; j++)
              setMatrixValue(mx, a->row+k, j, getMatrixValue(t, k, j));


          if(autoscaling == 1){
            MatrixColSDEV(mx, &colscaling);
          }
          else if(autoscaling == 2){
            MatrixColRMS(mx, &colscaling);
          }
          else{
            fprintf(stderr, "Unrecognized Autoscaling Method.");
            fflush(stderr);
            abort();
          }

          for(uint j = 0; j < a->col; j++){
            if(FLOAT_EQ(getDVectorValue(colscaling, j), 0, 1*10e-8)){
              for(uint k = 0; k < a->row; k++){
                setMatrixValue(a, k, j, 0);
              }

              for(uint k = 0; k < t->row; k++){
                setMatrixValue(t, k, j, 0);
              }
            }
            else{
              for(uint k = 0; k < a->row; k++){
                setMatrixValue(a, k, j, getMatrixValue(a, k, j)/getDVectorValue(colscaling, j));
              }

              for(uint k = 0; k < t->row; k++){
                setMatrixValue(t, k, j, getMatrixValue(t, k, j)/getDVectorValue(colscaling, j));
              }
            }
          }
          DelDVector(&colscaling);
        }
        else{
          fprintf(stderr, "No Autoscaling Possible. The number of column differ.");
          fflush(stderr);
          abort();
        }
      }

      EuclideanDistance(t, a, &dtp);

      if(DATAIO::CheckDir(outputpath) == true){
        DATAIO::RemoveDir(outputpath);
      }

      DATAIO::MakeDir(outputpath);

      DATAIO::WriteMatrix(outputpath+"/"+"TruePositiveDistances.txt", dtp);

      for(size_t k = 0; k < dtp->col; k++){
        dvector *v = getMatrixColumn(dtp, k);

        if(FLOAT_EQ(rmin, -99, 1*10e-8) || FLOAT_EQ(rmax, -99, 1*10e-8)){
          getMinMax(v, &rmin, &rmax);
        }

        Rescale(v, rmin, rmax);
        setDVectorValue(mm, 0, rmin);
        setDVectorValue(mm, 1, rmax);

        initMatrix(&s);
        Sensitivity(v, 0.0, 1.0, 1/res, &s);

        string oname = "Sensitivity-template-";
        stringstream ss;//create a stringstream
        ss << k+1;//add number to the stream
        oname.append(ss.str());
        oname.append(".txt");

        DATAIO::WriteMatrix(outputpath+"/"+oname, s);

        oname.clear();
        oname = "RangeMaxMin-template-";
        oname.append(ss.str());
        oname.append(".txt");

        vector<string> strv;
        strv.push_back("#In order to calculate the real distance you need to use this formula:");
        strv.push_back("#");
        strv.push_back("#  Real_Distance = Normalized_Distance * (max-min) + min ");

        DATAIO::WriteComments(outputpath+"/"+oname, strv);
        DATAIO::WriteDvector(outputpath+"/"+oname, mm);

        DelMatrix(&s);
        DelDVector(&v);
      }

      DelDVector(&mm);
      DelMatrix(&dtp);
      DelMatrix(&a);
      DelMatrix(&t);
    }
    else if(dtype == -1  && template_.empty() && !active.empty() && inactive.empty() && decoys.empty() && !outputpath.empty()){ //Make only sensitivity by computing distances
      matrix *dtp, *s;
      dvector *mm;

      initMatrix(&dtp);
      NewDVector(&mm, 2); //Allocate min max dvector

      DATAIO::ImportMatrix(active, " \t", dtp);

      if(DATAIO::CheckDir(outputpath) == true){
        DATAIO::RemoveDir(outputpath);
      }

      DATAIO::MakeDir(outputpath);

      for(size_t k = 0; k < dtp->col; k++){
        dvector *v = getMatrixColumn(dtp, k);

        if(FLOAT_EQ(rmin, -99, 1*10e-8) || FLOAT_EQ(rmax, -99, 1*10e-8)){
          getMinMax(v, &rmin, &rmax);
        }

        Rescale(v, rmin, rmax);

        setDVectorValue(mm, 0, rmin);
        setDVectorValue(mm, 1, rmax);

        initMatrix(&s);
        Sensitivity(v, 0.0, 1.0, 1/res, &s);

        string oname = "Sensitivity-template-";
        stringstream ss;//create a stringstream
        ss << k+1;//add number to the stream
        oname.append(ss.str());
        oname.append(".txt");

        DATAIO::WriteMatrix(outputpath+"/"+oname, s);

        oname.clear();
        oname = "RangeMaxMin-template-";
        oname.append(ss.str());
        oname.append(".txt");

        vector<string> strv;
        strv.push_back("#In order to calculate the real distance you need to use this formula:");
        strv.push_back("#");
        strv.push_back("#  Real_Distance = Normalized_Distance * (max-min) + min ");

        DATAIO::WriteComments(outputpath+"/"+oname, strv);
        DATAIO::WriteDvector(outputpath+"/"+oname, mm);

        DelMatrix(&s);
        DelDVector(&v);
      }

      DelDVector(&mm);
      DelMatrix(&dtp);
    }
    else if(dtype != -1 && !template_.empty() && !active.empty() && !inactive.empty() && decoys.empty() && !outputpath.empty()){ //Make senitivity and PPV by computing distances
      matrix *a, *i, *t;
      matrix *dtp, *dtn, *s, *ppv;
      dvector *mm;

      initMatrix(&dtp);
      initMatrix(&dtn);
      NewDVector(&mm, 2);

      initMatrix(&a);
      initMatrix(&i);
      initMatrix(&t);

      DATAIO::ImportMatrix(template_, " \t", t);
      DATAIO::ImportMatrix(active, " \t", a);
      DATAIO::ImportMatrix(inactive, " \t", i);

      if(autoscaling > 0){
        if(a->col == t->col){
          matrix *mx;
          dvector *colscaling;
          NewMatrix(&mx, a->row+t->row+i->row, a->col);
          initDVector(&colscaling);

          // Copy all data in one matrix.
          for(uint k = 0; k < a->row; k++)
            for(uint j = 0; j < a->col; j++)
              setMatrixValue(mx, k, j, getMatrixValue(a, k, j));

          for(uint k = 0; k < t->row; k++)
            for(uint j = 0; j < t->col; j++)
              setMatrixValue(mx, a->row+k, j, getMatrixValue(t, k, j));

          for(uint k = 0; k < i->row; k++)
            for(uint j = 0; j < i->col; j++)
              setMatrixValue(mx, a->row+t->row+k, j, getMatrixValue(i, k, j));

          if(autoscaling == 1){
            MatrixColSDEV(mx, &colscaling);
          }
          else if(autoscaling == 2){
            MatrixColRMS(mx, &colscaling);
          }
          else{
            fprintf(stderr, "Unrecognized Autoscaling Method.");
            fflush(stderr);
            abort();
          }

          for(uint j = 0; j < a->col; j++){
            if(FLOAT_EQ(getDVectorValue(colscaling, j), 0, 1*10e-8)){
              for(uint k = 0; k < a->row; k++){
                setMatrixValue(a, k, j, 0);
              }

              for(uint k = 0; k < t->row; k++){
                setMatrixValue(t, k, j, 0);
              }

              for(uint k = 0; k < i->row; k++){
                setMatrixValue(i, k, j, 0);
              }
            }
            else{
              for(uint k = 0; k < a->row; k++){
                setMatrixValue(a, k, j, getMatrixValue(a, k, j)/getDVectorValue(colscaling, j));
              }

              for(uint k = 0; k < t->row; k++){
                setMatrixValue(t, k, j, getMatrixValue(t, k, j)/getDVectorValue(colscaling, j));
              }

              for(uint k = 0; k < i->row; k++){
                setMatrixValue(i, k, j, getMatrixValue(i, k, j)/getDVectorValue(colscaling, j));
              }
            }
          }
          DelDVector(&colscaling);
          DelMatrix(&mx);
        }
        else{
          fprintf(stderr, "No Autoscaling Possible. The number of column differ.");
          fflush(stderr);
          abort();
        }
      }

      EuclideanDistance(t, a, &dtp);
      EuclideanDistance(t, i, &dtn);

      if(DATAIO::CheckDir(outputpath) == true){
        DATAIO::RemoveDir(outputpath);
      }

      DATAIO::MakeDir(outputpath);

      DATAIO::WriteMatrix(outputpath+"/"+"TruePositiveDistances.txt", dtp);
      DATAIO::WriteMatrix(outputpath+"/"+"TrueNegativeDistances.txt", dtn);

      for(size_t k = 0; k < dtp->col; k++){

        dvector *va = getMatrixColumn(dtp, k);
        dvector *vi = getMatrixColumn(dtn, k);

        if(FLOAT_EQ(rmin, -99, 1*10e-8) || FLOAT_EQ(rmax, -99, 1*10e-8)){
          double t_min1, t_max1, t_min2, t_max2;

          getMinMax(va, &t_min1, &t_max1);
          getMinMax(vi, &t_min2, &t_max2);

          if(t_min1 < t_min2)
            rmin = t_min1;
          else
            rmin = t_min2;

          if(t_max1 > t_max2)
            rmax = t_max1;
          else
            rmax = t_max2;
        }

        Rescale(va, rmin, rmax);
        Rescale(vi, rmin, rmax);

        setDVectorValue(mm, 0, rmin);
        setDVectorValue(mm, 1, rmax);

        initMatrix(&s);

        Sensitivity(va, 0.0, 1.0, 1/res, &s);

        string oname = "Sensitivity-active-";
        stringstream ss;//create a stringstream
        ss << k+1;//add number to the stream
        oname.append(ss.str());
        oname += ".txt";

        DATAIO::WriteMatrix(outputpath+"/"+oname, s);
        DelMatrix(&s);

        initMatrix(&s);

        Sensitivity(vi, 0.0, 1.0, 1/res, &s);

        oname.clear();
        oname = "Sensitivity-inactive-";
        oname.append(ss.str());
        oname += ".txt";

        DATAIO::WriteMatrix(outputpath+"/"+oname, s);

        initMatrix(&ppv);

        PositivePredictedValue(va, vi, 0.0, 1.0, 1/res, &ppv);

        oname.clear();
        oname = "PositivePredictedValue-";
        oname.append(ss.str());
        oname += ".txt";

        DATAIO::WriteMatrix(outputpath+"/"+oname, ppv);


        oname.clear();
        oname = "RangeMaxMin-";
        oname.append(ss.str());
        oname += ".txt";

        vector<string> strv;
        strv.push_back("#In order to calculate the real distance you need to use this formula:");
        strv.push_back("#");
        strv.push_back("#  Real_Distance = Normalized_Distance * (max-min) + min ");

        DATAIO::WriteComments(outputpath+"/"+oname, strv);
        DATAIO::WriteDvector(outputpath+"/"+oname, mm);

        DelMatrix(&s);
        DelMatrix(&ppv);
        DelDVector(&va);
        DelDVector(&vi);
      }

      DelDVector(&mm);
      DelMatrix(&dtp);
      DelMatrix(&dtn);

      DelMatrix(&a);
      DelMatrix(&t);
      DelMatrix(&i);
    }
    else if(dtype == -1 && template_.empty() && !active.empty() && !inactive.empty() && decoys.empty() && !outputpath.empty()){ //Make senitivity and PPV by computing distances
      matrix *dtp, *dtn, *s, *ppv;
      dvector *mm;

      initMatrix(&dtp);
      initMatrix(&dtn);
      NewDVector(&mm, 2);

      DATAIO::ImportMatrix(active, " \t", dtp);
      DATAIO::ImportMatrix(inactive, " \t", dtn);

      if(dtp->col == dtn->col){

        if(DATAIO::CheckDir(outputpath) == true){
          DATAIO::RemoveDir(outputpath);
        }

        DATAIO::MakeDir(outputpath);

        for(size_t k = 0; k < dtp->col; k++){

          dvector *va = getMatrixColumn(dtp, k);
          dvector *vi = getMatrixColumn(dtn, k);

          if(FLOAT_EQ(rmin, -99, 1*10e-8) || FLOAT_EQ(rmax, -99, 1*10e-8)){
            double t_min1, t_max1, t_min2, t_max2;

            getMinMax(va, &t_min1, &t_max1);
            getMinMax(vi, &t_min2, &t_max2);

            if(t_min1 < t_min2)
              rmin = t_min1;
            else
              rmin = t_min2;

            if(t_max1 > t_max2)
              rmax = t_max1;
            else
              rmax = t_max2;
          }

          Rescale(va, rmin, rmax);
          Rescale(vi, rmin, rmax);

          setDVectorValue(mm, 0, rmin);
          setDVectorValue(mm, 1, rmax);

          initMatrix(&s);

          Sensitivity(va, 0.0, 1.0, 1/res, &s);

          string oname = "Sensitivity-active-";
          stringstream ss;//create a stringstream
          ss << k+1;//add number to the stream
          oname.append(ss.str());
          oname += ".txt";

          DATAIO::WriteMatrix(outputpath+"/"+oname, s);
          DelMatrix(&s);

          initMatrix(&s);

          Sensitivity(vi, 0.0, 1.0, 1/res, &s);

          oname.clear();
          oname = "Sensitivity-inactive-";
          oname.append(ss.str());
          oname += ".txt";

          DATAIO::WriteMatrix(outputpath+"/"+oname, s);

          initMatrix(&ppv);

          PositivePredictedValue(va, vi, 0.0, 1.0, 1/res, &ppv);

          oname.clear();
          oname = "PositivePredictedValue-";
          oname.append(ss.str());
          oname += ".txt";

          DATAIO::WriteMatrix(outputpath+"/"+oname, ppv);


          oname.clear();
          oname = "RangeMaxMin-";
          oname.append(ss.str());
          oname += ".txt";

          vector<string> strv;
          strv.push_back("#In order to calculate the real distance you need to use this formula:");
          strv.push_back("#");
          strv.push_back("#  Real_Distance = Normalized_Distance * (max-min) + min ");

          DATAIO::WriteComments(outputpath+"/"+oname, strv);
          DATAIO::WriteDvector(outputpath+"/"+oname, mm);

          DelMatrix(&s);
          DelMatrix(&ppv);
          DelDVector(&va);
          DelDVector(&vi);
        }
      }
      else{
        fprintf(stderr, "Unable to do Sensitivity/Positive Predicted Value. There are different columns between True Positive and True Negative\n");
        fflush(stderr);
      }

      DelDVector(&mm);
      DelMatrix(&dtp);
      DelMatrix(&dtn);
    }
    else if(dtype != -1 && !template_.empty() && !active.empty() && !inactive.empty() && !decoys.empty() && !outputpath.empty()){ //Make senitivity, PPV and Abundance

      matrix *a, *i, *t, *d;
      matrix *dtp, *dtn, *dd, *s, *ppv, *abd;
      matrix *lig, *dlig;
      dvector *mm;

      initMatrix(&dtp);
      initMatrix(&dtn);
      initMatrix(&dd);
      NewDVector(&mm, 2);

      initMatrix(&a);
      initMatrix(&i);
      initMatrix(&t);
      initMatrix(&d);

      DATAIO::ImportMatrix(template_, " \t", t);
      DATAIO::ImportMatrix(active, " \t", a);
      DATAIO::ImportMatrix(inactive, " \t", i);
      DATAIO::ImportMatrix(decoys, " \t", d);


      if(!ligands.empty()){
        initMatrix(&lig);
        initMatrix(&dlig);
        DATAIO::ImportMatrix(ligands, " \t", lig);
      }


      if(autoscaling > 0){
        if(a->col == t->col){
          matrix *mx;
          dvector *colscaling;
          NewMatrix(&mx, a->row+t->row+i->row+d->row, a->col);
          initDVector(&colscaling);

          // Copy all data in one matrix.
          for(uint k = 0; k < a->row; k++)
            for(uint j = 0; j < a->col; j++)
              setMatrixValue(mx, k, j, getMatrixValue(a, k, j));

          for(uint k = 0; k < t->row; k++)
            for(uint j = 0; j < t->col; j++)
              setMatrixValue(mx, a->row+k, j, getMatrixValue(t, k, j));

          for(uint k = 0; k < i->row; k++)
            for(uint j = 0; j < i->col; j++)
              setMatrixValue(mx, a->row+t->row+k, j, getMatrixValue(i, k, j));

          for(uint k = 0; k < d->row; k++)
            for(uint j = 0; j < d->col; j++)
              setMatrixValue(mx, a->row+t->row+i->row+k, j, getMatrixValue(d, k, j));


          if(autoscaling == 1){
            MatrixColSDEV(mx, &colscaling);
          }
          else if(autoscaling == 2){
            MatrixColRMS(mx, &colscaling);
          }
          else{
            fprintf(stderr, "Unrecognized Autoscaling Method.");
            fflush(stderr);
            abort();
          }

          for(uint j = 0; j < a->col; j++){
            if(FLOAT_EQ(getDVectorValue(colscaling, j), 0, 1*10e-8)){
              for(uint k = 0; k < a->row; k++){
                setMatrixValue(a, k, j, 0);
              }

              for(uint k = 0; k < t->row; k++){
                setMatrixValue(t, k, j, 0);
              }

              for(uint k = 0; k < i->row; k++){
                setMatrixValue(i, k, j, 0);
              }

              for(uint k = 0; k < d->row; k++){
                setMatrixValue(d, k, j, 0);
              }
            }
            else{
              for(uint k = 0; k < a->row; k++){
                setMatrixValue(a, k, j, getMatrixValue(a, k, j)/getDVectorValue(colscaling, j));
              }

              for(uint k = 0; k < t->row; k++){
                setMatrixValue(t, k, j, getMatrixValue(t, k, j)/getDVectorValue(colscaling, j));
              }

              for(uint k = 0; k < i->row; k++){
                setMatrixValue(i, k, j, getMatrixValue(i, k, j)/getDVectorValue(colscaling, j));
              }

              for(uint k = 0; k < d->row; k++){
                setMatrixValue(d, k, j, getMatrixValue(d, k, j)/getDVectorValue(colscaling, j));
              }

              if(!ligands.empty()){
                for(uint k = 0; k < lig->row; k++){
                  setMatrixValue(lig, k, j, getMatrixValue(lig, k, j)/getDVectorValue(colscaling, j));
                }
              }

            }
          }
          DelDVector(&colscaling);
          DelMatrix(&mx);
        }
        else{
          fprintf(stderr, "No Autoscaling Possible. The number of column differ.");
          fflush(stderr);
          abort();
        }
      }


      EuclideanDistance(t, a, &dtp);
      EuclideanDistance(t, i, &dtn);
      EuclideanDistance(t, d, &dd);

      if(!ligands.empty()){
        EuclideanDistance(t, lig, &dlig);
      }

      if(DATAIO::CheckDir(outputpath) == true){
        DATAIO::RemoveDir(outputpath);
      }

      DATAIO::MakeDir(outputpath);


      DATAIO::WriteMatrix(outputpath+"/"+"TruePositiveDistances.txt", dtp);
      DATAIO::WriteMatrix(outputpath+"/"+"TrueNegativeDistances.txt", dtn);
      DATAIO::WriteMatrix(outputpath+"/"+"DecoysDistances.txt", dd);


      for(size_t k = 0; k < dtp->col; k++){

        dvector *va = getMatrixColumn(dtp, k);
        dvector *vi = getMatrixColumn(dtn, k);
        dvector *vd = getMatrixColumn(dd, k);

        if(FLOAT_EQ(rmin, -99, 1*10e-8) || FLOAT_EQ(rmax, -99, 1*10e-8)){
          double t_min1, t_max1, t_min2, t_max2/*, t_min3, t_max3*/;

          getMinMax(va, &t_min1, &t_max1);
          getMinMax(vi, &t_min2, &t_max2);
          /* getMinMax(vd, &t_min3, &t_max3);

          if(t_min1 < t_min2){
            if(t_min1 < t_min3){
              min = t_min1;
            }
            else{
              min = t_min3;
            }
          }
          else{
            if(t_min2 < t_min3){
              min = t_min2;
            }
            else{
              min = t_min3;
            }
          }

          if(t_max1 > t_max2){
            if(t_max1 > t_max3){
              max = t_max1;
            }
            else{
              max = t_max3;
            }
          }
          else{
            if(t_max2 > t_max3){
              max = t_max2;
            }
            else{
              max = t_max3;
            }
          }*/

          if(t_min1 < t_min2)
            rmin = t_min1;
          else
            rmin = t_min2;

          if(t_max1 > t_max2)
            rmax = t_max1;
          else
            rmax = t_max2;
        }

        Rescale(va, rmin, rmax);
        Rescale(vi, rmin, rmax);
        Rescale(vd, rmin, rmax);

        setDVectorValue(mm, 0, rmin);
        setDVectorValue(mm, 1, rmax);


        initMatrix(&s);

        Sensitivity(va, 0.0, 1.0, 1/res, &s);

        string oname = "Sensitivity-active-";
        stringstream ss;//create a stringstream
        ss << k+1;//add number to the stream
        oname.append(ss.str());
        oname += ".txt";

        DATAIO::WriteMatrix(outputpath+"/"+oname, s);
        DelMatrix(&s);

        initMatrix(&s);

        Sensitivity(vi, 0.0, 1.0, 1/res, &s);

        oname.clear();
        oname = "Sensitivity-inactive-";
        oname.append(ss.str());
        oname += ".txt";

        DATAIO::WriteMatrix(outputpath+"/"+oname, s);


        if(!ligands.empty()){
          dvector *vl = getMatrixColumn(dlig, k);
          Rescale(vl, rmin, rmax);
          DelMatrix(&s);

          initMatrix(&s);

          Sensitivity(vl, 0.0, 1.0, 1/res, &s);

          oname.clear();
          oname = "Sensitivity-ligands-";
          oname.append(ss.str());
          oname += ".txt";

          DATAIO::WriteMatrix(outputpath+"/"+oname, s);
          DelDVector(&vl);
        }


        initMatrix(&ppv);

        PositivePredictedValue(va, vi, 0.0, 1.0, 1/res, &ppv);

        oname.clear();
        oname = "PositivePredictedValue-";
        oname.append(ss.str());
        oname += ".txt";

        DATAIO::WriteMatrix(outputpath+"/"+oname, ppv);


        initMatrix(&abd);

        Sensitivity(vd, 0.0, 1.0, 1/res, &abd);

        oname.clear();
        oname = "AbundanceDecoys-";
        oname.append(ss.str());
        oname += ".txt";

        DATAIO::WriteMatrix(outputpath+"/"+oname, abd);


        oname.clear();
        oname = "RangeMaxMin-";
        oname.append(ss.str());
        oname += ".txt";

        vector<string> strv;
        strv.push_back("#In order to calculate the real distance you need to use this formula:");
        strv.push_back("#");
        strv.push_back("#  Real_Distance = Normalized_Distance * (max-min) + min ");

        DATAIO::WriteComments(outputpath+"/"+oname, strv);
        DATAIO::WriteDvector(outputpath+"/"+oname, mm);

        DelMatrix(&s);
        DelMatrix(&abd);
        DelMatrix(&ppv);
        DelDVector(&va);
        DelDVector(&vi);
        DelDVector(&vd);
      }

      DelDVector(&mm);
      DelMatrix(&dtp);
      DelMatrix(&dtn);
      DelMatrix(&dd);

      DelMatrix(&a);
      DelMatrix(&t);
      DelMatrix(&i);
      DelMatrix(&d);
    }
    else if(dtype == -1 && template_.empty() && !active.empty() && !inactive.empty() && !decoys.empty() && !outputpath.empty()){ //Make senitivity, PPV and Abundance
      matrix *dtp, *dtn, *dd, *s, *ppv, *abd;
      dvector *mm;


      initMatrix(&dtp);
      initMatrix(&dtn);
      initMatrix(&dd);
      NewDVector(&mm, 2);

      DATAIO::ImportMatrix(active, " \t", dtp);
      DATAIO::ImportMatrix(inactive, " \t", dtn);
      DATAIO::ImportMatrix(decoys, " \t", dd);

      if(dtp->col == dtn->col && dtp->col == dd->col){

        if(DATAIO::CheckDir(outputpath) == true){
          DATAIO::RemoveDir(outputpath);
        }

        DATAIO::MakeDir(outputpath);

        for(size_t k  = 0; k < dtp->col; k++){

          dvector *va = getMatrixColumn(dtp, k);
          dvector *vi = getMatrixColumn(dtn, k);
          dvector *vd = getMatrixColumn(dd, k);

          if(FLOAT_EQ(rmin, -99, 1*10e-8) || FLOAT_EQ(rmax, -99, 1*10e-8)){
            double t_min1, t_max1, t_min2, t_max2/*, t_min3, t_max3*/;

            getMinMax(va, &t_min1, &t_max1);
            getMinMax(vi, &t_min2, &t_max2);
            /* getMinMax(vd, &t_min3, &t_max3);

            if(t_min1 < t_min2){
              if(t_min1 < t_min3){
                min = t_min1;
              }
              else{
                min = t_min3;
              }
            }
            else{
              if(t_min2 < t_min3){
                min = t_min2;
              }
              else{
                min = t_min3;
              }
            }

            if(t_max1 > t_max2){
              if(t_max1 > t_max3){
                max = t_max1;
              }
              else{
                max = t_max3;
              }
            }
            else{
              if(t_max2 > t_max3){
                max = t_max2;
              }
              else{
                max = t_max3;
              }
            }*/

            if(t_min1 < t_min2)
              rmin = t_min1;
            else
              rmin = t_min2;

            if(t_max1 > t_max2)
              rmax = t_max1;
            else
              rmax = t_max2;
          }

          Rescale(va, rmin, rmax);
          Rescale(vi, rmin, rmax);
          Rescale(vd, rmin, rmax);


          setDVectorValue(mm, 0, rmin);
          setDVectorValue(mm, 1, rmax);

          initMatrix(&s);

          Sensitivity(va, 0.0, 1.0, 1/res, &s);

          string oname = "Sensitivity-active-";
          stringstream ss;//create a stringstream
          ss << k+1;//add number to the stream
          oname.append(ss.str());
          oname += ".txt";

          DATAIO::WriteMatrix(outputpath+"/"+oname, s);
          DelMatrix(&s);

          initMatrix(&s);

          Sensitivity(vi, 0.0, 1.0, 1/res, &s);

          oname.clear();
          oname = "Sensitivity-inactive-";
          oname.append(ss.str());
          oname += ".txt";

          DATAIO::WriteMatrix(outputpath+"/"+oname, s);

          initMatrix(&ppv);

          PositivePredictedValue(va, vi, 0.0, 1.0, 1/res, &ppv);

          oname.clear();
          oname = "PositivePredictedValue-";
          oname.append(ss.str());
          oname += ".txt";

          DATAIO::WriteMatrix(outputpath+"/"+oname, ppv);


          initMatrix(&abd);

          Sensitivity(vd, 0.0, 1.0, 1/res, &abd);

          oname.clear();
          oname = "AbundanceDecoys-";
          oname.append(ss.str());
          oname += ".txt";

          DATAIO::WriteMatrix(outputpath+"/"+oname, abd);


          oname.clear();
          oname = "RangeMaxMin-";
          oname.append(ss.str());
          oname += ".txt";

          vector<string> strv;
          strv.push_back("#In order to calculate the real distance you need to use this formula:");
          strv.push_back("#");
          strv.push_back("#  Real_Distance = Normalized_Distance * (max-min) + min ");

          DATAIO::WriteComments(outputpath+"/"+oname, strv);
          DATAIO::WriteDvector(outputpath+"/"+oname, mm);

          DelMatrix(&s);
          DelMatrix(&abd);
          DelMatrix(&ppv);
          DelDVector(&va);
          DelDVector(&vi);
        }
      }

      DelDVector(&mm);
      DelMatrix(&dtp);
      DelMatrix(&dtn);
      DelMatrix(&dd);
    }
    else{
      cout << "No option selected." << endl;
    }
  }
  return 0;

}
