#include <Wire.h>
#include <MPU6050.h> //Biblioteca sensor
#include <I2Cdev.h>  //Biblioteca comunicacion I2C
#include "Button2.h" //Biblioteca pulsaciones
#include <ArduinoJson.h> //Biblioteca JSON
#include <PubSubClient.h> //Biblioteca MQTT
#include <ESP8266WiFi.h> // Biblioteca WIFI ESP8266

/////////////////////////////////////////////////////////////////

#define BUTTON_PIN  12
#define MPU 0x68

/////////////////////////////////////////////////////////////////
Button2 button;
MPU6050 mpu;

// Inicializamos el cliente WiFi
WiFiClient espClient;
PubSubClient mqtt_client(espClient);
/////////////////////////////////////////////////////////////////

// Definimos pines generales
int pinled = 13;
int led = 2;
int Vibration_signal = 14;


// Declaramos variables
int brillo = 0;
float temperatura;
bool estado_stop=false;
bool estado_play=false;
bool skiptoprevious=false;
bool skiptonext=false;
float temperatura;
int termostato=0;

// Definimos datos de la red WiFi
// const String ssid = "dlink-AE58";
// const String password = "ceyza33866";
 const String ssid = "infind";
 const String password = "1518wifi";

// Definimos el servidor MQTT
const String mqttServer = "iot.ac.uma.es";
const String mqttUser = "infind";
const String mqttPassword = "zancudo";

// Definimos String
String ID_PLACA;
String topic_conexion;
String topic_Spotify;
String topic_Alexa;

// Variables del acelerometro
int16_t ax, ay, az; 

// Variables del giroscopio
int16_t gx, gy, gz;
int16_t temp_raw;

long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y, girosc_ang_z;
float girosc_ang_x_prev, girosc_ang_y_prev, girosc_ang_z_prev;

float umbral = 50.0; // Umbral para detección de giro
//-------------------------------------------------------
//                     CONECTA WIFI
//-------------------------------------------------------
void conecta_wifi() {
  Serial.println("Conectando a " + ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi conectado, direccion IP: " + WiFi.localIP().toString());
}

//-------------------------------------------------------
//                     CONECTA MQTT
//-------------------------------------------------------
void conecta_mqtt() {
  // Bucle hasta que estemos reconectados

  while (!mqtt_client.connected()) {
    Serial.println("Conectando al servidor MQTT...");

    // Intentamos conectarnos
    if (mqtt_client.connect("ESP8266Client", mqttUser.c_str(), mqttPassword.c_str(), topic_conexion.c_str(), 1, true, "\"online\": false")) {
      Serial.println("Conectado a: " + mqttServer);

      // Enviamos mensaje de conexión al topic topic_conexion.c_str
      mqtt_client.publish(topic_conexion.c_str(), "\"online\": true", true); // Mensaje retenido

    } else {
       Serial.println("ERROR:"+ String(mqtt_client.state()) +" reintento en 5s" );

      // Esperamos 2 segundos antes de volver a intentarlo
      delay(5000);
    }
  }
}
//-------------------------------------------------------
//                      SETUP
//-------------------------------------------------------
void setup(void) {
  Serial.begin(115200);

  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.endTransmission(true);

  pinMode(pinled, OUTPUT);
  pinMode(Vibration_signal, INPUT); //Set pin as input

  mpu.initialize();

  button.begin(BUTTON_PIN);
  button.setClickHandler(singleClick);
  button.setLongClickHandler(longClick);
  button.setDoubleClickHandler(doubleClick);
  button.setTripleClickHandler(tripleClick);
  
  ID_PLACA = "ESP" + String(ESP.getChipId()); //Obtenemos el chipID

  //Creamos los topics
  topic_conexion="II7/"+ ID_PLACA +"/conexion"; //Publicar
  topic_Alexa="II7/"+ ID_PLACA +"/Alexa"; //Publicar
  topic_Spotify="II7/"+ ID_PLACA +"/Spotify"; //Publicar
  

  // Nos conectamos al WiFi
  conecta_wifi();
   // Inicializamos conexión MQTT
  mqtt_client.setServer(mqttServer.c_str(), 1883);
  mqtt_client.setBufferSize(512); // para poder enviar mensajes de hasta X bytes

  conecta_mqtt();
    
  if (mpu.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  
  Serial.println("MPU6050 Found!");

  mpu.CalibrateGyro(6);
  
  delay(100);

  tiempo_prev=millis();
  Serial.println("Fin setup");
}
//-------------------------------------------------------
//                      LOOP
//-------------------------------------------------------
void loop() {
  loop2(); //SOLO EN MODO PRUEBA
  button.loop(); //Comprueba todo el tiempo el tipo de pulsación
}

//-------------------------------------------------------
//               FUNCIONES PULSACIONES
//-------------------------------------------------------
void singleClick(Button2& btn) {
    Serial.println("click\n");
    estado_stop=false;
    estado_play=true;
}
void longClickDetected(Button2& btn) {
    Serial.println("long click detected");
}
void longClick(Button2& btn) {
    Serial.println("long click\n");
    loop2();
}
void doubleClick(Button2& btn) {
    Serial.println("double click\n");
    estado_stop=true;
    estado_play=false;
}
void tripleClick(Button2& btn) {
    Serial.println("triple click\n");
}
//-------------------------------------------------------
//               FUNCION SPOTIFY
//-------------------------------------------------------
void altavoz(){
  StaticJsonDocument<300> jsonDoc;
  jsonDoc["Pause"] = estado_stop;
  jsonDoc["Play"] = estado_play;
  jsonDoc["Skip"] = skiptoprevious;
  jsonDoc["Next"] = skiptonext;

  // Serializar el objeto JSON a una cadena
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);

  // Publicar el JSON en el tema MQTT
  mqtt_client.publish(topic_Spotify.c_str(), jsonPayload.c_str());
  Serial.println("Topic   : "+ topic_Spotify);
  Serial.println("Payload : "+ jsonPayload);
}
//-------------------------------------------------------
//               PULSACIÓN LARGA
//-------------------------------------------------------
void loop2(){
  // Leer las aceleraciones y velocidades angulares
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  temp_raw = mpu.getTemperature();
  temperatura = float(temp_raw + 521)/340 + 35.0

  float ax_m_s2 = ax * (9.81/16384.0);
  float ay_m_s2 = ay * (9.81/16384.0);
  float az_m_s2 = az * (9.81/16384.0);
  float gx_deg_s = gx * (250.0/32768.0);
  float gy_deg_s = gy * (250.0/32768.0);
  float gz_deg_s = gz * (250.0/32768.0);

  //Mostrar las lecturas separadas por un [tab]
  // Serial.print("a[x y z](m/s2):\t");
  // Serial.print(ax_m_s2); Serial.print("\t");
  // Serial.print(ay_m_s2); Serial.print("\t");
  // Serial.print(az_m_s2);
  // Serial.print("\ng[x y z](deg/s):\t");
  // Serial.print(gx_deg_s); Serial.print("\t");
  // Serial.print(gy_deg_s); Serial.print("\t");
  // Serial.println(gz_deg_s);

  //Calcular los angulos de inclinacion:
  float accel_ang_x=atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_y=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_z=atan(az/sqrt(pow(ax,2) + pow(ay,2)))*(180.0/3.14);

  //Mostrar los angulos separadas por un [tab]
  // Serial.print("Inclinacion en X: ");
  // Serial.print(accel_ang_x); 
  // Serial.print("\nInclinacion en Y: ");
  // Serial.println(accel_ang_y);
  // Serial.print("\nInclinacion en Z: ");
  // Serial.println(accel_ang_z);

  dt = millis()-tiempo_prev;
  tiempo_prev=millis();
  
  girosc_ang_x = (gx/131)*dt/1000.0 + girosc_ang_x_prev;
  girosc_ang_y = (gy/131)*dt/1000.0 + girosc_ang_y_prev;
  girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;

  girosc_ang_x_prev=girosc_ang_x;
  girosc_ang_y_prev=girosc_ang_y;
  girosc_ang_z_prev=girosc_ang_z;

  float girz = fmod(girosc_ang_z,float(180));

  //Mostrar los angulos separadas por un [tab]
  Serial.print("Rotacion en X:  ");
  Serial.print(girosc_ang_x); 
  Serial.print("\tRotacion en Y: ");
  Serial.println(girosc_ang_y);
  Serial.print("Rotacion en mod: ");
  Serial.println(girz);
  
  // Si la rotación en el eje X supera el umbral establecido
  if (abs(accel_ang_x) >= umbral) {
    //Serial.println("¡Giro en el eje X detectado!");
    if(accel_ang_x>0){
      skiptoprevious = false;
      skiptonext = true;
    } else if (accel_ang<0){
      skiptoprevious = true;
      skiptonext = false;
    }    
  }
    if (abs(accel_ang_y) >= umbral) {
    //Serial.println("¡Giro en el eje Y detectado!");
    // Realiza alguna acción aquí (p. ej., encender un LED, enviar una señal, etc.)
      if(accel_ang_y<0){
        termostato=-1;
      }else if(accel_ang_y>0){
        termostato=1;
      }
  }
      // Si la rotación en el eje Z supera el umbral establecido
 if (abs(accel_ang_z) >= umbral) {
  //Serial.println("¡Giro en el eje Z detectado!");
    // Limita la intensidad a 0 y 255
    brillo = map(girz, 0,180, 255,0);
    brillo = constrain(brillo,0,255);
    analogWrite(pinled, brillo);
  }
  StaticJsonDocument<300> jsonDoc;
  jsonDoc["Lampara"] = brillo;
  jsonDoc["Temperatura"] = temperatura;
  jsonDoc["Termostato"] = termostato;

  // Serializar el objeto JSON a una cadena
  String jsonPayload;
  serializeJson(jsonDoc, jsonPayload);

  // Publicar el JSON en el tema MQTT
  mqtt_client.publish(topic_Alexa.c_str(), jsonPayload.c_str());
  Serial.println("Topic   : "+ topic_Alexa);
  Serial.println("Payload : "+ jsonPayload);

  Serial.print("\nVibration status: ");
  bool Sensor_State = digitalRead(Vibration_signal);
  if (Sensor_State == true) {
    Serial.println("Sensing vibration\n");
    delay(500);
  }
  else if(Sensor_State == false){
    Serial.println("No vibration\n");
    delay(500);
  }

  delay(100);
}
