#include "Arduino.h"
#include "Servo.h"
#include "ServoManager.h"

const int SERVO_PIN = 9;
ServoManager* servoManager;
int i = 0;

void setup() {
  Serial.begin(9600);
  servoManager = new ServoManager(SERVO_PIN);
}

void loop() {
  servoManager->setVelocity(i++);
  servoManager->loop();
}
