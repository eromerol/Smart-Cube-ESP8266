// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Wire.h>
#include <MPU6050.h>
#include <I2Cdev.h>
#include <string>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "DHTesp.h"

// Dispositivos
MPU6050 mpu;
DHTesp dht;

// Cliente MQTT
WiFiClient wClient;
PubSubClient mqtt_client(wClient);
const String mqtt_server = "iot.ac.uma.es";
const String mqtt_user = "infind";
const String mqtt_pass = "zancudo";
const String ssid = "María's Galaxy A13";
const String password = "mariaguapa";

int Vibration_signal = 16;
int led = 2;
const uint8_t scl = D1;
const uint8_t sda = D2;

int16_t ax, ay, az; // Valores del acelerometro
int16_t gx, gy, gz; // Valores del giroscopio
int16_t temperatura;

long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y, girosc_ang_z;
float girosc_ang_x_prev, girosc_ang_y_prev, girosc_ang_z_prev;

float umbral_giro_x = 50.0; // Umbral para detección de giro en el eje X

String ID_PLACA, topic_pub, topic_sub;

//---------------------- WiFi -------------------------------
void conecta_wifi() {
  Serial.println("Connecting to " + ssid);
 
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("ssid:"+ssid);
  Serial.print("pw:"+password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".  ");
    Serial.print("ssid:"+ssid);
    Serial.print("pw:"+password);
  }
  Serial.println();
  Serial.println("WiFi connected, IP address: " + WiFi.localIP().toString());
}

