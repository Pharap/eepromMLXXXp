#include "eepromMLXXXp.h"
#include <avr/eeprom.h>

int ArduboyEeprom::begin(unsigned int start, uint16_t userID, uint8_t sketchID, unsigned int length) {
  eeStart = start + 3; // user data starts after user ID and sketch ID
  eeLength = length;

  // check bounds
  if (start < ARDUBOY_EEPROM_RESERVED) {
    return EEPROM_ERROR_ALLOCATE;
  }
  if ((eeStart + eeLength) >= ARDUBOY_EEPROM_SIZE) {
    return EEPROM_ERROR_ALLOCATE;
  }

  // verify IDs (which indicates previously allocated)
  if ((readRawData(start) == (userID >> 8)) &&
      (readRawData(start + 1) == (userID & 0xFF)) &&
      (readRawData(start + 2) == (sketchID))) {
    return 0;
  }

  // indicate area was allocated by setting user ID and sketch ID
  writeRawData(start, userID >> 8);
  writeRawData(start + 1, userID & 0xFF);
  writeRawData(start + 2, sketchID);
  return EEPROM_ALLOCATED;
}

uint8_t ArduboyEeprom::read(unsigned int address) const {
  return readRawData(eeStart + address);
}

boolean ArduboyEeprom::write(unsigned int address, uint8_t data) {
  if (address >= eeLength) {
    return false;
  }

  writeRawData(eeStart + address, data);
  return true;
}

boolean ArduboyEeprom::read(unsigned int address, uint8_t *buffer, size_t size) const {
  unsigned int end = address + size;
  if (end > eeLength) {
    return false;
  }

  end += eeStart;
  for (unsigned int i = eeStart + address; i < end; i++) {
    *buffer++ = readRawData(i);
  }
  return true;
}

boolean ArduboyEeprom::write(unsigned int address, const uint8_t *buffer, size_t size) {
  unsigned int end = address + size;
  if (end > eeLength) {
    return false;
  }

  end += eeStart;
  for (unsigned int i = eeStart + address; i < end; i++) {
    writeRawData(i, *buffer++);
  }
  return true;
}

template<typename T>
boolean ArduboyEeprom::read(unsigned int address, T &object) const {
  return read(address, reinterpret_cast<uint8_t *>(&object), sizeof(T));
}

template<typename T>
boolean ArduboyEeprom::write(unsigned int address, const T &object) {
  return write(address, reinterpret_cast<const uint8_t *>(&object), sizeof(T));
}

uint8_t ArduboyEeprom::readRawData(unsigned int address) {
  return eeprom_read_byte(reinterpret_cast<const uint8_t *>(eeStart + address));
}

uint8_t ArduboyEeprom::writeRawData(unsigned int address, uint8_t data) {
  uint8_t * addressPointer = reinterpret_cast<uint8_t *>(eeStart + address);
  if(eeprom_read_byte(addressPointer) != data) {
    eeprom_write_byte(addressPointer);
  }
}