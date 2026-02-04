/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for variableplot.
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

#ifndef VARIABLEPLOT_H
#define VARIABLEPLOT_H
#include "BarPlot.h"
#include "ScatterPlot.h"
#include "qsmdata.h"

#include "qstudiometricstypes.h"
#include <QList>
#include <QString>
#include <QStringList>

class VariablePlot {
public:
  explicit VariablePlot(PROJECTS *projects_);
  void setPlotType(int type_) { type = type_; } // MATRIXDATA, ARRATDATA
  void setPID(int pid_) { pid = pid_; }
  void setXHash(QStringList xhash_) { xhash = xhash_; }
  void setYHash(QStringList yhash_) { yhash = yhash_; }
  void setObjects(QList<QStringList> objects_) { objects = objects_; }
  void setObjLabels(QStringList selobjlabels_) { selobjlabels = selobjlabels_; }
  void setVarID1(int varid1_) { varid1 = varid1_; }
  void setVarID2(int varid2_) { varid2 = varid2_; }
  void setLayerID(int layerid_) { layerid = layerid_; }

  ScatterPlot *VariableVSVariable();
  BarPlot *VariableDistribution();

private:
  PROJECTS *projects;
  int type, pid, layerid, varid1, varid2;
  QStringList selobjlabels, xhash, yhash;
  QList<QStringList> objects;
};
#endif
