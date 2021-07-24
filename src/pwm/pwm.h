#ifndef PWM_PWM_H_INCLUDED
#define PWM_PWM_H_INCLUDED

#include "../proto/state.pb.h"

void pwmSetup(Configuration configuration);
void pwmWrite(int pin, PowerState powerState);

#endif