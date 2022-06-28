// Logs to both mqtt and serial at the same time for debugging

#ifndef _MQTT_SERIAL_LOGGER_H_
#define _MQTT_SERIAL_LOGGER_H_

#include <Arduino.h>
#include <ArduinoMqttClient.h>

class MQTTSerialPrinter {
 public:
  MQTTSerialPrinter();
  MQTTSerialPrinter(MqttClient & mqtt_client, const char* topic);
  MQTTSerialPrinter(MqttClient & mqtt_client, const String& topic);
  
  ~MQTTSerialPrinter();

  void setMQTTEnabled(bool enabled);
  
  void setMQTTTopic(const char* topic);
  void setMQTTTopic(const String& topic);
  
  void beginMessage();
  void endMessage();


  template <typename T>
  void print(const T& print_arg) {
    if (_mqtt_enabled) {
      _mqtt_client->print(print_arg);
    }
    Serial.print(print_arg);
  }

  template <typename T>
  void print(const T& print_arg, int base) {
    if (_mqtt_enabled) {
      _mqtt_client->print(print_arg, base);
    }
    Serial.print(print_arg, base);
  }
  
  template <typename T>
  void println(const T& print_arg) {
    if (_mqtt_enabled) {
      if (_send_on_newline) {
        _mqtt_client->print(print_arg);
        endMessage();
      } else {
        _mqtt_client->println(print_arg);
      }
    }
    Serial.println(print_arg);
  }

  template <typename T>
  void println(const T& print_arg, int base) {
    if (_mqtt_enabled) {
      if (_send_on_newline) {
        _mqtt_client->print(print_arg, base);
        endMessage();
      } else {
        _mqtt_client->println(print_arg, base);
      }
    }
    Serial.println(print_arg);
  }
  
  template <typename... T>
  void printf(const String& str, T... args) {
    char buff[str.length()];
    snprintf(buff, str.length(), str.c_str(), args...);
    if (_mqtt_enabled) {
      _mqtt_client->print(buff);
    }
    Serial.print(buff);
  }
  
  template <typename... T>
  void printlnf(const String& str, T... args) {
    char buff[str.length()];
    snprintf(buff, str.length(), str.c_str(), args...);
    if (_mqtt_enabled) {
      if (_send_on_newline) {
        _mqtt_client->print(buff);
        endMessage();
      } else {
        _mqtt_client->println(buff);
      }
    }
    Serial.println(buff);
  }

 private:
  static const int _qos = 2; // Force QoS 2 because dropped and duplicate messages interfere with user debugging experience
  
  MqttClient* _mqtt_client;
  char* _topic = nullptr;
  bool _mqtt_enabled;
  bool _send_on_newline = true;
};

#endif
