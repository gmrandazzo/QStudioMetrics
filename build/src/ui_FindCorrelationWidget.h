/********************************************************************************
** Form generated from reading UI file 'FindCorrelationWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDCORRELATIONWIDGET_H
#define UI_FINDCORRELATIONWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FindCorrelationWidget
{
public:
    QGridLayout *gridLayout_5;
    QCheckBox *directly_proportional_checkBox;
    QCheckBox *inversely_proportional_checkBox;
    QGridLayout *gridLayout;
    QLabel *label;
    QSpacerItem *horizontalSpacer;
    QDoubleSpinBox *angle_error;
    QLabel *label_2;
    QSpacerItem *horizontalSpacer_2;
    QDoubleSpinBox *distance_error;
    QFrame *line;
    QGridLayout *gridLayout_2;
    QRadioButton *allcorrelations;
    QRadioButton *correlationsto;
    QComboBox *variables;
    QRadioButton *correlations_from_list;
    QComboBox *varlist;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *findButton;
    QSpacerItem *horizontalSpacer_5;
    QFrame *line_2;
    QGridLayout *gridLayout_4;
    QPushButton *highlightButton;
    QSpacerItem *verticalSpacer_2;
    QPushButton *invertselectionButton;
    QListView *listView;
    QPushButton *selectallButton;
    QPushButton *unselectallButton;
    QPushButton *plotButton;
    QGridLayout *gridLayout_3;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *closeButton;

    void setupUi(QWidget *FindCorrelationWidget)
    {
        if (FindCorrelationWidget->objectName().isEmpty())
            FindCorrelationWidget->setObjectName(QString::fromUtf8("FindCorrelationWidget"));
        FindCorrelationWidget->resize(408, 428);
        FindCorrelationWidget->setMinimumSize(QSize(0, 0));
        gridLayout_5 = new QGridLayout(FindCorrelationWidget);
        gridLayout_5->setObjectName(QString::fromUtf8("gridLayout_5"));
        directly_proportional_checkBox = new QCheckBox(FindCorrelationWidget);
        directly_proportional_checkBox->setObjectName(QString::fromUtf8("directly_proportional_checkBox"));
        directly_proportional_checkBox->setChecked(true);

        gridLayout_5->addWidget(directly_proportional_checkBox, 0, 0, 1, 3);

        inversely_proportional_checkBox = new QCheckBox(FindCorrelationWidget);
        inversely_proportional_checkBox->setObjectName(QString::fromUtf8("inversely_proportional_checkBox"));
        inversely_proportional_checkBox->setChecked(true);

        gridLayout_5->addWidget(inversely_proportional_checkBox, 0, 3, 1, 2);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(FindCorrelationWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 1, 1, 1);

        angle_error = new QDoubleSpinBox(FindCorrelationWidget);
        angle_error->setObjectName(QString::fromUtf8("angle_error"));
        angle_error->setDecimals(3);
        angle_error->setMinimum(0.001000000000000);
        angle_error->setMaximum(99.000000000000000);
        angle_error->setSingleStep(0.500000000000000);
        angle_error->setValue(1.000000000000000);

        gridLayout->addWidget(angle_error, 0, 2, 2, 1);

        label_2 = new QLabel(FindCorrelationWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 2, 1);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 1, 1, 2, 1);

        distance_error = new QDoubleSpinBox(FindCorrelationWidget);
        distance_error->setObjectName(QString::fromUtf8("distance_error"));
        distance_error->setDecimals(3);
        distance_error->setMinimum(0.001000000000000);
        distance_error->setMaximum(100.000000000000000);
        distance_error->setSingleStep(0.050000000000000);
        distance_error->setValue(0.100000000000000);

        gridLayout->addWidget(distance_error, 2, 2, 1, 1);


        gridLayout_5->addLayout(gridLayout, 1, 0, 2, 5);

        line = new QFrame(FindCorrelationWidget);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_5->addWidget(line, 2, 1, 1, 4);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        allcorrelations = new QRadioButton(FindCorrelationWidget);
        allcorrelations->setObjectName(QString::fromUtf8("allcorrelations"));
        allcorrelations->setChecked(true);

        gridLayout_2->addWidget(allcorrelations, 0, 0, 1, 1);

        correlationsto = new QRadioButton(FindCorrelationWidget);
        correlationsto->setObjectName(QString::fromUtf8("correlationsto"));

        gridLayout_2->addWidget(correlationsto, 1, 0, 1, 1);

        variables = new QComboBox(FindCorrelationWidget);
        variables->setObjectName(QString::fromUtf8("variables"));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(variables->sizePolicy().hasHeightForWidth());
        variables->setSizePolicy(sizePolicy);
        variables->setMinimumSize(QSize(131, 23));
        variables->setMaximumSize(QSize(16777215, 16777215));
        variables->setEditable(false);

        gridLayout_2->addWidget(variables, 2, 0, 1, 1);

        correlations_from_list = new QRadioButton(FindCorrelationWidget);
        correlations_from_list->setObjectName(QString::fromUtf8("correlations_from_list"));

        gridLayout_2->addWidget(correlations_from_list, 3, 0, 1, 1);

        varlist = new QComboBox(FindCorrelationWidget);
        varlist->setObjectName(QString::fromUtf8("varlist"));
        sizePolicy.setHeightForWidth(varlist->sizePolicy().hasHeightForWidth());
        varlist->setSizePolicy(sizePolicy);
        varlist->setMinimumSize(QSize(131, 23));
        varlist->setMaximumSize(QSize(16777215, 16777215));
        varlist->setEditable(false);

        gridLayout_2->addWidget(varlist, 4, 0, 1, 1);


        gridLayout_5->addLayout(gridLayout_2, 3, 1, 1, 4);

        horizontalSpacer_4 = new QSpacerItem(147, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_4, 4, 1, 1, 1);

        findButton = new QPushButton(FindCorrelationWidget);
        findButton->setObjectName(QString::fromUtf8("findButton"));
        findButton->setMinimumSize(QSize(80, 24));
        findButton->setMaximumSize(QSize(130, 24));

        gridLayout_5->addWidget(findButton, 4, 2, 1, 2);

        horizontalSpacer_5 = new QSpacerItem(155, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_5, 4, 4, 1, 1);

        line_2 = new QFrame(FindCorrelationWidget);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_5->addWidget(line_2, 5, 1, 1, 4);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        highlightButton = new QPushButton(FindCorrelationWidget);
        highlightButton->setObjectName(QString::fromUtf8("highlightButton"));
        highlightButton->setMinimumSize(QSize(150, 24));
        highlightButton->setMaximumSize(QSize(188, 24));

        gridLayout_4->addWidget(highlightButton, 4, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(72, 38, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_4->addItem(verticalSpacer_2, 5, 1, 1, 1);

        invertselectionButton = new QPushButton(FindCorrelationWidget);
        invertselectionButton->setObjectName(QString::fromUtf8("invertselectionButton"));
        invertselectionButton->setMinimumSize(QSize(150, 24));

        gridLayout_4->addWidget(invertselectionButton, 1, 1, 1, 1);

        listView = new QListView(FindCorrelationWidget);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout_4->addWidget(listView, 0, 0, 6, 1);

        selectallButton = new QPushButton(FindCorrelationWidget);
        selectallButton->setObjectName(QString::fromUtf8("selectallButton"));
        selectallButton->setMinimumSize(QSize(150, 24));

        gridLayout_4->addWidget(selectallButton, 0, 1, 1, 1);

        unselectallButton = new QPushButton(FindCorrelationWidget);
        unselectallButton->setObjectName(QString::fromUtf8("unselectallButton"));
        unselectallButton->setMinimumSize(QSize(150, 24));

        gridLayout_4->addWidget(unselectallButton, 2, 1, 1, 1);

        plotButton = new QPushButton(FindCorrelationWidget);
        plotButton->setObjectName(QString::fromUtf8("plotButton"));
        plotButton->setMinimumSize(QSize(150, 24));
        plotButton->setMaximumSize(QSize(188, 24));

        gridLayout_4->addWidget(plotButton, 3, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 6, 1, 1, 4);

        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalSpacer_3 = new QSpacerItem(168, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_3, 0, 0, 1, 1);

        closeButton = new QPushButton(FindCorrelationWidget);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));
        closeButton->setMinimumSize(QSize(70, 24));
        closeButton->setMaximumSize(QSize(70, 24));

        gridLayout_3->addWidget(closeButton, 0, 1, 1, 1);


        gridLayout_5->addLayout(gridLayout_3, 7, 1, 1, 4);

        inversely_proportional_checkBox->raise();
        line->raise();
        line_2->raise();
        findButton->raise();
        directly_proportional_checkBox->raise();

        retranslateUi(FindCorrelationWidget);

        QMetaObject::connectSlotsByName(FindCorrelationWidget);
    } // setupUi

    void retranslateUi(QWidget *FindCorrelationWidget)
    {
        FindCorrelationWidget->setWindowTitle(QCoreApplication::translate("FindCorrelationWidget", "Find Correlations", nullptr));
        directly_proportional_checkBox->setText(QCoreApplication::translate("FindCorrelationWidget", "Directly proportional", nullptr));
        inversely_proportional_checkBox->setText(QCoreApplication::translate("FindCorrelationWidget", " inversely proportional", nullptr));
        label->setText(QCoreApplication::translate("FindCorrelationWidget", "Angle Error (rad)", nullptr));
        label_2->setText(QCoreApplication::translate("FindCorrelationWidget", "% Distance Error", nullptr));
        allcorrelations->setText(QCoreApplication::translate("FindCorrelationWidget", "Find all correlations", nullptr));
        correlationsto->setText(QCoreApplication::translate("FindCorrelationWidget", "Find correlated to", nullptr));
        correlations_from_list->setText(QCoreApplication::translate("FindCorrelationWidget", "Find correlated to variable list", nullptr));
        findButton->setText(QCoreApplication::translate("FindCorrelationWidget", "Find", nullptr));
        highlightButton->setText(QCoreApplication::translate("FindCorrelationWidget", "Highlight Selected", nullptr));
        invertselectionButton->setText(QCoreApplication::translate("FindCorrelationWidget", "Invert Selection", nullptr));
        selectallButton->setText(QCoreApplication::translate("FindCorrelationWidget", "Select All", nullptr));
        unselectallButton->setText(QCoreApplication::translate("FindCorrelationWidget", "Unselect All", nullptr));
        plotButton->setText(QCoreApplication::translate("FindCorrelationWidget", "Plot Selected", nullptr));
        closeButton->setText(QCoreApplication::translate("FindCorrelationWidget", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindCorrelationWidget: public Ui_FindCorrelationWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDCORRELATIONWIDGET_H
