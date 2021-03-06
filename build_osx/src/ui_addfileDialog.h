/********************************************************************************
** Form generated from reading UI file 'addfileDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDFILEDIALOG_H
#define UI_ADDFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_AddFileDialog
{
public:
    QGridLayout *gridLayout_4;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QLineEdit *file;
    QDialogButtonBox *buttonBox;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_6;
    QLineEdit *filename;
    QSpacerItem *horizontalSpacer_6;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QComboBox *splitlineby;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *ignorelinebychar;
    QHBoxLayout *horizontalLayout_2;
    QGridLayout *gridLayout_3;
    QCheckBox *fileobjname;
    QLineEdit *file_objname;
    QPushButton *openbutton;
    QCheckBox *filevarname;
    QLineEdit *file_varname;
    QPushButton *openbutton_2;
    QLabel *label_5;
    QTableView *tableView;
    QGridLayout *gridLayout_2;
    QPushButton *cancel;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *ok;

    void setupUi(QDialog *AddFileDialog)
    {
        if (AddFileDialog->objectName().isEmpty())
            AddFileDialog->setObjectName(QString::fromUtf8("AddFileDialog"));
        AddFileDialog->resize(557, 402);
        AddFileDialog->setModal(true);
        gridLayout_4 = new QGridLayout(AddFileDialog);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(AddFileDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_4->sizePolicy().hasHeightForWidth());
        label_4->setSizePolicy(sizePolicy);

        horizontalLayout_5->addWidget(label_4);

        file = new QLineEdit(AddFileDialog);
        file->setObjectName(QString::fromUtf8("file"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(file->sizePolicy().hasHeightForWidth());
        file->setSizePolicy(sizePolicy1);

        horizontalLayout_5->addWidget(file);

        buttonBox = new QDialogButtonBox(AddFileDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        sizePolicy.setHeightForWidth(buttonBox->sizePolicy().hasHeightForWidth());
        buttonBox->setSizePolicy(sizePolicy);
        buttonBox->setStandardButtons(QDialogButtonBox::Open);

        horizontalLayout_5->addWidget(buttonBox);


        gridLayout_4->addLayout(horizontalLayout_5, 0, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_6 = new QLabel(AddFileDialog);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_6->addWidget(label_6);

        filename = new QLineEdit(AddFileDialog);
        filename->setObjectName(QString::fromUtf8("filename"));

        horizontalLayout_6->addWidget(filename);

        horizontalSpacer_6 = new QSpacerItem(81, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_6);


        gridLayout_4->addLayout(horizontalLayout_6, 1, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(AddFileDialog);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMaximumSize(QSize(159, 23));

        horizontalLayout->addWidget(label);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        splitlineby = new QComboBox(AddFileDialog);
        splitlineby->addItem(QString());
        splitlineby->addItem(QString());
        splitlineby->addItem(QString());
        splitlineby->addItem(QString());
        splitlineby->setObjectName(QString::fromUtf8("splitlineby"));
        sizePolicy1.setHeightForWidth(splitlineby->sizePolicy().hasHeightForWidth());
        splitlineby->setSizePolicy(sizePolicy1);
        splitlineby->setMaximumSize(QSize(112, 23));
        splitlineby->setEditable(true);

        horizontalLayout->addWidget(splitlineby);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(AddFileDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
        label_3->setSizePolicy(sizePolicy1);
        label_3->setMaximumSize(QSize(159, 23));

        horizontalLayout_3->addWidget(label_3);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        ignorelinebychar = new QLineEdit(AddFileDialog);
        ignorelinebychar->setObjectName(QString::fromUtf8("ignorelinebychar"));
        sizePolicy1.setHeightForWidth(ignorelinebychar->sizePolicy().hasHeightForWidth());
        ignorelinebychar->setSizePolicy(sizePolicy1);
        ignorelinebychar->setMaximumSize(QSize(112, 23));

        horizontalLayout_3->addWidget(ignorelinebychar);


        gridLayout->addLayout(horizontalLayout_3, 2, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));

        gridLayout->addLayout(horizontalLayout_2, 1, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout, 2, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        fileobjname = new QCheckBox(AddFileDialog);
        fileobjname->setObjectName(QString::fromUtf8("fileobjname"));

        gridLayout_3->addWidget(fileobjname, 0, 0, 1, 1);

        file_objname = new QLineEdit(AddFileDialog);
        file_objname->setObjectName(QString::fromUtf8("file_objname"));

        gridLayout_3->addWidget(file_objname, 0, 1, 1, 1);

        openbutton = new QPushButton(AddFileDialog);
        openbutton->setObjectName(QString::fromUtf8("openbutton"));

        gridLayout_3->addWidget(openbutton, 0, 2, 1, 1);

        filevarname = new QCheckBox(AddFileDialog);
        filevarname->setObjectName(QString::fromUtf8("filevarname"));

        gridLayout_3->addWidget(filevarname, 1, 0, 1, 1);

        file_varname = new QLineEdit(AddFileDialog);
        file_varname->setObjectName(QString::fromUtf8("file_varname"));

        gridLayout_3->addWidget(file_varname, 1, 1, 1, 1);

        openbutton_2 = new QPushButton(AddFileDialog);
        openbutton_2->setObjectName(QString::fromUtf8("openbutton_2"));

        gridLayout_3->addWidget(openbutton_2, 1, 2, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 3, 0, 1, 1);

        label_5 = new QLabel(AddFileDialog);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_4->addWidget(label_5, 4, 0, 1, 1);

        tableView = new QTableView(AddFileDialog);
        tableView->setObjectName(QString::fromUtf8("tableView"));

        gridLayout_4->addWidget(tableView, 5, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        cancel = new QPushButton(AddFileDialog);
        cancel->setObjectName(QString::fromUtf8("cancel"));
        cancel->setMinimumSize(QSize(100, 0));
        cancel->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(cancel, 0, 2, 1, 1);

        horizontalSpacer_5 = new QSpacerItem(318, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_5, 0, 0, 1, 1);

        ok = new QPushButton(AddFileDialog);
        ok->setObjectName(QString::fromUtf8("ok"));
        ok->setMinimumSize(QSize(100, 0));
        ok->setMaximumSize(QSize(100, 16777215));

        gridLayout_2->addWidget(ok, 0, 3, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 6, 0, 1, 1);


        retranslateUi(AddFileDialog);

        QMetaObject::connectSlotsByName(AddFileDialog);
    } // setupUi

    void retranslateUi(QDialog *AddFileDialog)
    {
        AddFileDialog->setWindowTitle(QCoreApplication::translate("AddFileDialog", "Dialog", nullptr));
        label_4->setText(QCoreApplication::translate("AddFileDialog", "FIle...", nullptr));
        label_6->setText(QCoreApplication::translate("AddFileDialog", "Label", nullptr));
        label->setText(QCoreApplication::translate("AddFileDialog", "Separator:", nullptr));
        splitlineby->setItemText(0, QCoreApplication::translate("AddFileDialog", "TAB", nullptr));
        splitlineby->setItemText(1, QCoreApplication::translate("AddFileDialog", "SPACE", nullptr));
        splitlineby->setItemText(2, QCoreApplication::translate("AddFileDialog", ";", nullptr));
        splitlineby->setItemText(3, QString());

        label_3->setText(QCoreApplication::translate("AddFileDialog", "Ignore lines starting with:", nullptr));
        ignorelinebychar->setText(QCoreApplication::translate("AddFileDialog", "#", nullptr));
        fileobjname->setText(QCoreApplication::translate("AddFileDialog", "File Object Names", nullptr));
        openbutton->setText(QCoreApplication::translate("AddFileDialog", "Open", nullptr));
        filevarname->setText(QCoreApplication::translate("AddFileDialog", "File Variable Names", nullptr));
        openbutton_2->setText(QCoreApplication::translate("AddFileDialog", "Open", nullptr));
        label_5->setText(QCoreApplication::translate("AddFileDialog", "Previev...", nullptr));
        cancel->setText(QCoreApplication::translate("AddFileDialog", "Cancel", nullptr));
        ok->setText(QCoreApplication::translate("AddFileDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AddFileDialog: public Ui_AddFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDFILEDIALOG_H
