# Setup
1. Connect the LoRa module to the correct pins on BeagleBone Black
2. Enable SPI by running the "spidev_enable" script on BeagleBone
3. If required, enable GPIO pins by running the "gpio_enable" script on BeagleBone
4. Compile the source code by running "make" in the project directory and transfer the compiled files (lora_rx & lora_tx) to BeagleBone
5. Initiate the driver:
    - receiver: run "lora_rx" in sudo mode
    - transmitter run "lora_tx" in sudo mode
