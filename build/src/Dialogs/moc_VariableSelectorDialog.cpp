/****************************************************************************
** Meta object code from reading C++ file 'VariableSelectorDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/VariableSelectorDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'VariableSelectorDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VariableSelectorDialog_t {
    QByteArrayData data[10];
    char stringdata0[115];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VariableSelectorDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VariableSelectorDialog_t qt_meta_stringdata_VariableSelectorDialog = {
    {
QT_MOC_LITERAL(0, 0, 22), // "VariableSelectorDialog"
QT_MOC_LITERAL(1, 23, 6), // "Select"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 15), // "InvertSelection"
QT_MOC_LITERAL(4, 47, 15), // "AppendSelection"
QT_MOC_LITERAL(5, 63, 8), // "Unselect"
QT_MOC_LITERAL(6, 72, 4), // "find"
QT_MOC_LITERAL(7, 77, 2), // "OK"
QT_MOC_LITERAL(8, 80, 13), // "EnableDisable"
QT_MOC_LITERAL(9, 94, 20) // "UpdateLabelSelection"

    },
    "VariableSelectorDialog\0Select\0\0"
    "InvertSelection\0AppendSelection\0"
    "Unselect\0find\0OK\0EnableDisable\0"
    "UpdateLabelSelection"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VariableSelectorDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x08 /* Private */,
       3,    0,   55,    2, 0x08 /* Private */,
       4,    0,   56,    2, 0x08 /* Private */,
       5,    0,   57,    2, 0x08 /* Private */,
       6,    0,   58,    2, 0x08 /* Private */,
       7,    0,   59,    2, 0x08 /* Private */,
       8,    0,   60,    2, 0x08 /* Private */,
       9,    0,   61,    2, 0x08 /* Private */,

 // slots: parameters
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

void VariableSelectorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VariableSelectorDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Select(); break;
        case 1: _t->InvertSelection(); break;
        case 2: _t->AppendSelection(); break;
        case 3: _t->Unselect(); break;
        case 4: _t->find(); break;
        case 5: _t->OK(); break;
        case 6: _t->EnableDisable(); break;
        case 7: _t->UpdateLabelSelection(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject VariableSelectorDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_VariableSelectorDialog.data,
    qt_meta_data_VariableSelectorDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VariableSelectorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VariableSelectorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VariableSelectorDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int VariableSelectorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
