/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for mlrplot.
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

#include "MLRPlot.h"
#include "scientific.h"

void MLRPlot::RecalcVSExperimentalAndPrediction(ScatterPlot **plot2D) {
  MLRModel *mod = getMLRModel();
  if (!mod || predid == -1) return;

  int did = projects->value(pid)->getMatrixID(mod->getDataHash());
  MLRPREDICTION *pred = mod->getMLRPrediction(predid);
  int pred_id = projects->value(pid)->getMatrixID(pred->getDataHash());

  if (did > -1 && pred_id > -1) {
    matrix *y, *real_pred_y, *recalc_y, *pred_y;
    
    extractExperimentalY(did, mod->getObjName(), mod->getYVarName(), &y);
    extractExperimentalY(pred_id, pred->getObjName(), mod->getYVarName(), &real_pred_y);

    initMatrix(&recalc_y);
    MatrixCopy(mod->Model()->recalculated_y, &recalc_y);
    initMatrix(&pred_y);
    MatrixCopy(pred->getYDipVar(), &pred_y);

    QList<matrix *> mx = {y, real_pred_y};
    QList<matrix *> my = {recalc_y, pred_y};
    QList<QStringList> objname = {mod->getObjName(), pred->getObjName()};
    QStringList xhash = {mod->getDataHash(), pred->getDataHash()};
    QStringList yhash = {mod->getDataHash(), pred->getDataHash()};

    (*plot2D) = new ScatterPlot(
        mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), QString("Experimental Y"),
        QString("Recalculated and Predicted Y"),
        QString("%1 | %2 | %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg("MLR Recalc VS Experimental Plot"),
        ScatterPlot::SCORES);

    (*plot2D)->setImages(projects->value(pid)->getImages());
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setPID(pid);

    DelMatrix(&recalc_y);
    DelMatrix(&y);
    DelMatrix(&real_pred_y);
    DelMatrix(&pred_y);
  }
}

void MLRPlot::PredictedVSExperimentalAndPrediction(ScatterPlot **plot2D) {
  MLRModel *mod = getMLRModel();
  if (!mod || predid == -1) return;

  int did = projects->value(pid)->getMatrixID(mod->getDataHash());
  MLRPREDICTION *pred = mod->getMLRPrediction(predid);
  int pred_id = projects->value(pid)->getMatrixID(pred->getDataHash());

  if (did > -1 && pred_id > -1) {
    matrix *model_exp_y, *pred_exp_y, *model_predicted_y, *pred_predicted_y;

    extractExperimentalY(did, mod->getObjName(), mod->getYVarName(), &model_exp_y);
    extractExperimentalY(pred_id, pred->getObjName(), mod->getYVarName(), &pred_exp_y);

    initMatrix(&model_predicted_y);
    MatrixCopy(mod->Model()->predicted_y, &model_predicted_y);
    initMatrix(&pred_predicted_y);
    MatrixCopy(pred->getYDipVar(), &pred_predicted_y);

    QList<matrix *> mx = {model_exp_y, pred_exp_y};
    QList<matrix *> my = {model_predicted_y, pred_predicted_y};
    QList<QStringList> objname = {mod->getObjName(), pred->getObjName()};
    QStringList xhash = {mod->getDataHash(), pred->getDataHash()};
    QStringList yhash = {mod->getDataHash(), pred->getDataHash()};

    (*plot2D) = new ScatterPlot(
        mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), QString("Experimental"),
        QString("Predicted"),
        QString("%1 | %2 | %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg("MLR Predicted VS Experimental Plot"),
        ScatterPlot::SCORES);

    (*plot2D)->setImages(projects->value(pid)->getImages());
    (*plot2D)->setAxisNameExtensions(mod->getYVarName());
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setPID(pid);

    DelMatrix(&model_exp_y);
    DelMatrix(&model_predicted_y);
    DelMatrix(&pred_exp_y);
    DelMatrix(&pred_predicted_y);
  }
}

QList<BarPlot *> MLRPlot::BetaCoefficients() {
  MLRModel *mod = getMLRModel();
  if (!mod) return QList<BarPlot *>();

  QStringList varnames = mod->getXVarName();
  matrix *betas = mod->Model()->b;
  QList<BarPlot *> barplots;
  for (size_t j = 0; j < betas->col; j++) {
    dvector *v = getMatrixColumn(betas, j);
    barplots.append(
        new BarPlot(v, varnames,
                    QString("%1 | %2 | Beta Coefficients")
                        .arg(getProjectName())
                        .arg(getModelName()),
                    "Betas", "Variables"));
    DelDVector(&v);
  }
  return barplots;
}

