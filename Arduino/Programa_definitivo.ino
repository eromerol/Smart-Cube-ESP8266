// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Wire.h>
#include <MPU6050.h>
#include <I2Cdev.h>


MPU6050 mpu;

int Vibration_signal = 16;

int16_t ax, ay, az; // Valores del acelerometro
int16_t gx, gy, gz; // Valores del giroscopio
int16_t temperatura;

long tiempo_prev, dt;
float girosc_ang_x, girosc_ang_y, girosc_ang_z;
float girosc_ang_x_prev, girosc_ang_y_prev, girosc_ang_z_prev;

void setup(void) {
  Serial.begin(115200);

  // Try to initialize!
  mpu.initialize(); 
  Wire.begin();

  if (mpu.testConnection()) Serial.println("Sensor iniciado correctamente");
  else Serial.println("Error al iniciar el sensor");
  
  Serial.println("MPU6050 Found!");
  
  pinMode(Vibration_signal, INPUT); //Set pin as input

  mpu.CalibrateGyro(6);
  
  delay(100);

  tiempo_prev=millis();
}

void loop() {

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

  //Mostrar los angulos separadas por un [tab]
  Serial.print("\nRotacion en X:  ");
  Serial.print(girosc_ang_x); 
  Serial.print("\nRotacion en Y: ");
  Serial.println(girosc_ang_y);
  Serial.print("\nRotacion en Z: ");
  Serial.println(girosc_ang_z);


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

  delay(2000);
  
}