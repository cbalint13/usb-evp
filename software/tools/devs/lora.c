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

#include <stdio.h>

#include <stdint.h>
#include <unistd.h>

#include "drvs/spi.h"
#include "devs/lora.h"


uint8_t _headerMode;
uint8_t _payloadLength;

void lora_init(uint8_t headerMode)
{
    // reset lora
    lora_reset();
}

void lora_reset(void)
{
    /* not implemented */
}

void lora_config(uint8_t headerMode)
{
    // set global header mode
    _headerMode = headerMode;

    // settings mode require sleep
    lora_enterSleepMode();

    // set power dBm
    // -3..1, 2..17, 20
    lora_setTxPower(2);

    // close ocp
    SPIWriteReg(LR_RegOcp, LR_OCPON_ON | 0x0B);

    // enable LNA
    SPIWriteReg(LR_RegLna, LR_LNAGAIN_G1
                         | LR_LNABOOSTHF_1);
    // bandwidth
    // coding rate
    // spreading factor
    // implict header mode
    lora_setRFparams(868000000,
                     LR_BW_500k,
                     LR_CODINGRATE_2,
                     LR_SPREADING_FACTOR_7,
                     LR_PAYLOAD_CRC_ON);

    // Sync Word 0x13
    SPIWriteReg(LR_RegSyncWord, 0x13);

    // max rx time out
    lora_setRxTimeOut(0x05);

    // preamble 12+4.25 bytes
    lora_setPreambleLen(13);

    // max payload length (250 bytes)
    SPIWriteReg(LR_RegMaxPayloadLength, 250);

    // switch states
    lora_enterStbyMode();
    lora_enterSleepMode();
}

void lora_setFrequency(uint32_t freq)
{
    uint8_t reg[3];
    /*
     * Frf(23:0)=frequency/(XOSC/2^19)
     */
    uint64_t frf = ((uint64_t)freq << 19) / LORA_XOSC;

    reg[0] = (uint8_t)(frf >> 16);
    reg[1] = (uint8_t)(frf >>  8);
    reg[2] = (uint8_t)(frf >>  0);

    SPIWriteReg(LR_RegFrMsb, reg[0]);
    SPIWriteReg(LR_RegFrMid, reg[1]);
    SPIWriteReg(LR_RegFrLsb, reg[2]);
}

uint32_t lora_setRFerrata(uint32_t freq, uint8_t BW)
{
    // only for silicon version 1b
    if (SPIReadReg(LR_RegVERSION) != 0x12)
        return freq;

    // sensitivity
    // for 500kHz bandwidth
    // see SX1276/77/78 Errata
    // section 2.1 for details
    SPIWriteReg(LR_RegHighBwOptimize1, 0x03);
    if (BW == LR_BW_500k)
    {
        if ((freq <= 525000000)
         && (freq >= 410000000))
        {
            SPIWriteReg(LR_RegHighBwOptimize1, 0x02);
            SPIWriteReg(LR_RegHighBwOptimize2, 0x7F);
        }
        if ((freq <= 1020000000)
         && (freq >=  862000000))
        {
            SPIWriteReg(LR_RegHighBwOptimize1, 0x02);
            SPIWriteReg(LR_RegHighBwOptimize2, 0x64);
        }
    }

    // receiver
    // spurious response
    // see SX1276/77/78 Errata
    // section 2.3 for details
    uint8_t val = SPIReadReg(LR_RegDetectOptimize);
    if (BW == LR_BW_500k)
        SPIWriteReg(LR_RegDetectOptimize, val |  (1 << 7));
    else
        SPIWriteReg(LR_RegDetectOptimize, val & ~(1 << 7));

    switch (BW)
    {
        case LR_BW_7p8k:
            SPIWriteReg(LR_RegIfFreq1, 0x48);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            freq += 7800;
            break;
        case LR_BW_10p4k:
            SPIWriteReg(LR_RegIfFreq1, 0x44);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            freq += 10400;
            break;
        case LR_BW_15p6k:
            SPIWriteReg(LR_RegIfFreq1, 0x44);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            freq += 15600;
            break;
        case LR_BW_20p8k:
            SPIWriteReg(LR_RegIfFreq1, 0x44);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            freq += 20800;
            break;
        case LR_BW_31p25k:
            SPIWriteReg(LR_RegIfFreq1, 0x44);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            freq += 31250;
            break;
        case LR_BW_41p7k:
            SPIWriteReg(LR_RegIfFreq1, 0x44);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            freq += 41700;
            break;
        case LR_BW_62p5k:
            SPIWriteReg(LR_RegIfFreq1, 0x40);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            break;
        case LR_BW_125k:
            SPIWriteReg(LR_RegIfFreq1, 0x40);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            break;
        case LR_BW_250k:
            SPIWriteReg(LR_RegIfFreq1, 0x40);
            SPIWriteReg(LR_RegIfFreq2, 0x00);
            break;
    }
    return freq;
}

