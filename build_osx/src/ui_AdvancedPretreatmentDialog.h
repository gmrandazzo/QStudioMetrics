/********************************************************************************
** Form generated from reading UI file 'AdvancedPretreatmentDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADVANCEDPRETREATMENTDIALOG_H
#define UI_ADVANCEDPRETREATMENTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_AdvancedPretreatmentDialog
{
public:
    QGridLayout *gridLayout_10;
    QGroupBox *projectsGroupBox;
    QGridLayout *gridLayout;
    QListView *listView;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_3;
    QListView *listView_2;
    QGroupBox *variableGroupBox;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QPushButton *selectAllButton;
    QPushButton *invertSelectionButton;
    QComboBox *selectByLabelButton;
    QPushButton *unselectButton;
    QRadioButton *addNoModVars;
    QListView *listView_3;
    QRadioButton *ExcludeNoModVars;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_9;
    QSpacerItem *horizontalSpacer_4;
    QGridLayout *gridLayout_7;
    QRadioButton *logTransform;
    QRadioButton *squareTransform;
    QRadioButton *sqrtTransform;
    QGridLayout *gridLayout_2;
    QRadioButton *absTransform;
    QRadioButton *rowcenteringScaling;
    QRadioButton *SVNScaling;
    QLabel *label_5;
    QLineEdit *dataname;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *AdvancedPretreatmentDialog)
    {
        if (AdvancedPretreatmentDialog->objectName().isEmpty())
            AdvancedPretreatmentDialog->setObjectName(QString::fromUtf8("AdvancedPretreatmentDialog"));
        AdvancedPretreatmentDialog->resize(894, 606);
        gridLayout_10 = new QGridLayout(AdvancedPretreatmentDialog);
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        projectsGroupBox = new QGroupBox(AdvancedPretreatmentDialog);
        projectsGroupBox->setObjectName(QString::fromUtf8("projectsGroupBox"));
        gridLayout = new QGridLayout(projectsGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listView = new QListView(projectsGroupBox);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setSelectionMode(QAbstractItemView::SingleSelection);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(listView, 0, 0, 1, 1);


        gridLayout_10->addWidget(projectsGroupBox, 0, 0, 1, 2);

        dataGroupBox = new QGroupBox(AdvancedPretreatmentDialog);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        gridLayout_3 = new QGridLayout(dataGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listView_2 = new QListView(dataGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_3->addWidget(listView_2, 0, 0, 1, 2);


        gridLayout_10->addWidget(dataGroupBox, 0, 2, 1, 2);

        variableGroupBox = new QGroupBox(AdvancedPretreatmentDialog);
        variableGroupBox->setObjectName(QString::fromUtf8("variableGroupBox"));
        gridLayout_6 = new QGridLayout(variableGroupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        selectAllButton = new QPushButton(variableGroupBox);
        selectAllButton->setObjectName(QString::fromUtf8("selectAllButton"));
        selectAllButton->setMinimumSize(QSize(130, 32));
        selectAllButton->setMaximumSize(QSize(16777215, 32));

        gridLayout_5->addWidget(selectAllButton, 0, 0, 1, 1);

        invertSelectionButton = new QPushButton(variableGroupBox);
        invertSelectionButton->setObjectName(QString::fromUtf8("invertSelectionButton"));
        invertSelectionButton->setMinimumSize(QSize(130, 32));
        invertSelectionButton->setMaximumSize(QSize(16777215, 32));

        gridLayout_5->addWidget(invertSelectionButton, 0, 1, 1, 1);

        selectByLabelButton = new QComboBox(variableGroupBox);
        selectByLabelButton->setObjectName(QString::fromUtf8("selectByLabelButton"));
        selectByLabelButton->setMinimumSize(QSize(130, 32));
        selectByLabelButton->setMaximumSize(QSize(130, 32));

        gridLayout_5->addWidget(selectByLabelButton, 1, 0, 1, 1);

        unselectButton = new QPushButton(variableGroupBox);
        unselectButton->setObjectName(QString::fromUtf8("unselectButton"));
        unselectButton->setMinimumSize(QSize(130, 32));
        unselectButton->setMaximumSize(QSize(16777215, 32));

        gridLayout_5->addWidget(unselectButton, 1, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 1, 0, 1, 1);

        addNoModVars = new QRadioButton(variableGroupBox);
        addNoModVars->setObjectName(QString::fromUtf8("addNoModVars"));
        addNoModVars->setChecked(true);

        gridLayout_6->addWidget(addNoModVars, 2, 0, 1, 1);

        listView_3 = new QListView(variableGroupBox);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        listView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_3->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_3->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_6->addWidget(listView_3, 0, 0, 1, 1);

        ExcludeNoModVars = new QRadioButton(variableGroupBox);
        ExcludeNoModVars->setObjectName(QString::fromUtf8("ExcludeNoModVars"));

        gridLayout_6->addWidget(ExcludeNoModVars, 3, 0, 1, 1);


        gridLayout_10->addWidget(variableGroupBox, 0, 4, 1, 1);

        groupBox = new QGroupBox(AdvancedPretreatmentDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_9 = new QGridLayout(groupBox);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        horizontalSpacer_4 = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer_4, 0, 2, 1, 1);

        gridLayout_7 = new QGridLayout();
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        logTransform = new QRadioButton(groupBox);
        logTransform->setObjectName(QString::fromUtf8("logTransform"));
        logTransform->setChecked(true);

        gridLayout_7->addWidget(logTransform, 0, 0, 1, 1);

        squareTransform = new QRadioButton(groupBox);
        squareTransform->setObjectName(QString::fromUtf8("squareTransform"));

        gridLayout_7->addWidget(squareTransform, 1, 0, 1, 1);

        sqrtTransform = new QRadioButton(groupBox);
        sqrtTransform->setObjectName(QString::fromUtf8("sqrtTransform"));

        gridLayout_7->addWidget(sqrtTransform, 2, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_7, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        absTransform = new QRadioButton(groupBox);
        absTransform->setObjectName(QString::fromUtf8("absTransform"));

        gridLayout_2->addWidget(absTransform, 0, 0, 1, 1);

        rowcenteringScaling = new QRadioButton(groupBox);
        rowcenteringScaling->setObjectName(QString::fromUtf8("rowcenteringScaling"));

        gridLayout_2->addWidget(rowcenteringScaling, 1, 0, 1, 1);

        SVNScaling = new QRadioButton(groupBox);
        SVNScaling->setObjectName(QString::fromUtf8("SVNScaling"));

        gridLayout_2->addWidget(SVNScaling, 2, 0, 1, 1);


        gridLayout_9->addLayout(gridLayout_2, 0, 1, 1, 1);


        gridLayout_10->addWidget(groupBox, 1, 0, 1, 5);

        label_5 = new QLabel(AdvancedPretreatmentDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_10->addWidget(label_5, 2, 0, 1, 1);

        dataname = new QLineEdit(AdvancedPretreatmentDialog);
        dataname->setObjectName(QString::fromUtf8("dataname"));
        dataname->setMinimumSize(QSize(200, 20));

        gridLayout_10->addWidget(dataname, 2, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(342, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_10->addItem(horizontalSpacer, 2, 3, 1, 2);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        cancelButton = new QPushButton(AdvancedPretreatmentDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 32));
        cancelButton->setMaximumSize(QSize(100, 32));

        gridLayout_4->addWidget(cancelButton, 0, 1, 1, 1);

        okButton = new QPushButton(AdvancedPretreatmentDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 32));
        okButton->setMaximumSize(QSize(100, 32));

        gridLayout_4->addWidget(okButton, 0, 2, 1, 1);


        gridLayout_10->addLayout(gridLayout_4, 3, 0, 1, 5);


        retranslateUi(AdvancedPretreatmentDialog);

        QMetaObject::connectSlotsByName(AdvancedPretreatmentDialog);
    } // setupUi

    void retranslateUi(QDialog *AdvancedPretreatmentDialog)
    {
        AdvancedPretreatmentDialog->setWindowTitle(QCoreApplication::translate("AdvancedPretreatmentDialog", "Advanced Pretreatment", nullptr));
        projectsGroupBox->setTitle(QCoreApplication::translate("AdvancedPretreatmentDialog", "Projects", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("AdvancedPretreatmentDialog", "Data", nullptr));
        variableGroupBox->setTitle(QCoreApplication::translate("AdvancedPretreatmentDialog", "Variables", nullptr));
        selectAllButton->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Select All", nullptr));
        invertSelectionButton->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Invert Selection", nullptr));
        unselectButton->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Unselect All", nullptr));
        addNoModVars->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Add not selected variables \"as is\"", nullptr));
        ExcludeNoModVars->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Exclude not selected variables", nullptr));
        groupBox->setTitle(QCoreApplication::translate("AdvancedPretreatmentDialog", "Advanced Pretreatment", nullptr));
        logTransform->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Log10(x+1) Transform", nullptr));
        squareTransform->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Square(x) Transform", nullptr));
        sqrtTransform->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "SQRT(x) Transform", nullptr));
        absTransform->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "ABS(x) Transform", nullptr));
        rowcenteringScaling->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Row Centering Scaling", nullptr));
        SVNScaling->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "SVN Scaling", nullptr));
        label_5->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Scaled Data Name", nullptr));
        cancelButton->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("AdvancedPretreatmentDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AdvancedPretreatmentDialog: public Ui_AdvancedPretreatmentDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADVANCEDPRETREATMENTDIALOG_H
