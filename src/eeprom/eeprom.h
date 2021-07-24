#ifndef EEPROM_H_INCLUDED
#define EEPROM_H_INCLUDED

#define byte uint8_t

#include <Print.h>
#include <EEPROMWearLevel.h>
#include "layout.h"
#include "../proto/state.pb.h"

Configuration eepromInit();
void eepromWrite(Configuration configuration);
Configuration eepromRead();

#endif