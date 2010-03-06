#ifndef CAR_BODY_H
#define CAR_BODY_H
#include "PhysicsObject.h"
#include "Core/Math.h"
namespace Physics{
class CarBody : public PhysicsObject
{
public:
	CarBody(bool dynamic = true, float mass = 0, const Point3& pos = ORIGIN, const Matrix& orient = IDENTITY);
	~CarBody();

};
}
#endif 