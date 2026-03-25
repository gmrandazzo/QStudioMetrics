/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for plsplot.
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

#include "PLSPlot.h"
#include "scientific.h"

#ifdef DEBUG
#include <QDebug>
#endif

void PLSPlot::TU_Plot(ScatterPlot **plot2D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mx = {m->Model()->xscores};
  QList<matrix *> my = {m->Model()->yscores};
  QList<QStringList> objname = {m->getObjName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot2D) = new ScatterPlot(
      mx, my, m->Model()->b, objname, &projects->value(pid)->getMATRIXList(),
      xhash, yhash, &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "t", "u",
      QString("%1 | %2 | PLS Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::SCORES);
  (*plot2D)->setPID(pid);
  (*plot2D)->setImages(projects->value(pid)->getImages());
  (*plot2D)->resize(500, 300);
}

void PLSPlot::T_ScorePlot2D(ScatterPlot **plot2D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->xscores};
  QList<QStringList> objnamelst = {m->getObjName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot2D) = new ScatterPlot(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "t", "t",
      QString("%1 | %2 | PLS T/T Score Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::SCORES);
  (*plot2D)->setPID(pid);
  (*plot2D)->setImages(projects->value(pid)->getImages());
}

void PLSPlot::P_LoadingsPlot2D(ScatterPlot **plot2D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->xloadings};
  QList<QStringList> objnamelst = {m->getXVarName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot2D) = new ScatterPlot(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "p", "p",
      QString("%1 | %2 | PLS P Loadings Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
}

void PLSPlot::WeightsPlot2D(ScatterPlot **plot2D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  matrix *yloadingsbis;
  NewMatrix(&yloadingsbis, m->Model()->yloadings->row, m->Model()->yloadings->col);

  if (m->Model()->yloadings->row > 1) {
    for (uint j = 0; j < m->Model()->xweights->col; j++) {
      double xmin, xmax, ymin, ymax;
      MatrixColumnMinMax(m->Model()->xweights, j, &xmin, &xmax);
      MatrixColumnMinMax(m->Model()->yloadings, j, &ymin, &ymax);
      xmax += xmax * 0.1;
      xmin += xmin * 0.1;
      for (uint i = 0; i < m->Model()->yloadings->row; i++) {
        yloadingsbis->data[i][j] = (((xmax - xmin) * (m->Model()->yloadings->data[i][j] - ymin)) / (ymax - ymin)) + xmin;
      }
    }
  } else {
    for (uint j = 0; j < m->Model()->xweights->col; j++) {
      double xmin, xmax;
      MatrixColumnMinMax(m->Model()->xweights, j, &xmin, &xmax);
      xmax += xmax * 0.1;
      for (uint i = 0; i < m->Model()->yloadings->row; i++) {
        yloadingsbis->data[i][j] = xmax * m->Model()->yloadings->data[i][j];
      }
    }
  }

  QList<matrix *> mxlst = {m->Model()->xweights, yloadingsbis};
  QList<QStringList> objnamelst = {m->getXVarName(), m->getYVarName()};
  QStringList xhash = {m->getDataHash(), m->getDataHash()};
  QStringList yhash = {m->getDataHash(), m->getDataHash()};

  (*plot2D) = new ScatterPlot(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "w", "w",
      QString("%1 | %2 | PLS W Weights Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
  DelMatrix(&yloadingsbis);
}

void PLSPlot::U_ScorePlot2D(ScatterPlot **plot2D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->yscores};
  QList<QStringList> objnamelst = {m->getObjName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot2D) = new ScatterPlot(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "u", "u",
      QString("%1 | %2 | PLS U/U Score Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::SCORES);
  (*plot2D)->setPID(pid);
  (*plot2D)->setImages(projects->value(pid)->getImages());
}

void PLSPlot::Q_LoadingsPlot2D(ScatterPlot **plot2D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->yloadings};
  QList<QStringList> objnamelst = {m->getYVarName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot2D) = new ScatterPlot(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "q", "q",
      QString("%1 | %2 | PLS Q Loadings Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
}

void PLSPlot::PQ_LoadingsPlot2D(ScatterPlot **plot2D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->xloadings, m->Model()->yloadings};
  QList<QStringList> objnamelst = {m->getXVarName(), m->getYVarName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot2D) =
      new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(),
                      xhash, yhash, &projects->value(pid)->getObjectLabels(),
                      &projects->value(pid)->getVariableLabels(), "p/q", "p/q",
                      QString("%1 | %2 | PLS P/Q Loadings Plot")
                          .arg(getProjectName()).arg(getModelName()),
                      ScatterPlot::LOADINGS);
  (*plot2D)->setPID(pid);
  (*plot2D)->setMID(mid);
  (*plot2D)->setModelType(PLS_);
}

void PLSPlot::T_ScorePlotPrediction2D(ScatterPlot **plot2D) {
  PLSModel *m = getPLSModel();
  if (!m || predid == -1) return;

  QList<matrix *> mxlst = {m->Model()->xscores, m->getPLSPrediction(predid)->getXPredScores()};
  QList<QStringList> objnamelst = {m->getObjName(), m->getPLSPrediction(predid)->getObjName()};
  QStringList xhash = {m->getDataHash(), m->getPLSPrediction(predid)->getDataHash()};
  QStringList yhash = {m->getDataHash(), m->getDataHash()};

  (*plot2D) = new ScatterPlot(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "t", "t",
      QString("%1 | %2 | PLS Score Plot Prediction").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::SCORES);
  (*plot2D)->setPID(pid);
  (*plot2D)->setImages(projects->value(pid)->getImages());
}

void PLSPlot::RecalcVSExperimentalAndPrediction(ScatterPlot **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if (nlv > projects->value(pid)->getPLSModel(mid)->getNPC()) {
    nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(
      projects->value(pid)->getPLSModel(mid)->getDataHash());

  if (did > -1 && predid > -1) {
    int pred_id =
        projects->value(pid)->getMatrixID(projects->value(pid)
                                              ->getPLSModel(mid)
                                              ->getPLSPrediction(predid)
                                              ->getDataHash());
    if (pred_id > -1) {
      QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel =
          projects->value(pid)->getPLSModel(mid)->getYVarName();
      uint nobjects = objsel.size();
      uint nvars = yvarsel.size();
      matrix *recalc_y;
      NewMatrix(&recalc_y, nobjects, nvars);

      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for (uint i = 0; i < nobjects; i++) {
        uint ny = nvars;
        for (uint j = 0; j < ny; j++) {
          setMatrixValue(recalc_y, i, j,
                         getMatrixValue(projects->value(pid)
                                            ->getPLSModel(mid)
                                            ->Model()
                                            ->recalculated_y,
                                        i, j + (ny * nlv) - ny));
        }
      }

      /*bug... not did ma HASH!!*/
      matrix *y;
      NewMatrix(&y, nobjects, nvars);
      int ii = 0;
      for (int i = 0;
           i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++) {
        if (objsel.contains(
                projects->value(pid)->getMatrix(did)->getObjName()[i]) ==
            true) {
          int jy = 0;
          for (int j = 1;
               j < projects->value(pid)->getMatrix(did)->getVarName().size();
               j++) {
            if (yvarsel.contains(
                    projects->value(pid)->getMatrix(did)->getVarName()[j]) ==
                true) {
              setMatrixValue(
                  y, ii, jy,
                  getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(),
                                 i, j - 1));
              jy++;
            } else {
              continue;
            }
          }
          ii++;
        } else {
          continue;
        }
      }

      // Get predictions to plot...
      QStringList pred_objsel = projects->value(pid)
                                    ->getPLSModel(mid)
                                    ->getPLSPrediction(predid)
                                    ->getObjName();
      uint pred_nobjects = pred_objsel.size();
      matrix *pred_y;
      NewMatrix(&pred_y, pred_nobjects, nvars);

      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for (uint i = 0; i < pred_nobjects; i++) {
        uint ny = nvars;
        for (uint j = 0; j < ny; j++) {
          setMatrixValue(pred_y, i, j,
                         getMatrixValue(projects->value(pid)
                                            ->getPLSModel(mid)
                                            ->getPLSPrediction(predid)
                                            ->getYDipVar(),
                                        i, j + (ny * nlv) - ny));
        }
      }

      /*bug... not did ma HASH!!*/
      matrix *real_pred_y;
      NewMatrix(&real_pred_y, pred_nobjects, nvars);
      ii = 0;
      for (int i = 0;
           i < projects->value(pid)->getMatrix(pred_id)->getObjName().size();
           i++) {
        if (pred_objsel.contains(
                projects->value(pid)->getMatrix(pred_id)->getObjName()[i]) ==
            true) {
          int jy = 0;
          for (int j = 1;
               j <
               projects->value(pid)->getMatrix(pred_id)->getVarName().size();
               j++) {
            if (yvarsel.contains(projects->value(pid)
                                     ->getMatrix(pred_id)
                                     ->getVarName()[j]) == true) {
              setMatrixValue(
                  real_pred_y, ii, jy,
                  getMatrixValue(
                      projects->value(pid)->getMatrix(pred_id)->Matrix(), i,
                      j - 1));
              jy++;
            } else {
              continue;
            }
          }
          ii++;
        } else {
          continue;
        }
      }

      QList<matrix *> mx, my;
      mx.append(y);
      mx.append(real_pred_y);
      my.append(recalc_y);
      my.append(pred_y);
      QList<QStringList> objname;
      objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
      objname.append(projects->value(pid)
                         ->getPLSModel(mid)
                         ->getPLSPrediction(predid)
                         ->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      xhash.append(projects->value(pid)
                       ->getPLSModel(mid)
                       ->getPLSPrediction(predid)
                       ->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)
                       ->getPLSModel(mid)
                       ->getPLSPrediction(predid)
                       ->getDataHash());
      (*plot2D) = new ScatterPlot(
          mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels(), QString("Experimental"),
          QString("Recalc. and Predicted"),
          QString("%1 | %2 | %3")
              .arg(projectname)
              .arg(modelname)
              .arg(QString("PLS Recalc VS Experimental Plot (LV: %1)").arg(nlv)),
          ScatterPlot::SCORES);
      DelMatrix(&recalc_y);
      (*plot2D)->setImages(projects->value(pid)->getImages());
      (*plot2D)->BuildDiagonal();
      (*plot2D)->setAxisNameExtensions(varname);
      (*plot2D)->setPID(pid);
      DelMatrix(&y);
      DelMatrix(&real_pred_y);
      DelMatrix(&pred_y);
    }
  }
}

void PLSPlot::BetaCoefficients(BarPlot **betas_bar) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varnames = projects->value(pid)->getPLSModel(mid)->getXVarName();
  if (nlv > projects->value(pid)->getPLSModel(mid)->getNPC()) {
    nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
  }

  dvector *betas;
  initDVector(&betas);
  PLSBetasCoeff(projects->value(pid)->getPLSModel(mid)->Model(), nlv, betas);

  // compute the intercept
  // meanY - meanX*beta
  PrintDVector(betas);

  (*betas_bar) =
      new BarPlot(betas, varnames,
                  QString("Betas Model - %1 LV %2")
                      .arg(projects->value(pid)->getPLSModel(mid)->getName())
                      .arg(QString::number(nlv)));

  DelDVector(&betas);
}

void PLSPlot::BetaCoefficientsDurbinWatson(SimpleLine2DPlot **dw_betas_plot) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  uint model_nlv = projects->value(pid)->getPLSModel(mid)->getNPC() +
             1; // +1 because we start from 0

  dvector *dw;
  NewDVector(&dw, model_nlv - 1);

  PLSMODEL *plsmod = projects->value(pid)->getPLSModel(mid)->Model();
  PrintMatrix(projects->value(pid)->getPLSModel(mid)->Model()->xweights);
  for (uint i = 1; i < model_nlv; i++) {
    dvector *betas;
    initDVector(&betas);

    PLSBetasCoeff(plsmod, i, betas);
    // PrintDVector(betas);

    double n = 0.f, d = 0.f;
    d += square(betas->data[0]);
    for (uint k = 1; k < betas->size; k++) {
      n += square(betas->data[k] - betas->data[k - 1]);
      d += square(betas->data[k]);
    }
    dw->data[i - 1] = n / d;

    DelDVector(&betas);
  }

  // PrintMatrix(dw);
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, model_nlv, 2);
  curvenames << "DW";

  QString yname;
  for (int j = 0;
       j < projects->value(pid)->getPLSModel(mid)->getYVarName().size(); j++) {
    yname += " - " + projects->value(pid)->getPLSModel(mid)->getYVarName()[j];
  }

  m->data[0][0] = 0.f;
  m->data[0][1] = 0.f;
  for (uint i = 0; i < dw->size; i++) {
    m->data[i + 1][0] = i + 1;
    m->data[i + 1][1] = dw->data[i];
  }

  (*dw_betas_plot) = new SimpleLine2DPlot(m, curvenames,
                                          QString(" %1 - %2 - DW Plot %3")
                                              .arg(projectname)
                                              .arg(modelname)
                                              .arg(yname),
                                          "Latent Variables", "DW");

  DelMatrix(&m);
  DelDVector(&dw);
}

void PLSPlot::PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D) {
  QString projectname = projects->value(pid)->getProjectName();
  QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
  QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

  if (nlv > projects->value(pid)->getPLSModel(mid)->getNPC()) {
    nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
  }

  // Get Prediction
  int did = projects->value(pid)->getMatrixID(
      projects->value(pid)->getPLSModel(mid)->getDataHash());
  if (did > -1 && predid > -1) {
    int pred_id = projects->value(pid)->getMatrixID(
        projects->value(pid)
            ->getPLSModel(mid)
            ->getPLSPrediction(predid)
            ->getDataHash()); // used to get real y when find real y
    if (pred_id > -1) {
      QStringList model_objsel =
          projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel =
          projects->value(pid)->getPLSModel(mid)->getYVarName();
      uint nobjects = model_objsel.size();
      uint nvars = yvarsel.size();
      matrix *model_predicted_y;
      NewMatrix(&model_predicted_y, nobjects, nvars);
      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for (uint i = 0; i < nobjects; i++) {
        uint ny = nvars;
        for (uint j = 0; j < ny; j++) {
          setMatrixValue(
              model_predicted_y, i, j,
              getMatrixValue(
                  projects->value(pid)->getPLSModel(mid)->Model()->predicted_y,
                  i, j + (ny * nlv) - ny));
        }
      }

      matrix *model_exp_y;
      NewMatrix(&model_exp_y, nobjects, nvars);
      int ii = 0;
      for (int i = 0;
           i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++) {
        if (model_objsel.contains(
                projects->value(pid)->getMatrix(did)->getObjName()[i]) ==
            true) {
          int jy = 0;
          for (int j = 1;
               j < projects->value(pid)->getMatrix(did)->getVarName().size();
               j++) {
            if (yvarsel.contains(
                    projects->value(pid)->getMatrix(did)->getVarName()[j]) ==
                true) {
              setMatrixValue(
                  model_exp_y, ii, jy,
                  getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(),
                                 i, j - 1));
              jy++;
            } else {
              continue;
            }
          }
          ii++;
        } else {
          continue;
        }
      }

      // Get predictions to plot...
      QStringList pred_objsel = projects->value(pid)
                                    ->getPLSModel(mid)
                                    ->getPLSPrediction(predid)
                                    ->getObjName();
      uint pred_nobjects = pred_objsel.size();
      matrix *pred_predicted_y;
      NewMatrix(&pred_predicted_y, pred_nobjects, nvars);

      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for (uint i = 0; i < pred_nobjects; i++) {
        uint ny = nvars;
        for (uint j = 0; j < ny; j++) {
          setMatrixValue(pred_predicted_y, i, j,
                         getMatrixValue(projects->value(pid)
                                            ->getPLSModel(mid)
                                            ->getPLSPrediction(predid)
                                            ->getYDipVar(),
                                        i, j + (ny * nlv) - ny));
        }
      }

      /*bug... not did ma HASH!!*/
      matrix *pred_exp_y;
      NewMatrix(&pred_exp_y, pred_nobjects, nvars);
      ii = 0;
      for (int i = 0;
           i < projects->value(pid)->getMatrix(pred_id)->getObjName().size();
           i++) {
        if (pred_objsel.contains(
                projects->value(pid)->getMatrix(pred_id)->getObjName()[i]) ==
            true) {
          int jy = 0;
          for (int j = 1;
               j <
               projects->value(pid)->getMatrix(pred_id)->getVarName().size();
               j++) {
            if (yvarsel.contains(projects->value(pid)
                                     ->getMatrix(pred_id)
                                     ->getVarName()[j]) == true) {
              setMatrixValue(
                  pred_exp_y, ii, jy,
                  getMatrixValue(
                      projects->value(pid)->getMatrix(pred_id)->Matrix(), i,
                      j - 1));
              jy++;
            } else {
              continue;
            }
          }
          ii++;
        } else {
          continue;
        }
      }

      QList<matrix *> mx, my;
      mx.append(model_exp_y);
      mx.append(pred_exp_y);
      my.append(model_predicted_y);
      my.append(pred_predicted_y);
      QList<QStringList> objname;
      objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
      objname.append(projects->value(pid)
                         ->getPLSModel(mid)
                         ->getPLSPrediction(predid)
                         ->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      xhash.append(projects->value(pid)
                       ->getPLSModel(mid)
                       ->getPLSPrediction(predid)
                       ->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)
                       ->getPLSModel(mid)
                       ->getPLSPrediction(predid)
                       ->getDataHash());
      (*plot2D) = new ScatterPlot(
          mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels(), QString("Experimental"),
          QString("Predicted"),
          QString("%1 | %2 | %3")
              .arg(projectname)
              .arg(modelname)
              .arg(QString("PLS Recalc VS Experimental Plot (LV: %1)").arg(nlv)),
          ScatterPlot::SCORES);
      (*plot2D)->setImages(projects->value(pid)->getImages());
      (*plot2D)->BuildDiagonal();
      (*plot2D)->setAxisNameExtensions(varname);
      (*plot2D)->setPID(pid);
      DelMatrix(&model_exp_y);
      DelMatrix(&model_predicted_y);
      DelMatrix(&pred_exp_y);
      DelMatrix(&pred_predicted_y);
    }
  }
}

void PLSPlot::RecalcVSExperimental(ScatterPlot **plot2D) {
  if (projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_) {
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

    if (nlv > projects->value(pid)->getPLSModel(mid)->getNPC()) {
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    // Get Prediction
    int did = projects->value(pid)->getMatrixID(
        projects->value(pid)->getPLSModel(mid)->getDataHash());

    QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
    QStringList yvarsel = projects->value(pid)->getPLSModel(mid)->getYVarName();
    uint nobjects = objsel.size();
    uint nvars = yvarsel.size();
    matrix *model_recalc_y;
    NewMatrix(&model_recalc_y, nobjects, nvars);

    /*
     Prediction for example are organized as this example
    y = 2
    number_of_pc = 4

        pc1    pc2     pc3       pc4
       |  |   |   |   |   |    |    |
       y1 y2  y1  y2  y1  y2   y1   y2
   pos 0 1    2   3   4   5    6    7
    */

    for (uint i = 0; i < nobjects; i++) {
      uint ny = nvars;
      for (uint j = 0; j < ny; j++) {
        setMatrixValue(
            model_recalc_y, i, j,
            getMatrixValue(
                projects->value(pid)->getPLSModel(mid)->Model()->recalculated_y,
                i, j + (ny * nlv) - ny));
      }
    }

    /*bug... not did ma HASH!!*/
    matrix *model_exp_y;
    NewMatrix(&model_exp_y, nobjects, nvars);
    int ii = 0;
    for (int i = 0;
         i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++) {
      if (objsel.contains(
              projects->value(pid)->getMatrix(did)->getObjName()[i]) == true) {
        int jy = 0;
        for (int j = 1;
             j < projects->value(pid)->getMatrix(did)->getVarName().size();
             j++) {
          if (yvarsel.contains(
                  projects->value(pid)->getMatrix(did)->getVarName()[j]) ==
              true) {
            setMatrixValue(
                model_exp_y, ii, jy,
                getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(),
                               i, j - 1));
            jy++;
          } else {
            continue;
          }
        }
        ii++;
      } else {
        continue;
      }
    }

    QList<matrix *> mx, my;
    mx.append(model_exp_y);
    my.append(model_recalc_y);
    QList<QStringList> objname;
    objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
    yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(
        mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), QString("Experimental"),
        QString("Recalculated"),
        QString("%1 | %2 | %3")
            .arg(projectname)
            .arg(modelname)
            .arg(QString("PLS Recalculated VS Experimental Plot (LV: %1)")
                     .arg(nlv)),
        ScatterPlot::SCORES);
    (*plot2D)->setImages(projects->value(pid)->getImages());
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setAxisNameExtensions(varname);
    (*plot2D)->setPID(pid);
    DelMatrix(&model_recalc_y);
    DelMatrix(&model_exp_y);
  }
}

