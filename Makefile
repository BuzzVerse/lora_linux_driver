# CC = gcc
CC = arm-linux-gnueabihf-gcc
CFLAGS = -static -Iinclude/ -Iprotocols/ -Isrc/

all: make_build lora_rx lora_tx 

make_build:
	mkdir -p ./build/

lora_rx.o: ./src/lora_rx.c
	$(CC) $(CFLAGS) ./src/lora_rx.c -o ./build/lora_rx.o -c

lora_tx.o: ./src/lora_tx.c
	$(CC) $(CFLAGS) ./src/lora_tx.c -o ./build/lora_tx.o -c

bbb_api_impl.o: ./src/bbb_api_impl.c
	$(CC) $(CFLAGS) ./src/bbb_api_impl.c -o ./build/bbb_api_impl.o -c

lora.o: ./src/lora.c
	$(CC) $(CFLAGS) ./src/lora.c -o ./build/lora.o -c

mqtt_config.o: ./src/mqtt_config.c
	$(CC) $(CFLAGS) ./src/mqtt_config.c -o ./build/mqtt_config.o -c

lora_driver.o: ./include/driver/lora_driver.c 
	$(CC) $(CFLAGS) ./include/driver/lora_driver.c -o ./build/lora_driver.o -c

lora_rx: lora_rx.o lora_driver.o bbb_api_impl.o lora.o mqtt_config.o
	$(CC) $(CFLAGS) -o ./build/lora_rx ./build/lora_rx.o ./build/lora_driver.o ./build/bbb_api_impl.o ./build/lora.o ./build/mqtt_config.o

lora_tx: lora_tx.o lora_driver.o bbb_api_impl.o lora.o
	$(CC) $(CFLAGS) -o ./build/lora_tx ./build/lora_tx.o ./build/lora_driver.o ./build/bbb_api_impl.o ./build/lora.o

tests: lora_tests

lora_tests: ./test/lora_tests.c ./src/lora.c ./test/unity.c
	gcc $(CFLAGS) -DMOCK ./test/lora_tests.c ./test/unity.c ./src/lora.c ./src/bbb_api_impl.c -o ./test/lora_tests
