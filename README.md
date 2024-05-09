# Setup
1. Connect the LoRa module to the correct pins on BeagleBone Black
2. Enable SPI by running spidev_enable.sh on BeagleBone
3. If required, enable GPIO pins by running gpio_enable.sh on BeagleBone
    - this is required if LoRa NRST is being connected to one of the programmable GPIO pins on BeagleBone
    - connecting NRST to GPIO pins is recommended, as it allows to manually reset the chip (e.g. by running reset.sh)
    - the script is configured to work with GPIO pins 66 (P8.07) and 69 (P8.09)
4. Compile the source code by running "make" in the project directory
5. Transfer the compiled files (lora_rx & lora_tx) to BeagleBone by running send_to_bbb.sh
6. Initiate the driver:
    - receiver: run "lora_rx" in sudo mode
    - transmitter run "lora_tx" in sudo mode
