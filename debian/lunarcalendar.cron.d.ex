#
# Regular cron jobs for the lunarcalendar package
#
0 4	* * *	root	[ -x /usr/bin/lunarcalendar_maintenance ] && /usr/bin/lunarcalendar_maintenance
