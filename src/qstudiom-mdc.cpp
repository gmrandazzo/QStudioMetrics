#include <iostream>
#include <cstdio>
#include "DATAIO.h"


int main(int argc, char **argv)
{
  if(argc >= 4){
    matrix *x;
    uivector *idsel;
    initMatrix(&x);
    DATAIO::ImportMatrix(argv[1], "\t", x);
    initUIVector(&idsel);

    size_t nthreads;
    if(argc == 4){
      nthreads = atoi(argv[3]);
    }
    else{
      nthreads = atoi(argv[4]);
    }

    if(argc == 4){
      MDC(x, 0, 0, idsel, nthreads);
    }
    else{
      MaxDis_Fast(x, atoi(argv[3]), 0, idsel, nthreads);
    }
    DATAIO::WriteUIvector(argv[2], idsel);
    DelMatrix(&x);
    DelUIVector(&idsel);
    return 0;
  }
  else{
    printf("\n Usage: %s <Input Table File> <Out File> <N compounds> <nthreads>\n", argv[0]);
    return 0;
  }
}
