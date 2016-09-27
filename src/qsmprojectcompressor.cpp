#include "dircompressor.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <QString>

int main(int argc, char **argv)
{
  if(argc > 2){
    DirCompressor dc;
    if(atoi(argv[1]) == 0){ // compress
      dc.setDir(argv[2]);
      dc.setOutput(argv[3]);
      dc.compress();
    }
    else{
      if(atoi(argv[1]) == 1){
        dc.setFile(argv[2]);
        dc.setExtractPath(argv[3]);
        dc.decompress();
      }
      else{
        std::cout << "Error!!! compress or decompress!!" << std::endl;
      }
    }
    return 0;
  }
  else{
    std::cout << "Usage: " << std::endl;
    std::cout << argv[0] << " 0 <dir or file to compress>  <output name>" << std::endl;
    std::cout << argv[0] << " 1 <file to decompress>  <extraction path>" << std::endl;
    return 0;
  }
}

