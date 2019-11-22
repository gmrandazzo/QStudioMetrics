/****************************************************************************
** Meta object code from reading C++ file 'ValidatorDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/ValidatorDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ValidatorDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ValidatorDialog_t {
    QByteArrayData data[13];
    char stringdata0[155];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ValidatorDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ValidatorDialog_t qt_meta_stringdata_ValidatorDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "ValidatorDialog"
QT_MOC_LITERAL(1, 16, 2), // "OK"
QT_MOC_LITERAL(2, 19, 0), // ""
QT_MOC_LITERAL(3, 20, 10), // "setProject"
QT_MOC_LITERAL(4, 31, 11), // "QModelIndex"
QT_MOC_LITERAL(5, 43, 7), // "current"
QT_MOC_LITERAL(6, 51, 10), // "setModelID"
QT_MOC_LITERAL(7, 62, 17), // "setValidationType"
QT_MOC_LITERAL(8, 80, 13), // "setKFoldClass"
QT_MOC_LITERAL(9, 94, 14), // "setNIterations"
QT_MOC_LITERAL(10, 109, 9), // "setNGroup"
QT_MOC_LITERAL(11, 119, 14), // "setYScrambling"
QT_MOC_LITERAL(12, 134, 20) // "setYSCramblingModels"

    },
    "ValidatorDialog\0OK\0\0setProject\0"
    "QModelIndex\0current\0setModelID\0"
    "setValidationType\0setKFoldClass\0"
    "setNIterations\0setNGroup\0setYScrambling\0"
    "setYSCramblingModels"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ValidatorDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x08 /* Private */,
       3,    1,   60,    2, 0x08 /* Private */,
       6,    1,   63,    2, 0x08 /* Private */,
       7,    0,   66,    2, 0x08 /* Private */,
       8,    0,   67,    2, 0x08 /* Private */,
       9,    0,   68,    2, 0x08 /* Private */,
      10,    0,   69,    2, 0x08 /* Private */,
      11,    0,   70,    2, 0x08 /* Private */,
      12,    0,   71,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void, 0x80000000 | 4,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void ValidatorDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ValidatorDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->OK(); break;
        case 1: _t->setProject((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 2: _t->setModelID((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 3: _t->setValidationType(); break;
        case 4: _t->setKFoldClass(); break;
        case 5: _t->setNIterations(); break;
        case 6: _t->setNGroup(); break;
        case 7: _t->setYScrambling(); break;
        case 8: _t->setYSCramblingModels(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject ValidatorDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ValidatorDialog.data,
    qt_meta_data_ValidatorDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ValidatorDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ValidatorDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ValidatorDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ValidatorDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
