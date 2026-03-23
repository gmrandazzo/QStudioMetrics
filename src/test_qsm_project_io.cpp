#include "Dialogs/GenericProgressDialog.h"
#include "qsmdata.h"
#include <iostream>
#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDir>
#include <cassert>
#include <QTemporaryFile>

#include "MainWindow.h"
#include <QTreeWidgetItem>

// We still need to mock MainWindow static methods used by qsmdata.cpp
QTreeWidgetItem *MainWindow::getProjectItem(int pid, QTreeWidget *treeWidget) {
    Q_UNUSED(pid);
    Q_UNUSED(treeWidget);
    return nullptr;
}

QTreeWidgetItem *MainWindow::getModelItem(int pid, int mid, QTreeWidget *treeWidget) {
    Q_UNUSED(pid);
    Q_UNUSED(mid);
    Q_UNUSED(treeWidget);
    return nullptr;
}

int main(int argc, char **argv) {
    QCoreApplication app(argc, argv);

    std::cout << "Testing QSM Project SQL IO..." << std::endl;

    QString fullDbPath;
    QString projectDir;
    QString projectName = "TestProject";

    {
        QTemporaryFile tempFile;
        tempFile.setAutoRemove(false);
        if (!tempFile.open()) {
            std::cerr << "Could not create temporary file" << std::endl;
            return 1;
        }
        projectDir = QFileInfo(tempFile.fileName()).absolutePath();
        fullDbPath = projectDir + "/" + projectName + ".qsm";
        tempFile.close();
        QFile::remove(tempFile.fileName());
    }
    
    // Cleanup any existing
    if (QFile::exists(fullDbPath)) QFile::remove(fullDbPath);

    {
        DATA data;
        data.setProjectName(projectName);
        data.setProjectID(1);
        
        // Add a matrix
        data.addMatrix();
        MATRIX* m = data.getMatrix(0);
        m->setName("TestMatrix");
        m->MatrixResize(3, 3);
        for(uint i=0; i<3; ++i) {
            for(uint j=0; j<3; ++j) {
                setMatrixValue(m->Matrix(), i, j, (double)(i*3 + j));
            }
        }
        m->getObjName() << "R1" << "R2" << "R3";
        m->getVarName() << "C1" << "C2" << "C3";

        // Add a PCA model
        data.addPCAModel();
        PCAModel* pca = data.getLastPCAModel();
        pca->setName("TestPCA");
        pca->setNPC(2);
        pca->setDataHash(m->getHash());
        pca->setDID(0);
        
        // Initialize model structures
        NewMatrix(&(pca->Model()->scores), 3, 2);
        NewMatrix(&(pca->Model()->loadings), 3, 2);
        NewMatrix(&(pca->Model()->dmodx), 3, 1);
        NewDVector(&(pca->Model()->varexp), 2);
        NewDVector(&(pca->Model()->colscaling), 3);
        NewDVector(&(pca->Model()->colaverage), 3);
        
        setMatrixValue(pca->Model()->scores, 0, 0, 1.23);
        setDVectorValue(pca->Model()->varexp, 0, 0.85);

        // Add a validated PLS model
        data.addPLSModel();
        PLSModel* pls = data.getLastPLSModel();
        pls->setName("TestPLS");
        pls->setNPC(2);
        pls->setDataHash(m->getHash());
        pls->setDID(0);
        pls->setValidation(1); // LOO_

        // Initialize validation structures
        initMatrix(&pls->Model()->xscores);
        initMatrix(&pls->Model()->predicted_y);
        initMatrix(&pls->Model()->q2y);
        initMatrix(&pls->Model()->r2y_recalculated);
        
        ResizeMatrix(pls->Model()->xscores, 3, 2);
        ResizeMatrix(pls->Model()->predicted_y, 3, 1);
        ResizeMatrix(pls->Model()->q2y, 2, 1);
        ResizeMatrix(pls->Model()->r2y_recalculated, 2, 1);
        
        setMatrixValue(pls->Model()->q2y, 0, 0, 0.75);
        setMatrixValue(pls->Model()->predicted_y, 0, 0, 5.5);

        std::cout << "Saving project to " << fullDbPath.toStdString() << std::endl;
        // After refactoring, SaveSQLData takes an optional pbdialog
        QString savedPath = data.SaveSQLData(projectDir, nullptr);
        assert(!savedPath.isEmpty());
        assert(QFile::exists(fullDbPath));
    }

    {
        std::cout << "Loading project from " << fullDbPath.toStdString() << std::endl;
        DATA data;
        data.setProjectID(1);
        int tabcount = 0;
        int mid = 0;
        QStringList log;
        
        // treeWidget is optional (we added NULL checks in refactoring)
        data.OpenSQLData(fullDbPath, nullptr, &tabcount, &mid, &log);
        
        assert(data.MatrixCount() == 1);
        assert(data.getMatrix(0)->getName() == "TestMatrix");
        assert(data.getMatrix(0)->Matrix()->row == 3);
        assert(getMatrixValue(data.getMatrix(0)->Matrix(), 1, 1) == 4.0);
        
        assert(data.PCACount() == 1);
        assert(data.getPCAModelAt(0)->getName() == "TestPCA");
        assert(data.getPCAModelAt(0)->getNPC() == 2);
        assert(getMatrixValue(data.getPCAModelAt(0)->Model()->scores, 0, 0) == 1.23);
        assert(getDVectorValue(data.getPCAModelAt(0)->Model()->varexp, 0) == 0.85);
        
        assert(data.PLSCount() == 1);
        assert(data.getPLSModelAt(0)->getName() == "TestPLS");
        assert(data.getPLSModelAt(0)->getValidation() == 1);
        assert(data.getPLSModelAt(0)->Model()->q2y != nullptr);
        assert(getMatrixValue(data.getPLSModelAt(0)->Model()->q2y, 0, 0) == 0.75);
        assert(getMatrixValue(data.getPLSModelAt(0)->Model()->predicted_y, 0, 0) == 5.5);
        
        std::cout << "Load successful!" << std::endl;
    }

    QFile::remove(fullDbPath);
    
    std::cout << "All Project IO tests passed!" << std::endl;
    return 0;
}
