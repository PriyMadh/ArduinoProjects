#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>          
#include <MPU6050.h>       

MPU6050 mpu;               

// ---------------- motor ----------------
int stby = 3;
int ENA = 5;
int ENB = 6;
int INA = 7;
int INB = 8;

// ---------------- servo and ultrasonic ----------------
Servo servo;
const int trig = 13;
int echo = 12;

// Angles for scanning
int angles[7] = {0, 30, 60, 90, 120, 150, 180};

// ---------------- gyro vars ----------------
float current_heading = 90;     //  tracks angle (initialized to 90 degrees cause start facing head on)
unsigned long last_time = 0;
float filtered_gyro_z = 0;    // low pass filter vars
const float alpha = 0.2;      

// ---------------- setup ----------------
void setup() {
  pinMode(stby, OUTPUT);
  digitalWrite(stby, HIGH);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT); 

  analogWrite(ENA, 75);
  analogWrite(ENB, 75);

  servo.attach(10);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  Serial.begin(9600);

  // GYRO
  Wire.begin();
  mpu.initialize();

  last_time = millis();
}

// ------------ getting orientatin w/ respect to space ---------------

float get_orientation() {
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  
  //  conversion to degrees/sec minus hardcoded calibration offset
  float raw_gyro_z = ((gz) / 131.0) - 0.86;
  
  // Low Pass Filter 
  filtered_gyro_z = (alpha * raw_gyro_z) + ((1.0 - alpha) * filtered_gyro_z);
  
  unsigned long current_time = millis();
  float dt = (current_time - last_time) / 1000.0;
  last_time = current_time;
  
  // Integrtation with the filtered val
  current_heading = current_heading + (filtered_gyro_z * dt);
  
  // keep angle btwn 0 and 360
  if (current_heading >= 360.0) {
    current_heading -= 360.0;
  } else if (current_heading < 0.0) {
    current_heading += 360.0;
  }
  
  return current_angle;
}

// ---------------- dist to objects using ultrasonic ----------------------

int get_distance() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  
  long duration = pulseIn(echo, HIGH, 12000);
  get_orientation(); // Keep background orientation ticking
  
  if (duration == 0) {
    return 200; // Return max distance if timeout hit
  }
    
  float distance = duration * (0.0343 / 2);
  return (int)distance;
}

// ---------------- movements -------------------

void stop() {
  digitalWrite(stby, 0);
}

void forward() {
  digitalWrite(stby, 1);
  digitalWrite(INA, 1);
  digitalWrite(INB, 1);
}

// ------------------- using gyro reaings to determine how to turn --------------

void turn_gyro(float target_servo_angle) {
  get_orientation();
  
  // Body frame to Sapce Frame conversion: Current heading + (Relative Target - Forward offset)
  float target_heading = current_heading + (target_servo_angle - 90.0);
  
  //  wrap target heading between 0 and 360
  if (target_heading >= 360.0) target_heading -= 360.0;
  if (target_heading < 0.0) target_heading += 360.0;

  while (true) {
    get_orientation();
    
    // Calculate shortest  path error (-180 to 180) to stop long turns
    float error = target_heading - current_heading;
    if (error > 180.0) error -= 360.0;
    if (error < -180.0) error += 360.0;
    
    // If  within a 3 degree tolerance window, stop turning
    if (abs(error) <= 3.0) {
      break;
    }
    
    digitalWrite(stby, 1);
    if (error > 0) {
      // Turn Left
      digitalWrite(INA, 1);
      digitalWrite(INB, 0);
    } else {
      // Turn Right
      digitalWrite(INA, 0);
      digitalWrite(INB, 1);
    }
  }
  stop(); // stop motors after achieving target heaidng
}

void loop() {
  int score = 0;
  int best_score = -10000;
  int best_angle = 90;
  
  get_orientation();
  servo.write(90);
  
  unsigned long start = millis();
  while ((millis() - start) < 400) {
    get_orientation();
  }
  
  int front_distance = get_distance();
  start = millis();
  while ((millis() - start) < 400) {
    get_orientation();
  } 
  
  if (front_distance < 50) {
    stop();
    start = millis();
    while ((millis() - start) < 500) {
      get_orientation();
    }
    
    for (int i = 0; i < 7; i++) {
      servo.write(angles[i]);
      start = millis();
      while ((millis() - start) < 400) {
        get_orientation();
      }
      
      int dist = get_distance();
      start = millis();
      while ((millis() - start) < 400) {
        get_orientation();
      }
      
      score = dist; 
      if (score > best_score) {
        best_score = score;
        best_angle = angles[i];
      }
    }
    turn_gyro(best_angle);
  } else {
    forward();
  }
}