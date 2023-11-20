El cubo de Aqara es un dispositivo inteligente que se puede utilizar para controlar la domótica de tu hogar. Tiene seis lados, cada uno con un sensor diferente que puede detectar movimientos, gestos y sonidos.

El cubo de Aqara se puede utilizar para controlar una amplia gama de dispositivos inteligentes, como luces, termostatos, puertas y ventanas. También se puede utilizar para crear automatizaciones, como encender las luces cuando entras en una habitación o apagar el termostato cuando sales de casa.

Nuestra versión hecha a mano se encargará de las mismas caraterísticas, y en este documento te enseñaremos como hacerlo:

🔌 COMPONENTES Y BATERÍAS
--------------------------

<img align="right" src="https://github.com/isbkch/esp32-aws-weather-station/blob/master/docs/esp-32.jpg?raw=true" style="max-width:100%;" height="200">

*   [Placa ESP8266](https://amzn.eu/d/aHI7mCz)
*   [3 ejes acelerómetro giroscopio MPU-6050](https://amzn.eu/d/f57rFlo) 
*   [Sensor de Vibración SW-420](https://amzn.eu/d/2fdRQOv) 
*   [Baterías y Cargador LiPo](https://amzn.eu/d/7FSwxyN)
*   [Placa de pruebas](https://amzn.eu/d/8O4BMnn)
*   [Cables Dupont](https://amzn.eu/d/6FpwoLk)

👨‍💻​APPS
------

*   [Arduino IDE](https://www.arduino.cc/en/software)
*   [Fritzing](https://fritzing.org/)
*   [Node-RED](https://nodered.org/)
*   [Mongo DB](https://www.mongodb.com/es)
*   [Telegram](https://web.telegram.org/)

📚LIBRERÍAS
==========
*   [Adafruit_MPU6050](https://github.com/adafruit/Adafruit_MPU6050)
*   [Adafruit_Sensor](https://github.com/adafruit/Adafruit_Sensor)
*   [PubSubClient](https://www.arduinolibraries.info/libraries/pub-sub-client)
*   [Wire](https://github.com/espressif/arduino-esp32/tree/master/libraries/Wire)
*   [WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi)



DESCRIPCIÓN DEL HARDWARE
==============================
* MPU6050
El MPU-6050 es un sensor de movimiento de 6 ejes que combina un acelerómetro de 3 ejes y un giroscopio de 3 ejes en un solo chip. Es una opción popular para diversas aplicaciones, como robótica, drones y realidad virtual.

El MPU-6050 puede medir la aceleración y la velocidad angular en tres ejes: x, y y z. Estos datos se van a utilizar para seguir el movimiento de un objeto, como la posición, orientación o rotación de nuestro cubo mágico.

Info vía: http://www.sinaptec.alomar.com.ar/2017/10/tutorial-23-esp8266-obtener-inclinacion.html
