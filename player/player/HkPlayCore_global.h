#ifndef HKPLAYCORE_GLOBAL_H
#define HKPLAYCORE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(HKPLAYCORE_LIBRARY)
#  define HKPLAYCORE_EXPORT Q_DECL_EXPORT
#else
#  define HKPLAYCORE_EXPORT Q_DECL_IMPORT
#endif

#endif // HKPLAYCORE_GLOBAL_H