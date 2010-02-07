/**
 * @file StaticMeshNode.cpp
 * @brief Implementation of the StaticMeshNode class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "StaticMeshNode.h"
#include "GraphicsLayer.h"
#include "d3d9types.h"
#include <iostream>


using namespace std;

namespace Graphics {

StaticMeshNode::StaticMeshNode(const string& name, const Matrix& transform)
	: MeshNode(name)
{
	transform_ = transform;
}

StaticMeshNode::~StaticMeshNode(){
}

void StaticMeshNode::SetMesh(ID3DXMesh* mesh){
	//this method can only be called once
	assert(NULL == mesh_);
	assert(NULL != mesh);
	mesh_ = mesh;
	
	UpdateBounds();
}


};
