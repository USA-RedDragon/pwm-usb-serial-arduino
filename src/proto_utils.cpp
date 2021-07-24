#include <pb_encode.h>
#include <pb_decode.h>

#include "proto_utils.h"

size_t pbToBuffer(const pb_msgdesc_t *fields, void *pb_struct, uint8_t *buffer, size_t bufsize) {
  pb_ostream_t stream = pb_ostream_from_buffer(buffer, bufsize);
  pb_encode(&stream, fields, pb_struct);
  return stream.bytes_written;
}

void bufferToPb(uint8_t *buffer, size_t bufsize, const pb_msgdesc_t *fields, void *pb_struct) {
  pb_istream_t stream = pb_istream_from_buffer(buffer, bufsize);
  pb_decode(&stream, fields, pb_struct);
}

bool compareConfiguration(Configuration a, Configuration b) {
  if (a.has_usb0Restore && !b.has_usb0Restore)
    return false;
  if (a.has_usb1Restore && !b.has_usb1Restore)
    return false;
  if (!comparePowerState(a.usb0Restore, b.usb0Restore))
    return false;
  return comparePowerState(a.usb1Restore, b.usb1Restore);
}

bool comparePowerState(PowerState a, PowerState b) {
  if (a.dutyCycle != b.dutyCycle)
    return false;
  if (a.power != b.power)
    return false;
}