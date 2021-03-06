/****************************************************************************
** Meta object code from reading C++ file 'ClassModelDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/ClassModelDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ClassModelDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ClassModelDialog_t {
    QByteArrayData data[20];
    char stringdata0[235];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ClassModelDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ClassModelDialog_t qt_meta_stringdata_ClassModelDialog = {
    {
QT_MOC_LITERAL(0, 0, 16), // "ClassModelDialog"
QT_MOC_LITERAL(1, 17, 4), // "next"
QT_MOC_LITERAL(2, 22, 0), // ""
QT_MOC_LITERAL(3, 23, 8), // "previous"
QT_MOC_LITERAL(4, 32, 2), // "OK"
QT_MOC_LITERAL(5, 35, 11), // "genListView"
QT_MOC_LITERAL(6, 47, 11), // "QModelIndex"
QT_MOC_LITERAL(7, 59, 7), // "current"
QT_MOC_LITERAL(8, 67, 7), // "setData"
QT_MOC_LITERAL(9, 75, 20), // "EnableDisableButtons"
QT_MOC_LITERAL(10, 96, 8), // "AddClass"
QT_MOC_LITERAL(11, 105, 11), // "RemoveClass"
QT_MOC_LITERAL(12, 117, 12), // "ObjSelectAll"
QT_MOC_LITERAL(13, 130, 18), // "ObjInvertSelection"
QT_MOC_LITERAL(14, 149, 14), // "ObjUnselectAll"
QT_MOC_LITERAL(15, 164, 11), // "ObjSelectBy"
QT_MOC_LITERAL(16, 176, 12), // "VarSelectAll"
QT_MOC_LITERAL(17, 189, 18), // "VarInvertSelection"
QT_MOC_LITERAL(18, 208, 14), // "VarUnselectAll"
QT_MOC_LITERAL(19, 223, 11) // "VarSelectBy"

    },
    "ClassModelDialog\0next\0\0previous\0OK\0"
    "genListView\0QModelIndex\0current\0setData\0"
    "EnableDisableButtons\0AddClass\0RemoveClass\0"
    "ObjSelectAll\0ObjInvertSelection\0"
    "ObjUnselectAll\0ObjSelectBy\0VarSelectAll\0"
    "VarInvertSelection\0VarUnselectAll\0"
    "VarSelectBy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ClassModelDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   94,    2, 0x08 /* Private */,
       3,    0,   95,    2, 0x08 /* Private */,
       4,    0,   96,    2, 0x08 /* Private */,
       5,    1,   97,    2, 0x08 /* Private */,
       8,    1,  100,    2, 0x08 /* Private */,
       9,    0,  103,    2, 0x08 /* Private */,
      10,    0,  104,    2, 0x08 /* Private */,
      11,    0,  105,    2, 0x08 /* Private */,
      12,    0,  106,    2, 0x08 /* Private */,
      13,    0,  107,    2, 0x08 /* Private */,
      14,    0,  108,    2, 0x08 /* Private */,
      15,    0,  109,    2, 0x08 /* Private */,
      16,    0,  110,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,
      18,    0,  112,    2, 0x08 /* Private */,
      19,    0,  113,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
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

void ClassModelDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ClassModelDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->next(); break;
        case 1: _t->previous(); break;
        case 2: _t->OK(); break;
        case 3: _t->genListView((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 4: _t->setData((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 5: _t->EnableDisableButtons(); break;
        case 6: _t->AddClass(); break;
        case 7: _t->RemoveClass(); break;
        case 8: _t->ObjSelectAll(); break;
        case 9: _t->ObjInvertSelection(); break;
        case 10: _t->ObjUnselectAll(); break;
        case 11: _t->ObjSelectBy(); break;
        case 12: _t->VarSelectAll(); break;
        case 13: _t->VarInvertSelection(); break;
        case 14: _t->VarUnselectAll(); break;
        case 15: _t->VarSelectBy(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ClassModelDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ClassModelDialog.data,
    qt_meta_data_ClassModelDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ClassModelDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ClassModelDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ClassModelDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ClassModelDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
