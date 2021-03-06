/********************************************************************************
** Form generated from reading UI file 'ModelDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELDIALOG_H
#define UI_MODELDIALOG_H

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
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ModelDialog
{
public:
    QGridLayout *gridLayout_12;
    QGridLayout *gridLayout;
    QPushButton *nextButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QPushButton *prevButton;
    QFrame *modnameFrame;
    QGridLayout *gridLayout_9;
    QLabel *label_5;
    QLineEdit *modelname;
    QGroupBox *OptGroupBox;
    QGridLayout *gridLayout_5;
    QSpinBox *NPrincipalComponent;
    QFrame *xScalingFrame;
    QGridLayout *gridLayout_4;
    QLabel *label;
    QComboBox *xscalinglist;
    QFrame *yScalingFrame;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QComboBox *yscalinglist;
    QGroupBox *XvariableGroupBox;
    QGridLayout *gridLayout_3;
    QListView *listView_4;
    QPushButton *xvarSelectAllButton;
    QPushButton *xvarInvertSelectionButton;
    QComboBox *xvarSelectByLabel;
    QPushButton *xvarUnselectButton;
    QGroupBox *YvariableGroupBox;
    QGridLayout *gridLayout_6;
    QComboBox *yvarSelectByLabel;
    QPushButton *yvarSelectAllButton;
    QPushButton *yvarInvertSelectionButton;
    QPushButton *yvarUnselectButton;
    QListView *listView_5;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_7;
    QListView *listView_2;
    QGroupBox *projectsGroupBox;
    QGridLayout *gridLayout_8;
    QListView *listView_1;
    QGroupBox *objectGroupBox;
    QGridLayout *gridLayout_10;
    QListView *listView_3;
    QPushButton *objSelectAllButton;
    QPushButton *objInvertSelectionButton;
    QComboBox *objSelectByLabel;
    QPushButton *objUnselectButton;

    void setupUi(QDialog *ModelDialog)
    {
        if (ModelDialog->objectName().isEmpty())
            ModelDialog->setObjectName(QString::fromUtf8("ModelDialog"));
        ModelDialog->setWindowModality(Qt::WindowModal);
        ModelDialog->resize(1639, 367);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ModelDialog->sizePolicy().hasHeightForWidth());
        ModelDialog->setSizePolicy(sizePolicy);
        ModelDialog->setMinimumSize(QSize(800, 320));
        ModelDialog->setMaximumSize(QSize(2080, 800));
        ModelDialog->setModal(true);
        gridLayout_12 = new QGridLayout(ModelDialog);
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        nextButton = new QPushButton(ModelDialog);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setMinimumSize(QSize(100, 24));
        nextButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(nextButton, 0, 2, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(ModelDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(cancelButton, 0, 3, 1, 1);

        okButton = new QPushButton(ModelDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(okButton, 0, 4, 1, 1);

        prevButton = new QPushButton(ModelDialog);
        prevButton->setObjectName(QString::fromUtf8("prevButton"));
        prevButton->setMinimumSize(QSize(100, 24));
        prevButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(prevButton, 0, 1, 1, 1);


        gridLayout_12->addLayout(gridLayout, 2, 0, 1, 6);

        modnameFrame = new QFrame(ModelDialog);
        modnameFrame->setObjectName(QString::fromUtf8("modnameFrame"));
        gridLayout_9 = new QGridLayout(modnameFrame);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        label_5 = new QLabel(modnameFrame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_9->addWidget(label_5, 0, 0, 1, 1);

        modelname = new QLineEdit(modnameFrame);
        modelname->setObjectName(QString::fromUtf8("modelname"));

        gridLayout_9->addWidget(modelname, 0, 1, 1, 1);


        gridLayout_12->addWidget(modnameFrame, 0, 0, 1, 6);

        OptGroupBox = new QGroupBox(ModelDialog);
        OptGroupBox->setObjectName(QString::fromUtf8("OptGroupBox"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(OptGroupBox->sizePolicy().hasHeightForWidth());
        OptGroupBox->setSizePolicy(sizePolicy1);
        OptGroupBox->setMinimumSize(QSize(280, 64));
        OptGroupBox->setMaximumSize(QSize(280, 16777215));
        gridLayout_5 = new QGridLayout(OptGroupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        NPrincipalComponent = new QSpinBox(OptGroupBox);
        NPrincipalComponent->setObjectName(QString::fromUtf8("NPrincipalComponent"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(NPrincipalComponent->sizePolicy().hasHeightForWidth());
        NPrincipalComponent->setSizePolicy(sizePolicy2);
        NPrincipalComponent->setMinimumSize(QSize(56, 27));
        NPrincipalComponent->setMaximumSize(QSize(180, 27));
        NPrincipalComponent->setMinimum(1);
        NPrincipalComponent->setMaximum(500);
        NPrincipalComponent->setValue(5);

        gridLayout_5->addWidget(NPrincipalComponent, 0, 0, 1, 1);

        xScalingFrame = new QFrame(OptGroupBox);
        xScalingFrame->setObjectName(QString::fromUtf8("xScalingFrame"));
        xScalingFrame->setFrameShape(QFrame::StyledPanel);
        xScalingFrame->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(xScalingFrame);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        label = new QLabel(xScalingFrame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_4->addWidget(label, 0, 0, 1, 1);

        xscalinglist = new QComboBox(xScalingFrame);
        xscalinglist->addItem(QString());
        xscalinglist->addItem(QString());
        xscalinglist->addItem(QString());
        xscalinglist->addItem(QString());
        xscalinglist->addItem(QString());
        xscalinglist->addItem(QString());
        xscalinglist->setObjectName(QString::fromUtf8("xscalinglist"));
        QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(xscalinglist->sizePolicy().hasHeightForWidth());
        xscalinglist->setSizePolicy(sizePolicy3);
        xscalinglist->setMinimumSize(QSize(200, 24));
        xscalinglist->setMaximumSize(QSize(200, 24));
        xscalinglist->setEditable(false);

        gridLayout_4->addWidget(xscalinglist, 1, 0, 1, 1);


        gridLayout_5->addWidget(xScalingFrame, 1, 0, 1, 1);

        yScalingFrame = new QFrame(OptGroupBox);
        yScalingFrame->setObjectName(QString::fromUtf8("yScalingFrame"));
        yScalingFrame->setFrameShape(QFrame::StyledPanel);
        yScalingFrame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(yScalingFrame);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_2 = new QLabel(yScalingFrame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        yscalinglist = new QComboBox(yScalingFrame);
        yscalinglist->addItem(QString());
        yscalinglist->addItem(QString());
        yscalinglist->addItem(QString());
        yscalinglist->addItem(QString());
        yscalinglist->addItem(QString());
        yscalinglist->addItem(QString());
        yscalinglist->setObjectName(QString::fromUtf8("yscalinglist"));
        sizePolicy3.setHeightForWidth(yscalinglist->sizePolicy().hasHeightForWidth());
        yscalinglist->setSizePolicy(sizePolicy3);
        yscalinglist->setMinimumSize(QSize(200, 24));
        yscalinglist->setMaximumSize(QSize(200, 24));
        yscalinglist->setEditable(false);

        gridLayout_2->addWidget(yscalinglist, 1, 0, 1, 1);


        gridLayout_5->addWidget(yScalingFrame, 2, 0, 1, 1);


        gridLayout_12->addWidget(OptGroupBox, 1, 2, 1, 1);

        XvariableGroupBox = new QGroupBox(ModelDialog);
        XvariableGroupBox->setObjectName(QString::fromUtf8("XvariableGroupBox"));
        sizePolicy1.setHeightForWidth(XvariableGroupBox->sizePolicy().hasHeightForWidth());
        XvariableGroupBox->setSizePolicy(sizePolicy1);
        gridLayout_3 = new QGridLayout(XvariableGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listView_4 = new QListView(XvariableGroupBox);
        listView_4->setObjectName(QString::fromUtf8("listView_4"));
        QSizePolicy sizePolicy4(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(listView_4->sizePolicy().hasHeightForWidth());
        listView_4->setSizePolicy(sizePolicy4);
        listView_4->setMinimumSize(QSize(0, 0));
        listView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_4->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_4->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_3->addWidget(listView_4, 0, 0, 1, 2);

        xvarSelectAllButton = new QPushButton(XvariableGroupBox);
        xvarSelectAllButton->setObjectName(QString::fromUtf8("xvarSelectAllButton"));
        xvarSelectAllButton->setMinimumSize(QSize(125, 24));
        xvarSelectAllButton->setMaximumSize(QSize(125, 24));

        gridLayout_3->addWidget(xvarSelectAllButton, 1, 0, 1, 1);

        xvarInvertSelectionButton = new QPushButton(XvariableGroupBox);
        xvarInvertSelectionButton->setObjectName(QString::fromUtf8("xvarInvertSelectionButton"));
        xvarInvertSelectionButton->setMinimumSize(QSize(125, 24));
        xvarInvertSelectionButton->setMaximumSize(QSize(125, 24));

        gridLayout_3->addWidget(xvarInvertSelectionButton, 1, 1, 1, 1);

        xvarSelectByLabel = new QComboBox(XvariableGroupBox);
        xvarSelectByLabel->addItem(QString());
        xvarSelectByLabel->setObjectName(QString::fromUtf8("xvarSelectByLabel"));
        xvarSelectByLabel->setMinimumSize(QSize(125, 24));
        xvarSelectByLabel->setMaximumSize(QSize(125, 24));

        gridLayout_3->addWidget(xvarSelectByLabel, 2, 0, 1, 1);

        xvarUnselectButton = new QPushButton(XvariableGroupBox);
        xvarUnselectButton->setObjectName(QString::fromUtf8("xvarUnselectButton"));
        xvarUnselectButton->setMinimumSize(QSize(125, 24));
        xvarUnselectButton->setMaximumSize(QSize(125, 24));

        gridLayout_3->addWidget(xvarUnselectButton, 2, 1, 1, 1);


        gridLayout_12->addWidget(XvariableGroupBox, 1, 4, 1, 1);

        YvariableGroupBox = new QGroupBox(ModelDialog);
        YvariableGroupBox->setObjectName(QString::fromUtf8("YvariableGroupBox"));
        gridLayout_6 = new QGridLayout(YvariableGroupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        yvarSelectByLabel = new QComboBox(YvariableGroupBox);
        yvarSelectByLabel->addItem(QString());
        yvarSelectByLabel->setObjectName(QString::fromUtf8("yvarSelectByLabel"));
        yvarSelectByLabel->setMinimumSize(QSize(125, 24));
        yvarSelectByLabel->setMaximumSize(QSize(125, 24));

        gridLayout_6->addWidget(yvarSelectByLabel, 2, 0, 1, 1);

        yvarSelectAllButton = new QPushButton(YvariableGroupBox);
        yvarSelectAllButton->setObjectName(QString::fromUtf8("yvarSelectAllButton"));
        yvarSelectAllButton->setMinimumSize(QSize(125, 24));
        yvarSelectAllButton->setMaximumSize(QSize(125, 24));

        gridLayout_6->addWidget(yvarSelectAllButton, 1, 0, 1, 1);

        yvarInvertSelectionButton = new QPushButton(YvariableGroupBox);
        yvarInvertSelectionButton->setObjectName(QString::fromUtf8("yvarInvertSelectionButton"));
        yvarInvertSelectionButton->setMinimumSize(QSize(125, 24));
        yvarInvertSelectionButton->setMaximumSize(QSize(125, 24));

        gridLayout_6->addWidget(yvarInvertSelectionButton, 1, 1, 1, 1);

        yvarUnselectButton = new QPushButton(YvariableGroupBox);
        yvarUnselectButton->setObjectName(QString::fromUtf8("yvarUnselectButton"));
        yvarUnselectButton->setMinimumSize(QSize(125, 24));
        yvarUnselectButton->setMaximumSize(QSize(125, 24));

        gridLayout_6->addWidget(yvarUnselectButton, 2, 1, 1, 1);

        listView_5 = new QListView(YvariableGroupBox);
        listView_5->setObjectName(QString::fromUtf8("listView_5"));
        sizePolicy4.setHeightForWidth(listView_5->sizePolicy().hasHeightForWidth());
        listView_5->setSizePolicy(sizePolicy4);
        listView_5->setMinimumSize(QSize(0, 0));
        listView_5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_5->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_5->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_6->addWidget(listView_5, 0, 0, 1, 2);


        gridLayout_12->addWidget(YvariableGroupBox, 1, 5, 1, 1);

        dataGroupBox = new QGroupBox(ModelDialog);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        sizePolicy1.setHeightForWidth(dataGroupBox->sizePolicy().hasHeightForWidth());
        dataGroupBox->setSizePolicy(sizePolicy1);
        gridLayout_7 = new QGridLayout(dataGroupBox);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        listView_2 = new QListView(dataGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        sizePolicy4.setHeightForWidth(listView_2->sizePolicy().hasHeightForWidth());
        listView_2->setSizePolicy(sizePolicy4);
        listView_2->setMinimumSize(QSize(0, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_7->addWidget(listView_2, 0, 0, 1, 1);


        gridLayout_12->addWidget(dataGroupBox, 1, 1, 1, 1);

        projectsGroupBox = new QGroupBox(ModelDialog);
        projectsGroupBox->setObjectName(QString::fromUtf8("projectsGroupBox"));
        sizePolicy1.setHeightForWidth(projectsGroupBox->sizePolicy().hasHeightForWidth());
        projectsGroupBox->setSizePolicy(sizePolicy1);
        gridLayout_8 = new QGridLayout(projectsGroupBox);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        listView_1 = new QListView(projectsGroupBox);
        listView_1->setObjectName(QString::fromUtf8("listView_1"));
        sizePolicy4.setHeightForWidth(listView_1->sizePolicy().hasHeightForWidth());
        listView_1->setSizePolicy(sizePolicy4);
        listView_1->setMinimumSize(QSize(0, 0));
        listView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_1->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_1->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_8->addWidget(listView_1, 0, 0, 1, 1);


        gridLayout_12->addWidget(projectsGroupBox, 1, 0, 1, 1);

        objectGroupBox = new QGroupBox(ModelDialog);
        objectGroupBox->setObjectName(QString::fromUtf8("objectGroupBox"));
        sizePolicy1.setHeightForWidth(objectGroupBox->sizePolicy().hasHeightForWidth());
        objectGroupBox->setSizePolicy(sizePolicy1);
        gridLayout_10 = new QGridLayout(objectGroupBox);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        listView_3 = new QListView(objectGroupBox);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        sizePolicy4.setHeightForWidth(listView_3->sizePolicy().hasHeightForWidth());
        listView_3->setSizePolicy(sizePolicy4);
        listView_3->setMinimumSize(QSize(0, 0));
        listView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_3->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_3->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_10->addWidget(listView_3, 0, 0, 1, 2);

        objSelectAllButton = new QPushButton(objectGroupBox);
        objSelectAllButton->setObjectName(QString::fromUtf8("objSelectAllButton"));
        objSelectAllButton->setMinimumSize(QSize(125, 24));
        objSelectAllButton->setMaximumSize(QSize(125, 24));

        gridLayout_10->addWidget(objSelectAllButton, 1, 0, 1, 1);

        objInvertSelectionButton = new QPushButton(objectGroupBox);
        objInvertSelectionButton->setObjectName(QString::fromUtf8("objInvertSelectionButton"));
        objInvertSelectionButton->setMinimumSize(QSize(125, 24));
        objInvertSelectionButton->setMaximumSize(QSize(125, 24));

        gridLayout_10->addWidget(objInvertSelectionButton, 1, 1, 1, 1);

        objSelectByLabel = new QComboBox(objectGroupBox);
        objSelectByLabel->addItem(QString());
        objSelectByLabel->setObjectName(QString::fromUtf8("objSelectByLabel"));
        objSelectByLabel->setMinimumSize(QSize(125, 24));
        objSelectByLabel->setMaximumSize(QSize(125, 24));

        gridLayout_10->addWidget(objSelectByLabel, 2, 0, 1, 1);

        objUnselectButton = new QPushButton(objectGroupBox);
        objUnselectButton->setObjectName(QString::fromUtf8("objUnselectButton"));
        objUnselectButton->setMinimumSize(QSize(125, 24));
        objUnselectButton->setMaximumSize(QSize(125, 24));

        gridLayout_10->addWidget(objUnselectButton, 2, 1, 1, 1);


        gridLayout_12->addWidget(objectGroupBox, 1, 3, 1, 1);


        retranslateUi(ModelDialog);

        xscalinglist->setCurrentIndex(1);
        yscalinglist->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ModelDialog);
    } // setupUi

    void retranslateUi(QDialog *ModelDialog)
    {
        ModelDialog->setWindowTitle(QCoreApplication::translate("ModelDialog", "Compute...", nullptr));
        nextButton->setText(QCoreApplication::translate("ModelDialog", "Next", nullptr));
        cancelButton->setText(QCoreApplication::translate("ModelDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ModelDialog", "OK", nullptr));
        prevButton->setText(QCoreApplication::translate("ModelDialog", "Prev", nullptr));
        label_5->setText(QCoreApplication::translate("ModelDialog", "Model Name", nullptr));
        OptGroupBox->setTitle(QCoreApplication::translate("ModelDialog", "N. of Principal components", nullptr));
        label->setText(QCoreApplication::translate("ModelDialog", "X Scaling", nullptr));
        xscalinglist->setItemText(0, QCoreApplication::translate("ModelDialog", "No Scaling", nullptr));
        xscalinglist->setItemText(1, QCoreApplication::translate("ModelDialog", "SDEV Column Scaling", nullptr));
        xscalinglist->setItemText(2, QCoreApplication::translate("ModelDialog", "RMS Column Scaling", nullptr));
        xscalinglist->setItemText(3, QCoreApplication::translate("ModelDialog", "Pareto Scaling", nullptr));
        xscalinglist->setItemText(4, QCoreApplication::translate("ModelDialog", "Range Scaling", nullptr));
        xscalinglist->setItemText(5, QCoreApplication::translate("ModelDialog", "Level Scaling", nullptr));

        label_2->setText(QCoreApplication::translate("ModelDialog", "Y Scaling", nullptr));
        yscalinglist->setItemText(0, QCoreApplication::translate("ModelDialog", "No Scaling", nullptr));
        yscalinglist->setItemText(1, QCoreApplication::translate("ModelDialog", "SDEV Column Scaling", nullptr));
        yscalinglist->setItemText(2, QCoreApplication::translate("ModelDialog", "RMS Column Scaling", nullptr));
        yscalinglist->setItemText(3, QCoreApplication::translate("ModelDialog", "Pareto Scaling", nullptr));
        yscalinglist->setItemText(4, QCoreApplication::translate("ModelDialog", "Range Scaling", nullptr));
        yscalinglist->setItemText(5, QCoreApplication::translate("ModelDialog", "Level Scaling", nullptr));

        XvariableGroupBox->setTitle(QCoreApplication::translate("ModelDialog", "X Variables", nullptr));
        xvarSelectAllButton->setText(QCoreApplication::translate("ModelDialog", "Select All", nullptr));
        xvarInvertSelectionButton->setText(QCoreApplication::translate("ModelDialog", "Invert Selection", nullptr));
        xvarSelectByLabel->setItemText(0, QCoreApplication::translate("ModelDialog", "Select by label...", nullptr));

        xvarUnselectButton->setText(QCoreApplication::translate("ModelDialog", "Unselect", nullptr));
        YvariableGroupBox->setTitle(QCoreApplication::translate("ModelDialog", "Y Variables", nullptr));
        yvarSelectByLabel->setItemText(0, QCoreApplication::translate("ModelDialog", "Select by label...", nullptr));

        yvarSelectAllButton->setText(QCoreApplication::translate("ModelDialog", "Select All", nullptr));
        yvarInvertSelectionButton->setText(QCoreApplication::translate("ModelDialog", "Invert Selection", nullptr));
        yvarUnselectButton->setText(QCoreApplication::translate("ModelDialog", "Unselect", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("ModelDialog", "Data", nullptr));
        projectsGroupBox->setTitle(QCoreApplication::translate("ModelDialog", "Projects", nullptr));
        objectGroupBox->setTitle(QCoreApplication::translate("ModelDialog", "Objects", nullptr));
        objSelectAllButton->setText(QCoreApplication::translate("ModelDialog", "Select All", nullptr));
        objInvertSelectionButton->setText(QCoreApplication::translate("ModelDialog", "Invert Selection", nullptr));
        objSelectByLabel->setItemText(0, QCoreApplication::translate("ModelDialog", "Select by label...", nullptr));

        objUnselectButton->setText(QCoreApplication::translate("ModelDialog", "Unselect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelDialog: public Ui_ModelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELDIALOG_H