//---------------------- MQTT -------------------------------
void conecta_mqtt() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (mqtt_client.connect(ID_PLACA.c_str(), mqtt_user.c_str(), mqtt_pass.c_str())) {
      Serial.println(" conectado a broker: " + mqtt_server);
    } else {
      Serial.println("ERROR:"+ String(mqtt_client.state()) +" reintento en 5s" );
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// -------------------- MENSAJE MQTT ---------------------
void procesa_mensaje(char* topic, byte* payload, unsigned int length) { 
  //String mensaje=String(std::string((char*) payload,length).c_str());
  //Serial.println("Mensaje recibido ["+ String(topic) +"] "+ mensaje);

  char *mensaje=(char *)malloc(length+1); // reservo memoria para copia del mensaje
  strncpy(mensaje,(char*)payload,length); // copio el mensaje en cadena de caracteres
  mensaje[length]='\0';

  // compruebo el topic
  if(String(topic)==topic_sub) 
  {
    if (mensaje[0] == '1') {
        digitalWrite(led, LOW);   // Turn the LED on (Note that LOW is the voltage level 
      } else {
        digitalWrite(led, HIGH);  // Turn the LED off by making the voltage HIGH
      }
  }
  else
  {
    Serial.println("Error: Topic desconocido");
  }
  
  free(mensaje); // libero memoria
}

//---------------------- SET UP -------------------------
void setup(void) 
{
  Serial.begin(115200);
  pinMode(led, OUTPUT);

  // Try to initialize!
  mpu.initialize();
  Wire.begin(sda,scl);
    
  if (mpu.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  
  Serial.println("MPU6050 Found!");
  
  conecta_wifi();

  ID_PLACA = "ESP" + String( ESP.getChipId() );
  topic_pub = "II7/" + ID_PLACA;// + ""; // Falta por rellenar
  topic_sub = "II7/" + ID_PLACA + "orden";

  mqtt_client.setServer(mqtt_server.c_str(), 1883);
  mqtt_client.setBufferSize(512); // para poder enviar mensajes de hasta X bytes
  mqtt_client.setCallback(procesa_mensaje);
  conecta_mqtt();

  dht.setup(5, DHTesp::DHT11); // Conectar el sensor DHT al GPIO 5
  pinMode(Vibration_signal, INPUT); //Set pin as input

  mpu.CalibrateGyro(6);
  
  delay(100);

  tiempo_prev=millis();
  Serial.println("Fin setup");
}

void loop() 
{
// Leer las aceleraciones y velocidades angulares
  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  float ax_m_s2 = ax * (9.81/16384.0);
  float ay_m_s2 = ay * (9.81/16384.0);
  float az_m_s2 = az * (9.81/16384.0);
  float gx_deg_s = gx * (250.0/32768.0);
  float gy_deg_s = gy * (250.0/32768.0);
  float gz_deg_s = gz * (250.0/32768.0);

  //Mostrar las lecturas separadas por un [tab]
  Serial.println();
  Serial.print("a[x y z](m/s2):\t");
  Serial.print(ax_m_s2); Serial.print("\t");
  Serial.print(ay_m_s2); Serial.print("\t");
  Serial.print(az_m_s2);
  Serial.print("\ng[x y z](deg/s):\t");
  Serial.print(gx_deg_s); Serial.print("\t");
  Serial.print(gy_deg_s); Serial.print("\t");
  Serial.println(gz_deg_s);

  //Calcular los angulos de inclinacion:
  float accel_ang_x=atan(ax/sqrt(pow(ay,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_y=atan(ay/sqrt(pow(ax,2) + pow(az,2)))*(180.0/3.14);
  float accel_ang_z=atan(az/sqrt(pow(ax,2) + pow(ay,2)))*(180.0/3.14);

  //Mostrar los angulos separadas por un [tab]
  Serial.println();
  Serial.print("Inclinacion en X: ");
  Serial.print(accel_ang_x); 
  Serial.print("\nInclinacion en Y: ");
  Serial.println(accel_ang_y);
  Serial.print("\nInclinacion en Z: ");
  Serial.println(accel_ang_z);

  dt = millis()-tiempo_prev;
  tiempo_prev=millis();
  
  girosc_ang_x = (gx/131)*dt/1000.0 + girosc_ang_x_prev;
  girosc_ang_y = (gy/131)*dt/1000.0 + girosc_ang_y_prev;
  girosc_ang_z = (gz/131)*dt/1000.0 + girosc_ang_z_prev;

  girosc_ang_x_prev=girosc_ang_x;
  girosc_ang_y_prev=girosc_ang_y;
  girosc_ang_z_prev=girosc_ang_z;

  float girz = fmod(girosc_ang_z,float(360));

  //Mostrar los angulos separadas por un [tab]
  Serial.println();
  Serial.print("\nRotacion en X:  ");
  Serial.print(girosc_ang_x); 
  Serial.print("\nRotacion en Y: ");
  Serial.println(girosc_ang_y);
  Serial.print("\nRotacion en mod: ");
  Serial.println(girz);
  int brillo = map(abs(girz), 360, 0, 0, 1024);
  analogWrite(led, brillo);

  // Si la rotación en el eje X supera el umbral establecido
  if (abs(accel_ang_x) >= umbral_giro_x) {
    Serial.println("¡Giro en el eje X detectado!");
    // Realiza alguna acción aquí (p. ej., encender un LED, enviar una señal, etc.)
  }
    if (abs(accel_ang_y) >= umbral_giro_x) {
    Serial.println("¡Giro en el eje Y detectado!");
    // Realiza alguna acción aquí (p. ej., encender un LED, enviar una señal, etc.)
  }
    if (abs(accel_ang_z) >= umbral_giro_x) {
    Serial.println("¡Giro en el eje Z detectado!");
    // Realiza alguna acción aquí (p. ej., encender un LED, enviar una señal, etc.)
  }

  // ------------ SENSOR DE VIBRACION -------------
  Serial.print("\nVibration status: ");
  bool Sensor_State = digitalRead(Vibration_signal);
  if (Sensor_State == true) {
    Serial.println("Sensing vibration\n");
    delay(1000);
  }
  else if(Sensor_State == false){
    Serial.println("No vibration\n");
    delay(1000);
  }

  // --------- PUBLICADOR MQTT ------------
  temperatura = dht.getTemperature();
  mqtt_client.publish(topic_pub.c_str(), String(temperatura).c_str());
  Serial.println();
  Serial.println("Topic   : "+ topic_pub);
  Serial.println("Payload : "+ String(temperatura));

  // --------- SUSCRIPTOR MQTT ------------
  mqtt_client.subscribe(topic_sub.c_str());

  delay(1000);  
}
