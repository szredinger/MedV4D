/****************************************************************************
** Meta object code from reading C++ file 'StManagerStudyListComp.h'
**
** Created: Sun Apr 20 16:37:32 2008
**      by: The Qt Meta Object Compiler version 59 (Qt 4.3.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../widgets/components/StManagerStudyListComp.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'StManagerStudyListComp.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_StManagerStudyListComp[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       0,    0, // enums/sets

       0        // eod
};

static const char qt_meta_stringdata_StManagerStudyListComp[] = {
    "StManagerStudyListComp\0"
};

const QMetaObject StManagerStudyListComp::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_StManagerStudyListComp,
      qt_meta_data_StManagerStudyListComp, 0 }
};

const QMetaObject *StManagerStudyListComp::metaObject() const
{
    return &staticMetaObject;
}

void *StManagerStudyListComp::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_StManagerStudyListComp))
	return static_cast<void*>(const_cast< StManagerStudyListComp*>(this));
    return QWidget::qt_metacast(_clname);
}

int StManagerStudyListComp::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
