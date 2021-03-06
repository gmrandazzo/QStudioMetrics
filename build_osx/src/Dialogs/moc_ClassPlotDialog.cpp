/****************************************************************************
** Meta object code from reading C++ file 'ClassPlotDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/ClassPlotDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClassPlotDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ClassPlotDialog_t {
    QByteArrayData data[14];
    char stringdata0[143];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClassPlotDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClassPlotDialog_t qt_meta_stringdata_ClassPlotDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ClassPlotDialog"
QT_MOC_LITERAL(1, 16, 10), // "actionPlot"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 10), // "setProject"
QT_MOC_LITERAL(4, 39, 11), // "QModelIndex"
QT_MOC_LITERAL(5, 51, 10), // "setModelID"
QT_MOC_LITERAL(6, 62, 4), // "Find"
QT_MOC_LITERAL(7, 67, 7), // "FindAll"
QT_MOC_LITERAL(8, 75, 8), // "AddGroup"
QT_MOC_LITERAL(9, 84, 11), // "RemoveGroup"
QT_MOC_LITERAL(10, 96, 9), // "SelectAll"
QT_MOC_LITERAL(11, 106, 15), // "InvertSelection"
QT_MOC_LITERAL(12, 122, 8), // "SelectBy"
QT_MOC_LITERAL(13, 131, 11) // "UnselectAll"

    },
    "ClassPlotDialog\0actionPlot\0\0setProject\0"
    "QModelIndex\0setModelID\0Find\0FindAll\0"
    "AddGroup\0RemoveGroup\0SelectAll\0"
    "InvertSelection\0SelectBy\0UnselectAll"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClassPlotDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   69,    2, 0x08 /* Private */,
       3,    1,   70,    2, 0x08 /* Private */,
       5,    1,   73,    2, 0x08 /* Private */,
       6,    0,   76,    2, 0x08 /* Private */,
       7,    0,   77,    2, 0x08 /* Private */,
       8,    0,   78,    2, 0x08 /* Private */,
       9,    0,   79,    2, 0x08 /* Private */,
      10,    0,   80,    2, 0x08 /* Private */,
      11,    0,   81,    2, 0x08 /* Private */,
      12,    0,   82,    2, 0x08 /* Private */,
      13,    0,   83,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void, 0x80000000 | 4,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ClassPlotDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClassPlotDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->actionPlot(); break;
        case 1: _t->setProject((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->setModelID((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 3: _t->Find(); break;
        case 4: _t->FindAll(); break;
        case 5: _t->AddGroup(); break;
        case 6: _t->RemoveGroup(); break;
        case 7: _t->SelectAll(); break;
        case 8: _t->InvertSelection(); break;
        case 9: _t->SelectBy(); break;
        case 10: _t->UnselectAll(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ClassPlotDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ClassPlotDialog.data,
    qt_meta_data_ClassPlotDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ClassPlotDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClassPlotDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClassPlotDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ClassPlotDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 11;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
