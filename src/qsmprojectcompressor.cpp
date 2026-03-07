/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Core data structure or utility for projectcompressor.
 *
 * Copyright (C) 2016-2026 designed, written and mantained by Giuseppe Marco Randazzo <gmrandazzo@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "dircompressor.h"
#include <QString>
#include <cstdio>
#include <cstdlib>
#include <iostream>

int main(int argc, char **argv) {
  if (argc > 2) {
    DirCompressor dc;
    if (atoi(argv[1]) == 0) { // compress
      dc.setDir(argv[2]);
      dc.setOutput(argv[3]);
      dc.compress();
    } else {
      if (atoi(argv[1]) == 1) {
        dc.setFile(argv[2]);
        dc.setExtractPath(argv[3]);
        dc.decompress();
      } else {
        std::cout << "Error!!! compress or decompress!!" << std::endl;
      }
    }
    return 0;
  } else {
    std::cout << "Usage: " << std::endl;
    std::cout << argv[0] << " 0 <dir or file to compress>  <output name>"
              << std::endl;
    std::cout << argv[0] << " 1 <file to decompress>  <extraction path>"
              << std::endl;
    return 0;
  }
}
