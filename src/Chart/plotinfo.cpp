#include <iostream>
#include "plotinfo.h"

void QPlotVersion(int *major, int *minor, int *patch)
{
  (*major) = MAJOR;
  (*minor) = MINOR;
  (*patch) = PATCH;
}
