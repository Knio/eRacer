/**
 * @file StaticGeometry.cpp
 * @brief Implementation of the StaticGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "StaticGeometry.h"
#include "GraphicsLayer.h"

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

	Vertex* vertices;
		
	assert(SUCCEEDED(mesh_->LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*) &vertices)));

	Point3 min, max;
	min.x = max.x = vertices->x;
	min.y = max.y = vertices->y;
	min.z = max.z = vertices->z;

    for (unsigned int i=0; i<mesh_->GetNumVertices(); i++) {
        if(vertices->x < min.x)
			min.x = vertices->x;
		else if(vertices->x > max.x)
			max.x = vertices->x;

		if(vertices->y < min.y)
			min.y = vertices->y;
		else if(vertices->y > max.y)
			max.y = vertices->y;

		if(vertices->z < min.z)
			min.z = vertices->z;
		else if(vertices->z > max.z)
			max.z = vertices->z;

        vertices++;
    }

	mesh_->UnlockVertexBuffer();

	worldBoundingVolume_.set(transformAffine(transform_,min),transformAffine(transform_, max));
}


StaticGeometry::StaticGeometry(const string& name)
	: Spatial(name),
	  transform_(IDENTITY)
{
	
}

};
