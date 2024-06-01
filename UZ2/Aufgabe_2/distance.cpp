#include "distance.h"
#include <Arduino.h>

DistanceSensor::DistanceSensor(int tPin, int ePin) {
  trigPin = tPin;
  echoPin = ePin;
}

void DistanceSensor::begin() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

int DistanceSensor::getDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  
  return distance;
}
