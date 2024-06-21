#ifndef SETUP_PINS_H
#define SETUP_PINS_H

#include <Arduino.h>

extern const int ch1Pin;
extern const int ch2Pin;
extern const int buttonPin[];
extern const int pump1;
extern const int pump2;
extern const int dcMotorPin1;
extern const int dcMotorPin2;
extern const int enablePin;

extern bool buttonPressed[];

//stepper
extern const int stepPin1;
extern const int stepPin2;
extern const int stepPin3;
extern const int stepPin4;
extern const int enablePinA;
extern const int enablePinB;



void setupPins();
#endif