void PLSPlot::ClassRecalcVSExperimental(QList<QStringList> *cellnames,
                                        QList<QList<QPixmap>> *images,
                                        QList<QList<QColor>> *colors) {
  PLSModel *m = getPLSModel();
  if (!m || !isPLSDA()) return;

  if (nlv > getNPC()) {
    nlv = getNPC();
  }

  QStringList objsel = m->getObjName();
  LABELS classes = m->getClasses();

  uint nclass = classes.size();
  if (nclass == 2) {
    nclass = 1;
  }

  matrix *mx;

  for (size_t j = 0; j < nclass; j++) {
    QStringList row_name;
    QList<QPixmap> row_pixmap;
    QList<QColor> row_colors;

    NewMatrix(&mx, m->Model()->recalculated_y->row, 3);
    for (size_t i = 0; i < m->Model()->recalculated_y->row; i++) {
      mx->data[i][0] = m->Model()->recalculated_y->data[i][j + (nclass * nlv) - nclass];
      if (classes[j].objects.contains(objsel[i]) == true) {
        // True
        mx->data[i][1] = 1.f;
      } else {
        // False
        mx->data[i][1] = 0.f;
      }
      mx->data[i][2] = i; // objec row id
    }
    // Sort by the 1 probabilities to the 0 problabilities
    MatrixReverseSort(mx, 0);
    for (int i = 0; i < (int)mx->row; i++) {
      int objid = (int)mx->data[i][2];
      QString name = objsel[objid] + "\n";
      if ((int)mx->data[i][1] == 1) {
        name += QString("Is %1\n").arg(classes[j].name);
        row_colors << Qt::green;
      } else {
        name += QString("Not %1\n").arg(classes[j].name);
        row_colors << Qt::red;
      }
      name += QString("Score: %1").arg(QString::number(mx->data[i][0], 'g', 3));
      row_name << name;

      int pixid = -1;
      for (int k = 0; k < projects->value(pid)->getImages().size(); k++) {
        if (objsel[objid].compare(projects->value(pid)->getImages()[k].name) == 0) {
          pixid = k;
          break;
        } else {
          continue;
        }
      }

      if (pixid > -1)
        row_pixmap << projects->value(pid)->getImages()[pixid].image;
      else
        row_pixmap << QPixmap();
    }
    DelMatrix(&mx);
    (*cellnames) << row_name;
    (*images) << row_pixmap;
    (*colors) << row_colors;
  }
}

