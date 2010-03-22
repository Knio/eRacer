#ifndef CAR_BODY_H
#define CAR_BODY_H
#include "PhysicsObject.h"
#include "Core/Math.h"
#include "Logic/Track.h"

namespace Physics{
class CarBody : public PhysicsObject
{
public:
	CarBody(float mass = 0, const Point3& pos = ORIGIN, const Matrix& orient = IDENTITY);
	~CarBody();
  
  
  float SimWheel(
    int i, 
    const Point3 &localpos, 
    const Logic::Frame &frame, 
    float turning, 
    float enginespeed, 
    bool braking
  );
    
protected:
  Point3  wheelvel[4];
  bool    sliding[4];
  
};
}
#endif 