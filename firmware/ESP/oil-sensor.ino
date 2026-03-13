
#include <WiFi.h>
#include <PubSubClient.h>

// WiFi
const char* ssid = "Sedimat";
const char* password = "WelcomeSedimat38!!";

// MQTT
const char* mqtt_server = "192.168.3.51";
const int mqtt_port = 1883;
const char* mqtt_topic = "camion/donnees";
const char* mqtt_client_id = "esp32_huile_1";

WiFiClient espClient;
PubSubClient mqttClient(espClient);

// Broches
const int ledPin = 2;
const int buzzerPin = 4;

// Filtre
float tensionFiltree = 0.0;
bool filtreInit = false;
const float alpha = 0.2;

// Calibration
const int nbPoints = 4;
float tensions[nbPoints] = {0.05, 0.23, 0.39, 0.60};
float volumes[nbPoints]  = {5.0, 10.0, 15.0, 20.0};

float interpolationSegment(float x) {
  for (int i = 0; i < nbPoints - 1; i++) {
    if (x >= tensions[i] && x <= tensions[i + 1]) {
      return volumes[i] + (x - tensions[i]) * (volumes[i + 1] - volumes[i]) / (tensions[i + 1] - tensions[i]);
    }
  }
  return -1.0;
}

void connectWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) delay(500);
}

void connectMQTT() {
  while (!mqttClient.connected()) {
    mqttClient.connect(mqtt_client_id);
    delay(1000);
  }
}

void publishVolume(float volume) {
  if (!mqttClient.connected()) connectMQTT();

  String msg = "{\"device_id\":\"esp32_test\",\"volume\":" + String(volume, 2) + "}";
  mqttClient.publish(mqtt_topic, msg.c_str());
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  digitalWrite(buzzerPin, LOW);

  connectWiFi();
  mqttClient.setServer(mqtt_server, mqtt_port);
  connectMQTT();
}

void loop() {
  mqttClient.loop();

  float tension = 0.2;  // simulation

  if (!filtreInit) {
    tensionFiltree = tension;
    filtreInit = true;
  } else {
    tensionFiltree = alpha * tension + (1.0 - alpha) * tensionFiltree;
  }

  float tClamp = constrain(tensionFiltree, tensions[0], tensions[nbPoints - 1]);
  float volume = interpolationSegment(tClamp);

  if (volume >= 10.0) {
    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
  }

  publishVolume(volume);
  delay(3000);
}
