/********************************************************************************
** Form generated from reading UI file 'MergeDataDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MERGEDATADIALOG_H
#define UI_MERGEDATADIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_MergeDataDialog
{
public:
    QGridLayout *gridLayout_2;
    QGroupBox *projectsGroupBox;
    QGridLayout *gridLayout;
    QListView *listView;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_3;
    QListView *listView_2;
    QRadioButton *mergeallcol;
    QRadioButton *mergematchcol;
    QSpacerItem *horizontalSpacer;
    QLabel *label_4;
    QLineEdit *dataname;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *MergeDataDialog)
    {
        if (MergeDataDialog->objectName().isEmpty())
            MergeDataDialog->setObjectName(QString::fromUtf8("MergeDataDialog"));
        MergeDataDialog->resize(598, 382);
        gridLayout_2 = new QGridLayout(MergeDataDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        projectsGroupBox = new QGroupBox(MergeDataDialog);
        projectsGroupBox->setObjectName(QString::fromUtf8("projectsGroupBox"));
        gridLayout = new QGridLayout(projectsGroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listView = new QListView(projectsGroupBox);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setMinimumSize(QSize(200, 0));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setSelectionMode(QAbstractItemView::SingleSelection);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout->addWidget(listView, 0, 0, 1, 1);


        gridLayout_2->addWidget(projectsGroupBox, 0, 0, 1, 2);

        dataGroupBox = new QGroupBox(MergeDataDialog);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        gridLayout_3 = new QGridLayout(dataGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listView_2 = new QListView(dataGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setMinimumSize(QSize(200, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::MultiSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_3->addWidget(listView_2, 0, 0, 1, 2);


        gridLayout_2->addWidget(dataGroupBox, 0, 2, 1, 2);

        mergeallcol = new QRadioButton(MergeDataDialog);
        mergeallcol->setObjectName(QString::fromUtf8("mergeallcol"));
        mergeallcol->setChecked(true);

        gridLayout_2->addWidget(mergeallcol, 1, 0, 1, 1);

        mergematchcol = new QRadioButton(MergeDataDialog);
        mergematchcol->setObjectName(QString::fromUtf8("mergematchcol"));

        gridLayout_2->addWidget(mergematchcol, 1, 1, 1, 2);

        horizontalSpacer = new QSpacerItem(387, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 1, 3, 1, 1);

        label_4 = new QLabel(MergeDataDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        dataname = new QLineEdit(MergeDataDialog);
        dataname->setObjectName(QString::fromUtf8("dataname"));
        dataname->setMinimumSize(QSize(200, 20));

        gridLayout_2->addWidget(dataname, 2, 1, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(448, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_2, 2, 3, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        cancelButton = new QPushButton(MergeDataDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 32));
        cancelButton->setMaximumSize(QSize(100, 32));

        gridLayout_4->addWidget(cancelButton, 0, 1, 1, 1);

        okButton = new QPushButton(MergeDataDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 32));
        okButton->setMaximumSize(QSize(100, 32));

        gridLayout_4->addWidget(okButton, 0, 2, 1, 1);


        gridLayout_2->addLayout(gridLayout_4, 3, 0, 1, 4);


        retranslateUi(MergeDataDialog);

        QMetaObject::connectSlotsByName(MergeDataDialog);
    } // setupUi

    void retranslateUi(QDialog *MergeDataDialog)
    {
        MergeDataDialog->setWindowTitle(QCoreApplication::translate("MergeDataDialog", "Merge Data Dialog", nullptr));
        projectsGroupBox->setTitle(QCoreApplication::translate("MergeDataDialog", "Projects", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("MergeDataDialog", "Data", nullptr));
        mergeallcol->setText(QCoreApplication::translate("MergeDataDialog", "Merge All Column", nullptr));
        mergematchcol->setText(QCoreApplication::translate("MergeDataDialog", "Merge Matched Column", nullptr));
        label_4->setText(QCoreApplication::translate("MergeDataDialog", "Merged Data Name", nullptr));
        cancelButton->setText(QCoreApplication::translate("MergeDataDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("MergeDataDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MergeDataDialog: public Ui_MergeDataDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MERGEDATADIALOG_H
