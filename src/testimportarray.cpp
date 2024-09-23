#include "DATAIO.h"
#include <iostream>
#include <scientific.h>

int main(int argc, char **argv) {
  tensor *a;
  initTensor(&a);

  DATAIO::ImportTensor(argv[1], " \t,;\n", a);

  PrintTensor(a);

  DelTensor(&a);
  return 0;
}
