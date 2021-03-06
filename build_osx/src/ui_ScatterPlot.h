/********************************************************************************
** Form generated from reading UI file 'ScatterPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SCATTERPLOT_H
#define UI_SCATTERPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ScatterPlot
{
public:
    QAction *actionExit;
    QGridLayout *gridLayout_2;
    QProgressBar *progressBar;
    QPushButton *abortButton;
    QGridLayout *gridLayout;
    QSplitter *splitter;
    QLabel *axis1_name;
    QSpinBox *axis1;
    QSplitter *splitter_2;
    QLabel *axis2_name;
    QSpinBox *axis2;
    QSplitter *splitter_3;
    QLabel *axis3_name;
    QSpinBox *axis3;
    QSpacerItem *horizontalSpacer;
    QGroupBox *plotgroupBox;
    QGridLayout *gridLayout_5;
    QGridLayout *plotlayout;

    void setupUi(QWidget *ScatterPlot)
    {
        if (ScatterPlot->objectName().isEmpty())
            ScatterPlot->setObjectName(QString::fromUtf8("ScatterPlot"));
        ScatterPlot->resize(612, 634);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ScatterPlot->sizePolicy().hasHeightForWidth());
        ScatterPlot->setSizePolicy(sizePolicy);
        ScatterPlot->setMinimumSize(QSize(510, 425));
        ScatterPlot->setCursor(QCursor(Qt::ArrowCursor));
        actionExit = new QAction(ScatterPlot);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        gridLayout_2 = new QGridLayout(ScatterPlot);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        progressBar = new QProgressBar(ScatterPlot);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy1);
        progressBar->setMaximumSize(QSize(100, 20));
        progressBar->setValue(24);

        gridLayout_2->addWidget(progressBar, 2, 0, 1, 1);

        abortButton = new QPushButton(ScatterPlot);
        abortButton->setObjectName(QString::fromUtf8("abortButton"));
        abortButton->setMaximumSize(QSize(100, 25));

        gridLayout_2->addWidget(abortButton, 2, 1, 1, 2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        splitter = new QSplitter(ScatterPlot);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setOrientation(Qt::Horizontal);
        axis1_name = new QLabel(splitter);
        axis1_name->setObjectName(QString::fromUtf8("axis1_name"));
        sizePolicy.setHeightForWidth(axis1_name->sizePolicy().hasHeightForWidth());
        axis1_name->setSizePolicy(sizePolicy);
        axis1_name->setMinimumSize(QSize(57, 15));
        axis1_name->setMaximumSize(QSize(16777215, 16777215));
        axis1_name->setAlignment(Qt::AlignCenter);
        splitter->addWidget(axis1_name);
        axis1 = new QSpinBox(splitter);
        axis1->setObjectName(QString::fromUtf8("axis1"));
        axis1->setMinimumSize(QSize(80, 23));
        axis1->setMaximumSize(QSize(16777215, 16777215));
        axis1->setMinimum(1);
        splitter->addWidget(axis1);

        gridLayout->addWidget(splitter, 0, 0, 1, 1);

        splitter_2 = new QSplitter(ScatterPlot);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setOrientation(Qt::Horizontal);
        axis2_name = new QLabel(splitter_2);
        axis2_name->setObjectName(QString::fromUtf8("axis2_name"));
        sizePolicy.setHeightForWidth(axis2_name->sizePolicy().hasHeightForWidth());
        axis2_name->setSizePolicy(sizePolicy);
        axis2_name->setMinimumSize(QSize(57, 15));
        axis2_name->setMaximumSize(QSize(16777215, 16777215));
        axis2_name->setAlignment(Qt::AlignCenter);
        splitter_2->addWidget(axis2_name);
        axis2 = new QSpinBox(splitter_2);
        axis2->setObjectName(QString::fromUtf8("axis2"));
        axis2->setMinimumSize(QSize(80, 23));
        axis2->setMaximumSize(QSize(16777215, 16777215));
        axis2->setMinimum(1);
        axis2->setValue(2);
        splitter_2->addWidget(axis2);

        gridLayout->addWidget(splitter_2, 1, 0, 1, 1);

        splitter_3 = new QSplitter(ScatterPlot);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setOrientation(Qt::Horizontal);
        axis3_name = new QLabel(splitter_3);
        axis3_name->setObjectName(QString::fromUtf8("axis3_name"));
        sizePolicy.setHeightForWidth(axis3_name->sizePolicy().hasHeightForWidth());
        axis3_name->setSizePolicy(sizePolicy);
        axis3_name->setMinimumSize(QSize(57, 15));
        axis3_name->setMaximumSize(QSize(16777215, 16777215));
        axis3_name->setAlignment(Qt::AlignCenter);
        splitter_3->addWidget(axis3_name);
        axis3 = new QSpinBox(splitter_3);
        axis3->setObjectName(QString::fromUtf8("axis3"));
        axis3->setMinimumSize(QSize(80, 23));
        axis3->setMaximumSize(QSize(16777215, 16777215));
        axis3->setMinimum(1);
        splitter_3->addWidget(axis3);

        gridLayout->addWidget(splitter_3, 2, 0, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(424, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 2, 1, 1);

        plotgroupBox = new QGroupBox(ScatterPlot);
        plotgroupBox->setObjectName(QString::fromUtf8("plotgroupBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(plotgroupBox->sizePolicy().hasHeightForWidth());
        plotgroupBox->setSizePolicy(sizePolicy2);
        plotgroupBox->setMinimumSize(QSize(500, 300));
        gridLayout_5 = new QGridLayout(plotgroupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        plotlayout = new QGridLayout();
        plotlayout->setObjectName(QString::fromUtf8("plotlayout"));

        gridLayout_5->addLayout(plotlayout, 0, 0, 1, 1);


        gridLayout_2->addWidget(plotgroupBox, 0, 0, 1, 3);


        retranslateUi(ScatterPlot);

        QMetaObject::connectSlotsByName(ScatterPlot);
    } // setupUi

    void retranslateUi(QWidget *ScatterPlot)
    {
        ScatterPlot->setWindowTitle(QCoreApplication::translate("ScatterPlot", "Form", nullptr));
        actionExit->setText(QCoreApplication::translate("ScatterPlot", "Exit", nullptr));
#if QT_CONFIG(tooltip)
        actionExit->setToolTip(QCoreApplication::translate("ScatterPlot", "Exit", nullptr));
#endif // QT_CONFIG(tooltip)
        abortButton->setText(QCoreApplication::translate("ScatterPlot", "Abort", nullptr));
        axis1_name->setText(QCoreApplication::translate("ScatterPlot", "PC", nullptr));
        axis2_name->setText(QCoreApplication::translate("ScatterPlot", "PC", nullptr));
        axis3_name->setText(QCoreApplication::translate("ScatterPlot", "PC", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ScatterPlot: public Ui_ScatterPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SCATTERPLOT_H
