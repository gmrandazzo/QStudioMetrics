/********************************************************************************
** Form generated from reading UI file 'LabelDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LABELDIALOG_H
#define UI_LABELDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_LabelDialog
{
public:
    QGridLayout *gridLayout_7;
    QGroupBox *projectgroupBox;
    QGridLayout *gridLayout_4;
    QListView *projectView;
    QGridLayout *gridLayout_2;
    QPushButton *cancelButton;
    QPushButton *closeButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QGroupBox *labelgroupBox;
    QGridLayout *gridLayout;
    QPushButton *addButton;
    QListView *labelView;
    QPushButton *importlistButton;
    QPushButton *importtabButton;
    QPushButton *showButton;
    QPushButton *removeButton;
    QSpacerItem *verticalSpacer;

    void setupUi(QDialog *LabelDialog)
    {
        if (LabelDialog->objectName().isEmpty())
            LabelDialog->setObjectName(QString::fromUtf8("LabelDialog"));
        LabelDialog->resize(529, 278);
        gridLayout_7 = new QGridLayout(LabelDialog);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        projectgroupBox = new QGroupBox(LabelDialog);
        projectgroupBox->setObjectName(QString::fromUtf8("projectgroupBox"));
        gridLayout_4 = new QGridLayout(projectgroupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        projectView = new QListView(projectgroupBox);
        projectView->setObjectName(QString::fromUtf8("projectView"));
        projectView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        projectView->setAlternatingRowColors(true);

        gridLayout_4->addWidget(projectView, 0, 0, 1, 1);


        gridLayout_7->addWidget(projectgroupBox, 0, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        cancelButton = new QPushButton(LabelDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(120, 32));
        cancelButton->setMaximumSize(QSize(120, 32));

        gridLayout_2->addWidget(cancelButton, 0, 2, 1, 1);

        closeButton = new QPushButton(LabelDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setMinimumSize(QSize(120, 32));
        closeButton->setMaximumSize(QSize(120, 32));

        gridLayout_2->addWidget(closeButton, 0, 4, 1, 1);

        horizontalSpacer = new QSpacerItem(47, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer, 0, 0, 1, 1);

        okButton = new QPushButton(LabelDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(120, 32));
        okButton->setMaximumSize(QSize(120, 32));

        gridLayout_2->addWidget(okButton, 0, 3, 1, 1);


        gridLayout_7->addLayout(gridLayout_2, 1, 0, 1, 2);

        labelgroupBox = new QGroupBox(LabelDialog);
        labelgroupBox->setObjectName(QString::fromUtf8("labelgroupBox"));
        gridLayout = new QGridLayout(labelgroupBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        addButton = new QPushButton(labelgroupBox);
        addButton->setObjectName(QString::fromUtf8("addButton"));
        addButton->setMinimumSize(QSize(120, 32));
        addButton->setMaximumSize(QSize(120, 32));

        gridLayout->addWidget(addButton, 0, 0, 1, 1);

        labelView = new QListView(labelgroupBox);
        labelView->setObjectName(QString::fromUtf8("labelView"));
        labelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        labelView->setAlternatingRowColors(true);

        gridLayout->addWidget(labelView, 0, 1, 5, 1);

        importlistButton = new QPushButton(labelgroupBox);
        importlistButton->setObjectName(QString::fromUtf8("importlistButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(85);
        sizePolicy.setVerticalStretch(24);
        sizePolicy.setHeightForWidth(importlistButton->sizePolicy().hasHeightForWidth());
        importlistButton->setSizePolicy(sizePolicy);
        importlistButton->setMinimumSize(QSize(120, 32));
        importlistButton->setMaximumSize(QSize(120, 32));

        gridLayout->addWidget(importlistButton, 1, 0, 1, 1);

        importtabButton = new QPushButton(labelgroupBox);
        importtabButton->setObjectName(QString::fromUtf8("importtabButton"));
        importtabButton->setMinimumSize(QSize(120, 32));
        importtabButton->setMaximumSize(QSize(120, 32));

        gridLayout->addWidget(importtabButton, 2, 0, 1, 1);

        showButton = new QPushButton(labelgroupBox);
        showButton->setObjectName(QString::fromUtf8("showButton"));
        showButton->setMinimumSize(QSize(120, 32));
        showButton->setMaximumSize(QSize(120, 32));

        gridLayout->addWidget(showButton, 3, 0, 1, 1);

        removeButton = new QPushButton(labelgroupBox);
        removeButton->setObjectName(QString::fromUtf8("removeButton"));
        removeButton->setMinimumSize(QSize(120, 32));
        removeButton->setMaximumSize(QSize(120, 32));

        gridLayout->addWidget(removeButton, 4, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 14, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 5, 0, 1, 1);

        importlistButton->raise();
        importtabButton->raise();
        showButton->raise();
        removeButton->raise();
        labelView->raise();
        addButton->raise();

        gridLayout_7->addWidget(labelgroupBox, 0, 1, 1, 1);

        labelgroupBox->raise();
        projectgroupBox->raise();

        retranslateUi(LabelDialog);

        closeButton->setDefault(false);
        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(LabelDialog);
    } // setupUi

    void retranslateUi(QDialog *LabelDialog)
    {
        LabelDialog->setWindowTitle(QCoreApplication::translate("LabelDialog", "Dialog", nullptr));
        projectgroupBox->setTitle(QCoreApplication::translate("LabelDialog", "Project List", nullptr));
        cancelButton->setText(QCoreApplication::translate("LabelDialog", "Cancel", nullptr));
        closeButton->setText(QCoreApplication::translate("LabelDialog", "Close", nullptr));
        okButton->setText(QCoreApplication::translate("LabelDialog", "OK", nullptr));
        labelgroupBox->setTitle(QCoreApplication::translate("LabelDialog", "Label List", nullptr));
        addButton->setText(QCoreApplication::translate("LabelDialog", "Add", nullptr));
        importlistButton->setText(QCoreApplication::translate("LabelDialog", "Import List", nullptr));
        importtabButton->setText(QCoreApplication::translate("LabelDialog", "Import Table", nullptr));
        showButton->setText(QCoreApplication::translate("LabelDialog", "Show", nullptr));
        removeButton->setText(QCoreApplication::translate("LabelDialog", "Remove", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LabelDialog: public Ui_LabelDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LABELDIALOG_H
