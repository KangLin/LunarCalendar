#!/bin/bash

EXIT_CODE=0
PROJECT_NAME="LunarCalendar"

if [ -n "$1" ]; then
    echo "$PROJECT_NAME"
	PROJECT_NAME=$1
fi

if [ ! -f /opt/share/applications/${PROJECT_NAME}.desktop ]; then
	echo "There are not /opt/share/applications/${PROJECT_NAME}.desktop"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /usr/share/applications/${PROJECT_NAME}.desktop ]; then
	echo "There are not /usr/share/applications/${PROJECT_NAME}.desktop"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /opt/share/pixmaps/${PROJECT_NAME}.png ]; then
	echo "There are not /opt/share/pixmaps/${PROJECT_NAME}.png"
	EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /usr/share/pixmaps/${PROJECT_NAME}.png ]; then
    echo "There are not /usr/share/pixmaps/${PROJECT_NAME}.png"
    EXIT_CODE=$[EXIT_CODE+1]
fi

if [ ! -f /etc/ld.so.conf.d/LunarCalendar.conf ]; then
	echo "There are not /etc/ld.so.conf.d/${PROJECT_NAME}.conf"
	EXIT_CODE=$[EXIT_CODE+1]
fi

exit $EXIT_CODE