void PLSPlot::RecalcResidualsVSExperimental(ScatterPlot **plot2D) {
  if (projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_) {
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

    if (nlv > projects->value(pid)->getPLSModel(mid)->getNPC()) {
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    // Get Prediction
    int did = projects->value(pid)->getMatrixID(
        projects->value(pid)->getPLSModel(mid)->getDataHash());

    if (did > -1) {
      QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel =
          projects->value(pid)->getPLSModel(mid)->getYVarName();
      uint nobjects = objsel.size();
      uint nvars = yvarsel.size();
      matrix *recalc_res;
      NewMatrix(&recalc_res, nobjects, nvars);

      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for (uint i = 0; i < nobjects; i++) {
        uint ny = nvars;
        for (uint j = 0; j < ny; j++) {
          setMatrixValue(recalc_res, i, j,
                         getMatrixValue(projects->value(pid)
                                            ->getPLSModel(mid)
                                            ->Model()
                                            ->recalc_residuals,
                                        i, j + (ny * nlv) - ny));
        }
      }

      /*bug... not did ma HASH!!*/
      matrix *model_exp_y;
      NewMatrix(&model_exp_y, nobjects, nvars);
      int ii = 0;
      for (int i = 0;
           i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++) {
        if (objsel.contains(
                projects->value(pid)->getMatrix(did)->getObjName()[i]) ==
            true) {
          int jy = 0;
          for (int j = 1;
               j < projects->value(pid)->getMatrix(did)->getVarName().size();
               j++) {
            if (yvarsel.contains(
                    projects->value(pid)->getMatrix(did)->getVarName()[j]) ==
                true) {
              setMatrixValue(
                  model_exp_y, ii, jy,
                  getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(),
                                 i, j - 1));
              jy++;
            } else {
              continue;
            }
          }
          ii++;
        } else {
          continue;
        }
      }

      QList<matrix *> mx, my;
      mx.append(model_exp_y);
      my.append(recalc_res);
      QList<QStringList> objname;
      objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      (*plot2D) = new ScatterPlot(
          mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels(), QString("Experimental"),
          QString("Recalculated Residuals"),
          QString("%1 | %2 | %3")
              .arg(projectname)
              .arg(modelname)
              .arg(QString("PLS Experimental VS Recalculated Residuals Y Plot "
                           "(LV: %1)")
                       .arg(nlv)),
          ScatterPlot::SCORES);
      (*plot2D)->setImages(projects->value(pid)->getImages());
      DelMatrix(&recalc_res);
      (*plot2D)->setPID(pid);
      (*plot2D)->setAxisNameExtensions(varname);
      DelMatrix(&model_exp_y);
    }
  }
}

