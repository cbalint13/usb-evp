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
#include <stdio.h>
#include <unistd.h>
#include <random>

#include "drvs/spi.h"
#include "devs/lora.h"
#include "devs/mcp2210.h"

#include "comm/comm.hpp"

extern uint8_t _headerMode;
extern uint8_t _payloadLength;

LORA::LORA(char hiddevice[],
           uint8_t headerMode = LR_EXPLICIT_HEADER_MODE,
           int8_t txpower = 2,
           uint32_t freq = 868000000,
           uint8_t bandwidth = LR_BW_500k,
           uint8_t codingrate4 = LR_CODINGRATE_2,
           uint8_t spreadfactor = LR_SPREADING_FACTOR_7,
           uint8_t crcpayload = LR_PAYLOAD_CRC_ON,
           uint8_t syncword = 13,
           uint8_t preamblelen = 8,
           uint8_t maxlength = 250)
{
    // set global header mode
    _headerMode = headerMode;
    _payloadLength = maxlength;

    // open USBRAW device
    _fd = open(hiddevice, O_RDWR);

    // reset lora
    lora_reset();
    // settings mode require sleep
    lora_enterSleepMode();
    // set power dBm
    // -3..1, 2..17, 20
    lora_setTxPower(txpower);
    // close ocp
    SPIWriteReg(LR_RegOcp, LR_OCPON_ON | 0x0B);
    // enable LNA
    SPIWriteReg(LR_RegLna, LR_LNAGAIN_G1
                         | LR_LNABOOSTHF_1);
    // bandwidth
    // coding rate
    // spreading factor
    // implict header mode
    lora_setRFparams(freq,
                     bandwidth,
                     codingrate4,
                     spreadfactor,
                     crcpayload);

    // Low datarate schedule
    //SPIWriteReg(LR_RegModemConfig3, LR_LOW_DATARATE_OPTIMIZE);
    // Sync Word 0x13
    SPIWriteReg(LR_RegSyncWord, syncword);
    // max rx time out
    lora_setRxTimeOut(0x05);
    // preamble 12+4.25 bytes
    lora_setPreambleLen(preamblelen);
    // max payload length (250 bytes)
    SPIWriteReg(LR_RegMaxPayloadLength, maxlength);
    lora_disableInvertIQ();

    // switch states
    lora_enterStbyMode();
    lora_enterSleepMode();

    // set state
    LORA::loraState = LR_Mode_SLEEP;
}

LORA::~LORA(void)
{
    close(_fd);
}

uint8_t LORA::rx_packet(uint8_t *msg_buf)
{
    uint8_t rx_len = 0;

    led(LED_TX, LED_OFF);
    led(LED_RX, LED_OFF);

    // no rx mode reinit
    if (LORA::loraState != LR_Mode_RXCONTINUOUS)
    {
        lora_rxInit(LR_Mode_RXCONTINUOUS, 0);
        LORA::loraState = LR_Mode_RXCONTINUOUS;
    }

    // soft poll IRQ
    while (true)
    {
        // check IRQ status
        uint8_t flag = SPIReadReg(LR_RegIrqFlags);

        // rx CRC error
        if(flag & LR_RXPCRCERROR_MASK)
        {
            printf("LoRa RX CRC error.\n");
            lora_clrInterrupt();
        }
        // rx TIMEOUT error
        if(flag & LR_RXTIMEOUT_MASK)
        {
            printf("LoRa RX TIMEOUT error.\n");
            lora_clrInterrupt();
        }
        // rx DONE
        if(flag == LR_RXDONE_MASK)
        {
            led(LED_RX, LED_ON);
            // fetch len & data
            rx_len = lora_rxPacket(msg_buf);
            // clear IRQ
            lora_clrInterrupt();

            break;
        }
        // 10ms wait
        usleep(50000);
    }

    led(LED_RX, LED_OFF);
    return rx_len;
}

void LORA::tx_packet(uint8_t *msg_buf, const uint8_t msg_len)
{
    // send packet
    led(LED_TX, LED_ON);
    lora_txPacket(msg_buf, msg_len, LORA_TX_WAITACK);
    LORA::loraState = LR_Mode_SLEEP;
    led(LED_TX, LED_OFF);
}

int8_t LORA::rssi(void)
{
   return (-137 + lora_readRSSI(LORA_RSSI_LAST));
}

int8_t LORA::snr(void)
{
   return (lora_readSNR() / 4);
}

void LORA::led(uint8_t rx_tx, bool value)
{
    if ((rx_tx < 7) || (rx_tx > 8)) return;
    mcp2210_packet packet = {0, };

    mcp2210_command(_fd, packet, MCP2210_GPIO_DIR_GET);
    mcp2210_chip_set_default_direction(packet, rx_tx, 1);
    mcp2210_command(_fd, packet, MCP2210_GPIO_DIR_SET);

    mcp2210_command(_fd, packet, MCP2210_GPIO_VAL_GET);
    mcp2210_gpio_set_pin(packet, rx_tx, value);
    mcp2210_command(_fd, packet, MCP2210_GPIO_VAL_SET);
}

uint32_t LORA::rx_count(void)
{
    mcp2210_packet packet = {0, };
    return mcp2210_gp6_count_get(_fd, packet, true /*no reset*/);
}
