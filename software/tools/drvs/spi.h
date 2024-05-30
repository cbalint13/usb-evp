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

#define SPI_WNR 0x80 // write command

#ifdef __cplusplus
extern "C" {
#endif

extern int _fd;

void SPIConfig(void);
uint8_t SPIReadReg(uint8_t addr);
void SPIWriteReg(uint8_t addr, uint8_t data);
void SPIBurstRead(uint8_t addr, uint8_t *ptr, uint8_t len);
void SPIBurstWrite(uint8_t addr, uint8_t *ptr, uint8_t len);

#ifdef __cplusplus
}
#endif
