# Wiznet W5100 and W5200 chips driver

A driver class for Wiznet W5100 and W5200 ethernet communication chips.
Each folder contains:

- w5x00.cpp and w5x00.h: driver implementation files
- spi_impl.cpp and spi_impl.h: files used to create a kind of hardware abstraction layer used by the driver to access the host's SPI bus
- w5x00_regs.h: an header file containing chip's registers defintions and other stuff

In order to use this driver you have to:

- include w5100.h or w5200.h in your main file
- add w5100.cpp or w5200.cpp and spi_impl.cpp to the makefile (or similar)
- edit the function bodies in spi_impl.cpp in order to add all the code needed to manage the SPI communication between chip and host
