#ifndef DIBSE_H
#define DIBSE_H

#include "distance.h"

class DibsE {
  private:
    DistanceSensor sensor;

  public:
    DibsE(DistanceSensor sensor);
    void begin();
    void update();
};

#endif
