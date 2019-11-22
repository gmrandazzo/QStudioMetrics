/********************************************************************************
** Form generated from reading UI file 'SelectionStyleColor.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELECTIONSTYLECOLOR_H
#define UI_SELECTIONSTYLECOLOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_SelectionStyleColor
{
public:
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout;
    QRadioButton *solidcolor;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *actionSelectColor;
    QLabel *colorLabel;
    QSpacerItem *horizontalSpacer_8;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QRadioButton *variablecolor;
    QSpacerItem *horizontalSpacer_4;
    QComboBox *colorvarvaltype;
    QComboBox *colorvarlist;
    QComboBox *colorvarlevels;
    QSpacerItem *horizontalSpacer;
    QGridLayout *gridLayout_2;
    QPushButton *actionSelectColorMin;
    QPushButton *actionSelectColorMax;
    QLabel *label;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *SelectionStyleColor)
    {
        if (SelectionStyleColor->objectName().isEmpty())
            SelectionStyleColor->setObjectName(QString::fromUtf8("SelectionStyleColor"));
        SelectionStyleColor->resize(443, 174);
        gridLayout_4 = new QGridLayout(SelectionStyleColor);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        solidcolor = new QRadioButton(SelectionStyleColor);
        solidcolor->setObjectName(QString::fromUtf8("solidcolor"));

        horizontalLayout->addWidget(solidcolor);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        actionSelectColor = new QPushButton(SelectionStyleColor);
        actionSelectColor->setObjectName(QString::fromUtf8("actionSelectColor"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(actionSelectColor->sizePolicy().hasHeightForWidth());
        actionSelectColor->setSizePolicy(sizePolicy);
        actionSelectColor->setMinimumSize(QSize(100, 24));
        actionSelectColor->setMaximumSize(QSize(101, 24));
        QFont font;
        font.setBold(false);
        font.setWeight(50);
        actionSelectColor->setFont(font);

        horizontalLayout->addWidget(actionSelectColor);

        colorLabel = new QLabel(SelectionStyleColor);
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


        gridLayout_4->addLayout(horizontalLayout, 0, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        variablecolor = new QRadioButton(SelectionStyleColor);
        variablecolor->setObjectName(QString::fromUtf8("variablecolor"));

        gridLayout->addWidget(variablecolor, 0, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(13, 37, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_4, 0, 1, 1, 1);

        colorvarvaltype = new QComboBox(SelectionStyleColor);
        colorvarvaltype->addItem(QString());
        colorvarvaltype->addItem(QString());
        colorvarvaltype->setObjectName(QString::fromUtf8("colorvarvaltype"));
        sizePolicy.setHeightForWidth(colorvarvaltype->sizePolicy().hasHeightForWidth());
        colorvarvaltype->setSizePolicy(sizePolicy);
        colorvarvaltype->setMinimumSize(QSize(131, 23));
        colorvarvaltype->setMaximumSize(QSize(16777215, 16777215));
        colorvarvaltype->setEditable(false);

        gridLayout->addWidget(colorvarvaltype, 0, 2, 1, 1);

        colorvarlist = new QComboBox(SelectionStyleColor);
        colorvarlist->addItem(QString());
        colorvarlist->setObjectName(QString::fromUtf8("colorvarlist"));
        sizePolicy.setHeightForWidth(colorvarlist->sizePolicy().hasHeightForWidth());
        colorvarlist->setSizePolicy(sizePolicy);
        colorvarlist->setMinimumSize(QSize(131, 23));
        colorvarlist->setMaximumSize(QSize(16777215, 16777215));
        colorvarlist->setEditable(false);

        gridLayout->addWidget(colorvarlist, 0, 3, 1, 1);

        colorvarlevels = new QComboBox(SelectionStyleColor);
        colorvarlevels->addItem(QString());
        colorvarlevels->setObjectName(QString::fromUtf8("colorvarlevels"));
        sizePolicy.setHeightForWidth(colorvarlevels->sizePolicy().hasHeightForWidth());
        colorvarlevels->setSizePolicy(sizePolicy);
        colorvarlevels->setMinimumSize(QSize(58, 23));
        colorvarlevels->setMaximumSize(QSize(16777215, 16777215));
        colorvarlevels->setEditable(false);

        gridLayout->addWidget(colorvarlevels, 0, 4, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 3);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer, 1, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        actionSelectColorMin = new QPushButton(SelectionStyleColor);
        actionSelectColorMin->setObjectName(QString::fromUtf8("actionSelectColorMin"));
        actionSelectColorMin->setMinimumSize(QSize(60, 40));

        gridLayout_2->addWidget(actionSelectColorMin, 0, 0, 1, 1);

        actionSelectColorMax = new QPushButton(SelectionStyleColor);
        actionSelectColorMax->setObjectName(QString::fromUtf8("actionSelectColorMax"));
        actionSelectColorMax->setMinimumSize(QSize(60, 40));

        gridLayout_2->addWidget(actionSelectColorMax, 0, 1, 1, 1);

        label = new QLabel(SelectionStyleColor);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(SelectionStyleColor);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 1, 1, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_5, 1, 2, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 1, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalSpacer_3 = new QSpacerItem(86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_3);

        cancelButton = new QPushButton(SelectionStyleColor);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(70, 24));

        horizontalLayout_5->addWidget(cancelButton);

        okButton = new QPushButton(SelectionStyleColor);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(70, 24));

        horizontalLayout_5->addWidget(okButton);


        gridLayout_4->addLayout(horizontalLayout_5, 2, 0, 1, 1);


        retranslateUi(SelectionStyleColor);

        QMetaObject::connectSlotsByName(SelectionStyleColor);
    } // setupUi

    void retranslateUi(QDialog *SelectionStyleColor)
    {
        SelectionStyleColor->setWindowTitle(QCoreApplication::translate("SelectionStyleColor", "Dialog", nullptr));
        solidcolor->setText(QCoreApplication::translate("SelectionStyleColor", "Solid", nullptr));
        actionSelectColor->setText(QCoreApplication::translate("SelectionStyleColor", "Select Color", nullptr));
        colorLabel->setText(QString());
        variablecolor->setText(QCoreApplication::translate("SelectionStyleColor", "by Variable", nullptr));
        colorvarvaltype->setItemText(0, QCoreApplication::translate("SelectionStyleColor", "X Variable Value", nullptr));
        colorvarvaltype->setItemText(1, QCoreApplication::translate("SelectionStyleColor", "Y Variable Value", nullptr));

        colorvarlist->setItemText(0, QCoreApplication::translate("SelectionStyleColor", "Variable...", nullptr));

        colorvarlevels->setItemText(0, QCoreApplication::translate("SelectionStyleColor", "Levels...", nullptr));

        actionSelectColorMin->setText(QString());
        actionSelectColorMax->setText(QString());
        label->setText(QCoreApplication::translate("SelectionStyleColor", "Min:", nullptr));
        label_2->setText(QCoreApplication::translate("SelectionStyleColor", "Max:", nullptr));
        cancelButton->setText(QCoreApplication::translate("SelectionStyleColor", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("SelectionStyleColor", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SelectionStyleColor: public Ui_SelectionStyleColor {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELECTIONSTYLECOLOR_H
