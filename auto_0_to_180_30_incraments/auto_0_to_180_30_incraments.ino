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
int angles[7] = {0, 30, 60, 90, 120, 150, 180};





  


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

void stop(){
  digitalWrite(stby, 0);
}

void forward(){
  digitalWrite(stby, 1);
  digitalWrite(INA, 1);
  digitalWrite(INB, 1);
}

int best_val = 0;
int best_index = 0;
int best_angle = 0;

void loop() {
  servo.write(90);
  delay(500);
  int front_distance = get_distance();
  delay(500);
  if (front_distance < 45){
    stop();
    delay(500);
    for (int i = 0; i < 7; i++){
      servo.write(angles[i]);
      delay(400);
      int distance = get_distance();
      delay(400);

    

      if (distance > best_val){
        best_val = distance;
        best_index = i;
        best_angle = angles[best_index];
      }
      }

      if (best_angle == 90){
        stop();
      }

      else if (best_angle < 90){
        turn_right(90-(best_angle));
      }

      else if (best_angle> 90){
        turn_left((best_angle) - 90);
      }

    
    
    
    
    


  }

  else{
    forward();
  }


}
