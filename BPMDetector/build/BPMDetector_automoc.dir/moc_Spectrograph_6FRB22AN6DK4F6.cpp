/****************************************************************************
** Meta object code from reading C++ file 'Spectrograph.hpp'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../Gui/Spectrograph.hpp"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'Spectrograph.hpp' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_Spectrograph_t {
    QByteArrayData data[9];
    char stringdata0[97];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_Spectrograph_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_Spectrograph_t qt_meta_stringdata_Spectrograph = {
    {
QT_MOC_LITERAL(0, 0, 12), // "Spectrograph"
QT_MOC_LITERAL(1, 13, 12), // "info_message"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 7), // "message"
QT_MOC_LITERAL(4, 35, 11), // "interval_ms"
QT_MOC_LITERAL(5, 47, 5), // "reset"
QT_MOC_LITERAL(6, 53, 16), // "spectrum_changed"
QT_MOC_LITERAL(7, 70, 17), // "FrequencySpectrum"
QT_MOC_LITERAL(8, 88, 8) // "spectrum"

    },
    "Spectrograph\0info_message\0\0message\0"
    "interval_ms\0reset\0spectrum_changed\0"
    "FrequencySpectrum\0spectrum"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_Spectrograph[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   34,    2, 0x0a /* Public */,
       6,    1,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::UInt,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void Spectrograph::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Spectrograph *_t = static_cast<Spectrograph *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->info_message((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< uint(*)>(_a[2]))); break;
        case 1: _t->reset(); break;
        case 2: _t->spectrum_changed((*reinterpret_cast< const FrequencySpectrum(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (Spectrograph::*_t)(const QString & , unsigned int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&Spectrograph::info_message)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject Spectrograph::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_Spectrograph.data,
      qt_meta_data_Spectrograph,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *Spectrograph::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *Spectrograph::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_Spectrograph.stringdata0))
        return static_cast<void*>(const_cast< Spectrograph*>(this));
    return QWidget::qt_metacast(_clname);
}

int Spectrograph::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void Spectrograph::info_message(const QString & _t1, unsigned int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
