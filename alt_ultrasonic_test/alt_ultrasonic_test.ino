#include <arduino.h>
#include <Servo.h>
#include <Wire.h>          //  GYRO (I2C communication)
#include <MPU6050.h>      //  GYRO (library)


const int trig = 13;
int echo = 12;

void setup() {
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 

  Serial.begin(9600);

}

void loop() {
  float start_time;
  float end_time;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long timeout = micros() + 30000;
  while (digitalRead(echo) == LOW){
    if (micros() > timeout){
      return;
    }
  } //this waits for the echo to go high (it keeps looping (doing nothing) until it goes hgih)
    start_time = micros();
  timeout = micros() + 30000;
  while (digitalRead(echo) == HIGH){
    if (micros() > timeout){
      return;
    }
  }
    end_time = micros();
  
  float duration = (end_time - start_time);
  float distance = duration * (0.0343/2);
  Serial.println(distance);




}
