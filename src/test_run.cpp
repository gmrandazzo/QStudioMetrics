#include "run.h"
#include <iostream>
#include <cassert>
#include <QCoreApplication>
#include <QFuture>
#include "qstudiometricstypes.h"

// Stubs for MainWindow static methods used by qsmdata.cpp
#include "MainWindow.h"
#include <QTreeWidgetItem>

QTreeWidgetItem *MainWindow::getProjectItem(int pid, QTreeWidget *treeWidget) {
    Q_UNUSED(pid); Q_UNUSED(treeWidget); return nullptr;
}
QTreeWidgetItem *MainWindow::getModelItem(int pid, int mid, QTreeWidget *treeWidget) {
    Q_UNUSED(pid); Q_UNUSED(mid); Q_UNUSED(treeWidget); return nullptr;
}

// Utility to create a dummy matrix
matrix* createDummyMatrix(uint rows, uint cols) {
    matrix* m;
    initMatrix(&m);
    ResizeMatrix(m, rows, cols);
    for(uint i=0; i<rows; ++i)
        for(uint j=0; j<cols; ++j)
            setMatrixValue(m, i, j, (double)(i + j));
    return m;
}

// Utility to create a dummy tensor
tensor* createDummyTensor(uint order, uint rows, uint cols) {
    tensor* t;
    initTensor(&t);
    for(uint i=0; i<order; ++i)
        AddTensorMatrix(t, rows, cols);
    for(uint k=0; k<order; ++k)
        for(uint i=0; i<rows; ++i)
            for(uint j=0; j<cols; ++j)
                setTensorValue(t, k, i, j, (double)(k + i + j));
    return t;
}

void test_PCA() {
    std::cout << "Testing PCA..." << std::endl;
    RUN runner;
    matrix* x = createDummyMatrix(10, 5);
    PCAModel model;
    model.setNPC(2);
    runner.setXMatrix(x);
    runner.setPCAModel(&model);
    runner.setNumberPC(2);
    runner.RunPCA().waitForFinished();
    assert(model.Model()->scores != nullptr);
    
    std::cout << "Testing PCA Prediction..." << std::endl;
    model.addPCAPrediction();
    matrix* x_pred = createDummyMatrix(3, 5);
    runner.setXMatrix(x_pred);
    runner.RunPCAPrediction().waitForFinished();
    assert(model.getLastPCAPrediction()->getPredScores() != nullptr);
    
    DelMatrix(&x);
    DelMatrix(&x_pred);
}

void test_CPCA() {
    std::cout << "Testing CPCA..." << std::endl;
    RUN runner;
    tensor* ax = createDummyTensor(3, 10, 5);
    CPCAModel model;
    model.setNPC(2);
    runner.setXTensor(ax);
    runner.setCPCAModel(&model);
    runner.setNumberPC(2);
    runner.RunCPCA().waitForFinished();
    assert(model.Model()->super_scores != nullptr);

    std::cout << "Testing CPCA Prediction..." << std::endl;
    model.addCPCAPrediction();
    tensor* ax_pred = createDummyTensor(3, 2, 5);
    runner.setXTensor(ax_pred);
    runner.RunCPCAPrediction().waitForFinished();
    assert(model.getLastCPCAPrediction()->getPredSuperScores() != nullptr);

    DelTensor(&ax);
    DelTensor(&ax_pred);
}

void test_PLS() {
    std::cout << "Testing PLS..." << std::endl;
    RUN runner;
    matrix* x = createDummyMatrix(10, 5);
    matrix* y = createDummyMatrix(10, 1);
    PLSModel model;
    model.setNPC(1);
    runner.setXMatrix(x);
    runner.setYMatrix(y);
    runner.setPLSModel(&model);
    runner.setNumberPC(1);
    runner.RunPLS(PLS_).waitForFinished();
    assert(model.Model()->xscores != nullptr);

    std::cout << "Testing PLS Validation..." << std::endl;
    runner.setValidationType(LOO_);
    runner.RunPLSValidation(PLS_).waitForFinished();
    assert(model.Model()->predicted_y != nullptr);

    std::cout << "Testing PLS Prediction..." << std::endl;
    model.addPLSPrediction();
    matrix* x_pred = createDummyMatrix(3, 5);
    runner.setXMatrix(x_pred);
    runner.setYMatrix(nullptr);
    runner.RunPLSPrediction().waitForFinished();
    assert(model.getLastPLSPrediction()->getYDipVar() != nullptr);

    DelMatrix(&x);
    DelMatrix(&y);
    DelMatrix(&x_pred);
}

