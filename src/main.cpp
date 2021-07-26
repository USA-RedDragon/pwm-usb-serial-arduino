#include <Arduino.h>

#include <PacketSerial.h>

#include <pb_encode.h>
#include <pb_decode.h>

#include "pwm/pwm.h"
#include "pwm/pins.h"

#include "eeprom/eeprom.h"

#include "proto/state.pb.h"
#include "proto_utils.h"

DeviceState currentState;
PacketSerial serial;

void reportState() {
  uint8_t buffer[DeviceState_size];
  size_t bytesWritten = pbToBuffer(DeviceState_fields, &currentState, buffer, sizeof(buffer));
  serial.send(buffer, bytesWritten);
}

void packetHandler(const uint8_t *buffer, size_t size) {
    DeviceState desiredState;
    bufferToPb(buffer, size, DeviceState_fields, &desiredState);

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

void setup() {
  serial.begin(9600);
  serial.setPacketHandler(&packetHandler);

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
  serial.update();

  pwmWrite(PIN_PWM_USB0, currentState.usb0);
  pwmWrite(PIN_PWM_USB1, currentState.usb1);

  reportState();

  delay(100);
}