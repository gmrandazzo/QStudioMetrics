#include <iostream>
#include "DATAIO.h"
#include <scientific.h>

int main(int argc, char **argv)
{
  PCAMODEL *m;
  NewPCAModel(&m);

  DATAIO::ImportPCAModel(argv[1], m);
  
  PrintPCA(m);
  std::cout << "Var EXP" << std::endl;
  PrintDVector(m->varexp);
  std::cout << "Column Average" << std::endl;
  PrintDVector(m->colaverage);
  std::cout << "Column Scaling" << std::endl;
  PrintDVector(m->colscaling);
  
  DelPCAModel(&m);
  return 0;
}
