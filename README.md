# IoT-Indoor-Air-Quality-Monitoring-System
IoT-based indoor air quality monitoring system that uses an ESP32 programmed on Arduino IDE to collect telemetry data from air quality sensors and then transmit and display it at ThingsBoard. The air quality parameters this project monitors are temperature, humidity, CO2 levels, and PM2.5.

This repository contains the schematic, scripts, and libraries used for the project. The scripts in this repository include the main script running on the node and the each of the stand-alone scripts for each of the component of the node, i.e. each sensor used has their stand-alone script, Wi-Fi connection script, and the ThingsBoard connection script. The '_main_.ino' code is where each of these scripts are incorporated to work as the main script of the node.

## Guide
This section will show you how to use the '_main.ino_' sciprt used by the sensor node.

### Components
First, to use the script, make sure you have the following components:
- ESP32 Microcontroller (The exact microcontrolelr model used in this project is the ESP32-WROOM-32U)
- DHT22 sensor
- PMS7003 sensor
- MQ135 sensor
- External antenna (Optional, but highly recommended)

### Schematic
Follow the schematic below in connecting the components.

### Arduino IDE
Make sure that you have already installed Arduino IDE (Version is not important). After installation, add ESP32 as one of the group of boards since this environment is naturally for Arduinos. After that, select your appropriate development kit. 

#### Libraries
Now, to proceed on using the scripts, all the appropriate libraries must be installed. You can manually install them on the IDE but you can also find the libraries used here in the repository.
- WiFi library (included with the ESP32 package)
- DHT sensor library by Adafruit 
- MQ135 by Phoenix1747
- PMS Library by Mariusz Kacki
- EspSoftwareSerial by Dirk Kaar
- ThingsBoard  by ThingsBoard Team

##### Additional Note:
For the MQ135 library, there are modifications made on the version in the repository so it would be better to directly import the library here. To use it for your MQ135 sensor, do the following:
- If you are using a microcontroller that is not Arduino, go to the MQ135.cpp file in your library folder and change the original '1023' value to whatever the ADC resolution of your microcontroller is. For this project's case, the ESP32 has a 12-bits ADC so **'1023'** was replaced with **'4095'**
- You must also correct the default parameter **'rzero'** and **'rload'** value in the MQ135.h file in your library folder. You can find the  **'rzero'** of your MQ135 sensor by running the _'mq135_r0.ino'_ script for at least an hour in clean air, or around 414 CO2 ppm, and use the value it will stabilize on and replace the existing **'rzero'** in your MQ135.h file. For the **'rload'**, you can check it on the board of your MQ135 and replace the default value accordingly.

### ThingsBoard
The only thing you should change from this code with regards to ThingsBoard is the device access token you will gain from initiating a device on your ThingsBoard instance. You can find it on '_main.ino_', line 18.

Going back to the '_main.ino_' script, after following the previous instructions, you are ready to run the script and monitor.
