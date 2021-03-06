/********************************************************************************
** Form generated from reading UI file 'BarPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BARPLOT_H
#define UI_BARPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BarPlot
{
public:
    QAction *actionExit;
    QGridLayout *gridLayout_2;
    QWidget *widget;

    void setupUi(QWidget *BarPlot)
    {
        if (BarPlot->objectName().isEmpty())
            BarPlot->setObjectName(QString::fromUtf8("BarPlot"));
        BarPlot->resize(704, 500);
        BarPlot->setMinimumSize(QSize(700, 500));
        actionExit = new QAction(BarPlot);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        gridLayout_2 = new QGridLayout(BarPlot);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        widget = new QWidget(BarPlot);
        widget->setObjectName(QString::fromUtf8("widget"));

        gridLayout_2->addWidget(widget, 0, 0, 1, 1);


        retranslateUi(BarPlot);

        QMetaObject::connectSlotsByName(BarPlot);
    } // setupUi

    void retranslateUi(QWidget *BarPlot)
    {
        BarPlot->setWindowTitle(QCoreApplication::translate("BarPlot", "Form", nullptr));
        actionExit->setText(QCoreApplication::translate("BarPlot", "Exit", nullptr));
#if QT_CONFIG(tooltip)
        actionExit->setToolTip(QCoreApplication::translate("BarPlot", "Exit", nullptr));
#endif // QT_CONFIG(tooltip)
    } // retranslateUi

};

namespace Ui {
    class BarPlot: public Ui_BarPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BARPLOT_H
