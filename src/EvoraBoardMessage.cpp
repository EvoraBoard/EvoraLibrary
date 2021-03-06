// Copyright © 2019 Evora Board
// Use of this source code is governed by the MIT license that can be found in the LICENSE file.

#include "EvoraBoardMessage.h"

bool EvoraBoardMessage::decodeAppData(appdata_t *receiveData, const byte *payload, size_t length)
{
  pb_istream_t stream = pb_istream_from_buffer(payload, length);
  if (!pb_decode(&stream, api_AppData_fields, receiveData))
  {
    return false;
  }
#ifdef LED_BUILTIN
  receiveData->light ? digitalWrite(LED_BUILTIN, HIGH) : digitalWrite(LED_BUILTIN, LOW);
#endif
  return true;
}

void EvoraBoardMessage::encodeDeviceData(devicedata_t *data, byte **buffer, size_t *size)
{
  byte message[TTN_BUFFER_SIZE];

  pb_ostream_t sendStream = pb_ostream_from_buffer(message, sizeof(message));
  pb_encode(&sendStream, api_DeviceData_fields, data);
  *buffer = message;
  *size = sendStream.bytes_written;
}
