/** Ser23K256 - drive the Microchip 23K256 SRAM using SPI
* Copyright (c) 2010 Romilly Cocking
* Released under the MIT License: http://mbed.org/license/mit
*
* 23K256 data sheet at http://ww1.microchip.com/downloads/en/DeviceDoc/22100B.pdf
*
* Page-mode commands have not been implemented; I have found no need for them yet.
*
* Assumes spi mode is default (8,0).
*
* You can clock the 23K256 at up to 20MHz, so it supports the mbed's maximum SPI frequency of 12MHz.
*/

#include "mbed.h"

#ifndef  SER23K256_H
#define  SER23K256_H

// mode codes for 23K256
#define BYTE_MODE       0x00
#define SEQUENTIAL_MODE 0x40

// command codes for 23K256
#define READ            0x03
#define WRITE           0x02
#define READ_STATUS     0x05 // called RDSR in datasheet
#define WRITE_STATUS    0x01 // called WRSR in datasheet

/** An interface for the Microchip 32k byte 23K256 SRAM over SPI
*
* 
*
* @code
* #include "mbed.h"
* #include "Ser23K256.h"
*
*
* SPI spi(p5,p6,p7);
* Ser23K256 sram(spi,p14);
*
* int main() {
*   uint8_t buff[50];
*   sram.write(0, 'h');
*   sram.write(1, 'i');
*   sram.write(2, '!');
*   sram.write(3, '\0');
*   for (int address = 0; address < 4; address++) {
*       buff[address] = sram.read(address);
*   }
*   printf("sram = %s\r\n", buff);
*   sram.write(0, "Hello world!",12);
*   sram.read(0, buff, 12);
*   buff[12]='\0';
*   printf("now = %s\r\n", buff);
*}
* @endcode
* connections:
* chip pin 1 to mbed ncs (see below)
* chip pin 2 SO to mbed MISO
* chip pin 3 - no connection
* chip pin 4 to mbed Gnd
* chip pin 5 SI pin to mbed MOSI
* chip pin 6 SCK to mbed sck 
* chip pin 7 (notHOLD) to mbed Vout
* chip pin 8 to mbed Vout
*/
class Ser23K256 {
public:
/** Create an interface
*
*
* @param spi   An SPI object
* @param ncs   Not chip select pin - any free Digital pin will do
*/
    Ser23K256(SPI& spi, PinName ncs);
/** read a byte from SRAM
* @param address    The address to read from
* @return the uint8_tacter at that address
*/
    uint8_t read(int address);
/** read multiple bytes from SRAM into a buffer
* @param address    The SRAM address to read from
* @param buffer     The buffer to read into (must be big enough!)
* @param count      The number of bytes to read
*/
    void read(int address, uint8_t * buffer, int count);
/** write a byte to SRAM
* @param address    The address SRAM to write to
* @param byte       The byte to write there
*/
    void write(int address, uint8_t byte);
    /** write multiple bytes to SRAM from a buffer
* @param address    The SRAM address write to
* @param buffer     The buffer to write from
* @param count      The number of bytes to write
*/
    void write(int address, uint8_t * buffer, int count);
private:
    SPI& _spi;
    DigitalOut _ncs;
    uint8_t readStatus();
    void writeStatus(uint8_t status);
    void prepareCommand(uint8_t command, int address);
    void select();
    void deselect();
};

#endif
