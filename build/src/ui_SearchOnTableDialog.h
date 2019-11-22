/********************************************************************************
** Form generated from reading UI file 'SearchOnTableDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SEARCHONTABLEDIALOG_H
#define UI_SEARCHONTABLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SearchOnTableDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QComboBox *selectionlist;
    QFrame *variableframe;
    QGridLayout *gridLayout_3;
    QGridLayout *gridLayout_2;
    QLineEdit *minvaledit;
    QLabel *label_3;
    QLabel *label_2;
    QLineEdit *maxvaledit;
    QFrame *labelframe;
    QGridLayout *gridLayout_4;
    QLineEdit *lineEdit;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *SearchOnTableDialog)
    {
        if (SearchOnTableDialog->objectName().isEmpty())
            SearchOnTableDialog->setObjectName(QString::fromUtf8("SearchOnTableDialog"));
        SearchOnTableDialog->resize(330, 316);
        SearchOnTableDialog->setMinimumSize(QSize(330, 316));
        SearchOnTableDialog->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(SearchOnTableDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(SearchOnTableDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        selectionlist = new QComboBox(SearchOnTableDialog);
        selectionlist->setObjectName(QString::fromUtf8("selectionlist"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(selectionlist->sizePolicy().hasHeightForWidth());
        selectionlist->setSizePolicy(sizePolicy);
        selectionlist->setMinimumSize(QSize(130, 24));
        selectionlist->setMaximumSize(QSize(16777215, 16777215));
        selectionlist->setEditable(false);

        gridLayout->addWidget(selectionlist, 0, 1, 1, 1);

        variableframe = new QFrame(SearchOnTableDialog);
        variableframe->setObjectName(QString::fromUtf8("variableframe"));
        variableframe->setMinimumSize(QSize(220, 110));
        variableframe->setFrameShape(QFrame::StyledPanel);
        variableframe->setFrameShadow(QFrame::Raised);
        gridLayout_3 = new QGridLayout(variableframe);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        minvaledit = new QLineEdit(variableframe);
        minvaledit->setObjectName(QString::fromUtf8("minvaledit"));
        minvaledit->setMinimumSize(QSize(0, 24));

        gridLayout_2->addWidget(minvaledit, 0, 1, 1, 1);

        label_3 = new QLabel(variableframe);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        label_2 = new QLabel(variableframe);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        maxvaledit = new QLineEdit(variableframe);
        maxvaledit->setObjectName(QString::fromUtf8("maxvaledit"));
        maxvaledit->setMinimumSize(QSize(0, 24));

        gridLayout_2->addWidget(maxvaledit, 1, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 0, 0, 1, 2);


        gridLayout->addWidget(variableframe, 1, 0, 1, 3);

        labelframe = new QFrame(SearchOnTableDialog);
        labelframe->setObjectName(QString::fromUtf8("labelframe"));
        labelframe->setMinimumSize(QSize(220, 37));
        labelframe->setMaximumSize(QSize(16777215, 16777215));
        labelframe->setFrameShape(QFrame::StyledPanel);
        labelframe->setFrameShadow(QFrame::Raised);
        gridLayout_4 = new QGridLayout(labelframe);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        lineEdit = new QLineEdit(labelframe);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setMinimumSize(QSize(0, 24));

        gridLayout_4->addWidget(lineEdit, 0, 0, 1, 1);


        gridLayout->addWidget(labelframe, 2, 0, 1, 3);

        cancelButton = new QPushButton(SearchOnTableDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(cancelButton, 3, 0, 1, 1);

        okButton = new QPushButton(SearchOnTableDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(okButton, 3, 2, 1, 1);


        retranslateUi(SearchOnTableDialog);

        QMetaObject::connectSlotsByName(SearchOnTableDialog);
    } // setupUi

    void retranslateUi(QDialog *SearchOnTableDialog)
    {
        SearchOnTableDialog->setWindowTitle(QCoreApplication::translate("SearchOnTableDialog", "Search by...", nullptr));
        label->setText(QCoreApplication::translate("SearchOnTableDialog", "Search by...", nullptr));
        label_3->setText(QCoreApplication::translate("SearchOnTableDialog", "Max:", nullptr));
        label_2->setText(QCoreApplication::translate("SearchOnTableDialog", "Min:", nullptr));
        cancelButton->setText(QCoreApplication::translate("SearchOnTableDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("SearchOnTableDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SearchOnTableDialog: public Ui_SearchOnTableDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SEARCHONTABLEDIALOG_H
