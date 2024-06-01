#include "dibse.h"

DistanceSensor sensor(9, 10); // Trigger pin 9, Echo pin 10
DibsE dibse(sensor);

void setup() {
  dibse.begin();
}

void loop() {
  dibse.update();
  delay(3000); // Update every 3 seconds
}
