#
# bldc configuration written in linux configuration language
#
# Written by miaofng@2010
#

#use gpio as chip select
bool "PA2 as CS" CONFIG_SPI_CS_PA2
bool "PA3 as CS" CONFIG_SPI_CS_PA3
bool "PA4/SPI1_NSS as CS" CONFIG_SPI_CS_PA4
bool "PA12 as CS" CONFIG_SPI_CS_PA12

bool "PB0 as CS" CONFIG_SPI_CS_PB0
bool "PB1 as CS" CONFIG_SPI_CS_PB1
bool "PB10 as CS" CONFIG_SPI_CS_PB10
bool "PB12/SPI2_NSS as CS" CONFIG_SPI_CS_PB12

bool "PC3 as CS" CONFIG_SPI_CS_PC3
bool "PC4 as CS" CONFIG_SPI_CS_PC4
bool "PC5 as CS" CONFIG_SPI_CS_PC5
bool "PC7 as CS" CONFIG_SPI_CS_PC7
bool "PC8 as CS" CONFIG_SPI_CS_PC8

bool "PD8 as CS" CONFIG_SPI_CS_PD8
bool "PD9 as CS" CONFIG_SPI_CS_PD9
bool "PD12 as CS" CONFIG_SPI_CS_PD12

bool "PF11 as CS" CONFIG_SPI_CS_PF11
