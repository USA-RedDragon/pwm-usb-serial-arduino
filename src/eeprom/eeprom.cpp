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
    pbToBuffer(Configuration_fields, &configuration, buffer, sizeof(buffer));
    EEPROMwl.put(INDEX_CONFIGURATION, buffer);
}

Configuration eepromRead() {
    Configuration configuration;
    EEPROMwl.get(INDEX_CONFIGURATION, configuration);
    return configuration;
}