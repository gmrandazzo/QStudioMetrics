#include <iostream>
#include "DATAIO.h"
#include <scientific.h>

int main(int argc, char **argv)
{
  dvector *v;
  initDVector(&v);

  DATAIO::ImportDvector(argv[1], v);

  PrintDVector(v);

  DelDVector(&v);
  return 0;
}
