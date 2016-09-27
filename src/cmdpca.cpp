#include <iostream>
#include <QFile>
#include <QList>
#include <QString>
#include <QTextStream>
#include <QStringList>
#include <cmath>

#include <scientific.h>

int main(int argc, char **argv)
{
  if(argc == 5){
    matrix *x;
    uint npc = atoi(argv[2]);
    uint scaling = atoi(argv[3]);
    
    QFile filex(argv[1]);
    if (!filex.open(QIODevice::ReadOnly | QIODevice::Text)){
      fprintf(stderr, "Error! Unable to open the file \"%s\"\n", argv[1]);
      return 0;
    }
    
    QTextStream xin(&filex);
    QList<QStringList> xitem;
    QStringList xname;
    while (!xin.atEnd()) {
      xitem.append(xin.readLine().split(";", QString::SkipEmptyParts));
    }
    
    for(uint i = 1; i < (uint)xitem.size(); i++) {
      xname.append(xitem[i].first());
    }

    
    NewMatrix(&x, xname.size(), xitem[1].size()-1);
    
    if(xname.size() > yname.size()){
      // alloc the memory of data struct
      
      NewMatrix(&y, yname.size(), yitem[1].size()-1);
      
      int cc = 0;
      for(uint i = 0; i < (uint)yname.size(); i++) {
        int xid = xname.indexOf(yname[i])+1;
        if(xid > -1){
          for(uint j = 1; j < (uint)xitem[xid].size(); j++ ){
            setMatrixValue(x, cc, j-1, xitem[xid][j].toDouble());
          }
          int yid = yname.indexOf(yname[i])+1;
          for(uint j = 1; j < (uint)yitem[yid].size(); j++ ){
            setMatrixValue(y, cc, j-1, yitem[yid][j].toDouble());
          }
          cc++;
        }
        else{
          continue;
        }
      }
    }
    else{
      if(xname.size() < yname.size()){
        NewMatrix(&x, xname.size(), xitem[1].size()-1);
        NewMatrix(&y, xname.size(), yitem[1].size()-1);
        
        int cc = 0;
        for(uint i = 0; i < (uint)xname.size(); i++) {
          int xid = xname.indexOf(xname[i])+1;
          if(xid > -1){
            for(uint j = 1; j < (uint)xitem[xid].size(); j++ ){
              setMatrixValue(x, cc, j-1, xitem[xid][j].toDouble());
            }
            int yid = yname.indexOf(xname[i])+1;
            for(uint j = 1; j < (uint)yitem[yid].size(); j++ ){
              setMatrixValue(y, cc, j-1, yitem[yid][j].toDouble());
            }
            cc++;
          }
          else{
            continue;
          }
        }
      }
      else{
        NewMatrix(&x, xname.size(), xitem[1].size()-1);
        NewMatrix(&y, yname.size(), yitem[1].size()-1);
        
        int cc = 0;
        for(uint i = 0; i < (uint)xname.size(); i++) {
          int xid = xname.indexOf(xname[i])+1;
          if(xid > -1){
            for(uint j = 1; j < (uint)xitem[xid].size(); j++ ){
              setMatrixValue(x, cc, j-1, xitem[xid][j].toDouble());
            }
            int yid = yname.indexOf(xname[i])+1;
            for(uint j = 1; j < (uint)yitem[yid].size(); j++){
              setMatrixValue(y, cc, j-1, yitem[yid][j].toDouble());
            }
            cc++;
          }
          else{
            continue;
          }
        }
      }
    }
    
    matrix *q2y, *sdep, *predicted_y;
    initMatrix(&q2y);
    initMatrix(&sdep);
    initMatrix(&predicted_y);
    PLSLOOCV(x, y, 1, 1, npc, NULL, &q2y, &sdep, &predicted_y, NULL, nthreads, NULL);
  
    /*PrintMatrix(x);
    PrintMatrix(y);
    PrintMatrix(predicted_y);
    PrintMatrix(q2y);
    PrintMatrix(sdep);*/
    
    /* Simple SDEP 
    double rmsd = 0.f;
    
    for(uint i = 0; i < y->row; i++){
      uint ny = y->col;
      for(uint j = 0; j < y->col; j++){
        rmsd += square(predicted_y->data[i][j+(ny*npc)-ny] - y->data[i][j]);
      }
    }
    
    rmsd /= y->row;
    rmsd = sqrt(rmsd);
    
    printf("Y_RMSD: %f\n", rmsd);
    */
    /*Spearman Sdep wich not consider the outliers */
    double sdep_spearmann = 0.f;
    matrix *smx;
    NewMatrix(&smx, y->row, 2);
    
    uint ny = y->col;
    for(uint j = 0; j < y->col; j++){
      double ymed = 0.f;
      for(uint i = 0; i < y->row; i++){
        smx->data[i][0] = y->data[i][j]; // real y
        ymed += y->data[i][j];
        smx->data[i][1] = predicted_y->data[i][j+(ny*npc)-ny]; // pred y
      }
      
      ymed /= (double)y->row;
      
      double sstot = 0.f;
      for(uint i = 0; i < y->row; i++){
        sstot += square(y->data[i][j] - ymed);
      }
     
      matrix *csmx;
      initMatrix(&csmx);
      SpearmanCorrelMatrix(smx, csmx);
      
      sdep_spearmann += sqrt(((1 - fabs(csmx->data[1][0])) * sstot)/(double)y->row);
      DelMatrix(&csmx);
    }
    
    printf("Y_SDEP_SPEARMAN: %f\n", sdep_spearmann);
    
    DelMatrix(&x);
    DelMatrix(&y);
    DelMatrix(&q2y);
    DelMatrix(&sdep);
    DelMatrix(&predicted_y);
    return 0;
  }
  else{
    std::cout << QString("\n Usage: %1 <x matrix> <y matrix> <npc> <nthreads>\n").arg(argv[0]).toStdString() << std::endl;
    std::cout << QString(" N.B.: x matrix and y matrix must have the first column as column name and the first row as variable name.\n The object names must be equal!! There is an auto aligner between xmatrix and ymatrix using the object names.").toStdString() << std::endl;
    return 0;
  }
}

