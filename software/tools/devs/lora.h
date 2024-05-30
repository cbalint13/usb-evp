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

#ifndef _LORA_H_
#define _LORA_H_

// XOSC frequency
#define LORA_XOSC                  32000000

// RF frequency band.
// Low frequency band is up to 525M
// lora1278 only support low frequency band
#define LORA_FREQUENCY_BAND        LR_HighFreqModeOn_868M

// Waiting time for txdone irq (noWait=0)
// this time is depended on BW, SF, CR, length of packet
// you must change this value according to actual parameter and length
// see more detail on page 27 of datasheet
#define LORA_TX_TIMEOUT            20000

// Define waiting label for lora_txPacket()
// In soft waiting mode CPU will wait for ACK
#define LORA_TX_WAITACK            0
#define LORA_TX_NOWAIT             1

// Define whuich RSSI to read
#define LORA_RSSI_LAST            0
#define LORA_RSSI_CURRENT         1

/*
 * Internal registers
 */
#define LR_RegFifo                 0x00
#define LR_RegOpMode               0x01
#define LR_RegFrMsb                0x06
#define LR_RegFrMid                0x07
#define LR_RegFrLsb                0x08
#define LR_RegPaConfig             0x09
#define LR_RegPaRamp               0x0A
#define LR_RegOcp                  0x0B
#define LR_RegLna                  0x0C
#define LR_RegFifoAddrPtr          0x0D
#define LR_RegFifoTxBaseAddr       0x0E
#define LR_RegFifoRxBaseAddr       0x0F
#define LR_RegFifoRxCurrentAddr    0x10
#define LR_RegIrqFlagsMask         0x11
#define LR_RegIrqFlags             0x12
#define LR_RegRxNbBytes            0x13
#define LR_RegRxHeaderCntValueMsb  0x14
#define LR_RegRxHeaderCntValueLsb  0x15
#define LR_RegRxPacketCntValueMsb  0x16
#define LR_RegRxPacketCntValueLsb  0x17
#define LR_RegModemStat            0x18
#define LR_RegPktSnrValue          0x19
#define LR_RegPktRssiValue         0x1A
#define LR_RegRssiValue            0x1B
#define LR_RegHopChannel           0x1C
#define LR_RegModemConfig1         0x1D
#define LR_RegModemConfig2         0x1E
#define LR_RegSymbTimeoutLsb       0x1F
#define LR_RegPreambleMsb          0x20
#define LR_RegPreambleLsb          0x21
#define LR_RegPayloadLength        0x22
#define LR_RegMaxPayloadLength     0x23
#define LR_RegHopPeriod            0x24
#define LR_RegFifoRxByteAddr       0x25
#define LR_RegModemConfig3         0x26
#define LR_RegRegFeiMsb            0x28
#define LR_RegRegFeiMid            0x29
#define LR_RegRegFeiLsb            0x2A
#define LR_RegRssiWideband         0x2C
#define LR_RegIfFreq1              0x2F
#define LR_RegIfFreq2              0x30
#define LR_RegDetectOptimize       0x31
#define LR_RegInvertIQ             0x33
#define LR_RegHighBwOptimize1      0x36
#define LR_RegDetectionThreshold   0x37
#define LR_RegSyncWord             0x39
#define LR_RegSyncWord             0x39
#define LR_RegHighBwOptimize2      0x3A
#define LR_RegInvertIQ2            0x3B
#define LR_RegDIOMAPPING1          0x40
#define LR_RegDIOMAPPING2          0x41
#define LR_RegVERSION              0x42
#define LR_RegPLLHOP               0x44
#define LR_RegTCXO                 0x4B
#define LR_RegPADAC                0x4D
#define LR_RegFORMERTEMP           0x5B
#define LR_RegAGCREF               0x61
#define LR_RegAGCTHRESH1           0x62
#define LR_RegAGCTHRESH2           0x63
#define LR_RegAGCTHRESH3           0x64

/*
 * Register bit MASK
 */

//#define LR_RegOpMode             0x01
#define LR_LongRangeMode_FSK       0x00
#define LR_LongRangeMode_LORA      0x80
#define LR_ModulationType_FSK      0x00
#define LR_ModulationType_OOK      0x20
#define LR_HighFreqModeOn_868M     0x00
#define LR_LowFreqModeOn_433M      0x08
#define LR_Mode_SLEEP              0x00
#define LR_Mode_STBY               0x01
#define LR_Mode_FSTX               0x02
#define LR_Mode_TX                 0x03
#define LR_Mode_FSRX               0x04
#define LR_Mode_RXCONTINUOUS       0x05
#define LR_Mode_RXSINGLE           0x06
#define LR_Mode_CAD                0x07

//#define LR_RegPaConfig           0x09
#define LR_PASELECT_RFO            0x00
#define LR_PASELECT_PA_BOOST       0x80

//#define LR_RegOcp                0x0B
#define LR_OCPON_ON                0x20
#define LR_OCPON_OFF               0x00

