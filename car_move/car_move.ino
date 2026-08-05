//#include "DeviceDriverSet_xxx0.h"
#include <arduino.h>
#include <Servo.h>
#include <avr/wdt.h>
int stby = 3;
int ENA = 5; //enable A - the control pin to turn the left motors on/off
int ENB = 6; // same as ENA but for right side
int INA = 7; 
int INB = 8; // IN just refers to the input pins that correspnd with the left or right motor (really there are 4 motors, but the driver reads it as 2 motors) and the IN inputs tell wi=hich way the current should flow and based in the direction of the current, the motor (really motors) spin in that direction
Servo servo;
const int trig = 12; //ultrasonic - sends pulse outconst 
int echo = 13;// receives pulse


int get_distance()
{ 
  digitalWrite(trig, 0);
  delayMicroseconds(2);
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);
  float distance = (pulseIn(echo, HIGH))/58;
  Serial.println(distance);
  return (int)distance;
}



void setup() { 
  pinMode(stby, OUTPUT); 
  digitalWrite(stby, HIGH); 
  pinMode(ENA, OUTPUT); 
  pinMode(ENB, OUTPUT); 
  pinMode(INA, OUTPUT); 
  pinMode(INB, OUTPUT);  
  analogWrite(ENA, 75); //set speed of motors 
  analogWrite(ENB, 75); 
  servo.attach(10); 
  pinMode(trig, OUTPUT); 
  pinMode(echo, INPUT); 
  Serial.begin(9600);}
  
  void stop()
  { 
    digitalWrite(ENA, 0); 
    digitalWrite(ENB, 0); 
    digitalWrite(stby,0);
    }
  void forward()
  { //goes forward - calibrated in meters
  digitalWrite(stby, 1);
  digitalWrite(INA, 1);
  digitalWrite(INB, 1);
  }
  void back(float d)
  { // goes back meters
  digitalWrite(stby, 1);
  digitalWrite(INA, LOW); 
  float t = d/0.275; delay(t*1000); 
  stop();
  }
  void turn_right(float theta)
  { // turns right - calibrated in degrees 
  digitalWrite(stby, 1); 
  digitalWrite(INB, 1); 
  float omega = 112.5; 
  float t = (theta/omega); 
  delay(t*1000); 
  stop();
  }
  void turn_left(float theta)
  { //turns left degrees 
  digitalWrite(stby, 1); 
  digitalWrite(INA, 1); 
  float omega = 112.5; 
  float t = (theta/omega); 
  delay(t*1000); 
  stop();
  }
 void loop()
 {
  servo.write(90);
  delay(500);
  float dist_90 = get_distance();
  if (dist_90 < 20)
  {
    stop();
    delay(500);
    servo.write(180);
    delay(500);
    int dist_180 = get_distance();
    delay(1000);
   
  
    servo.write(0);
    delay(250);
    servo.write(90);
    delay(100);
    int dist_0 = get_distance();
    delay(500);
    servo.write(90);
    delay(500);

    if ((dist_180 < 20) || (dist_0 < 20)){
      back(0.3);
    }

    else if (dist_180<dist_0){
      turn_right(90);
    }

    else if (dist_180>dist_0){
      turn_left(90);
    }

   else{
    forward();
   }
  }
 }
 
 
  



