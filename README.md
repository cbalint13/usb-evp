# usb-evp
**USB Event Packet** protocol (LoRA) Stick

Original project dating back @ 2018 [CircuitMaker USB-EVP-LoRa](https://workspace.circuitmaker.com/Projects/Details/Cristian-Balint/USB-EVP-LoRa)

![OVERVIEW-3D-CAD](docs/images/USB-EVP-LORA-3D-CAD.png)

## Description

USB stick for **Event Packet** (over LoRa) with integrated antenna on ISM 868/915 bands.

Exposes a generic USB HID interface by MCP2210 directly enabling OS & driver agnostic access from userland.

It have a minimal BOM (bill of materials).
![OVERVIEW-3D-SCHD](docs/images/USB-EVP-LORA-SCH-KICAD.png)

## Specs:

* **Smallest LoRa stick** on the market by 1.6 x 4 cm
* Uses FCC and CE **certified** 100mW NiceRF module
* Own **ceramic micro-antenna** (VNA proved RF) for ISM band
* **OS agnostic**, opensource USB stack for windows, linux, android etc
* **C / C++** frontend with various examples
* May support **LPWAN networking** (IPv6 over 802.15.4 mac layer), with third party kernel module

Can be used in any USB port realying packets over the air.

![OVERVIEW-3D-TOP](docs/images/USB-EVP-LORA-3D-TOP.png)
![OVERVIEW-3D-BOTTOM](docs/images/USB-EVP-LORA-3D-BOTTOM.png)

## EU ISM band usage 
* [LoRa 868Mhz bands diagram](docs/legal)

## Design files
* See the [design files](hardware) available for Altium ®, CircuitMaker ®, KiCAD and Gerber format.
* Check [schematic](hardware/USB-eVo_v1.1.pdf) available as quick readable format.
* There is also an CircuitMaker ® [online version](https://circuitmaker.com/Projects/Details/Cristian-Balint/USB-EVP-LoRa) available.

## Software support
* See [software](software) section for configuration and support.

    ```
    $ ./bin/lora-receive /dev/hidraw4 
    LoRa RX count #1 frame length = 14 rssi = -61 dB snr = 8 dB
    0x00000000 20 6C 6F 72 61 20 6D 65  73 73 61 67 65 00 |  A lora message.
    ```

## Board images
![OVERVIEW-TOP](docs/images/USB-EVP-LORA-LOT.png)
