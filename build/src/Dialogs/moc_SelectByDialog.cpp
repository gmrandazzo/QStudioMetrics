/****************************************************************************
** Meta object code from reading C++ file 'SelectByDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/SelectByDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SelectByDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SelectByDialog_t {
    QByteArrayData data[13];
    char stringdata0[169];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SelectByDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SelectByDialog_t qt_meta_stringdata_SelectByDialog = {
    {
QT_MOC_LITERAL(0, 0, 14), // "SelectByDialog"
QT_MOC_LITERAL(1, 15, 22), // "UpdateSelectionCounter"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 12), // "selectButton"
QT_MOC_LITERAL(4, 52, 18), // "invertselectButton"
QT_MOC_LITERAL(5, 71, 21), // "appendselectionButton"
QT_MOC_LITERAL(6, 93, 14), // "unselectButton"
QT_MOC_LITERAL(7, 108, 16), // "SetSelectionList"
QT_MOC_LITERAL(8, 125, 18), // "OpenSelectorButton"
QT_MOC_LITERAL(9, 144, 8), // "FindName"
QT_MOC_LITERAL(10, 153, 5), // "Abort"
QT_MOC_LITERAL(11, 159, 2), // "OK"
QT_MOC_LITERAL(12, 162, 6) // "Cancel"

    },
    "SelectByDialog\0UpdateSelectionCounter\0"
    "\0selectButton\0invertselectButton\0"
    "appendselectionButton\0unselectButton\0"
    "SetSelectionList\0OpenSelectorButton\0"
    "FindName\0Abort\0OK\0Cancel"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SelectByDialog[] = {

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
       3,    0,   70,    2, 0x08 /* Private */,
       4,    0,   71,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    0,   73,    2, 0x08 /* Private */,
       7,    0,   74,    2, 0x08 /* Private */,
       8,    0,   75,    2, 0x08 /* Private */,
       9,    0,   76,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x08 /* Private */,
      11,    0,   78,    2, 0x08 /* Private */,
      12,    0,   79,    2, 0x08 /* Private */,

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

       0        // eod
};

void SelectByDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SelectByDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->UpdateSelectionCounter(); break;
        case 1: _t->selectButton(); break;
        case 2: _t->invertselectButton(); break;
        case 3: _t->appendselectionButton(); break;
        case 4: _t->unselectButton(); break;
        case 5: _t->SetSelectionList(); break;
        case 6: _t->OpenSelectorButton(); break;
        case 7: _t->FindName(); break;
        case 8: _t->Abort(); break;
        case 9: _t->OK(); break;
        case 10: _t->Cancel(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SelectByDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_SelectByDialog.data,
    qt_meta_data_SelectByDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SelectByDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SelectByDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SelectByDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SelectByDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
