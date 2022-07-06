// Logs to both mqtt and serial at the same time for debugging

#include <Arduino.h>
#include "MQTTSerialPrinter.h"
#include "StringPrinter.h"

MQTTSerialPrinter::MQTTSerialPrinter(bool serial, unsigned int buff_size)
  : _mqtt_enabled(false), _serial_enabled(serial), _printer(buff_size) {}

#ifdef PUBSUB
  MQTTSerialPrinter::MQTTSerialPrinter(PubSubClient & mqtt_client, const char* topic, bool serial, unsigned int buff_size)
    : _pubsub_mqtt_client(&mqtt_client), _mqtt_enabled(true), _serial_enabled(serial), _printer(buff_size) {
    setMQTTTopic(topic);
  }

  MQTTSerialPrinter::MQTTSerialPrinter(PubSubClient & mqtt_client, const String& topic, bool serial, unsigned int buff_size)
    : MQTTSerialPrinter(mqtt_client, topic.c_str(), serial, buff_size) {}
#endif

#ifdef ARDUINO_MQTT
  MQTTSerialPrinter::MQTTSerialPrinter(MqttClient & mqtt_client, const char* topic, bool serial, unsigned int buff_size)
    : _arduino_mqtt_client(&mqtt_client), _mqtt_enabled(true), _serial_enabled(serial), _printer(buff_size) {
    setMQTTTopic(topic);
  }

  MQTTSerialPrinter::MQTTSerialPrinter(MqttClient & mqtt_client, const String& topic, bool serial, unsigned int buff_size)
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
  if (_printer.length() > 0) {
    const uint8_t* buff = _printer.get_buff();
    unsigned int length = _printer.length();
    _printer.reset();

    if (_mqtt_enabled) {
      if (buff[length-1] == '\n') {
        // Ignore final newline since it will mess up MQTT formatting
        length -= 1;
      }
      _publish(buff, length);
    }
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

void MQTTSerialPrinter::_publish(const uint8_t * buff, unsigned int length) {
  #ifdef PUBSUB
    if (_pubsub_mqtt_client != nullptr) {
      _pubsub_mqtt_client->publish(_topic, buff, length);
    }
  #endif
  #ifdef ARDUINO_MQTT
    if (_arduino_mqtt_client != nullptr) {
      _arduino_mqtt_client->beginMessage(_topic, length, false, QOS);
      _arduino_mqtt_client->write(buff, length);
      _arduino_mqtt_client->endMessage();
    }
  #endif
}
