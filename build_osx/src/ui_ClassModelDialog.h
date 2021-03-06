/********************************************************************************
** Form generated from reading UI file 'ClassModelDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLASSMODELDIALOG_H
#define UI_CLASSMODELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ClassModelDialog
{
public:
    QGridLayout *gridLayout_4;
    QFrame *modnameFrame;
    QGridLayout *gridLayout_9;
    QLabel *label_5;
    QLineEdit *modelname;
    QSpacerItem *horizontalSpacer_2;
    QGroupBox *projectsGroupBox;
    QGridLayout *gridLayout_8;
    QListView *listView_1;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_7;
    QListView *listView_2;
    QGroupBox *objectGroupBox;
    QGridLayout *gridLayout_5;
    QListView *listView_3;
    QSpacerItem *verticalSpacer;
    QListView *listView_4;
    QPushButton *addClass;
    QPushButton *removeClass;
    QSpacerItem *verticalSpacer_2;
    QGridLayout *gridLayout_2;
    QPushButton *objSelectAllButton;
    QPushButton *objInvertSelectionButton;
    QComboBox *objSelectByLabel;
    QPushButton *objUnselectButton;
    QGroupBox *variableGroupBox;
    QGridLayout *gridLayout_3;
    QListView *listView_5;
    QPushButton *varSelectAllButton;
    QPushButton *varInvertSelectionButton;
    QComboBox *varSelectByLabel;
    QPushButton *varUnselectButton;
    QGridLayout *gridLayout;
    QPushButton *nextButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QPushButton *prevButton;

    void setupUi(QDialog *ClassModelDialog)
    {
        if (ClassModelDialog->objectName().isEmpty())
            ClassModelDialog->setObjectName(QString::fromUtf8("ClassModelDialog"));
        ClassModelDialog->resize(1542, 417);
        gridLayout_4 = new QGridLayout(ClassModelDialog);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        modnameFrame = new QFrame(ClassModelDialog);
        modnameFrame->setObjectName(QString::fromUtf8("modnameFrame"));
        gridLayout_9 = new QGridLayout(modnameFrame);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_5 = new QLabel(modnameFrame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_9->addWidget(label_5, 0, 0, 1, 1);

        modelname = new QLineEdit(modnameFrame);
        modelname->setObjectName(QString::fromUtf8("modelname"));

        gridLayout_9->addWidget(modelname, 0, 1, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_2, 0, 2, 1, 1);


        gridLayout_4->addWidget(modnameFrame, 0, 0, 1, 4);

        projectsGroupBox = new QGroupBox(ClassModelDialog);
        projectsGroupBox->setObjectName(QString::fromUtf8("projectsGroupBox"));
        projectsGroupBox->setMinimumSize(QSize(280, 280));
        gridLayout_8 = new QGridLayout(projectsGroupBox);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        listView_1 = new QListView(projectsGroupBox);
        listView_1->setObjectName(QString::fromUtf8("listView_1"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listView_1->sizePolicy().hasHeightForWidth());
        listView_1->setSizePolicy(sizePolicy);
        listView_1->setMinimumSize(QSize(0, 0));
        listView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_1->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_1->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_8->addWidget(listView_1, 0, 0, 1, 1);


        gridLayout_4->addWidget(projectsGroupBox, 1, 0, 1, 1);

        dataGroupBox = new QGroupBox(ClassModelDialog);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        dataGroupBox->setMinimumSize(QSize(280, 280));
        gridLayout_7 = new QGridLayout(dataGroupBox);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        listView_2 = new QListView(dataGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        sizePolicy.setHeightForWidth(listView_2->sizePolicy().hasHeightForWidth());
        listView_2->setSizePolicy(sizePolicy);
        listView_2->setMinimumSize(QSize(0, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_7->addWidget(listView_2, 0, 0, 1, 1);


        gridLayout_4->addWidget(dataGroupBox, 1, 1, 1, 1);

        objectGroupBox = new QGroupBox(ClassModelDialog);
        objectGroupBox->setObjectName(QString::fromUtf8("objectGroupBox"));
        objectGroupBox->setMinimumSize(QSize(630, 300));
        gridLayout_5 = new QGridLayout(objectGroupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        listView_3 = new QListView(objectGroupBox);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        sizePolicy.setHeightForWidth(listView_3->sizePolicy().hasHeightForWidth());
        listView_3->setSizePolicy(sizePolicy);
        listView_3->setMinimumSize(QSize(270, 190));
        listView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_3->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_3->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_5->addWidget(listView_3, 0, 0, 4, 1);

        verticalSpacer = new QSpacerItem(20, 41, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 0, 1, 1, 1);

        listView_4 = new QListView(objectGroupBox);
        listView_4->setObjectName(QString::fromUtf8("listView_4"));
        sizePolicy.setHeightForWidth(listView_4->sizePolicy().hasHeightForWidth());
        listView_4->setSizePolicy(sizePolicy);
        listView_4->setMinimumSize(QSize(180, 240));
        listView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_4->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_4->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_5->addWidget(listView_4, 0, 2, 5, 1);

        addClass = new QPushButton(objectGroupBox);
        addClass->setObjectName(QString::fromUtf8("addClass"));
        addClass->setMinimumSize(QSize(130, 24));
        addClass->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(addClass, 1, 1, 1, 1);

        removeClass = new QPushButton(objectGroupBox);
        removeClass->setObjectName(QString::fromUtf8("removeClass"));
        removeClass->setMinimumSize(QSize(130, 24));
        removeClass->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(removeClass, 2, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 72, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 3, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        objSelectAllButton = new QPushButton(objectGroupBox);
        objSelectAllButton->setObjectName(QString::fromUtf8("objSelectAllButton"));
        objSelectAllButton->setMinimumSize(QSize(130, 24));
        objSelectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout_2->addWidget(objSelectAllButton, 0, 0, 1, 1);

        objInvertSelectionButton = new QPushButton(objectGroupBox);
        objInvertSelectionButton->setObjectName(QString::fromUtf8("objInvertSelectionButton"));
        objInvertSelectionButton->setMinimumSize(QSize(130, 24));
        objInvertSelectionButton->setMaximumSize(QSize(130, 24));

        gridLayout_2->addWidget(objInvertSelectionButton, 0, 1, 1, 1);

        objSelectByLabel = new QComboBox(objectGroupBox);
        objSelectByLabel->addItem(QString());
        objSelectByLabel->setObjectName(QString::fromUtf8("objSelectByLabel"));
        objSelectByLabel->setMinimumSize(QSize(130, 24));
        objSelectByLabel->setMaximumSize(QSize(130, 24));

        gridLayout_2->addWidget(objSelectByLabel, 1, 0, 1, 1);

        objUnselectButton = new QPushButton(objectGroupBox);
        objUnselectButton->setObjectName(QString::fromUtf8("objUnselectButton"));
        objUnselectButton->setMinimumSize(QSize(130, 24));
        objUnselectButton->setMaximumSize(QSize(130, 24));

        gridLayout_2->addWidget(objUnselectButton, 1, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 4, 0, 1, 1);


        gridLayout_4->addWidget(objectGroupBox, 1, 2, 1, 1);

        variableGroupBox = new QGroupBox(ClassModelDialog);
        variableGroupBox->setObjectName(QString::fromUtf8("variableGroupBox"));
        variableGroupBox->setMinimumSize(QSize(280, 280));
        gridLayout_3 = new QGridLayout(variableGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listView_5 = new QListView(variableGroupBox);
        listView_5->setObjectName(QString::fromUtf8("listView_5"));
        sizePolicy.setHeightForWidth(listView_5->sizePolicy().hasHeightForWidth());
        listView_5->setSizePolicy(sizePolicy);
        listView_5->setMinimumSize(QSize(0, 0));
        listView_5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_5->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_5->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_3->addWidget(listView_5, 0, 0, 1, 2);

        varSelectAllButton = new QPushButton(variableGroupBox);
        varSelectAllButton->setObjectName(QString::fromUtf8("varSelectAllButton"));
        varSelectAllButton->setMinimumSize(QSize(130, 24));
        varSelectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(varSelectAllButton, 1, 0, 1, 1);

        varInvertSelectionButton = new QPushButton(variableGroupBox);
        varInvertSelectionButton->setObjectName(QString::fromUtf8("varInvertSelectionButton"));
        varInvertSelectionButton->setMinimumSize(QSize(130, 24));
        varInvertSelectionButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(varInvertSelectionButton, 1, 1, 1, 1);

        varSelectByLabel = new QComboBox(variableGroupBox);
        varSelectByLabel->addItem(QString());
        varSelectByLabel->setObjectName(QString::fromUtf8("varSelectByLabel"));
        varSelectByLabel->setMinimumSize(QSize(130, 24));
        varSelectByLabel->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(varSelectByLabel, 2, 0, 1, 1);

        varUnselectButton = new QPushButton(variableGroupBox);
        varUnselectButton->setObjectName(QString::fromUtf8("varUnselectButton"));
        varUnselectButton->setMinimumSize(QSize(130, 24));
        varUnselectButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(varUnselectButton, 2, 1, 1, 1);


        gridLayout_4->addWidget(variableGroupBox, 1, 3, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        nextButton = new QPushButton(ClassModelDialog);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setMinimumSize(QSize(100, 24));
        nextButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(nextButton, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(ClassModelDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(cancelButton, 0, 3, 1, 1);

        okButton = new QPushButton(ClassModelDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(okButton, 0, 4, 1, 1);

        prevButton = new QPushButton(ClassModelDialog);
        prevButton->setObjectName(QString::fromUtf8("prevButton"));
        prevButton->setMinimumSize(QSize(100, 24));
        prevButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(prevButton, 0, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout, 2, 0, 1, 4);


        retranslateUi(ClassModelDialog);

        QMetaObject::connectSlotsByName(ClassModelDialog);
    } // setupUi

    void retranslateUi(QDialog *ClassModelDialog)
    {
        ClassModelDialog->setWindowTitle(QCoreApplication::translate("ClassModelDialog", "Dialog", nullptr));
        label_5->setText(QCoreApplication::translate("ClassModelDialog", "Model Name", nullptr));
        projectsGroupBox->setTitle(QCoreApplication::translate("ClassModelDialog", "Projects", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("ClassModelDialog", "Data", nullptr));
        objectGroupBox->setTitle(QCoreApplication::translate("ClassModelDialog", "ObJects", nullptr));
        addClass->setText(QCoreApplication::translate("ClassModelDialog", "Add Class", nullptr));
        removeClass->setText(QCoreApplication::translate("ClassModelDialog", "Remove Class", nullptr));
        objSelectAllButton->setText(QCoreApplication::translate("ClassModelDialog", "Select All", nullptr));
        objInvertSelectionButton->setText(QCoreApplication::translate("ClassModelDialog", "Invert Selection", nullptr));
        objSelectByLabel->setItemText(0, QCoreApplication::translate("ClassModelDialog", "Select by label...", nullptr));

        objUnselectButton->setText(QCoreApplication::translate("ClassModelDialog", "Unselect", nullptr));
        variableGroupBox->setTitle(QCoreApplication::translate("ClassModelDialog", "Variables", nullptr));
        varSelectAllButton->setText(QCoreApplication::translate("ClassModelDialog", "Select All", nullptr));
        varInvertSelectionButton->setText(QCoreApplication::translate("ClassModelDialog", "Invert Selection", nullptr));
        varSelectByLabel->setItemText(0, QCoreApplication::translate("ClassModelDialog", "Select by label...", nullptr));

        varUnselectButton->setText(QCoreApplication::translate("ClassModelDialog", "Unselect", nullptr));
        nextButton->setText(QCoreApplication::translate("ClassModelDialog", "Next", nullptr));
        cancelButton->setText(QCoreApplication::translate("ClassModelDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ClassModelDialog", "OK", nullptr));
        prevButton->setText(QCoreApplication::translate("ClassModelDialog", "Prev", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClassModelDialog: public Ui_ClassModelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLASSMODELDIALOG_H
