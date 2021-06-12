#pragma once

#include <QtCore/qglobal.h>

#if defined(UTIL_LIBRARY)
#define UTIL_EXPORT Q_DECL_EXPORT
#else
#define UTIL_EXPORT Q_DECL_IMPORT
#endif
