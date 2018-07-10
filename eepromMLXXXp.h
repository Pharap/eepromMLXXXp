#ifndef EEPROMMLXXXP_H
#define EEPROMMLXXXP_H

#include "Arduino.h"
#include "EEPROM.h"

#define ARDUBOY_EEPROM_RESERVED 16
#define ARDUBOY_EEPROM_SIZE 1024

// open() return values
#define EEPROM_ALLOCATED 1
#define EEPROM_ERROR_ALLOCATE -1

class ArduboyEeprom
{
  public:
    int begin(unsigned int start, uint16_t userID, uint8_t sketchID, unsigned int length);

    uint8_t read(unsigned int address) const;
    boolean write(unsigned int address, uint8_t data);
    boolean read(unsigned int address, uint8_t *buffer, size_t size) const;
    boolean write(unsigned int address, const uint8_t *buffer, size_t size);
    template<typename T>
    boolean read(unsigned int address, T &object) const;
    template<typename T>
    boolean write(unsigned int address, const T &object);
	
  private:
    static uint8_t readRawData(unsigned int address);
	static void writeRawData(unsigned int address, uint8_t data);

  private:
    unsigned int eeStart;
    unsigned int eeLength;
};

#endif
