/********************************************************************************
** Form generated from reading UI file 'DoPredictionDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DOPREDICTIONDIALOG_H
#define UI_DOPREDICTIONDIALOG_H

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
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_DoPredictionDialog
{
public:
    QGridLayout *gridLayout_9;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_3;
    QListView *listView_3;
    QFrame *prednameFrame;
    QGridLayout *gridLayout_8;
    QLabel *label;
    QLineEdit *predname;
    QGroupBox *projectGroupBox;
    QGridLayout *gridLayout;
    QListView *listView;
    QGroupBox *modelGroupBox;
    QGridLayout *gridLayout_2;
    QListView *listView_2;
    QGroupBox *objectGroupBox;
    QGridLayout *gridLayout_5;
    QListView *listView_4;
    QPushButton *objSelectAllButton;
    QPushButton *objInvertSelectionButton;
    QComboBox *objSelectByLabel;
    QPushButton *objUnselectButton;
    QGroupBox *YvariableGroupBox;
    QGridLayout *gridLayout_6;
    QPushButton *yvarInvertSelectionButton;
    QListView *listView_5;
    QPushButton *yvarSelectAllButton;
    QComboBox *yvarSelectByLabel;
    QPushButton *yvarUnselectButton;
    QPushButton *pushButton;
    QPlainTextEdit *plainTextEdit;
    QGridLayout *gridLayout_7;
    QPushButton *nextButton;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QPushButton *prevButton;

    void setupUi(QDialog *DoPredictionDialog)
    {
        if (DoPredictionDialog->objectName().isEmpty())
            DoPredictionDialog->setObjectName(QString::fromUtf8("DoPredictionDialog"));
        DoPredictionDialog->resize(1628, 655);
        gridLayout_9 = new QGridLayout(DoPredictionDialog);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        dataGroupBox = new QGroupBox(DoPredictionDialog);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(dataGroupBox->sizePolicy().hasHeightForWidth());
        dataGroupBox->setSizePolicy(sizePolicy);
        gridLayout_3 = new QGridLayout(dataGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listView_3 = new QListView(dataGroupBox);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        listView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_3->addWidget(listView_3, 0, 0, 1, 1);


        gridLayout_9->addWidget(dataGroupBox, 1, 2, 1, 1);

        prednameFrame = new QFrame(DoPredictionDialog);
        prednameFrame->setObjectName(QString::fromUtf8("prednameFrame"));
        prednameFrame->setFrameShape(QFrame::StyledPanel);
        prednameFrame->setFrameShadow(QFrame::Plain);
        gridLayout_8 = new QGridLayout(prednameFrame);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        label = new QLabel(prednameFrame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_8->addWidget(label, 0, 0, 1, 1);

        predname = new QLineEdit(prednameFrame);
        predname->setObjectName(QString::fromUtf8("predname"));

        gridLayout_8->addWidget(predname, 0, 1, 1, 1);


        gridLayout_9->addWidget(prednameFrame, 0, 0, 1, 5);

        projectGroupBox = new QGroupBox(DoPredictionDialog);
        projectGroupBox->setObjectName(QString::fromUtf8("projectGroupBox"));
        sizePolicy.setHeightForWidth(projectGroupBox->sizePolicy().hasHeightForWidth());
        projectGroupBox->setSizePolicy(sizePolicy);
        gridLayout = new QGridLayout(projectGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listView = new QListView(projectGroupBox);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout->addWidget(listView, 0, 0, 1, 1);


        gridLayout_9->addWidget(projectGroupBox, 1, 0, 1, 1);

        modelGroupBox = new QGroupBox(DoPredictionDialog);
        modelGroupBox->setObjectName(QString::fromUtf8("modelGroupBox"));
        sizePolicy.setHeightForWidth(modelGroupBox->sizePolicy().hasHeightForWidth());
        modelGroupBox->setSizePolicy(sizePolicy);
        gridLayout_2 = new QGridLayout(modelGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        listView_2 = new QListView(modelGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

        gridLayout_2->addWidget(listView_2, 0, 0, 1, 1);


        gridLayout_9->addWidget(modelGroupBox, 1, 1, 1, 1);

        objectGroupBox = new QGroupBox(DoPredictionDialog);
        objectGroupBox->setObjectName(QString::fromUtf8("objectGroupBox"));
        sizePolicy.setHeightForWidth(objectGroupBox->sizePolicy().hasHeightForWidth());
        objectGroupBox->setSizePolicy(sizePolicy);
        gridLayout_5 = new QGridLayout(objectGroupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        listView_4 = new QListView(objectGroupBox);
        listView_4->setObjectName(QString::fromUtf8("listView_4"));
        listView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_4->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_5->addWidget(listView_4, 0, 0, 1, 2);

        objSelectAllButton = new QPushButton(objectGroupBox);
        objSelectAllButton->setObjectName(QString::fromUtf8("objSelectAllButton"));
        objSelectAllButton->setMinimumSize(QSize(130, 24));
        objSelectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(objSelectAllButton, 1, 0, 1, 1);

        objInvertSelectionButton = new QPushButton(objectGroupBox);
        objInvertSelectionButton->setObjectName(QString::fromUtf8("objInvertSelectionButton"));
        objInvertSelectionButton->setMinimumSize(QSize(131, 24));
        objInvertSelectionButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(objInvertSelectionButton, 1, 1, 1, 1);

        objSelectByLabel = new QComboBox(objectGroupBox);
        objSelectByLabel->addItem(QString());
        objSelectByLabel->setObjectName(QString::fromUtf8("objSelectByLabel"));
        objSelectByLabel->setMinimumSize(QSize(130, 0));
        objSelectByLabel->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(objSelectByLabel, 2, 0, 1, 1);

        objUnselectButton = new QPushButton(objectGroupBox);
        objUnselectButton->setObjectName(QString::fromUtf8("objUnselectButton"));
        objUnselectButton->setMinimumSize(QSize(130, 24));
        objUnselectButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(objUnselectButton, 2, 1, 1, 1);


        gridLayout_9->addWidget(objectGroupBox, 1, 3, 1, 1);

        YvariableGroupBox = new QGroupBox(DoPredictionDialog);
        YvariableGroupBox->setObjectName(QString::fromUtf8("YvariableGroupBox"));
        sizePolicy.setHeightForWidth(YvariableGroupBox->sizePolicy().hasHeightForWidth());
        YvariableGroupBox->setSizePolicy(sizePolicy);
        YvariableGroupBox->setCheckable(true);
        YvariableGroupBox->setChecked(false);
        gridLayout_6 = new QGridLayout(YvariableGroupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        yvarInvertSelectionButton = new QPushButton(YvariableGroupBox);
        yvarInvertSelectionButton->setObjectName(QString::fromUtf8("yvarInvertSelectionButton"));
        yvarInvertSelectionButton->setMinimumSize(QSize(131, 24));
        yvarInvertSelectionButton->setMaximumSize(QSize(130, 24));

        gridLayout_6->addWidget(yvarInvertSelectionButton, 3, 1, 1, 1);

        listView_5 = new QListView(YvariableGroupBox);
        listView_5->setObjectName(QString::fromUtf8("listView_5"));
        listView_5->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_5->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_6->addWidget(listView_5, 2, 0, 1, 2);

        yvarSelectAllButton = new QPushButton(YvariableGroupBox);
        yvarSelectAllButton->setObjectName(QString::fromUtf8("yvarSelectAllButton"));
        yvarSelectAllButton->setMinimumSize(QSize(130, 24));
        yvarSelectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout_6->addWidget(yvarSelectAllButton, 3, 0, 1, 1);

        yvarSelectByLabel = new QComboBox(YvariableGroupBox);
        yvarSelectByLabel->addItem(QString());
        yvarSelectByLabel->setObjectName(QString::fromUtf8("yvarSelectByLabel"));
        yvarSelectByLabel->setMinimumSize(QSize(130, 0));
        yvarSelectByLabel->setMaximumSize(QSize(130, 24));

        gridLayout_6->addWidget(yvarSelectByLabel, 4, 0, 1, 1);

        yvarUnselectButton = new QPushButton(YvariableGroupBox);
        yvarUnselectButton->setObjectName(QString::fromUtf8("yvarUnselectButton"));
        yvarUnselectButton->setMinimumSize(QSize(130, 24));
        yvarUnselectButton->setMaximumSize(QSize(130, 24));

        gridLayout_6->addWidget(yvarUnselectButton, 4, 1, 1, 1);

        pushButton = new QPushButton(YvariableGroupBox);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));

        gridLayout_6->addWidget(pushButton, 0, 0, 1, 1);

        plainTextEdit = new QPlainTextEdit(YvariableGroupBox);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout_6->addWidget(plainTextEdit, 1, 0, 1, 1);


        gridLayout_9->addWidget(YvariableGroupBox, 1, 4, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        nextButton = new QPushButton(DoPredictionDialog);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setMinimumSize(QSize(100, 24));
        nextButton->setMaximumSize(QSize(100, 24));

        gridLayout_7->addWidget(nextButton, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        cancelButton = new QPushButton(DoPredictionDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout_7->addWidget(cancelButton, 0, 3, 1, 1);

        okButton = new QPushButton(DoPredictionDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout_7->addWidget(okButton, 0, 4, 1, 1);

        prevButton = new QPushButton(DoPredictionDialog);
        prevButton->setObjectName(QString::fromUtf8("prevButton"));
        prevButton->setMinimumSize(QSize(100, 24));
        prevButton->setMaximumSize(QSize(100, 24));

        gridLayout_7->addWidget(prevButton, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_7, 2, 0, 1, 5);


        retranslateUi(DoPredictionDialog);

        QMetaObject::connectSlotsByName(DoPredictionDialog);
    } // setupUi

    void retranslateUi(QDialog *DoPredictionDialog)
    {
        DoPredictionDialog->setWindowTitle(QCoreApplication::translate("DoPredictionDialog", "Dialog", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("DoPredictionDialog", "Data", nullptr));
        label->setText(QCoreApplication::translate("DoPredictionDialog", "Prediction Name", nullptr));
        projectGroupBox->setTitle(QCoreApplication::translate("DoPredictionDialog", "Projects", nullptr));
        modelGroupBox->setTitle(QCoreApplication::translate("DoPredictionDialog", "Models", nullptr));
        objectGroupBox->setTitle(QCoreApplication::translate("DoPredictionDialog", "Objects", nullptr));
        objSelectAllButton->setText(QCoreApplication::translate("DoPredictionDialog", "Select All", nullptr));
        objInvertSelectionButton->setText(QCoreApplication::translate("DoPredictionDialog", "Invert Selection", nullptr));
        objSelectByLabel->setItemText(0, QCoreApplication::translate("DoPredictionDialog", "Select by label...", nullptr));

        objUnselectButton->setText(QCoreApplication::translate("DoPredictionDialog", "Unselect", nullptr));
        YvariableGroupBox->setTitle(QCoreApplication::translate("DoPredictionDialog", "Y Variables", nullptr));
        yvarInvertSelectionButton->setText(QCoreApplication::translate("DoPredictionDialog", "Invert Selection", nullptr));
        yvarSelectAllButton->setText(QCoreApplication::translate("DoPredictionDialog", "Select All", nullptr));
        yvarSelectByLabel->setItemText(0, QCoreApplication::translate("DoPredictionDialog", "Select by label...", nullptr));

        yvarUnselectButton->setText(QCoreApplication::translate("DoPredictionDialog", "Unselect", nullptr));
        pushButton->setText(QCoreApplication::translate("DoPredictionDialog", "Import target", nullptr));
        nextButton->setText(QCoreApplication::translate("DoPredictionDialog", "Next", nullptr));
        cancelButton->setText(QCoreApplication::translate("DoPredictionDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("DoPredictionDialog", "OK", nullptr));
        prevButton->setText(QCoreApplication::translate("DoPredictionDialog", "Prev", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DoPredictionDialog: public Ui_DoPredictionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DOPREDICTIONDIALOG_H
