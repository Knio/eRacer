#ifndef CAPSULE_H
#define CAPSULE_H
#include "PhysicsObject.h"
namespace Physics{
class Capsule : public PhysicsObject
{
public:
	Capsule(bool dynamic = true, float mass = 0, const Point3& pos = ORIGIN, const Matrix& orient = IDENTITY, float height = 1.0, float radius = 0.1);
	~Capsule();

};
}
#endif 