/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for icaplot.
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

#include "ICAPlot.h"
#include <memory>

ICAPlot::ICAPlot(PROJECTS *projects_) {
  projects = projects_;
  pid = mid = predid = -1;
}

void ICAPlot::ScorePlot2D(ScatterPlot **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getICAModel(mid)->getName();

  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getICAModel(mid)->Model()->S);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getICAModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getICAModel(mid)->getDataHash());
  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(),
      xhash, yhash, &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "IC", "IC",
      QString("%1 | %2 | ICA Independent Component Plot").arg(projectname).arg(modelname),
      ScatterPlot::SCORES);
  
  temp_plot->setHotellingConfidenceEllipse(false);
  temp_plot->setPID(pid);
  temp_plot->setImages(projects->value(pid)->getImages());
  *plot2D = temp_plot.release();
}

void ICAPlot::ScorePlotPrediction2D(ScatterPlot **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getICAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getICAModel(mid)->Model()->S);
  mxlst.append(projects->value(pid)
                   ->getICAModel(mid)
                   ->getICAPrediction(predid)
                   ->getPredScores());
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getICAModel(mid)->getObjName());
  objnamelst.append(projects->value(pid)
                        ->getICAModel(mid)
                        ->getICAPrediction(predid)
                        ->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getICAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)
                   ->getICAModel(mid)
                   ->getICAPrediction(predid)
                   ->getDataHash());
  auto temp_plot = std::make_unique<ScatterPlot>(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "IC", "IC",
      QString("%1 | %2 | ICA Independent Component Plot Prediction").arg(projectname).arg(modelname),
      ScatterPlot::SCORES);
  temp_plot->setHotellingConfidenceEllipse(false);
  temp_plot->setPID(pid);
  temp_plot->setImages(projects->value(pid)->getImages());
  *plot2D = temp_plot.release();
}
