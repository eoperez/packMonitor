#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "SerialComm.h"

#define ADDRESS_BROADCAST 0x0

#define REG_ADDRESS 0x1
#define REG_VOLTAGE_REF 0x2
#define REG_VOLTAGE 0x3
#define REG_TEMP 0x4
#define REG_BALANCE 0x5

typedef struct {
  uint8_t address;
  uint8_t request;
  uint8_t reg;
  uint8_t write;
  uint16_t value;
} packet_t;

class Protocol {
 public:
  Protocol(SerialComm *comm);
  bool handle(uint8_t *data, packet_t *packet);
  void respond(packet_t *packet, uint16_t value);
  void send(packet_t *packet);
  packet_t getPacket(uint8_t *data, packet_t *packet);

 private:
  void forward(packet_t *packet);
  SerialComm *_comm;
  uint8_t _address;
};

#endif
