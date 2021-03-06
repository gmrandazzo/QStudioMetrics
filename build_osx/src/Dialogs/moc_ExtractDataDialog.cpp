/****************************************************************************
** Meta object code from reading C++ file 'ExtractDataDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/ExtractDataDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ExtractDataDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ExtractDataDialog_t {
    QByteArrayData data[18];
    char stringdata0[291];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ExtractDataDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ExtractDataDialog_t qt_meta_stringdata_ExtractDataDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ExtractDataDialog"
QT_MOC_LITERAL(1, 18, 16), // "ObjectsSelectAll"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 22), // "ObjectsInvertSelection"
QT_MOC_LITERAL(4, 59, 15), // "ObjectsSelectBy"
QT_MOC_LITERAL(5, 75, 15), // "ObjectsUnselect"
QT_MOC_LITERAL(6, 91, 18), // "VariablesSelectAll"
QT_MOC_LITERAL(7, 110, 24), // "VariablesInvertSelection"
QT_MOC_LITERAL(8, 135, 17), // "VariablesSelectBy"
QT_MOC_LITERAL(9, 153, 17), // "VariablesUnselect"
QT_MOC_LITERAL(10, 171, 27), // "UpdateSelectedObjectCounter"
QT_MOC_LITERAL(11, 199, 29), // "UpdateSelectedVariableCounter"
QT_MOC_LITERAL(12, 229, 2), // "OK"
QT_MOC_LITERAL(13, 232, 11), // "genListView"
QT_MOC_LITERAL(14, 244, 12), // "setProjectID"
QT_MOC_LITERAL(15, 257, 11), // "QModelIndex"
QT_MOC_LITERAL(16, 269, 7), // "current"
QT_MOC_LITERAL(17, 277, 13) // "genObjVarView"

    },
    "ExtractDataDialog\0ObjectsSelectAll\0\0"
    "ObjectsInvertSelection\0ObjectsSelectBy\0"
    "ObjectsUnselect\0VariablesSelectAll\0"
    "VariablesInvertSelection\0VariablesSelectBy\0"
    "VariablesUnselect\0UpdateSelectedObjectCounter\0"
    "UpdateSelectedVariableCounter\0OK\0"
    "genListView\0setProjectID\0QModelIndex\0"
    "current\0genObjVarView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ExtractDataDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x08 /* Private */,
       3,    0,   85,    2, 0x08 /* Private */,
       4,    0,   86,    2, 0x08 /* Private */,
       5,    0,   87,    2, 0x08 /* Private */,
       6,    0,   88,    2, 0x08 /* Private */,
       7,    0,   89,    2, 0x08 /* Private */,
       8,    0,   90,    2, 0x08 /* Private */,
       9,    0,   91,    2, 0x08 /* Private */,
      10,    0,   92,    2, 0x08 /* Private */,
      11,    0,   93,    2, 0x08 /* Private */,
      12,    0,   94,    2, 0x08 /* Private */,
      13,    0,   95,    2, 0x08 /* Private */,
      14,    1,   96,    2, 0x08 /* Private */,
      17,    1,   99,    2, 0x08 /* Private */,

 // slots: parameters
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
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 15,   16,

       0        // eod
};

void ExtractDataDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ExtractDataDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ObjectsSelectAll(); break;
        case 1: _t->ObjectsInvertSelection(); break;
        case 2: _t->ObjectsSelectBy(); break;
        case 3: _t->ObjectsUnselect(); break;
        case 4: _t->VariablesSelectAll(); break;
        case 5: _t->VariablesInvertSelection(); break;
        case 6: _t->VariablesSelectBy(); break;
        case 7: _t->VariablesUnselect(); break;
        case 8: _t->UpdateSelectedObjectCounter(); break;
        case 9: _t->UpdateSelectedVariableCounter(); break;
        case 10: _t->OK(); break;
        case 11: _t->genListView(); break;
        case 12: _t->setProjectID((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 13: _t->genObjVarView((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ExtractDataDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_ExtractDataDialog.data,
    qt_meta_data_ExtractDataDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ExtractDataDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ExtractDataDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ExtractDataDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ExtractDataDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
