CC = arm-linux-gnueabihf-gcc
CFLAGS = -static -Iinclude/

all: lora_rx lora_tx lora_tx_new test

lora_rx.o: ./src/lora_rx.c
	$(CC) $(CFLAGS) ./src/lora_rx.c -o ./build/lora_rx.o -c

lora_tx.o: ./src/lora_tx.c
	$(CC) $(CFLAGS) ./src/lora_tx.c -o ./build/lora_tx.o -c

lora_tx_new.o: ./src/lora_tx_new.c
	$(CC) $(CFLAGS) ./src/lora_tx_new.c -o ./build/lora_tx_new.o -c

spi_io.o: ./src/spi_io.c
	$(CC) $(CFLAGS) ./src/spi_io.c -o ./build/spi_io.o -c

lora_utility.o: ./src/lora_utility.c
	$(CC) $(CFLAGS) ./src/lora_utility.c -o ./build/lora_utility.o -c

lora_api_impl.o: ./src/lora_api_impl.c
	$(CC) $(CFLAGS) ./src/lora_api_impl.c -o ./build/lora_api_impl.o -c

lora_driver.o: ./include/driver/lora_driver.c 
	$(CC) $(CFLAGS) ./include/driver/lora_driver.c -o ./build/lora_driver.o -c

lora_rx: lora_rx.o spi_io.o lora_driver.o lora_api_impl.o
	$(CC) $(CFLAGS) -o  ./build/lora_rx ./build/lora_rx.o ./build/spi_io.o ./build/lora_driver.o ./build/lora_api_impl.o

lora_tx: lora_tx.o spi_io.o lora_utility.o
	$(CC) $(CFLAGS) -o ./build/lora_tx ./build/lora_tx.o ./build/spi_io.o ./build/lora_utility.o

lora_tx_new: lora_tx_new.o spi_io.o lora_driver.o lora_api_impl.o
	$(CC) $(CFLAGS) -o ./build/lora_tx_new ./build/lora_tx_new.o ./build/spi_io.o ./build/lora_driver.o ./build/lora_api_impl.o

test: ./src/test.c
	$(CC) $(CFLAGS) -o ./build/test ./src/test.c
