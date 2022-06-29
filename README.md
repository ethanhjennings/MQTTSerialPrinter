
# MQTT and Serial Printer
Arduino library that allows you to print to both serial and MQTT at the same time. This way you can debug your projects over either wifi, usb, or both at the same time without changing the code. This has an interface identical to the `Serial` class (plus extras) to make things easier, but you can still use `Serial` alongside it.

> Note: This library uses ArduinoMqttClient and support for PubSubClient and others is a WIP.

# Installation
Install via the library manager in the Arduino IDE. `Tools -> Manage Libraries...`. Then search for "MQTT and Serial Printer".

You also need an MQTT broker running somewhere on your network (like a raspberry pi). I recommend [mosquitto](https://mosquitto.org/).

# Example Usage

Setup:
```c++
#include <WiFi.h>
#include <ArduinoMqttClient.h>
#include <MQTTSerialPrinter.h>

WiFiClient wifi_client;
MqttClient mqtt_client(wifi_client);
MQTTSerialPrinter Log(mqtt_client, "my_topic");

// Serial printing now works, connect to wifi and mqtt for mqtt printing to also work. See the examples.
```

Sending single message:
```c++
Log.println("Example status message.");
```

Sending single message using multiple function calls:
```c++
Log.print("Example data value: ");
Log.println(42);
```

You can also send multi-line messages with `beginMessage()` and `endMessage()`:
```c++
Log.beginMessage();
Log.println("This is a multi-line message!");
Log.print("Here's some extra data:");
Log.println(42);
Log.endMessage(); // Don't forget to end the message!
```

Newline chars still work too!
```c++
Log.println("This is a multi-line message!\nHere's another line!");
```

There's also a formatting function based on strprintf!
```c++
Log.printlnf("Example formatting with data: %d", 42);
```

If you decide you're done debugging and don't want to send excess messages over the network anymore, you can simply disable MQTT printing without affecting serial printing with:
```c++
Log.setMQTTEnabled(false);
```

# Contributing and future
Right now this library only supports the official ArduinoMQTTClient, but I would like to support more. Feel free to open a pull request with support for other MQTT libraries. It ideally should support QoS 2 because any dropped or repeated messages make debugging more confusing.
