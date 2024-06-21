#include <Arduino.h>
#include "functions.h"
#include "setup_pins.h"

void runSequenceA() {
  // Add the implementation for Sequence A
}

//void runSequenceB(int row) {
//  unsigned long startTime = millis() / 1000;
//  
//  for (int i = 0; i < colCount; i++) {
//    // Calculate the delay based on the difference between the current time and the timestamp
//    unsigned long waitTime = timestamps[row][i] - (millis() / 1000 - startTime);
//    
//    if (waitTime > 0) {
//      delay(waitTime * 1000); // Wait for the required time in milliseconds
//    }
//
//    // Perform actions based on the userInput values
//    switch (userInput[row][i]) {
//      case 1:
//        liquidRelay(1, 5000);
//        break;
//      case 2:
//        liquidRelay(2, 5000);
//        break;
//      case 4:
//        controlDCMotor(200, true, true);
//        break;
//      case 5:
//        controlDCMotor(0, false, true);
//        break;
//      case 6:
//        setHeating(1);
//        break;
//      case 7:
//        setHeating(2);
//        break;
//      case 8:
//        setHeating(3);
//        break;
//      case 9:
//        controlDCMotor(0, false, true);
//        setHeating(3);
//        break;
//      default:
//        // Do nothing if the value is not recognized
//        break;
//    }
//  } 
//}


void runSequenceC() {
  setHeating(1);
  delay(10000);
  liquidRelay(pump1, 5000);
  rotateStepperMotor(90);

  controlDCMotor(200, true, true);
  liquidRelay(pump2, 5000);

  delay(20000);
  rotateStepperMotor(90);
  delay(50000);

  setHeating(2);
  rotateStepperMotor(90);
  delay(50000);

  controlDCMotor(0, false, true);
  setHeating(3);
}

void setHeating(int setting) {
  switch (setting) {
    case 1:
      digitalWrite(ch1Pin, HIGH);
      digitalWrite(ch2Pin, LOW);
      break;
    case 2:
      digitalWrite(ch1Pin, LOW);
      digitalWrite(ch2Pin, HIGH);
      break;
    case 3:
      digitalWrite(ch1Pin, LOW);
      digitalWrite(ch2Pin, LOW);
      break;
    default:
      // Invalid setting, turn off both relays as a safety measure
      digitalWrite(ch1Pin, LOW);
      digitalWrite(ch2Pin, LOW);
      break;
  }
}



void controlDCMotor(int speed, bool run, bool forward) {
  if (run) {
    analogWrite(enablePin, speed);  // Set motor speed (0-255)
    digitalWrite(dcMotorPin1, forward ? HIGH : LOW);   // Set direction
    digitalWrite(dcMotorPin2, forward ? LOW : HIGH);   // Set direction
  } else {
    analogWrite(enablePin, 0);      // Stop the motor
    digitalWrite(dcMotorPin1, LOW);
    digitalWrite(dcMotorPin2, LOW);
  }
}

void liquidRelay(int relayPin, int duration) {
  digitalWrite(relayPin, LOW);   // Turn on relay
  delay(duration);                // Wait for the specified duration
  digitalWrite(relayPin, HIGH);    // Turn off relay
}

void shutAll(){
  controlDCMotor(0, false, true);
  setHeating(3);
  }

// Number of steps for 90 degree rotation (adjust this value based on your motor's specifications)
const int stepsPer90Degrees = 50;
const int stepDelay = 10; // Delay between steps in milliseconds

void stepMotor(int step) {
  switch (step) {
    case 1:
      digitalWrite(stepPin1, HIGH);
      digitalWrite(stepPin2, LOW);
      digitalWrite(stepPin3, HIGH);
      digitalWrite(stepPin4, LOW);
      break;
    case 2:
      digitalWrite(stepPin1, LOW);
      digitalWrite(stepPin2, HIGH);
      digitalWrite(stepPin3, HIGH);
      digitalWrite(stepPin4, LOW);
      break;
    case 3:
      digitalWrite(stepPin1, LOW);
      digitalWrite(stepPin2, HIGH);
      digitalWrite(stepPin3, LOW);
      digitalWrite(stepPin4, HIGH);
      break;
    case 4:
      digitalWrite(stepPin1, HIGH);
      digitalWrite(stepPin2, LOW);
      digitalWrite(stepPin3, LOW);
      digitalWrite(stepPin4, HIGH);
      break;
  }
}

void rotate90Degrees() {
  digitalWrite(enablePinA, HIGH); // Enable motor driver A
  digitalWrite(enablePinB, HIGH); // Enable motor driver B
  delay(10); // Small delay to ensure the driver is enabled

  for (int i = 0; i < stepsPer90Degrees; i++) {
    stepMotor(1);
    delay(stepDelay);
    stepMotor(2);
    delay(stepDelay);
    stepMotor(3);
    delay(stepDelay);
    stepMotor(4);
    delay(stepDelay);
  }

  digitalWrite(enablePinA, LOW); // Disable motor driver A to prevent overheating
  digitalWrite(enablePinB, LOW); // Disable motor driver B to prevent overheating
}
