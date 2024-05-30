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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "devs/mcp2210.h"
#include "drvs/spi.h"

int _fd;

void SPIConfig(void) {}

uint8_t SPIReadReg(uint8_t addr)
{
    // two byte transaction
    uint8_t rw[2] = {addr, 0xFF};

    // SPI_GET command
    mcp2210_packet packet = { 0, };
    mcp2210_command(_fd, packet, MCP2210_SPI_GET);
    // SPI transfer
    mcp2210_spi_transfer(_fd, packet, rw, 2);

    return rw[1];
}

void SPIWriteReg(uint8_t addr, uint8_t value)
{
    // two byte transaction
    uint8_t rw[2] = {addr | SPI_WNR, value};

    // SPI_GET command
    mcp2210_packet packet = { 0, };
    mcp2210_command(_fd, packet, MCP2210_SPI_GET);
    // SPI transfer
    mcp2210_spi_transfer(_fd, packet, rw, 2);
}

void SPIBurstRead(uint8_t addr, uint8_t *ptr, uint8_t len)
{
    if (len <= 1)
        return;

    uint8_t *rw = (uint8_t *)malloc(len + 1);

    rw[0] = addr;
    for (uint8_t i = 0; i < len; i++)
        rw[i + 1] = 0xFF;

    // SPI_GET command
    mcp2210_packet packet = {0, };
    mcp2210_command(_fd, packet, MCP2210_SPI_GET);
    mcp2210_spi_set_transaction_size(packet, len + 1);
    mcp2210_command(_fd, packet, MCP2210_SPI_SET);
    // SPI transfer
    mcp2210_spi_transfer(_fd, packet, rw, len + 1);

    for (uint8_t i = 0; i < len; i++)
      ptr[i] = rw[i + 1];

    free(rw);

    mcp2210_command(_fd, packet, MCP2210_SPI_GET);
    mcp2210_spi_set_transaction_size(packet, 2);
    mcp2210_command(_fd, packet, MCP2210_SPI_SET);
}

void SPIBurstWrite(uint8_t addr, uint8_t *ptr, uint8_t len)
{
    if (len <= 1)
        return;

    uint8_t *rw = (uint8_t *)malloc(len + 1);

    rw[0] = addr | SPI_WNR;
    for (uint8_t i = 0; i < len; i++)
        rw[i + 1] = ptr[i];

    // SPI_GET command
    mcp2210_packet packet = { 0, };
    mcp2210_command(_fd, packet, MCP2210_SPI_GET);
    mcp2210_spi_set_transaction_size(packet, len + 1);
    mcp2210_command(_fd, packet, MCP2210_SPI_SET);
    // SPI transfer
    mcp2210_spi_transfer(_fd, packet, rw, len + 1);

    free(rw);

    mcp2210_command(_fd, packet, MCP2210_SPI_GET);
    mcp2210_spi_set_transaction_size(packet, 2);
    mcp2210_command(_fd, packet, MCP2210_SPI_SET);
}
