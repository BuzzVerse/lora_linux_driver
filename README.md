# Setup
1. Connect the LoRa module to the correct pins on BeagleBone Black
1. Compile the source code by running "make" in the project directory
1. Transfer the compiled files (lora_rx & lora_tx) to BeagleBone by running send_to_bbb.sh
1. Initiate the driver:
    - receiver: run "lora_rx" in sudo mode
    - transmitter: run "lora_tx" in sudo mode
    - DISCLAIMER: receiver needs to be ran first (at the moment, receiver program contains hardware initialization)
