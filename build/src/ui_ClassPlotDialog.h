/********************************************************************************
** Form generated from reading UI file 'ClassPlotDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLASSPLOTDIALOG_H
#define UI_CLASSPLOTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_ClassPlotDialog
{
public:
    QGridLayout *gridLayout_6;
    QGroupBox *projectsGroupBox;
    QGridLayout *gridLayout_4;
    QListView *listView;
    QGroupBox *modelsGroupBox;
    QGridLayout *gridLayout_2;
    QListView *listView_2;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_5;
    QGridLayout *gridLayout_3;
    QLineEdit *lineEdit;
    QPushButton *findButton;
    QPushButton *findAllButton;
    QSpacerItem *verticalSpacer;
    QListView *listView_4;
    QListView *listView_3;
    QPushButton *addgroupButton;
    QPushButton *removegroupButton;
    QSpacerItem *verticalSpacer_2;
    QPushButton *selectButton;
    QPushButton *invertselectionButton;
    QComboBox *selbycomboBox;
    QPushButton *unselectButton;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *plotButton;

    void setupUi(QDialog *ClassPlotDialog)
    {
        if (ClassPlotDialog->objectName().isEmpty())
            ClassPlotDialog->setObjectName(QString::fromUtf8("ClassPlotDialog"));
        ClassPlotDialog->resize(619, 472);
        gridLayout_6 = new QGridLayout(ClassPlotDialog);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        projectsGroupBox = new QGroupBox(ClassPlotDialog);
        projectsGroupBox->setObjectName(QString::fromUtf8("projectsGroupBox"));
        gridLayout_4 = new QGridLayout(projectsGroupBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        listView = new QListView(projectsGroupBox);
        listView->setObjectName(QString::fromUtf8("listView"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
        listView->setSizePolicy(sizePolicy);
        listView->setMinimumSize(QSize(0, 0));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setSelectionMode(QAbstractItemView::SingleSelection);
        listView->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_4->addWidget(listView, 0, 0, 1, 1);


        gridLayout_6->addWidget(projectsGroupBox, 0, 0, 1, 1);

        modelsGroupBox = new QGroupBox(ClassPlotDialog);
        modelsGroupBox->setObjectName(QString::fromUtf8("modelsGroupBox"));
        gridLayout_2 = new QGridLayout(modelsGroupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        listView_2 = new QListView(modelsGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setMinimumSize(QSize(0, 0));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_2->addWidget(listView_2, 0, 0, 1, 1);


        gridLayout_6->addWidget(modelsGroupBox, 0, 1, 1, 1);

        groupBox = new QGroupBox(ClassPlotDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_5 = new QGridLayout(groupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

        gridLayout_3->addWidget(lineEdit, 0, 0, 1, 1);

        findButton = new QPushButton(groupBox);
        findButton->setObjectName(QString::fromUtf8("findButton"));
        findButton->setMinimumSize(QSize(60, 24));
        findButton->setMaximumSize(QSize(60, 24));

        gridLayout_3->addWidget(findButton, 0, 1, 1, 1);

        findAllButton = new QPushButton(groupBox);
        findAllButton->setObjectName(QString::fromUtf8("findAllButton"));
        findAllButton->setMinimumSize(QSize(70, 24));
        findAllButton->setMaximumSize(QSize(70, 24));

        gridLayout_3->addWidget(findAllButton, 0, 2, 1, 1);


        gridLayout_5->addLayout(gridLayout_3, 0, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 53, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer, 0, 3, 2, 1);

        listView_4 = new QListView(groupBox);
        listView_4->setObjectName(QString::fromUtf8("listView_4"));
        listView_4->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_4->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_5->addWidget(listView_4, 0, 4, 7, 1);

        listView_3 = new QListView(groupBox);
        listView_3->setObjectName(QString::fromUtf8("listView_3"));
        listView_3->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_3->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_5->addWidget(listView_3, 1, 0, 4, 2);

        addgroupButton = new QPushButton(groupBox);
        addgroupButton->setObjectName(QString::fromUtf8("addgroupButton"));
        addgroupButton->setMinimumSize(QSize(130, 40));
        addgroupButton->setMaximumSize(QSize(130, 40));

        gridLayout_5->addWidget(addgroupButton, 2, 2, 1, 2);

        removegroupButton = new QPushButton(groupBox);
        removegroupButton->setObjectName(QString::fromUtf8("removegroupButton"));
        removegroupButton->setMinimumSize(QSize(130, 40));
        removegroupButton->setMaximumSize(QSize(130, 40));

        gridLayout_5->addWidget(removegroupButton, 3, 2, 1, 2);

        verticalSpacer_2 = new QSpacerItem(20, 81, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_5->addItem(verticalSpacer_2, 4, 2, 3, 1);

        selectButton = new QPushButton(groupBox);
        selectButton->setObjectName(QString::fromUtf8("selectButton"));
        selectButton->setMinimumSize(QSize(130, 24));

        gridLayout_5->addWidget(selectButton, 5, 0, 1, 1);

        invertselectionButton = new QPushButton(groupBox);
        invertselectionButton->setObjectName(QString::fromUtf8("invertselectionButton"));
        invertselectionButton->setMinimumSize(QSize(130, 24));
        invertselectionButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(invertselectionButton, 5, 1, 1, 1);

        selbycomboBox = new QComboBox(groupBox);
        selbycomboBox->addItem(QString());
        selbycomboBox->setObjectName(QString::fromUtf8("selbycomboBox"));
        selbycomboBox->setMinimumSize(QSize(130, 24));
        selbycomboBox->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(selbycomboBox, 6, 0, 1, 1);

        unselectButton = new QPushButton(groupBox);
        unselectButton->setObjectName(QString::fromUtf8("unselectButton"));
        unselectButton->setMinimumSize(QSize(130, 24));
        unselectButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(unselectButton, 6, 1, 1, 1);


        gridLayout_6->addWidget(groupBox, 1, 0, 1, 2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(ClassPlotDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(cancelButton, 0, 1, 1, 1);

        plotButton = new QPushButton(ClassPlotDialog);
        plotButton->setObjectName(QString::fromUtf8("plotButton"));
        plotButton->setMinimumSize(QSize(100, 24));
        plotButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(plotButton, 0, 2, 1, 1);


        gridLayout_6->addLayout(gridLayout, 2, 0, 1, 2);

        projectsGroupBox->raise();
        modelsGroupBox->raise();
        groupBox->raise();

        retranslateUi(ClassPlotDialog);

        QMetaObject::connectSlotsByName(ClassPlotDialog);
    } // setupUi

    void retranslateUi(QDialog *ClassPlotDialog)
    {
        ClassPlotDialog->setWindowTitle(QCoreApplication::translate("ClassPlotDialog", "Plot Dialog", nullptr));
        projectsGroupBox->setTitle(QCoreApplication::translate("ClassPlotDialog", "Projects", nullptr));
        modelsGroupBox->setTitle(QCoreApplication::translate("ClassPlotDialog", "Models", nullptr));
        groupBox->setTitle(QString());
        findButton->setText(QCoreApplication::translate("ClassPlotDialog", "Find", nullptr));
        findAllButton->setText(QCoreApplication::translate("ClassPlotDialog", "Find All", nullptr));
        addgroupButton->setText(QCoreApplication::translate("ClassPlotDialog", "Add Group", nullptr));
        removegroupButton->setText(QCoreApplication::translate("ClassPlotDialog", "Remove Group", nullptr));
        selectButton->setText(QCoreApplication::translate("ClassPlotDialog", "Select All", nullptr));
        invertselectionButton->setText(QCoreApplication::translate("ClassPlotDialog", "Invert Selection", nullptr));
        selbycomboBox->setItemText(0, QCoreApplication::translate("ClassPlotDialog", "Select By...", nullptr));

        unselectButton->setText(QCoreApplication::translate("ClassPlotDialog", "Unselect", nullptr));
        cancelButton->setText(QCoreApplication::translate("ClassPlotDialog", "Cancel", nullptr));
        plotButton->setText(QCoreApplication::translate("ClassPlotDialog", "Plot", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ClassPlotDialog: public Ui_ClassPlotDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLASSPLOTDIALOG_H
