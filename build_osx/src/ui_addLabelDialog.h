/********************************************************************************
** Form generated from reading UI file 'addLabelDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDLABELDIALOG_H
#define UI_ADDLABELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_addLabelDialog
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *addLabelDialog)
    {
        if (addLabelDialog->objectName().isEmpty())
            addLabelDialog->setObjectName(QString::fromUtf8("addLabelDialog"));
        addLabelDialog->resize(207, 65);
        gridLayout = new QGridLayout(addLabelDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        lineEdit = new QLineEdit(addLabelDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 0, 1, 2);

        cancelButton = new QPushButton(addLabelDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        gridLayout->addWidget(cancelButton, 1, 0, 1, 1);

        okButton = new QPushButton(addLabelDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));

        gridLayout->addWidget(okButton, 1, 1, 1, 1);


        retranslateUi(addLabelDialog);

        QMetaObject::connectSlotsByName(addLabelDialog);
    } // setupUi

    void retranslateUi(QDialog *addLabelDialog)
    {
        addLabelDialog->setWindowTitle(QCoreApplication::translate("addLabelDialog", "addLabelDialog", nullptr));
        cancelButton->setText(QCoreApplication::translate("addLabelDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("addLabelDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class addLabelDialog: public Ui_addLabelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDLABELDIALOG_H
