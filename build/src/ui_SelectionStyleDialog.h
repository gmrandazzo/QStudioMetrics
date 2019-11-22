/********************************************************************************
** Form generated from reading UI file 'SelectionStyleDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTIONSTYLEDIALOG_H
#define UI_SELECTIONSTYLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_SelectionStyleDialog
{
public:
    QGridLayout *gridLayout_5;
    QGroupBox *ShapeGroupBox;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QComboBox *actionSymbolType;
    QSpacerItem *horizontalSpacer_7;
    QGroupBox *ColorGroupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QRadioButton *solidcolor;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *actionSelectColor;
    QLabel *colorLabel;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_4;
    QRadioButton *variablecolor;
    QSpacerItem *horizontalSpacer_4;
    QComboBox *colorvarlist;
    QLabel *label;
    QPushButton *actionSelectColorMin;
    QLabel *label_2;
    QPushButton *actionSelectColorMax;
    QSpacerItem *horizontalSpacer_10;
    QGroupBox *SizeGroupBox;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *fixedsize;
    QSpacerItem *horizontalSpacer_5;
    QDoubleSpinBox *actionSymbolSize;
    QSpacerItem *horizontalSpacer_9;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *variablesize;
    QSpacerItem *horizontalSpacer_6;
    QComboBox *sizevarlist;
    QLabel *label_3;
    QSpinBox *actionSymbolSizeMin;
    QLabel *label_4;
    QSpinBox *actionSymbolSizeMax;
    QSpacerItem *horizontalSpacer_11;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *SelectionStyleDialog)
    {
        if (SelectionStyleDialog->objectName().isEmpty())
            SelectionStyleDialog->setObjectName(QString::fromUtf8("SelectionStyleDialog"));
        SelectionStyleDialog->resize(720, 408);
        SelectionStyleDialog->setMinimumSize(QSize(0, 0));
        SelectionStyleDialog->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_5 = new QGridLayout(SelectionStyleDialog);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        ShapeGroupBox = new QGroupBox(SelectionStyleDialog);
        ShapeGroupBox->setObjectName(QString::fromUtf8("ShapeGroupBox"));
        ShapeGroupBox->setCheckable(true);
        gridLayout_4 = new QGridLayout(ShapeGroupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(162, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        actionSymbolType = new QComboBox(ShapeGroupBox);
        actionSymbolType->addItem(QString());
        actionSymbolType->addItem(QString());
        actionSymbolType->addItem(QString());
        actionSymbolType->setObjectName(QString::fromUtf8("actionSymbolType"));
        actionSymbolType->setMinimumSize(QSize(100, 24));
        actionSymbolType->setMaximumSize(QSize(100, 24));
        actionSymbolType->setEditable(false);

        gridLayout_4->addWidget(actionSymbolType, 0, 1, 1, 1);

        horizontalSpacer_7 = new QSpacerItem(162, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_7, 0, 2, 1, 1);


        gridLayout_5->addWidget(ShapeGroupBox, 0, 0, 1, 1);

        ColorGroupBox = new QGroupBox(SelectionStyleDialog);
        ColorGroupBox->setObjectName(QString::fromUtf8("ColorGroupBox"));
        ColorGroupBox->setMinimumSize(QSize(0, 0));
        ColorGroupBox->setCheckable(true);
        gridLayout = new QGridLayout(ColorGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        solidcolor = new QRadioButton(ColorGroupBox);
        solidcolor->setObjectName(QString::fromUtf8("solidcolor"));

        horizontalLayout->addWidget(solidcolor);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        actionSelectColor = new QPushButton(ColorGroupBox);
        actionSelectColor->setObjectName(QString::fromUtf8("actionSelectColor"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(actionSelectColor->sizePolicy().hasHeightForWidth());
        actionSelectColor->setSizePolicy(sizePolicy);
        actionSelectColor->setMinimumSize(QSize(100, 24));
        actionSelectColor->setMaximumSize(QSize(100, 24));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        actionSelectColor->setFont(font);

        horizontalLayout->addWidget(actionSelectColor);

        colorLabel = new QLabel(ColorGroupBox);
        colorLabel->setObjectName(QString::fromUtf8("colorLabel"));
        sizePolicy.setHeightForWidth(colorLabel->sizePolicy().hasHeightForWidth());
        colorLabel->setSizePolicy(sizePolicy);
        colorLabel->setMinimumSize(QSize(48, 15));
        colorLabel->setMaximumSize(QSize(16777215, 16777215));
        QFont font1;
        font1.setBold(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        colorLabel->setFont(font1);
        colorLabel->setAutoFillBackground(true);

        horizontalLayout->addWidget(colorLabel);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_8);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        variablecolor = new QRadioButton(ColorGroupBox);
        variablecolor->setObjectName(QString::fromUtf8("variablecolor"));

        horizontalLayout_4->addWidget(variablecolor);

        horizontalSpacer_4 = new QSpacerItem(13, 37, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        colorvarlist = new QComboBox(ColorGroupBox);
        colorvarlist->addItem(QString());
        colorvarlist->setObjectName(QString::fromUtf8("colorvarlist"));
        sizePolicy.setHeightForWidth(colorvarlist->sizePolicy().hasHeightForWidth());
        colorvarlist->setSizePolicy(sizePolicy);
        colorvarlist->setMinimumSize(QSize(130, 24));
        colorvarlist->setMaximumSize(QSize(130, 24));
        colorvarlist->setEditable(false);

        horizontalLayout_4->addWidget(colorvarlist);

        label = new QLabel(ColorGroupBox);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_4->addWidget(label);

        actionSelectColorMin = new QPushButton(ColorGroupBox);
        actionSelectColorMin->setObjectName(QString::fromUtf8("actionSelectColorMin"));
        actionSelectColorMin->setMinimumSize(QSize(60, 40));

        horizontalLayout_4->addWidget(actionSelectColorMin);

        label_2 = new QLabel(ColorGroupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_4->addWidget(label_2);

        actionSelectColorMax = new QPushButton(ColorGroupBox);
        actionSelectColorMax->setObjectName(QString::fromUtf8("actionSelectColorMax"));
        actionSelectColorMax->setMinimumSize(QSize(60, 40));

        horizontalLayout_4->addWidget(actionSelectColorMax);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_10);


        gridLayout->addLayout(horizontalLayout_4, 1, 0, 1, 1);


        gridLayout_5->addWidget(ColorGroupBox, 1, 0, 1, 1);

        SizeGroupBox = new QGroupBox(SelectionStyleDialog);
        SizeGroupBox->setObjectName(QString::fromUtf8("SizeGroupBox"));
        SizeGroupBox->setCheckable(true);
        gridLayout_2 = new QGridLayout(SizeGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        fixedsize = new QRadioButton(SizeGroupBox);
        fixedsize->setObjectName(QString::fromUtf8("fixedsize"));

        horizontalLayout_2->addWidget(fixedsize);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        actionSymbolSize = new QDoubleSpinBox(SizeGroupBox);
        actionSymbolSize->setObjectName(QString::fromUtf8("actionSymbolSize"));
        actionSymbolSize->setMinimumSize(QSize(74, 24));
        actionSymbolSize->setMaximumSize(QSize(74, 23));
        actionSymbolSize->setDecimals(1);
        actionSymbolSize->setMinimum(1.000000000000000);
        actionSymbolSize->setMaximum(999999999.000000000000000);
        actionSymbolSize->setValue(8.000000000000000);

        horizontalLayout_2->addWidget(actionSymbolSize);

        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_9);


        gridLayout_2->addLayout(horizontalLayout_2, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        variablesize = new QRadioButton(SizeGroupBox);
        variablesize->setObjectName(QString::fromUtf8("variablesize"));

        horizontalLayout_3->addWidget(variablesize);

        horizontalSpacer_6 = new QSpacerItem(13, 21, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_6);

        sizevarlist = new QComboBox(SizeGroupBox);
        sizevarlist->addItem(QString());
        sizevarlist->setObjectName(QString::fromUtf8("sizevarlist"));
        sizePolicy.setHeightForWidth(sizevarlist->sizePolicy().hasHeightForWidth());
        sizevarlist->setSizePolicy(sizePolicy);
        sizevarlist->setMinimumSize(QSize(130, 24));
        sizevarlist->setMaximumSize(QSize(130, 24));
        sizevarlist->setEditable(false);

        horizontalLayout_3->addWidget(sizevarlist);

        label_3 = new QLabel(SizeGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        actionSymbolSizeMin = new QSpinBox(SizeGroupBox);
        actionSymbolSizeMin->setObjectName(QString::fromUtf8("actionSymbolSizeMin"));
        actionSymbolSizeMin->setMinimum(1);
        actionSymbolSizeMin->setMaximum(100);
        actionSymbolSizeMin->setValue(1);

        horizontalLayout_3->addWidget(actionSymbolSizeMin);

        label_4 = new QLabel(SizeGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_3->addWidget(label_4);

        actionSymbolSizeMax = new QSpinBox(SizeGroupBox);
        actionSymbolSizeMax->setObjectName(QString::fromUtf8("actionSymbolSizeMax"));
        actionSymbolSizeMax->setMinimum(1);
        actionSymbolSizeMax->setMaximum(100);
        actionSymbolSizeMax->setValue(10);

        horizontalLayout_3->addWidget(actionSymbolSizeMax);

        horizontalSpacer_11 = new QSpacerItem(13, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_11);


        gridLayout_2->addLayout(horizontalLayout_3, 1, 0, 1, 1);


        gridLayout_5->addWidget(SizeGroupBox, 2, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalSpacer = new QSpacerItem(86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(SelectionStyleDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout_3->addWidget(cancelButton, 0, 1, 1, 1);

        okButton = new QPushButton(SelectionStyleDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout_3->addWidget(okButton, 0, 2, 1, 1);


        gridLayout_5->addLayout(gridLayout_3, 3, 0, 1, 1);


        retranslateUi(SelectionStyleDialog);

        QMetaObject::connectSlotsByName(SelectionStyleDialog);
    } // setupUi

    void retranslateUi(QDialog *SelectionStyleDialog)
    {
        SelectionStyleDialog->setWindowTitle(QCoreApplication::translate("SelectionStyleDialog", "Set Selection Style...", nullptr));
        ShapeGroupBox->setTitle(QCoreApplication::translate("SelectionStyleDialog", "Shape", nullptr));
        actionSymbolType->setItemText(0, QCoreApplication::translate("SelectionStyleDialog", "Circle", nullptr));
        actionSymbolType->setItemText(1, QCoreApplication::translate("SelectionStyleDialog", "Square", nullptr));
        actionSymbolType->setItemText(2, QCoreApplication::translate("SelectionStyleDialog", "Triangle", nullptr));

        ColorGroupBox->setTitle(QCoreApplication::translate("SelectionStyleDialog", "Color", nullptr));
        solidcolor->setText(QCoreApplication::translate("SelectionStyleDialog", "Solid", nullptr));
        actionSelectColor->setText(QCoreApplication::translate("SelectionStyleDialog", "Select Color", nullptr));
        colorLabel->setText(QString());
        variablecolor->setText(QCoreApplication::translate("SelectionStyleDialog", "by Variable", nullptr));
        colorvarlist->setItemText(0, QCoreApplication::translate("SelectionStyleDialog", "Variable...", nullptr));

        label->setText(QCoreApplication::translate("SelectionStyleDialog", "Min:", nullptr));
        actionSelectColorMin->setText(QString());
        label_2->setText(QCoreApplication::translate("SelectionStyleDialog", "Max:", nullptr));
        actionSelectColorMax->setText(QString());
        SizeGroupBox->setTitle(QCoreApplication::translate("SelectionStyleDialog", "Size", nullptr));
        fixedsize->setText(QCoreApplication::translate("SelectionStyleDialog", "fixed", nullptr));
        variablesize->setText(QCoreApplication::translate("SelectionStyleDialog", "by variable", nullptr));
        sizevarlist->setItemText(0, QCoreApplication::translate("SelectionStyleDialog", "Variable...", nullptr));

        label_3->setText(QCoreApplication::translate("SelectionStyleDialog", "min", nullptr));
        label_4->setText(QCoreApplication::translate("SelectionStyleDialog", "max", nullptr));
        cancelButton->setText(QCoreApplication::translate("SelectionStyleDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("SelectionStyleDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectionStyleDialog: public Ui_SelectionStyleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTIONSTYLEDIALOG_H
