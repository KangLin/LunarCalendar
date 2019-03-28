
#ifndef LUNAR_CALENDAR_EXPORT_LINUX_H
#define LUNAR_CALENDAR_EXPORT_LINUX_H

#ifdef LUNAR_CALENDAR_STATIC_DEFINE
#  define LUNAR_CALENDAR_EXPORT
#  define LUNAR_CALENDAR_NO_EXPORT
#else
#  ifndef LUNAR_CALENDAR_EXPORT
#    ifdef LUNAR_CALENDAR_EXPORTS
        /* We are building this library */
#      define LUNAR_CALENDAR_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define LUNAR_CALENDAR_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef LUNAR_CALENDAR_NO_EXPORT
#    define LUNAR_CALENDAR_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef LUNAR_CALENDAR_DEPRECATED
#  define LUNAR_CALENDAR_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef LUNAR_CALENDAR_DEPRECATED_EXPORT
#  define LUNAR_CALENDAR_DEPRECATED_EXPORT LUNAR_CALENDAR_EXPORT LUNAR_CALENDAR_DEPRECATED
#endif

#ifndef LUNAR_CALENDAR_DEPRECATED_NO_EXPORT
#  define LUNAR_CALENDAR_DEPRECATED_NO_EXPORT LUNAR_CALENDAR_NO_EXPORT LUNAR_CALENDAR_DEPRECATED
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define LUNAR_CALENDAR_NO_DEPRECATED
#endif

#endif