void test_MLR() {
    std::cout << "Testing MLR..." << std::endl;
    RUN runner;
    matrix* x = createDummyMatrix(10, 3);
    matrix* y = createDummyMatrix(10, 1);
    MLRModel model;
    runner.setXMatrix(x);
    runner.setYMatrix(y);
    runner.setMLRModel(&model);
    runner.RunMLR().waitForFinished();
    assert(model.Model()->b != nullptr);

    std::cout << "Testing MLR Validation..." << std::endl;
    runner.setValidationType(LOO_);
    runner.RunMLRValidation().waitForFinished();
    assert(model.Model()->predicted_y != nullptr);

    std::cout << "Testing MLR Prediction..." << std::endl;
    model.addMLRPrediction();
    matrix* x_pred = createDummyMatrix(2, 3);
    runner.setXMatrix(x_pred);
    runner.setYMatrix(nullptr);
    runner.RunMLRPrediction().waitForFinished();
    assert(model.getLastMLRPrediction()->getYDipVar() != nullptr);

    DelMatrix(&x);
    DelMatrix(&y);
    DelMatrix(&x_pred);
}

void test_LDA() {
    std::cout << "Testing LDA..." << std::endl;
    RUN runner;
    matrix* x = createDummyMatrix(10, 5);
    matrix* y = createDummyMatrix(10, 1);
    // Y must have class labels
    for(uint i=0; i<5; i++) setMatrixValue(y, i, 0, 0.0);
    for(uint i=5; i<10; i++) setMatrixValue(y, i, 0, 1.0);

    LDAModel model;
    runner.setXMatrix(x);
    runner.setYMatrix(y);
    runner.setLDAModel(&model);
    runner.RunLDA().waitForFinished();
    assert(model.Model()->mu != nullptr);

    std::cout << "Testing LDA Validation..." << std::endl;
    runner.setValidationType(LOO_);
    runner.RunLDAValidation().waitForFinished();
    assert(model.Model()->predicted_y != nullptr);

    std::cout << "Testing LDA Prediction..." << std::endl;
    model.addLDAPrediction();
    matrix* x_pred = createDummyMatrix(2, 5);
    runner.setXMatrix(x_pred);
    runner.setYMatrix(nullptr);
    runner.RunLDAPrediction().waitForFinished();
    assert(model.getLastLDAPrediction()->getPredClasses() != nullptr);

    DelMatrix(&x);
    DelMatrix(&y);
    DelMatrix(&x_pred);
}

void test_Selections() {
    std::cout << "Testing MDC Selection..." << std::endl;
    RUN runner;
    matrix* m = createDummyMatrix(20, 5);
    uivector* uiv;
    initUIVector(&uiv);
    runner.setMatrix(m);
    runner.setUIVector(uiv);
    runner.setNumberOfObject(5);
    runner.setMetric(EUCLIDEANDISTANCE);
    
    runner.RunMDCSelection().waitForFinished();
    assert(uiv->size == 5);

    std::cout << "Testing MaxDis Selection..." << std::endl;
    DelUIVector(&uiv);
    initUIVector(&uiv);
    runner.setUIVector(uiv);
    runner.RunMaxDisSelection().waitForFinished();
    assert(uiv->size == 5);

    std::cout << "Testing Random Selection..." << std::endl;
    DelUIVector(&uiv);
    initUIVector(&uiv);
    runner.setUIVector(uiv);
    runner.setNumberMaxOfObject(20);
    runner.setNumberOfObject(5);
    runner.RunRandomSelection().waitForFinished();
    assert(uiv->size == 5);

    DelMatrix(&m);
    DelUIVector(&uiv);
}

void test_Clustering() {
    std::cout << "Testing Clustering..." << std::endl;
    RUN runner;
    matrix* m = createDummyMatrix(20, 5);
    uivector* uiv;
    initUIVector(&uiv);
    runner.setMatrix(m);
    runner.setUIVector(uiv);
    runner.setNumberOfCluster(3);
    runner.setClusteringAlgorithm(0); // KMeans
    runner.setObjectSelectionType(3); // Get all
    
    runner.RunClustering().waitForFinished();
    assert(uiv->size == 20);

    std::cout << "Testing Cluster Validation..." << std::endl;
    dvector* dv;
    initDVector(&dv);
    runner.setDVector(dv);
    runner.setNMaxClusters(5);
    runner.setClusteringAlgorithm(0);
    runner.setValidationType(JUMPMETHOD);
    runner.RunClusterValidation().waitForFinished();
    assert(dv->size > 0);

    DelMatrix(&m);
    DelUIVector(&uiv);
    DelDVector(&dv);
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);
    try {
        test_PCA();
        test_CPCA();
        test_PLS();
        test_MLR();
        test_LDA();
        test_Selections();
        test_Clustering();
        std::cout << "\nAll Comprehensive RUN tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
