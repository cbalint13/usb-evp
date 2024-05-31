/*
 * Copyright (C) 2019  Balint Cristian <crisitan dot balint at gmail dot com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <cstring>

#include "comm/comm.hpp"


int main(int argc, char **argv)
{

    if (argc < 2) {
          printf("Usage: %s /dev/hidrawX\n", argv[0]);
          exit(1);
    }

    const int8_t txpower = 10;
    const uint8_t headerMode = LR_EXPLICIT_HEADER_MODE;
    const uint32_t freq = 869500000;
    const uint8_t bandwidth = LR_BW_125k;
    const uint8_t codingrate4 = LR_CODINGRATE_1p75;
    const uint8_t spreadfactor = LR_SPREADING_FACTOR_9;
    const uint8_t crcpayload = LR_PAYLOAD_CRC_ON;
    const uint8_t syncword = 18;
    const uint8_t preamblelen = 8;
    const uint8_t maxlength = 250;

    // LoRa modem
    LORA lora(argv[1], headerMode, txpower, freq, bandwidth, codingrate4,
              spreadfactor, crcpayload, syncword, preamblelen, maxlength);

    // message block
    const char msg[] = "A lora message";

    printf("LoRa sent message.\n");
    lora.tx_packet((uint8_t *)msg, strlen(msg));

    return 0;
}
