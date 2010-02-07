/**
 * @file StaticGeometry.cpp
 * @brief Implementation of the StaticGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "StaticGeometry.h"
#include "GraphicsLayer.h"
#include "d3d9types.h"
#include <iostream>


using namespace std;

namespace Graphics {

StaticGeometry::StaticGeometry(const string& name, const Matrix& transform)
	: MeshNode(name)
{
	transform_ = transform;
}

StaticGeometry::~StaticGeometry(){
}

void StaticGeometry::SetMesh(ID3DXMesh* mesh){
	//this method can only be called once
	assert(NULL == mesh_);
	assert(NULL != mesh);
	mesh_ = mesh;
	
	UpdateBounds();
}


};
