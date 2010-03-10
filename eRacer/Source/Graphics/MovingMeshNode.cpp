/**
 * @file MovingMeshNode.cpp
 * @brief Implementation of the MovingMeshNode class
 *
 * @date 13.01.2010
 * @author: Ole Rehmsen
 */

#include "MovingMeshNode.h"
namespace Graphics {

MovingMeshNode::MovingMeshNode(const string& name, const Matrix& tx)
	: MeshNode(name, tx)
{
}
MovingMeshNode::~MovingMeshNode(){
	
}

void MovingMeshNode::SetTransform(const  Matrix& transform){
	transform_ = transform;
	if(initialized)	
		UpdateWorldBounds();
}


}
