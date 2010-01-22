#ifndef PLANE_H
#define PLANE_H

#include "PhysicsObject.h"

namespace Physics{

class Plane : public PhysicsObject{
public:
	Plane(bool dynamic = true, PhysicsLayer* pL = NULL, Point3 pos = ORIGIN, Matrix orient = IDENTITY, Vector3 dimensions = Vector3(5, 5, 5));
	~Plane();
};
}
#endif 