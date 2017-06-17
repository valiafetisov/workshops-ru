#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// номер пина с подключенным к нему светодиодом
int out_pin = 16;

// Данные для подключению к WIFI
const char* wifiSSID = "";
const char* wifiPassword = "";

// Данные для подключения к MQTT-серверу
const char* mqttServer = "";
const char* mqttClientName = ""; // это имя нужно придумать
const char* mqttUser = "";
const char* mqttPassword = "";

// топик, в который мы будем отправлять сообщения
const char* outTopic = "/rodchenko/movie/status";
// топик, в котором мы будем слушать сообщения
const char* inTopic = "/rodchenko/movie/bzz";

// вспомогательные переменные
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

// функция, которая подключает WIFI
void setup_wifi() {
  delay(10);

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifiSSID);

  WiFi.begin(wifiSSID, wifiPassword);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
  delay(500);
  digitalWrite(13, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// функция, которая вызывается при получении нового сообщения
// в топик, на который мы подписались
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print((char*)payload);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // если сообщение == символу
  if ((char)payload[0] == '1') {
    Serial.println("switching on");
    digitalWrite(out_pin, LOW);
  } else if ((char)payload[0] == '0') {
    Serial.println("switching off");
    digitalWrite(out_pin, HIGH);
  }
}

void reconnect() {
  // выполняем цикл, пока не подключимся к mqtt-серверу
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // подключаемся к серверу, отправляя Last Will сообщение "offline"
    int isMqttConnected = client.connect(mqttClientName, mqttUser, mqttPassword, outTopic, 2, 1, "offline");

    if (isMqttConnected) {
      // как только подключаемся, публикуем сообщение 'online'
      Serial.println("connected");
      client.publish(outTopic, "online");
      // и подписываемся на обновления топика
      client.subscribe(inTopic);
    } else {
      // в противном случае ждем 5 секунд и подключаемся снова
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  // запускаем seiral
  Serial.begin(115200);

  // инициализируем out_pin как выход
  pinMode(out_pin, OUTPUT);

  // выключаем out_pin (так как он подключен к плюсу)
  digitalWrite(out_pin, HIGH);

  // подключаемся к wifi
  setup_wifi();

  // подключаемся к mqtt-серверу
  client.setServer(mqttServer, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
