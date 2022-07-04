#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include <MQTTSerialPrinter.h>

// Edit these fields to match your own network setup
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
const char* mqtt_broker = "YOUR_MQTT_BROKER"; // can be ip or domain name
const char* mqtt_topic = "YOUR_TOPIC";
const int   mqtt_port     = 1883;

WiFiClient wifi_client;
MqttClient mqtt_client(wifi_client);
MQTTSerialPrinter Log(mqtt_client, mqtt_topic);

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Log.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }
  Log.println("Connected!");
}

void setup_mqtt() {
  if (!mqtt_client.connected()) {
    Log.print("Connecting to MQTT...");
    if (!mqtt_client.connect(mqtt_broker, mqtt_port)) {
      Log.print("MQTT connection failed! Error code = ");
      Log.println(mqtt_client.connectError());
    } else {
      Log.println("Connected to MQTT broker!");
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);
  
  setup_wifi();
  setup_mqtt();
}

void loop() {
  // Sending simple message
  Log.println("Example status message.");

  // Sending single message using multiple function calls
  Log.print("Example data value: ");
  Log.println(42);

  // Sending single multi-line message
  Log.beginMessage();
  Log.println("This is a multi-line message!");
  Log.print("Here's some extra data: ");
  Log.println(42); // Last newline is used in serial but ignored in MQTT.
  Log.endMessage(); // Don't forget to end the message!

  // Newline chars still work as well
  Log.println("This is a multi-line message!\nHere's another line!");

  // Extra formatting function for fun:
  Log.printlnf("Example string formatting with data: %d", 42);
  
  // Keep the connection alive and recieve incoming data
  // (not strictly needed in this sample, but good practice)
  mqtt_client.poll();

  delay(2000);
}