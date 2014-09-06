#include "Arduino.h"
#include "ServoManager.h"
#include "Servo.h"

float currentVelocity = 0;
float targetVelocity = 0;
float currentAcceleration = 0;

ServoManager::ServoManager(int servoPin) {
  pin = servoPin;
  servo.attach(pin);
}

void ServoManager::loop() {
  // check to see if we need to modify our velocity
  if (currentVelocity != targetVelocity) {
    // if we have a set acceleration, use it to modify the velocity
    if (currentAcceleration != 0) {
    
    } else {
      currentVelocity = targetVelocity;
    }  
  }
  
  // move according to our current velocity
  servo.write(currentVelocity);
}

void ServoManager::setMaxVelocity(float velocity) {
  maxVelocity = velocity;
}

void ServoManager::setVelocity(float velocity) {
  targetVelocity = velocity;
}
    
void ServoManager::setMaxAcceleration(float acceleration) {
  maxAcceleration = acceleration;
}

void ServoManager::setAcceleration(float acceleration) {
  currentAcceleration = acceleration;
}

void ServoManager::accelerateToVelocity(float acceleration, float velocity) {
  currentAcceleration = acceleration;
  targetVelocity = velocity;
}
