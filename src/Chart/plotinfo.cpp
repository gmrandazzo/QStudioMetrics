#include "plotinfo.h"
#include <iostream>

void QPlotVersion(int *major, int *minor, int *patch) {
  (*major) = MAJOR;
  (*minor) = MINOR;
  (*patch) = PATCH;
}
