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

// true if different, false if same
bool compareConfiguration(Configuration a, Configuration b) {
  if (a.has_usb0Restore && !b.has_usb0Restore)
    return true;
  if (a.has_usb1Restore && !b.has_usb1Restore)
    return true;
  if (comparePowerState(a.usb0Restore, b.usb0Restore))
    return true;
  return comparePowerState(a.usb1Restore, b.usb1Restore);
}

// true if different, false if same
bool comparePowerState(PowerState a, PowerState b) {
  if (a.dutyCycle != b.dutyCycle)
    return true;
  if (a.power != b.power)
    return true;
  return false;
}