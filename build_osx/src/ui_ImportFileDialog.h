/********************************************************************************
** Form generated from reading UI file 'ImportFileDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORTFILEDIALOG_H
#define UI_IMPORTFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_ImportFileDialog
{
public:
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_11;
    QLineEdit *filename;
    QLabel *label_8;
    QLineEdit *file;
    QPushButton *openButton;
    QLabel *label_7;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_5;
    QCheckBox *transpose;
    QLineEdit *ignorelinebychar;
    QCheckBox *skipcolmissingvalue;
    QCheckBox *firstcolobjname;
    QCheckBox *firstrowvarname;
    QCheckBox *fix_empty_values;
    QComboBox *splitlineby;
    QGridLayout *gridLayout_2;
    QSpinBox *colmissvalueBox;
    QLabel *label_9;
    QLabel *label;
    QCheckBox *ignore_lines_start_char;
    QSpacerItem *horizontalSpacer;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QTableView *tableView;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *ImportFileDialog)
    {
        if (ImportFileDialog->objectName().isEmpty())
            ImportFileDialog->setObjectName(QString::fromUtf8("ImportFileDialog"));
        ImportFileDialog->setWindowModality(Qt::NonModal);
        ImportFileDialog->resize(676, 538);
        ImportFileDialog->setMaximumSize(QSize(16777215, 16777215));
        ImportFileDialog->setModal(true);
        gridLayout_4 = new QGridLayout(ImportFileDialog);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_11 = new QGridLayout();
        gridLayout_11->setObjectName(QString::fromUtf8("gridLayout_11"));
        filename = new QLineEdit(ImportFileDialog);
        filename->setObjectName(QString::fromUtf8("filename"));
        filename->setMinimumSize(QSize(0, 23));
        filename->setMaximumSize(QSize(16777215, 23));

        gridLayout_11->addWidget(filename, 1, 1, 1, 1);

        label_8 = new QLabel(ImportFileDialog);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(0, 23));
        label_8->setMaximumSize(QSize(16777215, 23));

        gridLayout_11->addWidget(label_8, 1, 0, 1, 1);

        file = new QLineEdit(ImportFileDialog);
        file->setObjectName(QString::fromUtf8("file"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(file->sizePolicy().hasHeightForWidth());
        file->setSizePolicy(sizePolicy);
        file->setMinimumSize(QSize(0, 23));
        file->setMaximumSize(QSize(16777215, 23));

        gridLayout_11->addWidget(file, 0, 1, 1, 1);

        openButton = new QPushButton(ImportFileDialog);
        openButton->setObjectName(QString::fromUtf8("openButton"));
        openButton->setMinimumSize(QSize(100, 32));
        openButton->setMaximumSize(QSize(100, 32));

        gridLayout_11->addWidget(openButton, 0, 2, 1, 1);

        label_7 = new QLabel(ImportFileDialog);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label_7->sizePolicy().hasHeightForWidth());
        label_7->setSizePolicy(sizePolicy1);
        label_7->setMinimumSize(QSize(0, 15));
        label_7->setMaximumSize(QSize(16777215, 15));

        gridLayout_11->addWidget(label_7, 0, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_11, 0, 0, 1, 1);

        groupBox = new QGroupBox(ImportFileDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMinimumSize(QSize(0, 0));
        groupBox->setMaximumSize(QSize(16777215, 16777215));
        gridLayout_6 = new QGridLayout(groupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        transpose = new QCheckBox(groupBox);
        transpose->setObjectName(QString::fromUtf8("transpose"));
        transpose->setMinimumSize(QSize(0, 21));
        transpose->setMaximumSize(QSize(16777215, 21));

        gridLayout_5->addWidget(transpose, 3, 0, 1, 1);

        ignorelinebychar = new QLineEdit(groupBox);
        ignorelinebychar->setObjectName(QString::fromUtf8("ignorelinebychar"));
        sizePolicy.setHeightForWidth(ignorelinebychar->sizePolicy().hasHeightForWidth());
        ignorelinebychar->setSizePolicy(sizePolicy);
        ignorelinebychar->setMinimumSize(QSize(0, 23));
        ignorelinebychar->setMaximumSize(QSize(112, 23));

        gridLayout_5->addWidget(ignorelinebychar, 6, 1, 1, 1);

        skipcolmissingvalue = new QCheckBox(groupBox);
        skipcolmissingvalue->setObjectName(QString::fromUtf8("skipcolmissingvalue"));
        skipcolmissingvalue->setMinimumSize(QSize(0, 21));
        skipcolmissingvalue->setMaximumSize(QSize(16777215, 21));
        skipcolmissingvalue->setChecked(false);

        gridLayout_5->addWidget(skipcolmissingvalue, 4, 0, 1, 1);

        firstcolobjname = new QCheckBox(groupBox);
        firstcolobjname->setObjectName(QString::fromUtf8("firstcolobjname"));
        firstcolobjname->setMinimumSize(QSize(0, 21));
        firstcolobjname->setMaximumSize(QSize(16777215, 21));

        gridLayout_5->addWidget(firstcolobjname, 0, 0, 1, 1);

        firstrowvarname = new QCheckBox(groupBox);
        firstrowvarname->setObjectName(QString::fromUtf8("firstrowvarname"));
        firstrowvarname->setMinimumSize(QSize(0, 21));
        firstrowvarname->setMaximumSize(QSize(16777215, 21));

        gridLayout_5->addWidget(firstrowvarname, 1, 0, 1, 1);

        fix_empty_values = new QCheckBox(groupBox);
        fix_empty_values->setObjectName(QString::fromUtf8("fix_empty_values"));
        fix_empty_values->setChecked(false);

        gridLayout_5->addWidget(fix_empty_values, 2, 0, 1, 2);

        splitlineby = new QComboBox(groupBox);
        splitlineby->addItem(QString());
        splitlineby->addItem(QString());
        splitlineby->addItem(QString());
        splitlineby->addItem(QString());
        splitlineby->addItem(QString());
        splitlineby->setObjectName(QString::fromUtf8("splitlineby"));
        sizePolicy.setHeightForWidth(splitlineby->sizePolicy().hasHeightForWidth());
        splitlineby->setSizePolicy(sizePolicy);
        splitlineby->setMinimumSize(QSize(0, 23));
        splitlineby->setMaximumSize(QSize(112, 23));
        splitlineby->setEditable(true);

        gridLayout_5->addWidget(splitlineby, 5, 1, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        colmissvalueBox = new QSpinBox(groupBox);
        colmissvalueBox->setObjectName(QString::fromUtf8("colmissvalueBox"));
        colmissvalueBox->setMinimum(1);
        colmissvalueBox->setMaximum(100);
        colmissvalueBox->setValue(20);

        gridLayout_2->addWidget(colmissvalueBox, 0, 0, 1, 1);

        label_9 = new QLabel(groupBox);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_2->addWidget(label_9, 0, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 4, 1, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        label->setMinimumSize(QSize(0, 23));
        label->setMaximumSize(QSize(159, 23));

        gridLayout_5->addWidget(label, 5, 0, 1, 1);

        ignore_lines_start_char = new QCheckBox(groupBox);
        ignore_lines_start_char->setObjectName(QString::fromUtf8("ignore_lines_start_char"));

        gridLayout_5->addWidget(ignore_lines_start_char, 6, 0, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(317, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer, 0, 1, 1, 1);


        gridLayout_4->addWidget(groupBox, 1, 0, 1, 1);

        groupBox_2 = new QGroupBox(ImportFileDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        tableView = new QTableView(groupBox_2);
        tableView->setObjectName(QString::fromUtf8("tableView"));
        tableView->setMinimumSize(QSize(0, 0));
        tableView->setMaximumSize(QSize(16777215, 330));

        gridLayout_3->addWidget(tableView, 0, 0, 1, 1);


        gridLayout_4->addWidget(groupBox_2, 2, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_5, 0, 0, 1, 1);

        cancelButton = new QPushButton(ImportFileDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 32));
        cancelButton->setMaximumSize(QSize(100, 32));

        gridLayout->addWidget(cancelButton, 0, 1, 1, 1);

        okButton = new QPushButton(ImportFileDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 32));
        okButton->setMaximumSize(QSize(100, 32));

        gridLayout->addWidget(okButton, 0, 2, 1, 1);


        gridLayout_4->addLayout(gridLayout, 3, 0, 1, 1);


        retranslateUi(ImportFileDialog);

        openButton->setDefault(true);
        okButton->setDefault(false);


        QMetaObject::connectSlotsByName(ImportFileDialog);
    } // setupUi

    void retranslateUi(QDialog *ImportFileDialog)
    {
        ImportFileDialog->setWindowTitle(QCoreApplication::translate("ImportFileDialog", "Import File...", nullptr));
        label_8->setText(QCoreApplication::translate("ImportFileDialog", "Label", nullptr));
        file->setText(QString());
        openButton->setText(QCoreApplication::translate("ImportFileDialog", "Open", nullptr));
        label_7->setText(QCoreApplication::translate("ImportFileDialog", "FIle...", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ImportFileDialog", "Options", nullptr));
        transpose->setText(QCoreApplication::translate("ImportFileDialog", "Transpose", nullptr));
        ignorelinebychar->setText(QCoreApplication::translate("ImportFileDialog", "#", nullptr));
        skipcolmissingvalue->setText(QCoreApplication::translate("ImportFileDialog", "Skip Column with missing values over", nullptr));
        firstcolobjname->setText(QCoreApplication::translate("ImportFileDialog", "First columns contain object names", nullptr));
        firstrowvarname->setText(QCoreApplication::translate("ImportFileDialog", "First row contain column names", nullptr));
        fix_empty_values->setText(QCoreApplication::translate("ImportFileDialog", "Fix empty values with column average value", nullptr));
        splitlineby->setItemText(0, QCoreApplication::translate("ImportFileDialog", "TAB", nullptr));
        splitlineby->setItemText(1, QCoreApplication::translate("ImportFileDialog", "SPACE", nullptr));
        splitlineby->setItemText(2, QCoreApplication::translate("ImportFileDialog", ",", nullptr));
        splitlineby->setItemText(3, QCoreApplication::translate("ImportFileDialog", ";", nullptr));
        splitlineby->setItemText(4, QCoreApplication::translate("ImportFileDialog", "<CUSTOM>", nullptr));

        label_9->setText(QCoreApplication::translate("ImportFileDialog", "%", nullptr));
        label->setText(QCoreApplication::translate("ImportFileDialog", "Separator:", nullptr));
        ignore_lines_start_char->setText(QCoreApplication::translate("ImportFileDialog", "Ignore lines starting with:", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ImportFileDialog", "Preview", nullptr));
        cancelButton->setText(QCoreApplication::translate("ImportFileDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ImportFileDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ImportFileDialog: public Ui_ImportFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORTFILEDIALOG_H