void PLSPlot::ClassPredictedVSExperimental(QList<QStringList> *cellnames,
                                           QList<QList<QPixmap>> *images,
                                           QList<QList<QColor>> *colors) {
  PLSModel *m = getPLSModel();
  if (!m || !isPLSDA()) return;

  if (nlv > getNPC()) {
    nlv = getNPC();
  }

  QStringList objsel = m->getObjName();
  LABELS classes = m->getClasses();

  uint nclass = classes.size();
  if (nclass == 2) {
    nclass = 1;
  }

  matrix *mx;

  for (size_t j = 0; j < nclass; j++) {
    QStringList row_name;
    QList<QPixmap> row_pixmap;
    QList<QColor> row_colors;

    NewMatrix(&mx, m->Model()->predicted_y->row, 3);
    for (size_t i = 0; i < m->Model()->predicted_y->row; i++) {
      mx->data[i][0] = m->Model()->predicted_y->data[i][j + (nclass * nlv) - nclass];
      if (classes[j].objects.contains(objsel[i]) == true) {
        // True
        mx->data[i][1] = 1.f;
      } else {
        // False
        mx->data[i][1] = 0.f;
      }
      mx->data[i][2] = i; // objec row id
    }
    // Sort by the 1 probabilities to the 0 problabilities
    MatrixReverseSort(mx, 0);
    for (int i = 0; i < (int)mx->row; i++) {
      int objid = (int)mx->data[i][2];
      QString name = objsel[objid] + "\n";
      if ((int)mx->data[i][1] == 1) {
        name += QString("Is %1\n").arg(classes[j].name);
        row_colors << Qt::green;
      } else {
        name += QString("Not %1\n").arg(classes[j].name);
        row_colors << Qt::red;
      }
      name += QString("Score: %1").arg(QString::number(mx->data[i][0], 'g', 3));
      row_name << name;

      int pixid = -1;
      for (int k = 0; k < projects->value(pid)->getImages().size(); k++) {
        if (objsel[objid].compare(projects->value(pid)->getImages()[k].name) == 0) {
          pixid = k;
          break;
        } else {
          continue;
        }
      }

      if (pixid > -1)
        row_pixmap << projects->value(pid)->getImages()[pixid].image;
      else
        row_pixmap << QPixmap();
    }
    DelMatrix(&mx);
    (*cellnames) << row_name;
    (*images) << row_pixmap;
    (*colors) << row_colors;
  }
}

void PLSPlot::PredictedVSExperimental(ScatterPlot **plot2D) {
  if (projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_) {
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();
    QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

    if (nlv > projects->value(pid)->getPLSModel(mid)->getNPC()) {
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    // Get Prediction
    int did = projects->value(pid)->getMatrixID(
        projects->value(pid)->getPLSModel(mid)->getDataHash());

    if (did > -1) {
      QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel =
          projects->value(pid)->getPLSModel(mid)->getYVarName();
      uint nobjects = objsel.size();
      uint nvars = yvarsel.size();
      matrix *model_pred_y;
      NewMatrix(&model_pred_y, nobjects, nvars);

      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for (uint i = 0; i < nobjects; i++) {
        uint ny = nvars;
        for (uint j = 0; j < ny; j++) {
          setMatrixValue(
              model_pred_y, i, j,
              getMatrixValue(
                  projects->value(pid)->getPLSModel(mid)->Model()->predicted_y,
                  i, j + (ny * nlv) - ny));
        }
      }

      matrix *model_exp_y;
      NewMatrix(&model_exp_y, nobjects, nvars);
      int ii = 0;
      for (int i = 0;
           i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++) {
        if (objsel.contains(
                projects->value(pid)->getMatrix(did)->getObjName()[i]) ==
            true) {
          int jy = 0;
          for (int j = 1;
               j < projects->value(pid)->getMatrix(did)->getVarName().size();
               j++) {
            if (yvarsel.contains(
                    projects->value(pid)->getMatrix(did)->getVarName()[j]) ==
                true) {
              setMatrixValue(
                  model_exp_y, ii, jy,
                  getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(),
                                 i, j - 1));
              jy++;
            } else {
              continue;
            }
          }
          ii++;
        } else {
          continue;
        }
      }

      QList<matrix *> mx, my;
      mx.append(model_exp_y);
      my.append(model_pred_y);
      QList<QStringList> objname;
      objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      (*plot2D) = new ScatterPlot(
          mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels(), QString("Experimental"),
          QString("Predicted"),
          QString("%1 | %2 | %3")
              .arg(projectname)
              .arg(modelname)
              .arg(QString("PLS Predicted VS Experimental Plot (LV: %1)")
                       .arg(nlv)),
          ScatterPlot::SCORES);
      (*plot2D)->setImages(projects->value(pid)->getImages());
      (*plot2D)->setAxisNameExtensions(varname);
      (*plot2D)->BuildDiagonal();
      (*plot2D)->setPID(pid);
      DelMatrix(&model_exp_y);
      DelMatrix(&model_pred_y);
    }
  }
}

