/********************************************************************************
** Form generated from reading UI file 'ObjectSelectorDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OBJECTSELECTORDIALOG_H
#define UI_OBJECTSELECTORDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>

QT_BEGIN_NAMESPACE

class Ui_ObjectSelectorDialog
{
public:
    QGridLayout *gridLayout_7;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_3;
    QRadioButton *mdcButton;
    QRadioButton *maxdissButton;
    QRadioButton *randomButton;
    QLabel *label;
    QSpinBox *nobjectsBox;
    QFrame *line;
    QGridLayout *gridLayout_2;
    QRadioButton *selectbyobjlabelbutton;
    QComboBox *objlabelslst;
    QGroupBox *dataGroupBox;
    QGridLayout *gridLayout_5;
    QRadioButton *sourcedataButton;
    QListView *listView_2;
    QRadioButton *plotcoordinatesButton;
    QGroupBox *varspaceGroupBox;
    QGridLayout *gridLayout_6;
    QComboBox *metricspacelist;
    QSpacerItem *horizontalSpacer_2;
    QFrame *varlistframe;
    QGridLayout *gridLayout;
    QListView *listView;
    QPushButton *selectAllButton;
    QPushButton *invertSelectionButton;
    QComboBox *selectByVarLabel;
    QPushButton *unselectButton;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout_4;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *cancelButton;
    QPushButton *okButton;

    void setupUi(QDialog *ObjectSelectorDialog)
    {
        if (ObjectSelectorDialog->objectName().isEmpty())
            ObjectSelectorDialog->setObjectName(QString::fromUtf8("ObjectSelectorDialog"));
        ObjectSelectorDialog->resize(920, 318);
        gridLayout_7 = new QGridLayout(ObjectSelectorDialog);
        gridLayout_7->setObjectName(QString::fromUtf8("gridLayout_7"));
        groupBox = new QGroupBox(ObjectSelectorDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_3 = new QGridLayout(groupBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        mdcButton = new QRadioButton(groupBox);
        mdcButton->setObjectName(QString::fromUtf8("mdcButton"));
        mdcButton->setChecked(true);

        gridLayout_3->addWidget(mdcButton, 0, 0, 1, 2);

        maxdissButton = new QRadioButton(groupBox);
        maxdissButton->setObjectName(QString::fromUtf8("maxdissButton"));

        gridLayout_3->addWidget(maxdissButton, 1, 0, 1, 2);

        randomButton = new QRadioButton(groupBox);
        randomButton->setObjectName(QString::fromUtf8("randomButton"));

        gridLayout_3->addWidget(randomButton, 2, 0, 1, 1);

        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout_3->addWidget(label, 3, 0, 1, 1);

        nobjectsBox = new QSpinBox(groupBox);
        nobjectsBox->setObjectName(QString::fromUtf8("nobjectsBox"));
        nobjectsBox->setMinimum(1);
        nobjectsBox->setMaximum(999999999);
        nobjectsBox->setValue(1);

        gridLayout_3->addWidget(nobjectsBox, 3, 1, 1, 1);

        line = new QFrame(groupBox);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_3->addWidget(line, 4, 0, 1, 2);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        selectbyobjlabelbutton = new QRadioButton(groupBox);
        selectbyobjlabelbutton->setObjectName(QString::fromUtf8("selectbyobjlabelbutton"));

        gridLayout_2->addWidget(selectbyobjlabelbutton, 0, 0, 1, 1);

        objlabelslst = new QComboBox(groupBox);
        objlabelslst->setObjectName(QString::fromUtf8("objlabelslst"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(objlabelslst->sizePolicy().hasHeightForWidth());
        objlabelslst->setSizePolicy(sizePolicy);
        objlabelslst->setMinimumSize(QSize(130, 24));
        objlabelslst->setMaximumSize(QSize(16777215, 16777215));
        objlabelslst->setEditable(false);

        gridLayout_2->addWidget(objlabelslst, 0, 1, 1, 1);


        gridLayout_3->addLayout(gridLayout_2, 5, 0, 1, 2);


        gridLayout_7->addWidget(groupBox, 0, 0, 1, 1);

        dataGroupBox = new QGroupBox(ObjectSelectorDialog);
        dataGroupBox->setObjectName(QString::fromUtf8("dataGroupBox"));
        gridLayout_5 = new QGridLayout(dataGroupBox);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        sourcedataButton = new QRadioButton(dataGroupBox);
        sourcedataButton->setObjectName(QString::fromUtf8("sourcedataButton"));

        gridLayout_5->addWidget(sourcedataButton, 1, 0, 1, 1);

        listView_2 = new QListView(dataGroupBox);
        listView_2->setObjectName(QString::fromUtf8("listView_2"));
        listView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView_2->setSelectionMode(QAbstractItemView::SingleSelection);
        listView_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        gridLayout_5->addWidget(listView_2, 2, 0, 1, 1);

        plotcoordinatesButton = new QRadioButton(dataGroupBox);
        plotcoordinatesButton->setObjectName(QString::fromUtf8("plotcoordinatesButton"));
        plotcoordinatesButton->setChecked(true);

        gridLayout_5->addWidget(plotcoordinatesButton, 0, 0, 1, 1);


        gridLayout_7->addWidget(dataGroupBox, 0, 1, 2, 1);

        varspaceGroupBox = new QGroupBox(ObjectSelectorDialog);
        varspaceGroupBox->setObjectName(QString::fromUtf8("varspaceGroupBox"));
        gridLayout_6 = new QGridLayout(varspaceGroupBox);
        gridLayout_6->setObjectName(QString::fromUtf8("gridLayout_6"));
        metricspacelist = new QComboBox(varspaceGroupBox);
        metricspacelist->addItem(QString());
        metricspacelist->addItem(QString());
        metricspacelist->addItem(QString());
        metricspacelist->setObjectName(QString::fromUtf8("metricspacelist"));
        sizePolicy.setHeightForWidth(metricspacelist->sizePolicy().hasHeightForWidth());
        metricspacelist->setSizePolicy(sizePolicy);
        metricspacelist->setMinimumSize(QSize(0, 0));
        metricspacelist->setMaximumSize(QSize(16777215, 16777215));
        metricspacelist->setEditable(false);

        gridLayout_6->addWidget(metricspacelist, 0, 0, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_6->addItem(horizontalSpacer_2, 0, 1, 1, 1);

        varlistframe = new QFrame(varspaceGroupBox);
        varlistframe->setObjectName(QString::fromUtf8("varlistframe"));
        varlistframe->setFrameShape(QFrame::StyledPanel);
        varlistframe->setFrameShadow(QFrame::Raised);
        gridLayout = new QGridLayout(varlistframe);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listView = new QListView(varlistframe);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setMinimumSize(QSize(0, 0));
        listView->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout->addWidget(listView, 0, 0, 1, 2);

        selectAllButton = new QPushButton(varlistframe);
        selectAllButton->setObjectName(QString::fromUtf8("selectAllButton"));
        selectAllButton->setMinimumSize(QSize(130, 24));
        selectAllButton->setMaximumSize(QSize(130, 24));

        gridLayout->addWidget(selectAllButton, 1, 0, 1, 1);

        invertSelectionButton = new QPushButton(varlistframe);
        invertSelectionButton->setObjectName(QString::fromUtf8("invertSelectionButton"));
        invertSelectionButton->setMinimumSize(QSize(130, 24));
        invertSelectionButton->setMaximumSize(QSize(130, 24));

        gridLayout->addWidget(invertSelectionButton, 1, 1, 1, 1);

        selectByVarLabel = new QComboBox(varlistframe);
        selectByVarLabel->addItem(QString());
        selectByVarLabel->setObjectName(QString::fromUtf8("selectByVarLabel"));
        sizePolicy.setHeightForWidth(selectByVarLabel->sizePolicy().hasHeightForWidth());
        selectByVarLabel->setSizePolicy(sizePolicy);
        selectByVarLabel->setMinimumSize(QSize(130, 24));
        selectByVarLabel->setMaximumSize(QSize(130, 24));
        selectByVarLabel->setEditable(false);

        gridLayout->addWidget(selectByVarLabel, 2, 0, 1, 1);

        unselectButton = new QPushButton(varlistframe);
        unselectButton->setObjectName(QString::fromUtf8("unselectButton"));
        unselectButton->setMinimumSize(QSize(130, 24));
        unselectButton->setMaximumSize(QSize(130, 24));

        gridLayout->addWidget(unselectButton, 2, 1, 1, 1);


        gridLayout_6->addWidget(varlistframe, 1, 0, 1, 2);


        gridLayout_7->addWidget(varspaceGroupBox, 0, 2, 2, 1);

        verticalSpacer = new QSpacerItem(20, 163, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer, 1, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        horizontalSpacer_3 = new QSpacerItem(86, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_4->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        cancelButton = new QPushButton(ObjectSelectorDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 24));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout_4->addWidget(cancelButton, 0, 1, 1, 1);

        okButton = new QPushButton(ObjectSelectorDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout_4->addWidget(okButton, 0, 2, 1, 1);


        gridLayout_7->addLayout(gridLayout_4, 2, 0, 1, 3);


        retranslateUi(ObjectSelectorDialog);

        QMetaObject::connectSlotsByName(ObjectSelectorDialog);
    } // setupUi

    void retranslateUi(QDialog *ObjectSelectorDialog)
    {
        ObjectSelectorDialog->setWindowTitle(QCoreApplication::translate("ObjectSelectorDialog", "Select Objects", nullptr));
        groupBox->setTitle(QCoreApplication::translate("ObjectSelectorDialog", "Object Selection Type", nullptr));
        mdcButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Most Descriptive Compounds", nullptr));
        maxdissButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Maximum Dissimilarity Compounds", nullptr));
        randomButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Random", nullptr));
        label->setText(QCoreApplication::translate("ObjectSelectorDialog", "Number of Objects", nullptr));
        selectbyobjlabelbutton->setText(QCoreApplication::translate("ObjectSelectorDialog", " Select by object label", nullptr));
        dataGroupBox->setTitle(QCoreApplication::translate("ObjectSelectorDialog", "Data", nullptr));
        sourcedataButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Source Data", nullptr));
        plotcoordinatesButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Plot Coordinates", nullptr));
        varspaceGroupBox->setTitle(QCoreApplication::translate("ObjectSelectorDialog", "Variable Space", nullptr));
        metricspacelist->setItemText(0, QCoreApplication::translate("ObjectSelectorDialog", "Euclidean distance", nullptr));
        metricspacelist->setItemText(1, QCoreApplication::translate("ObjectSelectorDialog", "Manhattan distance", nullptr));
        metricspacelist->setItemText(2, QCoreApplication::translate("ObjectSelectorDialog", "Cosine similarity", nullptr));

        selectAllButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Select All", nullptr));
        invertSelectionButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Invert Selection", nullptr));
        selectByVarLabel->setItemText(0, QCoreApplication::translate("ObjectSelectorDialog", "Select by Label", nullptr));

        unselectButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Unselect", nullptr));
        cancelButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ObjectSelectorDialog", "OK", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ObjectSelectorDialog: public Ui_ObjectSelectorDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OBJECTSELECTORDIALOG_H
