#include "Plane.h"
namespace Physics{
Plane::Plane(bool dynamic, float mass){
	PhysicsObject::dynamic = dynamic;
	PhysicsObject::mass = (dynamic ? mass : 0);

}
Plane::~Plane(){

}
}