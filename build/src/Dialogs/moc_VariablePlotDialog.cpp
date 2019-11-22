/****************************************************************************
** Meta object code from reading C++ file 'VariablePlotDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/VariablePlotDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VariablePlotDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VariablePlotDialog_t {
    QByteArrayData data[10];
    char stringdata0[137];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VariablePlotDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VariablePlotDialog_t qt_meta_stringdata_VariablePlotDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "VariablePlotDialog"
QT_MOC_LITERAL(1, 19, 14), // "setVariableID1"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 11), // "QModelIndex"
QT_MOC_LITERAL(4, 47, 14), // "setVariableID2"
QT_MOC_LITERAL(5, 62, 25), // "GenVariableViewAndSetHash"
QT_MOC_LITERAL(6, 88, 23), // "GenDataViewAndLabelView"
QT_MOC_LITERAL(7, 112, 14), // "getObjectNames"
QT_MOC_LITERAL(8, 127, 2), // "OK"
QT_MOC_LITERAL(9, 130, 6) // "Cancel"

    },
    "VariablePlotDialog\0setVariableID1\0\0"
    "QModelIndex\0setVariableID2\0"
    "GenVariableViewAndSetHash\0"
    "GenDataViewAndLabelView\0getObjectNames\0"
    "OK\0Cancel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VariablePlotDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x08 /* Private */,
       4,    1,   52,    2, 0x08 /* Private */,
       5,    0,   55,    2, 0x08 /* Private */,
       6,    1,   56,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void VariablePlotDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VariablePlotDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->setVariableID1((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 1: _t->setVariableID2((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->GenVariableViewAndSetHash(); break;
        case 3: _t->GenDataViewAndLabelView((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 4: _t->getObjectNames(); break;
        case 5: _t->OK(); break;
        case 6: _t->Cancel(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VariablePlotDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_VariablePlotDialog.data,
    qt_meta_data_VariablePlotDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VariablePlotDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VariablePlotDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VariablePlotDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int VariablePlotDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
