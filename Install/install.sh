#!/bin/bash

# Use to install appimage in linux

case "$1" in
    remove)
        echo "remove ..."
        rm -f /usr/share/applications/LunarCalendar.desktop
        rm -f /usr/share/pixmaps/LunarCalendar.png
    ;;
    
    install|*)
        echo "install ..."
        if [ -f /usr/share/applications/LunarCalendar.desktop ]; then
            rm /usr/share/applications/LunarCalendar.desktop
        fi
       	ln -s `pwd`/share/applications/LunarCalendar.desktop /usr/share/applications/LunarCalendar.desktop
        sed -i "s/Exec=.*//g" `pwd`/share/applications/LunarCalendar.desktop
        echo "Exec=`pwd`/LunarCalendar-x86_64.AppImage" >> `pwd`/share/applications/LunarCalendar.desktop
        
        if [ -f /usr/share/pixmaps/calendar.png ]; then
            rm /usr/share/pixmaps/calendar.png
        fi
        if [ ! -d /usr/share/pixmaps ]; then
            mkdir -p /usr/share/pixmaps
        fi
        ln -s `pwd`/share/pixmaps/LunarCalendar.png /usr/share/pixmaps/LunarCalendar.png
        ;;
        
esac
