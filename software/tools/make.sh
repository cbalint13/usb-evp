#!/bin/sh

CFLAGS="-O3 -g -Wall -I."

rm -rf bin build
mkdir -p bin build

#
# Libs
#

gcc $CFLAGS -c ./devs/lora.c    -o build/lora.o
gcc $CFLAGS -c ./devs/mcp2210.c -o build/mcp2210.o
gcc $CFLAGS -c ./drvs/spi.c     -o build/spi.o

c++ $CFLAGS -c ./comm/comm.cpp  -o build/comm.o

#
# Tools
#

# SX127x version checker example
gcc $CFLAGS src/check-version.c -o bin/check-version \
            build/mcp2210.o

# SX127x random number generator example
gcc $CFLAGS src/sx127x-random.c -o bin/sx127x-random \
            build/lora.o build/spi.o build/mcp2210.o

# LoRA packet receiver example
c++ $CFLAGS src/lora-receive.cpp -o bin/lora-receive \
            build/lora.o build/spi.o build/mcp2210.o build/comm.o

# LoRA packet sender example
c++ $CFLAGS src/lora-send.cpp -o bin/lora-send \
            build/lora.o build/spi.o build/mcp2210.o build/comm.o
