/********************************************************************************
** Form generated from reading UI file 'ExtractDataDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTRACTDATADIALOG_H
#define UI_EXTRACTDATADIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTableView>

QT_BEGIN_NAMESPACE

class Ui_ExtractDataDialog
{
public:
    QGridLayout *gridLayout_5;
    QGroupBox *projectsGroupBox;
    QGridLayout *gridLayout;
    QListView *listView;
    QGroupBox *variableGroupBox;
    QGridLayout *gridLayout_4;
    QTableView *tableView_4;
    QLabel *nselvar;
    QPushButton *varSelectAllButton;
    QPushButton *varInvertSelectionButton;
    QPushButton *varSelectByButton;
    QPushButton *varUnselectButton;
    QGroupBox *objectsGroupBox;
    QGridLayout *gridLayout_2;
    QPushButton *objInvertSelectionButton;
    QPushButton *objSelectByButton;
    QPushButton *objUnselectButton;
    QPushButton *objSelectAllButton;
    QLabel *nselobj;
    QTableView *tableView_3;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *dataname;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_3;
    QListView *listView_2;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *ExtractDataDialog)
    {
        if (ExtractDataDialog->objectName().isEmpty())
            ExtractDataDialog->setObjectName(QString::fromUtf8("ExtractDataDialog"));
        ExtractDataDialog->resize(1126, 460);
        gridLayout_5 = new QGridLayout(ExtractDataDialog);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        projectsGroupBox = new QGroupBox(ExtractDataDialog);
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


        gridLayout_5->addWidget(projectsGroupBox, 1, 0, 1, 2);

        variableGroupBox = new QGroupBox(ExtractDataDialog);
        variableGroupBox->setObjectName(QString::fromUtf8("variableGroupBox"));
        gridLayout_4 = new QGridLayout(variableGroupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        tableView_4 = new QTableView(variableGroupBox);
        tableView_4->setObjectName(QString::fromUtf8("tableView_4"));
        tableView_4->setMinimumSize(QSize(200, 0));
        tableView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView_4->setSelectionMode(QAbstractItemView::MultiSelection);
        tableView_4->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView_4->horizontalHeader()->setVisible(true);
        tableView_4->horizontalHeader()->setDefaultSectionSize(200);

        gridLayout_4->addWidget(tableView_4, 0, 0, 1, 2);

        nselvar = new QLabel(variableGroupBox);
        nselvar->setObjectName(QString::fromUtf8("nselvar"));

        gridLayout_4->addWidget(nselvar, 1, 0, 1, 1);

        varSelectAllButton = new QPushButton(variableGroupBox);
        varSelectAllButton->setObjectName(QString::fromUtf8("varSelectAllButton"));
        varSelectAllButton->setMinimumSize(QSize(130, 32));
        varSelectAllButton->setMaximumSize(QSize(130, 32));

        gridLayout_4->addWidget(varSelectAllButton, 2, 0, 1, 1);

        varInvertSelectionButton = new QPushButton(variableGroupBox);
        varInvertSelectionButton->setObjectName(QString::fromUtf8("varInvertSelectionButton"));
        varInvertSelectionButton->setMinimumSize(QSize(130, 32));
        varInvertSelectionButton->setMaximumSize(QSize(130, 32));

        gridLayout_4->addWidget(varInvertSelectionButton, 2, 1, 1, 1);

        varSelectByButton = new QPushButton(variableGroupBox);
        varSelectByButton->setObjectName(QString::fromUtf8("varSelectByButton"));
        varSelectByButton->setMinimumSize(QSize(130, 32));
        varSelectByButton->setMaximumSize(QSize(130, 32));

        gridLayout_4->addWidget(varSelectByButton, 3, 0, 1, 1);

        varUnselectButton = new QPushButton(variableGroupBox);
        varUnselectButton->setObjectName(QString::fromUtf8("varUnselectButton"));
        varUnselectButton->setMinimumSize(QSize(130, 32));
        varUnselectButton->setMaximumSize(QSize(130, 32));

        gridLayout_4->addWidget(varUnselectButton, 3, 1, 1, 1);


        gridLayout_5->addWidget(variableGroupBox, 1, 5, 1, 1);

        objectsGroupBox = new QGroupBox(ExtractDataDialog);
        objectsGroupBox->setObjectName(QString::fromUtf8("objectsGroupBox"));
        gridLayout_2 = new QGridLayout(objectsGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        objInvertSelectionButton = new QPushButton(objectsGroupBox);
        objInvertSelectionButton->setObjectName(QString::fromUtf8("objInvertSelectionButton"));
        objInvertSelectionButton->setMinimumSize(QSize(131, 32));
        objInvertSelectionButton->setMaximumSize(QSize(130, 32));

        gridLayout_2->addWidget(objInvertSelectionButton, 2, 1, 1, 1);

        objSelectByButton = new QPushButton(objectsGroupBox);
        objSelectByButton->setObjectName(QString::fromUtf8("objSelectByButton"));
        objSelectByButton->setMinimumSize(QSize(130, 32));
        objSelectByButton->setMaximumSize(QSize(130, 32));

        gridLayout_2->addWidget(objSelectByButton, 3, 0, 1, 1);

        objUnselectButton = new QPushButton(objectsGroupBox);
        objUnselectButton->setObjectName(QString::fromUtf8("objUnselectButton"));
        objUnselectButton->setMinimumSize(QSize(130, 32));
        objUnselectButton->setMaximumSize(QSize(130, 32));

        gridLayout_2->addWidget(objUnselectButton, 3, 1, 1, 1);

        objSelectAllButton = new QPushButton(objectsGroupBox);
        objSelectAllButton->setObjectName(QString::fromUtf8("objSelectAllButton"));
        objSelectAllButton->setMinimumSize(QSize(130, 32));
        objSelectAllButton->setMaximumSize(QSize(130, 32));

        gridLayout_2->addWidget(objSelectAllButton, 2, 0, 1, 1);

        nselobj = new QLabel(objectsGroupBox);
        nselobj->setObjectName(QString::fromUtf8("nselobj"));

        gridLayout_2->addWidget(nselobj, 1, 0, 1, 1);

        tableView_3 = new QTableView(objectsGroupBox);
        tableView_3->setObjectName(QString::fromUtf8("tableView_3"));
        tableView_3->setMinimumSize(QSize(200, 0));
        tableView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableView_3->setSelectionMode(QAbstractItemView::MultiSelection);
        tableView_3->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView_3->horizontalHeader()->setVisible(true);
        tableView_3->horizontalHeader()->setDefaultSectionSize(200);

        gridLayout_2->addWidget(tableView_3, 0, 0, 1, 2);


        gridLayout_5->addWidget(objectsGroupBox, 1, 3, 1, 2);

        horizontalSpacer_2 = new QSpacerItem(192, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_2, 0, 4, 1, 2);

        dataname = new QLineEdit(ExtractDataDialog);
        dataname->setObjectName(QString::fromUtf8("dataname"));

        gridLayout_5->addWidget(dataname, 0, 1, 1, 3);

        dataGroupBox = new QGroupBox(ExtractDataDialog);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        gridLayout_3 = new QGridLayout(dataGroupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        listView_2 = new QListView(dataGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setMinimumSize(QSize(200, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_3->addWidget(listView_2, 0, 0, 1, 2);


        gridLayout_5->addWidget(dataGroupBox, 1, 2, 1, 1);

        label_4 = new QLabel(ExtractDataDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_5->addWidget(label_4, 0, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        cancelButton = new QPushButton(ExtractDataDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 32));
        cancelButton->setMaximumSize(QSize(100, 32));

        horizontalLayout_2->addWidget(cancelButton);

        okButton = new QPushButton(ExtractDataDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 32));
        okButton->setMaximumSize(QSize(100, 32));

        horizontalLayout_2->addWidget(okButton);


        gridLayout_5->addLayout(horizontalLayout_2, 2, 0, 1, 6);


        retranslateUi(ExtractDataDialog);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(ExtractDataDialog);
    } // setupUi

    void retranslateUi(QDialog *ExtractDataDialog)
    {
        ExtractDataDialog->setWindowTitle(QCoreApplication::translate("ExtractDataDialog", "Extract Data...", nullptr));
        projectsGroupBox->setTitle(QCoreApplication::translate("ExtractDataDialog", "Projects", nullptr));
        variableGroupBox->setTitle(QString());
        nselvar->setText(QCoreApplication::translate("ExtractDataDialog", "TextLabel", nullptr));
        varSelectAllButton->setText(QCoreApplication::translate("ExtractDataDialog", "Select All", nullptr));
        varInvertSelectionButton->setText(QCoreApplication::translate("ExtractDataDialog", "Invert Selection", nullptr));
        varSelectByButton->setText(QCoreApplication::translate("ExtractDataDialog", "Select By...", nullptr));
        varUnselectButton->setText(QCoreApplication::translate("ExtractDataDialog", "Unselect", nullptr));
        objectsGroupBox->setTitle(QString());
        objInvertSelectionButton->setText(QCoreApplication::translate("ExtractDataDialog", "Invert Selection", nullptr));
        objSelectByButton->setText(QCoreApplication::translate("ExtractDataDialog", "Select By...", nullptr));
        objUnselectButton->setText(QCoreApplication::translate("ExtractDataDialog", "Unselect", nullptr));
        objSelectAllButton->setText(QCoreApplication::translate("ExtractDataDialog", "Select All", nullptr));
        nselobj->setText(QCoreApplication::translate("ExtractDataDialog", "TextLabel", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("ExtractDataDialog", "Data", nullptr));
        label_4->setText(QCoreApplication::translate("ExtractDataDialog", "Data Name", nullptr));
        cancelButton->setText(QCoreApplication::translate("ExtractDataDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ExtractDataDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExtractDataDialog: public Ui_ExtractDataDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTRACTDATADIALOG_H
