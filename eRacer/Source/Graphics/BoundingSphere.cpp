#include "BoundingSphere.h"

#include <cassert>

namespace Graphics{

void BoundingSphere::recompute(ID3DXMesh& mesh){
	unsigned int positionOffset = -1;

	D3DVERTEXELEMENT9 vertexElement[MAX_FVF_DECL_SIZE];
	mesh.GetDeclaration(vertexElement);

	unsigned int i=0;
	while(i<MAX_FVF_DECL_SIZE && vertexElement[i].Stream != 0xFF){
		if(D3DDECLUSAGE_POSITION==vertexElement[i].Usage){
			positionOffset = vertexElement[i].Offset;
		}
		i++;
	}
	assert(positionOffset>=0);

	unsigned char* vertices;
		
	assert(SUCCEEDED(mesh.LockVertexBuffer(D3DLOCK_READONLY,(LPVOID*) &vertices)));

	recompute(vertices, mesh.GetNumVertices(), mesh.GetNumBytesPerVertex());	

	mesh.UnlockVertexBuffer();
}


void BoundingSphere::recompute(const unsigned char* vertices, unsigned int nVertices, unsigned int bytesPerVertex){
	unsigned int index = bytesPerVertex;
	Vector3 sum = *((Vector3*)vertices);

	for(unsigned int i = 1; i<nVertices; i++, index+=bytesPerVertex)
		sum += *(Point3*)(vertices+index);

	center = sum /= (float)nVertices;

	float radiusSquared = 0;

	for(unsigned int i = 0, index =0; i<nVertices; i++, index+=bytesPerVertex){
		Point3 v = *(Point3*)(vertices+index);
		float newRadiusSquared = D3DXVec3LengthSq(&(v-center));
		if(newRadiusSquared > radiusSquared)
			radiusSquared = newRadiusSquared;
	}
	radius = sqrt(radiusSquared);
}

void BoundingSphere::merge(const BoundingSphere& sphere){
	Vector3 centerDifference = sphere.center - center;
	float centerDistanceSquared = D3DXVec3LengthSq(&centerDifference);

	float radiusDistance = sphere.radius - radius;
	float radiusDistanceSquared = radiusDistance*radiusDistance;

	if(radiusDistanceSquared >= centerDistanceSquared){
		if(radiusDistance > 0.0f)
			*this = sphere;
	}
	else{
		float centerDistance = sqrt(centerDistanceSquared);
		float u = (centerDistance+radiusDistance)/(2*centerDistance);
		center+=u*centerDifference;
		radius = (centerDistance + radius + sphere.radius)/2;
	}
}

bool BoundingSphere::cull(const Plane& plane) const{
	return dot(plane.normal,center) - plane.distance < -radius;
}

}