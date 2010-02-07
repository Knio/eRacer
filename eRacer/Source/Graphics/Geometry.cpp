/**
 * @file StaticGeometry.cpp
 * @brief Implementation of the StaticGeometry class
 *
 * @date 12.01.2010
 * @author: Ole Rehmsen
 */

#include "Geometry.h"
#include "GraphicsLayer.h"
#include "d3d9types.h"
#include <iostream>


using namespace std;

namespace Graphics {

Geometry::Geometry(const string& name)
	: Spatial(name),
	  transform_(IDENTITY)
{
	
}


Geometry::~Geometry(){
}

void Geometry::cullRecursive(const Camera&, vector<const Geometry*>& visibleNodes) const{
	visibleNodes.push_back(this);
}

void Geometry::Draw(IDirect3DDevice9* device) const{
	assert(NULL != device);

    //there need to be the same number of textures and materials
	assert(textures_.size()==materials_.size());
    // Meshes are divided into subsets, one for each material. Render them in a loop
    
    // set the transform
    // TODO unset it after!
    // is this even the right matrix?
	device->SetTransform(D3DTS_WORLDMATRIX(0), &transform_);
    
    for(unsigned int i = 0; i<materials_.size(); i++){
        device->SetMaterial( materials_[i]);
        device->SetTexture(0, textures_[i]);
        
        //make sure the mesh has been initialized at this point
        assert(NULL != mesh_);

        mesh_->DrawSubset(i);
    }
}


void Geometry::UpdateBounds(){
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
	Point3 position = transformedAffine(transform_,*(Point3*)(vertices+positionOffset));
	min.x = max.x = position.x;
	min.y = max.y = position.y;
	min.z = max.z = position.z;
    
	vertices+=bytesPerVertex;
    
	for (unsigned int i=0; i<mesh_->GetNumVertices(); i++) {
		position = transformedAffine(transform_,*(Point3*)vertices);
        if(position.x < min.x)
			min.x = position.x;
		else if(position.x > max.x)
			max.x = position.x;

		if(position.y < min.y)
			min.y = position.y;
		else if(position.y > max.y)
			max.y = position.y;

		if(position.z < min.z)
			min.z = position.z;
		else if(position.z > max.z)
			max.z = position.z;

        vertices+=bytesPerVertex;
    }

	mesh_->UnlockVertexBuffer();

	worldBoundingVolume_.set(min, max);
}



};
