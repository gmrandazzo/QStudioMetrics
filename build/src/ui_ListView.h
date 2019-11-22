/********************************************************************************
** Form generated from reading UI file 'ListView.ui'
**
** Created by: Qt User Interface Compiler version 5.13.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LISTVIEW_H
#define UI_LISTVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QListView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ListView
{
public:
    QGridLayout *gridLayout;
    QListView *listView;

    void setupUi(QWidget *ListView)
    {
        if (ListView->objectName().isEmpty())
            ListView->setObjectName(QString::fromUtf8("ListView"));
        ListView->resize(301, 392);
        gridLayout = new QGridLayout(ListView);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        listView = new QListView(ListView);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        listView->setAlternatingRowColors(true);
        listView->setSelectionMode(QAbstractItemView::MultiSelection);

        gridLayout->addWidget(listView, 0, 0, 1, 1);


        retranslateUi(ListView);

        QMetaObject::connectSlotsByName(ListView);
    } // setupUi

    void retranslateUi(QWidget *ListView)
    {
        ListView->setWindowTitle(QCoreApplication::translate("ListView", "List View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ListView: public Ui_ListView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LISTVIEW_H