//#define LR_RegLna                0x0C
#define LR_LNAGAIN_G1              0x20
#define LR_LNAGAIN_G2              0x40
#define LR_LNAGAIN_G3              0x60
#define LR_LNAGAIN_G4              0x80
#define LR_LNAGAIN_G5              0xA0
#define LR_LNAGAIN_G6              0xC0
#define LR_LNABOOSTHF_0            0x00
#define LR_LNABOOSTHF_1            0x03

//#define LR_RegIrqFlagsMask       0x11
#define LR_RXTIMEOUT_MASK          0x80
#define LR_RXDONE_MASK             0x40
#define LR_RXPCRCERROR_MASK        0x20
#define LR_VALIDHEADER_MASK        0x10
#define LR_TXDONE_MASK             0x08
#define LR_CADDONE_MASK            0x04
#define LR_FHSSCHANGECH_MASK       0x02
#define LR_CADDETECTED_MASK        0x01

//#define LR_RegModemConfig1       0x1D
#define LR_BW_7p8k                 0x00
#define LR_BW_10p4k                0x10
#define LR_BW_15p6k                0x20
#define LR_BW_20p8k                0x30
#define LR_BW_31p25k               0x40
#define LR_BW_41p7k                0x50
#define LR_BW_62p5k                0x60
#define LR_BW_125k                 0x70
#define LR_BW_250k                 0x80
#define LR_BW_500k                 0x90
#define LR_CODINGRATE_1p25         0x02 // 4/5
#define LR_CODINGRATE_1p5          0x04 // 4/6
#define LR_CODINGRATE_1p75         0x06 // 4/7
#define LR_CODINGRATE_2            0x08 // 4/8
#define LR_IMPLICIT_HEADER_MODE    0x01
#define LR_EXPLICIT_HEADER_MODE    0x00

//#define LR_RegModemConfig2       0x1E
#define LR_SPREADING_FACTOR_6      0x60
#define LR_SPREADING_FACTOR_7      0x70
#define LR_SPREADING_FACTOR_8      0x80
#define LR_SPREADING_FACTOR_9      0x90
#define LR_SPREADING_FACTOR_10     0xA0
#define LR_SPREADING_FACTOR_11     0xB0
#define LR_SPREADING_FACTOR_12     0xC0
#define LR_TX_CONTINUOUS_MODE      0x08
#define LR_TX_NORMAL_MODE          0x00
#define LR_PAYLOAD_CRC_ON          0x04
#define LR_PAYLOAD_CRC_OFF         0x00

//#define LR_RegModemConfig3       0x26
#define LR_LOW_DATARATE_OPTIMIZE   0x08
#define LR_AGC_AUTO_ON             0x04

//#define LR_RegDIOMAPPING1        0x40
#define LR_DIO0_RXDONE             0x00
#define LR_DIO0_TXDONE             0x40
#define LR_DIO0_CADDONE            0x80
#define LR_DIO1_RXTIMEOUT          0x00
#define LR_DIO1_FHSSCHANGECH       0x10
#define LR_DIO1_CADDETECTED        0x20
#define LR_DIO2_FHSSCHANGECH       0x00
#define LR_DIO3_CADDONE            0x00
#define LR_DIO3_VALIDHEADER        0x01
#define LR_DIO3_CRCERROR           0x02

//#define LR_RegDIOMAPPING2        0x41
#define LR_DIO4_CADDETECTED        0x00
#define LR_DIO4_PLLLOCK            0x40
#define LR_DIO5_MODEREADY          0x00
#define LR_DIO5_CLKOUT             0x10

//#define LR_RegTCXO               0x4B
#define LR_EXT_CRYSTAL             0x00
#define LR_TCXO_INPUT_ON           0x10
#define LR_REGTCXO_RESERVED        0x09

//#define LR_RegPADAC              0x4D
#define LR_REGPADAC_RESERVED       0x80
#define LR_20DB_OUTPUT_ON          0x07
#define LR_20DB_OUTPUT_OFF         0x04

#define RF_MID_BAND_THRESHOLD      525e6
#define RSSI_OFFSET_HF_PORT        157
#define RSSI_OFFSET_LF_PORT        164


/*
 * Functions
 */
