#include <stdlib.h>

int main(void) {
    // For SPI0, /dev/spidev0.#
    system("config-pin p9_17 spi_cs");
    system("config-pin p9_18 spi");
    system("config-pin p9_21 spi");
    system("config-pin p9_22 spi_sclk");

    // For SPI1, /dev/spidev1.#
    system("config-pin p9_28 spi_cs");
    system("config-pin p9_29 spi");
    system("config-pin p9_30 spi");
    system("config-pin p9_31 spi_sclk");

}
