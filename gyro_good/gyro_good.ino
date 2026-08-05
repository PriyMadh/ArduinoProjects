#include <arduino.h>
#include <Servo.h>
#include <Wire.h>          // 🔁 GYRO (I2C communication)
#include <MPU6050.h>      // 🔁 GYRO (library)

MPU6050 mpu;              // 🔁 GYRO object

// ---------------- MOTOR PINS ----------------
int stby = 3;
int ENA = 5;
int ENB = 6;
int INA = 7; 
int INB = 8;

float current_angle = 90;     // 🔁 GYRO: tracks angle
unsigned long last_time = 0;

void setup() {
  Serial.begin(9600);

  // GYRO 
  Wire.begin();
  mpu.initialize();

  

  last_time = millis();

 


}

float get_orientation(){
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  float gyro_z = ((gz)/131.0)-0.84;
  unsigned long current_time = millis();
  float dt = (current_time - last_time)/1000.0;
  current_angle = current_angle+(gyro_z*dt);
  last_time = current_time;
  if (current_angle > 359.5 && current_angle < 360.5){
    current_angle = 0;
  }
  Serial.println(current_angle);
  return current_angle;
}



void loop() {
  get_orientation();
 

}
