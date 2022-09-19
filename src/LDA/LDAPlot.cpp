#include "LDAPlot.h"
#include <unistd.h>

void LDAPlot::FeaturePlot2D(ScatterPlot** plot2D)
{
  if(mid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

    QList<matrix*> mxlst;
    QList<QStringList> objnamelst;
    uivector *classid = projects->value(pid)->getLDAModel(mid)->Model()->classid;
    uint cc = 0;
    for(uint k = 0; k < projects->value(pid)->getLDAModel(mid)->Model()->features->order; k++){
      objnamelst.append(QStringList());
      mxlst.append(projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]);
      for(uint i = 0; i < projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]->row; i++){
        objnamelst.last().append(projects->value(pid)->getLDAModel(mid)->getObjName()[classid->data[cc]]);
        cc++;
      }
    }

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    (*plot2D) = new ScatterPlot(mxlst, objnamelst,  &projects->value(pid)->getMATRIXList(), xhash, yhash,  &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "LD", "LD", QString(projectname + modelname + " - LDA Feature Plot "), ScatterPlot::SCORES);
    (*plot2D)->setPID(pid);
  }
}

void LDAPlot::FeaturePlot3D(ScatterPlot** plot3D)
{
  if(mid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

    QList<matrix*> mxlst;
    QList<QStringList> objnamelst;
    uivector *classid = projects->value(pid)->getLDAModel(mid)->Model()->classid;
    uint cc = 0;
    for(uint k = 0; k < projects->value(pid)->getLDAModel(mid)->Model()->features->order; k++){
      objnamelst.append(QStringList());
      mxlst.append(projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]);
      for(uint i = 0; i < projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]->row; i++){
        objnamelst.last().append(projects->value(pid)->getLDAModel(mid)->getObjName()[classid->data[cc]]);
        cc++;
      }
    }
    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    (*plot3D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - LDA Feature Plot").arg(projectname).arg(modelname), "LD", "LD", "LD", ScatterPlot::SCORES);
    (*plot3D)->setPID(pid);
  }
}

void LDAPlot::ProbabilityDistribution(ScatterPlot** plot2D)
{
  if(mid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

    QList<matrix*> mx, my;
    QList<QStringList> objnamelst;
    uivector *classid = projects->value(pid)->getLDAModel(mid)->Model()->classid;
    uint cc = 0;
    for(uint k = 0; k < projects->value(pid)->getLDAModel(mid)->Model()->features->order; k++){
      objnamelst.append(QStringList());
      mx.append(projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]);
      my.append(projects->value(pid)->getLDAModel(mid)->Model()->mnpdf->m[k]);

      for(uint i = 0; i < projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]->row; i++){
        objnamelst.last().append(projects->value(pid)->getLDAModel(mid)->getObjName()[classid->data[cc]]);
        cc++;
      }
    }

    /*
     * AGGIUNGERE DUE MATRIXI X E Y:
     * X CI SONO I VALORI DA MINIMO A MASSIMO DELLE CLASSI DEGLI OGGETTI
     * Y I VALORI DI MVNDISTRIB
     */

    QList< matrix * > mnpdf;
    QStringList mnpdfname;
    QList <QColor> colors;
    LDAMODEL *lda = projects->value(pid)->getLDAModel(mid)->Model();

    QList<int> QtColours;
    QtColours.append(2);
    QtColours.append(7);
    QtColours.append(9);
    QtColours.append(8);
    QtColours.append(10);
    QtColours.append(11);
    QtColours.append(12);
    QtColours.append(5);
    QtColours.append(16);
    QtColours.append(17);
    QtColours.append(18);
    QtColours.append(4);
    QtColours.append(6);
    QtColours.append(13);
    QtColours.append(14);
    QtColours.append(15);

    size_t npoints = 500;
    for(uint k = 0; k < lda->features->order; k++){
      mnpdf.append(new matrix);
      NewMatrix(&mnpdf.last(), npoints, lda->evect->col*2);
      mnpdfname.append(QString("mnpdf_%1").arg(k+1));
      if((int)k < QtColours.size()){
        colors.append(((Qt::GlobalColor)QtColours[k]));
      }
      else{
        colors.append(QColor(randInt(0, 255), randInt(0, 255), randInt(0, 255)));
      }
    }

    double ldmin, ldmax;
    ldmin = 999999999999;
    ldmax = -999999999999;
    for(uint l = 0; l < lda->evect->col; l++){ // for each ldfeature
      for(uint k = 0; k < lda->features->order; k++){ // for each class of objects...
        dvector *ldfeature;
        NewDVector(&ldfeature, lda->features->m[k]->row);
        for(uint i = 0; i < ldfeature->size; i++){
          ldfeature->data[i] = lda->features->m[k]->data[i][l]; // copy the feature
        }
        double tmpldmin, tmpldmax;
        DVectorMinMax(ldfeature, &tmpldmin, &tmpldmax);
        if(tmpldmin < ldmin)
          ldmin = tmpldmin;

        if(tmpldmax > ldmax)
          ldmax = tmpldmax;

        DelDVector(&ldfeature);
      }
    }

    uint xcol = 0; uint ycol = 1;
    for(uint l = 0; l < lda->evect->col; l++){ // for each ldfeature
      for(uint k = 0; k < lda->features->order; k++){ // for each class of objects...
        dvector *ldfeature;
        NewDVector(&ldfeature, lda->features->m[k]->row);
        for(uint i = 0; i < ldfeature->size; i++){
          ldfeature->data[i] = lda->features->m[k]->data[i][l]; // copy the feature
        }
//        Calculate the multivariate normal distribution
        double mean = 0.f, sdev = 0.f;
        DVectorMean(ldfeature, &mean);
        DVectorSDEV(ldfeature, &sdev);

        double dx = (ldmax-ldmin)/(double)npoints;
        double x = ldmin;
        for(uint i = 0; i < npoints; i++){
          mnpdf[k]->data[i][xcol] = x;
          mnpdf[k]->data[i][ycol] =  1./sqrt(2 * _pi_* sdev) * exp(-square((x - mean)/sdev)/2.f);
          x += dx;
        }
        DelDVector(&ldfeature);
      }
      xcol +=2;
      ycol +=2;
    }

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());

    (*plot2D) = new ScatterPlot(mx, my, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "Y", "Prob Y", projectname + modelname + " - LDA Multivariate Normal Distribution of Probabilities", ScatterPlot::SCORES);

    (*plot2D)->addCurve(mnpdf, mnpdfname, colors);
    (*plot2D)->setPID(pid);

    for(uint k = 0; k < lda->features->order; k++){
      DelMatrix(&mnpdf[k]);
    }
  }
}

