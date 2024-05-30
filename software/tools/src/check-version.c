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

#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "devs/lora.h"
#include "devs/mcp2210.h"

int main(int argc, char **argv)
{
    if (argc < 2) {
          printf("Usage: %s /dev/hidrawX\n", argv[0]);
          exit(1);
    }

    int fd;
    int err;

    fd = open(argv[1], O_RDWR);

    mcp2210_packet packet = { 0, };
    // SPI_GET
    err = mcp2210_command(fd, packet, MCP2210_SPI_GET);
    if (err < 0) {
      fprintf(stderr, "Error: %s\n", mcp2210_strerror(err));
      close(fd);
      return -1;
    }
    mcp2210_spi_set_transaction_size(packet, 2);
    mcp2210_command(fd, packet, MCP2210_SPI_SET);
    // SPI transaction
    uint8_t data[2] = {LR_RegVERSION, 0xFF};
    mcp2210_spi_transfer(fd, packet, data, 2);

    printf("SX127X chip version: 0x%02x\n", data[1]);

    close(fd);

    return 0;
}
