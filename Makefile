lora_rx.o: ./src/lora_rx.c
	arm-linux-gnueabihf-gcc -static ./src/lora_rx.c -o ./build/lora_rx.o -c  

lora_tx.o: ./src/lora_tx.c
	arm-linux-gnueabihf-gcc -static ./src/lora_tx.c -o ./build/lora_tx.o -c

spi_io.o: ./src/spi_io.c
	arm-linux-gnueabihf-gcc -static ./src/spi_io.c -o ./build/spi_io.o -c

lora_rx: lora_rx.o spi_io.o
	arm-linux-gnueabihf-gcc -static -o ./build/lora_rx.o ./build/spi_io.o

