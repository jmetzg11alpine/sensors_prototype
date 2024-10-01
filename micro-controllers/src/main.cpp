#include <WiFi.h>
#include <PubSubClient.h>

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;
const char *mqttServer = "192.168.1.86";
const int mqttPort = 1883;
const char *mqttUser = "";
const char *mqttPassword = "";

WiFiClient espClient;
PubSubClient client(espClient);

const char *items[] = {"dog", "cat", "bird", "lizard"};
int currentItem = 0;

IPAddress local_IP(192, 168, 1, 91); // Set this to an available IP address
IPAddress gateway(192, 168, 1, 1);   // Replace with your router's gateway
IPAddress subnet(255, 255, 255, 0);  // Most common subnet mask

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  // Configure static IP
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Function to connect to the MQTT broker
void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP32Client", mqttUser, mqttPassword))
    {
      Serial.println("connected to MQTT broker");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  setup_wifi();
  // client.setServer(mqttServer, mqttPort);
}

void pingServer()
{
  Serial.print("Pinging ");
  Serial.print(mqttServer);
  Serial.print(" on port ");
  Serial.println(mqttPort);
  Serial.println(WiFi.localIP());

  if (espClient.connect(mqttServer, mqttPort))
  {
    Serial.println("Ping successful: Connected to target!");
    espClient.stop(); // Close the connection
  }
  else
  {
    Serial.println("Ping failed: Could not connect to target.");
  }
}

void loop()
{
  // if (!client.connected())
  // {
  //   reconnect();
  // }

  // Process MQTT tasks
  // client.loop();
  // String message = "{\"mac_address\": \"" + String(items[currentItem]) + "\"}";
  // Serial.print("Publishing message: ");
  // Serial.println(message);

  // client.publish("devices/mac", message.c_str());
  // currentItem = (currentItem + 1) % 4;

  delay(6000);
  pingServer();
}
