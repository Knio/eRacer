#include "Plane.h"
namespace Physics{
Plane::Plane(bool dynamic = true, float mass = 0, Vector3 normal=Y, float dist=0)
	PhysicsObject::dynamic = dynamic;
	PhysicsObject::mass = (dynamic ? mass : 0);


}
Plane::~Plane(){

}
}