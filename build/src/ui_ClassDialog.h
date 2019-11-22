/********************************************************************************
** Form generated from reading UI file 'ClassDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLASSDIALOG_H
#define UI_CLASSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ClassDialog
{
public:
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_5;
    QListView *listView;
    QGridLayout *gridLayout;
    QPushButton *importButton;
    QPushButton *addButton;
    QPushButton *removeButton;
    QGroupBox *modelsGroupBox;
    QGridLayout *gridLayout_2;
    QListView *listView_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *ClassDialog)
    {
        if (ClassDialog->objectName().isEmpty())
            ClassDialog->setObjectName(QString::fromUtf8("ClassDialog"));
        ClassDialog->resize(730, 367);
        gridLayout_4 = new QGridLayout(ClassDialog);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        groupBox_2 = new QGroupBox(ClassDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(189, 261));
        gridLayout_5 = new QGridLayout(groupBox_2);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        listView = new QListView(groupBox_2);
        listView->setObjectName(QString::fromUtf8("listView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy);
        listView->setMinimumSize(QSize(0, 0));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_5->addWidget(listView, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        importButton = new QPushButton(ClassDialog);
        importButton->setObjectName(QString::fromUtf8("importButton"));

        gridLayout->addWidget(importButton, 0, 0, 1, 1);

        addButton = new QPushButton(ClassDialog);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setMinimumSize(QSize(80, 32));

        gridLayout->addWidget(addButton, 1, 0, 1, 1);

        removeButton = new QPushButton(ClassDialog);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        removeButton->setMinimumSize(QSize(80, 34));

        gridLayout->addWidget(removeButton, 2, 0, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 1, 1, 1);

        modelsGroupBox = new QGroupBox(ClassDialog);
        modelsGroupBox->setObjectName(QString::fromUtf8("modelsGroupBox"));
        gridLayout_2 = new QGridLayout(modelsGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        listView_2 = new QListView(modelsGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setMinimumSize(QSize(0, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setAlternatingRowColors(true);
        listView_2->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_2->addWidget(listView_2, 0, 0, 1, 1);


        gridLayout_3->addWidget(modelsGroupBox, 0, 2, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(412, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer, 1, 0, 1, 1);

        cancelButton = new QPushButton(ClassDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout_4->addWidget(cancelButton, 1, 1, 1, 1);

        okButton = new QPushButton(ClassDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout_4->addWidget(okButton, 1, 2, 1, 1);


        retranslateUi(ClassDialog);

        QMetaObject::connectSlotsByName(ClassDialog);
    } // setupUi

    void retranslateUi(QDialog *ClassDialog)
    {
        ClassDialog->setWindowTitle(QCoreApplication::translate("ClassDialog", "ClassDialog", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ClassDialog", "Objects", nullptr));
        importButton->setText(QCoreApplication::translate("ClassDialog", "Import Classes", nullptr));
        addButton->setText(QCoreApplication::translate("ClassDialog", ">", nullptr));
        removeButton->setText(QCoreApplication::translate("ClassDialog", "<", nullptr));
        modelsGroupBox->setTitle(QCoreApplication::translate("ClassDialog", "Classes", nullptr));
        cancelButton->setText(QCoreApplication::translate("ClassDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ClassDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClassDialog: public Ui_ClassDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLASSDIALOG_H
