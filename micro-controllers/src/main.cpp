#include <WiFi.h>
#include <PubSubClient.h>

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
}

void loop()
{
  // Ensure the connection is alive
  if (!client.connected())
  {
    connectToMQTT();
  }
  client.loop();

  // Publish a message every 3 seconds
  String message = "Hello from ESP32";
  Serial.print("Publishing message: ");
  Serial.println(message);

  // Publish the message to the topic "esp32/output"
  client.publish("esp32/output", message.c_str());

  delay(3000); // Wait 3 seconds before sending the next message
}
