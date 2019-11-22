/********************************************************************************
** Form generated from reading UI file 'GenericProgressDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GENERICPROGRESSDIALOG_H
#define UI_GENERICPROGRESSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_GenericProgressDialog
{
public:
    QGridLayout *gridLayout;
    QProgressBar *progressBar;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;

    void setupUi(QDialog *GenericProgressDialog)
    {
        if (GenericProgressDialog->objectName().isEmpty())
            GenericProgressDialog->setObjectName(QString::fromUtf8("GenericProgressDialog"));
        GenericProgressDialog->setWindowModality(Qt::NonModal);
        GenericProgressDialog->resize(252, 74);
        GenericProgressDialog->setModal(true);
        gridLayout = new QGridLayout(GenericProgressDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        progressBar = new QProgressBar(GenericProgressDialog);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        progressBar->setValue(24);

        gridLayout->addWidget(progressBar, 0, 0, 1, 2);

        horizontalSpacer = new QSpacerItem(168, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 1, 0, 1, 1);

        cancelButton = new QPushButton(GenericProgressDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 1, 1, 1, 1);


        retranslateUi(GenericProgressDialog);

        QMetaObject::connectSlotsByName(GenericProgressDialog);
    } // setupUi

    void retranslateUi(QDialog *GenericProgressDialog)
    {
        GenericProgressDialog->setWindowTitle(QCoreApplication::translate("GenericProgressDialog", "Running progres..", nullptr));
        cancelButton->setText(QCoreApplication::translate("GenericProgressDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class GenericProgressDialog: public Ui_GenericProgressDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GENERICPROGRESSDIALOG_H
