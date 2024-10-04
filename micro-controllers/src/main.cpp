#include <WiFi.h>
#include <PubSubClient.h>

#define TRIG_PIN 5
#define ECHO_PIN 18

// WiFi credentials
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// MQTT broker details
const char *mqttServer = "192.168.1.86";
const int mqttPort = 1883;
const char *mqttUser = "";     // Set this if your MQTT broker requires username
const char *mqttPassword = ""; // Set this if your MQTT broker requires password

WiFiClient espClient;
PubSubClient client(espClient);

// Function to connect to WiFi
void setup_wifi()
{
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to connect to MQTT broker
void connectToMQTT()
{
  while (!client.connected())
  {
    Serial.print("Connecting to MQTT broker...");

    // Attempt to connect
    if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {
      Serial.println("connected.");
    }
    else
    {
      Serial.print("failed with state ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

void setup()
{
  // Connect to WiFi
  setup_wifi();

  // Set MQTT broker and port
  client.setServer(mqttServer, mqttPort);

  // Connect to the MQTT broker
  connectToMQTT();

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

void loop()
{
  // Ensure the connection is alive
  if (!client.connected())
  {
    connectToMQTT();
  }
  client.loop();

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH);

  float distance = (duration * 0.0343) / 2;

  String message = String(distance, 2) + " - centimeters";
  Serial.print("Publishing distance: ");
  Serial.println(message);

  client.publish("esp32/output", message.c_str());

  delay(500);
}
