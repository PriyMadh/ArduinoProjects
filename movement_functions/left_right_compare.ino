#include <arduino.h>
#include <Servo.h>
#include <avr/wdt.h>
int stby = 3;
int ENA = 5; //enable A - the control pin to turn the left motors on/off
int ENB = 6; // same as ENA but for right side
int INA = 7; 
int INB = 8; // IN just refers to the input pins that correspnd with the left or right motor (really there are 4 motors, but the driver reads it as 2 motors) and the IN inputs tell wi=hich way the current should flow and based in the direction of the current, the motor (really motors) spin in that direction
Servo servo;
const int trig = 13; //ultrasonic - sends pulse outconst 
int echo = 12;// receives pulse




void stop(){
  digitalWrite(stby, 0);
}

void turn_right(float theta)
  { // turns right - calibrated in degrees 
  digitalWrite(stby, 1); 
  digitalWrite(INB, 1);
  digitalWrite(INA, 0); 
  float omega = 112.5; 
  float t = (theta/omega); 
  delay(t*1000); 
  stop();
  }
  void turn_left(float theta)
  { //turns left degrees 
  digitalWrite(stby, 1); 
  digitalWrite(INA, 1); 
  digitalWrite(INB, 0);
  float omega = 112.5; 
  float t = (theta/omega); 
  delay(t*1000); 
  stop();
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

void loop() {
  turn_right(90);
  delay(500);
  turn_left(90);
  delay(500);
}
