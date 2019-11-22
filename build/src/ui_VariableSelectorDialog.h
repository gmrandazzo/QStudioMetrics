/********************************************************************************
** Form generated from reading UI file 'VariableSelectorDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VARIABLESELECTORDIALOG_H
#define UI_VARIABLESELECTORDIALOG_H

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
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_VariableSelectorDialog
{
public:
    QGridLayout *gridLayout_3;
    QFrame *frame;
    QGridLayout *gridLayout_2;
    QRadioButton *selectbyregexpButton;
    QLineEdit *regexp;
    QRadioButton *selectbyvarlabelButton;
    QComboBox *varlabelslst;
    QRadioButton *selectbyObj;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_9;
    QLabel *selectedlabel;
    QPushButton *findButton;
    QSpacerItem *horizontalSpacer;
    QListView *varnamelistView;
    QLineEdit *searchline;
    QGroupBox *objectGroupBox;
    QGridLayout *gridLayout;
    QGridLayout *gridLayout_13;
    QGridLayout *gridLayout_14;
    QGridLayout *gridLayout_15;
    QLabel *label_9;
    QLineEdit *min;
    QLabel *label_10;
    QLineEdit *max;
    QRadioButton *valuerange;
    QGridLayout *gridLayout_16;
    QRadioButton *valueinlimits;
    QGridLayout *gridLayout_17;
    QLabel *label_11;
    QLineEdit *limitinf;
    QLabel *label_12;
    QLineEdit *limitsup;
    QComboBox *variablelst;
    QListView *objectlistView;
    QLabel *label_13;
    QSpacerItem *verticalSpacer_2;
    QSpinBox *objlayerBox;
    QPushButton *selectButton;
    QPushButton *invertselectButton;
    QPushButton *appendselectionButton;
    QPushButton *unselectButton;
    QGridLayout *gridLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *VariableSelectorDialog)
    {
        if (VariableSelectorDialog->objectName().isEmpty())
            VariableSelectorDialog->setObjectName(QString::fromUtf8("VariableSelectorDialog"));
        VariableSelectorDialog->resize(668, 404);
        gridLayout_3 = new QGridLayout(VariableSelectorDialog);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        frame = new QFrame(VariableSelectorDialog);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setFrameShape(QFrame::NoFrame);
        frame->setFrameShadow(QFrame::Raised);
        gridLayout_2 = new QGridLayout(frame);
        gridLayout_2->setContentsMargins(0, 0, 0, 0);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        selectbyregexpButton = new QRadioButton(frame);
        selectbyregexpButton->setObjectName(QString::fromUtf8("selectbyregexpButton"));
        selectbyregexpButton->setChecked(true);

        gridLayout_2->addWidget(selectbyregexpButton, 0, 0, 1, 1);

        regexp = new QLineEdit(frame);
        regexp->setObjectName(QString::fromUtf8("regexp"));

        gridLayout_2->addWidget(regexp, 0, 1, 1, 1);

        selectbyvarlabelButton = new QRadioButton(frame);
        selectbyvarlabelButton->setObjectName(QString::fromUtf8("selectbyvarlabelButton"));
        selectbyvarlabelButton->setChecked(false);

        gridLayout_2->addWidget(selectbyvarlabelButton, 1, 0, 1, 1);

        varlabelslst = new QComboBox(frame);
        varlabelslst->setObjectName(QString::fromUtf8("varlabelslst"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(varlabelslst->sizePolicy().hasHeightForWidth());
        varlabelslst->setSizePolicy(sizePolicy);
        varlabelslst->setMinimumSize(QSize(130, 24));
        varlabelslst->setMaximumSize(QSize(16777215, 16777215));
        varlabelslst->setEditable(false);

        gridLayout_2->addWidget(varlabelslst, 1, 1, 1, 1);

        selectbyObj = new QRadioButton(frame);
        selectbyObj->setObjectName(QString::fromUtf8("selectbyObj"));

        gridLayout_2->addWidget(selectbyObj, 2, 0, 1, 1);


        gridLayout_3->addWidget(frame, 0, 0, 1, 2);

        groupBox_2 = new QGroupBox(VariableSelectorDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(234, 0));
        gridLayout_9 = new QGridLayout(groupBox_2);
        gridLayout_9->setObjectName(QString::fromUtf8("gridLayout_9"));
        selectedlabel = new QLabel(groupBox_2);
        selectedlabel->setObjectName(QString::fromUtf8("selectedlabel"));

        gridLayout_9->addWidget(selectedlabel, 2, 2, 1, 1);

        findButton = new QPushButton(groupBox_2);
        findButton->setObjectName(QString::fromUtf8("findButton"));
        findButton->setMinimumSize(QSize(100, 24));
        findButton->setMaximumSize(QSize(100, 24));

        gridLayout_9->addWidget(findButton, 0, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(128, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_9->addItem(horizontalSpacer, 2, 0, 1, 2);

        varnamelistView = new QListView(groupBox_2);
        varnamelistView->setObjectName(QString::fromUtf8("varnamelistView"));
        varnamelistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        varnamelistView->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_9->addWidget(varnamelistView, 1, 0, 1, 3);

        searchline = new QLineEdit(groupBox_2);
        searchline->setObjectName(QString::fromUtf8("searchline"));

        gridLayout_9->addWidget(searchline, 0, 0, 1, 1);


        gridLayout_3->addWidget(groupBox_2, 0, 2, 4, 1);

        objectGroupBox = new QGroupBox(VariableSelectorDialog);
        objectGroupBox->setObjectName(QString::fromUtf8("objectGroupBox"));
        gridLayout = new QGridLayout(objectGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout_13 = new QGridLayout();
        gridLayout_13->setObjectName(QString::fromUtf8("gridLayout_13"));
        gridLayout_14 = new QGridLayout();
        gridLayout_14->setObjectName(QString::fromUtf8("gridLayout_14"));
        gridLayout_15 = new QGridLayout();
        gridLayout_15->setObjectName(QString::fromUtf8("gridLayout_15"));
        label_9 = new QLabel(objectGroupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_15->addWidget(label_9, 0, 0, 1, 1);

        min = new QLineEdit(objectGroupBox);
        min->setObjectName(QString::fromUtf8("min"));

        gridLayout_15->addWidget(min, 0, 1, 1, 1);

        label_10 = new QLabel(objectGroupBox);
        label_10->setObjectName(QString::fromUtf8("label_10"));

        gridLayout_15->addWidget(label_10, 1, 0, 1, 1);

        max = new QLineEdit(objectGroupBox);
        max->setObjectName(QString::fromUtf8("max"));

        gridLayout_15->addWidget(max, 1, 1, 1, 1);


        gridLayout_14->addLayout(gridLayout_15, 1, 0, 1, 1);

        valuerange = new QRadioButton(objectGroupBox);
        valuerange->setObjectName(QString::fromUtf8("valuerange"));

        gridLayout_14->addWidget(valuerange, 0, 0, 1, 1);


        gridLayout_13->addLayout(gridLayout_14, 0, 1, 1, 1);

        gridLayout_16 = new QGridLayout();
        gridLayout_16->setObjectName(QString::fromUtf8("gridLayout_16"));
        valueinlimits = new QRadioButton(objectGroupBox);
        valueinlimits->setObjectName(QString::fromUtf8("valueinlimits"));
        valueinlimits->setChecked(true);

        gridLayout_16->addWidget(valueinlimits, 0, 0, 1, 1);

        gridLayout_17 = new QGridLayout();
        gridLayout_17->setObjectName(QString::fromUtf8("gridLayout_17"));
        label_11 = new QLabel(objectGroupBox);
        label_11->setObjectName(QString::fromUtf8("label_11"));

        gridLayout_17->addWidget(label_11, 0, 0, 1, 1);

        limitinf = new QLineEdit(objectGroupBox);
        limitinf->setObjectName(QString::fromUtf8("limitinf"));

        gridLayout_17->addWidget(limitinf, 0, 1, 1, 1);

        label_12 = new QLabel(objectGroupBox);
        label_12->setObjectName(QString::fromUtf8("label_12"));

        gridLayout_17->addWidget(label_12, 1, 0, 1, 1);

        limitsup = new QLineEdit(objectGroupBox);
        limitsup->setObjectName(QString::fromUtf8("limitsup"));

        gridLayout_17->addWidget(limitsup, 1, 1, 1, 1);


        gridLayout_16->addLayout(gridLayout_17, 1, 0, 1, 1);


        gridLayout_13->addLayout(gridLayout_16, 0, 0, 1, 1);


        gridLayout->addLayout(gridLayout_13, 2, 1, 1, 2);

        variablelst = new QComboBox(objectGroupBox);
        variablelst->setObjectName(QString::fromUtf8("variablelst"));
        sizePolicy.setHeightForWidth(variablelst->sizePolicy().hasHeightForWidth());
        variablelst->setSizePolicy(sizePolicy);
        variablelst->setMinimumSize(QSize(130, 24));
        variablelst->setMaximumSize(QSize(16777215, 16777215));
        variablelst->setEditable(false);

        gridLayout->addWidget(variablelst, 0, 1, 1, 2);

        objectlistView = new QListView(objectGroupBox);
        objectlistView->setObjectName(QString::fromUtf8("objectlistView"));
        objectlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        objectlistView->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout->addWidget(objectlistView, 0, 0, 4, 1);

        label_13 = new QLabel(objectGroupBox);
        label_13->setObjectName(QString::fromUtf8("label_13"));

        gridLayout->addWidget(label_13, 1, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 43, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 3, 2, 1, 1);

        objlayerBox = new QSpinBox(objectGroupBox);
        objlayerBox->setObjectName(QString::fromUtf8("objlayerBox"));
        objlayerBox->setMinimum(1);
        objlayerBox->setMaximum(999999999);

        gridLayout->addWidget(objlayerBox, 1, 2, 1, 1);


        gridLayout_3->addWidget(objectGroupBox, 1, 0, 1, 2);

        selectButton = new QPushButton(VariableSelectorDialog);
        selectButton->setObjectName(QString::fromUtf8("selectButton"));
        selectButton->setMinimumSize(QSize(130, 24));
        selectButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(selectButton, 2, 0, 1, 1);

        invertselectButton = new QPushButton(VariableSelectorDialog);
        invertselectButton->setObjectName(QString::fromUtf8("invertselectButton"));
        invertselectButton->setMinimumSize(QSize(130, 24));
        invertselectButton->setMaximumSize(QSize(100, 16777215));

        gridLayout_3->addWidget(invertselectButton, 2, 1, 1, 1);

        appendselectionButton = new QPushButton(VariableSelectorDialog);
        appendselectionButton->setObjectName(QString::fromUtf8("appendselectionButton"));
        appendselectionButton->setMinimumSize(QSize(130, 24));
        appendselectionButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(appendselectionButton, 3, 0, 1, 1);

        unselectButton = new QPushButton(VariableSelectorDialog);
        unselectButton->setObjectName(QString::fromUtf8("unselectButton"));
        unselectButton->setMinimumSize(QSize(130, 24));
        unselectButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(unselectButton, 3, 1, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        horizontalSpacer_3 = new QSpacerItem(86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        okButton = new QPushButton(VariableSelectorDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout_6->addWidget(okButton, 0, 2, 1, 1);

        cancelButton = new QPushButton(VariableSelectorDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout_6->addWidget(cancelButton, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_6, 4, 0, 1, 3);

        frame->raise();
        groupBox_2->raise();
        objectGroupBox->raise();
        selectButton->raise();
        appendselectionButton->raise();
        invertselectButton->raise();
        unselectButton->raise();

        retranslateUi(VariableSelectorDialog);

        QMetaObject::connectSlotsByName(VariableSelectorDialog);
    } // setupUi

    void retranslateUi(QDialog *VariableSelectorDialog)
    {
        VariableSelectorDialog->setWindowTitle(QCoreApplication::translate("VariableSelectorDialog", "Variable Selection...", nullptr));
        selectbyregexpButton->setText(QCoreApplication::translate("VariableSelectorDialog", "Select by match variable string name", nullptr));
        selectbyvarlabelButton->setText(QCoreApplication::translate("VariableSelectorDialog", " Select by variable labels", nullptr));
        selectbyObj->setText(QCoreApplication::translate("VariableSelectorDialog", "Select by Object ", nullptr));
        groupBox_2->setTitle(QString());
        selectedlabel->setText(QCoreApplication::translate("VariableSelectorDialog", "TextLabel", nullptr));
        findButton->setText(QCoreApplication::translate("VariableSelectorDialog", "Find", nullptr));
        objectGroupBox->setTitle(QString());
        label_9->setText(QCoreApplication::translate("VariableSelectorDialog", "Min", nullptr));
        label_10->setText(QCoreApplication::translate("VariableSelectorDialog", "Max", nullptr));
        valuerange->setText(QCoreApplication::translate("VariableSelectorDialog", "Values in range", nullptr));
        valueinlimits->setText(QCoreApplication::translate("VariableSelectorDialog", "Value over limits", nullptr));
        label_11->setText(QCoreApplication::translate("VariableSelectorDialog", "inferior (<)", nullptr));
        label_12->setText(QCoreApplication::translate("VariableSelectorDialog", "superior (>)", nullptr));
        label_13->setText(QCoreApplication::translate("VariableSelectorDialog", "Layer", nullptr));
        selectButton->setText(QCoreApplication::translate("VariableSelectorDialog", "Select", nullptr));
        invertselectButton->setText(QCoreApplication::translate("VariableSelectorDialog", "Invert Selection", nullptr));
        appendselectionButton->setText(QCoreApplication::translate("VariableSelectorDialog", "Append Selection...", nullptr));
        unselectButton->setText(QCoreApplication::translate("VariableSelectorDialog", "Unselect", nullptr));
        okButton->setText(QCoreApplication::translate("VariableSelectorDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("VariableSelectorDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VariableSelectorDialog: public Ui_VariableSelectorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VARIABLESELECTORDIALOG_H
