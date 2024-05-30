#!/bin/bash

FILES=/dev/hidraw*
for f in $FILES
do
  FILE=${f##*/}
  DEVICE="$(cat /sys/class/hidraw/${FILE}/device/uevent | grep HID_NAME | cut -d '=' -f2)"
  printf "/dev/%s \t %s\n" $FILE "$DEVICE"
done
