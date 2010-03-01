#include "BoundingSphere.h"

namespace Graphics{

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