#include <Arduino.h>
#include <Servo.h>
#include <avr/wdt.h>

// ---------------- motors ----------------
int stby = 3;
int ENA = 5; // enable A - control pin for left motors speed/on/off
int ENB = 6; // enable B - control pin for right motors speed/on/off
int INA = 7;
int INB = 8; // Input pins control current direction (and thus wheel direction)

// ---------------- servo and ultrasonc ----------------
Servo servo;
const int trig = 13; // ultrasonic trigger pin
int echo = 12;        // ultrasonic echo pin
int angles[7] = {0, 30, 60, 90, 120, 150, 180};

// Tracking variables for the sweep
int best_val = 0;
int best_index = 0;
int best_angle = 0;


void setup() {
  pinMode(stby, OUTPUT);
  digitalWrite(stby, HIGH);
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT); 
  
  analogWrite(ENA, 75); // set speed of left motors
  analogWrite(ENB, 75); // set speed of right motors
  
  servo.attach(10);
  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  
  Serial.begin(9600);
}

// -------- finding dist using the ultasonic --------

int get_distance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  float distance = pulseIn(echo, HIGH) / 58.0;
  Serial.println(distance);
  return (int)distance;
}

// ------------- movemnet funcitons ----------------

void stop() {
  digitalWrite(stby, 0);
}

void forward() {
  digitalWrite(stby, 1);
  digitalWrite(INA, 1);
  digitalWrite(INB, 1);
}

void turn_right(float theta) { 
  // turns right - calibrated in degrees
  digitalWrite(stby, 1);
  digitalWrite(INB, 1);
  digitalWrite(INA, 0);
  float omega = 112.5;
  float t = (theta / omega);
  delay(t * 1000);
  stop();
}

void turn_left(float theta) { 
  // turns left degrees
  digitalWrite(stby, 1);
  digitalWrite(INA, 1);
  digitalWrite(INB, 0);
  float omega = 112.5;
  float t = (theta / omega);
  delay(t * 1000);
  stop();
}


void loop() {
  servo.write(90);
  delay(500);
  int front_distance = get_distance();
  delay(500);
  
  if (front_distance < 45) {
    stop();
    delay(500);
    
    // Reset best value tracking before new scanning
    best_val = 0; 
    
    for (int i = 0; i < 7; i++) {
      servo.write(angles[i]);
      delay(400);
      int distance = get_distance();
      delay(400);

      if (distance > best_val) {
        best_val = distance;
        best_index = i;
        best_angle = angles[best_index];
      }
    }

    if (best_angle == 90) {
      stop();
    } else if (best_angle < 90) {
      turn_right(90 - best_angle);
    } else if (best_angle > 90) {
      turn_left(best_angle - 90);
    }
  } else {
    forward();
  }
}