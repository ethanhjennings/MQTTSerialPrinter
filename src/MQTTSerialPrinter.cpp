// Logs to both mqtt and serial at the same time for debugging

#include <Arduino.h>
#include "MQTTSerialPrinter.h"
#include "StringPrinter.h"

MQTTSerialPrinter::MQTTSerialPrinter(bool serial, int buff_size)
  : _mqtt_enabled(false), _serial_enabled(serial), _printer(buff_size) {}

#ifdef PUBSUB
  MQTTSerialPrinter::MQTTSerialPrinter(PubSubClient & mqtt_client, const char* topic, bool serial, int buff_size)
    : _pubsub_mqtt_client(&mqtt_client), _mqtt_enabled(true), _serial_enabled(serial), _printer(buff_size) {
    setMQTTTopic(topic);
  }

  MQTTSerialPrinter::MQTTSerialPrinter(PubSubClient & mqtt_client, const String& topic, bool serial, int buff_size)
    : MQTTSerialPrinter(mqtt_client, topic.c_str(), serial, buff_size) {}
#endif

#ifdef ARDUINO_MQTT
  MQTTSerialPrinter::MQTTSerialPrinter(MqttClient & mqtt_client, const char* topic, bool serial, int buff_size)
    : _arduino_mqtt_client(&mqtt_client), _mqtt_enabled(true), _serial_enabled(serial), _printer(buff_size) {
    setMQTTTopic(topic);
  }

  MQTTSerialPrinter::MQTTSerialPrinter(MqttClient & mqtt_client, const String& topic, bool serial, int buff_size)
    : MQTTSerialPrinter(mqtt_client, topic.c_str(), serial, buff_size) {}
#endif

MQTTSerialPrinter::~MQTTSerialPrinter() {
  if (_topic != nullptr) {
    delete[] _topic;
  }
}

void MQTTSerialPrinter::setMQTTEnabled(bool enabled) {
  _mqtt_enabled = enabled;
}

void MQTTSerialPrinter::setSerialEnabled(bool enabled) {
  _serial_enabled = enabled;
}

void MQTTSerialPrinter::setMQTTTopic(const char* topic) {
  if (topic != nullptr) {
    delete[] _topic;
  }
  _topic = new char[strlen(topic)];
  strcpy(_topic, topic);
}

void MQTTSerialPrinter::setMQTTTopic(const String& topic) {
  setMQTTTopic(topic.c_str());
}

void MQTTSerialPrinter::beginMessage() {
  _send_on_newline = false;
}

void MQTTSerialPrinter::endMessage() {
  if (_printer.has_data()) {
    char* buff = _printer.get_buff();
    _printer.reset();

    if (_mqtt_enabled) {
      // Remove final newline since it isn't needed for MQTT
      size_t buff_len = strlen(buff);
      if (buff[buff_len-1] == '\n') {
        buff[buff_len-1] = '\0';
      }

      _publish(buff);
    }

    delete[] buff;
  }
  _send_on_newline = true;
}

void MQTTSerialPrinter::_handle_newline() {
  // Maybe send on newlines
  _printer.print('\n');
  if (_send_on_newline) {
    endMessage();
  }
}