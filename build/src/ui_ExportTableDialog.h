/********************************************************************************
** Form generated from reading UI file 'ExportTableDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORTTABLEDIALOG_H
#define UI_EXPORTTABLEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ExportTableDialog
{
public:
    QGridLayout *gridLayout_7;
    QGridLayout *gridLayout;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_4;
    QListView *listView_1;
    QGridLayout *gridLayout_3;
    QPushButton *objselectAllButton;
    QPushButton *objinvertselectionButton;
    QComboBox *objSelectByLabel;
    QPushButton *objunselectAllButton;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_6;
    QListView *listView_2;
    QGridLayout *gridLayout_5;
    QPushButton *varselectAllButton;
    QPushButton *varinvertselectionButton;
    QComboBox *varSelectByLabel;
    QPushButton *varunselectAllButton;
    QSpacerItem *horizontalSpacer_2;
    QGridLayout *gridLayout_2;
    QLabel *label;
    QComboBox *separator;
    QPushButton *saveButton;
    QLineEdit *filetosave;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *ExportTableDialog)
    {
        if (ExportTableDialog->objectName().isEmpty())
            ExportTableDialog->setObjectName(QString::fromUtf8("ExportTableDialog"));
        ExportTableDialog->resize(665, 541);
        ExportTableDialog->setMinimumSize(QSize(665, 400));
        gridLayout_7 = new QGridLayout(ExportTableDialog);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        groupBox_2 = new QGroupBox(ExportTableDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(groupBox_2->sizePolicy().hasHeightForWidth());
        groupBox_2->setSizePolicy(sizePolicy);
        groupBox_2->setMinimumSize(QSize(280, 250));
        gridLayout_4 = new QGridLayout(groupBox_2);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        listView_1 = new QListView(groupBox_2);
        listView_1->setObjectName(QString::fromUtf8("listView_1"));
        listView_1->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_1->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_4->addWidget(listView_1, 0, 0, 1, 1);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        objselectAllButton = new QPushButton(groupBox_2);
        objselectAllButton->setObjectName(QString::fromUtf8("objselectAllButton"));
        objselectAllButton->setMinimumSize(QSize(130, 24));
        objselectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(objselectAllButton, 0, 0, 1, 1);

        objinvertselectionButton = new QPushButton(groupBox_2);
        objinvertselectionButton->setObjectName(QString::fromUtf8("objinvertselectionButton"));
        objinvertselectionButton->setMinimumSize(QSize(130, 24));
        objinvertselectionButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(objinvertselectionButton, 0, 1, 1, 1);

        objSelectByLabel = new QComboBox(groupBox_2);
        objSelectByLabel->setObjectName(QString::fromUtf8("objSelectByLabel"));
        objSelectByLabel->setMinimumSize(QSize(130, 24));
        objSelectByLabel->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(objSelectByLabel, 1, 0, 1, 1);

        objunselectAllButton = new QPushButton(groupBox_2);
        objunselectAllButton->setObjectName(QString::fromUtf8("objunselectAllButton"));
        objunselectAllButton->setMinimumSize(QSize(130, 24));
        objunselectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout_3->addWidget(objunselectAllButton, 1, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_2, 0, 0, 1, 1);

        groupBox_3 = new QGroupBox(ExportTableDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setMinimumSize(QSize(280, 250));
        gridLayout_6 = new QGridLayout(groupBox_3);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        listView_2 = new QListView(groupBox_3);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_6->addWidget(listView_2, 0, 0, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        varselectAllButton = new QPushButton(groupBox_3);
        varselectAllButton->setObjectName(QString::fromUtf8("varselectAllButton"));
        varselectAllButton->setMinimumSize(QSize(130, 24));
        varselectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(varselectAllButton, 0, 0, 1, 1);

        varinvertselectionButton = new QPushButton(groupBox_3);
        varinvertselectionButton->setObjectName(QString::fromUtf8("varinvertselectionButton"));
        varinvertselectionButton->setMinimumSize(QSize(130, 24));
        varinvertselectionButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(varinvertselectionButton, 0, 1, 1, 1);

        varSelectByLabel = new QComboBox(groupBox_3);
        varSelectByLabel->setObjectName(QString::fromUtf8("varSelectByLabel"));
        varSelectByLabel->setMinimumSize(QSize(130, 24));
        varSelectByLabel->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(varSelectByLabel, 1, 0, 1, 1);

        varunselectAllButton = new QPushButton(groupBox_3);
        varunselectAllButton->setObjectName(QString::fromUtf8("varunselectAllButton"));
        varunselectAllButton->setMinimumSize(QSize(130, 24));
        varunselectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(varunselectAllButton, 1, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 1, 0, 1, 1);


        gridLayout->addWidget(groupBox_3, 0, 1, 1, 1);


        gridLayout_7->addLayout(gridLayout, 0, 0, 1, 4);

        horizontalSpacer_2 = new QSpacerItem(291, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_2, 1, 0, 1, 2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label = new QLabel(ExportTableDialog);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);
        label->setMinimumSize(QSize(0, 23));
        label->setMaximumSize(QSize(159, 23));

        gridLayout_2->addWidget(label, 0, 0, 1, 1);

        separator = new QComboBox(ExportTableDialog);
        separator->addItem(QString());
        separator->addItem(QString());
        separator->addItem(QString());
        separator->addItem(QString());
        separator->addItem(QString());
        separator->setObjectName(QString::fromUtf8("separator"));
        sizePolicy1.setHeightForWidth(separator->sizePolicy().hasHeightForWidth());
        separator->setSizePolicy(sizePolicy1);
        separator->setMinimumSize(QSize(0, 23));
        separator->setMaximumSize(QSize(112, 23));
        separator->setEditable(true);

        gridLayout_2->addWidget(separator, 0, 1, 1, 1);


        gridLayout_7->addLayout(gridLayout_2, 1, 2, 1, 2);

        saveButton = new QPushButton(ExportTableDialog);
        saveButton->setObjectName(QString::fromUtf8("saveButton"));
        saveButton->setMinimumSize(QSize(100, 24));
        saveButton->setMaximumSize(QSize(100, 24));

        gridLayout_7->addWidget(saveButton, 2, 0, 1, 1);

        filetosave = new QLineEdit(ExportTableDialog);
        filetosave->setObjectName(QString::fromUtf8("filetosave"));
        sizePolicy1.setHeightForWidth(filetosave->sizePolicy().hasHeightForWidth());
        filetosave->setSizePolicy(sizePolicy1);
        filetosave->setMinimumSize(QSize(0, 23));
        filetosave->setMaximumSize(QSize(16777215, 23));

        gridLayout_7->addWidget(filetosave, 2, 1, 1, 3);

        horizontalSpacer = new QSpacerItem(434, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_7->addItem(horizontalSpacer, 3, 0, 1, 2);

        cancelButton = new QPushButton(ExportTableDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout_7->addWidget(cancelButton, 3, 2, 1, 1);

        okButton = new QPushButton(ExportTableDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout_7->addWidget(okButton, 3, 3, 1, 1);

        groupBox_2->raise();
        groupBox_3->raise();

        retranslateUi(ExportTableDialog);

        QMetaObject::connectSlotsByName(ExportTableDialog);
    } // setupUi

    void retranslateUi(QDialog *ExportTableDialog)
    {
        ExportTableDialog->setWindowTitle(QCoreApplication::translate("ExportTableDialog", "Export Table...", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("ExportTableDialog", "Objects", nullptr));
        objselectAllButton->setText(QCoreApplication::translate("ExportTableDialog", "Select All", nullptr));
        objinvertselectionButton->setText(QCoreApplication::translate("ExportTableDialog", "Invert Selection...", nullptr));
        objunselectAllButton->setText(QCoreApplication::translate("ExportTableDialog", "Unselect All", nullptr));
        groupBox_3->setTitle(QCoreApplication::translate("ExportTableDialog", "Variables", nullptr));
        varselectAllButton->setText(QCoreApplication::translate("ExportTableDialog", "Select All", nullptr));
        varinvertselectionButton->setText(QCoreApplication::translate("ExportTableDialog", "Invert Selection...", nullptr));
        varunselectAllButton->setText(QCoreApplication::translate("ExportTableDialog", "Unselect All", nullptr));
        label->setText(QCoreApplication::translate("ExportTableDialog", "Separator:", nullptr));
        separator->setItemText(0, QCoreApplication::translate("ExportTableDialog", "TAB", nullptr));
        separator->setItemText(1, QCoreApplication::translate("ExportTableDialog", "SPACE", nullptr));
        separator->setItemText(2, QCoreApplication::translate("ExportTableDialog", ",", nullptr));
        separator->setItemText(3, QCoreApplication::translate("ExportTableDialog", ";", nullptr));
        separator->setItemText(4, QCoreApplication::translate("ExportTableDialog", "<CUSTOM>", nullptr));

        saveButton->setText(QCoreApplication::translate("ExportTableDialog", "Save As...", nullptr));
        cancelButton->setText(QCoreApplication::translate("ExportTableDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ExportTableDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ExportTableDialog: public Ui_ExportTableDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORTTABLEDIALOG_H
