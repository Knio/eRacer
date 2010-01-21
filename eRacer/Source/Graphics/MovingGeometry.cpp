/**
 * @file MovingGeometry.cpp
 * @brief Implementation of the MovingGeometry class
 *
 * @date 13.01.2010
 * @author: Ole Rehmsen
 */

#include "MovingGeometry.h"
#include <cstdio>
namespace Graphics {

MovingGeometry::MovingGeometry(const string& name)
	: StaticGeometry(name)
{
	printf("%p\n", mesh_);
}
MovingGeometry::~MovingGeometry(){
	
}

void MovingGeometry::setTransform(const  Matrix& transform){
	transform_ = transform;
}

};
