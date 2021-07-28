#include "eeprom.h"

void eepromWriteBuffer(int address, uint8_t *buffer, size_t bufSize)
{
  int dataLength = sizeof(uint8_t);
  int startIndex = EEPROMwl.getStartIndexEEPROM(address);
  for (int i = 0; i < bufSize; i++) {
    buffer[i] = EEPROM.put(startIndex + i * dataLength, buffer[i]);
  }
}

void eepromReadBuffer(int address, uint8_t *buffer, size_t bufSize) {
  int dataLength = sizeof(uint8_t);
  int startIndex = EEPROMwl.getStartIndexEEPROM(address);
  for (int i = 0; i < bufSize; i++) {
    buffer[i] = EEPROM.get(startIndex + i * dataLength, buffer[i]);
  }
}