void PLSPlot::PredictedResidualsVSExperimental(ScatterPlot **plot2D) {
  if (projects->value(pid)->getPLSModel(mid)->getAlgorithm() == PLS_) {
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getPLSModel(mid)->getName();

    QStringList varname = projects->value(pid)->getPLSModel(mid)->getYVarName();

    if (nlv > projects->value(pid)->getPLSModel(mid)->getNPC()) {
      nlv = projects->value(pid)->getPLSModel(mid)->getNPC();
    }

    // Get Prediction
    int did = projects->value(pid)->getMatrixID(
        projects->value(pid)->getPLSModel(mid)->getDataHash());

    if (did > -1) {
      QStringList objsel = projects->value(pid)->getPLSModel(mid)->getObjName();
      QStringList yvarsel =
          projects->value(pid)->getPLSModel(mid)->getYVarName();
      uint nobjects = objsel.size();
      uint nvars = yvarsel.size();
      matrix *pred_res;
      NewMatrix(&pred_res, nobjects, nvars);

      /*
      Prediction for example are organized as this example
      y = 2
      number_of_pc = 4

          pc1    pc2     pc3       pc4
        |  |   |   |   |   |    |    |
        y1 y2  y1  y2  y1  y2   y1   y2
    pos 0 1    2   3   4   5    6    7
      */

      for (uint i = 0; i < nobjects; i++) {
        uint ny = nvars;
        for (uint j = 0; j < ny; j++) {
          setMatrixValue(pred_res, i, j,
                         getMatrixValue(projects->value(pid)
                                            ->getPLSModel(mid)
                                            ->Model()
                                            ->pred_residuals,
                                        i, j + (ny * nlv) - ny));
        }
      }

      /*bug... not did ma HASH!!*/
      matrix *model_exp_y;
      NewMatrix(&model_exp_y, nobjects, nvars);
      int ii = 0;
      for (int i = 0;
           i < projects->value(pid)->getMatrix(did)->getObjName().size(); i++) {
        if (objsel.contains(
                projects->value(pid)->getMatrix(did)->getObjName()[i]) ==
            true) {
          int jy = 0;
          for (int j = 1;
               j < projects->value(pid)->getMatrix(did)->getVarName().size();
               j++) {
            if (yvarsel.contains(
                    projects->value(pid)->getMatrix(did)->getVarName()[j]) ==
                true) {
              setMatrixValue(
                  model_exp_y, ii, jy,
                  getMatrixValue(projects->value(pid)->getMatrix(did)->Matrix(),
                                 i, j - 1));
              jy++;
            } else {
              continue;
            }
          }
          ii++;
        } else {
          continue;
        }
      }

      QList<matrix *> mx, my;
      mx.append(model_exp_y);
      my.append(pred_res);
      QList<QStringList> objname;
      objname.append(projects->value(pid)->getPLSModel(mid)->getObjName());
      QStringList xhash, yhash;
      xhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      yhash.append(projects->value(pid)->getPLSModel(mid)->getDataHash());
      (*plot2D) = new ScatterPlot(
          mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
          &projects->value(pid)->getObjectLabels(),
          &projects->value(pid)->getVariableLabels(), QString("Experimental"),
          QString("Predicted Residuals"),
          QString("%1 | %2 | %3")
              .arg(projectname)
              .arg(modelname)
              .arg(QString("PLS Experimental VS Predicted Residuals Y Plot (LV: "
                           "%1)")
                       .arg(nlv)),
          ScatterPlot::SCORES);
      (*plot2D)->setImages(projects->value(pid)->getImages());
      DelMatrix(&pred_res);
      (*plot2D)->setPID(pid);
      (*plot2D)->setAxisNameExtensions(varname);
      DelMatrix(&model_exp_y);
    }
  }
}

QList<SimpleLine2DPlot *> PLSPlot::R2Q2() {
  QList<SimpleLine2DPlot *> plots;
  PLSModel *m_obj = getPLSModel();
  if (!m_obj || !isPLS()) return plots;

  uint model_nlv = getNPC() + 1; // +1 because we start from 0
  uint yval = m_obj->Model()->r2y_recalculated->col;
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, model_nlv, 3);
  curvenames << "R2" << "Q2";

  // set the X assis that is the principal component
  for (uint i = 0; i < model_nlv; i++) {
    setMatrixValue(m, i, 0, i);
  }

  // Set the r2 values into the matrix and set the q2 values into the matrix
  uint l = 0;
  for (uint j = 0; j < yval; j++) {
    QString yname = m_obj->getYVarName()[j];

    setMatrixValue(m, 0, 1, 0); // R^2 in 0 pc is 0
    setMatrixValue(m, 0, 2, 0); // Q^2 in 0 pc is 0

    for (uint i = 0; i < model_nlv - 1; i++) {
      setMatrixValue(m, i + 1, 1,
                     getMatrixValue(m_obj->Model()->r2y_recalculated, i, l));
      double q2 = getMatrixValue(m_obj->Model()->q2y, i, l);
      if (q2 < 0) {
        setMatrixValue(m, i + 1, 2, 0.f);
      } else {
        setMatrixValue(m, i + 1, 2, q2);
      }
    }
    l++;

    plots.append(new SimpleLine2DPlot(m, curvenames,
                                      QString("%1 | %2 | R2 Q2 Plot %3")
                                          .arg(getProjectName())
                                          .arg(getModelName())
                                          .arg(yname),
                                      "Latent Variables", "R2 / Q2"));
    plots.last()->setXminXmaxXTick(0, model_nlv, model_nlv);
    plots.last()->setYminYmaxYTick(0, 1, 10);
  }
  DelMatrix(&m);
  return plots;
}

QList<SimpleLine2DPlot *> PLSPlot::RMSE() {
  QList<SimpleLine2DPlot *> plots;
  PLSModel *m_obj = getPLSModel();
  if (!m_obj || !isPLS()) return plots;

  uint model_nlv = getNPC();
  uint yval = m_obj->Model()->r2y_recalculated->col;
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, model_nlv, 3);
  curvenames << "RMSE(Training)" << "RMSE(Validation)";

  // set the X assis that is the principal component
  for (uint i = 0; i < model_nlv; i++) {
    setMatrixValue(m, i, 0, i + 1);
  }

  // Set the rmse values into the matrix
  uint l = 0;
  for (uint j = 0; j < yval; j++) {
    QString yname = m_obj->getYVarName()[j];

    double y_max = -9999.f;
    for (uint i = 0; i < model_nlv; i++) {
      m->data[i][1] = m_obj->Model()->sdec->data[i][l];
      m->data[i][2] = m_obj->Model()->sdep->data[i][l];

      if (m->data[i][1] > y_max)
        y_max = m->data[i][1];

      if (m->data[i][2] > y_max)
        y_max = m->data[i][2];
    }
    l++;

    plots.append(new SimpleLine2DPlot(
        m, curvenames,
        QString("%1 | %2 | RMSE Training/Validation Plot %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg(yname),
        "Latent Variables", "RMSE (Training/Validation)"));
    plots.last()->setXminXmaxXTick(0, model_nlv, model_nlv);
    plots.last()->setYminYmaxYTick(0, y_max, 10);
  }
  DelMatrix(&m);
  return plots;
}

