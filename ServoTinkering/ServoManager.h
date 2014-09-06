#ifndef ServoManager_h
#define ServoManager_h

#include "Arduino.h"
#include "Servo.h"

/*
  A class that will provide an interface for a single Servo Motor
*/
class ServoManager {
  public:
    // Constructed with the pin leading to the Servo to be managed
    ServoManager(int servoPin);
    void setMaxVelocity(float velocity);
    void setVelocity(float velocity);
    void setMaxAcceleration(float acceleration);
    void setAcceleration(float acceleration);
    void accelerateToVelocity(float acceleration, float velocity);
    void loop();
  private:  
    Servo servo;
    int pin;
    float maxVelocity;
    float maxAcceleration;
    float currentVelocity;
    float targetVelocity;
    float currentAcceleration;
};

#endif
