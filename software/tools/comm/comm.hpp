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

#ifndef _COMM_HPP_
#define _COMM_HPP_

#include <stdint.h>
#include <random>

#include <devs/lora.h>

#define LED_ON  true
#define LED_OFF false
#define LED_RX  8 // gpio8
#define LED_TX  7 // gpio7

extern int _fd;

class LORA
{
    public:
        LORA(char hiddevice[], uint8_t headerMode, int8_t txpower,
           uint32_t freq, uint8_t bandwidth, uint8_t codingrate4,
           uint8_t spreadfactor, uint8_t crcpayload, uint8_t syncword,
           uint8_t preamblelen, uint8_t maxlength);
        ~LORA();

        // recv lora package
        uint8_t rx_packet(uint8_t *msg_buf);

        // send lora package
        void tx_packet(uint8_t *msg_buf, const uint8_t msg_len);

        // GP6 RX int counter
        uint32_t rx_count(void);

        // Read RF values
        int8_t rssi(void);
        int8_t snr(void);

    protected:
        // control RX/TX LED
        void led(uint8_t rx_tx, bool value);

    private:
        uint8_t loraState;

};

#endif // _COMM_HPP_
