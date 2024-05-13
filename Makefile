all: lora_rx lora_tx spidev_enable

lora_rx.o: ./src/lora_rx.c
	arm-linux-gnueabihf-gcc -static ./src/lora_rx.c -o ./build/lora_rx.o -c  

lora_tx.o: ./src/lora_tx.c
	arm-linux-gnueabihf-gcc -static ./src/lora_tx.c -o ./build/lora_tx.o -c

spi_io.o: ./src/spi_io.c
	arm-linux-gnueabihf-gcc -static ./src/spi_io.c -o ./build/spi_io.o -c

lora_utility.o: ./src/lora_utility.c
	arm-linux-gnueabihf-gcc -static ./src/lora_utility.c -o ./build/lora_utility.o -c

lora_rx: lora_rx.o spi_io.o lora_utility.o
	arm-linux-gnueabihf-gcc -static -o  ./build/lora_rx ./build/lora_rx.o ./build/spi_io.o ./build/lora_utility.o

lora_tx: lora_tx.o spi_io.o lora_utility.o
	arm-linux-gnueabihf-gcc -static -o ./build/lora_tx ./build/lora_tx.o ./build/spi_io.o ./build/lora_utility.o

spidev_enable: ./src/spidev_enable.c
	arm-linux-gnueabihf-gcc -static ./src/spidev_enable.c ./build/spidev_enable