void lora_setRFparams(uint32_t freq, uint8_t BW, uint8_t CR, uint8_t SF, uint8_t payloadCRC)
{
    uint8_t temp;

    temp = SPIReadReg(LR_RegModemConfig2);
    temp = temp & 0x03;

    // with SF = 6 selected
    // only implicit header mode is available
    if(SF == LR_SPREADING_FACTOR_6)
    {
        _headerMode = LR_IMPLICIT_HEADER_MODE;
        SPIWriteReg(LR_RegModemConfig1, BW | CR | _headerMode);
        SPIWriteReg(LR_RegModemConfig2, SF| payloadCRC| temp);

        // according to datasheet
        temp = SPIReadReg(LR_RegDetectOptimize);

        temp &= 0xF8;
        temp |= 0x05;

        SPIWriteReg(LR_RegDetectOptimize,     temp);
        SPIWriteReg(LR_RegDetectionThreshold, 0x0C);
    }
    else
    {
        SPIWriteReg(LR_RegModemConfig1, BW | CR | _headerMode );
        SPIWriteReg(LR_RegModemConfig2, SF | payloadCRC | temp);
    }

    // Low Data Rate optimizaition
    lora_setLdoFlag();

    // set erratas
    freq = lora_setRFerrata(freq, BW);

    // set frequency
    lora_setFrequency(freq);

}

void lora_setPreambleLen(uint16_t length)
{
    // the actual preamble len is length+4.25
    SPIWriteReg(LR_RegPreambleMsb, length   >> 8);
    SPIWriteReg(LR_RegPreambleLsb, length & 0xFF);
}

void lora_setHeaderMode(uint8_t mode)
{
    _headerMode = mode;

    uint8_t temp;
    // avoid overload the other setting
    temp = SPIReadReg(LR_RegModemConfig1);
    temp = temp & 0xFE;

    SPIWriteReg(LR_RegModemConfig1, temp | mode);
}

// in implict header mode, the payload length is fixed
// len thus need to set payload length first in this mode
void lora_setPayloadLength(uint8_t len)
{
    _payloadLength = len;
    SPIWriteReg(LR_RegPayloadLength, len);
}

// set TX power
// -2 to 17 or 20 [dBm]
void lora_setTxPower(int8_t power)
{
    if(power < -3)
    {
        return;
    }
    else if(power < 2)
    {
        // MaxPower = 10.8 + 0.6*2 (0x20) = 12dBm
        // Pout = MaxPower - ( 15 - OutputPower )
        SPIWriteReg(LR_RegPaConfig, LR_PASELECT_RFO      | 0x20 | (uint8_t)(power + 3));
        SPIWriteReg(LR_RegPADAC,    LR_20DB_OUTPUT_OFF);
    }
    else if((power >= 2) && (power <= 17))
    {
        // power is 2 - 17 dBm, enable PA1 + PA2 on PA_BOOST
        // Pout = 17 - ( 15 - OutputPower )
        SPIWriteReg(LR_RegPaConfig, LR_PASELECT_PA_BOOST | 0x70 | (uint8_t)(power - 2));
        SPIWriteReg(LR_RegPADAC,    LR_20DB_OUTPUT_OFF);
    }
    else if(power == 20)
    {
        // power is 20 dBm, enable PA1 + PA2 on PA_BOOST
        // and enable high power mode on LR_20DB_OUTPUT_ON + external
        SPIWriteReg(LR_RegPaConfig, LR_PASELECT_PA_BOOST | 0x70 | (uint8_t)(power - 5));
        SPIWriteReg(LR_RegPADAC,    LR_20DB_OUTPUT_ON
                                  | LR_REGPADAC_RESERVED);
    }
}

