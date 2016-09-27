#include <iostream>
#include "DATAIO.h"
#include <scientific.h>

int main(int argc, char **argv)
{
  array *a;
  initArray(&a);
  
  DATAIO::ImportArray(argv[1], " \t,;\n", a);
  
  PrintArray(a);
  
  DelArray(&a);
  return 0;
}
