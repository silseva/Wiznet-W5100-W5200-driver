# Wiznet-W5100-W5200-driver

A driver class for Wiznet W5100 and W5200 ethernet communication chips.
The folder contains:

- w5x00.cpp and w5x00.h: files containing the driver class
- spi_impl.cpp and spi_impl.h: files containing a bunch of hardware abstraction layer functions used by the driver interact with host's SPI bus
- w5x00_defs.h: an header file containing chip's registers defintions and other stuff