QList< SimpleLine2DPlot* > LDAPlot::ROCPlot()
{
  QList< SimpleLine2DPlot* > plots;
  if(mid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();
    
    matrix *m;

    LDAMODEL *lda = projects->value(pid)->getLDAModel(mid)->Model();
    QStringList curvenames;
    curvenames << "Current classifier" << "Random classifier";

    for(size_t k = 0; k < lda->roc->order; k++){
      QString classname = projects->value(pid)->getLDAModel(mid)->getNameClasses()[k];
      NewMatrix(&m, lda->roc->m[k]->row+1, 3);
      for(size_t i = 0; i < lda->roc->m[k]->row; i++){
        m->data[i][0] = lda->roc->m[k]->data[i][0];
        m->data[i][1] = lda->roc->m[k]->data[i][1]; // current classifier
        m->data[i][2] = lda->roc->m[k]->data[i][0]; //random classifier
      }
      m->data[lda->roc->m[k]->row][0] = 1.;
      m->data[lda->roc->m[k]->row][1] = 1.; // current classifier
      m->data[lda->roc->m[k]->row][2] = 1.; //random classifier
      
      #ifdef DEBUG
      qDebug() << "Final Matrix";
      PrintMatrix(m);
      #endif
      plots.append(new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - ROC Plot %3").arg(projectname).arg(modelname).arg(classname), "FP Rate", "TP Rate"));
      plots.last()->setXminXmaxXTick(0, 1., 10);
      plots.last()->setYminYmaxYTick(0, 1., 10);
      DelMatrix(&m);
    }
  }
  return plots;
}

QList< SimpleLine2DPlot* > LDAPlot::PRPlot()
{
  QList< SimpleLine2DPlot* > plots;
  if(mid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();
    
    matrix *m;

    LDAMODEL *lda = projects->value(pid)->getLDAModel(mid)->Model();
    QStringList curvenames;
    curvenames << "Current classifier" << "Random classifier";

    for(size_t k = 0; k < lda->pr->order; k++){
      QString classname = projects->value(pid)->getLDAModel(mid)->getNameClasses()[k];
      NewMatrix(&m, lda->pr->m[k]->row+1, 3);
      for(size_t i = 0; i < lda->pr->m[k]->row; i++){
        m->data[i][0] = lda->pr->m[k]->data[i][0];
        m->data[i][1] = lda->pr->m[k]->data[i][1]; // current classifier
        m->data[i][2] = 1-lda->pr->m[k]->data[i][0]; //random classifier
      }
      m->data[lda->pr->m[k]->row][0] = 1.;
      m->data[lda->pr->m[k]->row][1] = 0.;
      m->data[lda->pr->m[k]->row][2] = 0.; 
      #ifdef DEBUG
      qDebug() << "Final Matrix";
      PrintMatrix(m);
      #endif
      plots.append(new SimpleLine2DPlot(m, curvenames, QString(" %1 - %2 - ROC Plot %3").arg(projectname).arg(modelname).arg(classname), "FP Rate", "TP Rate"));
      plots.last()->setXminXmaxXTick(0, 1., 10);
      plots.last()->setYminYmaxYTick(0, 1., 10);
      DelMatrix(&m);
    }
  }
  return plots;
}
void LDAPlot::FeaturePlotAndPrediction2D(ScatterPlot** plot2D)
{
  if(mid > -1 && predid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

    QList<matrix*> mxlst;
    QList<QStringList> objnamelst;
    uivector *classid = projects->value(pid)->getLDAModel(mid)->Model()->classid;
    uint cc = 0;
    for(uint k = 0; k < projects->value(pid)->getLDAModel(mid)->Model()->features->order; k++){
      mxlst.append(projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]);
      objnamelst.append(QStringList());
      for(uint i = 0; i < projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]->row; i++){
        objnamelst.last().append(projects->value(pid)->getLDAModel(mid)->getObjName()[classid->data[cc]]);
        cc++;
      }
    }

    mxlst.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getPredFeatures());
    objnamelst.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getObjName());

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    xhash.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getDataHash());
    (*plot2D) = new ScatterPlot(mxlst, objnamelst,  &projects->value(pid)->getMATRIXList(), xhash, yhash,  &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "LD", "LD", QString(projectname + modelname + " - LDA Feature Plot "), ScatterPlot::SCORES);
    (*plot2D)->setPID(pid);
  }
}