void MLRPlot::RecalcVSExperimental(ScatterPlot **plot2D) {
  MLRModel *mod = getMLRModel();
  if (!mod) return;

  int did = projects->value(pid)->getMatrixID(mod->getDataHash());
  matrix *model_recalc_y, *model_exp_y;

  initMatrix(&model_recalc_y);
  MatrixCopy(mod->Model()->recalculated_y, &model_recalc_y);
  extractExperimentalY(did, mod->getObjName(), mod->getYVarName(), &model_exp_y);

  QList<matrix *> mx = {model_exp_y};
  QList<matrix *> my = {model_recalc_y};
  QList<QStringList> objname = {mod->getObjName()};
  QStringList xhash = {mod->getDataHash()};
  QStringList yhash = {mod->getDataHash()};

  (*plot2D) = new ScatterPlot(
      mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
      &projects->value(pid)->getObjectLabels(),
      &projects->value(pid)->getVariableLabels(), QString("Experimental"),
      QString("Recalculated"),
      QString("%1 | %2 | %3")
          .arg(getProjectName())
          .arg(getModelName())
          .arg("MLR Recalculated VS Experimental Plot"),
      ScatterPlot::SCORES);

  (*plot2D)->setImages(projects->value(pid)->getImages());
  (*plot2D)->BuildDiagonal();
  (*plot2D)->setPID(pid);
  (*plot2D)->setAxisNameExtensions(mod->getYVarName());
  DelMatrix(&model_recalc_y);
  DelMatrix(&model_exp_y);
}

