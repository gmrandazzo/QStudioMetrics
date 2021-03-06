/********************************************************************************
** Form generated from reading UI file 'DoClusterAnalysisDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOCLUSTERANALYSISDIALOG_H
#define UI_DOCLUSTERANALYSISDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DoClusterAnalysisDialog
{
public:
    QGridLayout *gridLayout_16;
    QGroupBox *optionBox;
    QGridLayout *gridLayout_10;
    QGridLayout *gridLayout_9;
    QCheckBox *saveClusterLabelBox;
    QLineEdit *lineEdit;
    QGridLayout *gridLayout_4;
    QSpinBox *maxnobjectBox;
    QSpacerItem *horizontalSpacer_2;
    QCheckBox *extractObjectBox;
    QFrame *frame_2;
    QGridLayout *gridLayout_2;
    QRadioButton *farobjcentroidButton;
    QRadioButton *nearobjcentroidButton;
    QLabel *label_5;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_5;
    QListView *listView;
    QRadioButton *plotcoordinatesButton;
    QComboBox *layerlist;
    QRadioButton *sourcedataButton;
    QGroupBox *algorithmBox;
    QGridLayout *gridLayout;
    QRadioButton *kmeansrandom;
    QRadioButton *kmeanspp;
    QRadioButton *kmeansmdc;
    QRadioButton *kmeansmaxmindis;
    QFrame *line;
    QRadioButton *hclsinglelink;
    QRadioButton *hclcompletelink;
    QRadioButton *hclaveragelink;
    QRadioButton *hclwardlink;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *clustersBox;
    QGridLayout *gridLayout_15;
    QWidget *widget;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_8;
    QRadioButton *fixedClustersButton;
    QSpinBox *nclusterBox;
    QRadioButton *validationClustersButton;
    QGridLayout *gridLayout_12;
    QLabel *label_3;
    QSpinBox *maxnclustersBox;
    QSpacerItem *horizontalSpacer_4;
    QFrame *frame;
    QGridLayout *gridLayout_11;
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QSpinBox *groupBox;
    QLabel *label_2;
    QSpinBox *iterationBox;
    QSpacerItem *horizontalSpacer;
    QRadioButton *randomGroupCrossValidationButton;
    QRadioButton *jumpMethodButton;
    QGridLayout *gridLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *DoClusterAnalysisDialog)
    {
        if (DoClusterAnalysisDialog->objectName().isEmpty())
            DoClusterAnalysisDialog->setObjectName(QString::fromUtf8("DoClusterAnalysisDialog"));
        DoClusterAnalysisDialog->resize(854, 536);
        gridLayout_16 = new QGridLayout(DoClusterAnalysisDialog);
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        optionBox = new QGroupBox(DoClusterAnalysisDialog);
        optionBox->setObjectName(QString::fromUtf8("optionBox"));
        gridLayout_10 = new QGridLayout(optionBox);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        gridLayout_9 = new QGridLayout();
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        saveClusterLabelBox = new QCheckBox(optionBox);
        saveClusterLabelBox->setObjectName(QString::fromUtf8("saveClusterLabelBox"));

        gridLayout_9->addWidget(saveClusterLabelBox, 0, 0, 1, 1);

        lineEdit = new QLineEdit(optionBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(170, 24));

        gridLayout_9->addWidget(lineEdit, 0, 1, 1, 1);


        gridLayout_10->addLayout(gridLayout_9, 0, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        maxnobjectBox = new QSpinBox(optionBox);
        maxnobjectBox->setObjectName(QString::fromUtf8("maxnobjectBox"));
        maxnobjectBox->setMinimum(1);
        maxnobjectBox->setMaximum(999);

        gridLayout_4->addWidget(maxnobjectBox, 2, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(18, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_2, 2, 0, 1, 1);

        extractObjectBox = new QCheckBox(optionBox);
        extractObjectBox->setObjectName(QString::fromUtf8("extractObjectBox"));

        gridLayout_4->addWidget(extractObjectBox, 1, 0, 1, 5);

        frame_2 = new QFrame(optionBox);
        frame_2->setObjectName(QString::fromUtf8("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame_2);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        farobjcentroidButton = new QRadioButton(frame_2);
        farobjcentroidButton->setObjectName(QString::fromUtf8("farobjcentroidButton"));

        gridLayout_2->addWidget(farobjcentroidButton, 2, 0, 1, 1);

        nearobjcentroidButton = new QRadioButton(frame_2);
        nearobjcentroidButton->setObjectName(QString::fromUtf8("nearobjcentroidButton"));
        nearobjcentroidButton->setChecked(true);

        gridLayout_2->addWidget(nearobjcentroidButton, 1, 0, 1, 1);


        gridLayout_4->addWidget(frame_2, 2, 3, 1, 1);

        label_5 = new QLabel(optionBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 2, 1, 1, 1);


        gridLayout_10->addLayout(gridLayout_4, 1, 0, 1, 1);


        gridLayout_16->addWidget(optionBox, 0, 2, 2, 1);

        dataGroupBox = new QGroupBox(DoClusterAnalysisDialog);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        gridLayout_5 = new QGridLayout(dataGroupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        listView = new QListView(dataGroupBox);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setSelectionMode(QAbstractItemView::SingleSelection);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_5->addWidget(listView, 2, 0, 1, 1);

        plotcoordinatesButton = new QRadioButton(dataGroupBox);
        plotcoordinatesButton->setObjectName(QString::fromUtf8("plotcoordinatesButton"));
        plotcoordinatesButton->setChecked(true);

        gridLayout_5->addWidget(plotcoordinatesButton, 0, 0, 1, 1);

        layerlist = new QComboBox(dataGroupBox);
        layerlist->addItem(QString());
        layerlist->setObjectName(QString::fromUtf8("layerlist"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(layerlist->sizePolicy().hasHeightForWidth());
        layerlist->setSizePolicy(sizePolicy);
        layerlist->setMinimumSize(QSize(130, 24));
        layerlist->setMaximumSize(QSize(16777215, 16777215));
        layerlist->setEditable(false);

        gridLayout_5->addWidget(layerlist, 3, 0, 1, 1);

        sourcedataButton = new QRadioButton(dataGroupBox);
        sourcedataButton->setObjectName(QString::fromUtf8("sourcedataButton"));

        gridLayout_5->addWidget(sourcedataButton, 1, 0, 1, 1);


        gridLayout_16->addWidget(dataGroupBox, 0, 0, 4, 1);

        algorithmBox = new QGroupBox(DoClusterAnalysisDialog);
        algorithmBox->setObjectName(QString::fromUtf8("algorithmBox"));
        gridLayout = new QGridLayout(algorithmBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        kmeansrandom = new QRadioButton(algorithmBox);
        kmeansrandom->setObjectName(QString::fromUtf8("kmeansrandom"));

        gridLayout->addWidget(kmeansrandom, 0, 0, 1, 1);

        kmeanspp = new QRadioButton(algorithmBox);
        kmeanspp->setObjectName(QString::fromUtf8("kmeanspp"));
        kmeanspp->setChecked(true);

        gridLayout->addWidget(kmeanspp, 1, 0, 1, 1);

        kmeansmdc = new QRadioButton(algorithmBox);
        kmeansmdc->setObjectName(QString::fromUtf8("kmeansmdc"));

        gridLayout->addWidget(kmeansmdc, 2, 0, 1, 1);

        kmeansmaxmindis = new QRadioButton(algorithmBox);
        kmeansmaxmindis->setObjectName(QString::fromUtf8("kmeansmaxmindis"));

        gridLayout->addWidget(kmeansmaxmindis, 3, 0, 1, 1);

        line = new QFrame(algorithmBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 4, 0, 1, 1);

        hclsinglelink = new QRadioButton(algorithmBox);
        hclsinglelink->setObjectName(QString::fromUtf8("hclsinglelink"));

        gridLayout->addWidget(hclsinglelink, 5, 0, 1, 1);

        hclcompletelink = new QRadioButton(algorithmBox);
        hclcompletelink->setObjectName(QString::fromUtf8("hclcompletelink"));

        gridLayout->addWidget(hclcompletelink, 6, 0, 1, 1);

        hclaveragelink = new QRadioButton(algorithmBox);
        hclaveragelink->setObjectName(QString::fromUtf8("hclaveragelink"));

        gridLayout->addWidget(hclaveragelink, 7, 0, 1, 1);

        hclwardlink = new QRadioButton(algorithmBox);
        hclwardlink->setObjectName(QString::fromUtf8("hclwardlink"));

        gridLayout->addWidget(hclwardlink, 8, 0, 1, 1);


        gridLayout_16->addWidget(algorithmBox, 0, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 15, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_16->addItem(verticalSpacer_2, 3, 2, 1, 1);

        clustersBox = new QGroupBox(DoClusterAnalysisDialog);
        clustersBox->setObjectName(QString::fromUtf8("clustersBox"));
        gridLayout_15 = new QGridLayout(clustersBox);
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        widget = new QWidget(clustersBox);
        widget->setObjectName(QString::fromUtf8("widget"));
        gridLayout_13 = new QGridLayout(widget);
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout_8 = new QGridLayout();
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        fixedClustersButton = new QRadioButton(widget);
        fixedClustersButton->setObjectName(QString::fromUtf8("fixedClustersButton"));
        fixedClustersButton->setChecked(true);

        gridLayout_8->addWidget(fixedClustersButton, 0, 0, 1, 1);

        nclusterBox = new QSpinBox(widget);
        nclusterBox->setObjectName(QString::fromUtf8("nclusterBox"));
        nclusterBox->setMinimum(2);
        nclusterBox->setMaximum(9999);

        gridLayout_8->addWidget(nclusterBox, 0, 1, 1, 1);


        gridLayout_13->addLayout(gridLayout_8, 0, 0, 1, 1);

        validationClustersButton = new QRadioButton(widget);
        validationClustersButton->setObjectName(QString::fromUtf8("validationClustersButton"));

        gridLayout_13->addWidget(validationClustersButton, 1, 0, 1, 1);


        gridLayout_15->addWidget(widget, 0, 0, 1, 3);

        gridLayout_12 = new QGridLayout();
        gridLayout_12->setObjectName(QString::fromUtf8("gridLayout_12"));
        label_3 = new QLabel(clustersBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_12->addWidget(label_3, 0, 0, 1, 1);

        maxnclustersBox = new QSpinBox(clustersBox);
        maxnclustersBox->setObjectName(QString::fromUtf8("maxnclustersBox"));
        maxnclustersBox->setMinimum(2);
        maxnclustersBox->setMaximum(9999);
        maxnclustersBox->setValue(10);

        gridLayout_12->addWidget(maxnclustersBox, 0, 1, 1, 1);


        gridLayout_15->addLayout(gridLayout_12, 1, 1, 1, 2);

        horizontalSpacer_4 = new QSpacerItem(38, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_15->addItem(horizontalSpacer_4, 2, 0, 1, 1);

        frame = new QFrame(clustersBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_11 = new QGridLayout(frame);
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        groupBox = new QSpinBox(frame);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimum(2);
        groupBox->setMaximum(99999);
        groupBox->setValue(5);

        gridLayout_3->addWidget(groupBox, 0, 1, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_3->addWidget(label_2, 1, 0, 1, 1);

        iterationBox = new QSpinBox(frame);
        iterationBox->setObjectName(QString::fromUtf8("iterationBox"));
        iterationBox->setMinimum(1);
        iterationBox->setMaximum(99999);
        iterationBox->setValue(3);

        gridLayout_3->addWidget(iterationBox, 1, 1, 1, 1);


        gridLayout_7->addLayout(gridLayout_3, 1, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer, 1, 0, 1, 1);

        randomGroupCrossValidationButton = new QRadioButton(frame);
        randomGroupCrossValidationButton->setObjectName(QString::fromUtf8("randomGroupCrossValidationButton"));
        randomGroupCrossValidationButton->setChecked(true);

        gridLayout_7->addWidget(randomGroupCrossValidationButton, 0, 0, 1, 2);


        gridLayout_11->addLayout(gridLayout_7, 2, 0, 1, 1);

        jumpMethodButton = new QRadioButton(frame);
        jumpMethodButton->setObjectName(QString::fromUtf8("jumpMethodButton"));
        jumpMethodButton->setEnabled(true);
        jumpMethodButton->setChecked(false);

        gridLayout_11->addWidget(jumpMethodButton, 1, 0, 1, 1);


        gridLayout_15->addWidget(frame, 2, 1, 1, 2);


        gridLayout_16->addWidget(clustersBox, 2, 2, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        cancelButton = new QPushButton(DoClusterAnalysisDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(60, 0));
        cancelButton->setMaximumSize(QSize(60, 16777215));

        gridLayout_6->addWidget(cancelButton, 0, 1, 1, 1);

        okButton = new QPushButton(DoClusterAnalysisDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(60, 0));
        okButton->setMaximumSize(QSize(60, 16777215));

        gridLayout_6->addWidget(okButton, 0, 2, 1, 1);


        gridLayout_16->addLayout(gridLayout_6, 4, 0, 1, 3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_16->addItem(verticalSpacer, 2, 1, 1, 1);


        retranslateUi(DoClusterAnalysisDialog);

        QMetaObject::connectSlotsByName(DoClusterAnalysisDialog);
    } // setupUi

    void retranslateUi(QDialog *DoClusterAnalysisDialog)
    {
        DoClusterAnalysisDialog->setWindowTitle(QCoreApplication::translate("DoClusterAnalysisDialog", "Dialog", nullptr));
        optionBox->setTitle(QCoreApplication::translate("DoClusterAnalysisDialog", "Options", nullptr));
        saveClusterLabelBox->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Save Cluster Labels", nullptr));
        extractObjectBox->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Extract only objects...", nullptr));
        farobjcentroidButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Far from the centroid", nullptr));
        nearobjcentroidButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Near the centroid", nullptr));
        label_5->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Number of Objects", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("DoClusterAnalysisDialog", "Data", nullptr));
        plotcoordinatesButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Plot Coordinates", nullptr));
        layerlist->setItemText(0, QCoreApplication::translate("DoClusterAnalysisDialog", "layer list", nullptr));

        sourcedataButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Source Data", nullptr));
        algorithmBox->setTitle(QCoreApplication::translate("DoClusterAnalysisDialog", "Algorithms", nullptr));
        kmeansrandom->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "K-Means Random", nullptr));
        kmeanspp->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "K-Means++", nullptr));
        kmeansmdc->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "K-Means MDC", nullptr));
        kmeansmaxmindis->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "K-Means Max Dissimilarity", nullptr));
        hclsinglelink->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Hierarchical Single Linkage", nullptr));
        hclcompletelink->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Hierarchical Complete Linkage", nullptr));
        hclaveragelink->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Hierarchical Average Linkage", nullptr));
        hclwardlink->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Hierarchical Ward Linkage", nullptr));
        clustersBox->setTitle(QCoreApplication::translate("DoClusterAnalysisDialog", "Clusters", nullptr));
        fixedClustersButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Fixed number of clusters", nullptr));
        validationClustersButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Selection by validation", nullptr));
        label_3->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Max Number of Clusters", nullptr));
        label->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Groups", nullptr));
        label_2->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Iterations", nullptr));
        randomGroupCrossValidationButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Random Group Cross Validation", nullptr));
        jumpMethodButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Jump Method", nullptr));
        cancelButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("DoClusterAnalysisDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DoClusterAnalysisDialog: public Ui_DoClusterAnalysisDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOCLUSTERANALYSISDIALOG_H
