# USB stick library stack

You can compile all tools using the provided ```make.sh```.

---

## Applications

 * Use ```scan.sh``` to fetch the exact device path:

    ```
    $ ./scan.sh
    /dev/hidraw0 Lite-On Technology Corp. Lenovo Low Profile USB Keyboard
    /dev/hidraw1 Lite-On Technology Corp. Lenovo Low Profile USB Keyboard
    /dev/hidraw2 ITE Tech. Inc. ITE Device
    /dev/hidraw3 Designer Mouse
    /dev/hidraw4 EventPacket [eVo] USB Stick v1.2
    ```

 * Chipset (SX127x) revision check:

    ```
    $ ./bin/check-version /dev/hidraw4
    SX127X chip version: 0x12
    ```

 * Random number generator based on wideband RSSI measurement:

    ```
    $ ./bin/sx127x-random /dev/hidraw4
    Found SX127X chip version: 12
    253
    186
    97
    216
    157
    ...
    ```
 * Chipset (SX127x) revision check:

    ```
    $ ./bin/check-version /dev/hidraw4
    SX127X chip version: 0x12
    ```

 * LoRa receive example:

    ```
    $ ./bin/lora-receive /dev/hidraw4 
    LoRa RX count #1 frame length = 14 rssi = -61 dB snr = 8 dB
    0x00000000 20 6C 6F 72 61 20 6D 65  73 73 61 67 65 00 |  A lora message.
    ```

 * LoRa sending example:

    ```
    $ ./bin/lora-send /dev/hidraw4
    LoRa sent message
    ```
