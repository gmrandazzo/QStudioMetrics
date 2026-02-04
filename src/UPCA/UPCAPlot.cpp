/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for upcaplot.
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

#include "UPCAPlot.h"

void UPCAPlot::ScorePlot2D(ScatterPlot2D **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPCAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getUPCAModel(mid)->Model()->scores);
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getUPCAModel(mid)->getObjName());

  /*QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPCAModel(mid)->getDataHash());
  */
  (*plot2D) = new ScatterPlot2D(
      mxlst, objnamelst, "PC", "PC",
      QString("%1 - %2 - UPCA Score Plot").arg(projectname).arg(modelname));
  (*plot2D)->setPID(pid);
}

void UPCAPlot::LoadingsPlot2D(QList<ScatterPlot2D *> *plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPCAModel(mid)->getName();

  /* xloadings->order represent the number of pc computed.
   * xloadings->m[i]->row represent the number of descriptor that is equal to
   * X_->m[i]->col xloadings->m[i]->col represent the number of order present in
   * the array X_ and is equal to X_->order
   */
  matrix *m;
  // the size is the number of variable x the number of components and we
  // generate a plot for each layer and the number of layer is  m[i]->col
  uint row =
      projects->value(pid)->getUPCAModel(mid)->Model()->loadings->m[0]->row;
  uint col = projects->value(pid)->getUPCAModel(mid)->Model()->loadings->order;
  uint layer =
      projects->value(pid)->getUPCAModel(mid)->Model()->loadings->m[0]->col;

  for (uint k = 0; k < layer; k++) {
    NewMatrix(&m, row, col);

    // copy the data on matrix to plot
    for (uint i = 0; i < row; i++) {
      for (uint j = 0; j < col; j++) {
        setMatrixValue(
            m, i, j,
            getArrayValue(
                projects->value(pid)->getUPCAModel(mid)->Model()->loadings, j,
                i, k));
      }
    }

    QList<matrix *> mxlst;
    mxlst.append(m);
    QList<QStringList> objnamelst;
    QStringList varname = projects->value(pid)->getUPCAModel(mid)->getVarName();
    objnamelst.append(varname);

    /*
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getUPCAModel(mid)->getDataHash());
    */
    (*plot2D).last() =
        new ScatterPlot2D(mxlst, objnamelst, "PC", "PC",
                          QString("%1 - %2 - UPCA Loadings Plot - Layer %3")
                              .arg(projectname)
                              .arg(modelname)
                              .arg(QString::number(k + 1)));
    (*plot2D).last()->setPID(pid);
    DelMatrix(&m);
  }
}

void UPCAPlot::ScorePlotPrediction2D(ScatterPlot2D **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPCAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getUPCAModel(mid)->Model()->scores);
  mxlst.append(projects->value(pid)
                   ->getUPCAModel(mid)
                   ->getUPCAPrediction(predid)
                   ->getPredScores());
  QList<QStringList> objnamelst;
  objnamelst.append(projects->value(pid)->getUPCAModel(mid)->getObjName());
  objnamelst.append(projects->value(pid)
                        ->getUPCAModel(mid)
                        ->getUPCAPrediction(predid)
                        ->getObjName());
  /*QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPCAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)->getUPCAModel(mid)->getUPCAPrediction(predid)->getDataHash());
  */
  (*plot2D) = new ScatterPlot2D(mxlst, objnamelst, "PC", "PC",
                                QString("%1 - %2 - UPCA Score Plot Prediction")
                                    .arg(projectname)
                                    .arg(modelname));
  (*plot2D)->setPID(pid);
}

void UPCAPlot::ScorePlot3D(ScatterPlot3D **plot3D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPCAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getUPCAModel(mid)->Model()->scores);
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getUPCAModel(mid)->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPCAModel(mid)->getDataHash());
  (*plot3D) = new ScatterPlot3D(
      mxlst, objname, &projects->value(pid)->getARRAYList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(),
      QString("%1 - %2 - UPCA Score Plot").arg(projectname).arg(modelname),
      "PC", "PC", "PC", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}

