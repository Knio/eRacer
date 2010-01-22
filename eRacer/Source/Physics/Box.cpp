#include "Box.h"
namespace Physics{
Box::Box(bool dynamic, float mass){
	PhysicsObject::dynamic = dynamic;
	PhysicsObject::mass = (dynamic ? mass : 0);

}
Box::~Box(){

}
}