/********************************************************************************
** Form generated from reading UI file 'ValidatorDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VALIDATORDIALOG_H
#define UI_VALIDATORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ValidatorDialog
{
public:
    QGridLayout *gridLayout_8;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_5;
    QListView *listView;
    QGroupBox *modelsGroupBox;
    QGridLayout *gridLayout_2;
    QListView *listView_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_7;
    QRadioButton *averageCrule;
    QRadioButton *medianCrule;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_9;
    QRadioButton *leaveoneout;
    QGridLayout *gridLayout_4;
    QPushButton *loadclassButton;
    QRadioButton *kfoldcrossvalid;
    QLabel *label_2;
    QRadioButton *crossvalid;
    QGridLayout *gridLayout_3;
    QSpinBox *iterations;
    QLabel *label_5;
    QLabel *label_6;
    QSpinBox *groupnumber;
    QGroupBox *YScramblingGroupBox;
    QGridLayout *gridLayout_6;
    QSpinBox *n_yscrambling;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *ValidatorDialog)
    {
        if (ValidatorDialog->objectName().isEmpty())
            ValidatorDialog->setObjectName(QString::fromUtf8("ValidatorDialog"));
        ValidatorDialog->setWindowModality(Qt::WindowModal);
        ValidatorDialog->resize(832, 468);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ValidatorDialog->sizePolicy().hasHeightForWidth());
        ValidatorDialog->setSizePolicy(sizePolicy);
        ValidatorDialog->setMinimumSize(QSize(783, 320));
        ValidatorDialog->setMaximumSize(QSize(2080, 800));
        ValidatorDialog->setModal(true);
        gridLayout_8 = new QGridLayout(ValidatorDialog);
        gridLayout_8->setObjectName(QString::fromUtf8("gridLayout_8"));
        groupBox_2 = new QGroupBox(ValidatorDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(189, 261));
        gridLayout_5 = new QGridLayout(groupBox_2);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        listView = new QListView(groupBox_2);
        listView->setObjectName(QString::fromUtf8("listView"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy1);
        listView->setMinimumSize(QSize(0, 0));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setSelectionMode(QAbstractItemView::SingleSelection);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_5->addWidget(listView, 0, 0, 1, 1);


        gridLayout_8->addWidget(groupBox_2, 0, 0, 3, 1);

        modelsGroupBox = new QGroupBox(ValidatorDialog);
        modelsGroupBox->setObjectName(QString::fromUtf8("modelsGroupBox"));
        gridLayout_2 = new QGridLayout(modelsGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        listView_2 = new QListView(modelsGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setMinimumSize(QSize(0, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_2->addWidget(listView_2, 0, 0, 1, 1);


        gridLayout_8->addWidget(modelsGroupBox, 0, 1, 3, 1);

        groupBox = new QGroupBox(ValidatorDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_7 = new QGridLayout(groupBox);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        averageCrule = new QRadioButton(groupBox);
        averageCrule->setObjectName(QString::fromUtf8("averageCrule"));
        averageCrule->setChecked(true);

        gridLayout_7->addWidget(averageCrule, 0, 0, 1, 1);

        medianCrule = new QRadioButton(groupBox);
        medianCrule->setObjectName(QString::fromUtf8("medianCrule"));

        gridLayout_7->addWidget(medianCrule, 1, 0, 1, 1);


        gridLayout_8->addWidget(groupBox, 0, 2, 1, 1);

        groupBox_3 = new QGroupBox(ValidatorDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(334, 198));
        gridLayout_9 = new QGridLayout(groupBox_3);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        leaveoneout = new QRadioButton(groupBox_3);
        leaveoneout->setObjectName(QString::fromUtf8("leaveoneout"));
        leaveoneout->setChecked(true);

        gridLayout_9->addWidget(leaveoneout, 0, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        loadclassButton = new QPushButton(groupBox_3);
        loadclassButton->setObjectName(QString::fromUtf8("loadclassButton"));

        gridLayout_4->addWidget(loadclassButton, 0, 1, 1, 1);

        kfoldcrossvalid = new QRadioButton(groupBox_3);
        kfoldcrossvalid->setObjectName(QString::fromUtf8("kfoldcrossvalid"));

        gridLayout_4->addWidget(kfoldcrossvalid, 0, 0, 1, 1);

        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setMinimumSize(QSize(120, 34));

        gridLayout_4->addWidget(label_2, 1, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_4, 1, 0, 1, 1);

        crossvalid = new QRadioButton(groupBox_3);
        crossvalid->setObjectName(QString::fromUtf8("crossvalid"));

        gridLayout_9->addWidget(crossvalid, 2, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        iterations = new QSpinBox(groupBox_3);
        iterations->setObjectName(QString::fromUtf8("iterations"));
        iterations->setMinimum(1);
        iterations->setMaximum(9999999);
        iterations->setValue(20);

        gridLayout_3->addWidget(iterations, 1, 1, 1, 1);

        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 0, 0, 1, 1);

        label_6 = new QLabel(groupBox_3);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_3->addWidget(label_6, 1, 0, 1, 1);

        groupnumber = new QSpinBox(groupBox_3);
        groupnumber->setObjectName(QString::fromUtf8("groupnumber"));
        groupnumber->setMinimum(2);
        groupnumber->setMaximum(9999);
        groupnumber->setValue(5);

        gridLayout_3->addWidget(groupnumber, 0, 1, 1, 1);


        gridLayout_9->addLayout(gridLayout_3, 3, 0, 1, 1);


        gridLayout_8->addWidget(groupBox_3, 1, 2, 1, 1);

        YScramblingGroupBox = new QGroupBox(ValidatorDialog);
        YScramblingGroupBox->setObjectName(QString::fromUtf8("YScramblingGroupBox"));
        YScramblingGroupBox->setCheckable(true);
        YScramblingGroupBox->setChecked(false);
        gridLayout_6 = new QGridLayout(YScramblingGroupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        n_yscrambling = new QSpinBox(YScramblingGroupBox);
        n_yscrambling->setObjectName(QString::fromUtf8("n_yscrambling"));
        n_yscrambling->setMinimum(1);
        n_yscrambling->setMaximum(9999999);
        n_yscrambling->setValue(100);
        n_yscrambling->setDisplayIntegerBase(10);

        gridLayout_6->addWidget(n_yscrambling, 0, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(165, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_3, 0, 2, 1, 1);

        label = new QLabel(YScramblingGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_6->addWidget(label, 0, 0, 1, 1);


        gridLayout_8->addWidget(YScramblingGroupBox, 2, 2, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(ValidatorDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(cancelButton, 0, 2, 1, 1);

        okButton = new QPushButton(ValidatorDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(okButton, 0, 3, 1, 1);


        gridLayout_8->addLayout(gridLayout, 3, 0, 1, 3);


        retranslateUi(ValidatorDialog);

        QMetaObject::connectSlotsByName(ValidatorDialog);
    } // setupUi

    void retranslateUi(QDialog *ValidatorDialog)
    {
        ValidatorDialog->setWindowTitle(QCoreApplication::translate("ValidatorDialog", "Compute...", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ValidatorDialog", "Projects", nullptr));
        modelsGroupBox->setTitle(QCoreApplication::translate("ValidatorDialog", "Models", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ValidatorDialog", "Ensemble Learning Combination Rule", nullptr));
        averageCrule->setText(QCoreApplication::translate("ValidatorDialog", "Average", nullptr));
        medianCrule->setText(QCoreApplication::translate("ValidatorDialog", "Median", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("ValidatorDialog", "Validation Type ", nullptr));
        leaveoneout->setText(QCoreApplication::translate("ValidatorDialog", "Leave One Out", nullptr));
        loadclassButton->setText(QCoreApplication::translate("ValidatorDialog", "Load Class", nullptr));
        kfoldcrossvalid->setText(QCoreApplication::translate("ValidatorDialog", "K-fold Cross Validation", nullptr));
        label_2->setText(QCoreApplication::translate("ValidatorDialog", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'.SF NS Text'; font-size:13pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:12px; margin-bottom:12px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:9pt;\">N.B.: each line correspond<br />         to a compound list<br />         comma separated</span></p></body></html>", nullptr));
        crossvalid->setText(QCoreApplication::translate("ValidatorDialog", "Bootstrap Random Groups Cross Validation", nullptr));
        label_5->setText(QCoreApplication::translate("ValidatorDialog", "Groups:", nullptr));
        label_6->setText(QCoreApplication::translate("ValidatorDialog", "Iterations: ", nullptr));
        YScramblingGroupBox->setTitle(QCoreApplication::translate("ValidatorDialog", "Y Scrambling", nullptr));
        label->setText(QCoreApplication::translate("ValidatorDialog", "N. models ", nullptr));
        cancelButton->setText(QCoreApplication::translate("ValidatorDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ValidatorDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ValidatorDialog: public Ui_ValidatorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VALIDATORDIALOG_H
