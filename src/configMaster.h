// Configuration
#define LEDPIN 2 // Led pin for visual debug
#define SSID "PerezNieto"; // SSID of Wifi network
#define WIFI_KEY "denatyes11"; // password for the network.
#define MQTT_SERVER "192.168.0.4"; // MQTT Broker IP
#define MQTT_SERVER_PORT 1883; // MQTT Broker Port
#define STARTADDRESS 1 // address to be assigned to the first monitor
#define NUM_ADDRESS_ASSIG_TRY 10 // should try 10 time to reach the monitors otherwise it should restart.
#define WAIT_TO_RESTARD 3000 // how much time should wait in case a restart is needed.
#define TIME_AFTER_MQTT_MSG 200 // how much time should wait before attempting to send another message.