void LDAPlot::FeaturePlotAndPrediction3D(ScatterPlot** plot3D)
{
  if(mid > -1 && predid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

    QList<matrix*> mxlst;
    QList<QStringList> objnamelst;
    uivector *classid = projects->value(pid)->getLDAModel(mid)->Model()->classid;
    uint cc = 0;
    for(uint k = 0; k < projects->value(pid)->getLDAModel(mid)->Model()->features->order; k++){
      mxlst.append(projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]);
      objnamelst.append(QStringList());
      for(uint i = 0; i < projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]->row; i++){
        objnamelst.last().append(projects->value(pid)->getLDAModel(mid)->getObjName()[classid->data[cc]]);
        cc++;
      }
    }

    mxlst.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getPredFeatures());
    objnamelst.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getObjName());

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    xhash.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getDataHash());
    (*plot3D) = new ScatterPlot(mxlst, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), QString("%1 - %2 - LDA Feature Plot").arg(projectname).arg(modelname), "LD", "LD", "LD", ScatterPlot::SCORES);
    (*plot3D)->setPID(pid);
  }
}

void LDAPlot::ProbabilityDistributionWithPredictions(ScatterPlot** plot2D)
{
  if(mid > -1 && predid > -1){
    QString projectname = projects->value(pid)->getProjectName();
    QString modelname = projects->value(pid)->getLDAModel(mid)->getName();

    QList<matrix*> mx, my;
    QList<QStringList> objnamelst;
    uivector *classid = projects->value(pid)->getLDAModel(mid)->Model()->classid;
    uint cc = 0;
    for(uint k = 0; k < projects->value(pid)->getLDAModel(mid)->Model()->features->order; k++){
      objnamelst.append(QStringList());
      mx.append(projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]);
      my.append(projects->value(pid)->getLDAModel(mid)->Model()->mnpdf->m[k]);

      for(uint i = 0; i < projects->value(pid)->getLDAModel(mid)->Model()->features->m[k]->row; i++){
        objnamelst.last().append(projects->value(pid)->getLDAModel(mid)->getObjName()[classid->data[cc]]);
        cc++;
      }
    }

    mx.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getPredFeatures());
