/********************************************************************************
** Form generated from reading UI file 'SelectByDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTBYDIALOG_H
#define UI_SELECTBYDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_SelectByDialog
{
public:
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout_10;
    QSpacerItem *horizontalSpacer_12;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *findButton;
    QLabel *selobjectlabel;
    QLineEdit *searchline;
    QListView *objnamelistView;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_4;
    QFrame *frame;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer_7;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *maxvaledit;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *minvaledit;
    QRadioButton *advancedSelector;
    QRadioButton *variableSelector;
    QPushButton *openSelectorButton;
    QComboBox *varlist;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QProgressBar *progressBar;
    QPushButton *abortButton;
    QFrame *buttonframe;
    QGridLayout *gridLayout_5;
    QPushButton *selectButton;
    QPushButton *invertselectButton;
    QPushButton *appendselecttionButton;
    QPushButton *unselectButton;

    void setupUi(QDialog *SelectByDialog)
    {
        if (SelectByDialog->objectName().isEmpty())
            SelectByDialog->setObjectName(QString::fromUtf8("SelectByDialog"));
        SelectByDialog->resize(832, 490);
        SelectByDialog->setMinimumSize(QSize(832, 462));
        gridLayout_7 = new QGridLayout(SelectByDialog);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout_10 = new QGridLayout();
        gridLayout_10->setObjectName(QString::fromUtf8("gridLayout_10"));
        horizontalSpacer_12 = new QSpacerItem(382, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_10->addItem(horizontalSpacer_12, 0, 0, 1, 1);

        cancelButton = new QPushButton(SelectByDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout_10->addWidget(cancelButton, 0, 1, 1, 1);

        okButton = new QPushButton(SelectByDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout_10->addWidget(okButton, 0, 2, 1, 1);


        gridLayout_7->addLayout(gridLayout_10, 1, 0, 1, 2);

        groupBox_2 = new QGroupBox(SelectByDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(401, 361));
        gridLayout = new QGridLayout(groupBox_2);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(128, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 2, 0, 1, 2);

        findButton = new QPushButton(groupBox_2);
        findButton->setObjectName(QString::fromUtf8("findButton"));
        findButton->setMinimumSize(QSize(100, 24));
        findButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(findButton, 0, 1, 1, 2);

        selobjectlabel = new QLabel(groupBox_2);
        selobjectlabel->setObjectName(QString::fromUtf8("selobjectlabel"));

        gridLayout->addWidget(selobjectlabel, 2, 2, 1, 1);

        searchline = new QLineEdit(groupBox_2);
        searchline->setObjectName(QString::fromUtf8("searchline"));

        gridLayout->addWidget(searchline, 0, 0, 1, 1);

        objnamelistView = new QListView(groupBox_2);
        objnamelistView->setObjectName(QString::fromUtf8("objnamelistView"));
        objnamelistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        objnamelistView->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout->addWidget(objnamelistView, 1, 0, 1, 3);


        gridLayout_7->addWidget(groupBox_2, 0, 1, 1, 1);

        groupBox = new QGroupBox(SelectByDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(401, 363));
        groupBox->setMaximumSize(QSize(401, 431));
        gridLayout_4 = new QGridLayout(groupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        frame = new QFrame(groupBox);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setMaximumSize(QSize(16777215, 16777215));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(frame);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalSpacer_7 = new QSpacerItem(194, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_7, 1, 1, 1, 2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 0, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        maxvaledit = new QLineEdit(frame);
        maxvaledit->setObjectName(QString::fromUtf8("maxvaledit"));

        gridLayout_2->addWidget(maxvaledit, 1, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 1, 1, 1, 1);

        minvaledit = new QLineEdit(frame);
        minvaledit->setObjectName(QString::fromUtf8("minvaledit"));

        gridLayout_2->addWidget(minvaledit, 0, 2, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 4, 0, 1, 3);

        advancedSelector = new QRadioButton(frame);
        advancedSelector->setObjectName(QString::fromUtf8("advancedSelector"));
        advancedSelector->setChecked(true);

        gridLayout_3->addWidget(advancedSelector, 0, 0, 1, 2);

        variableSelector = new QRadioButton(frame);
        variableSelector->setObjectName(QString::fromUtf8("variableSelector"));

        gridLayout_3->addWidget(variableSelector, 2, 0, 1, 2);

        openSelectorButton = new QPushButton(frame);
        openSelectorButton->setObjectName(QString::fromUtf8("openSelectorButton"));
        openSelectorButton->setMinimumSize(QSize(130, 24));

        gridLayout_3->addWidget(openSelectorButton, 1, 0, 1, 1);

        varlist = new QComboBox(frame);
        varlist->addItem(QString());
        varlist->setObjectName(QString::fromUtf8("varlist"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(varlist->sizePolicy().hasHeightForWidth());
        varlist->setSizePolicy(sizePolicy);
        varlist->setMinimumSize(QSize(140, 24));
        varlist->setMaximumSize(QSize(16777215, 24));
        varlist->setEditable(false);

        gridLayout_3->addWidget(varlist, 3, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(184, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 3, 2, 1, 1);


        gridLayout_4->addWidget(frame, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        progressBar = new QProgressBar(groupBox);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        horizontalLayout_2->addWidget(progressBar);

        abortButton = new QPushButton(groupBox);
        abortButton->setObjectName(QString::fromUtf8("abortButton"));

        horizontalLayout_2->addWidget(abortButton);


        gridLayout_4->addLayout(horizontalLayout_2, 2, 0, 1, 1);

        buttonframe = new QFrame(groupBox);
        buttonframe->setObjectName(QString::fromUtf8("buttonframe"));
        buttonframe->setMaximumSize(QSize(16777215, 100));
        buttonframe->setFrameShape(QFrame::StyledPanel);
        buttonframe->setFrameShadow(QFrame::Raised);
        gridLayout_5 = new QGridLayout(buttonframe);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        selectButton = new QPushButton(buttonframe);
        selectButton->setObjectName(QString::fromUtf8("selectButton"));
        selectButton->setMinimumSize(QSize(140, 24));
        selectButton->setMaximumSize(QSize(140, 24));

        gridLayout_5->addWidget(selectButton, 0, 0, 1, 1);

        invertselectButton = new QPushButton(buttonframe);
        invertselectButton->setObjectName(QString::fromUtf8("invertselectButton"));
        invertselectButton->setMinimumSize(QSize(140, 24));
        invertselectButton->setMaximumSize(QSize(140, 24));

        gridLayout_5->addWidget(invertselectButton, 0, 1, 1, 1);

        appendselecttionButton = new QPushButton(buttonframe);
        appendselecttionButton->setObjectName(QString::fromUtf8("appendselecttionButton"));
        appendselecttionButton->setMinimumSize(QSize(140, 24));
        appendselecttionButton->setMaximumSize(QSize(140, 24));

        gridLayout_5->addWidget(appendselecttionButton, 1, 0, 1, 1);

        unselectButton = new QPushButton(buttonframe);
        unselectButton->setObjectName(QString::fromUtf8("unselectButton"));
        unselectButton->setMinimumSize(QSize(140, 24));
        unselectButton->setMaximumSize(QSize(140, 24));

        gridLayout_5->addWidget(unselectButton, 1, 1, 1, 1);


        gridLayout_4->addWidget(buttonframe, 1, 0, 1, 1);


        gridLayout_7->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(SelectByDialog);

        QMetaObject::connectSlotsByName(SelectByDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectByDialog)
    {
        SelectByDialog->setWindowTitle(QCoreApplication::translate("SelectByDialog", "Select by ...", nullptr));
        cancelButton->setText(QCoreApplication::translate("SelectByDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("SelectByDialog", "OK", nullptr));
        groupBox_2->setTitle(QString());
        findButton->setText(QCoreApplication::translate("SelectByDialog", "Find", nullptr));
        selobjectlabel->setText(QCoreApplication::translate("SelectByDialog", "TextLabel", nullptr));
        groupBox->setTitle(QCoreApplication::translate("SelectByDialog", "Select by...", nullptr));
        label_2->setText(QCoreApplication::translate("SelectByDialog", "Max (x \342\211\244):", nullptr));
        label->setText(QCoreApplication::translate("SelectByDialog", "Min (x \342\211\245):", nullptr));
        advancedSelector->setText(QCoreApplication::translate("SelectByDialog", "Advanced Selector", nullptr));
        variableSelector->setText(QCoreApplication::translate("SelectByDialog", "Variable in Matrix", nullptr));
        openSelectorButton->setText(QCoreApplication::translate("SelectByDialog", "Open Selector", nullptr));
        varlist->setItemText(0, QCoreApplication::translate("SelectByDialog", "Variable...", nullptr));

        label_3->setText(QCoreApplication::translate("SelectByDialog", "Selection In Progress...", nullptr));
        abortButton->setText(QCoreApplication::translate("SelectByDialog", "Abort", nullptr));
        selectButton->setText(QCoreApplication::translate("SelectByDialog", "Select", nullptr));
        invertselectButton->setText(QCoreApplication::translate("SelectByDialog", "Invert Selection", nullptr));
        appendselecttionButton->setText(QCoreApplication::translate("SelectByDialog", "Append Selection", nullptr));
        unselectButton->setText(QCoreApplication::translate("SelectByDialog", "Unselect", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectByDialog: public Ui_SelectByDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTBYDIALOG_H
