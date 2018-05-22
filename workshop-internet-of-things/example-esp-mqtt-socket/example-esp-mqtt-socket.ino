#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

const char* mqttServer = "iot.eclipse.org";
const char* mqttClientName = "ESP8266_socket";
const char* outStatus = "/socket02/device/status";
const char* outTopic = "/socket02/socket/status";
const char* inTopic = "/socket02/socket/command";

int relayPin = 12;
bool relayInitialState = LOW;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
  delay(10);
  WiFiManager wifiManager;
  if(!wifiManager.autoConnect("Please Connect Me To WIFI")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
    delay(5000);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Open the door
  if ((char)payload[0] == 'o') {
    digitalWrite(relayPin, !relayInitialState);
    client.publish(outTopic, "on");
  } else if ((char)payload[0] == 'O') {
    digitalWrite(relayPin, relayInitialState);
    client.publish(outTopic, "off");
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(mqttClientName, outStatus, 2, 1, "offline")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outStatus, "online");
      // ... and resubscribe
      client.subscribe(inTopic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // Initialize the relay pin as an output
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, relayInitialState);

  // Blink to indicate setup
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);

  Serial.begin(115200);

  // Connect to wifi
  setup_wifi();
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
