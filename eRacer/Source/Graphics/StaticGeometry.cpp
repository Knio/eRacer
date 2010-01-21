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
	: Spatial(name),
	  transform_(transform),
	  mesh_(NULL)
{
}

StaticGeometry::~StaticGeometry(){
}

void StaticGeometry::cullRecursive(const Camera&, vector<const StaticGeometry*>& visibleNodes) const{
	visibleNodes.push_back(this);
}

void StaticGeometry::SetMesh(const LPD3DXMESH mesh){
	//this method can only be called once
	assert(NULL == mesh_);
	assert(NULL != mesh);
	mesh_ = mesh;

	unsigned int bytesPerVertex = mesh_->GetNumBytesPerVertex();
	unsigned int positionOffset = -1;

	D3DVERTEXELEMENT9 vertexElement[MAX_FVF_DECL_SIZE];
	mesh_->GetDeclaration(vertexElement);

	unsigned int i=0;
	while(i<MAX_FVF_DECL_SIZE && vertexElement[i].Stream != 0xFF){
		if(D3DDECLUSAGE_POSITION==vertexElement[i].Usage){
			positionOffset = vertexElement[i].Offset;
		}
		i++;
	}
	assert(positionOffset>=0);

	unsigned char* vertices;
		
	assert(SUCCEEDED(mesh_->LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*) &vertices)));


	Point3 min, max;
	Point3* position = (Point3*)(vertices+positionOffset);
	min.x = max.x = position->x;
	min.y = max.y = position->y;
	min.z = max.z = position->z;
    
	vertices+=bytesPerVertex;
    
	for (unsigned int i=0; i<mesh_->GetNumVertices(); i++) {
		position = (Point3*)vertices;
        if(position->x < min.x)
			min.x = position->x;
		else if(position->x > max.x)
			max.x = position->x;

		if(position->y < min.y)
			min.y = position->y;
		else if(position->y > max.y)
			max.y = position->y;

		if(position->z < min.z)
			min.z = position->z;
		else if(position->z > max.z)
			max.z = position->z;

        vertices+=bytesPerVertex;
    }

	mesh_->UnlockVertexBuffer();

	worldBoundingVolume_.set(transformAffine(transform_,min),transformAffine(transform_, max));
	
}


StaticGeometry::StaticGeometry(const string& name)
	: Spatial(name),
	  transform_(IDENTITY),
	  mesh_(NULL)
{
	
}

};
