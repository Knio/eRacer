#ifndef BOX_H
#define BOX_H
#include "PhysicsObject.h"
namespace Physics{
class Box : public PhysicsObject
{
public:
	Box(bool dynamic = true, float mass = 0, const Point3& pos = ORIGIN, const Matrix& orient = IDENTITY, const Vector3& dimensions = Vector3(1, 1, 1));
	~Box();
	float RaycastDown(const Point3& susAttachPoint, Vector3& norm);

};
}
#endif 