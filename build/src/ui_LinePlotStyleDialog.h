/********************************************************************************
** Form generated from reading UI file 'LinePlotStyleDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LINEPLOTSTYLEDIALOG_H
#define UI_LINEPLOTSTYLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_LinePlotStyleDialog
{
public:
    QGridLayout *gridLayout_4;
    QGroupBox *ShapeGroupBox;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QComboBox *actionSelectShape;
    QSpacerItem *horizontalSpacer;
    QGroupBox *ColorGroupBox;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *actionSelectColor;
    QLabel *colorLabel;
    QSpacerItem *horizontalSpacer_3;
    QGroupBox *SizeGroupBox;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_6;
    QSpacerItem *horizontalSpacer_5;
    QSpinBox *actionSelectSize;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_7;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *LinePlotStyleDialog)
    {
        if (LinePlotStyleDialog->objectName().isEmpty())
            LinePlotStyleDialog->setObjectName(QString::fromUtf8("LinePlotStyleDialog"));
        LinePlotStyleDialog->resize(183, 227);
        gridLayout_4 = new QGridLayout(LinePlotStyleDialog);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        ShapeGroupBox = new QGroupBox(LinePlotStyleDialog);
        ShapeGroupBox->setObjectName(QString::fromUtf8("ShapeGroupBox"));
        ShapeGroupBox->setCheckable(true);
        gridLayout_3 = new QGridLayout(ShapeGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(29, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        actionSelectShape = new QComboBox(ShapeGroupBox);
        actionSelectShape->addItem(QString());
        actionSelectShape->addItem(QString());
        actionSelectShape->addItem(QString());
        actionSelectShape->addItem(QString());
        actionSelectShape->addItem(QString());
        actionSelectShape->addItem(QString());
        actionSelectShape->setObjectName(QString::fromUtf8("actionSelectShape"));
        actionSelectShape->setMinimumSize(QSize(89, 23));
        actionSelectShape->setMaximumSize(QSize(89, 23));
        actionSelectShape->setEditable(false);

        gridLayout_3->addWidget(actionSelectShape, 0, 1, 1, 1);

        horizontalSpacer = new QSpacerItem(29, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 0, 2, 1, 1);


        gridLayout_4->addWidget(ShapeGroupBox, 0, 0, 1, 1);

        ColorGroupBox = new QGroupBox(LinePlotStyleDialog);
        ColorGroupBox->setObjectName(QString::fromUtf8("ColorGroupBox"));
        ColorGroupBox->setCheckable(true);
        gridLayout_2 = new QGridLayout(ColorGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalSpacer_4 = new QSpacerItem(2, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 0, 0, 1, 1);

        actionSelectColor = new QPushButton(ColorGroupBox);
        actionSelectColor->setObjectName(QString::fromUtf8("actionSelectColor"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(actionSelectColor->sizePolicy().hasHeightForWidth());
        actionSelectColor->setSizePolicy(sizePolicy);
        actionSelectColor->setMinimumSize(QSize(101, 24));
        actionSelectColor->setMaximumSize(QSize(101, 24));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        actionSelectColor->setFont(font);

        gridLayout_2->addWidget(actionSelectColor, 0, 1, 1, 1);

        colorLabel = new QLabel(ColorGroupBox);
        colorLabel->setObjectName(QString::fromUtf8("colorLabel"));
        sizePolicy.setHeightForWidth(colorLabel->sizePolicy().hasHeightForWidth());
        colorLabel->setSizePolicy(sizePolicy);
        colorLabel->setMinimumSize(QSize(48, 15));
        colorLabel->setMaximumSize(QSize(48, 15));
        QFont font1;
        font1.setBold(false);
        font1.setUnderline(false);
        font1.setWeight(50);
        colorLabel->setFont(font1);
        colorLabel->setAutoFillBackground(true);

        gridLayout_2->addWidget(colorLabel, 0, 2, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(2, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 0, 3, 1, 1);


        gridLayout_4->addWidget(ColorGroupBox, 1, 0, 1, 1);

        SizeGroupBox = new QGroupBox(LinePlotStyleDialog);
        SizeGroupBox->setObjectName(QString::fromUtf8("SizeGroupBox"));
        SizeGroupBox->setCheckable(true);
        gridLayout = new QGridLayout(SizeGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer_6 = new QSpacerItem(36, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_6, 0, 0, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(37, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 2, 1, 1);

        actionSelectSize = new QSpinBox(SizeGroupBox);
        actionSelectSize->setObjectName(QString::fromUtf8("actionSelectSize"));
        actionSelectSize->setMinimumSize(QSize(74, 23));
        actionSelectSize->setMaximumSize(QSize(74, 23));
        actionSelectSize->setValue(2);

        gridLayout->addWidget(actionSelectSize, 0, 1, 1, 1);


        gridLayout_4->addWidget(SizeGroupBox, 2, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_7 = new QSpacerItem(86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_7);

        cancelButton = new QPushButton(LinePlotStyleDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(70, 24));

        horizontalLayout_5->addWidget(cancelButton);

        okButton = new QPushButton(LinePlotStyleDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(70, 24));

        horizontalLayout_5->addWidget(okButton);


        gridLayout_4->addLayout(horizontalLayout_5, 3, 0, 1, 1);


        retranslateUi(LinePlotStyleDialog);

        QMetaObject::connectSlotsByName(LinePlotStyleDialog);
    } // setupUi

    void retranslateUi(QDialog *LinePlotStyleDialog)
    {
        LinePlotStyleDialog->setWindowTitle(QCoreApplication::translate("LinePlotStyleDialog", "Dialog", nullptr));
        ShapeGroupBox->setTitle(QCoreApplication::translate("LinePlotStyleDialog", "Shape", nullptr));
        actionSelectShape->setItemText(0, QCoreApplication::translate("LinePlotStyleDialog", "Solid", nullptr));
        actionSelectShape->setItemText(1, QCoreApplication::translate("LinePlotStyleDialog", "Dash", nullptr));
        actionSelectShape->setItemText(2, QCoreApplication::translate("LinePlotStyleDialog", "Dot", nullptr));
        actionSelectShape->setItemText(3, QCoreApplication::translate("LinePlotStyleDialog", "Dash Dot", nullptr));
        actionSelectShape->setItemText(4, QCoreApplication::translate("LinePlotStyleDialog", "Dash Dot Dot", nullptr));
        actionSelectShape->setItemText(5, QCoreApplication::translate("LinePlotStyleDialog", "None", nullptr));

        ColorGroupBox->setTitle(QCoreApplication::translate("LinePlotStyleDialog", "Color", nullptr));
        actionSelectColor->setText(QCoreApplication::translate("LinePlotStyleDialog", "Select Color", nullptr));
        colorLabel->setText(QString());
        SizeGroupBox->setTitle(QCoreApplication::translate("LinePlotStyleDialog", "Size", nullptr));
        cancelButton->setText(QCoreApplication::translate("LinePlotStyleDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("LinePlotStyleDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LinePlotStyleDialog: public Ui_LinePlotStyleDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LINEPLOTSTYLEDIALOG_H
