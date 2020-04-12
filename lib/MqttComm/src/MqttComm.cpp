#include "MqttComm.h"
#include <LedIndicator.h>
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient _wifiClient; // declaring wiFiClient
PubSubClient client(_wifiClient); // setting the MQTT Client.

MqttComm::MqttComm(MqqtSettings_t settings, LedIndicator led){
    isConnected = false;
    _topic = (char*) TOPIC;
    _clientName = (char*) CLIENT_NAME;
    WifiInit(settings);
    client.setServer(settings.mqttServer, settings.mqttServerPort);
    // Connect Client
    
    if (client.connect(_clientName)) {
        Serial.println("Connected to MQTT broker");
        isConnected = true;
    }
    else {
        Serial.println("MQTT connect failed");
        isConnected = false; // caller should abord to try again
        // abort();
    }
}

void MqttComm::WifiInit(MqqtSettings_t settings){
    WiFi.begin(settings.ssid, settings.wifiKey);
    Serial.print("Connecting to network");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\n Connected to Network");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void MqttComm::sendMQTTData(String sensor, int monitorNum, int value){
    char valueString[5];
    // Debug
    String fullTopicString = TOPIC + sensor + "-" + monitorNum;
    const char* fullTopic = fullTopicString.c_str();
    itoa( value, valueString, 10 );
    Serial.println(valueString);
    bool success = client.publish(fullTopic, valueString);
    if(success)
    {
        Serial.print("Topic: "); Serial.print(fullTopic); 
        Serial.print(" with value: "); Serial.print(valueString);
        Serial.println(" published!");
    } else {
        Serial.print("FAIL to pusblish topic: "); Serial.print(fullTopic); 
        Serial.print(" with value: "); Serial.print(valueString);
        Serial.println("... moving on...");
    }
}