QList<SimpleLine2DPlot *> PLSPlot::ROCAUCs() {
  QList<SimpleLine2DPlot *> plots;
  PLSModel *m_obj = getPLSModel();
  if (!m_obj || !isPLSDA()) return plots;

  uint model_nlv = getNPC() + 1; // +1 because we start from 0
  uint yval = m_obj->Model()->roc_auc_recalculated->col;
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, model_nlv, 3);
  curvenames << "AUC Recalculated" << "AUC Predicted";

  // set the X assis that is the principal component
  for (uint i = 0; i < model_nlv; i++) {
    setMatrixValue(m, i, 0, i);
  }

  // Set the values into the matrix
  uint l = 0;
  for (uint j = 0; j < yval; j++) {
    QString yname = m_obj->getClasses()[j].name;

    setMatrixValue(m, 0, 1, 0); // AUC Recalc in 0 pc is 0
    setMatrixValue(m, 0, 2, 0); // AUC Predicted in 0 pc is 0

    for (uint i = 0; i < model_nlv - 1; i++) {
      setMatrixValue(m, i + 1, 1,
                     getMatrixValue(m_obj->Model()->roc_auc_recalculated, i, l));
      setMatrixValue(m, i + 1, 2,
                     getMatrixValue(m_obj->Model()->roc_auc_validation, i, l));
    }
    l++;

    plots.append(new SimpleLine2DPlot(
        m, curvenames,
        QString("%1 | %2 | ROC AUC Recalculated/Predicted Plot %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg(yname),
        "Latent Variables", "ROC AUC Recalculated / AUC Predicted"));
    plots.last()->setXminXmaxXTick(0., (float)model_nlv, 1);
    plots.last()->setYminYmaxYTick(0., 1., 10);
  }
  DelMatrix(&m);
  return plots;
}

QList<SimpleLine2DPlot *> PLSPlot::ROCCurves() {
  QList<SimpleLine2DPlot *> plots;
  PLSModel *m_obj = getPLSModel();
  if (!m_obj || !isPLSDA()) return plots;

  uint model_nlv = getNPC();
  uint yval = m_obj->Model()->roc_recalculated->m[0]->col / 2.;

  QStringList curvenames;
  curvenames << "ROC Recalculated" << "ROC Predicted";

  tensor *roc_recalculated = m_obj->Model()->roc_recalculated;
  tensor *roc_predicted = m_obj->Model()->roc_validation;

  for (uint i = 0; i < model_nlv; i++) {
    for (uint j = 0; j < yval; j++) {
      QString yname = m_obj->getClasses()[j].name;
      matrix *mrec, *mpred;
      NewMatrix(&mrec, roc_recalculated->m[i]->row, 2);
      NewMatrix(&mpred, roc_predicted->m[i]->row, 2);
      for (uint k = 0; k < roc_recalculated->m[i]->row; k++) {
        mrec->data[k][0] = roc_recalculated->m[i]->data[k][j * 2];
        mrec->data[k][1] = roc_recalculated->m[i]->data[k][j * 2 + 1];
      }

      for (uint k = 0; k < roc_predicted->m[i]->row; k++) {
        mpred->data[k][0] = roc_predicted->m[i]->data[k][j * 2];
        mpred->data[k][1] = roc_predicted->m[i]->data[k][j * 2 + 1];
      }
      QList<matrix *> mlst;
      mlst.append(mrec);
      mlst.append(mpred);
      plots.append(new SimpleLine2DPlot(
          mlst, curvenames,
          QString("%1 | %2 | ROC Curve Recalculated/Predicted Plot %3 (LV: %4)")
              .arg(getProjectName())
              .arg(getModelName())
              .arg(yname)
              .arg(i + 1),
          "False positive rate", "True positive rate"));
      plots.last()->setPlotTitle(QString("N. LV: %1; Class name: %2")
                                     .arg(QString::number(i + 1))
                                     .arg(yname));
      plots.last()->setXminXmaxXTick(0, 1., model_nlv);
      plots.last()->setYminYmaxYTick(0, 1., model_nlv);
      DelMatrix(&mrec);
      DelMatrix(&mpred);
      mlst.clear();
    }
  }
  return plots;
}

