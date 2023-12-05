/****************************************************************************
** Meta object code from reading C++ file 'mainwindow_find.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtLing/mainwindow_find.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow_find.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FindDockWidget_t {
    QByteArrayData data[5];
    char stringdata0[46];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FindDockWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FindDockWidget_t qt_meta_stringdata_FindDockWidget = {
    {
QT_MOC_LITERAL(0, 0, 14), // "FindDockWidget"
QT_MOC_LITERAL(1, 15, 10), // "closeEvent"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 12), // "QCloseEvent*"
QT_MOC_LITERAL(4, 40, 5) // "event"

    },
    "FindDockWidget\0closeEvent\0\0QCloseEvent*\0"
    "event"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FindDockWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   19,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

void FindDockWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FindDockWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FindDockWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QDockWidget::staticMetaObject>(),
    qt_meta_stringdata_FindDockWidget.data,
    qt_meta_data_FindDockWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FindDockWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FindDockWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FindDockWidget.stringdata0))
        return static_cast<void*>(this);
    return QDockWidget::qt_metacast(_clname);
}

int FindDockWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDockWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_FindDialog_t {
    QByteArrayData data[16];
    char stringdata0[225];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FindDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FindDialog_t qt_meta_stringdata_FindDialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "FindDialog"
QT_MOC_LITERAL(1, 11, 20), // "search_for_left_next"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 8), // "QString&"
QT_MOC_LITERAL(4, 42, 1), // "s"
QT_MOC_LITERAL(5, 44, 21), // "search_for_right_next"
QT_MOC_LITERAL(6, 66, 20), // "search_for_left_prev"
QT_MOC_LITERAL(7, 87, 21), // "search_for_right_prev"
QT_MOC_LITERAL(8, 109, 17), // "clear_left_search"
QT_MOC_LITERAL(9, 127, 18), // "clear_right_search"
QT_MOC_LITERAL(10, 146, 19), // "change_search_range"
QT_MOC_LITERAL(11, 166, 14), // "do_next_search"
QT_MOC_LITERAL(12, 181, 14), // "do_prev_search"
QT_MOC_LITERAL(13, 196, 10), // "item_found"
QT_MOC_LITERAL(14, 207, 1), // "n"
QT_MOC_LITERAL(15, 209, 15) // "do_clear_search"

    },
    "FindDialog\0search_for_left_next\0\0"
    "QString&\0s\0search_for_right_next\0"
    "search_for_left_prev\0search_for_right_prev\0"
    "clear_left_search\0clear_right_search\0"
    "change_search_range\0do_next_search\0"
    "do_prev_search\0item_found\0n\0do_clear_search"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FindDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      11,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   69,    2, 0x06 /* Public */,
       5,    1,   72,    2, 0x06 /* Public */,
       6,    1,   75,    2, 0x06 /* Public */,
       7,    1,   78,    2, 0x06 /* Public */,
       8,    0,   81,    2, 0x06 /* Public */,
       9,    0,   82,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    1,   83,    2, 0x0a /* Public */,
      11,    0,   86,    2, 0x0a /* Public */,
      12,    0,   87,    2, 0x0a /* Public */,
      13,    1,   88,    2, 0x0a /* Public */,
      15,    0,   91,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   14,
    QMetaType::Void,

       0        // eod
};

void FindDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FindDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->search_for_left_next((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->search_for_right_next((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->search_for_left_prev((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: _t->search_for_right_prev((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 4: _t->clear_left_search(); break;
        case 5: _t->clear_right_search(); break;
        case 6: _t->change_search_range((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->do_next_search(); break;
        case 8: _t->do_prev_search(); break;
        case 9: _t->item_found((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 10: _t->do_clear_search(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FindDialog::*)(QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindDialog::search_for_left_next)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FindDialog::*)(QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindDialog::search_for_right_next)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FindDialog::*)(QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindDialog::search_for_left_prev)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FindDialog::*)(QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindDialog::search_for_right_prev)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FindDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindDialog::clear_left_search)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (FindDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FindDialog::clear_right_search)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FindDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_FindDialog.data,
    qt_meta_data_FindDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FindDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FindDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FindDialog.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int FindDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void FindDialog::search_for_left_next(QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void FindDialog::search_for_right_next(QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void FindDialog::search_for_left_prev(QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FindDialog::search_for_right_prev(QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FindDialog::clear_left_search()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void FindDialog::clear_right_search()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
