/****************************************************************************
** Meta object code from reading C++ file 'ExportTableDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/ExportTableDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ExportTableDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ExportTableDialog_t {
    QByteArrayData data[12];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ExportTableDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ExportTableDialog_t qt_meta_stringdata_ExportTableDialog = {
    {
QT_MOC_LITERAL(0, 0, 17), // "ExportTableDialog"
QT_MOC_LITERAL(1, 18, 6), // "SaveAs"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 14), // "EnableOKButton"
QT_MOC_LITERAL(4, 41, 12), // "ObjSelectAll"
QT_MOC_LITERAL(5, 54, 18), // "ObjInvertSelection"
QT_MOC_LITERAL(6, 73, 14), // "ObjUnselectAll"
QT_MOC_LITERAL(7, 88, 11), // "ObjSelectBy"
QT_MOC_LITERAL(8, 100, 12), // "VarSelectAll"
QT_MOC_LITERAL(9, 113, 18), // "VarInvertSelection"
QT_MOC_LITERAL(10, 132, 14), // "VarUnselectAll"
QT_MOC_LITERAL(11, 147, 11) // "VarSelectBy"

    },
    "ExportTableDialog\0SaveAs\0\0EnableOKButton\0"
    "ObjSelectAll\0ObjInvertSelection\0"
    "ObjUnselectAll\0ObjSelectBy\0VarSelectAll\0"
    "VarInvertSelection\0VarUnselectAll\0"
    "VarSelectBy"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ExportTableDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,

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

       0        // eod
};

void ExportTableDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<ExportTableDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->SaveAs(); break;
        case 1: _t->EnableOKButton(); break;
        case 2: _t->ObjSelectAll(); break;
        case 3: _t->ObjInvertSelection(); break;
        case 4: _t->ObjUnselectAll(); break;
        case 5: _t->ObjSelectBy(); break;
        case 6: _t->VarSelectAll(); break;
        case 7: _t->VarInvertSelection(); break;
        case 8: _t->VarUnselectAll(); break;
        case 9: _t->VarSelectBy(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject ExportTableDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_ExportTableDialog.data,
    qt_meta_data_ExportTableDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *ExportTableDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ExportTableDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ExportTableDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int ExportTableDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
