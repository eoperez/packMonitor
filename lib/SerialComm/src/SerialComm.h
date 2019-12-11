#ifndef __SERIALCOMM_H__
#define __SERIALCOMM_H__

#include <Arduino.h>
#include <Stream.h>
#include "LedIndicator.h"

#define PACKET_LENGTH 4 // not including trailing CRC byte
#define TIMEOUT 500 // ms

class SerialComm {
 public:
  SerialComm(Stream &stream);
  bool update(uint32_t tick);
  bool empty();
  uint8_t *data();
  void send(uint8_t *buffer);

 private:
   Stream &_stream;
   uint8_t _buffer[PACKET_LENGTH + 1];
   uint8_t _index;
   uint32_t _tick;
};

#endif
