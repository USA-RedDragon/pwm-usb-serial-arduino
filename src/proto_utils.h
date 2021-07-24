#ifndef PROTO_UTILS_H_INCLUDED
#define PROTO_UTILS_H_INCLUDED

#include <pb_common.h>
#include "proto/state.pb.h"

size_t pbToBuffer(const pb_msgdesc_t *fields, void *pb_struct, uint8_t *buffer, size_t bufsize);
void bufferToPb(uint8_t *buffer, size_t bufsize, const pb_msgdesc_t *fields, void *pb_struct);

bool compareConfiguration(Configuration a, Configuration b);
bool comparePowerState(PowerState a, PowerState b);

#endif