//     my.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getMVNProbDistrib());
    my.append(new matrix);
    NewMatrix(&my.last(), projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getMVNProbDistrib()->row, projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getMVNProbDistrib()->col);
    objnamelst.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getObjName());

    QStringList xhash, yhash;
    xhash.append(projects->value(pid)->getLDAModel(mid)->getDataHash());
    xhash.append(projects->value(pid)->getLDAModel(mid)->getLDAPrediction(predid)->getDataHash());

    QList< matrix * > mnpdf;
    QStringList mnpdfname;
    QList <QColor> colors;
    LDAMODEL *lda = projects->value(pid)->getLDAModel(mid)->Model();

    QList<int> QtColours;
    QtColours.append(2);
    QtColours.append(7);
    QtColours.append(9);
    QtColours.append(8);
    QtColours.append(10);
    QtColours.append(11);
    QtColours.append(12);
    QtColours.append(5);
    QtColours.append(16);
    QtColours.append(17);
    QtColours.append(18);
    QtColours.append(4);
    QtColours.append(6);
    QtColours.append(13);
    QtColours.append(14);
    QtColours.append(15);

    size_t npoints = 500;
    for(uint k = 0; k < lda->features->order; k++){
      mnpdf.append(new matrix);
      NewMatrix(&mnpdf.last(), npoints, lda->evect->col*2);
      mnpdfname.append(QString("mnpdf_%1").arg(k+1));
      if((int)k < QtColours.size()){
        colors.append(((Qt::GlobalColor)QtColours[k]));
      }
      else{
        colors.append(QColor(randInt(0, 255), randInt(0, 255), randInt(0, 255)));
      }
    }

    double ldmin, ldmax;
    ldmin = 999999999999;
    ldmax = -999999999999;
    for(uint l = 0; l < lda->evect->col; l++){ // for each ldfeature
      for(uint k = 0; k < lda->features->order; k++){ // for each class of objects...
        dvector *ldfeature;
        NewDVector(&ldfeature, lda->features->m[k]->row);
        for(uint i = 0; i < ldfeature->size; i++){
          ldfeature->data[i] = lda->features->m[k]->data[i][l]; // copy the feature
        }
        double tmpldmin, tmpldmax;
        DVectorMinMax(ldfeature, &tmpldmin, &tmpldmax);
        if(tmpldmin < ldmin)
          ldmin = tmpldmin;

        if(tmpldmax > ldmax)
          ldmax = tmpldmax;

        DelDVector(&ldfeature);
      }
    }

    uint xcol = 0; uint ycol = 1;
    for(uint l = 0; l < lda->evect->col; l++){ // for each ldfeature
      for(uint k = 0; k < lda->features->order; k++){ // for each class of objects...
        dvector *ldfeature;
        NewDVector(&ldfeature, lda->features->m[k]->row);
        for(uint i = 0; i < ldfeature->size; i++){
          ldfeature->data[i] = lda->features->m[k]->data[i][l]; // copy the feature
        }
//        Calculate the multivariate normal distribution
        double mean = 0.f, sdev = 0.f;
        DVectorMean(ldfeature, &mean);
        DVectorSDEV(ldfeature, &sdev);

        double dx = (ldmax-ldmin)/(double)npoints;
        double x = ldmin;
        for(uint i = 0; i < npoints; i++){
          mnpdf[k]->data[i][xcol] = x;
          mnpdf[k]->data[i][ycol] =  1./sqrt(2 * _pi_* sdev) * exp(-square((x - mean)/sdev)/2.f);
          x += dx;
        }
        DelDVector(&ldfeature);
      }
      xcol +=2;
      ycol +=2;
    }

    (*plot2D) = new ScatterPlot(mx, my, objnamelst, &projects->value(pid)->getMATRIXList(), xhash, yhash, &projects->value(pid)->getObjectLabels(), &projects->value(pid)->getVariableLabels(), "Y", "Prob Y", projectname + modelname + " - LDA Multivariate Normal Distribution of Probabilities", ScatterPlot::SCORES);

    (*plot2D)->addCurve(mnpdf, mnpdfname, colors);
    (*plot2D)->setPID(pid);
    (*plot2D)->PlotUpdate();
    for(uint k = 0; k < lda->features->order; k++){
      DelMatrix(&mnpdf[k]);
    }
  }
}

LDAPlot::LDAPlot(PROJECTS* projects_)
{
  pid = mid = predid = -1;
  projects = projects_;
}