QList<SimpleLine2DPlot *> PLSPlot::PrecisionRecallAveragePrecision() {
  QList<SimpleLine2DPlot *> plots;
  PLSModel *m_obj = getPLSModel();
  if (!m_obj || !isPLSDA()) return plots;

  uint model_nlv = getNPC() + 1; // +1 because we start from 0
  uint yval = m_obj->Model()->precision_recall_ap_recalculated->col;
  matrix *m;

  QStringList curvenames;
  NewMatrix(&m, model_nlv, 3);
  curvenames << "Precision-Recall AUC Recalculated"
             << "Precision-Recall AUC Predicted";

  // set the X assis that is the principal component
  for (uint i = 0; i < model_nlv; i++) {
    setMatrixValue(m, i, 0, i);
  }

  // Set the values into the matrix
  uint l = 0;
  for (uint j = 0; j < yval; j++) {
    QString yname = m_obj->getClasses()[j].name;

    setMatrixValue(m, 0, 1, 0); // AUC Recalc in 0 pc is 0
    setMatrixValue(m, 0, 2, 0); // AUC Predicted in 0 pc is 0

    for (uint i = 0; i < model_nlv - 1; i++) {
      setMatrixValue(m, i + 1, 1,
                     getMatrixValue(m_obj->Model()->precision_recall_ap_recalculated,
                                    i, l));
      setMatrixValue(m, i + 1, 2,
                     getMatrixValue(m_obj->Model()->precision_recall_ap_validation,
                                    i, l));
    }
    l++;

    plots.append(new SimpleLine2DPlot(
        m, curvenames,
        QString("%1 | %2 | Precision-Recall AUC Recalculated/Predicted Plot %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg(yname),
        "Latent Variables", "Prec./Rec. Recalculated and Predicted"));
    plots.last()->setXminXmaxXTick(0.f, (float)nlv, 1.f);
    plots.last()->setYminYmaxYTick(0., 1., 10);
  }
  DelMatrix(&m);
  return plots;
}

QList<SimpleLine2DPlot *> PLSPlot::PrecisionRecallCurves() {
  QList<SimpleLine2DPlot *> plots;
  PLSModel *m_obj = getPLSModel();
  if (!m_obj || !isPLSDA()) return plots;

  uint model_nlv = getNPC();
  uint yval = m_obj->Model()->precision_recall_recalculated->m[0]->col / 2.;

  QStringList curvenames;
  curvenames << "Precision-Recall Recalculated" << "Precision-Recall Predicted";

  tensor *pr_recalculated = m_obj->Model()->precision_recall_recalculated;
  tensor *pr_predicted = m_obj->Model()->precision_recall_validation;

  for (uint i = 0; i < model_nlv; i++) {
    for (uint j = 0; j < yval; j++) {
      QString yname = m_obj->getClasses()[j].name;
      matrix *mrec, *mpred;
      NewMatrix(&mrec, pr_recalculated->m[i]->row, 2);
      NewMatrix(&mpred, pr_predicted->m[i]->row, 2);
      for (uint k = 0; k < pr_recalculated->m[i]->row; k++) {
        mrec->data[k][0] = pr_recalculated->m[i]->data[k][j * 2];
        mrec->data[k][1] = pr_recalculated->m[i]->data[k][j * 2 + 1];
      }

      for (uint k = 0; k < pr_predicted->m[i]->row; k++) {
        mpred->data[k][0] = pr_predicted->m[i]->data[k][j * 2];
        mpred->data[k][1] = pr_predicted->m[i]->data[k][j * 2 + 1];
      }

      QList<matrix *> mlst;
      mlst.append(mrec);
      mlst.append(mpred);
      plots.append(new SimpleLine2DPlot(
          mlst, curvenames,
          QString("%1 | %2 | Precision-Recall Recalculated/Predicted Plot %3 (LV: %4)")
              .arg(getProjectName())
              .arg(getModelName())
              .arg(yname)
              .arg(i + 1),
          "Recall", "Precision"));
      plots.last()->setPlotTitle(QString("N. LV: %1; Class name: %2")
                                     .arg(QString::number(i + 1))
                                     .arg(yname));
      plots.last()->setXminXmaxXTick(0, 1., model_nlv);
      plots.last()->setYminYmaxYTick(0, 1., model_nlv);
      DelMatrix(&mrec);
      DelMatrix(&mpred);
      mlst.clear();
    }
  }
  return plots;
}

QList<SimpleLine2DPlot *> PLSPlot::R2R2Prediction() {
  QList<SimpleLine2DPlot *> plots;
  PLSModel *m_obj = getPLSModel();
  if (!m_obj || !isPLS()) return plots;

  uint model_nlv = getNPC() + 1; // +1 because we start from 0
  uint yval = m_obj->Model()->r2y_recalculated->col;

  bool getq2 = false;
  matrix *m;

  int ncol = 3;
  if (m_obj->Model()->q2y->row > 0) {
    ncol++;
    getq2 = true;
  }

  NewMatrix(&m, model_nlv, ncol);

  // set the X assis that is the principal component
  for (uint i = 0; i < model_nlv; i++) {
    setMatrixValue(m, i, 0, i);
  }

  // Set the values into the matrix
  uint l = 0;
  for (uint j = 0; j < yval; j++) {
    QString yname = m_obj->getYVarName()[j];

    QStringList curvenames;
    curvenames << "R2";
    if (getq2 == true) {
      curvenames << QString("Q^2 Y %1").arg(yname);
    }
    curvenames << QString("R^2 Predicted Y %1").arg(yname);

    int k = 1;
    setMatrixValue(m, 0, k, 0); // R^2 in 0 pc is 0
    k++;

    if (getq2 == true) {
      setMatrixValue(m, 0, k, 0); // Q^2 in 0 pc is 0
      k++;
    }

    setMatrixValue(m, 0, k, 0); // R2 Predicted

    for (uint i = 0; i < model_nlv - 1; i++) {
      k = 1;
      setMatrixValue(m, i + 1, k,
                     getMatrixValue(m_obj->Model()->r2y_recalculated, i, l));
      k++;

      if (getq2 == true) {
        double q2 = getMatrixValue(m_obj->Model()->q2y, i, l);
        if (q2 < 0) {
          setMatrixValue(m, i + 1, k, 0.f);
        } else {
          setMatrixValue(m, i + 1, k, q2);
        }
        k++;
      }

      double r2pred =
          getMatrixValue(m_obj->getPLSPrediction(predid)->getR2Y(), i, l);
      if (r2pred < 0) {
        setMatrixValue(m, i + 1, k, 0.f);
      } else {
        setMatrixValue(m, i + 1, k, r2pred);
      }
    }
    l++;

    QString yaxisname = "R^2";
    if (getq2 == true)
      yaxisname += " / Q2";
    yaxisname += " / R2 Predicted";

    plots.append(new SimpleLine2DPlot(m, curvenames,
                                      QString("%1 | %2 | R2 Q2 Plot Y %3")
                                          .arg(getProjectName())
                                          .arg(getModelName())
                                          .arg(yname),
                                      "Latent Variables", yaxisname));
    plots.last()->setXminXmaxXTick(0, model_nlv, model_nlv);
    plots.last()->setYminYmaxYTick(0, 1, 10);
  }
  DelMatrix(&m);
  return plots;
}

QList<SimpleLine2DPlot *> PLSPlot::RMSEPrediction() {
  QList<SimpleLine2DPlot *> plots;
  PLSModel *m_obj = getPLSModel();
  if (!m_obj || !isPLS()) return plots;

  uint model_nlv = getNPC();
  uint yval = m_obj->Model()->r2y_recalculated->col;

  bool getsdep = false;
  matrix *m;

  int ncol = 3;
  if (m_obj->Model()->sdep->row > 0) {
    ncol++;
    getsdep = true;
  }

  NewMatrix(&m, model_nlv, ncol);

  // set the X assis that is the principal component
  for (uint i = 0; i < model_nlv; i++) {
    setMatrixValue(m, i, 0, i + 1);
  }

  // Set the values into the matrix
  uint l = 0;
  for (uint j = 0; j < yval; j++) {
    QString yname = m_obj->getYVarName()[j];

    QStringList curvenames;
    curvenames << "RMSE(Training)";
    if (getsdep == true) {
      curvenames << QString("RMSE (Testing) Y %1").arg(yname);
    }
    curvenames << QString("RMSE (Prediction) Y %1").arg(yname);

    double y_max = -9999.f;
    for (uint i = 0; i < model_nlv; i++) {
      int k = 1;
      m->data[i][k] = m_obj->Model()->sdec->data[i][l];
      if (m->data[i][k] > y_max)
        y_max = m->data[i][k];
      k++;

      if (getsdep == true) {
        m->data[i][k] = m_obj->Model()->sdep->data[i][l];
        if (m->data[i][k] > y_max)
          y_max = m->data[i][k];
        k++;
      }

      m->data[i][k] = m_obj->getPLSPrediction(predid)->getSDEC()->data[i][l];
      if (m->data[i][k] > y_max)
        y_max = m->data[i][k];
    }
    l++;

    QString yaxisname = "RMSE (Training";
    if (getsdep == true)
      yaxisname += "/Testing";
    yaxisname += " /Prediction)";

    plots.append(new SimpleLine2DPlot(
        m, curvenames,
        QString("%1 | %2 | RMSE External Prediction Plot Y %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg(yname),
        "Latent Variables", yaxisname));
    plots.last()->setXminXmaxXTick(0, model_nlv, model_nlv);
    plots.last()->setYminYmaxYTick(0, y_max, 10);
  }
  DelMatrix(&m);
  return plots;
}

QList<ScatterPlot *> PLSPlot::YScramblingPlot() {
  QList<ScatterPlot *> plots2D;
  PLSModel *m = getPLSModel();
  if (!m) return plots2D;

  QList<matrix *> mxlst;
  mxlst.append(new matrix);
  size_t nobj = m->Model()->yscrambling->row;
  size_t ndepvar = (m->Model()->yscrambling->col - 1) / 2;
  NewMatrix(&mxlst.last(), nobj, 2); // R2
  mxlst.append(new matrix);
  NewMatrix(&mxlst.last(), nobj, 2); // Q2
  QList<QStringList> objnamelst;
  objnamelst.append(QStringList());
  objnamelst.append(QStringList());
  /*set the X Constant */
  for (size_t i = 0; i < nobj; i++) {
    mxlst[0]->data[i][0] = m->Model()->yscrambling->data[i][0];
    mxlst[1]->data[i][0] = m->Model()->yscrambling->data[i][0];
    objnamelst[0].append("R2");
    objnamelst[1].append("Q2");
  }

  for (size_t j = 0; j < ndepvar; j++) {
    for (size_t i = 0; i < nobj; i++) {
      mxlst[0]->data[i][1] = m->Model()->yscrambling->data[i][j + 1];
      mxlst[1]->data[i][1] = m->Model()->yscrambling->data[i][j + 1 + ndepvar];
    }
    plots2D.append(new ScatterPlot(
        mxlst, objnamelst, "Correlation with Y Real Vector", "R2/Q2",
        QString("%1 | %2 | PLS Y Scrambling Plot")
            .arg(getProjectName())
            .arg(getModelName())));
    plots2D.last()->setPID(pid);
  }

  DelMatrix(&mxlst[1]);
  DelMatrix(&mxlst[0]);
  mxlst.clear();
  return plots2D;
}

void PLSPlot::T_ScorePlot3D(ScatterPlot **plot3D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->xscores};
  QList<QStringList> objname = {m->getObjName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot3D) = new ScatterPlot(
      mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "t", "t", "t",
      QString("%1 | %2 | PLS T Score Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::SCORES);
  (*plot3D)->setPID(pid);
  (*plot3D)->setImages(projects->value(pid)->getImages());
}

void PLSPlot::P_LoadingsPlot3D(ScatterPlot **plot3D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->xloadings};
  QList<QStringList> objname = {m->getXVarName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot3D) = new ScatterPlot(
      mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "p", "p", "p",
      QString("%1 | %2 | PLS P Loadings Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::LOADINGS);
  (*plot3D)->setPID(pid);
}

void PLSPlot::WeightsPlot3D(ScatterPlot **plot3D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  matrix *yloadingsbis;
  NewMatrix(&yloadingsbis, m->Model()->yloadings->row, m->Model()->yloadings->col);

  if (m->Model()->yloadings->row > 1) {
    for (uint j = 0; j < m->Model()->xweights->col; j++) {
      double xmin, xmax, ymin, ymax;
      MatrixColumnMinMax(m->Model()->xweights, j, &xmin, &xmax);
      MatrixColumnMinMax(m->Model()->yloadings, j, &ymin, &ymax);
      xmax += xmax * 0.1;
      xmin += xmin * 0.1;
      for (uint i = 0; i < m->Model()->yloadings->row; i++) {
        yloadingsbis->data[i][j] = (((xmax - xmin) * (m->Model()->yloadings->data[i][j] - ymin)) / (ymax - ymin)) + xmin;
      }
    }
  } else {
    for (uint j = 0; j < m->Model()->xweights->col; j++) {
      double xmin, xmax;
      MatrixColumnMinMax(m->Model()->xweights, j, &xmin, &xmax);
      xmax += xmax * 0.1;
      for (uint i = 0; i < m->Model()->yloadings->row; i++) {
        yloadingsbis->data[i][j] = xmax * m->Model()->yloadings->data[i][j];
      }
    }
  }

  QList<matrix *> mxlst = {m->Model()->xweights, yloadingsbis};
  QList<QStringList> objnamelst = {m->getXVarName(), m->getYVarName()};
  QStringList xhash = {m->getDataHash(), m->getDataHash()};
  QStringList yhash = {m->getDataHash(), m->getDataHash()};

  (*plot3D) = new ScatterPlot(
      mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "w", "w", "w",
      QString("%1 | %2 | PLS W Weights Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::LOADINGS);
  (*plot3D)->setPID(pid);
  DelMatrix(&yloadingsbis);
}

void PLSPlot::U_ScorePlot3D(ScatterPlot **plot3D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->yscores};
  QList<QStringList> objname = {m->getObjName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot3D) = new ScatterPlot(
      mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "u", "u", "u",
      QString("%1 | %2 | PLS U Score Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::SCORES);
  (*plot3D)->setPID(pid);
  (*plot3D)->setImages(projects->value(pid)->getImages());
}

void PLSPlot::Q_LoadingsPlot3D(ScatterPlot **plot3D) {
  PLSModel *m = getPLSModel();
  if (!m) return;

  QList<matrix *> mxlst = {m->Model()->yloadings};
  QList<QStringList> objname = {m->getYVarName()};
  QStringList xhash = {m->getDataHash()}, yhash = {m->getDataHash()};

  (*plot3D) = new ScatterPlot(
      mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "q", "q", "q",
      QString("%1 | %2 | PLS Q Loadings Plot").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::LOADINGS);
  (*plot3D)->setPID(pid);
}

void PLSPlot::T_ScorePlotPrediction3D(ScatterPlot **plot3D) {
  PLSModel *m = getPLSModel();
  if (!m || predid == -1) return;

  QList<matrix *> mxlst = {m->Model()->xscores, m->getPLSPrediction(predid)->getXPredScores()};
  QList<QStringList> objname = {m->getObjName(), m->getPLSPrediction(predid)->getObjName()};
  QStringList xhash = {m->getDataHash(), m->getPLSPrediction(predid)->getDataHash()};
  QStringList yhash = {m->getDataHash(), m->getDataHash()};

  (*plot3D) = new ScatterPlot(
      mxlst, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), "t", "t", "t",
      QString("%1 | %2 | PLS T Score Plot Prediction").arg(getProjectName()).arg(getModelName()),
      ScatterPlot::SCORES);
  (*plot3D)->setPID(pid);
  (*plot3D)->setImages(projects->value(pid)->getImages());
}

PLSPlot::PLSPlot(PROJECTS *projects_) {
  pid = mid = predid = nlv = -1;
  projects = projects_;
}

QString PLSPlot::getProjectName() const {
  if (pid > -1 && projects->contains(pid))
    return projects->value(pid)->getProjectName();
  return QString();
}

QString PLSPlot::getModelName() const {
  PLSModel *m = getPLSModel();
  return m ? m->getName() : QString();
}

PLSModel *PLSPlot::getPLSModel() const {
  if (pid > -1 && mid > -1 && projects->contains(pid))
    return projects->value(pid)->getPLSModel(mid);
  return nullptr;
}

int PLSPlot::getNPC() const {
  PLSModel *m = getPLSModel();
  return m ? m->getNPC() : -1;
}

QString PLSPlot::getDataHash() const {
  PLSModel *m = getPLSModel();
  return m ? m->getDataHash() : QString();
}

bool PLSPlot::isPLSDA() const {
  PLSModel *m = getPLSModel();
  return m ? (m->getAlgorithm() == PLS_DA_) : false;
}

bool PLSPlot::isPLS() const {
  PLSModel *m = getPLSModel();
  return m ? (m->getAlgorithm() == PLS_) : false;
}

void PLSPlot::extractExperimentalY(int matrix_id, const QStringList &objsel,
                                   const QStringList &yvarsel, matrix **out_y) {
  if (matrix_id == -1 || !projects->contains(pid)) return;
  
  MATRIX *m_obj = projects->value(pid)->getMatrix(matrix_id);
  if (!m_obj) return;

  uint nobjects = objsel.size();
  uint nvars = yvarsel.size();
  NewMatrix(out_y, nobjects, nvars);
  
  QStringList m_objnames = m_obj->getObjName();
  QStringList m_varnames = m_obj->getVarName();
  matrix *m_data = m_obj->Matrix();

  int ii = 0;
  for (int i = 0; i < m_objnames.size(); i++) {
    if (objsel.contains(m_objnames[i])) {
      int jy = 0;
      for (int j = 1; j < m_varnames.size(); j++) {
        if (yvarsel.contains(m_varnames[j])) {
          setMatrixValue(*out_y, ii, jy, getMatrixValue(m_data, i, j - 1));
          jy++;
        }
      }
      ii++;
    }
  }
}

void PLSPlot::extractModelMatrix(matrix *source, uint nobjects, uint nvars,
                                 matrix **out_y) {
  if (!source) return;
  NewMatrix(out_y, nobjects, nvars);
  
  int current_nlv = nlv;
  if (current_nlv > getNPC()) current_nlv = getNPC();

  for (uint i = 0; i < nobjects; i++) {
    for (uint j = 0; j < nvars; j++) {
      setMatrixValue(*out_y, i, j,
                     getMatrixValue(source, i, j + (nvars * current_nlv) - nvars));
    }
  }
}
