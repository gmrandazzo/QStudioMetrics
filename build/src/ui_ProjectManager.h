/********************************************************************************
** Form generated from reading UI file 'ProjectManager.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROJECTMANAGER_H
#define UI_PROJECTMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_ProjectManager
{
public:
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancel;
    QPushButton *finish;
    QLabel *label;
    QTableView *tableView;
    QPushButton *removeButton;
    QPushButton *addFileButton;
    QPushButton *addImageAsRGB;
    QSpacerItem *verticalSpacer;
    QLineEdit *lineEdit;
    QPushButton *addImageListButton;
    QPushButton *pushButton;
    QFrame *line;

    void setupUi(QDialog *ProjectManager)
    {
        if (ProjectManager->objectName().isEmpty())
            ProjectManager->setObjectName(QString::fromUtf8("ProjectManager"));
        ProjectManager->setWindowModality(Qt::ApplicationModal);
        ProjectManager->resize(590, 451);
        ProjectManager->setMinimumSize(QSize(550, 274));
        ProjectManager->setMaximumSize(QSize(1000000, 1000000));
        ProjectManager->setModal(true);
        gridLayout = new QGridLayout(ProjectManager);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        cancel = new QPushButton(ProjectManager);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(100, 32));
        cancel->setMaximumSize(QSize(100, 32));

        gridLayout_3->addWidget(cancel, 0, 1, 1, 1);

        finish = new QPushButton(ProjectManager);
        finish->setObjectName(QString::fromUtf8("finish"));
        finish->setMinimumSize(QSize(100, 32));
        finish->setMaximumSize(QSize(100, 32));

        gridLayout_3->addWidget(finish, 0, 2, 1, 1);


        gridLayout->addLayout(gridLayout_3, 8, 0, 1, 3);

        label = new QLabel(ProjectManager);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(100, 16));
        label->setMaximumSize(QSize(100, 16));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        tableView = new QTableView(ProjectManager);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setEnabled(true);
        tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout->addWidget(tableView, 1, 0, 7, 2);

        removeButton = new QPushButton(ProjectManager);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        removeButton->setMinimumSize(QSize(170, 32));
        removeButton->setMaximumSize(QSize(170, 32));

        gridLayout->addWidget(removeButton, 6, 2, 1, 1);

        addFileButton = new QPushButton(ProjectManager);
        addFileButton->setObjectName(QString::fromUtf8("addFileButton"));
        addFileButton->setMinimumSize(QSize(170, 32));
        addFileButton->setMaximumSize(QSize(170, 32));

        gridLayout->addWidget(addFileButton, 1, 2, 1, 1);

        addImageAsRGB = new QPushButton(ProjectManager);
        addImageAsRGB->setObjectName(QString::fromUtf8("addImageAsRGB"));
        addImageAsRGB->setMinimumSize(QSize(170, 32));
        addImageAsRGB->setMaximumSize(QSize(170, 32));

        gridLayout->addWidget(addImageAsRGB, 2, 2, 1, 1);

        verticalSpacer = new QSpacerItem(20, 180, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 7, 2, 1, 1);

        lineEdit = new QLineEdit(ProjectManager);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 1, 1, 2);

        addImageListButton = new QPushButton(ProjectManager);
        addImageListButton->setObjectName(QString::fromUtf8("addImageListButton"));
        addImageListButton->setMinimumSize(QSize(170, 32));
        addImageListButton->setMaximumSize(QSize(170, 32));

        gridLayout->addWidget(addImageListButton, 5, 2, 1, 1);

        pushButton = new QPushButton(ProjectManager);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setMinimumSize(QSize(170, 32));
        pushButton->setMaximumSize(QSize(170, 32));

        gridLayout->addWidget(pushButton, 3, 2, 1, 1);

        line = new QFrame(ProjectManager);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 4, 2, 1, 1);


        retranslateUi(ProjectManager);

        QMetaObject::connectSlotsByName(ProjectManager);
    } // setupUi

    void retranslateUi(QDialog *ProjectManager)
    {
        ProjectManager->setWindowTitle(QCoreApplication::translate("ProjectManager", "CreateNew  Project", nullptr));
        cancel->setText(QCoreApplication::translate("ProjectManager", "Cancel", nullptr));
        finish->setText(QCoreApplication::translate("ProjectManager", "Finish", nullptr));
        label->setText(QCoreApplication::translate("ProjectManager", "Project Name:", nullptr));
        removeButton->setText(QCoreApplication::translate("ProjectManager", "Remove", nullptr));
        addFileButton->setText(QCoreApplication::translate("ProjectManager", "Import Table", nullptr));
        addImageAsRGB->setText(QCoreApplication::translate("ProjectManager", "Import Images", nullptr));
        addImageListButton->setText(QCoreApplication::translate("ProjectManager", "Add Obj/Var Depiction ", nullptr));
        pushButton->setText(QCoreApplication::translate("ProjectManager", "Import Text", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ProjectManager: public Ui_ProjectManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROJECTMANAGER_H
