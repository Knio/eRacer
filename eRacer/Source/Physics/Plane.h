#ifndef PLANE_H
#define PLANE_H
#include "PhysicsObject.h"
namespace Physics{
class Plane : public PhysicsObject{
public:
	Plane(bool dynamic = true, float mass = 0);
	~Plane();

};
}
#endif 