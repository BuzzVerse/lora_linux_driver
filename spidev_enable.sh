#!/usr/bin/env bash
# Execute on a BeagleBone Black with no overlays loaded.
# These commands require that the HDMI port is disabled.
# These commands require the default pin configuration in order to work.

# For SPI0, /dev/spidev0.#
#
config-pin p9_17 spi_cs
config-pin p9_18 spi
config-pin p9_21 spi
config-pin p9_22 spi_sclk

# For SPI1, /dev/spidev1.#
#
config-pin p9_28 spi_cs
config-pin p9_29 spi
config-pin p9_30 spi
config-pin p9_31 spi_sclk
