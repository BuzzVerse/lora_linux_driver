# Setup
1. Install arm gcc with ```sudo apt-get install gcc-arm-linux-gnueabihf```
2. Connect the LoRa module to the correct pins on BeagleBone Black
1. Compile the source code by running "make" in the project directory
1. Transfer the compiled files (lora_rx & lora_tx) to BeagleBone by running send_to_bbb.sh
1. Initiate the driver:
    - receiver: run "lora_rx" in sudo mode
    - transmitter: run "lora_tx" in sudo mode
    - DISCLAIMER: receiver needs to be ran first (at the moment, receiver program contains hardware initialization)

# MQTT dependencies
```
$ sudo apt install mosquitto mosquitto-clients libmosquitto-dev
```

# MQTT config
1. Create the config file at /etc/mqtt_config
1. Config file should look like this:
```
ip=192.168.0.69
login=buzzverse
password=verysecurepassword
topic=topic/something
```
## Known issues
If your mqtt password contains non-ascii symbols try to copy and paste it instead typing it.
