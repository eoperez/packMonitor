#include <Arduino.h>
#include <LedIndicator.h>
#include <MqttComm.h>
#include "configMaster.h"
#include "SerialComm.h"
#include "Protocol.h"

// Decrarations and initialization
LedIndicator led(LEDPIN);
MqqtSettings_t mqqtSettings;
HardwareSerial serial(2);
SerialComm comm(serial);
Protocol protocol(&comm);
packet_t packet;
bool MqttComm::isConnected;
uint8_t numPacks = 0;
bool sendMQTTData(String sensor, int monitorNum, int value);

void debugPacket(packet_t *packet){
  Serial.println("Packet: ");
  Serial.println(packet->address);
  Serial.println(packet->request);
  Serial.println(packet->reg);
  Serial.println(packet->write);
  Serial.println(packet->value);
}

void setMonitorAddress() {
  packet.address = ADDRESS_BROADCAST;
  packet.reg = REG_ADDRESS;
  packet.request = true;
  packet.value = STARTADDRESS;
  packet.write = true;
  protocol.send(&packet);
  Serial.print("Sending Original Packet, number of packs: ");
  Serial.println(numPacks);
  int num_try = 0;
  while (numPacks == 0)
  {
    Serial.println("Waiting on monitors to show.");
    uint32_t now = millis();
    if(comm.update(now)){
      uint8_t *data = comm.data();
      packet_t responsePacket = protocol.getPacket(data, &packet);
      if(responsePacket.value != 0) {
        numPacks = responsePacket.value - 1;
      }
      Serial.print("Number of packs found: ");
      Serial.println(numPacks);
    }
    if (num_try > NUM_ADDRESS_ASSIG_TRY)
    {
      Serial.println("Number of address assigments exceded. Restarting...");
      delay(WAIT_TO_RESTARD);
      abort();
    } else {
      num_try++;
    }
    
  }
}

void setup() {
  Serial.begin(9600);
  serial.begin(9600);
  // setup network connection
  Serial.println("ESP32 Master is setting up...");
  mqqtSettings.ssid = (char*) SSID;
  mqqtSettings.wifiKey = (char*) WIFI_KEY;
  mqqtSettings.mqttServer = (char*) MQTT_SERVER;
  mqqtSettings.mqttServerPort = MQTT_SERVER_PORT;
  MqttComm mqttConnection(mqqtSettings, led);  
  // Setup monitors
  setMonitorAddress();
}
// Gets requested registry value from target pack using serial communication and protocol.
uint16_t getResponseValue(int reg, int targetPack)
{
  bool response = false;
  // Prep packet paylod to request voltage
  packet.address = targetPack;
  packet.request = true;
  packet.reg = reg;
  packet.write = 0x0;
  packet.value = 0x0;
  // Send the request
  protocol.send(&packet);
  uint16_t responseValue = 0;
  while (!response)
  {
    // listen to response
    uint32_t now = millis();
    if (comm.update(now))
    {
      uint8_t *data = comm.data();
      packet_t responsePacket = protocol.getPacket(data, &packet);
      response = true;
      responseValue = responsePacket.value;
    }
    else
    {
      Serial.println("No response yet");
      delay(1000);
    }
  }
  return responseValue;
}

void loop() {
  for (size_t i = 0; i < numPacks; i++)
  {
    // Sets the address of the pack
    int monitorNumber = i+1;
    Serial.println("Getting Voltage...");

    // gets serial communication using the registry and the address + some debug.
    uint16_t mV = getResponseValue(REG_VOLTAGE, monitorNumber);
    Serial.print("Voltage for pack "); Serial.print(monitorNumber); Serial.print(":");
    Serial.println(mV);
    MqttComm::sendMQTTData("v", monitorNumber, mV);
    delay(TIME_AFTER_MQTT_MSG);
    // gets serial communication using the registry and the address + some debug.
    Serial.println("Getting Temperature...");
    uint16_t temp = getResponseValue(REG_TEMP, monitorNumber);
    Serial.print("Temperature for pack "); Serial.print(monitorNumber); Serial.print(":");
    Serial.println(temp);
    MqttComm::sendMQTTData("t", monitorNumber, temp);
    delay(TIME_AFTER_MQTT_MSG);
  }
  
}