/* Ser23K256 - drive the Microchip 23K256 SRAM using SPI
* Copyright (c) 2010 Romilly Cocking
* Released under the MIT License: http://mbed.org/license/mit
*/

#include "mbed.h"
#include "Ser23K256.h"

Ser23K256::Ser23K256(SPI& spi, PinName ncs) : _spi(spi), _ncs(ncs)  {
    deselect();
}

void Ser23K256::select() {
    _ncs = 0;
}

void Ser23K256::deselect() {
    _ncs = 1;
}

void Ser23K256::writeStatus(char status) {
    select();
    _spi.write(WRITE_STATUS);
    _spi.write(status);
    deselect();
}

char Ser23K256::readStatus() {
    select();
    _spi.write(READ_STATUS);
    char result = (char) _spi.write(0);
    deselect();
    return result;
}

void Ser23K256::prepareCommand(char command, int address) {
    select();
    _spi.write(command);
    _spi.write(address >> 8);
    _spi.write(address & 0xFF);
}

// write or read a single byte

void Ser23K256::write(int address, char byte) {
    prepareCommand(WRITE, address);
    _spi.write(byte);
    deselect();
}

char Ser23K256::read(int address) {
    prepareCommand(READ, address);
    int result = _spi.write(0);
    deselect();
    return (char) result;
}

// buffered write and read

/*
* the single-byte read and write assume the 23K256 is in its default byte-mode
* so sequential-model commands must switch the chip into sequential mode
* at the start and return it to byte mode at the end.
*/

void Ser23K256::write(int address, char * buffer, int count) {
    writeStatus(SEQUENTIAL_MODE);
    prepareCommand(WRITE, address);
    for (int i = 0; i < count; i++) {
        _spi.write(buffer[i]);
    }
    deselect();
    writeStatus(BYTE_MODE);
}

void Ser23K256::read(int address, char * buffer, int count) {
    writeStatus(SEQUENTIAL_MODE);
    prepareCommand(READ, address);
    for (int i = 0; i < count; i++) {
        buffer[i] = _spi.write(0);
    }
    deselect();
    writeStatus(BYTE_MODE);
}