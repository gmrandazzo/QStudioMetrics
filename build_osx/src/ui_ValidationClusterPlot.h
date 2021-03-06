/********************************************************************************
** Form generated from reading UI file 'ValidationClusterPlot.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VALIDATIONCLUSTERPLOT_H
#define UI_VALIDATIONCLUSTERPLOT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ValidationClusterPlot
{
public:
    QGridLayout *gridLayout_2;
    QWidget *plotwidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpinBox *nclustersBox;
    QPushButton *saveimageButton;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *ValidationClusterPlot)
    {
        if (ValidationClusterPlot->objectName().isEmpty())
            ValidationClusterPlot->setObjectName(QString::fromUtf8("ValidationClusterPlot"));
        ValidationClusterPlot->resize(524, 472);
        gridLayout_2 = new QGridLayout(ValidationClusterPlot);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        plotwidget = new QWidget(ValidationClusterPlot);
        plotwidget->setObjectName(QString::fromUtf8("plotwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(plotwidget->sizePolicy().hasHeightForWidth());
        plotwidget->setSizePolicy(sizePolicy);
        plotwidget->setMinimumSize(QSize(500, 300));

        gridLayout_2->addWidget(plotwidget, 0, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(ValidationClusterPlot);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        nclustersBox = new QSpinBox(ValidationClusterPlot);
        nclustersBox->setObjectName(QString::fromUtf8("nclustersBox"));
        nclustersBox->setMinimum(2);
        nclustersBox->setMaximum(9999);

        gridLayout->addWidget(nclustersBox, 0, 1, 1, 1);

        saveimageButton = new QPushButton(ValidationClusterPlot);
        saveimageButton->setObjectName(QString::fromUtf8("saveimageButton"));
        saveimageButton->setMinimumSize(QSize(100, 24));
        saveimageButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(saveimageButton, 0, 2, 1, 1);

        okButton = new QPushButton(ValidationClusterPlot);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(okButton, 0, 4, 1, 1);

        cancelButton = new QPushButton(ValidationClusterPlot);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(60, 16777215));

        gridLayout->addWidget(cancelButton, 0, 3, 1, 1);


        gridLayout_2->addLayout(gridLayout, 1, 0, 1, 1);


        retranslateUi(ValidationClusterPlot);

        QMetaObject::connectSlotsByName(ValidationClusterPlot);
    } // setupUi

    void retranslateUi(QDialog *ValidationClusterPlot)
    {
        ValidationClusterPlot->setWindowTitle(QCoreApplication::translate("ValidationClusterPlot", "Validation Cluster", nullptr));
        label->setText(QCoreApplication::translate("ValidationClusterPlot", "Number of Clusters", nullptr));
        saveimageButton->setText(QCoreApplication::translate("ValidationClusterPlot", "Save Image..", nullptr));
        okButton->setText(QCoreApplication::translate("ValidationClusterPlot", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("ValidationClusterPlot", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ValidationClusterPlot: public Ui_ValidationClusterPlot {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VALIDATIONCLUSTERPLOT_H
