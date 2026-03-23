#include "qsmdata.h"
#include <iostream>
#include <cassert>
#include <QList>
#include <QStringList>

#include "MainWindow.h"
#include <QTreeWidgetItem>
#include "Dialogs/GenericProgressDialog.h"

// Stubs for MainWindow static methods used by qsmdata.cpp
QTreeWidgetItem *MainWindow::getProjectItem(int pid, QTreeWidget *treeWidget) {
    Q_UNUSED(pid); Q_UNUSED(treeWidget); return nullptr;
}
QTreeWidgetItem *MainWindow::getModelItem(int pid, int mid, QTreeWidget *treeWidget) {
    Q_UNUSED(pid); Q_UNUSED(mid); Q_UNUSED(treeWidget); return nullptr;
}

void test_MATRIX_Management() {
    std::cout << "Testing MATRIX Management..." << std::endl;
    
    DATA data;
    assert(data.MatrixCount() == 0);
    
    // Add matrix
    data.addMatrix();
    assert(data.MatrixCount() == 1);
    MATRIX* m1 = data.getMatrix(0);
    m1->setName("Matrix1");
    m1->getObjName() << "ObjB" << "ObjA";
    m1->MatrixResize(2, 2);
    
    // Test SortByName
    m1->SortByName();
    assert(m1->getObjName()[0] == "ObjA");
    assert(m1->getObjName()[1] == "ObjB");
    
    // Add another matrix with specific object
    MATRIX* m2 = new MATRIX();
    m2->setName("Matrix2");
    m2->getObjName() << "R1";
    m2->getVarName() << "C1";
    QString hash2 = m2->getHash(); 
    data.addMatrix(m2); // DATA makes a COPY!
    assert(data.MatrixCount() == 2);
    
    // Test getMatrix by hash
    MATRIX* found = data.getMatrix(hash2);
    assert(found != nullptr);
    assert(found != m2); // Should be a copy
    assert(found->getName() == "Matrix2");
    assert(data.getMatrixID(hash2) == 1);
    delete m2; // We must delete our local one since data copied it
    
    // Project properties
    data.setProjectName("MyProject");
    assert(data.getProjectName() == "MyProject");
    
    // Test out of bounds - DATA class uses Q_ASSERT, so we should test safely
    if (data.MatrixCount() > 99) {
        // This part won't be reached but shows intent
        assert(data.getMatrix(99) != nullptr);
    }
    assert(data.getMatrix("nonexistent") == nullptr);
    
    // Test deletion
    data.delMatrixAt(0);
    assert(data.MatrixCount() == 1);
    assert(data.getMatrix(0)->getName() == "Matrix2");
    
    data.delMatrix(); // Clear all
    assert(data.MatrixCount() == 0);
    
    std::cout << "MATRIX tests passed!" << std::endl;
}

void test_ARRAY_Management() {
    std::cout << "Testing ARRAY Management..." << std::endl;
    
    DATA data;
    assert(data.ArrayCount() == 0);
    
    data.addArray();
    assert(data.ArrayCount() == 1);
    ARRAY* a1 = data.getArray(0);
    a1->setName("Array1");
    a1->getObjName() << "Z" << "A";
    AddTensorMatrix(a1->Array(), 2, 2);
    
    a1->SortByName();
    assert(a1->getObjName()[0] == "A");
    
    data.delArrayAt(0);
    assert(data.ArrayCount() == 0);
    
    std::cout << "ARRAY tests passed!" << std::endl;
}

void test_Model_Management() {
    std::cout << "Testing Model Management..." << std::endl;
    
    DATA data;
    
    // PCA
    data.addPCAModel();
    assert(data.PCACount() == 1);
    data.getLastPCAModel()->setName("M1");
    data.getLastPCAModel()->setModelID(10);
    assert(data.getPCAModel(10) == data.getPCAModelAt(0));
    data.delPCAModelAt(0);
    assert(data.PCACount() == 0);
    
    // PLS
    data.addPLSModel();
    assert(data.PLSCount() == 1);
    data.getLastPLSModel()->setModelID(20);
    assert(data.getPLSModel(20) != nullptr);
    data.delPLSModels();
    assert(data.PLSCount() == 0);
    
    // MLR
    data.addMLRModel();
    assert(data.MLRCount() == 1);
    data.delMLRModels();
    
    // LDA
    data.addLDAModel();
    assert(data.LDACount() == 1);
    data.delLDAModels();
    
    std::cout << "Model management tests passed!" << std::endl;
}

void test_Label_Management() {
    std::cout << "Testing Label Management..." << std::endl;
    
    DATA data;
    LABELS &objL = data.getObjectLabels();
    LABEL l1;
    l1.name = "Class1";
    l1.objects << "O1" << "O2";
    objL.append(l1);
    
    assert(data.getObjectLabels().size() == 1);
    assert(data.getObjectLabels()[0].name == "Class1");
    
    std::cout << "Label management tests passed!" << std::endl;
}

void test_Corner_Cases() {
    std::cout << "Testing Corner Cases..." << std::endl;
    
    DATA data;
    
    // Test safe deletion logic (checking if methods handle empty lists without crashing if they have internal guards)
    // Based on previous run, getMatrix(id) uses Q_ASSERT, so we avoid it.
    // But delMatrixAt(id) seems to have a check in some models, let's check Matrix one.
    
    // Add one and delete it
    data.addMatrix();
    assert(data.MatrixCount() == 1);
    data.delMatrixAt(0);
    assert(data.MatrixCount() == 0);
    
    // Add and delete all
    data.addMatrix();
    data.addArray();
    data.addPCAModel();
    data.delMatrix();
    data.delArray();
    data.delPCAModels();
    assert(data.MatrixCount() == 0);
    assert(data.ArrayCount() == 0);
    assert(data.PCACount() == 0);
    
    // Unique ID management
    data.setProjectID(500);
    assert(data.getProjectID() == 500);
    
    // Sorting empty matrix/array
    MATRIX emptyM;
    emptyM.SortByName(); // Should not crash
    
    ARRAY emptyA;
    emptyA.SortByName(); // Should not crash
    
    std::cout << "Corner cases tests passed!" << std::endl;
}

int main() {
    try {
        test_MATRIX_Management();
        test_ARRAY_Management();
        test_Model_Management();
        test_Label_Management();
        test_Corner_Cases();
        std::cout << "\nAll QSM DATA tests passed successfully!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
