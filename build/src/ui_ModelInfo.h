/********************************************************************************
** Form generated from reading UI file 'ModelInfo.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODELINFO_H
#define UI_MODELINFO_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ModelInfo
{
public:
    QGridLayout *gridLayout;
    QTextEdit *textEdit;

    void setupUi(QWidget *ModelInfo)
    {
        if (ModelInfo->objectName().isEmpty())
            ModelInfo->setObjectName(QString::fromUtf8("ModelInfo"));
        ModelInfo->resize(480, 310);
        ModelInfo->setMinimumSize(QSize(480, 310));
        ModelInfo->setMaximumSize(QSize(16777215, 16777215));
        gridLayout = new QGridLayout(ModelInfo);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        textEdit = new QTextEdit(ModelInfo);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        textEdit->setReadOnly(true);

        gridLayout->addWidget(textEdit, 0, 0, 1, 1);


        retranslateUi(ModelInfo);

        QMetaObject::connectSlotsByName(ModelInfo);
    } // setupUi

    void retranslateUi(QWidget *ModelInfo)
    {
        ModelInfo->setWindowTitle(QCoreApplication::translate("ModelInfo", "Model Info", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModelInfo: public Ui_ModelInfo {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODELINFO_H
