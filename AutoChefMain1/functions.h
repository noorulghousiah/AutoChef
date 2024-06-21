#ifndef FUNCTIONS_H
#define FUNCTIONS_H


void runSequenceA();
void runSequenceB();
void runSequenceC();
void executeCommonSteps();


void setHeating(int setting);
void controlDCMotor(int speed, bool run, bool forward);
void liquidRelay(int relayPin, int duration);
void shutAll();

void rotateStepperMotor(int angle);

void stepMotor(int step);
void rotate90Degrees();

#endif
