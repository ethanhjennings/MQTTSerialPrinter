# MQTT and Serial Printer
Arduino library that allows you to print to both serial and MQTT at the same time. This way you can debug your projects over either wifi, usb, or both at the same time without changing the code. This has an interface identical to the `Serial` class (plus extras) to make things easier, but you can stil use `Serial` alongside it.

# Installation
You can download this repo as a .zip and import it into the Arduino IDE with the menu option `Sketch -> Include Library -> Add .zip Library`. You also need to install the official ArduinoMQTTClient library from the library manager.

You also need an MQTT broker running somewhere on your network (like a raspberry pi). I recommend [mosquitto](https://mosquitto.org/).

# Example Usage
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

# Contributing and future
Right now this library only supports the official ArduinoMQTTClient, but I would like to support more. Feel free to open a pull request with support for other MQTT libraries. It ideally should support QoS 2 because any dropped or repeated messages make debugging more confusing.
