# SPI communication with LoRa - Example

## On the Beaglebone Black please run the script `config_pins.sh` before use of SPI0.

## Pin configuration (LORA to Beaglebone connections):

- `LORA CLK   : P9.22`
- `LORA MOSI  : P9.18`
- `LORA MISO  : P9.21`
- `LORA NSS   : P9.17`
- `LORA RESET : 3.3V`

{lease refer to: [this pin map](https://toptechboy.com/beaglebone-black-lesson-1-understanding-beaglebone-black-pinout/beaglebone-black-pinout/)

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
