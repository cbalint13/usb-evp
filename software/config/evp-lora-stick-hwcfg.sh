#!/bin/sh

#
# USB eVp Stick hardware initial configurator.
# Copyright (C) 2019,2021  Cristian Balint <cbalint@gmail.com>
#
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Use mcp2210-util (https://github.com/lkundrak/mcp2210)

if [ ${#@} -eq 0 ]
then
  echo
  echo "Usage:"
  echo "$0 /dev/hidrawX"
fi

echo "Using $1"

./mcp2210-util $1 --both --spi-mode 0
./mcp2210-util $1 --both --bit-rate 12000000

./mcp2210-util $1 --both --tx-size 2
./mcp2210-util $1 --both --byte-delay 0
./mcp2210-util $1 --both --cs-to-data-delay 0
./mcp2210-util $1 --both --data-to-cs-delay 0

./mcp2210-util $1 --both --vendor-id 0x04d8
./mcp2210-util $1 --both --product-id 0x00de
./mcp2210-util $1 --both --usb-product '[eVo] USB Stick v1.2'
./mcp2210-util $1 --both --usb-manufacturer 'EventPacket'

./mcp2210-util $1 --both --gp6-count-rising

./mcp2210-util $1 --both --default-off 8
./mcp2210-util $1 --both --default-off 7
./mcp2210-util $1 --both --default-off 6
./mcp2210-util $1 --both --default-off 5
./mcp2210-util $1 --both --default-off 4
./mcp2210-util $1 --both --default-off 3
./mcp2210-util $1 --both --default-off 2
./mcp2210-util $1 --both --default-off 1
./mcp2210-util $1 --both --default-on 0 # nSS

./mcp2210-util $1 --both --default-out 8 # LED-Rx
./mcp2210-util $1 --both --default-out 7 # LED-Tx
./mcp2210-util $1 --both --default-in 6  # DIO6
./mcp2210-util $1 --both --default-in 5  # DIO5
./mcp2210-util $1 --both --default-in 4  # DIO4
./mcp2210-util $1 --both --default-in 3  # DIO3
./mcp2210-util $1 --both --default-in 2  # DIO2
./mcp2210-util $1 --both --default-in 1  # DIO1
./mcp2210-util $1 --both --default-out 0 # nSS

./mcp2210-util $1 --both --gpio 8
./mcp2210-util $1 --both --gpio 7
./mcp2210-util $1 --both --func 6
./mcp2210-util $1 --both --gpio 5
./mcp2210-util $1 --both --gpio 4
./mcp2210-util $1 --both --gpio 3
./mcp2210-util $1 --both --gpio 2
./mcp2210-util $1 --both --gpio 1
./mcp2210-util $1 --both --cs 0

./mcp2210-util $1 --both --idle-cs-off 8
./mcp2210-util $1 --both --idle-cs-off 7
./mcp2210-util $1 --both --idle-cs-off 6
./mcp2210-util $1 --both --idle-cs-off 5
./mcp2210-util $1 --both --idle-cs-off 4
./mcp2210-util $1 --both --idle-cs-off 3
./mcp2210-util $1 --both --idle-cs-off 2
./mcp2210-util $1 --both --idle-cs-off 1
./mcp2210-util $1 --both --idle-cs-off 0

./mcp2210-util $1 --both --active-cs-off 8
./mcp2210-util $1 --both --active-cs-off 7
./mcp2210-util $1 --both --active-cs-off 6
./mcp2210-util $1 --both --active-cs-off 5
./mcp2210-util $1 --both --active-cs-off 4
./mcp2210-util $1 --both --active-cs-off 3
./mcp2210-util $1 --both --active-cs-off 2
./mcp2210-util $1 --both --active-cs-off 1
./mcp2210-util $1 --both --active-cs-on 0

