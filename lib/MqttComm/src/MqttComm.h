#ifndef __MQTTCOMM_h__
#define __MQTTCOMM_h__

#include "Arduino.h"
#include "WiFi.h"
#include "PubSubClient.h"
#include "LedIndicator.h"

#define TOPIC "Powerwall/Pack-" // topic prefix to publish
#define CLIENT_NAME "esp32-Powerwall" // MQTT Client name
#define SEND_DELAY 250

typedef struct {
  char* ssid;
  char* wifiKey;
  char* mqttServer;
  int mqttServerPort;
} MqqtSettings_t;

class MqttComm
{
private:
    char* _topic;
    char* _clientName;
public:
    MqttComm(MqqtSettings_t mqqtSettings, LedIndicator led);
    void WifiInit(MqqtSettings_t settings);
    bool isMqttConnected();
    static bool isConnected;
    static void sendMQTTData(String sensor, int monitorNum, int value);
};
#endif