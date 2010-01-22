#ifndef PLANE_H
#define PLANE_H
#include "PhysicsObject.h"
namespace Physics{
class Plane : public PhysicsObject{
public:
	Plane(bool dynamic = true, float mass = 0, Vector3 normal=Y, float dist=0);
	~Plane();

};
}
#endif 