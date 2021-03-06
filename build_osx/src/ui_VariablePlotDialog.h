/********************************************************************************
** Form generated from reading UI file 'VariablePlotDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VARIABLEPLOTDIALOG_H
#define UI_VARIABLEPLOTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_VariablePlotDialog
{
public:
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_6;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QGroupBox *projectgroupBox;
    QGridLayout *gridLayout_4;
    QListView *projectView;
    QGroupBox *datagroupBox;
    QGridLayout *gridLayout;
    QListView *dataView;
    QGroupBox *objectgroupBox;
    QGridLayout *gridLayout_2;
    QListView *objectsView;
    QGroupBox *variablegroupBox;
    QGridLayout *gridLayout_3;
    QListView *variableView;
    QGroupBox *variablegroupBox_2;
    QGridLayout *gridLayout_5;
    QListView *variableView_2;

    void setupUi(QDialog *VariablePlotDialog)
    {
        if (VariablePlotDialog->objectName().isEmpty())
            VariablePlotDialog->setObjectName(QString::fromUtf8("VariablePlotDialog"));
        VariablePlotDialog->resize(626, 505);
        gridLayout_7 = new QGridLayout(VariablePlotDialog);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(VariablePlotDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(80, 24));
        cancelButton->setMaximumSize(QSize(80, 24));

        gridLayout_6->addWidget(cancelButton, 0, 2, 1, 1);

        okButton = new QPushButton(VariablePlotDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(80, 24));
        okButton->setMaximumSize(QSize(80, 24));

        gridLayout_6->addWidget(okButton, 0, 3, 1, 1);


        gridLayout_7->addLayout(gridLayout_6, 2, 0, 1, 4);

        projectgroupBox = new QGroupBox(VariablePlotDialog);
        projectgroupBox->setObjectName(QString::fromUtf8("projectgroupBox"));
        projectgroupBox->setMinimumSize(QSize(190, 220));
        gridLayout_4 = new QGridLayout(projectgroupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        projectView = new QListView(projectgroupBox);
        projectView->setObjectName(QString::fromUtf8("projectView"));
        projectView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        projectView->setSelectionMode(QAbstractItemView::SingleSelection);

        gridLayout_4->addWidget(projectView, 0, 0, 1, 1);


        gridLayout_7->addWidget(projectgroupBox, 0, 0, 1, 1);

        datagroupBox = new QGroupBox(VariablePlotDialog);
        datagroupBox->setObjectName(QString::fromUtf8("datagroupBox"));
        datagroupBox->setMinimumSize(QSize(190, 220));
        datagroupBox->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(datagroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        dataView = new QListView(datagroupBox);
        dataView->setObjectName(QString::fromUtf8("dataView"));
        dataView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        dataView->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout->addWidget(dataView, 0, 0, 1, 1);


        gridLayout_7->addWidget(datagroupBox, 0, 1, 1, 2);

        objectgroupBox = new QGroupBox(VariablePlotDialog);
        objectgroupBox->setObjectName(QString::fromUtf8("objectgroupBox"));
        objectgroupBox->setMinimumSize(QSize(190, 220));
        gridLayout_2 = new QGridLayout(objectgroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        objectsView = new QListView(objectgroupBox);
        objectsView->setObjectName(QString::fromUtf8("objectsView"));
        objectsView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        objectsView->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_2->addWidget(objectsView, 0, 0, 1, 1);


        gridLayout_7->addWidget(objectgroupBox, 0, 3, 1, 1);

        variablegroupBox = new QGroupBox(VariablePlotDialog);
        variablegroupBox->setObjectName(QString::fromUtf8("variablegroupBox"));
        variablegroupBox->setMinimumSize(QSize(190, 190));
        gridLayout_3 = new QGridLayout(variablegroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        variableView = new QListView(variablegroupBox);
        variableView->setObjectName(QString::fromUtf8("variableView"));
        variableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_3->addWidget(variableView, 0, 0, 1, 1);


        gridLayout_7->addWidget(variablegroupBox, 1, 0, 1, 2);

        variablegroupBox_2 = new QGroupBox(VariablePlotDialog);
        variablegroupBox_2->setObjectName(QString::fromUtf8("variablegroupBox_2"));
        variablegroupBox_2->setMinimumSize(QSize(190, 190));
        gridLayout_5 = new QGridLayout(variablegroupBox_2);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        variableView_2 = new QListView(variablegroupBox_2);
        variableView_2->setObjectName(QString::fromUtf8("variableView_2"));
        variableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_5->addWidget(variableView_2, 0, 0, 1, 1);


        gridLayout_7->addWidget(variablegroupBox_2, 1, 2, 1, 2);


        retranslateUi(VariablePlotDialog);

        QMetaObject::connectSlotsByName(VariablePlotDialog);
    } // setupUi

    void retranslateUi(QDialog *VariablePlotDialog)
    {
        VariablePlotDialog->setWindowTitle(QCoreApplication::translate("VariablePlotDialog", "Variable Plot Dialog", nullptr));
        cancelButton->setText(QCoreApplication::translate("VariablePlotDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("VariablePlotDialog", "OK", nullptr));
        projectgroupBox->setTitle(QCoreApplication::translate("VariablePlotDialog", "Project List", nullptr));
        datagroupBox->setTitle(QCoreApplication::translate("VariablePlotDialog", "Data", nullptr));
        objectgroupBox->setTitle(QCoreApplication::translate("VariablePlotDialog", "Objects", nullptr));
        variablegroupBox->setTitle(QCoreApplication::translate("VariablePlotDialog", "Variable", nullptr));
        variablegroupBox_2->setTitle(QCoreApplication::translate("VariablePlotDialog", "Variable", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VariablePlotDialog: public Ui_VariablePlotDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VARIABLEPLOTDIALOG_H
