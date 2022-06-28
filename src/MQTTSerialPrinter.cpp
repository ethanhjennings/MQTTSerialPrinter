// Logs to both mqtt and serial at the same time for debugging

#include <Arduino.h>
#include <ArduinoMqttClient.h>
#include "MQTTSerialPrinter.h"

MQTTSerialPrinter::MQTTSerialPrinter() : _mqtt_enabled(false) {}
  
MQTTSerialPrinter::MQTTSerialPrinter(MqttClient & mqtt_client, const char* topic)
    : _mqtt_client(&mqtt_client), _mqtt_enabled(true) {
  setMQTTTopic(topic);
  endMessage();
}

MQTTSerialPrinter::MQTTSerialPrinter(MqttClient & mqtt_client, const String& topic)
    : MQTTSerialPrinter(mqtt_client, topic.c_str()) {}
    
MQTTSerialPrinter::~MQTTSerialPrinter() {
  if (_topic != nullptr) {
    free(_topic);
  }
}

void MQTTSerialPrinter::setMQTTEnabled(bool enabled) {
  _mqtt_enabled = enabled;
}

void MQTTSerialPrinter::setMQTTTopic(const char* topic) {
  if (topic != nullptr) {
    free(_topic);
  }
  _topic = (char*)malloc(strlen(topic));
  strcpy(_topic, topic);
}

void MQTTSerialPrinter::setMQTTTopic(const String& topic) {
  setMQTTTopic(topic.c_str());
}

void MQTTSerialPrinter::beginMessage() {
  if (_mqtt_enabled) {
    _mqtt_client->beginMessage(_topic, false, _qos);
  }
  _send_on_newline = false;
}

void MQTTSerialPrinter::endMessage() {
  if (_mqtt_enabled) {
    _mqtt_client->endMessage(); // It's ok to run this if we haven't begun a message yet, it'll just do nothing
    _mqtt_client->beginMessage(_topic, false, _qos);
  }
  _send_on_newline = true;
}
