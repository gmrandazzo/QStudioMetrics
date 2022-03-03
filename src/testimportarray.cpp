#include <iostream>
#include "DATAIO.h"
#include <scientific.h>

int main(int argc, char **argv)
{
  tensor *a;
  initTensor(&a);

  DATAIO::ImportArray(argv[1], " \t,;\n", a);

  PrintTensor(a);

  DelTensor(&a);
  return 0;
}
