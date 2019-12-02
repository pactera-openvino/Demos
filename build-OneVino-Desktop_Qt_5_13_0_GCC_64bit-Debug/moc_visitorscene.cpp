/****************************************************************************
** Meta object code from reading C++ file 'visitorscene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../OneVino/visitorscene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'visitorscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_VisitorSeceneThread_t {
    QByteArrayData data[23];
    char stringdata0[223];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_VisitorSeceneThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_VisitorSeceneThread_t qt_meta_stringdata_VisitorSeceneThread = {
    {
QT_MOC_LITERAL(0, 0, 19), // "VisitorSeceneThread"
QT_MOC_LITERAL(1, 20, 9), // "sendImage"
QT_MOC_LITERAL(2, 30, 0), // ""
QT_MOC_LITERAL(3, 31, 3), // "msg"
QT_MOC_LITERAL(4, 35, 13), // "sendTotalFace"
QT_MOC_LITERAL(5, 49, 7), // "FaceNum"
QT_MOC_LITERAL(6, 57, 12), // "sendFaceInfo"
QT_MOC_LITERAL(7, 70, 9), // "Face::Ptr"
QT_MOC_LITERAL(8, 80, 4), // "face"
QT_MOC_LITERAL(9, 85, 11), // "ImageFaceId"
QT_MOC_LITERAL(10, 97, 11), // "updatechart"
QT_MOC_LITERAL(11, 109, 7), // "malenum"
QT_MOC_LITERAL(12, 117, 9), // "femalenum"
QT_MOC_LITERAL(13, 127, 9), // "inittable"
QT_MOC_LITERAL(14, 137, 11), // "TotalRecord"
QT_MOC_LITERAL(15, 149, 16), // "personAttribute*"
QT_MOC_LITERAL(16, 166, 9), // "pAttTotal"
QT_MOC_LITERAL(17, 176, 11), // "updatetable"
QT_MOC_LITERAL(18, 188, 4), // "pAtt"
QT_MOC_LITERAL(19, 193, 13), // "updateEmChart"
QT_MOC_LITERAL(20, 207, 4), // "int*"
QT_MOC_LITERAL(21, 212, 2), // "em"
QT_MOC_LITERAL(22, 215, 7) // "sendpic"

    },
    "VisitorSeceneThread\0sendImage\0\0msg\0"
    "sendTotalFace\0FaceNum\0sendFaceInfo\0"
    "Face::Ptr\0face\0ImageFaceId\0updatechart\0"
    "malenum\0femalenum\0inittable\0TotalRecord\0"
    "personAttribute*\0pAttTotal\0updatetable\0"
    "pAtt\0updateEmChart\0int*\0em\0sendpic"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_VisitorSeceneThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       8,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       6,    2,   60,    2, 0x06 /* Public */,
      10,    2,   65,    2, 0x06 /* Public */,
      13,    2,   70,    2, 0x06 /* Public */,
      17,    1,   75,    2, 0x06 /* Public */,
      19,    1,   78,    2, 0x06 /* Public */,
      22,    1,   81,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int,    8,    9,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   11,   12,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,   14,   16,
    QMetaType::Void, 0x80000000 | 15,   18,
    QMetaType::Void, 0x80000000 | 20,   21,
    QMetaType::Void, QMetaType::QImage,    3,

       0        // eod
};

void VisitorSeceneThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<VisitorSeceneThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->sendTotalFace((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->sendFaceInfo((*reinterpret_cast< Face::Ptr(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 3: _t->updatechart((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->inittable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< personAttribute*(*)>(_a[2]))); break;
        case 5: _t->updatetable((*reinterpret_cast< personAttribute*(*)>(_a[1]))); break;
        case 6: _t->updateEmChart((*reinterpret_cast< int*(*)>(_a[1]))); break;
        case 7: _t->sendpic((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (VisitorSeceneThread::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VisitorSeceneThread::sendImage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (VisitorSeceneThread::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VisitorSeceneThread::sendTotalFace)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (VisitorSeceneThread::*)(Face::Ptr , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VisitorSeceneThread::sendFaceInfo)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (VisitorSeceneThread::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VisitorSeceneThread::updatechart)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (VisitorSeceneThread::*)(int , personAttribute * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VisitorSeceneThread::inittable)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (VisitorSeceneThread::*)(personAttribute * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VisitorSeceneThread::updatetable)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (VisitorSeceneThread::*)(int * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VisitorSeceneThread::updateEmChart)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (VisitorSeceneThread::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&VisitorSeceneThread::sendpic)) {
                *result = 7;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject VisitorSeceneThread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_VisitorSeceneThread.data,
    qt_meta_data_VisitorSeceneThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *VisitorSeceneThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *VisitorSeceneThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_VisitorSeceneThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int VisitorSeceneThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void VisitorSeceneThread::sendImage(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void VisitorSeceneThread::sendTotalFace(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void VisitorSeceneThread::sendFaceInfo(Face::Ptr _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void VisitorSeceneThread::updatechart(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void VisitorSeceneThread::inittable(int _t1, personAttribute * _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void VisitorSeceneThread::updatetable(personAttribute * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void VisitorSeceneThread::updateEmChart(int * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void VisitorSeceneThread::sendpic(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}
struct qt_meta_stringdata_visitorscene_t {
    QByteArrayData data[25];
    char stringdata0[268];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_visitorscene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_visitorscene_t qt_meta_stringdata_visitorscene = {
    {
QT_MOC_LITERAL(0, 0, 12), // "visitorscene"
QT_MOC_LITERAL(1, 13, 10), // "closeEvent"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "QCloseEvent*"
QT_MOC_LITERAL(4, 38, 5), // "event"
QT_MOC_LITERAL(5, 44, 11), // "acceptImage"
QT_MOC_LITERAL(6, 56, 3), // "msg"
QT_MOC_LITERAL(7, 60, 15), // "acceptTotalFace"
QT_MOC_LITERAL(8, 76, 7), // "FaceNum"
QT_MOC_LITERAL(9, 84, 14), // "acceptFaceInfo"
QT_MOC_LITERAL(10, 99, 9), // "Face::Ptr"
QT_MOC_LITERAL(11, 109, 4), // "face"
QT_MOC_LITERAL(12, 114, 11), // "ImageFaceId"
QT_MOC_LITERAL(13, 126, 17), // "acceptupdatechart"
QT_MOC_LITERAL(14, 144, 7), // "malenum"
QT_MOC_LITERAL(15, 152, 9), // "femalenum"
QT_MOC_LITERAL(16, 162, 15), // "acceptinittable"
QT_MOC_LITERAL(17, 178, 11), // "TotalRecord"
QT_MOC_LITERAL(18, 190, 16), // "personAttribute*"
QT_MOC_LITERAL(19, 207, 9), // "pAttTotal"
QT_MOC_LITERAL(20, 217, 17), // "acceptupdatetable"
QT_MOC_LITERAL(21, 235, 4), // "pAtt"
QT_MOC_LITERAL(22, 240, 19), // "acceptupdateEmchart"
QT_MOC_LITERAL(23, 260, 4), // "int*"
QT_MOC_LITERAL(24, 265, 2) // "em"

    },
    "visitorscene\0closeEvent\0\0QCloseEvent*\0"
    "event\0acceptImage\0msg\0acceptTotalFace\0"
    "FaceNum\0acceptFaceInfo\0Face::Ptr\0face\0"
    "ImageFaceId\0acceptupdatechart\0malenum\0"
    "femalenum\0acceptinittable\0TotalRecord\0"
    "personAttribute*\0pAttTotal\0acceptupdatetable\0"
    "pAtt\0acceptupdateEmchart\0int*\0em"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_visitorscene[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x08 /* Private */,
       5,    1,   57,    2, 0x08 /* Private */,
       7,    1,   60,    2, 0x08 /* Private */,
       9,    2,   63,    2, 0x08 /* Private */,
      13,    2,   68,    2, 0x08 /* Private */,
      16,    2,   73,    2, 0x08 /* Private */,
      20,    1,   78,    2, 0x08 /* Private */,
      22,    1,   81,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::QImage,    6,
    QMetaType::Void, QMetaType::Int,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Int,   11,   12,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   14,   15,
    QMetaType::Void, QMetaType::Int, 0x80000000 | 18,   17,   19,
    QMetaType::Void, 0x80000000 | 18,   21,
    QMetaType::Void, 0x80000000 | 23,   24,

       0        // eod
};

void visitorscene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<visitorscene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 1: _t->acceptImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 2: _t->acceptTotalFace((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->acceptFaceInfo((*reinterpret_cast< Face::Ptr(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 4: _t->acceptupdatechart((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->acceptinittable((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< personAttribute*(*)>(_a[2]))); break;
        case 6: _t->acceptupdatetable((*reinterpret_cast< personAttribute*(*)>(_a[1]))); break;
        case 7: _t->acceptupdateEmchart((*reinterpret_cast< int*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject visitorscene::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_visitorscene.data,
    qt_meta_data_visitorscene,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *visitorscene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *visitorscene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_visitorscene.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int visitorscene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
