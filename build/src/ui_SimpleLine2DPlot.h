/********************************************************************************
** Form generated from reading UI file 'SimpleLine2DPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIMPLELINE2DPLOT_H
#define UI_SIMPLELINE2DPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SimpleLine2DPlot
{
public:
    QAction *actionExit;
    QGridLayout *gridLayout_2;
    QWidget *plotwidget;
    QGridLayout *gridLayout;
    QPushButton *saveimageButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *SimpleLine2DPlot)
    {
        if (SimpleLine2DPlot->objectName().isEmpty())
            SimpleLine2DPlot->setObjectName(QString::fromUtf8("SimpleLine2DPlot"));
        SimpleLine2DPlot->resize(308, 338);
        SimpleLine2DPlot->setMinimumSize(QSize(0, 0));
        actionExit = new QAction(SimpleLine2DPlot);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        gridLayout_2 = new QGridLayout(SimpleLine2DPlot);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        plotwidget = new QWidget(SimpleLine2DPlot);
        plotwidget->setObjectName(QString::fromUtf8("plotwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plotwidget->sizePolicy().hasHeightForWidth());
        plotwidget->setSizePolicy(sizePolicy);
        plotwidget->setMinimumSize(QSize(300, 300));

        gridLayout_2->addWidget(plotwidget, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        saveimageButton = new QPushButton(SimpleLine2DPlot);
        saveimageButton->setObjectName(QString::fromUtf8("saveimageButton"));

        gridLayout->addWidget(saveimageButton, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);


        retranslateUi(SimpleLine2DPlot);

        QMetaObject::connectSlotsByName(SimpleLine2DPlot);
    } // setupUi

    void retranslateUi(QWidget *SimpleLine2DPlot)
    {
        SimpleLine2DPlot->setWindowTitle(QCoreApplication::translate("SimpleLine2DPlot", "Form", nullptr));
        actionExit->setText(QCoreApplication::translate("SimpleLine2DPlot", "Exit", nullptr));
#if QT_CONFIG(tooltip)
        actionExit->setToolTip(QCoreApplication::translate("SimpleLine2DPlot", "Exit", nullptr));
#endif // QT_CONFIG(tooltip)
        saveimageButton->setText(QCoreApplication::translate("SimpleLine2DPlot", "Save Image...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SimpleLine2DPlot: public Ui_SimpleLine2DPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIMPLELINE2DPLOT_H
