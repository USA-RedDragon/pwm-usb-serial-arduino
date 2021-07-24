#include "eeprom.h"
#include "../proto_utils.h"

Configuration createDefaultConfiguration() {
  PowerState usb0 = {
    .power = false,
    .dutyCycle = 0
  };

  PowerState usb1 = {
    .power = false,
    .dutyCycle = 0
  };

  Configuration configuration = {
    true, .usb0Restore = usb0,
    true, .usb1Restore = usb1
  };

  return configuration;
}

void writeBufferIntoEEPROM(int address, uint8_t *buffer, size_t bufSize)
{
  int addressIndex = address;
  for (int i = 0; i < bufSize; i++) 
  {
    EEPROMwl.write(addressIndex, buffer[i] >> 8);
    EEPROMwl.write(addressIndex + 1, buffer[i] & 0xFF);
    addressIndex += 2;
  }
}

void readBufferFromEEPROM(int address, uint8_t *buffer, size_t bufSize)
{
  int addressIndex = address;
  for (int i = 0; i < bufSize; i++)
  {
    buffer[i] = (EEPROMwl.read(addressIndex) << 8) + EEPROMwl.read(addressIndex + 1);
    addressIndex += 2;
  }
}

Configuration eepromInit() {
    Configuration configuration;
    EEPROMwl.begin(EEPROM_LAYOUT_VERSION, AMOUNT_OF_INDEXES);
    bool eepromInited = false;
    EEPROMwl.get(INDEX_EEPROM_INIT, eepromInited);
    if (!eepromInited) {
        configuration = createDefaultConfiguration();
        eepromWrite(configuration);
        EEPROMwl.put(INDEX_EEPROM_INIT, true);
    } else
        configuration = eepromRead();
    return configuration;
}

void eepromWrite(Configuration configuration) {
    uint8_t buffer[Configuration_size];
    size_t sizeWritten = pbToBuffer(Configuration_fields, &configuration, buffer, sizeof(buffer));
    EEPROMwl.put(INDEX_CONFIGURATION_BUFFER_SIZE, sizeWritten);
    writeBufferIntoEEPROM(INDEX_CONFIGURATION, buffer, sizeWritten);
}

Configuration eepromRead() {
    Configuration configuration;
    size_t bufferSize = Configuration_size;
    EEPROMwl.get(INDEX_CONFIGURATION_BUFFER_SIZE, bufferSize);
    uint8_t buffer[bufferSize];
    readBufferFromEEPROM(INDEX_CONFIGURATION, buffer, bufferSize);
    bufferToPb(buffer, bufferSize, Configuration_fields, &configuration);
    return configuration;
}