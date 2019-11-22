/****************************************************************************
** Meta object code from reading C++ file 'ObjectSelectorDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/ObjectSelectorDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ObjectSelectorDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ObjectSelectorDialog_t {
    QByteArrayData data[11];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ObjectSelectorDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ObjectSelectorDialog_t qt_meta_stringdata_ObjectSelectorDialog = {
    {
QT_MOC_LITERAL(0, 0, 20), // "ObjectSelectorDialog"
QT_MOC_LITERAL(1, 21, 13), // "EnableDisable"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 16), // "setVariablesView"
QT_MOC_LITERAL(4, 53, 11), // "QModelIndex"
QT_MOC_LITERAL(5, 65, 7), // "current"
QT_MOC_LITERAL(6, 73, 9), // "SelectAll"
QT_MOC_LITERAL(7, 83, 15), // "InvertSelection"
QT_MOC_LITERAL(8, 99, 8), // "Unselect"
QT_MOC_LITERAL(9, 108, 16), // "SelectByVarLabel"
QT_MOC_LITERAL(10, 125, 2) // "OK"

    },
    "ObjectSelectorDialog\0EnableDisable\0\0"
    "setVariablesView\0QModelIndex\0current\0"
    "SelectAll\0InvertSelection\0Unselect\0"
    "SelectByVarLabel\0OK"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ObjectSelectorDialog[] = {

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
       1,    0,   49,    2, 0x08 /* Private */,
       3,    1,   50,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,
       9,    0,   56,    2, 0x08 /* Private */,
      10,    0,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ObjectSelectorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ObjectSelectorDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->EnableDisable(); break;
        case 1: _t->setVariablesView((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->SelectAll(); break;
        case 3: _t->InvertSelection(); break;
        case 4: _t->Unselect(); break;
        case 5: _t->SelectByVarLabel(); break;
        case 6: _t->OK(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ObjectSelectorDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ObjectSelectorDialog.data,
    qt_meta_data_ObjectSelectorDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ObjectSelectorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ObjectSelectorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ObjectSelectorDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ObjectSelectorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
