/****************************************************************************
** Meta object code from reading C++ file 'LabelDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/LabelDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'LabelDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LabelDialog_t {
    QByteArrayData data[16];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LabelDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LabelDialog_t qt_meta_stringdata_LabelDialog = {
    {
QT_MOC_LITERAL(0, 0, 11), // "LabelDialog"
QT_MOC_LITERAL(1, 12, 8), // "ShowList"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 10), // "ListSignal"
QT_MOC_LITERAL(4, 33, 4), // "lsig"
QT_MOC_LITERAL(5, 38, 8), // "AddLabel"
QT_MOC_LITERAL(6, 47, 15), // "ImportLabelList"
QT_MOC_LITERAL(7, 63, 11), // "ImportTable"
QT_MOC_LITERAL(8, 75, 4), // "Show"
QT_MOC_LITERAL(9, 80, 11), // "RemoveLabel"
QT_MOC_LITERAL(10, 92, 2), // "OK"
QT_MOC_LITERAL(11, 95, 6), // "Cancel"
QT_MOC_LITERAL(12, 102, 5), // "Close"
QT_MOC_LITERAL(13, 108, 12), // "GenLabelView"
QT_MOC_LITERAL(14, 121, 11), // "QModelIndex"
QT_MOC_LITERAL(15, 133, 10) // "setLabelId"

    },
    "LabelDialog\0ShowList\0\0ListSignal\0lsig\0"
    "AddLabel\0ImportLabelList\0ImportTable\0"
    "Show\0RemoveLabel\0OK\0Cancel\0Close\0"
    "GenLabelView\0QModelIndex\0setLabelId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LabelDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    0,   75,    2, 0x08 /* Private */,
       9,    0,   76,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x08 /* Private */,
      11,    0,   78,    2, 0x08 /* Private */,
      12,    0,   79,    2, 0x08 /* Private */,
      13,    1,   80,    2, 0x08 /* Private */,
      15,    1,   83,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,    2,
    QMetaType::Void, 0x80000000 | 14,    2,

       0        // eod
};

void LabelDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LabelDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->ShowList((*reinterpret_cast< ListSignal(*)>(_a[1]))); break;
        case 1: _t->AddLabel(); break;
        case 2: _t->ImportLabelList(); break;
        case 3: _t->ImportTable(); break;
        case 4: _t->Show(); break;
        case 5: _t->RemoveLabel(); break;
        case 6: _t->OK(); break;
        case 7: _t->Cancel(); break;
        case 8: _t->Close(); break;
        case 9: _t->GenLabelView((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        case 10: _t->setLabelId((*reinterpret_cast< QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LabelDialog::*)(ListSignal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LabelDialog::ShowList)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LabelDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_LabelDialog.data,
    qt_meta_data_LabelDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LabelDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LabelDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LabelDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LabelDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

// SIGNAL 0
void LabelDialog::ShowList(ListSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
