#ifndef QUGEL_GLOBAL_H
#define QUGEL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(QUGEL_LIBRARY)
#  define QUGELSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QUGELSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QUGEL_GLOBAL_H