// only valid in rx single mode
// rxtimeout = symbTimeOut*(2^SF*BW)
void lora_setRxTimeOut(uint16_t symbTimeOut)
{
    uint8_t temp;

    temp = SPIReadReg(LR_RegModemConfig2);
    temp = temp & 0xFC;

    SPIWriteReg(LR_RegModemConfig2,   temp | (symbTimeOut >> 8 & 0x03));
    SPIWriteReg(LR_RegSymbTimeoutLsb, symbTimeOut & 0xFF              );
}

// RSSI[dBm] = -137 + rssi value
uint8_t lora_readRSSI(uint8_t mode)
{
    if(mode == LORA_RSSI_LAST)
        // read rssi of last packet received
        return SPIReadReg(LR_RegPktRssiValue);
    else
        // read current rssi
        return SPIReadReg(LR_RegRssiValue   );
}

// SN[dBm] = snr value / 4
int8_t lora_readSNR(void)
{
    // read snr of last packet received
    uint8_t snr = SPIReadReg(LR_RegPktSnrValue);

    return *(int8_t*) &snr;
}

void lora_rxInit(uint8_t rxMode, uint8_t recvLen)
{
    if(_headerMode == LR_IMPLICIT_HEADER_MODE)
        lora_setPayloadLength(recvLen);

    // enable RxDoneIrq
    lora_setRxInterrupt();
    // clear irq flag
    lora_clrInterrupt();
    // set FIFO addr
    lora_setFifoAddrPtr(LR_RegFifoRxBaseAddr);
    // start rx
    lora_enterRxMode(rxMode);
}

uint8_t lora_txPacket(uint8_t* sendbuf, uint8_t sendLen, uint8_t noWait)
{
    if(_headerMode == LR_IMPLICIT_HEADER_MODE)
        lora_setPayloadLength(sendLen);

    // enable TxDoneIrq
    lora_setTxInterrupt();
    // clear irq flag
    lora_clrInterrupt();
    // enable STBY
    lora_enterStbyMode();
    // fill FIFO
    lora_writeFifo(sendbuf, sendLen);
    // TX the data
    lora_enterTxMode();

    if (noWait == 0)
    {
        // you should make sure the tx timeout
        // is greater than the max time on air
        uint32_t txTimer = LORA_TX_TIMEOUT;

        while(txTimer--)
        {
            // wait for txdone
            if(lora_waitIrq(LR_TXDONE_MASK))
            {
                lora_enterSleepMode();
                lora_clrInterrupt();

                return 1;
            }
            usleep(200);
        }
        // if tx time out
        // reset lora module
        // never happen (20sec)
        //lora_init(_headerMode);
    }

    return 0;
}

uint8_t lora_rxPacket(uint8_t* recvbuf)
{
    uint8_t readLen = 0;

    // read length of packet
    if(_headerMode == LR_IMPLICIT_HEADER_MODE)
        readLen = _payloadLength;
    else
        readLen = SPIReadReg(LR_RegRxNbBytes);

    if (readLen > 0)
    {
        // read FIFO data
        lora_readFifo(recvbuf, readLen);
        // reset FIFO addr to base
        lora_setFifoAddrPtr(LR_RegFifoRxBaseAddr);
    }

    // read data from fifo
    return readLen;
}

uint8_t lora_waitIrq(uint8_t irqMask)
{
    uint8_t flag;
    // read irq flag
    flag = SPIReadReg(LR_RegIrqFlags);
    // if irq flag was set
    if(flag & irqMask)
        return 1;

    return 0;
}

void lora_setFifoAddrPtr(uint8_t addrReg)
{
    uint8_t addr;
    // read BaseAddr
    addr = SPIReadReg(addrReg);
    // BaseAddr->FifoAddrPtr
    SPIWriteReg(LR_RegFifoAddrPtr, addr);
}

void lora_enterRxMode(uint8_t rxMode)
{
    // enter rx mode
    SPIWriteReg(LR_RegOpMode, rxMode        | LORA_FREQUENCY_BAND | LR_LongRangeMode_LORA);
}

void lora_enterTxMode(void)
{
    // enter tx mode
    SPIWriteReg(LR_RegOpMode, LR_Mode_TX    | LORA_FREQUENCY_BAND | LR_LongRangeMode_LORA);
}

void lora_enterStbyMode(void)
{
    // enter Standby mode
    SPIWriteReg(LR_RegOpMode, LR_Mode_STBY  | LORA_FREQUENCY_BAND | LR_LongRangeMode_LORA);
}

