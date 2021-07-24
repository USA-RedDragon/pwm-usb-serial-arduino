#include <Arduino.h>

#include "pins.h"
#include "pwm.h"

void pwmSetup(Configuration configuration) {
    pinMode(PIN_PWM_USB0, OUTPUT);
    pinMode(PIN_PWM_USB1, OUTPUT);
   
    pwmWrite(PIN_PWM_USB0, configuration.usb0Restore);
    pwmWrite(PIN_PWM_USB1, configuration.usb1Restore);
}

void pwmWrite(int pin, PowerState powerState) {
    if (powerState.power) {
        analogWrite(pin, powerState.dutyCycle);
    }
}