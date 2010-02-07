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
	: Geometry(name)
{
}
MovingGeometry::~MovingGeometry(){
	
}

void MovingGeometry::SetTransform(const  Matrix& transform){
	transform_ = transform;
	if(NULL != mesh_)
		UpdateBounds();
}


void MovingGeometry::SetMesh(ID3DXMesh* mesh){
	//this method can only be called once
	assert(NULL == mesh_);
	assert(NULL != mesh);
	mesh_ = mesh;
	
	UpdateBounds();
}
};
