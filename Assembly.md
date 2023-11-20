The Aqara Cube is a smart device that can be used to control your home. It has six sides, each with a different sensor that can detect motion, gestures, and sounds.

The Aqara Cube can be used to control a wide range of smart devices, such as lights, thermostats, doors, and windows. It can also be used to create automations, such as turning on the lights when you enter a room or turning off the thermostat when you leave home.

Our handmade version will take care of the same features and in this document we will show you how to do it:

🔌 COMPONENTS AND SUPPLIES
--------------------------

<img align="right" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/esp-32.jpg?raw=true" style="max-width:100%;" height="200">

*   [ESP8266 Board](https://amzn.eu/d/aHI7mCz)
*   [3 Axis Accelerometer Gyroscope MPU-6050](https://amzn.eu/d/f57rFlo) 
*   [Vibration Sensor SW-420](https://amzn.eu/d/2fdRQOv) 
*   [5 LiPo Batteries And Charger](https://amzn.eu/d/7FSwxyN)
*   [Breadboard](https://amzn.eu/d/8O4BMnn)
*   [Jumper Wires](https://amzn.eu/d/6FpwoLk)

👨‍💻​APPS
------

*   [Arduino IDE](https://www.arduino.cc/en/software)
*   [Fritzing](https://fritzing.org/)
*   [Node-RED](https://nodered.org/)
*   [Mongo DB](https://www.mongodb.com/es)
*   [Telegram](https://web.telegram.org/)

📚Libraries
=====
*   [Adafruit_MPU6050](https://github.com/adafruit/Adafruit_MPU6050)
*   [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
*   [PubSubClient](https://www.arduinolibraries.info/libraries/pub-sub-client)
*   [Wire](https://github.com/espressif/arduino-esp32/tree/master/libraries/Wire)
*   [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)



COMPONENTS DESCRIPTION
==============================
* MPU6050
The MPU-6050 is a 6-axis motion sensor that combines a 3-axis accelerometer and a 3-axis gyroscope in a single chip. It is a popular choice for a variety of applications, including robotics, drones, and virtual reality.

The MPU-6050 can measure acceleration and angular velocity in three axes: x, y, and z. This data is going to be used to track the movement of an object, such as the position, orientation, or rotation of our magic cube.

Info via: http://www.sinaptec.alomar.com.ar/2017/10/tutorial-23-esp8266-obtener-inclinacion.html
