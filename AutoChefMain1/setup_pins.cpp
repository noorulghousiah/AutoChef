#include "setup_pins.h"


const int ch1Pin = A3;
const int ch2Pin = A2;
const int buttonPin[] = {9, 10, 11, 12, 13}; // Button pins
const int pump1 = A1;
const int pump2 = A0;
const int dcMotorPin1 = 7;
const int dcMotorPin2 = 8;
const int enablePin = 6;

bool buttonPressed[] = {false, false, false, false, false}; // Track button press state

//stepper
const int stepPin1 = 1; // IN1 on L298N
const int stepPin2 = 2; // IN2 on L298N
const int stepPin3 = 3; // IN3 on L298N
const int stepPin4 = 4; // IN4 on L298N
const int enablePinA = 0; // ENA on L298N
const int enablePinB = 5; // ENB on L298N



void setupPins() {
  
  pinMode(ch1Pin, OUTPUT);
  pinMode(ch2Pin, OUTPUT);
  
  for (int x = 0; x < 5; x++) {
    pinMode(buttonPin[x], INPUT_PULLUP);
  }
  
  pinMode(pump1, OUTPUT);
  pinMode(pump2, OUTPUT);
  pinMode(dcMotorPin1, OUTPUT);
  pinMode(dcMotorPin2, OUTPUT);
  pinMode(enablePin, OUTPUT);
  
  //stepper
  pinMode(stepPin1, OUTPUT);
  pinMode(stepPin2, OUTPUT);
  pinMode(stepPin3, OUTPUT);
  pinMode(stepPin4, OUTPUT);
  pinMode(enablePinA, OUTPUT);
  pinMode(enablePinB, OUTPUT);
  digitalWrite(enablePinA, LOW); // Ensure motor is disabled at start
  digitalWrite(enablePinB, LOW); // Ensure motor is disabled at start

}
