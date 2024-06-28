CC = arm-linux-gnueabihf-gcc
CFLAGS = -static -Iinclude/ -Iprotocls/

all: lora_rx lora_tx 

lora_rx.o: ./src/lora_rx.c
	$(CC) $(CFLAGS) ./src/lora_rx.c -o ./build/lora_rx.o -c

lora_tx.o: ./src/lora_tx.c
	$(CC) $(CFLAGS) ./src/lora_tx.c -o ./build/lora_tx.o -c

bbb_api_impl.o: ./src/bbb_api_impl.c
	$(CC) $(CFLAGS) ./src/bbb_api_impl.c -o ./build/bbb_api_impl.o -c

lora_driver.o: ./include/driver/lora_driver.c 
	$(CC) $(CFLAGS) ./include/driver/lora_driver.c -o ./build/lora_driver.o -c

mqtt.o: ./src/mqtt/mqtt.c ./src/mqtt/mqtt_pal.c
	$(CC) $(CFLAGS) ./src/mqtt/mqtt.c ./src/mqtt/mqtt_pal.c -o ./build/mqtt.o -c

lora_rx: lora_rx.o lora_driver.o bbb_api_impl.o mqtt.o
	$(CC) $(CFLAGS) -o ./build/lora_rx ./build/lora_rx.o ./build/lora_driver.o ./build/bbb_api_impl.o ./build/mqtt.o

lora_tx: lora_tx.o lora_driver.o bbb_api_impl.o
	$(CC) $(CFLAGS) -o ./build/lora_tx ./build/lora_tx.o ./build/lora_driver.o ./build/bbb_api_impl.o
