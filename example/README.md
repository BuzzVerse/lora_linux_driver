# SPI communication with LoRa - Example

## On the Beaglebone Black please run the script `config_pins.sh` before use of SPI0.

## Pin configuration (LORA to Beaglebone connections):

- `LORA CLK   : P9.22`
- `LORA MOSI  : P9.18`
- `LORA MISO  : P9.21`
- `LORA NSS   : P9.17`
- `LORA RESET : 3.3V`

Please refer to: [this pin map](https://toptechboy.com/beaglebone-black-lesson-1-understanding-beaglebone-black-pinout/beaglebone-black-pinout/)

**NOTE: All conections should be made when the device is turned off**

# Cross compilation

## Install cross compilation tools, on Ubuntu:
```bash
sudo apt update
sudo apt install gcc-arm-linux-gnueabihf -y
```

## Cross compile code:
```bash
arm-linux-gnueabihf-gcc -static -o spi_test main.c
```

Note that `-static` is used to avoid problems with `libc` version incompatibilities.

## Send the code to Beaglebone:

```bash
scp spi_test debian@192.168.7.2:/home/debian
```

# Run on target
To run on target use `sudo`:
```bash
sudo ./spi_test
```

This should dump all registers from LoRa device.

## Sample output:
```
Register at 0x00 has value: 0x00
Register at 0x01 has value: 0x09
Register at 0x02 has value: 0x1A
Register at 0x03 has value: 0x0B
Register at 0x04 has value: 0x00
Register at 0x05 has value: 0x52
Register at 0x06 has value: 0x6C
Register at 0x07 has value: 0x80
Register at 0x08 has value: 0x00
Register at 0x09 has value: 0x4F
Register at 0x0A has value: 0x09
Register at 0x0B has value: 0x2B
Register at 0x0C has value: 0x20
Register at 0x0D has value: 0x08
Register at 0x0E has value: 0x02
Register at 0x0F has value: 0x0A
Register at 0x10 has value: 0xFF
Register at 0x11 has value: 0x6F
Register at 0x12 has value: 0x15
Register at 0x13 has value: 0x0B
Register at 0x14 has value: 0x28
Register at 0x15 has value: 0x0C
Register at 0x16 has value: 0x12
Register at 0x17 has value: 0x47
Register at 0x18 has value: 0x32
Register at 0x19 has value: 0x3E
Register at 0x1A has value: 0x00
Register at 0x1B has value: 0x00
Register at 0x1C has value: 0x00
Register at 0x1D has value: 0x00
Register at 0x1E has value: 0x00
Register at 0x1F has value: 0x40
Register at 0x20 has value: 0x00
Register at 0x21 has value: 0x00
Register at 0x22 has value: 0x00
Register at 0x23 has value: 0x00
Register at 0x24 has value: 0x05
Register at 0x25 has value: 0x00
Register at 0x26 has value: 0x03
Register at 0x27 has value: 0x93
Register at 0x28 has value: 0x55
Register at 0x29 has value: 0x55
Register at 0x2A has value: 0x55
Register at 0x2B has value: 0x55
Register at 0x2C has value: 0x55
Register at 0x2D has value: 0x55
Register at 0x2E has value: 0x55
Register at 0x2F has value: 0x55
Register at 0x30 has value: 0x90
Register at 0x31 has value: 0x40
Register at 0x32 has value: 0x40
Register at 0x33 has value: 0x00
Register at 0x34 has value: 0x00
Register at 0x35 has value: 0x0F
Register at 0x36 has value: 0x00
Register at 0x37 has value: 0x00
Register at 0x38 has value: 0x00
Register at 0x39 has value: 0xF5
Register at 0x3A has value: 0x20
Register at 0x3B has value: 0x82
Register at 0x3C has value: 0xF3
Register at 0x3D has value: 0x02
Register at 0x3E has value: 0x80
Register at 0x3F has value: 0x40
Register at 0x40 has value: 0x00
Register at 0x41 has value: 0x00
Register at 0x42 has value: 0x12
Register at 0x43 has value: 0x24
Register at 0x44 has value: 0x2D
Register at 0x45 has value: 0x00
Register at 0x46 has value: 0x03
Register at 0x47 has value: 0x00
Register at 0x48 has value: 0x04
Register at 0x49 has value: 0x23
Register at 0x4A has value: 0x00
Register at 0x4B has value: 0x09
Register at 0x4C has value: 0x05
Register at 0x4D has value: 0x84
Register at 0x4E has value: 0x32
Register at 0x4F has value: 0x2B
Register at 0x50 has value: 0x14
Register at 0x51 has value: 0x00
Register at 0x52 has value: 0x00
Register at 0x53 has value: 0x10
Register at 0x54 has value: 0x00
Register at 0x55 has value: 0x00
Register at 0x56 has value: 0x00
Register at 0x57 has value: 0x0F
Register at 0x58 has value: 0xE0
Register at 0x59 has value: 0x00
Register at 0x5A has value: 0x0C
Register at 0x5B has value: 0xF4
Register at 0x5C has value: 0x06
Register at 0x5D has value: 0x00
Register at 0x5E has value: 0x5C
Register at 0x5F has value: 0x78
Register at 0x60 has value: 0x00
Register at 0x61 has value: 0x19
Register at 0x62 has value: 0x0C
Register at 0x63 has value: 0x4B
Register at 0x64 has value: 0xCC
Register at 0x65 has value: 0x0D
Register at 0x66 has value: 0xFD
Register at 0x67 has value: 0x20
Register at 0x68 has value: 0x04
Register at 0x69 has value: 0x47
Register at 0x6A has value: 0xAF
Register at 0x6B has value: 0x3F
Register at 0x6C has value: 0x73
Register at 0x6D has value: 0x3F
Register at 0x6E has value: 0xB0
Register at 0x6F has value: 0x0B
Register at 0x70 has value: 0xD0
```