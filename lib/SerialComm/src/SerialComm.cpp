#include "SerialComm.h"
#include "utils.h"

SerialComm::SerialComm(Stream &stream) : _stream(stream) {
  _index = 0;
}

bool SerialComm::update(uint32_t tick) {
  while (_stream.available()) {
    uint8_t byte = _stream.read();
    _buffer[_index] = byte;
    _index++;
    _tick = tick;
  }
  if (_index == (PACKET_LENGTH + 1)) {
    _index = 0;

    uint8_t crc = utils::crc8(_buffer, PACKET_LENGTH);
    if (crc == _buffer[PACKET_LENGTH]) {
      return true;
    } else {
      // CRC failed, what do we do?
    }
  }

  if (tick - _tick > TIMEOUT) {
    // we've timed out waiting for the rest of the packet
    // next byte received will be interpreted as the first byte of a new packet
    _index = 0;
  }
  return false;
}

bool SerialComm::empty() {
  return _index == 0;
}

uint8_t *SerialComm::data() {
  return _buffer;
}

void SerialComm::send(uint8_t *buffer) {
  for (uint8_t i = 0; i < PACKET_LENGTH; i++) {
    size_t sent = _stream.write(buffer[i]);
  }
  uint8_t crc = utils::crc8(buffer, PACKET_LENGTH);
  _stream.write(crc);
  _stream.flush();
}
