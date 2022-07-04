// Logs to both mqtt and serial at the same time for debugging

#ifndef _MQTT_SERIAL_LOGGER_H_
#define _MQTT_SERIAL_LOGGER_H_

#include <Arduino.h>
#include "StringPrinter.h"

// Preprocessor crap to handle multiple MQTT library options
#if __has_include(<PubSubClient.h>)
  #include <PubSubClient.h>
  #define PUBSUB
#endif

#if __has_include(<ArduinoMqttClient.h>)
  #include <ArduinoMqttClient.h>
  #define ARDUINO_MQTT
#endif

#if !__has_include(<PubSubClient.h>) && !__has_include(<ArduinoMqttClient.h>)
  #error "You need to install either PubSubClient or ArduinoMqttClient for MQTTSerialPrinter.h to work"
#endif

#define DEFAULT_BUFF_SIZE 2048

class MQTTSerialPrinter {
private:
  #ifdef ARDUINO_MQTT
    static const int QOS = 2; // Force QoS 2 because dropped and duplicate messages interfere with user debugging experience
  #endif
  // PubSubClient only supports qos 0 for publishing

  #ifdef PUBSUB
    PubSubClient* _pubsub_mqtt_client = nullptr;
  #endif

  #ifdef ARDUINO_MQTT
    MqttClient* _arduino_mqtt_client = nullptr;
  #endif

  int _mqttClientLib;
  char* _topic = nullptr;
  bool _mqtt_enabled;
  bool _serial_enabled;

  bool _send_on_newline = true;
  StringPrinter _printer;

  void _handle_newline(); // Maybe send on newlines

  void _publish(char * buff) {
    #ifdef PUBSUB
      if (_pubsub_mqtt_client != nullptr) {
        _pubsub_mqtt_client->publish(_topic, buff);
      }
    #endif
    #ifdef ARDUINO_MQTT
      if (_arduino_mqtt_client != nullptr) {
        _arduino_mqtt_client->beginMessage(_topic, false, QOS);
        _arduino_mqtt_client->print(buff);
        _arduino_mqtt_client->endMessage();
      }
    #endif
  }

public:
  MQTTSerialPrinter(bool serial=true, int buff_size=DEFAULT_BUFF_SIZE);

  #ifdef PUBSUB
    MQTTSerialPrinter(PubSubClient & mqtt_client, const char* topic, bool serial=true, int buff_size=DEFAULT_BUFF_SIZE);
    MQTTSerialPrinter(PubSubClient & mqtt_client, const String& topic, bool serial=true, int buff_size=DEFAULT_BUFF_SIZE);
  #endif

  #ifdef ARDUINO_MQTT
    MQTTSerialPrinter(MqttClient & mqtt_client, const char* topic, bool serial=true, int buff_size=DEFAULT_BUFF_SIZE);
    MQTTSerialPrinter(MqttClient & mqtt_client, const String& topic, bool serial=true, int buff_size=DEFAULT_BUFF_SIZE);
  #endif

  ~MQTTSerialPrinter();

  void setMQTTEnabled(bool enabled);
  void setSerialEnabled(bool enabled);

  void setMQTTTopic(const char* topic);
  void setMQTTTopic(const String& topic);

  void beginMessage();
  void endMessage();

  // We could just directly inherit from Printer instead of using these templates, but
  // then we wouldn't be able to detect when print() is called with a newline vs println() proper.
  template <typename T>
  void print(const T& print_arg) {
    _printer.print(print_arg);
    if (_serial_enabled) Serial.print(print_arg);
  }

  template <typename T>
  void print(const T& print_arg, int base) {
    _printer.print(print_arg, base);
    if (_serial_enabled) Serial.print(print_arg, base);
  }

  // No args, so don't need template
  void println() {
    _handle_newline();
    if (_serial_enabled) Serial.println();
  }

  template <typename T>
  void println(const T& print_arg) {
    _printer.print(print_arg);
    _handle_newline();
    if (_serial_enabled) Serial.println(print_arg);
  }

  template <typename T>
  void println(const T& print_arg, int base) {
    _printer.print(print_arg, base);
    _handle_newline();
    if (_serial_enabled) Serial.println(print_arg, base);
  }

  template <typename... T>
  void printf(const String& str, T... args) {
    char buff[str.length()+1];
    snprintf(buff, str.length()+1, str.c_str(), args...);
    _printer.print(buff);
    if (_serial_enabled) Serial.print(buff);
  }

  template <typename... T>
  void printlnf(const String& str, T... args) {
    char buff[str.length()+1];
    snprintf(buff, str.length()+1, str.c_str(), args...);
    _printer.print(buff);
    _handle_newline();
    if (_serial_enabled) Serial.println(buff);
  }
};

#endif