#ifdef __cplusplus
extern "C" {
#endif

///
///  Initialize lora module
///
void lora_init(uint8_t headerMode);

///
///  Initialize rx mode
///
void lora_rxInit(uint8_t rxMode, uint8_t recvLen);

///
///  Transmit packet through RF
///
///  @param sendbuf     buf of data to send
///  @param sendLen     length of data to send,less than 64 bytes
///
///  @return            1 if tx ok, 0 otherwise
///
uint8_t lora_txPacket(uint8_t* sendbuf, uint8_t sendLen, uint8_t noWait);

///
///  Send packet from RF
///
///  @param sendbuf     buf to send the tx data
///  @param sendLen     lenght of tx data
///  @param noWait      don't wait for confirmation (will not put in sleep)
///
///  @return            1 in case of TXDONE (wait mode only), 0 otherwise
///
uint8_t lora_rxPacket(uint8_t* recvbuf);

///
///  Inquire interrupt
///
///  @param irqMask     interrupt flag
///
///  @return            1 if interrupt occur, 0 if no interrupt
///
uint8_t lora_waitIrq(uint8_t irqMask);

///
///  Enter standby mode
///
void lora_enterStbyMode(void);

///
///  Enter sleep mode
///
void lora_enterSleepMode(void);

///
///  Clear interrupt
///
void lora_clrInterrupt(void);

///
///  Set RF frequency
///
///  @param freq        value of frequency in Hz
///
///  @note: the range is 137~1020Mhz for lora1276, 137~525Mhz for lora1278.
///
void lora_setFrequency(uint32_t freq);

///
///  Set RF parameters
///
///  @param F           frequency in Hz
///  @param BW          bandwidth , range from 7.8k to 500k
///  @param CR          coding rate , range from 4/5 to 4/8
///  @param SF          spreading factor , range from 6 to 12
///  @param CRC         LR_PAYLOAD_CRC_ON  : enable CRC,
///                     LR_PAYLOAD_CRC_OFF : disable CRC
///
///  @return            1 if ok, 0 otherwise
///
///  @note: the RF data rate depends on bandwidth and spreading factor
///         coding rate affects time on air if SF=6 it will turn to
///         implict mode in this function.
///
void lora_setRFparams(uint32_t F, uint8_t BW, uint8_t CR, uint8_t SF, uint8_t CRC);

///
///  Set preamble length
///
///  @param len         preamble length
///
///  @return            1 if ok, 0 otherwise
///
void lora_setPreambleLen(uint16_t length);

///
///  Set header mode
///
///  @param mode        LR_IMPLICIT_HEADER_MODE or LR_EXPLICIT_HEADER_MODE
///
///  @note: if SF=6 it must be implicit header mode.
///
void lora_setHeaderMode(uint8_t mode);

///
///  Set payload length
///
///  @param len         payload length
///
///  @note: in implicit header mode, payload length must be set first
///         length is fix in implicit header mode.
///
void lora_setPayloadLength(uint8_t len);

///
///  Set tx power
///
///  @param power       power level -2 to 20
///
void lora_setTxPower(int8_t power);

///
///  Set rx time out
///
///  @param symbTimeOut actual timeout = symTimeout * (2^SF/BW)
///
void lora_setRxTimeOut(uint16_t symbTimeOut);

///
///  Read SNR
///
///  @return            value of snr
///
int8_t lora_readSNR(void);

///
///  Read RSSI
///
///  @param mode        0 read  current rssi, 1 read rssi of last packet received
///
///  @return            value of rssi
///
uint8_t lora_readRSSI(uint8_t mode);

///
///  Reset on lora module
///
void lora_reset(void);

///
///  Set default parameters
///
void lora_config(uint8_t headerMode);

///
///  Set FIFO address
///
///  @note: both tx data and rx data store in same fifo
///         need to set start addr befor FIFO operating.
///
void lora_setFifoAddrPtr(uint8_t addrReg);

///
///  Enter RX mode
///
///  @param rxMode      LR_Mode_RXCONTINUOUS or LR_Mode_RXSINGLE
///
void lora_enterRxMode(uint8_t rxMode);

///
///  Enter TX mode
///
void lora_enterTxMode(void);

///
///  Write tx data in fifo
///
///  @param databuf     buf of tx data
///  @param length      length of tx data, less than 64 bytes
///
void lora_writeFifo(uint8_t* databuf,uint8_t length);

///
///  Read rx data in fifo
///
///  @param databuf     buf to save the rx data
///
///  @return            length of rx data
///
uint8_t lora_readFifo(uint8_t* databuf, uint8_t readLen);

///
///  Enable TxDone interrupt
///
void lora_setTxInterrupt(void);

///
///  Enable RxDone interrupt
///
void lora_setRxInterrupt(void);

///
///  set the low data rate flags
///
void lora_setLdoFlag(void);

///
///  Random number using wideband RSSI as entropy
///
///  @return            8 bit random number
///
uint8_t lora_readRandomNoise(void);

///
///  Get signal bandwidth
///
///  @return           BW in bits/s
///
///
uint32_t lora_getSignalBandwidth();

///
///  Get spreading factor
///
///  @return           SF as value
///
///
uint32_t lora_getSpreadingFactor();

///
///  Get frequency
///
///  @return           FREQ as value
///
///
uint32_t lora_getFrequency();

///
///  IQ inversion
///
void lora_enableInvertIQ(void);
void lora_disableInvertIQ(void);

uint32_t lora_setRFerrata(uint32_t freq, uint8_t BW);

#ifdef __cplusplus
}
#endif

#endif // _LORA_H_
