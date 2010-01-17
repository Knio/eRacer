/**
 * @file MovingGeometryNode.cpp
 * @brief Implementation of the MovingGeometryNode class
 *
 * @date 13.01.2010
 * @author: Ole Rehmsen
 */

#include "MovingGeometryNode.h"

namespace Graphics {

MovingGeometryNode::MovingGeometryNode(const string& name)
	: GeometryNode(name) 
{
	
}
MovingGeometryNode::~MovingGeometryNode(){
	
}

void MovingGeometryNode::setTransform(const  Matrix& transform){
	transform_ = transform;
}

};
