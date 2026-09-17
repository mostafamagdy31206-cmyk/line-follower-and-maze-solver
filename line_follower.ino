#include <Arduino.h>

#define IR_L2 35
#define IR_L1 34
#define IR_R1 27
#define IR_R2 14

#define IN1 12
#define IN2 26
#define IN3 25
#define IN4 33

float Kp = 20;
float Ki = 0;
float Kd = 7;

float error = 0;
float previousError = 0;
float integral = 0;
float derivative = 0;

unsigned long lostStartTime = 0;


// ====================================================
// Motor Functions
// ====================================================

void setLeftMotor(int speed1, int speed2) {
  analogWrite(IN1, speed1);
  analogWrite(IN2, speed2);
}

void setRightMotor(int speed1, int speed2) { 
  analogWrite(IN3, speed1);
  analogWrite(IN4, speed2);
}

void stopCar() {
  setLeftMotor(0, 0);
  setRightMotor(0, 0);
}

void moveForward(int leftSpeed, int rightSpeed) {

  leftSpeed = constrain(leftSpeed, 0, 100);
  rightSpeed = constrain(rightSpeed, 0, 100);

  setLeftMotor(leftSpeed, 0);
  setRightMotor(rightSpeed, 0);
}


// ====================================================
// Pivot Functions
// ====================================================

void pivotLeft() {
  setLeftMotor(0, 30);
  setRightMotor(80, 0);
}

void pivotRight() {
  setLeftMotor(80, 0);
  setRightMotor(0, 30);
}


// ====================================================
// 180 Degree Turn
// ====================================================

void turn180() {

  // Left wheel backward
  setLeftMotor(0, 75);

  // Right wheel forward
  setRightMotor(75, 0);
}


// ====================================================
// Setup
// ====================================================

void setup() {

  Serial.begin(115200);

  pinMode(IR_L2, INPUT);
  pinMode(IR_L1, INPUT);
  pinMode(IR_R1, INPUT);
  pinMode(IR_R2, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopCar();

  delay(1000);
}


// ====================================================
// Loop
// ====================================================

void loop() {

  int L2 = digitalRead(IR_L2);
  int L1 = digitalRead(IR_L1);
  int R1 = digitalRead(IR_R1);
  int R2 = digitalRead(IR_R2);


  // ==================================================
  // ALL BLACK = FINISH
  // ==================================================

  if (L2 == 1 && L1 == 1 && R1 == 1 && R2 == 1) {

    stopCar();
    return;

  }


  // ==================================================
  // ALL WHITE = SEARCH FOR LINE
  // ==================================================

  if (L2 == 0 && L1 == 0 && R1 == 0 && R2 == 0) {

    if (lostStartTime == 0) {
      lostStartTime = millis();
    }


    // ----------------------------------------------
    // Line lost for less than 300 ms
    // ----------------------------------------------

    if (millis() - lostStartTime < 300) {

      if (previousError < 0) {

        setLeftMotor(0, 30);
        setRightMotor(45, 0);

      }

      else if (previousError > 0) {

        setLeftMotor(45, 0);
        setRightMotor(0, 30);

      }

      else {

        stopCar();

      }

      delay(5);
      return;
    }


    // ----------------------------------------------
    // Line lost for 300 ms = DEAD END
    // ----------------------------------------------

    turn180();

    delay(5);
    return;
  }


  // ==================================================
  // LINE FOUND AGAIN
  // ==================================================

  lostStartTime = 0;


  // ==================================================
  // LEFT PRIORITY
  // ==================================================

  if (L2 == 1 && R2 == 0) {

    pivotLeft();

    previousError = -3;

    delay(8);

    return;
  }


  // ==================================================
  // HARD RIGHT
  // ==================================================

  if (R2 == 1 && L2 == 0) {

    pivotRight();

    previousError = 3;

    delay(8);

    return;
  }


  // ==================================================
  // LEFT CURVE
  // ==================================================

  if (L2 == 1 && L1 == 1 && R1 == 0 && R2 == 0) {

    error = -2;
  }


  // ==================================================
  // RIGHT CURVE
  // ==================================================

  else if (L2 == 0 && L1 == 0 && R1 == 1 && R2 == 1) {

    error = 2;
  }


  // ==================================================
  // NORMAL PD
  // ==================================================

  else {

    int sum = 0;
    int count = 0;

    if (L2 == 1) {
      sum += -3;
      count++;
    }

    if (L1 == 1) {
      sum += -1;
      count++;
    }

    if (R1 == 1) {
      sum += 1;
      count++;
    }

    if (R2 == 1) {
      sum += 3;
      count++;
    }


    if (count > 0) {
      error = (float)sum / count;
    }
  }


  // ==================================================
  // PD CALCULATION
  // ==================================================

  integral += error;

  integral = constrain(integral, -15, 15);

  derivative = error - previousError;

  float correction =
    Kp * error +
    Ki * integral +
    Kd * derivative;


  // ==================================================
  // MOTOR SPEED
  // ==================================================

  int leftSpeed;
  int rightSpeed;


  if (L2 == 1 || R2 == 1) {

    leftSpeed = 35 + correction;
    rightSpeed = 30 - correction;

  }

  else {

    leftSpeed = 60 + correction;
    rightSpeed = 55 - correction;
  }


  leftSpeed = constrain(leftSpeed, 0, 100);
  rightSpeed = constrain(rightSpeed, 0, 100);


  moveForward(leftSpeed, rightSpeed);

  previousError = error;

 
}
