all: lora_rx lora_tx test

lora_rx.o: ./src/lora_rx.c
	arm-linux-gnueabihf-gcc -static ./src/lora_rx.c -o ./build/lora_rx.o -c

lora_tx.o: ./src/lora_tx.c
	arm-linux-gnueabihf-gcc -static ./src/lora_tx.c -o ./build/lora_tx.o -c

spi_io.o: ./src/spi_io.c
	arm-linux-gnueabihf-gcc -static ./src/spi_io.c -o ./build/spi_io.o -c

lora_utility.o: ./src/lora_utility.c
	arm-linux-gnueabihf-gcc -static ./src/lora_utility.c -o ./build/lora_utility.o -c

lora_api_impl.o: ./src/lora_api_impl.c
	arm-linux-gnueabihf-gcc -static ./src/lora_api_impl.c -o ./build/lora_api_impl.o -c

lora_driver.o: ./include/driver/lora_driver.c 
	arm-linux-gnueabihf-gcc -static ./include/driver/lora_driver.c -o ./build/lora_driver.o -c

lora_rx: lora_rx.o spi_io.o lora_driver.o lora_api_impl.o
	arm-linux-gnueabihf-gcc -static -o  ./build/lora_rx ./build/lora_rx.o ./build/spi_io.o ./build/lora_driver.o ./build/lora_api_impl.o

lora_tx: lora_tx.o spi_io.o lora_utility.o
	arm-linux-gnueabihf-gcc -static -o ./build/lora_tx ./build/lora_tx.o ./build/spi_io.o ./build/lora_utility.o

test: ./src/test.c
	arm-linux-gnueabihf-gcc -static -o ./build/test ./src/test.c
