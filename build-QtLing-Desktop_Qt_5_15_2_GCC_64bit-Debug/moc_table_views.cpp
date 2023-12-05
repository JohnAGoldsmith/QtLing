/****************************************************************************
** Meta object code from reading C++ file 'table_views.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../QtLing/table_views.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'table_views.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UpperTableView_t {
    QByteArrayData data[19];
    char stringdata0[293];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UpperTableView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UpperTableView_t qt_meta_stringdata_UpperTableView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "UpperTableView"
QT_MOC_LITERAL(1, 15, 15), // "num_items_found"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 29), // "please_display_this_signature"
QT_MOC_LITERAL(4, 62, 3), // "sig"
QT_MOC_LITERAL(5, 66, 24), // "get_signature_sort_style"
QT_MOC_LITERAL(6, 91, 10), // "eSortStyle"
QT_MOC_LITERAL(7, 102, 24), // "set_signature_sort_style"
QT_MOC_LITERAL(8, 127, 5), // "style"
QT_MOC_LITERAL(9, 133, 24), // "ShowModelsUpperTableView"
QT_MOC_LITERAL(10, 158, 11), // "QModelIndex"
QT_MOC_LITERAL(11, 170, 31), // "display_this_affixes_signatures"
QT_MOC_LITERAL(12, 202, 5), // "index"
QT_MOC_LITERAL(13, 208, 23), // "find_prev_and_highlight"
QT_MOC_LITERAL(14, 232, 8), // "QString&"
QT_MOC_LITERAL(15, 241, 1), // "s"
QT_MOC_LITERAL(16, 243, 23), // "find_next_and_highlight"
QT_MOC_LITERAL(17, 267, 12), // "clear_search"
QT_MOC_LITERAL(18, 280, 12) // "focusInEvent"

    },
    "UpperTableView\0num_items_found\0\0"
    "please_display_this_signature\0sig\0"
    "get_signature_sort_style\0eSortStyle\0"
    "set_signature_sort_style\0style\0"
    "ShowModelsUpperTableView\0QModelIndex\0"
    "display_this_affixes_signatures\0index\0"
    "find_prev_and_highlight\0QString&\0s\0"
    "find_next_and_highlight\0clear_search\0"
    "focusInEvent"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UpperTableView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       3,    1,   67,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   70,    2, 0x0a /* Public */,
       7,    1,   71,    2, 0x0a /* Public */,
       9,    1,   74,    2, 0x0a /* Public */,
      11,    1,   77,    2, 0x0a /* Public */,
      13,    1,   80,    2, 0x0a /* Public */,
      16,    1,   83,    2, 0x0a /* Public */,
      17,    0,   86,    2, 0x0a /* Public */,
      18,    0,   87,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Int,    2,
    QMetaType::Void, QMetaType::QString,    4,

 // slots: parameters
    0x80000000 | 6,
    QMetaType::Void, 0x80000000 | 6,    8,
    QMetaType::Void, 0x80000000 | 10,    2,
    QMetaType::Void, 0x80000000 | 10,   12,
    QMetaType::Bool, 0x80000000 | 14,   15,
    QMetaType::Bool, 0x80000000 | 14,   15,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UpperTableView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UpperTableView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->num_items_found((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->please_display_this_signature((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: { eSortStyle _r = _t->get_signature_sort_style();
            if (_a[0]) *reinterpret_cast< eSortStyle*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->set_signature_sort_style((*reinterpret_cast< eSortStyle(*)>(_a[1]))); break;
        case 4: _t->ShowModelsUpperTableView((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 5: _t->display_this_affixes_signatures((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 6: { bool _r = _t->find_prev_and_highlight((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 7: { bool _r = _t->find_next_and_highlight((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 8: _t->clear_search(); break;
        case 9: _t->focusInEvent(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UpperTableView::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UpperTableView::num_items_found)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UpperTableView::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UpperTableView::please_display_this_signature)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UpperTableView::staticMetaObject = { {
    QMetaObject::SuperData::link<QTableView::staticMetaObject>(),
    qt_meta_stringdata_UpperTableView.data,
    qt_meta_data_UpperTableView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UpperTableView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UpperTableView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UpperTableView.stringdata0))
        return static_cast<void*>(this);
    return QTableView::qt_metacast(_clname);
}

int UpperTableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void UpperTableView::num_items_found(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void UpperTableView::please_display_this_signature(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_LowerTableView_t {
    QByteArrayData data[26];
    char stringdata0[347];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LowerTableView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LowerTableView_t qt_meta_stringdata_LowerTableView = {
    {
QT_MOC_LITERAL(0, 0, 14), // "LowerTableView"
QT_MOC_LITERAL(1, 15, 17), // "display_this_item"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 11), // "QModelIndex"
QT_MOC_LITERAL(4, 46, 11), // "get_lexicon"
QT_MOC_LITERAL(5, 58, 9), // "CLexicon*"
QT_MOC_LITERAL(6, 68, 21), // "get_number_of_columns"
QT_MOC_LITERAL(7, 90, 24), // "graphics_sig_graph_edges"
QT_MOC_LITERAL(8, 115, 11), // "CSignature*"
QT_MOC_LITERAL(9, 127, 15), // "table_signature"
QT_MOC_LITERAL(10, 143, 10), // "table_word"
QT_MOC_LITERAL(11, 154, 6), // "CWord*"
QT_MOC_LITERAL(12, 161, 19), // "table_one_signature"
QT_MOC_LITERAL(13, 181, 4), // "pSig"
QT_MOC_LITERAL(14, 186, 5), // "stems"
QT_MOC_LITERAL(15, 192, 7), // "str_sig"
QT_MOC_LITERAL(16, 200, 23), // "table_passive_signature"
QT_MOC_LITERAL(17, 224, 10), // "table_stem"
QT_MOC_LITERAL(18, 235, 6), // "stem_t"
QT_MOC_LITERAL(19, 242, 15), // "table_protostem"
QT_MOC_LITERAL(20, 258, 10), // "protostem*"
QT_MOC_LITERAL(21, 269, 11), // "p_protostem"
QT_MOC_LITERAL(22, 281, 26), // "table_compound_composition"
QT_MOC_LITERAL(23, 308, 13), // "CompoundWord*"
QT_MOC_LITERAL(24, 322, 10), // "p_compound"
QT_MOC_LITERAL(25, 333, 13) // "composition_i"

    },
    "LowerTableView\0display_this_item\0\0"
    "QModelIndex\0get_lexicon\0CLexicon*\0"
    "get_number_of_columns\0graphics_sig_graph_edges\0"
    "CSignature*\0table_signature\0table_word\0"
    "CWord*\0table_one_signature\0pSig\0stems\0"
    "str_sig\0table_passive_signature\0"
    "table_stem\0stem_t\0table_protostem\0"
    "protostem*\0p_protostem\0"
    "table_compound_composition\0CompoundWord*\0"
    "p_compound\0composition_i"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LowerTableView[] = {

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
       1,    1,   69,    2, 0x0a /* Public */,
       4,    0,   72,    2, 0x0a /* Public */,
       6,    0,   73,    2, 0x0a /* Public */,
       7,    2,   74,    2, 0x0a /* Public */,
       9,    1,   79,    2, 0x0a /* Public */,
      10,    1,   82,    2, 0x0a /* Public */,
      12,    3,   85,    2, 0x0a /* Public */,
      16,    1,   92,    2, 0x0a /* Public */,
      17,    2,   95,    2, 0x0a /* Public */,
      19,    1,  100,    2, 0x0a /* Public */,
      22,    2,  103,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    0x80000000 | 5,
    QMetaType::Int,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 5,    2,    2,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 11,    2,
    QMetaType::Void, 0x80000000 | 8, QMetaType::QStringList, QMetaType::QString,   13,   14,   15,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 18, 0x80000000 | 5,    2,    2,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, 0x80000000 | 23, QMetaType::Int,   24,   25,

       0        // eod
};

void LowerTableView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LowerTableView *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->display_this_item((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 1: { CLexicon* _r = _t->get_lexicon();
            if (_a[0]) *reinterpret_cast< CLexicon**>(_a[0]) = std::move(_r); }  break;
        case 2: { int _r = _t->get_number_of_columns();
            if (_a[0]) *reinterpret_cast< int*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->graphics_sig_graph_edges((*reinterpret_cast< CSignature*(*)>(_a[1])),(*reinterpret_cast< CLexicon*(*)>(_a[2]))); break;
        case 4: _t->table_signature((*reinterpret_cast< CSignature*(*)>(_a[1]))); break;
        case 5: _t->table_word((*reinterpret_cast< CWord*(*)>(_a[1]))); break;
        case 6: _t->table_one_signature((*reinterpret_cast< CSignature*(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 7: _t->table_passive_signature((*reinterpret_cast< CSignature*(*)>(_a[1]))); break;
        case 8: _t->table_stem((*reinterpret_cast< stem_t(*)>(_a[1])),(*reinterpret_cast< CLexicon*(*)>(_a[2]))); break;
        case 9: _t->table_protostem((*reinterpret_cast< protostem*(*)>(_a[1]))); break;
        case 10: _t->table_compound_composition((*reinterpret_cast< CompoundWord*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CSignature* >(); break;
            }
            break;
        case 4:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CSignature* >(); break;
            }
            break;
        case 6:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CSignature* >(); break;
            }
            break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CSignature* >(); break;
            }
            break;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LowerTableView::staticMetaObject = { {
    QMetaObject::SuperData::link<QTableView::staticMetaObject>(),
    qt_meta_stringdata_LowerTableView.data,
    qt_meta_data_LowerTableView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LowerTableView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LowerTableView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LowerTableView.stringdata0))
        return static_cast<void*>(this);
    return QTableView::qt_metacast(_clname);
}

int LowerTableView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTableView::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 11)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 11;
    }
    return _id;
}
struct qt_meta_stringdata_LeftSideTreeView_t {
    QByteArrayData data[1];
    char stringdata0[17];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LeftSideTreeView_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LeftSideTreeView_t qt_meta_stringdata_LeftSideTreeView = {
    {
QT_MOC_LITERAL(0, 0, 16) // "LeftSideTreeView"

    },
    "LeftSideTreeView"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LeftSideTreeView[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

       0        // eod
};

void LeftSideTreeView::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

QT_INIT_METAOBJECT const QMetaObject LeftSideTreeView::staticMetaObject = { {
    QMetaObject::SuperData::link<QTreeView::staticMetaObject>(),
    qt_meta_stringdata_LeftSideTreeView.data,
    qt_meta_data_LeftSideTreeView,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LeftSideTreeView::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LeftSideTreeView::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LeftSideTreeView.stringdata0))
        return static_cast<void*>(this);
    return QTreeView::qt_metacast(_clname);
}

int LeftSideTreeView::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeView::qt_metacall(_c, _id, _a);
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
