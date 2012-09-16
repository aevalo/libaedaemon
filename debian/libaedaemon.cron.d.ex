#
# Regular cron jobs for the libaedaemon package
#
0 4	* * *	root	[ -x /usr/bin/libaedaemon_maintenance ] && /usr/bin/libaedaemon_maintenance
