
#ifndef LUNAR_CALENDAR_EXPORT_WINDOWS_H
#define LUNAR_CALENDAR_EXPORT_WINDOWS_H

#ifdef LUNAR_CALENDAR_STATIC_DEFINE
#  define LUNAR_CALENDAR_EXPORT
#  define LUNAR_CALENDAR_NO_EXPORT
#else
#  ifndef LUNAR_CALENDAR_EXPORT
#    ifdef LUNAR_CALENDAR_EXPORTS
        /* We are building this library */
#      define LUNAR_CALENDAR_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define LUNAR_CALENDAR_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef LUNAR_CALENDAR_NO_EXPORT
#    define LUNAR_CALENDAR_NO_EXPORT 
#  endif
#endif

#ifndef LUNAR_CALENDAR_DEPRECATED
#  define LUNAR_CALENDAR_DEPRECATED __declspec(deprecated)
#endif

#ifndef LUNAR_CALENDAR_DEPRECATED_EXPORT
#  define LUNAR_CALENDAR_DEPRECATED_EXPORT LUNAR_CALENDAR_EXPORT LUNAR_CALENDAR_DEPRECATED
#endif

#ifndef LUNAR_CALENDAR_DEPRECATED_NO_EXPORT
#  define LUNAR_CALENDAR_DEPRECATED_NO_EXPORT LUNAR_CALENDAR_NO_EXPORT LUNAR_CALENDAR_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LUNAR_CALENDAR_NO_DEPRECATED
#    define LUNAR_CALENDAR_NO_DEPRECATED
#  endif
#endif

#endif /* LUNAR_CALENDAR_EXPORT_WINDOWS_H */
