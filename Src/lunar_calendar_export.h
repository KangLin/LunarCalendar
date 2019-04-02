#ifndef TASKS_EXPORT_H
#define TASKS_EXPORT_H

#if defined (Q_OS_WIN)
	#include "lunarcalendar_export_windows.h"
#endif

#if defined(Q_OS_LINUX)
	#include "lunar_calendar_export_linux.h"
#endif

#endif
