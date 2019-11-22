/****************************************************************************
** Meta object code from reading C++ file 'FindCorrelationWidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Dialogs/FindCorrelationWidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'FindCorrelationWidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FindCorrelationWidget_t {
    QByteArrayData data[15];
    char stringdata0[187];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FindCorrelationWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FindCorrelationWidget_t qt_meta_stringdata_FindCorrelationWidget = {
    {
QT_MOC_LITERAL(0, 0, 21), // "FindCorrelationWidget"
QT_MOC_LITERAL(1, 22, 12), // "VVPlotSignal"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 12), // "vvplotSignal"
QT_MOC_LITERAL(4, 49, 3), // "vvs"
QT_MOC_LITERAL(5, 53, 15), // "HighlightSignal"
QT_MOC_LITERAL(6, 69, 15), // "highlightSignal"
QT_MOC_LITERAL(7, 85, 3), // "hhs"
QT_MOC_LITERAL(8, 89, 32), // "CloseFindCorrelationWidgetSignal"
QT_MOC_LITERAL(9, 122, 4), // "Find"
QT_MOC_LITERAL(10, 127, 9), // "SelectAll"
QT_MOC_LITERAL(11, 137, 11), // "UnselectAll"
QT_MOC_LITERAL(12, 149, 15), // "InvertSelection"
QT_MOC_LITERAL(13, 165, 12), // "PlotVarVSVar"
QT_MOC_LITERAL(14, 178, 8) // "Higlight"

    },
    "FindCorrelationWidget\0VVPlotSignal\0\0"
    "vvplotSignal\0vvs\0HighlightSignal\0"
    "highlightSignal\0hhs\0"
    "CloseFindCorrelationWidgetSignal\0Find\0"
    "SelectAll\0UnselectAll\0InvertSelection\0"
    "PlotVarVSVar\0Higlight"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FindCorrelationWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       5,    1,   62,    2, 0x06 /* Public */,
       8,    0,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   66,    2, 0x08 /* Private */,
      10,    0,   67,    2, 0x08 /* Private */,
      11,    0,   68,    2, 0x08 /* Private */,
      12,    0,   69,    2, 0x08 /* Private */,
      13,    0,   70,    2, 0x08 /* Private */,
      14,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FindCorrelationWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FindCorrelationWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->VVPlotSignal((*reinterpret_cast< vvplotSignal(*)>(_a[1]))); break;
        case 1: _t->HighlightSignal((*reinterpret_cast< highlightSignal(*)>(_a[1]))); break;
        case 2: _t->CloseFindCorrelationWidgetSignal(); break;
        case 3: _t->Find(); break;
        case 4: _t->SelectAll(); break;
        case 5: _t->UnselectAll(); break;
        case 6: _t->InvertSelection(); break;
        case 7: _t->PlotVarVSVar(); break;
        case 8: _t->Higlight(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FindCorrelationWidget::*)(vvplotSignal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindCorrelationWidget::VVPlotSignal)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FindCorrelationWidget::*)(highlightSignal );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindCorrelationWidget::HighlightSignal)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FindCorrelationWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindCorrelationWidget::CloseFindCorrelationWidgetSignal)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FindCorrelationWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_FindCorrelationWidget.data,
    qt_meta_data_FindCorrelationWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FindCorrelationWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FindCorrelationWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FindCorrelationWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FindCorrelationWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void FindCorrelationWidget::VVPlotSignal(vvplotSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FindCorrelationWidget::HighlightSignal(highlightSignal _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FindCorrelationWidget::CloseFindCorrelationWidgetSignal()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
