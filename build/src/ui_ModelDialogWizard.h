/********************************************************************************
** Form generated from reading UI file 'ModelDialogWizard.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELDIALOGWIZARD_H
#define UI_MODELDIALOGWIZARD_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWizard>
#include <QtWidgets/QWizardPage>

QT_BEGIN_NAMESPACE

class Ui_ModelDialogWizard
{
public:
    QWizardPage *wizardPage1;
    QGridLayout *gridLayout_13;
    QFrame *modnameFrame;
    QGridLayout *gridLayout_8;
    QLabel *label_5;
    QLineEdit *modelname;
    QGroupBox *projectsGroupBox;
    QGridLayout *gridLayout;
    QListView *listView_1;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_7;
    QListView *listView_2;
    QWizardPage *wizardPage2;
    QGridLayout *gridLayout_23;
    QGridLayout *gridLayout_12;
    QGridLayout *gridLayout_24;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_11;
    QSpinBox *NPrincipalComponent;
    QGroupBox *ELearningMethodGroupBox;
    QGridLayout *gridLayout_21;
    QComboBox *elmethodComboBox;
    QGridLayout *gridLayout_14;
    QLabel *label_3;
    QSpinBox *nmodelsSpinBox;
    QGridLayout *gridLayout_19;
    QLabel *label;
    QDoubleSpinBox *trainSizeSpinBox;
    QGridLayout *gridLayout_20;
    QLabel *label_2;
    QSpinBox *nvarsSpinBox;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_10;
    QListView *listView_3;
    QGridLayout *gridLayout_22;
    QComboBox *objSelectByLabel;
    QPushButton *objSelectAllButton;
    QPushButton *objUnselectButton;
    QPushButton *objInvertSelectionButton;
    QGridLayout *gridLayout_5;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_25;
    QComboBox *xscalinglist;
    QGroupBox *XvariableGroupBox;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_4;
    QComboBox *xvarSelectByLabel;
    QPushButton *xvarUnselectButton;
    QPushButton *xvarSelectAllButton;
    QPushButton *xvarInvertSelectionButton;
    QListView *listView_4;
    QGridLayout *gridLayout_9;
    QGroupBox *yScaling;
    QGridLayout *gridLayout_2;
    QComboBox *yscalinglist;
    QGroupBox *YvariableGroupBox;
    QGridLayout *gridLayout_6;
    QListView *listView_5;
    QPushButton *yvarSelectAllButton;
    QPushButton *yvarInvertSelectionButton;
    QComboBox *yvarSelectByLabel;
    QPushButton *yvarUnselectButton;
    QGroupBox *YclassGroupBox;
    QGridLayout *gridLayout_17;
    QGridLayout *gridLayout_16;
    QComboBox *class_SelectByLabel;
    QPushButton *class_import;
    QFrame *line;
    QPushButton *class_addButton;
    QPushButton *class_removeButton;
    QGroupBox *modelsGroupBox;
    QGridLayout *gridLayout_18;
    QListView *listView_7;
    QSpacerItem *verticalSpacer;
    QGroupBox *groupBox_4;
    QGridLayout *gridLayout_15;
    QListView *listView_6;

    void setupUi(QWizard *ModelDialogWizard)
    {
        if (ModelDialogWizard->objectName().isEmpty())
            ModelDialogWizard->setObjectName(QString::fromUtf8("ModelDialogWizard"));
        ModelDialogWizard->resize(1856, 533);
        ModelDialogWizard->setMaximumSize(QSize(9999999, 524287));
        ModelDialogWizard->setWizardStyle(QWizard::ClassicStyle);
        wizardPage1 = new QWizardPage();
        wizardPage1->setObjectName(QString::fromUtf8("wizardPage1"));
        gridLayout_13 = new QGridLayout(wizardPage1);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        modnameFrame = new QFrame(wizardPage1);
        modnameFrame->setObjectName(QString::fromUtf8("modnameFrame"));
        gridLayout_8 = new QGridLayout(modnameFrame);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        label_5 = new QLabel(modnameFrame);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_8->addWidget(label_5, 0, 0, 1, 1);

        modelname = new QLineEdit(modnameFrame);
        modelname->setObjectName(QString::fromUtf8("modelname"));

        gridLayout_8->addWidget(modelname, 0, 1, 1, 1);


        gridLayout_13->addWidget(modnameFrame, 0, 0, 1, 2);

        projectsGroupBox = new QGroupBox(wizardPage1);
        projectsGroupBox->setObjectName(QString::fromUtf8("projectsGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(projectsGroupBox->sizePolicy().hasHeightForWidth());
        projectsGroupBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(projectsGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listView_1 = new QListView(projectsGroupBox);
        listView_1->setObjectName(QString::fromUtf8("listView_1"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listView_1->sizePolicy().hasHeightForWidth());
        listView_1->setSizePolicy(sizePolicy1);
        listView_1->setMinimumSize(QSize(200, 0));
        listView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_1->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_1->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(listView_1, 0, 0, 1, 1);


        gridLayout_13->addWidget(projectsGroupBox, 1, 0, 1, 1);

        dataGroupBox = new QGroupBox(wizardPage1);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        sizePolicy.setHeightForWidth(dataGroupBox->sizePolicy().hasHeightForWidth());
        dataGroupBox->setSizePolicy(sizePolicy);
        gridLayout_7 = new QGridLayout(dataGroupBox);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        listView_2 = new QListView(dataGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        sizePolicy1.setHeightForWidth(listView_2->sizePolicy().hasHeightForWidth());
        listView_2->setSizePolicy(sizePolicy1);
        listView_2->setMinimumSize(QSize(200, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_7->addWidget(listView_2, 0, 0, 1, 1);


        gridLayout_13->addWidget(dataGroupBox, 1, 1, 1, 1);

        ModelDialogWizard->addPage(wizardPage1);
        wizardPage2 = new QWizardPage();
        wizardPage2->setObjectName(QString::fromUtf8("wizardPage2"));
        gridLayout_23 = new QGridLayout(wizardPage2);
        gridLayout_23->setObjectName(QString::fromUtf8("gridLayout_23"));
        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        gridLayout_24 = new QGridLayout();
        gridLayout_24->setObjectName(QString::fromUtf8("gridLayout_24"));
        groupBox = new QGroupBox(wizardPage2);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 142));
        gridLayout_11 = new QGridLayout(groupBox);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        NPrincipalComponent = new QSpinBox(groupBox);
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

        gridLayout_11->addWidget(NPrincipalComponent, 0, 0, 1, 1);


        gridLayout_24->addWidget(groupBox, 0, 0, 1, 1);

        ELearningMethodGroupBox = new QGroupBox(wizardPage2);
        ELearningMethodGroupBox->setObjectName(QString::fromUtf8("ELearningMethodGroupBox"));
        ELearningMethodGroupBox->setMinimumSize(QSize(0, 142));
        gridLayout_21 = new QGridLayout(ELearningMethodGroupBox);
        gridLayout_21->setObjectName(QString::fromUtf8("gridLayout_21"));
        elmethodComboBox = new QComboBox(ELearningMethodGroupBox);
        elmethodComboBox->addItem(QString());
        elmethodComboBox->addItem(QString());
        elmethodComboBox->addItem(QString());
        elmethodComboBox->setObjectName(QString::fromUtf8("elmethodComboBox"));

        gridLayout_21->addWidget(elmethodComboBox, 0, 0, 1, 2);

        gridLayout_14 = new QGridLayout();
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        label_3 = new QLabel(ELearningMethodGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_14->addWidget(label_3, 0, 0, 1, 1);

        nmodelsSpinBox = new QSpinBox(ELearningMethodGroupBox);
        nmodelsSpinBox->setObjectName(QString::fromUtf8("nmodelsSpinBox"));
        nmodelsSpinBox->setMinimum(1);
        nmodelsSpinBox->setMaximum(9999);
        nmodelsSpinBox->setValue(100);

        gridLayout_14->addWidget(nmodelsSpinBox, 1, 0, 1, 1);


        gridLayout_21->addLayout(gridLayout_14, 1, 0, 1, 1);

        gridLayout_19 = new QGridLayout();
        gridLayout_19->setObjectName(QString::fromUtf8("gridLayout_19"));
        label = new QLabel(ELearningMethodGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_19->addWidget(label, 0, 0, 1, 1);

        trainSizeSpinBox = new QDoubleSpinBox(ELearningMethodGroupBox);
        trainSizeSpinBox->setObjectName(QString::fromUtf8("trainSizeSpinBox"));
        trainSizeSpinBox->setDecimals(1);
        trainSizeSpinBox->setMinimum(1.000000000000000);
        trainSizeSpinBox->setMaximum(100.000000000000000);
        trainSizeSpinBox->setSingleStep(10.000000000000000);
        trainSizeSpinBox->setValue(70.000000000000000);

        gridLayout_19->addWidget(trainSizeSpinBox, 1, 0, 1, 1);


        gridLayout_21->addLayout(gridLayout_19, 1, 1, 1, 1);

        gridLayout_20 = new QGridLayout();
        gridLayout_20->setObjectName(QString::fromUtf8("gridLayout_20"));
        label_2 = new QLabel(ELearningMethodGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_20->addWidget(label_2, 0, 0, 1, 1);

        nvarsSpinBox = new QSpinBox(ELearningMethodGroupBox);
        nvarsSpinBox->setObjectName(QString::fromUtf8("nvarsSpinBox"));
        nvarsSpinBox->setMinimum(1);
        nvarsSpinBox->setMaximum(999999999);

        gridLayout_20->addWidget(nvarsSpinBox, 1, 0, 1, 1);


        gridLayout_21->addLayout(gridLayout_20, 1, 2, 1, 1);


        gridLayout_24->addWidget(ELearningMethodGroupBox, 0, 1, 1, 1);


        gridLayout_12->addLayout(gridLayout_24, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(wizardPage2);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_10 = new QGridLayout(groupBox_3);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        listView_3 = new QListView(groupBox_3);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        listView_3->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_3->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_10->addWidget(listView_3, 0, 0, 1, 1);

        gridLayout_22 = new QGridLayout();
        gridLayout_22->setObjectName(QString::fromUtf8("gridLayout_22"));
        objSelectByLabel = new QComboBox(groupBox_3);
        objSelectByLabel->addItem(QString());
        objSelectByLabel->setObjectName(QString::fromUtf8("objSelectByLabel"));
        objSelectByLabel->setMinimumSize(QSize(140, 32));
        objSelectByLabel->setMaximumSize(QSize(125, 32));

        gridLayout_22->addWidget(objSelectByLabel, 1, 0, 1, 1);

        objSelectAllButton = new QPushButton(groupBox_3);
        objSelectAllButton->setObjectName(QString::fromUtf8("objSelectAllButton"));
        objSelectAllButton->setMinimumSize(QSize(140, 32));
        objSelectAllButton->setMaximumSize(QSize(125, 32));

        gridLayout_22->addWidget(objSelectAllButton, 0, 0, 1, 1);

        objUnselectButton = new QPushButton(groupBox_3);
        objUnselectButton->setObjectName(QString::fromUtf8("objUnselectButton"));
        objUnselectButton->setMinimumSize(QSize(140, 32));
        objUnselectButton->setMaximumSize(QSize(125, 32));

        gridLayout_22->addWidget(objUnselectButton, 1, 1, 1, 1);

        objInvertSelectionButton = new QPushButton(groupBox_3);
        objInvertSelectionButton->setObjectName(QString::fromUtf8("objInvertSelectionButton"));
        objInvertSelectionButton->setMinimumSize(QSize(140, 32));
        objInvertSelectionButton->setMaximumSize(QSize(125, 32));

        gridLayout_22->addWidget(objInvertSelectionButton, 0, 1, 1, 1);


        gridLayout_10->addLayout(gridLayout_22, 1, 0, 1, 1);


        gridLayout_12->addWidget(groupBox_3, 1, 0, 1, 1);


        gridLayout_23->addLayout(gridLayout_12, 0, 0, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        groupBox_2 = new QGroupBox(wizardPage2);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(320, 70));
        gridLayout_25 = new QGridLayout(groupBox_2);
        gridLayout_25->setObjectName(QString::fromUtf8("gridLayout_25"));
        xscalinglist = new QComboBox(groupBox_2);
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

        gridLayout_25->addWidget(xscalinglist, 0, 0, 1, 1);


        gridLayout_5->addWidget(groupBox_2, 0, 0, 1, 3);

        XvariableGroupBox = new QGroupBox(wizardPage2);
        XvariableGroupBox->setObjectName(QString::fromUtf8("XvariableGroupBox"));
        sizePolicy.setHeightForWidth(XvariableGroupBox->sizePolicy().hasHeightForWidth());
        XvariableGroupBox->setSizePolicy(sizePolicy);
        XvariableGroupBox->setMinimumSize(QSize(306, 0));
        gridLayout_3 = new QGridLayout(XvariableGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        xvarSelectByLabel = new QComboBox(XvariableGroupBox);
        xvarSelectByLabel->addItem(QString());
        xvarSelectByLabel->setObjectName(QString::fromUtf8("xvarSelectByLabel"));
        xvarSelectByLabel->setMinimumSize(QSize(140, 32));
        xvarSelectByLabel->setMaximumSize(QSize(125, 32));

        gridLayout_4->addWidget(xvarSelectByLabel, 1, 0, 1, 1);

        xvarUnselectButton = new QPushButton(XvariableGroupBox);
        xvarUnselectButton->setObjectName(QString::fromUtf8("xvarUnselectButton"));
        xvarUnselectButton->setMinimumSize(QSize(140, 32));
        xvarUnselectButton->setMaximumSize(QSize(125, 32));

        gridLayout_4->addWidget(xvarUnselectButton, 1, 1, 1, 1);

        xvarSelectAllButton = new QPushButton(XvariableGroupBox);
        xvarSelectAllButton->setObjectName(QString::fromUtf8("xvarSelectAllButton"));
        xvarSelectAllButton->setMinimumSize(QSize(140, 32));
        xvarSelectAllButton->setMaximumSize(QSize(125, 32));

        gridLayout_4->addWidget(xvarSelectAllButton, 0, 0, 1, 1);

        xvarInvertSelectionButton = new QPushButton(XvariableGroupBox);
        xvarInvertSelectionButton->setObjectName(QString::fromUtf8("xvarInvertSelectionButton"));
        xvarInvertSelectionButton->setMinimumSize(QSize(140, 32));
        xvarInvertSelectionButton->setMaximumSize(QSize(125, 32));

        gridLayout_4->addWidget(xvarInvertSelectionButton, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_4, 1, 0, 1, 1);

        listView_4 = new QListView(XvariableGroupBox);
        listView_4->setObjectName(QString::fromUtf8("listView_4"));
        sizePolicy1.setHeightForWidth(listView_4->sizePolicy().hasHeightForWidth());
        listView_4->setSizePolicy(sizePolicy1);
        listView_4->setMinimumSize(QSize(140, 0));
        listView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_4->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_4->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_3->addWidget(listView_4, 0, 0, 1, 1);


        gridLayout_5->addWidget(XvariableGroupBox, 1, 0, 1, 3);


        gridLayout_23->addLayout(gridLayout_5, 0, 1, 1, 1);

        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        yScaling = new QGroupBox(wizardPage2);
        yScaling->setObjectName(QString::fromUtf8("yScaling"));
        yScaling->setMinimumSize(QSize(320, 70));
        gridLayout_2 = new QGridLayout(yScaling);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        yscalinglist = new QComboBox(yScaling);
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

        gridLayout_2->addWidget(yscalinglist, 0, 0, 1, 1);


        gridLayout_9->addWidget(yScaling, 0, 0, 1, 1);

        YvariableGroupBox = new QGroupBox(wizardPage2);
        YvariableGroupBox->setObjectName(QString::fromUtf8("YvariableGroupBox"));
        YvariableGroupBox->setMinimumSize(QSize(306, 315));
        gridLayout_6 = new QGridLayout(YvariableGroupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        listView_5 = new QListView(YvariableGroupBox);
        listView_5->setObjectName(QString::fromUtf8("listView_5"));
        sizePolicy1.setHeightForWidth(listView_5->sizePolicy().hasHeightForWidth());
        listView_5->setSizePolicy(sizePolicy1);
        listView_5->setMinimumSize(QSize(140, 0));
        listView_5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_5->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_5->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_6->addWidget(listView_5, 0, 0, 1, 2);

        yvarSelectAllButton = new QPushButton(YvariableGroupBox);
        yvarSelectAllButton->setObjectName(QString::fromUtf8("yvarSelectAllButton"));
        yvarSelectAllButton->setMinimumSize(QSize(140, 32));
        yvarSelectAllButton->setMaximumSize(QSize(125, 32));

        gridLayout_6->addWidget(yvarSelectAllButton, 1, 0, 1, 1);

        yvarInvertSelectionButton = new QPushButton(YvariableGroupBox);
        yvarInvertSelectionButton->setObjectName(QString::fromUtf8("yvarInvertSelectionButton"));
        yvarInvertSelectionButton->setMinimumSize(QSize(140, 32));
        yvarInvertSelectionButton->setMaximumSize(QSize(125, 32));

        gridLayout_6->addWidget(yvarInvertSelectionButton, 1, 1, 1, 1);

        yvarSelectByLabel = new QComboBox(YvariableGroupBox);
        yvarSelectByLabel->addItem(QString());
        yvarSelectByLabel->setObjectName(QString::fromUtf8("yvarSelectByLabel"));
        yvarSelectByLabel->setMinimumSize(QSize(140, 32));
        yvarSelectByLabel->setMaximumSize(QSize(125, 32));

        gridLayout_6->addWidget(yvarSelectByLabel, 2, 0, 1, 1);

        yvarUnselectButton = new QPushButton(YvariableGroupBox);
        yvarUnselectButton->setObjectName(QString::fromUtf8("yvarUnselectButton"));
        yvarUnselectButton->setMinimumSize(QSize(140, 32));
        yvarUnselectButton->setMaximumSize(QSize(125, 32));

        gridLayout_6->addWidget(yvarUnselectButton, 2, 1, 1, 1);


        gridLayout_9->addWidget(YvariableGroupBox, 1, 0, 1, 1);


        gridLayout_23->addLayout(gridLayout_9, 0, 2, 1, 1);

        YclassGroupBox = new QGroupBox(wizardPage2);
        YclassGroupBox->setObjectName(QString::fromUtf8("YclassGroupBox"));
        YclassGroupBox->setMinimumSize(QSize(662, 410));
        gridLayout_17 = new QGridLayout(YclassGroupBox);
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        class_SelectByLabel = new QComboBox(YclassGroupBox);
        class_SelectByLabel->addItem(QString());
        class_SelectByLabel->setObjectName(QString::fromUtf8("class_SelectByLabel"));
        class_SelectByLabel->setMinimumSize(QSize(160, 32));
        class_SelectByLabel->setMaximumSize(QSize(160, 32));

        gridLayout_16->addWidget(class_SelectByLabel, 0, 0, 1, 1);

        class_import = new QPushButton(YclassGroupBox);
        class_import->setObjectName(QString::fromUtf8("class_import"));
        class_import->setMinimumSize(QSize(160, 32));
        class_import->setMaximumSize(QSize(160, 32));
        class_import->setAutoRepeatDelay(12);

        gridLayout_16->addWidget(class_import, 1, 0, 1, 1);

        line = new QFrame(YclassGroupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_16->addWidget(line, 2, 0, 1, 1);

        class_addButton = new QPushButton(YclassGroupBox);
        class_addButton->setObjectName(QString::fromUtf8("class_addButton"));
        class_addButton->setMinimumSize(QSize(160, 32));
        class_addButton->setMaximumSize(QSize(160, 32));
        class_addButton->setAutoRepeatDelay(12);

        gridLayout_16->addWidget(class_addButton, 3, 0, 1, 1);

        class_removeButton = new QPushButton(YclassGroupBox);
        class_removeButton->setObjectName(QString::fromUtf8("class_removeButton"));
        class_removeButton->setMinimumSize(QSize(160, 32));
        class_removeButton->setMaximumSize(QSize(160, 32));
        class_removeButton->setAutoRepeatDelay(12);

        gridLayout_16->addWidget(class_removeButton, 4, 0, 1, 1);


        gridLayout_17->addLayout(gridLayout_16, 0, 1, 1, 1);

        modelsGroupBox = new QGroupBox(YclassGroupBox);
        modelsGroupBox->setObjectName(QString::fromUtf8("modelsGroupBox"));
        modelsGroupBox->setMinimumSize(QSize(222, 262));
        gridLayout_18 = new QGridLayout(modelsGroupBox);
        gridLayout_18->setObjectName(QString::fromUtf8("gridLayout_18"));
        listView_7 = new QListView(modelsGroupBox);
        listView_7->setObjectName(QString::fromUtf8("listView_7"));
        listView_7->setMinimumSize(QSize(140, 0));
        listView_7->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_7->setAlternatingRowColors(true);
        listView_7->setSelectionMode(QAbstractItemView::ExtendedSelection);
        listView_7->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_18->addWidget(listView_7, 0, 0, 1, 1);


        gridLayout_17->addWidget(modelsGroupBox, 0, 2, 2, 1);

        verticalSpacer = new QSpacerItem(20, 196, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_17->addItem(verticalSpacer, 1, 1, 1, 1);

        groupBox_4 = new QGroupBox(YclassGroupBox);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setMinimumSize(QSize(221, 262));
        gridLayout_15 = new QGridLayout(groupBox_4);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        listView_6 = new QListView(groupBox_4);
        listView_6->setObjectName(QString::fromUtf8("listView_6"));
        listView_6->setMinimumSize(QSize(140, 0));
        listView_6->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_6->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_15->addWidget(listView_6, 0, 0, 1, 1);


        gridLayout_17->addWidget(groupBox_4, 0, 0, 2, 1);


        gridLayout_23->addWidget(YclassGroupBox, 0, 3, 1, 1);

        ModelDialogWizard->addPage(wizardPage2);

        retranslateUi(ModelDialogWizard);

        xscalinglist->setCurrentIndex(1);
        yscalinglist->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ModelDialogWizard);
    } // setupUi

    void retranslateUi(QWizard *ModelDialogWizard)
    {
        ModelDialogWizard->setWindowTitle(QCoreApplication::translate("ModelDialogWizard", "Wizard", nullptr));
        label_5->setText(QCoreApplication::translate("ModelDialogWizard", "Model Name", nullptr));
        projectsGroupBox->setTitle(QCoreApplication::translate("ModelDialogWizard", "Projects", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("ModelDialogWizard", "Data", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ModelDialogWizard", "N. principal components", nullptr));
        ELearningMethodGroupBox->setTitle(QCoreApplication::translate("ModelDialogWizard", "Ensemble Leaning Method", nullptr));
        elmethodComboBox->setItemText(0, QCoreApplication::translate("ModelDialogWizard", "Bagging", nullptr));
        elmethodComboBox->setItemText(1, QCoreApplication::translate("ModelDialogWizard", "Fixed RSM", nullptr));
        elmethodComboBox->setItemText(2, QCoreApplication::translate("ModelDialogWizard", "Dynamic RSM", nullptr));

        label_3->setText(QCoreApplication::translate("ModelDialogWizard", "N. models", nullptr));
        label->setText(QCoreApplication::translate("ModelDialogWizard", "Train size", nullptr));
        label_2->setText(QCoreApplication::translate("ModelDialogWizard", "N. vars ", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("ModelDialogWizard", "Objects", nullptr));
        objSelectByLabel->setItemText(0, QCoreApplication::translate("ModelDialogWizard", "Select by label...", nullptr));

        objSelectAllButton->setText(QCoreApplication::translate("ModelDialogWizard", "Select All", nullptr));
        objUnselectButton->setText(QCoreApplication::translate("ModelDialogWizard", "Unselect", nullptr));
        objInvertSelectionButton->setText(QCoreApplication::translate("ModelDialogWizard", "Invert Selection", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ModelDialogWizard", "X scaling", nullptr));
        xscalinglist->setItemText(0, QCoreApplication::translate("ModelDialogWizard", "No Scaling", nullptr));
        xscalinglist->setItemText(1, QCoreApplication::translate("ModelDialogWizard", "SDEV Column Scaling", nullptr));
        xscalinglist->setItemText(2, QCoreApplication::translate("ModelDialogWizard", "RMS Column Scaling", nullptr));
        xscalinglist->setItemText(3, QCoreApplication::translate("ModelDialogWizard", "Pareto Scaling", nullptr));
        xscalinglist->setItemText(4, QCoreApplication::translate("ModelDialogWizard", "Range Scaling", nullptr));
        xscalinglist->setItemText(5, QCoreApplication::translate("ModelDialogWizard", "Level Scaling", nullptr));

        XvariableGroupBox->setTitle(QCoreApplication::translate("ModelDialogWizard", "X variables", nullptr));
        xvarSelectByLabel->setItemText(0, QCoreApplication::translate("ModelDialogWizard", "Select by label...", nullptr));

        xvarUnselectButton->setText(QCoreApplication::translate("ModelDialogWizard", "Unselect", nullptr));
        xvarSelectAllButton->setText(QCoreApplication::translate("ModelDialogWizard", "Select All", nullptr));
        xvarInvertSelectionButton->setText(QCoreApplication::translate("ModelDialogWizard", "Invert Selection", nullptr));
        yScaling->setTitle(QCoreApplication::translate("ModelDialogWizard", "Y scaling", nullptr));
        yscalinglist->setItemText(0, QCoreApplication::translate("ModelDialogWizard", "No Scaling", nullptr));
        yscalinglist->setItemText(1, QCoreApplication::translate("ModelDialogWizard", "SDEV Column Scaling", nullptr));
        yscalinglist->setItemText(2, QCoreApplication::translate("ModelDialogWizard", "RMS Column Scaling", nullptr));
        yscalinglist->setItemText(3, QCoreApplication::translate("ModelDialogWizard", "Pareto Scaling", nullptr));
        yscalinglist->setItemText(4, QCoreApplication::translate("ModelDialogWizard", "Range Scaling", nullptr));
        yscalinglist->setItemText(5, QCoreApplication::translate("ModelDialogWizard", "Level Scaling", nullptr));

        YvariableGroupBox->setTitle(QCoreApplication::translate("ModelDialogWizard", "Y variables", nullptr));
        yvarSelectAllButton->setText(QCoreApplication::translate("ModelDialogWizard", "Select All", nullptr));
        yvarInvertSelectionButton->setText(QCoreApplication::translate("ModelDialogWizard", "Invert Selection", nullptr));
        yvarSelectByLabel->setItemText(0, QCoreApplication::translate("ModelDialogWizard", "Select by label...", nullptr));

        yvarUnselectButton->setText(QCoreApplication::translate("ModelDialogWizard", "Unselect", nullptr));
        YclassGroupBox->setTitle(QCoreApplication::translate("ModelDialogWizard", "Y classes", nullptr));
        class_SelectByLabel->setItemText(0, QCoreApplication::translate("ModelDialogWizard", "Select by label...", nullptr));

        class_import->setText(QCoreApplication::translate("ModelDialogWizard", "Class from CSV...", nullptr));
        class_addButton->setText(QCoreApplication::translate("ModelDialogWizard", ">", nullptr));
        class_removeButton->setText(QCoreApplication::translate("ModelDialogWizard", "<", nullptr));
        modelsGroupBox->setTitle(QCoreApplication::translate("ModelDialogWizard", "Classes", nullptr));
        groupBox_4->setTitle(QCoreApplication::translate("ModelDialogWizard", "Objects", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelDialogWizard: public Ui_ModelDialogWizard {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELDIALOGWIZARD_H
