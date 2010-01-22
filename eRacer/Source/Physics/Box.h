#ifndef BOX_H
#define BOX_H
#include "PhysicsObject.h"
namespace Physics{
class Box : public PhysicsObject
{
public:
	Box(bool dynamic = true, float mass = 0, Point3 pos = ORIGIN, Matrix orient = IDENTITY, Vector3 dimensions = Vector3(5, 5, 5));
	~Box();

};
}
#endif 