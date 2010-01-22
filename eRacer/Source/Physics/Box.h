#ifndef BOX_H
#define BOX_H
#include "PhysicsObject.h"
namespace Physics{
class Box : public PhysicsObject{
public:
	Box(bool dynamic = true, float mass = 0);
	~Box();

};
}
#endif 