/********************************************************************************
** Form generated from reading UI file 'ImportLabelListDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORTLABELLISTDIALOG_H
#define UI_IMPORTLABELLISTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ImportLabelListDialog
{
public:
    QGridLayout *gridLayout_2;
    QLabel *label_4;
    QLineEdit *file;
    QPushButton *openButton;
    QLabel *label_6;
    QLineEdit *labelname;
    QSpacerItem *horizontalSpacer_6;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *ImportLabelListDialog)
    {
        if (ImportLabelListDialog->objectName().isEmpty())
            ImportLabelListDialog->setObjectName(QString::fromUtf8("ImportLabelListDialog"));
        ImportLabelListDialog->resize(684, 134);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ImportLabelListDialog->sizePolicy().hasHeightForWidth());
        ImportLabelListDialog->setSizePolicy(sizePolicy);
        ImportLabelListDialog->setMinimumSize(QSize(684, 134));
        ImportLabelListDialog->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_2 = new QGridLayout(ImportLabelListDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_4 = new QLabel(ImportLabelListDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(label_4, 0, 0, 1, 1);

        file = new QLineEdit(ImportLabelListDialog);
        file->setObjectName(QString::fromUtf8("file"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(file->sizePolicy().hasHeightForWidth());
        file->setSizePolicy(sizePolicy2);
        file->setMinimumSize(QSize(346, 24));

        gridLayout_2->addWidget(file, 0, 1, 1, 1);

        openButton = new QPushButton(ImportLabelListDialog);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setMinimumSize(QSize(85, 24));
        openButton->setMaximumSize(QSize(85, 24));

        gridLayout_2->addWidget(openButton, 0, 2, 1, 1);

        label_6 = new QLabel(ImportLabelListDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        gridLayout_2->addWidget(label_6, 1, 0, 1, 1);

        labelname = new QLineEdit(ImportLabelListDialog);
        labelname->setObjectName(QString::fromUtf8("labelname"));
        labelname->setMinimumSize(QSize(0, 24));

        gridLayout_2->addWidget(labelname, 1, 1, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(78, 13, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_6, 1, 2, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(261, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(ImportLabelListDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(cancelButton, 0, 1, 1, 1);

        okButton = new QPushButton(ImportLabelListDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(okButton, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout, 2, 0, 1, 3);


        retranslateUi(ImportLabelListDialog);

        QMetaObject::connectSlotsByName(ImportLabelListDialog);
    } // setupUi

    void retranslateUi(QDialog *ImportLabelListDialog)
    {
        ImportLabelListDialog->setWindowTitle(QCoreApplication::translate("ImportLabelListDialog", "Import Label List", nullptr));
        label_4->setText(QCoreApplication::translate("ImportLabelListDialog", "FIle...", nullptr));
        openButton->setText(QCoreApplication::translate("ImportLabelListDialog", "Open", nullptr));
        label_6->setText(QCoreApplication::translate("ImportLabelListDialog", "Label", nullptr));
        cancelButton->setText(QCoreApplication::translate("ImportLabelListDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ImportLabelListDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImportLabelListDialog: public Ui_ImportLabelListDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORTLABELLISTDIALOG_H
