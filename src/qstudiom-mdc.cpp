#include <iostream>
#include <cstdio>
#include "DATAIO.h"
#include <scientific.h>

int main(int argc, char **argv)
{
  if(argc >= 3){
    matrix *x;
    uivector *idsel;
    initMatrix(&x);
    DATAIO::ImportMatrix(argv[1], "\t", x);
    initUIVector(&idsel);
    if(argc == 3){
      MDC(x, 0, 0, &idsel, NULL);
    }
    else{
      MDC(x, atoi(argv[3]), 0, &idsel, NULL);
    }
    DATAIO::WriteUIvector(argv[2], idsel);
    DelMatrix(&x);
    DelUIVector(&idsel);
    return 0;
  }
  else{
    printf("\n Usage: %s <Input Table File> <Out File> <N compounds>\n", argv[0]);
    return 0;
  }
}
