/********************************************************************************
** Form generated from reading UI file 'DialogPlots.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOGPLOTS_H
#define UI_DIALOGPLOTS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_DialogPlots
{
public:
    QGridLayout *gridLayout_7;
    QLabel *label;
    QGroupBox *projectsGroupBox;
    QGridLayout *gridLayout_4;
    QListView *listView;
    QGroupBox *modelsGroupBox;
    QGridLayout *gridLayout_2;
    QListView *listView_2;
    QGroupBox *predictionGroupBox;
    QGridLayout *gridLayout_3;
    QListView *listView_3;
    QGroupBox *OptionGroupBox;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QSpinBox *numpc;
    QSpacerItem *horizontalSpacer_2;
    QLabel *label_4;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *plotButton;

    void setupUi(QDialog *DialogPlots)
    {
        if (DialogPlots->objectName().isEmpty())
            DialogPlots->setObjectName(QString::fromUtf8("DialogPlots"));
        DialogPlots->resize(703, 444);
        DialogPlots->setMinimumSize(QSize(560, 250));
        gridLayout_7 = new QGridLayout(DialogPlots);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        label = new QLabel(DialogPlots);
        label->setObjectName(QString::fromUtf8("label"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);

        gridLayout_7->addWidget(label, 0, 0, 1, 1);

        projectsGroupBox = new QGroupBox(DialogPlots);
        projectsGroupBox->setObjectName(QString::fromUtf8("projectsGroupBox"));
        gridLayout_4 = new QGridLayout(projectsGroupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        listView = new QListView(projectsGroupBox);
        listView->setObjectName(QString::fromUtf8("listView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy);
        listView->setMinimumSize(QSize(0, 0));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setSelectionMode(QAbstractItemView::SingleSelection);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_4->addWidget(listView, 0, 0, 1, 1);


        gridLayout_7->addWidget(projectsGroupBox, 1, 0, 1, 1);

        modelsGroupBox = new QGroupBox(DialogPlots);
        modelsGroupBox->setObjectName(QString::fromUtf8("modelsGroupBox"));
        gridLayout_2 = new QGridLayout(modelsGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        listView_2 = new QListView(modelsGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setMinimumSize(QSize(0, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_2->addWidget(listView_2, 0, 0, 1, 1);


        gridLayout_7->addWidget(modelsGroupBox, 1, 1, 1, 1);

        predictionGroupBox = new QGroupBox(DialogPlots);
        predictionGroupBox->setObjectName(QString::fromUtf8("predictionGroupBox"));
        gridLayout_3 = new QGridLayout(predictionGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listView_3 = new QListView(predictionGroupBox);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        listView_3->setMinimumSize(QSize(0, 0));
        listView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_3->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_3->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_3->addWidget(listView_3, 0, 0, 1, 1);


        gridLayout_7->addWidget(predictionGroupBox, 1, 2, 1, 1);

        OptionGroupBox = new QGroupBox(DialogPlots);
        OptionGroupBox->setObjectName(QString::fromUtf8("OptionGroupBox"));
        gridLayout_6 = new QGridLayout(OptionGroupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        numpc = new QSpinBox(OptionGroupBox);
        numpc->setObjectName(QString::fromUtf8("numpc"));
        numpc->setMinimum(1);
        numpc->setMaximum(999999999);

        gridLayout_5->addWidget(numpc, 0, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_2, 0, 0, 1, 1);

        label_4 = new QLabel(OptionGroupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_5->addWidget(label_4, 0, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);


        gridLayout_7->addWidget(OptionGroupBox, 2, 0, 1, 3);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(DialogPlots);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(cancelButton, 0, 1, 1, 1);

        plotButton = new QPushButton(DialogPlots);
        plotButton->setObjectName(QString::fromUtf8("plotButton"));
        plotButton->setMinimumSize(QSize(100, 24));
        plotButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(plotButton, 0, 2, 1, 1);


        gridLayout_7->addLayout(gridLayout, 3, 0, 1, 3);


        retranslateUi(DialogPlots);

        QMetaObject::connectSlotsByName(DialogPlots);
    } // setupUi

    void retranslateUi(QDialog *DialogPlots)
    {
        DialogPlots->setWindowTitle(QCoreApplication::translate("DialogPlots", "Select Project to plot...", nullptr));
        label->setText(QCoreApplication::translate("DialogPlots", "Select Project and model to plot...", nullptr));
        projectsGroupBox->setTitle(QCoreApplication::translate("DialogPlots", "Projects", nullptr));
        modelsGroupBox->setTitle(QCoreApplication::translate("DialogPlots", "Models", nullptr));
        predictionGroupBox->setTitle(QCoreApplication::translate("DialogPlots", "Predictions", nullptr));
        OptionGroupBox->setTitle(QCoreApplication::translate("DialogPlots", "Options", nullptr));
        label_4->setText(QCoreApplication::translate("DialogPlots", "Number of Latent Variables", nullptr));
        cancelButton->setText(QCoreApplication::translate("DialogPlots", "Cancel", nullptr));
        plotButton->setText(QCoreApplication::translate("DialogPlots", "Plot", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DialogPlots: public Ui_DialogPlots {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGPLOTS_H
