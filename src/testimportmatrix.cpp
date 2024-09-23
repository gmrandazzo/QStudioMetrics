#include "DATAIO.h"
#include <iostream>
#include <scientific.h>

int main(int argc, char **argv) {
  matrix *m;
  initMatrix(&m);

  DATAIO::ImportMatrix(argv[1], " \t,;\n", m);

  PrintMatrix(m);

  DelMatrix(&m);
  return 0;
}