void lora_enterSleepMode(void)
{
    // enter sleep mode
    SPIWriteReg(LR_RegOpMode, LR_Mode_SLEEP | LORA_FREQUENCY_BAND | LR_LongRangeMode_LORA);
}

void lora_writeFifo(uint8_t* databuf, uint8_t length)
{
    // set packet length
    if(_headerMode == LR_EXPLICIT_HEADER_MODE)
        lora_setPayloadLength(length);

    // set Fifo addr
    lora_setFifoAddrPtr(LR_RegFifoTxBaseAddr);

    // fill data into fifo
    SPIBurstWrite(LR_RegFifo, databuf, length);
}

uint8_t lora_readFifo(uint8_t* databuf, uint8_t readLen)
{
    // set Fifo ptr to RxCurrentAddr
    lora_setFifoAddrPtr(LR_RegFifoRxCurrentAddr);

    // read from fifo
    SPIBurstRead(LR_RegFifo, databuf, readLen);

    return readLen;
}

void lora_setTxInterrupt(void)
{
    // DIO0 = TxDone
    SPIWriteReg(LR_RegDIOMAPPING1,  LR_DIO0_TXDONE       );
    // enable TxDone irq
    SPIWriteReg(LR_RegIrqFlagsMask, 0xFF ^ LR_TXDONE_MASK);
}

void lora_setRxInterrupt(void)
{
    // DIO0 = RxDone
    // DIO1 = RxTimeout
    // DIO3 = ValidHeader
    SPIWriteReg(LR_RegDIOMAPPING1,  LR_DIO0_RXDONE | LR_DIO3_CRCERROR | LR_DIO1_RXTIMEOUT            );
    // enable RxDone, CrcError, RxTimeout irq
    SPIWriteReg(LR_RegIrqFlagsMask, 0xFF ^ (LR_RXDONE_MASK | LR_RXPCRCERROR_MASK | LR_RXTIMEOUT_MASK));
}

void lora_setLdoFlag(void)
{
    // Section 4.1.1.5
    uint32_t symbolDuration = 1000 / (lora_getSignalBandwidth() / (1L << lora_getSpreadingFactor())) ;

    // Section 4.1.1.6
    uint8_t ldoOn = symbolDuration > 16;

    uint8_t config3 = SPIReadReg(LR_RegModemConfig3);
    if (ldoOn)
        config3 |= (1u << 3);
    else
        config3 &= ~(1u << 3);
    SPIWriteReg(LR_RegModemConfig3, config3);
}

void lora_clrInterrupt(void)
{
    SPIWriteReg(LR_RegIrqFlags, 0xFF);
    SPIWriteReg(LR_RegDIOMAPPING1, 0x00); // DIO0 => RXDONE
}

void lora_enableInvertIQ(void)
{
    SPIWriteReg(LR_RegInvertIQ,  0x66);
    SPIWriteReg(LR_RegInvertIQ2, 0x19);
}

void lora_disableInvertIQ(void)
{
    SPIWriteReg(LR_RegInvertIQ,  0x27);
    SPIWriteReg(LR_RegInvertIQ2, 0x1D);
}

uint8_t lora_readRandomNoise(void)
{
    uint8_t rand = 0x00;
    for (uint8_t i = 0; i < 8; i++)
    {
        rand |= (SPIReadReg(LR_RegRssiWideband) & 0x01) << i;
    }
    return rand;
}

uint32_t lora_getSignalBandwidth(void)
{
    uint8_t bw = SPIReadReg(LR_RegModemConfig1) >> 4;

    switch (bw) {
        case 0: return 7.8e3;
        case 1: return 10.4e3;
        case 2: return 15.6e3;
        case 3: return 20.8e3;
        case 4: return 31.25e3;
        case 5: return 41.7e3;
        case 6: return 62.5e3;
        case 7: return 125e3;
        case 8: return 250e3;
        case 9: return 500e3;
    }
    return 0;
}

uint32_t lora_getSpreadingFactor(void)
{
    return SPIReadReg(LR_RegModemConfig2) >> 4;
}

uint32_t lora_getFrequency()
{
    uint64_t frf = (((uint32_t) SPIReadReg(LR_RegFrMsb) << 16)
                 + ((uint16_t) SPIReadReg(LR_RegFrMid) << 8)
                 + SPIReadReg(LR_RegFrLsb));
    uint64_t frq = (frf * LORA_XOSC) >> 19;
    return (uint32_t)frq;
}
