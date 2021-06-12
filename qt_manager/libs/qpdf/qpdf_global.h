#pragma once

#include <QtCore/qglobal.h>

#if defined(QPDF_LIBRARY)
#  define QPDF_EXPORT Q_DECL_EXPORT
#else
#  define QPDF_EXPORT Q_DECL_IMPORT
#endif
