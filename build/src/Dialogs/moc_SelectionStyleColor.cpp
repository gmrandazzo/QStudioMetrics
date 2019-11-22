/****************************************************************************
** Meta object code from reading C++ file 'SelectionStyleColor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/SelectionStyleColor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SelectionStyleColor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SelectionStyleColor_t {
    QByteArrayData data[11];
    char stringdata0[178];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SelectionStyleColor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SelectionStyleColor_t qt_meta_stringdata_SelectionStyleColor = {
    {
QT_MOC_LITERAL(0, 0, 19), // "SelectionStyleColor"
QT_MOC_LITERAL(1, 20, 6), // "Cancel"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 2), // "OK"
QT_MOC_LITERAL(4, 31, 19), // "SetSolidSymbolColor"
QT_MOC_LITERAL(5, 51, 26), // "SetVariableTypeSymbolColor"
QT_MOC_LITERAL(6, 78, 17), // "SetMinSymbolColor"
QT_MOC_LITERAL(7, 96, 17), // "SetMaxSymbolColor"
QT_MOC_LITERAL(8, 114, 22), // "SetVariableSymbolColor"
QT_MOC_LITERAL(9, 137, 27), // "SetVariableLevelSymbolColor"
QT_MOC_LITERAL(10, 165, 12) // "CheckUncheck"

    },
    "SelectionStyleColor\0Cancel\0\0OK\0"
    "SetSolidSymbolColor\0SetVariableTypeSymbolColor\0"
    "SetMinSymbolColor\0SetMaxSymbolColor\0"
    "SetVariableSymbolColor\0"
    "SetVariableLevelSymbolColor\0CheckUncheck"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SelectionStyleColor[] = {

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
       1,    0,   59,    2, 0x0a /* Public */,
       3,    0,   60,    2, 0x0a /* Public */,
       4,    0,   61,    2, 0x0a /* Public */,
       5,    0,   62,    2, 0x0a /* Public */,
       6,    0,   63,    2, 0x0a /* Public */,
       7,    0,   64,    2, 0x0a /* Public */,
       8,    0,   65,    2, 0x0a /* Public */,
       9,    0,   66,    2, 0x0a /* Public */,
      10,    0,   67,    2, 0x0a /* Public */,

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

       0        // eod
};

void SelectionStyleColor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SelectionStyleColor *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->Cancel(); break;
        case 1: _t->OK(); break;
        case 2: _t->SetSolidSymbolColor(); break;
        case 3: _t->SetVariableTypeSymbolColor(); break;
        case 4: _t->SetMinSymbolColor(); break;
        case 5: _t->SetMaxSymbolColor(); break;
        case 6: _t->SetVariableSymbolColor(); break;
        case 7: _t->SetVariableLevelSymbolColor(); break;
        case 8: _t->CheckUncheck(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject SelectionStyleColor::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_SelectionStyleColor.data,
    qt_meta_data_SelectionStyleColor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SelectionStyleColor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SelectionStyleColor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SelectionStyleColor.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int SelectionStyleColor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
