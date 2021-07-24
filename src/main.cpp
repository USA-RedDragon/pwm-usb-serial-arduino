#include <Arduino.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include "pwm/pwm.h"
#include "pwm/pins.h"

#include "eeprom/eeprom.h"

#include "proto/state.pb.h"
#include "proto_utils.h"

DeviceState currentState;

void reportState() {
  uint8_t buffer[DeviceState_size];
  pbToBuffer(DeviceState_fields, &currentState, buffer, sizeof(buffer));
  Serial.write(buffer, sizeof(buffer));
}

void setup() {
  Serial.begin(9600);
  while(!Serial);

  Configuration configuration = eepromInit();

  currentState = {
    true, .configuration = configuration,
    true, .usb0 = configuration.usb0Restore,
    true, .usb1 = configuration.usb1Restore
  };

  pwmSetup(currentState.configuration);

  reportState();
}

void loop() {
  if (Serial.available() > 0) {
    uint8_t serialBuffer[DeviceState_size]; 
    int bytesRead = 0;
    while (Serial.peek() != '\r') {
      serialBuffer[bytesRead++] = Serial.read();
    }
    Serial.read(); // Drop the '\r'

    DeviceState desiredState;
    bufferToPb(serialBuffer, bytesRead, DeviceState_fields, &desiredState);

    if (desiredState.has_configuration && 
      compareConfiguration(desiredState.configuration, currentState.configuration)) {
        eepromWrite(desiredState.configuration);
        currentState.configuration = desiredState.configuration;
    }

    if (desiredState.has_usb0 && 
      comparePowerState(desiredState.usb0, currentState.usb0)) {
        currentState.usb0 = desiredState.usb0;
    }

    if (desiredState.has_usb1 && 
      comparePowerState(desiredState.usb1, currentState.usb1)) {
        currentState.usb1 = desiredState.usb1;
    }
  }

  pwmWrite(PIN_PWM_USB0, currentState.usb0);
  pwmWrite(PIN_PWM_USB1, currentState.usb1);

  reportState();

  delay(500);
}