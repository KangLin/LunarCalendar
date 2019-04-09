#ifndef LUNARCALENDAR_EXPORT_H
#define LUNARCALENDAR_EXPORT_H

#if defined (Q_OS_WIN)
	#include "lunarcalendar_export_windows.h"
#endif

#if defined(Q_OS_LINUX)
    #include "lunarcalendar_export_linux.h"
#endif

#endif //LUNARCALENDAR_EXPORT_H
