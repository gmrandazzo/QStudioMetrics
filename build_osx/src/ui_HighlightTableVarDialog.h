/********************************************************************************
** Form generated from reading UI file 'HighlightTableVarDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HIGHLIGHTTABLEVARDIALOG_H
#define UI_HIGHLIGHTTABLEVARDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_HighlightTableVarDialog
{
public:
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout;
    QLabel *label_3;
    QComboBox *varlist;
    QGridLayout *gridLayout_2;
    QPushButton *actionSelectColorMin;
    QPushButton *actionSelectColorMax;
    QLabel *label;
    QLabel *label_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *HighlightTableVarDialog)
    {
        if (HighlightTableVarDialog->objectName().isEmpty())
            HighlightTableVarDialog->setObjectName(QString::fromUtf8("HighlightTableVarDialog"));
        HighlightTableVarDialog->resize(317, 163);
        gridLayout_3 = new QGridLayout(HighlightTableVarDialog);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_3 = new QLabel(HighlightTableVarDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout->addWidget(label_3, 0, 0, 1, 1);

        varlist = new QComboBox(HighlightTableVarDialog);
        varlist->setObjectName(QString::fromUtf8("varlist"));
        varlist->setMinimumSize(QSize(120, 24));
        varlist->setMaximumSize(QSize(120, 24));

        gridLayout->addWidget(varlist, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        actionSelectColorMin = new QPushButton(HighlightTableVarDialog);
        actionSelectColorMin->setObjectName(QString::fromUtf8("actionSelectColorMin"));
        actionSelectColorMin->setMinimumSize(QSize(60, 40));

        gridLayout_2->addWidget(actionSelectColorMin, 0, 0, 1, 1);

        actionSelectColorMax = new QPushButton(HighlightTableVarDialog);
        actionSelectColorMax->setObjectName(QString::fromUtf8("actionSelectColorMax"));
        actionSelectColorMax->setMinimumSize(QSize(60, 40));

        gridLayout_2->addWidget(actionSelectColorMax, 0, 1, 1, 1);

        label = new QLabel(HighlightTableVarDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_2->addWidget(label, 1, 0, 1, 1);

        label_2 = new QLabel(HighlightTableVarDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 1, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 1, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        cancelButton = new QPushButton(HighlightTableVarDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        horizontalLayout_2->addWidget(cancelButton);

        okButton = new QPushButton(HighlightTableVarDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        horizontalLayout_2->addWidget(okButton);


        gridLayout_3->addLayout(horizontalLayout_2, 2, 0, 1, 1);


        retranslateUi(HighlightTableVarDialog);

        QMetaObject::connectSlotsByName(HighlightTableVarDialog);
    } // setupUi

    void retranslateUi(QDialog *HighlightTableVarDialog)
    {
        HighlightTableVarDialog->setWindowTitle(QCoreApplication::translate("HighlightTableVarDialog", "Highlight Variable...", nullptr));
        label_3->setText(QCoreApplication::translate("HighlightTableVarDialog", "Variable ", nullptr));
        actionSelectColorMin->setText(QString());
        actionSelectColorMax->setText(QString());
        label->setText(QCoreApplication::translate("HighlightTableVarDialog", "Min:", nullptr));
        label_2->setText(QCoreApplication::translate("HighlightTableVarDialog", "Max:", nullptr));
        cancelButton->setText(QCoreApplication::translate("HighlightTableVarDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("HighlightTableVarDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HighlightTableVarDialog: public Ui_HighlightTableVarDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HIGHLIGHTTABLEVARDIALOG_H
