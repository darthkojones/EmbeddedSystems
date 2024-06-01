#ifndef DISTANCE_H
#define DISTANCE_H

class DistanceSensor {
  private:
    int trigPin;
    int echoPin;
    long duration;
    int distance;

  public:
    DistanceSensor(int trigPin, int echoPin);
    void begin();
    int getDistance();
};

#endif
