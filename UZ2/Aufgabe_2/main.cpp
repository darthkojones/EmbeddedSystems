#include "dibse.h"

DistanceSensor sensor(8, 4); // Trigger pin 8, Echo pin 4
DibsE dibse(sensor);

void setup() {
  dibse.begin();
}

void loop() {
  dibse.update();
  delay(3000); // Update every 3 seconds
}
