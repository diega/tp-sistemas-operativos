#!/bin/sh
WORKING_KERNEL_VERSION="2.6.26-1-686"
CURRENT_KERNEL_VERSION=`uname -r`

if [ $CURRENT_KERNEL_VERSION != $WORKING_KERNEL_VERSION ]
then
echo el módulo está pensado para funcionar con la version $WORKING_KERNEL_VERSION
return 
fi

if [ ! -e /proc/keyboard_leds ]
then
echo el módulo no esta cargado
return
fi

if [ ! -x print_kbleds ]
then
echo corra el comando make para armar el proyecto
return
fi

echo Enciendo Num Lock
echo 1 > /proc/keyboard_leds
./print_kbleds
