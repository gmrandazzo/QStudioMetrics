/********************************************************************************
** Form generated from reading UI file 'SaveDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SAVEDIALOG_H
#define UI_SAVEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_SaveDialog
{
public:
    QGridLayout *gridLayout_4;
    QGridLayout *gridLayout_3;
    QLabel *label;
    QListView *listView;
    QFrame *line;
    QGridLayout *gridLayout_2;
    QLineEdit *savein;
    QPushButton *saveinButton;
    QGridLayout *gridLayout;
    QPushButton *okButton;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;

    void setupUi(QDialog *SaveDialog)
    {
        if (SaveDialog->objectName().isEmpty())
            SaveDialog->setObjectName(QString::fromUtf8("SaveDialog"));
        SaveDialog->resize(420, 300);
        SaveDialog->setMinimumSize(QSize(420, 300));
        gridLayout_4 = new QGridLayout(SaveDialog);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label = new QLabel(SaveDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(0, 0));
        label->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_3->addWidget(label, 0, 0, 1, 1);

        listView = new QListView(SaveDialog);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setMinimumSize(QSize(0, 0));
        listView->setMaximumSize(QSize(16777215, 16777215));

        gridLayout_3->addWidget(listView, 1, 0, 1, 1);


        gridLayout_4->addLayout(gridLayout_3, 0, 0, 1, 1);

        line = new QFrame(SaveDialog);
        line->setObjectName(QString::fromUtf8("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line, 1, 0, 1, 1);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        savein = new QLineEdit(SaveDialog);
        savein->setObjectName(QString::fromUtf8("savein"));
        savein->setMinimumSize(QSize(245, 22));

        gridLayout_2->addWidget(savein, 0, 0, 1, 1);

        saveinButton = new QPushButton(SaveDialog);
        saveinButton->setObjectName(QString::fromUtf8("saveinButton"));
        saveinButton->setMinimumSize(QSize(100, 24));

        gridLayout_2->addWidget(saveinButton, 0, 1, 1, 1);


        gridLayout_4->addLayout(gridLayout_2, 2, 0, 1, 1);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        okButton = new QPushButton(SaveDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setMinimumSize(QSize(100, 24));
        okButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(okButton, 0, 3, 1, 1);

        horizontalSpacer = new QSpacerItem(118, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 0, 1, 1);

        cancelButton = new QPushButton(SaveDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setMinimumSize(QSize(100, 23));
        cancelButton->setMaximumSize(QSize(100, 24));

        gridLayout->addWidget(cancelButton, 0, 2, 1, 1);


        gridLayout_4->addLayout(gridLayout, 3, 0, 1, 1);


        retranslateUi(SaveDialog);

        QMetaObject::connectSlotsByName(SaveDialog);
    } // setupUi

    void retranslateUi(QDialog *SaveDialog)
    {
        SaveDialog->setWindowTitle(QCoreApplication::translate("SaveDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("SaveDialog", "Select Project...", nullptr));
        saveinButton->setText(QCoreApplication::translate("SaveDialog", "Save in...", nullptr));
        okButton->setText(QCoreApplication::translate("SaveDialog", "OK", nullptr));
        cancelButton->setText(QCoreApplication::translate("SaveDialog", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SaveDialog: public Ui_SaveDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SAVEDIALOG_H
