#include <arduino.h>
#include <Servo.h>
#include <Wire.h>          // 🔁 GYRO (I2C communication)
#include <MPU6050.h> 
int stby = 3;
int ENA = 5;
int ENB = 6;
int INA = 7; 
int INB = 8;
MPU6050 mpu;

float gyro_z_error = 0;
float current_heading = 90;

long last_time = 0;

void setup() {

Serial.begin(9600);

pinMode(stby, OUTPUT); 
  digitalWrite(stby, HIGH); 

  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT); 
  pinMode(INA, OUTPUT); 
  pinMode(INB, OUTPUT);  

  analogWrite(ENA, 75); 
  analogWrite(ENB, 75); 

 Wire.begin();
  mpu.initialize();

  long sum = 0;
  float gyro_z_error = 0;
  int samples = 500;

  delay(5000);
  for (int n = 0; n < samples; n++){
    int16_t gx, gy, gz;
    mpu.getRotation(&gx, &gy, &gz);
    
    sum = sum + gz;
    delay(5);

  }

  gyro_z_error = ((float)sum/samples);
  Serial.println(gyro_z_error);

  last_time = millis();

}

void turn_right(){
  digitalWrite(stby, 1); 
  digitalWrite(INB, 1); 
  digitalWrite(INA, 0);
}

float get_orientation(){
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  

  // Convert to degrees/sec (approx)
  float gyroZ = (gz - 118) / 131.0;
  float current_time = millis();
  float dt = (current_time - last_time) / 1000.0;
  last_time = current_time;
  current_heading = current_heading + (gyroZ*dt);
  Serial.println(current_heading);
  return current_heading;



}

float get_rotation(){
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  float rotation_z = gz/131;
  Serial.println(rotation_z);
}

void loop() {
  
  turn_right();
  get_rotation();

}
