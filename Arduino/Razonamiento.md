# Desarrollo del '*programa_definitivo.ino*'

En este documento se detalla el funcionamiento del programa de Arduino.

Son necesarias las **librerías**:
  - Wire.h
  - MPU6050.h
  - I2Cdev.h

## SetUp

El SetUp tiene la forma:
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/3ec1909c-5a2e-47a7-b659-b6bda05bbcfb" width=600 heigt=300>

Donde se inicializa el sensor MPU6050 y se asigna el pin 16 (Vibration_signal) del sensor de vibración como entrada.

## Loop

Una vez dentro del bucle principal, obtenemos las lecturas de la rotación y de la aceleración:
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/e42afc8c-7d1c-4a4a-98b6-fa321014cadc" width=400 heigt=100>

Y se le asignan a las variables creadas previamente:

<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/5c9f5e09-9cc1-4ab5-80ac-49605efde076" width=400 heigt=100>

**NOTA:** Si vamos al [repositorio de la biblioteca MPU6050](https://github.com/ElectronicCats/mpu6050), dentro de 'src' encontramos la deifinición de los procedimientos en la carpeta MPU6050.cpp:

- **getRotation**
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/61b1f736-b5ac-4a50-8fb9-67414e7c6960" width=600 heigt=500>

- **getAcceleration**
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/ed7cf806-35cc-458a-8d7d-382aebef0202" width=600 heigt=500>

Donde se lee que las lecturas son de tipo entero de 16 bits y están en formato complemento a 2, por lo que después se les hace la conversión al sistema internacional: *EXPLICAR MAS*

- Rotación:
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/e77f92c8-dcf1-4098-8c06-d020185299f0" width=400 heigt=200>

- Aceleración:
<p align="center">
<img src="https://github.com/eromerol/Smart-Cube-ESP8266/assets/145780382/79c308a5-d7d2-46d0-870e-3205e1ab1006" width=400 heigt=200>

