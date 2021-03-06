/********************************************************************************
** Form generated from reading UI file 'AboutDialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTDIALOG_H
#define UI_ABOUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_AboutDialog
{
public:
    QGridLayout *gridLayout;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_version;
    QLabel *label_4;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *closeButton;

    void setupUi(QDialog *AboutDialog)
    {
        if (AboutDialog->objectName().isEmpty())
            AboutDialog->setObjectName(QString::fromUtf8("AboutDialog"));
        AboutDialog->resize(596, 640);
        gridLayout = new QGridLayout(AboutDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label = new QLabel(AboutDialog);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_2 = new QLabel(AboutDialog);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_version = new QLabel(AboutDialog);
        label_version->setObjectName(QString::fromUtf8("label_version"));
        QFont font;
        font.setPointSize(12);
        label_version->setFont(font);

        gridLayout->addWidget(label_version, 2, 0, 1, 1);

        label_4 = new QLabel(AboutDialog);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setOpenExternalLinks(true);

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label_3 = new QLabel(AboutDialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setOpenExternalLinks(true);

        gridLayout->addWidget(label_3, 4, 0, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(328, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        closeButton = new QPushButton(AboutDialog);
        closeButton->setObjectName(QString::fromUtf8("closeButton"));

        horizontalLayout->addWidget(closeButton);


        gridLayout->addLayout(horizontalLayout, 5, 0, 1, 1);


        retranslateUi(AboutDialog);

        QMetaObject::connectSlotsByName(AboutDialog);
    } // setupUi

    void retranslateUi(QDialog *AboutDialog)
    {
        AboutDialog->setWindowTitle(QCoreApplication::translate("AboutDialog", "Dialog", nullptr));
        label->setText(QCoreApplication::translate("AboutDialog", "<html><head/><body><p><img src=\":/qsmlogo.png\"/></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("AboutDialog", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:600;\">QStudiometrics</span></p></body></html>", nullptr));
        label_version->setText(QCoreApplication::translate("AboutDialog", "Version:", nullptr));
        label_4->setText(QCoreApplication::translate("AboutDialog", "<html><head/><body><p>Plots are realized using <a href=\"https://plotly.com/javascript/\">plotly.js</a> and a special C++ wrapper written by Giuseppe Marco Randazzo</p></body></html>", nullptr));
        label_3->setText(QCoreApplication::translate("AboutDialog", "<html><head/><body><p><span style=\" font-size:14pt;\">QStudiometrics was written by </span><a href=\"mailto:gmrandazzo@gmail.com\"><span style=\" font-size:14pt; text-decoration: underline; color:#0057ae;\">Giuseppe Marco Randazzo </span></a></p></body></html>", nullptr));
        closeButton->setText(QCoreApplication::translate("AboutDialog", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AboutDialog: public Ui_AboutDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTDIALOG_H
