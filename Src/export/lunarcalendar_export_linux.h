
#ifndef LUNARCALENDAR_EXPORT_LINUX_H
#define LUNARCALENDAR_EXPORT_LINUX_H

#ifdef LUNARCALENDAR_STATIC_DEFINE
#  define LUNARCALENDAR_EXPORT
#  define LUNARCALENDAR_NO_EXPORT
#else
#  ifndef LUNARCALENDAR_EXPORT
#    ifdef LunarCalendar_EXPORTS
        /* We are building this library */
#      define LUNARCALENDAR_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define LUNARCALENDAR_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef LUNARCALENDAR_NO_EXPORT
#    define LUNARCALENDAR_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef LUNARCALENDAR_DEPRECATED
#  define LUNARCALENDAR_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef LUNARCALENDAR_DEPRECATED_EXPORT
#  define LUNARCALENDAR_DEPRECATED_EXPORT LUNARCALENDAR_EXPORT LUNARCALENDAR_DEPRECATED
#endif

#ifndef LUNARCALENDAR_DEPRECATED_NO_EXPORT
#  define LUNARCALENDAR_DEPRECATED_NO_EXPORT LUNARCALENDAR_NO_EXPORT LUNARCALENDAR_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LUNARCALENDAR_NO_DEPRECATED
#    define LUNARCALENDAR_NO_DEPRECATED
#  endif
#endif

#endif //LUNARCALENDAR_EXPORT_LINUX_H
