/****************************************************************************
** Meta object code from reading C++ file 'qplotly.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../src/Chart/qplotly.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qplotly.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QPlotlyWindow_t {
    QByteArrayData data[6];
    char stringdata0[70];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QPlotlyWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QPlotlyWindow_t qt_meta_stringdata_QPlotlyWindow = {
    {
QT_MOC_LITERAL(0, 0, 13), // "QPlotlyWindow"
QT_MOC_LITERAL(1, 14, 26), // "WebEnginehandleCookieAdded"
QT_MOC_LITERAL(2, 41, 0), // ""
QT_MOC_LITERAL(3, 42, 14), // "QNetworkCookie"
QT_MOC_LITERAL(4, 57, 6), // "cookie"
QT_MOC_LITERAL(5, 64, 5) // "close"

    },
    "QPlotlyWindow\0WebEnginehandleCookieAdded\0"
    "\0QNetworkCookie\0cookie\0close"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QPlotlyWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   24,    2, 0x08 /* Private */,
       5,    0,   27,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

       0        // eod
};

void QPlotlyWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QPlotlyWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->WebEnginehandleCookieAdded((*reinterpret_cast< const QNetworkCookie(*)>(_a[1]))); break;
        case 1: _t->close(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QPlotlyWindow::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_QPlotlyWindow.data,
    qt_meta_data_QPlotlyWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QPlotlyWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QPlotlyWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QPlotlyWindow.stringdata0))
        return static_cast<void*>(this);
    if (!strcmp(_clname, "Graphs"))
        return static_cast< Graphs*>(this);
    return QWidget::qt_metacast(_clname);
}

int QPlotlyWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
