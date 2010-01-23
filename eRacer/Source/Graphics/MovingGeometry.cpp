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

void MovingGeometry::setTransform(const  Matrix& transform){
	transform_ = transform;
	UpdateWorldBounds(modelBoundingVolume_, worldBoundingVolume_);
}


void MovingGeometry::SetMesh(const LPD3DXMESH mesh){
	//this method can only be called once
	assert(NULL == mesh_);
	assert(NULL != mesh);
	mesh_ = mesh;
	
	UpdateModelBounds(modelBoundingVolume_);
	UpdateWorldBounds(modelBoundingVolume_, worldBoundingVolume_);
}
};
