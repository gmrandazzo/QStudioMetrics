/*
 * This project uses Qt under the GNU General Public License version 3.0 (GPL‑3.0).
 *
 * Visualization component for ldaplot.
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

#include "LDAPlot.h"
#include <unistd.h>

void LDAPlot::FeaturePlot2D(ScatterPlot **plot2D) {
  if (mid > -1) {
    QList<matrix *> mxlst;
    QList<QStringList> objnamelst;
    prepareFeaturesData(mxlst, objnamelst);

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(
        mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), "LD", "LD",
        QString("%1 | %2 | %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg("LDA Feature Plot"),
        ScatterPlot::SCORES);
    (*plot2D)->setPID(pid);
    (*plot2D)->setImages(projects->value(pid)->getImages());
  }
}

void LDAPlot::FeaturePlot3D(ScatterPlot **plot3D) {
  if (mid > -1) {
    QList<matrix *> mxlst;
    QList<QStringList> objnamelst;
    prepareFeaturesData(mxlst, objnamelst);

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    (*plot3D) = new ScatterPlot(
        mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(),
        QString("%1 | %2 | LDA Feature Plot").arg(getProjectName()).arg(getModelName()),
        "LD", "LD", "LD", ScatterPlot::SCORES);
    (*plot3D)->setPID(pid);
    (*plot3D)->setImages(projects->value(pid)->getImages());
  }
}

void LDAPlot::ProbabilityDistribution(ScatterPlot **plot2D) {
  if (mid > -1) {
    QList<matrix *> mx, my;
    QList<QStringList> objnamelst;
    prepareFeaturesData(mx, objnamelst);

    LDAMODEL *lda = getModel();
    for (uint k = 0; k < lda->features->order; k++) {
      my.append(lda->mnpdf->m[k]);
    }

    QList<matrix *> mnpdf;
    QStringList mnpdfname;
    QList<QColor> colors;
    prepareMNPDFData(mnpdf, mnpdfname, colors);

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());

    (*plot2D) = new ScatterPlot(
        mx, my, objnamelst, &projects->value(pid)->getMATRIXList(), xhash,
        yhash, &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), "Y", "Prob Y",
        QString("%1 | %2 | %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg("LDA Multivariate Normal Distribution of Probabilities"),
        ScatterPlot::SCORES);

    (*plot2D)->addCurve(mnpdf, mnpdfname, colors, true);
    (*plot2D)->setPID(pid);
    (*plot2D)->setImages(projects->value(pid)->getImages());

    for (uint k = 0; k < lda->features->order; k++) {
      DelMatrix(&mnpdf[k]);
    }
  }
}

QList<SimpleLine2DPlot *> LDAPlot::ROCPlot() {
  QList<SimpleLine2DPlot *> plots;
  LDAMODEL *lda = getModel();
  if (lda) {
    matrix *m;
    QStringList curvenames = {"Current classifier", "Random classifier"};

    for (size_t k = 0; k < lda->roc->order; k++) {
      QString classname = projects->value(pid)->getLDAModel(mid)->getNameClasses()[k];
      NewMatrix(&m, lda->roc->m[k]->row + 1, 3);
      for (size_t i = 0; i < lda->roc->m[k]->row; i++) {
        m->data[i][0] = lda->roc->m[k]->data[i][0];
        m->data[i][1] = lda->roc->m[k]->data[i][1]; // current classifier
        m->data[i][2] = lda->roc->m[k]->data[i][0]; // random classifier
      }
      m->data[lda->roc->m[k]->row][0] = 1.;
      m->data[lda->roc->m[k]->row][1] = 1.; // current classifier
      m->data[lda->roc->m[k]->row][2] = 1.; // random classifier

      plots.append(new SimpleLine2DPlot(m, curvenames,
                                        QString("%1 | %2 | ROC Plot - %3")
                                            .arg(getProjectName())
                                            .arg(getModelName())
                                            .arg(classname),
                                        "FP Rate", "TP Rate"));
      plots.last()->setXminXmaxXTick(0, 1., 10);
      plots.last()->setYminYmaxYTick(0, 1., 10);
      DelMatrix(&m);
    }
  }
  return plots;
}

QList<SimpleLine2DPlot *> LDAPlot::PRPlot() {
  QList<SimpleLine2DPlot *> plots;
  LDAMODEL *lda = getModel();
  if (lda) {
    matrix *m;
    QStringList curvenames = {"Current classifier", "Random classifier"};

    for (size_t k = 0; k < lda->pr->order; k++) {
      QString classname = projects->value(pid)->getLDAModel(mid)->getNameClasses()[k];
      NewMatrix(&m, lda->pr->m[k]->row + 1, 3);
      for (size_t i = 0; i < lda->pr->m[k]->row; i++) {
        m->data[i][0] = lda->pr->m[k]->data[i][0];
        m->data[i][1] = lda->pr->m[k]->data[i][1];     // current classifier
        m->data[i][2] = 1 - lda->pr->m[k]->data[i][0]; // random classifier
      }
      m->data[lda->pr->m[k]->row][0] = 1.;
      m->data[lda->pr->m[k]->row][1] = 0.;
      m->data[lda->pr->m[k]->row][2] = 0.;

      plots.append(new SimpleLine2DPlot(m, curvenames,
                                        QString("%1 | %2 | Precision-Recall Plot - %3")
                                            .arg(getProjectName())
                                            .arg(getModelName())
                                            .arg(classname),
                                        "Recall", "Precision"));
      plots.last()->setXminXmaxXTick(0, 1., 10);
      plots.last()->setYminYmaxYTick(0, 1., 10);
      DelMatrix(&m);
    }
  }
  return plots;
}
void LDAPlot::FeaturePlotAndPrediction2D(ScatterPlot **plot2D) {
  if (mid > -1 && predid > -1) {
    QList<matrix *> mxlst;
    QList<QStringList> objnamelst;
    prepareFeaturesData(mxlst, objnamelst);

    mxlst.append(projects->value(pid)
                     ->getLDAModel(mid)
                     ->getLDAPrediction(predid)
                     ->getPredFeatures());
    objnamelst.append(projects->value(pid)
                          ->getLDAModel(mid)
                          ->getLDAPrediction(predid)
                          ->getObjName());

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    xhash.append(projects->value(pid)
                     ->getLDAModel(mid)
                     ->getLDAPrediction(predid)
                     ->getDataHash());
    (*plot2D) = new ScatterPlot(
        mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), "LD", "LD",
        QString("%1 | %2 | %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg("LDA Feature Plot"),
        ScatterPlot::SCORES);
    (*plot2D)->setPID(pid);
    (*plot2D)->setImages(projects->value(pid)->getImages());
  }
}

void LDAPlot::FeaturePlotAndPrediction3D(ScatterPlot **plot3D) {
  if (mid > -1 && predid > -1) {
    QList<matrix *> mxlst;
    QList<QStringList> objnamelst;
    prepareFeaturesData(mxlst, objnamelst);

    mxlst.append(projects->value(pid)
                     ->getLDAModel(mid)
                     ->getLDAPrediction(predid)
                     ->getPredFeatures());
    objnamelst.append(projects->value(pid)
                          ->getLDAModel(mid)
                          ->getLDAPrediction(predid)
                          ->getObjName());

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    xhash.append(projects->value(pid)
                     ->getLDAModel(mid)
                     ->getLDAPrediction(predid)
                     ->getDataHash());
    (*plot3D) = new ScatterPlot(
        mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash,
        &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(),
        QString("%1 | %2 | LDA Feature Plot").arg(getProjectName()).arg(getModelName()),
        "LD", "LD", "LD", ScatterPlot::SCORES);
    (*plot3D)->setPID(pid);
    (*plot3D)->setImages(projects->value(pid)->getImages());
  }
}

void LDAPlot::ProbabilityDistributionWithPredictions(ScatterPlot **plot2D) {
  if (mid > -1 && predid > -1) {
    QList<matrix *> mx, my;
    QList<QStringList> objnamelst;
    prepareFeaturesData(mx, objnamelst);

    LDAMODEL *lda = getModel();
    for (uint k = 0; k < lda->features->order; k++) {
      my.append(lda->mnpdf->m[k]);
    }

    mx.append(projects->value(pid)
                  ->getLDAModel(mid)
                  ->getLDAPrediction(predid)
                  ->getPredFeatures());
    my.append(projects->value(pid)
                  ->getLDAModel(mid)
                  ->getLDAPrediction(predid)
                  ->getMVNProbDistrib());
    objnamelst.append(projects->value(pid)
                          ->getLDAModel(mid)
                          ->getLDAPrediction(predid)
                          ->getObjName());

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    xhash.append(projects->value(pid)
                     ->getLDAModel(mid)
                     ->getLDAPrediction(predid)
                     ->getDataHash());

    QList<matrix *> mnpdf;
    QStringList mnpdfname;
    QList<QColor> colors;
    prepareMNPDFData(mnpdf, mnpdfname, colors);

    (*plot2D) = new ScatterPlot(
        mx, my, objnamelst, &projects->value(pid)->getMATRIXList(), xhash,
        yhash, &projects->value(pid)->getObjectLabels(),
        &projects->value(pid)->getVariableLabels(), "Y", "Prob Y",
        QString("%1 | %2 | %3")
            .arg(getProjectName())
            .arg(getModelName())
            .arg("LDA Multivariate Normal Distribution of Probabilities"),
        ScatterPlot::SCORES);

    (*plot2D)->addCurve(mnpdf, mnpdfname, colors, true);
    (*plot2D)->setPID(pid);
    (*plot2D)->setImages(projects->value(pid)->getImages());
    (*plot2D)->PlotUpdate();
    for (uint k = 0; k < lda->features->order; k++) {
      DelMatrix(&mnpdf[k]);
    }
  }
}

LDAPlot::LDAPlot(PROJECTS *projects_) {
  pid = mid = predid = -1;
  projects = projects_;
}

QString LDAPlot::getProjectName() const {
  if (pid > -1 && projects->contains(pid))
    return projects->value(pid)->getProjectName();
  return QString();
}

QString LDAPlot::getModelName() const {
  if (pid > -1 && mid > -1 && projects->contains(pid))
    return projects->value(pid)->getLDAModel(mid)->getName();
  return QString();
}

LDAMODEL *LDAPlot::getModel() const {
  if (pid > -1 && mid > -1 && projects->contains(pid))
    return projects->value(pid)->getLDAModel(mid)->Model();
  return nullptr;
}

void LDAPlot::prepareFeaturesData(QList<matrix *> &mxlst,
                                  QList<QStringList> &objnamelst) {
  LDAMODEL *lda = getModel();
  if (!lda) return;

  uivector *classid = lda->classid;
  uint cc = 0;
  for (uint k = 0; k < lda->features->order; k++) {
    objnamelst.append(QStringList());
    mxlst.append(lda->features->m[k]);
    for (uint i = 0; i < lda->features->m[k]->row; i++) {
      objnamelst.last().append(
          projects->value(pid)->getLDAModel(mid)->getObjName()[classid->data[cc]]);
      cc++;
    }
  }
}

QList<QColor> LDAPlot::getColorList(int size) const {
  QList<int> QtColours = {2, 7, 9, 8, 10, 11, 12, 5, 16, 17, 18, 4, 6, 13, 14, 15};
  QList<QColor> colors;
  for (int i = 0; i < size; i++) {
    if (i < QtColours.size()) {
      colors.append(((Qt::GlobalColor)QtColours[i]));
    } else {
      colors.append(QColor(randInt(0, 255), randInt(0, 255), randInt(0, 255)));
    }
  }
  return colors;
}

void LDAPlot::prepareMNPDFData(QList<matrix *> &mnpdf, QStringList &mnpdfname,
                               QList<QColor> &colors) {
  LDAMODEL *lda = getModel();
  if (!lda) return;

  size_t npoints = 500;
  colors = getColorList(lda->features->order);

  for (uint k = 0; k < lda->features->order; k++) {
    mnpdf.append(new matrix);
    NewMatrix(&mnpdf.last(), npoints, lda->evect->col * 2);
    mnpdfname.append(QString("mnpdf_%1").arg(k + 1));
  }

  double ldmin = 999999999999;
  double ldmax = -999999999999;
  for (uint l = 0; l < lda->evect->col; l++) {
    for (uint k = 0; k < lda->features->order; k++) {
      dvector *ldfeature;
      NewDVector(&ldfeature, lda->features->m[k]->row);
      for (uint i = 0; i < ldfeature->size; i++) {
        ldfeature->data[i] = lda->features->m[k]->data[i][l];
      }
      double tmpldmin, tmpldmax;
      DVectorMinMax(ldfeature, &tmpldmin, &tmpldmax);
      if (tmpldmin < ldmin) ldmin = tmpldmin;
      if (tmpldmax > ldmax) ldmax = tmpldmax;
      DelDVector(&ldfeature);
    }
  }

  uint xcol = 0;
  uint ycol = 1;
  for (uint l = 0; l < lda->evect->col; l++) {
    for (uint k = 0; k < lda->features->order; k++) {
      dvector *ldfeature;
      NewDVector(&ldfeature, lda->features->m[k]->row);
      for (uint i = 0; i < ldfeature->size; i++) {
        ldfeature->data[i] = lda->features->m[k]->data[i][l];
      }
      double mean = 0.f, sdev = 0.f;
      DVectorMean(ldfeature, &mean);
      DVectorSDEV(ldfeature, &sdev);

      double dx = (ldmax - ldmin) / (double)npoints;
      double x = ldmin;
      for (uint i = 0; i < npoints; i++) {
        mnpdf[k]->data[i][xcol] = x;
        mnpdf[k]->data[i][ycol] = 1. / sqrt(2 * _pi_ * sdev) *
                                  exp(-square((x - mean) / sdev) / 2.f);
        x += dx;
      }
      DelDVector(&ldfeature);
    }
    xcol += 2;
    ycol += 2;
  }
}
