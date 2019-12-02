/****************************************************************************
** Meta object code from reading C++ file 'meetingscene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../OneVino/meetingscene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'meetingscene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MeetingThread_t {
    QByteArrayData data[7];
    char stringdata0[66];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MeetingThread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MeetingThread_t qt_meta_stringdata_MeetingThread = {
    {
QT_MOC_LITERAL(0, 0, 13), // "MeetingThread"
QT_MOC_LITERAL(1, 14, 9), // "sendImage"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 3), // "msg"
QT_MOC_LITERAL(4, 29, 12), // "sendfacename"
QT_MOC_LITERAL(5, 42, 4), // "name"
QT_MOC_LITERAL(6, 47, 18) // "sendupdateplaylist"

    },
    "MeetingThread\0sendImage\0\0msg\0sendfacename\0"
    "name\0sendupdateplaylist"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MeetingThread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    1,   32,    2, 0x06 /* Public */,
       6,    1,   35,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QImage,    3,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void MeetingThread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MeetingThread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 1: _t->sendfacename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->sendupdateplaylist((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MeetingThread::*)(QImage );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MeetingThread::sendImage)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MeetingThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MeetingThread::sendfacename)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MeetingThread::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MeetingThread::sendupdateplaylist)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MeetingThread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_MeetingThread.data,
    qt_meta_data_MeetingThread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MeetingThread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeetingThread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MeetingThread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int MeetingThread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void MeetingThread::sendImage(QImage _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MeetingThread::sendfacename(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MeetingThread::sendupdateplaylist(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_MeetingScene_t {
    QByteArrayData data[19];
    char stringdata0[263];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MeetingScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MeetingScene_t qt_meta_stringdata_MeetingScene = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MeetingScene"
QT_MOC_LITERAL(1, 13, 10), // "closeEvent"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 12), // "QCloseEvent*"
QT_MOC_LITERAL(4, 38, 5), // "event"
QT_MOC_LITERAL(5, 44, 24), // "on_AddMeeting_bt_clicked"
QT_MOC_LITERAL(6, 69, 24), // "on_MeetingList_activated"
QT_MOC_LITERAL(7, 94, 4), // "arg1"
QT_MOC_LITERAL(8, 99, 11), // "acceptImage"
QT_MOC_LITERAL(9, 111, 3), // "msg"
QT_MOC_LITERAL(10, 115, 14), // "acceptfacename"
QT_MOC_LITERAL(11, 130, 15), // "acceptplayImage"
QT_MOC_LITERAL(12, 146, 12), // "TimerTimeOut"
QT_MOC_LITERAL(13, 159, 18), // "UpdateTableTimeOut"
QT_MOC_LITERAL(14, 178, 20), // "acceptupdateplaylist"
QT_MOC_LITERAL(15, 199, 19), // "on_PlayList_clicked"
QT_MOC_LITERAL(16, 219, 11), // "QModelIndex"
QT_MOC_LITERAL(17, 231, 5), // "index"
QT_MOC_LITERAL(18, 237, 25) // "on_PlayList_doubleClicked"

    },
    "MeetingScene\0closeEvent\0\0QCloseEvent*\0"
    "event\0on_AddMeeting_bt_clicked\0"
    "on_MeetingList_activated\0arg1\0acceptImage\0"
    "msg\0acceptfacename\0acceptplayImage\0"
    "TimerTimeOut\0UpdateTableTimeOut\0"
    "acceptupdateplaylist\0on_PlayList_clicked\0"
    "QModelIndex\0index\0on_PlayList_doubleClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MeetingScene[] = {

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
       1,    1,   69,    2, 0x08 /* Private */,
       5,    0,   72,    2, 0x08 /* Private */,
       6,    1,   73,    2, 0x08 /* Private */,
       8,    1,   76,    2, 0x08 /* Private */,
      10,    1,   79,    2, 0x08 /* Private */,
      11,    1,   82,    2, 0x08 /* Private */,
      12,    0,   85,    2, 0x08 /* Private */,
      13,    0,   86,    2, 0x08 /* Private */,
      14,    1,   87,    2, 0x08 /* Private */,
      15,    1,   90,    2, 0x08 /* Private */,
      18,    1,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QImage,    9,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, QMetaType::QImage,    9,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    9,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 16,   17,

       0        // eod
};

void MeetingScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MeetingScene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 1: _t->on_AddMeeting_bt_clicked(); break;
        case 2: _t->on_MeetingList_activated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->acceptImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 4: _t->acceptfacename((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->acceptplayImage((*reinterpret_cast< QImage(*)>(_a[1]))); break;
        case 6: _t->TimerTimeOut(); break;
        case 7: _t->UpdateTableTimeOut(); break;
        case 8: _t->acceptupdateplaylist((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 9: _t->on_PlayList_clicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 10: _t->on_PlayList_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MeetingScene::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_MeetingScene.data,
    qt_meta_data_MeetingScene,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MeetingScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MeetingScene::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MeetingScene.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int MeetingScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
