# LoRa USB stick initial configuration

* Please check and compile the [mcp2210-tool](https://github.com/lkundrak/mcp2210)
* Use the ```evp-lora-stick-hwcfg.sh``` script to initialize the USB stick NVRAM settings

---

The ```dmesg``` output summary:

```
$ dmesg
...
[296427.566309] usb 1-6: new full-speed USB device number 7 using xhci_hcd
[296427.695951] usb 1-6: New USB device found, idVendor=04d8, idProduct=00de, bcdDevice= 0.02
[296427.695968] usb 1-6: New USB device strings: Mfr=1, Product=2, SerialNumber=3
[296427.695975] usb 1-6: Product: [eVo] USB Stick v1.2
[296427.695981] usb 1-6: Manufacturer: EventPacket
[296427.695986] usb 1-6: SerialNumber: 0000818649
[296427.700092] hid-generic 0003:04D8:00DE.0005: hiddev97,hidraw4: USB HID v1.11 Device [EventPacket [eVo] USB Stick v1.2] on usb-0000:00:14.0-6/input0
...
```
