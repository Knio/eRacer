/**
 * @file MovingGeometry.cpp
 * @brief Implementation of the MovingGeometry class
 *
 * @date 13.01.2010
 * @author: Ole Rehmsen
 */

#include "MovingGeometry.h"

namespace Graphics {

MovingGeometry::MovingGeometry(const string& name)
	: StaticGeometry(name) 
{
	
}
MovingGeometry::~MovingGeometry(){
	
}

void MovingGeometry::setTransform(const  Matrix& transform){
	transform_ = transform;
}

};