void UPCAPlot::LoadingsPlot3D(QList<ScatterPlot3D *> *plot3D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPCAModel(mid)->getName();

  QStringList varname = projects->value(pid)->getUPCAModel(mid)->getVarName();

  /* xloadings->order represent the number of pc computed.
   * xloadings->m[i]->row represent the number of descriptor that is equal to
   * X_->m[i]->col xloadings->m[i]->col represent the number of order present in
   * the array X_ and is equal to X_->order
   */

  matrix *m;

  // the size is the number of variable x the number of components and we
  // generate a plot for each layer and the number of layer is  m[i]->col
  uint row =
      projects->value(pid)->getUPCAModel(mid)->Model()->loadings->m[0]->row;
  uint col = projects->value(pid)->getUPCAModel(mid)->Model()->loadings->order;
  uint layer =
      projects->value(pid)->getUPCAModel(mid)->Model()->loadings->m[0]->col;

  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPCAModel(mid)->getDataHash());

  for (uint k = 0; k < layer; k++) {
    NewMatrix(&m, row, col);

    // copy the data on matrix to plot
    for (uint i = 0; i < row; i++) {
      for (uint j = 0; j < col; j++) {
        setMatrixValue(
            m, i, j,
            getArrayValue(
                projects->value(pid)->getUPCAModel(mid)->Model()->loadings, j,
                i, k));
      }
    }

    QList<matrix *> mxlst;
    mxlst.append(m);
    QList<QStringList> objname;
    objname.append(varname);
    (*plot3D).append(new ScatterPlot3D(
        mxlst, objname, &projects->value(pid)->getARRAYList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(),
        QString("%1 - %2 - UPCA Loadings Plot Layer: %3")
            .arg(projectname)
            .arg(modelname)
            .arg(QString::number(k + 1)),
        "PC", "PC", "PC", ScatterPlot3D::LOADINGS));
    /*plots.append(new ScatterPlot3D(mxlst, objname,
     * &projects->value(pid)->getVariableLabels(), QString(projectname + " - " +
     * modelname + " - UPLS Loadings Plot Layer: %1").arg(QString::number(k+1)),
     * ScatterPlot3D::VARIABLELABELS)); */
    (*plot3D).last()->setPID(pid);
    DelMatrix(&m);
  }
}

void UPCAPlot::ScorePlotPrediction3D(ScatterPlot3D **plot3D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getUPCAModel(mid)->getName();
  QList<matrix *> mxlst;
  mxlst.append(projects->value(pid)->getUPCAModel(mid)->Model()->scores);
  mxlst.append(projects->value(pid)
                   ->getUPCAModel(mid)
                   ->getUPCAPrediction(predid)
                   ->getPredScores());
  QList<QStringList> objname;
  objname.append(projects->value(pid)->getUPCAModel(mid)->getObjName());
  objname.append(projects->value(pid)
                     ->getUPCAModel(mid)
                     ->getUPCAPrediction(predid)
                     ->getObjName());
  QStringList xhash, yhash;
  xhash.append(projects->value(pid)->getUPCAModel(mid)->getDataHash());
  xhash.append(projects->value(pid)
                   ->getUPCAModel(mid)
                   ->getUPCAPrediction(predid)
                   ->getDataHash());
  (*plot3D) =
      new ScatterPlot3D(mxlst, objname, &projects->value(pid)->getARRAYList(),
                        xhash, yhash, &projects->value(pid)->getObjectLabels(),
                        &projects->value(pid)->getVariableLabels(),
                        QString("%1 - %2 - UPCA Score Plot Prediction")
                            .arg(projectname)
                            .arg(modelname),
                        "PC", "PC", "PC", ScatterPlot3D::SCORES);
  (*plot3D)->setPID(pid);
}

UPCAPlot::UPCAPlot(PROJECTS *projects_) {
  projects = projects_;
  pid = mid = predid = -1;
}