void MLRPlot::RecalcResidualsVSExperimental(ScatterPlot **plot2D) {
  MLRModel *mod = getMLRModel();
  if (!mod) return;

  int did = projects->value(pid)->getMatrixID(mod->getDataHash());
  if (did > -1) {
    matrix *recalc_res, *model_exp_y;
    initMatrix(&recalc_res);
    MatrixCopy(mod->Model()->recalc_residuals, &recalc_res);
    extractExperimentalY(did, mod->getObjName(), mod->getYVarName(), &model_exp_y);

    QList<matrix *> mx = {model_exp_y};
    QList<matrix *> my = {recalc_res};
    QList<QStringList> objname = {mod->getObjName()};
    QStringList xhash = {mod->getDataHash()};
    QStringList yhash = {mod->getDataHash()};

    (*plot2D) = new ScatterPlot(
        mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), QString("Experimental"),
        QString("Recalculated Residuals"),
        QString("%1 | %2 | %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg("MLR Experimental VS Recalculated Residuals Y Plot"),
        ScatterPlot::SCORES);

    (*plot2D)->setImages(projects->value(pid)->getImages());
    DelMatrix(&recalc_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(mod->getYVarName());
    DelMatrix(&model_exp_y);
  }
}

void MLRPlot::PredictedVSExperimental(ScatterPlot **plot2D) {
  MLRModel *mod = getMLRModel();
  if (!mod) return;

  int did = projects->value(pid)->getMatrixID(mod->getDataHash());
  if (did > -1) {
    matrix *model_pred_y, *model_exp_y;
    initMatrix(&model_pred_y);
    MatrixCopy(mod->Model()->predicted_y, &model_pred_y);
    extractExperimentalY(did, mod->getObjName(), mod->getYVarName(), &model_exp_y);

    QList<matrix *> mx = {model_exp_y};
    QList<matrix *> my = {model_pred_y};
    QList<QStringList> objname = {mod->getObjName()};
    QStringList xhash = {mod->getDataHash()};
    QStringList yhash = {mod->getDataHash()};

    (*plot2D) = new ScatterPlot(
        mx, my, objname, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), QString("Experimental"),
        QString("Predicted"),
        QString("%1 | %2 | %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg("MLR Predicted VS Experimental Plot"),
        ScatterPlot::SCORES);

    (*plot2D)->setImages(projects->value(pid)->getImages());
    (*plot2D)->BuildDiagonal();
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(mod->getYVarName());
    DelMatrix(&model_exp_y);
    DelMatrix(&model_pred_y);
  }
}

void MLRPlot::PredictedResidualsVSExperimental(ScatterPlot **plot2D) {
  MLRModel *mod = getMLRModel();
  if (!mod) return;

  int did = projects->value(pid)->getMatrixID(mod->getDataHash());
  if (did > -1) {
    matrix *pred_res, *model_exp_y;
    initMatrix(&pred_res);
    MatrixCopy(mod->Model()->pred_residuals, &pred_res);
    extractExperimentalY(did, mod->getObjName(), mod->getYVarName(), &model_exp_y);

    QList<matrix *> mx = {model_exp_y};
    QList<matrix *> my = {pred_res};
    QList<QStringList> objname = {mod->getObjName()};
    QStringList xhash = {mod->getDataHash()};
    QStringList yhash = {mod->getDataHash()};

    (*plot2D) =
        new ScatterPlot(mx, my, objname, &projects->value(pid)->getMATRIXList(),
                        xhash, yhash, &projects->value(pid)->getObjectLabels(),
                        &projects->value(pid)->getVariableLabels(),
                        QString("Experimental"), QString("Predicted Residuals"),
                        QString("%1 | %2 | %3")
                            .arg(getProjectName())
                            .arg(getModelName())
                            .arg("MLR Experimental VS Predicted Residuals Y Plot"),
                        ScatterPlot::SCORES);
    (*plot2D)->setImages(projects->value(pid)->getImages());
    DelMatrix(&pred_res);
    (*plot2D)->setPID(pid);
    (*plot2D)->setAxisNameExtensions(mod->getYVarName());
    DelMatrix(&model_exp_y);
  }
}

QList<ScatterPlot *> MLRPlot::YScramblingPlot() {
  MLRModel *mod = getMLRModel();
  if (!mod) return QList<ScatterPlot *>();

  QList<ScatterPlot *> plots2D;
  QList<matrix *> mxlst;
  mxlst.append(new matrix);
  size_t nobj = mod->Model()->r2q2scrambling->row;
  size_t ndepvar = (mod->Model()->r2q2scrambling->col - 1) / 2;
  NewMatrix(&mxlst.last(), nobj, 2); // R2
  mxlst.append(new matrix);
  NewMatrix(&mxlst.last(), nobj, 2); // Q2
  QList<QStringList> objnamelst;
  objnamelst.append(QStringList());
  objnamelst.append(QStringList());
  /*set the X Constant */
  for (size_t i = 0; i < nobj; i++) {
    mxlst[0]->data[i][0] = mod->Model()->r2q2scrambling->data[i][0];
    mxlst[1]->data[i][0] = mod->Model()->r2q2scrambling->data[i][0];
    objnamelst[0].append("R2");
    objnamelst[1].append("Q2");
  }

  for (size_t j = 0; j < ndepvar; j++) {
    for (size_t i = 0; i < nobj; i++) {
      mxlst[0]->data[i][1] = mod->Model()->r2q2scrambling->data[i][j + 1];
      mxlst[1]->data[i][1] = mod->Model()->r2q2scrambling->data[i][j + 1 + ndepvar];
    }
    plots2D.append(new ScatterPlot(mxlst, objnamelst,
                                   "Correlation with Y Real Vector", "R2/Q2",
                                   QString("%1 | %2 | Y Scrambling Plot")
                                       .arg(getProjectName())
                                       .arg(getModelName())));
    plots2D.last()->setPID(pid);
  }

  DelMatrix(&mxlst[1]);
  DelMatrix(&mxlst[0]);
  mxlst.clear();
  return plots2D;
}

MLRPlot::MLRPlot(PROJECTS *projects_) {
  pid = mid = predid = -1;
  projects = projects_;
}

QString MLRPlot::getProjectName() const {
  if (pid > -1 && projects->contains(pid))
    return projects->value(pid)->getProjectName();
  return QString();
}

QString MLRPlot::getModelName() const {
  MLRModel *m = getMLRModel();
  return m ? m->getName() : QString();
}

MLRModel *MLRPlot::getMLRModel() const {
  if (pid > -1 && mid > -1 && projects->contains(pid))
    return projects->value(pid)->getMLRModel(mid);
  return nullptr;
}

void MLRPlot::extractExperimentalY(int matrix_id, const QStringList &objsel,
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
