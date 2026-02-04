/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Implementation file for qstudiom-mdc.
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

#include "DATAIO.h"
#include <cstdio>
#include <iostream>

int main(int argc, char **argv) {
  if (argc >= 4) {
    matrix *x;
    uivector *idsel;
    initMatrix(&x);
    DATAIO::ImportMatrix(argv[1], "\t", x);
    initUIVector(&idsel);

    size_t nthreads;
    if (argc == 4) {
      nthreads = atoi(argv[3]);
    } else {
      nthreads = atoi(argv[4]);
    }

    if (argc == 4) {
      MDC(x, 0, 0, idsel, nthreads);
    } else {
      MaxDis_Fast(x, atoi(argv[3]), 0, idsel, nthreads);
    }
    DATAIO::WriteUIvector(argv[2], idsel);
    DelMatrix(&x);
    DelUIVector(&idsel);
    return 0;
  } else {
    printf(
        "\n Usage: %s <Input Table File> <Out File> <N compounds> <nthreads>\n",
        argv[0]);
    return 0;
  }
}
